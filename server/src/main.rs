use clap::Parser;
use rumqttc::{AsyncClient, Event, EventLoop, MqttOptions, Packet, QoS, SubscribeFilter};
use serde_json::json;
use std::sync::Arc;
use std::time::{Duration, Instant};
use tokio::sync::mpsc::{self, Sender};
use tokio::sync::{Mutex, watch};
use tokio::time::sleep;
use tokio_serial::SerialPort;

#[derive(Parser, Debug)]
#[command(author, version)]
struct Args {
    /// Communication Port
    #[arg(short, long, default_value_t = String::from("/dev/ttyACM1"))]
    port: String,

    /// Baud rate
    #[arg(short, long, default_value_t = 115200)]
    baud: u32,

    #[arg(long, default_value_t = String::from("server"),)]
    mqtt_id: String,

    #[arg(long, default_value_t = String::from("localhost"),)]
    mqtt_host: String,

    #[arg(long, default_value_t = 1883)]
    mqtt_port: u16,

    #[arg(long, default_value_t = 0)]
    filter_seconds: u64,

    #[arg(
        long,
        default_value_t = 5_000,
        help = "MQTT Re-connection delay in ms (milliseconds)."
    )]
    mqtt_reconnection_delay_ms: u64,

    #[arg(
        long,
        default_value_t = 5_000,
        help = "Serial Re-connection delay in ms (milliseconds)."
    )]
    serial_reconnection_delay_ms: u64,
}

fn match_topic(topic: &str) -> TopicType {
    match topic {
        "anemometer" => TopicType::Anemometer,
        "sps30" => TopicType::SPS30,
        "imu" => TopicType::Imu,
        "status" => TopicType::Status,
        _ => TopicType::Unknown,
    }
}

#[derive(PartialEq)]
enum TopicType {
    Anemometer,
    SPS30,
    Imu,
    Status,
    Unknown,
}

#[tokio::main(flavor = "current_thread")]
async fn main() {
    let args = Args::parse();

    println!("Serial JSON sender");
    println!("Port: {}", args.port);
    println!("Baud Rate: {}", args.baud);
    println!("MQTT id: {}", args.mqtt_id);
    println!("MQTT Host: {}", args.mqtt_host);
    println!("MQTT Port: {}", args.mqtt_port);
    println!("Filter Seconds: {}", args.filter_seconds);

    let mut mqttoptions = MqttOptions::new(args.mqtt_id, args.mqtt_host, args.mqtt_port);
    mqttoptions.set_keep_alive(Duration::from_secs(5));

    let (mqtt_watch_channel_tx, mqtt_watch_channel_rx) = watch::channel(false);
    let (mqtt_queue_channel_tx, mqtt_queue_channel_rx) = mpsc::channel::<String>(100);
    let mqtt_client = Arc::new(Mutex::new(None::<AsyncClient>));
    let mqtt_eventloop = Arc::new(Mutex::new(None::<EventLoop>));

    let (serial_watch_channel_tx, serial_watch_channel_rx) = watch::channel(false);
    let (serial_queue_channel_tx, serial_queue_channel_rx) = mpsc::channel::<String>(100);
    let serial_port = Arc::new(Mutex::new(None::<Box<dyn SerialPort>>));

    // MQTT reconnection task (only runs when disconnected)
    let mqtt_client_clone = mqtt_client.clone();
    let mqtt_eventloop_clone = mqtt_eventloop.clone();
    let mqtt_reconnect = tokio::spawn(async move {
        mqtt_reconnect_task(
            &mqttoptions,
            mqtt_client_clone,
            mqtt_eventloop_clone,
            mqtt_watch_channel_tx,
            mqtt_watch_channel_rx,
            Duration::from_millis(args.mqtt_reconnection_delay_ms),
        )
        .await;
    });

    // MQTT listener task (only runs when connected)
    let mqtt_eventloop_clone = mqtt_eventloop.clone();
    let mqtt_listener = tokio::spawn(async move {
        mqtt_listener_task(
            mqtt_eventloop_clone,
            &mqtt_queue_channel_tx,
            Duration::from_secs(args.filter_seconds),
        )
        .await;
    });

    // Serial port reconnection task (only runs when disconnected)
    let serial_port_clone = serial_port.clone();
    let serial_reconnect = tokio::spawn(async move {
        serial_reconnect_task(
            &args.port,
            args.baud,
            serial_port_clone,
            serial_watch_channel_tx,
            serial_watch_channel_rx,
            Duration::from_millis(args.serial_reconnection_delay_ms),
        )
        .await;
    });

    // Serial port listener task (only runs when connected)
    let serial_port_clone: Arc<Mutex<Option<Box<dyn SerialPort>>>> = serial_port.clone();
    let tx_clone = serial_queue_channel_tx.clone();
    let serial_listener = tokio::spawn(async move {
        serial_to_mqtt_channel_task(serial_port_clone, tx_clone).await;
    });

    let serial_port_clone: Arc<Mutex<Option<Box<dyn SerialPort>>>> = serial_port.clone();

    let mqtt_to_seial = tokio::spawn(async move {
        mqtt_to_serial_channel_task(mqtt_queue_channel_rx, serial_port_clone).await;
    });

    let mqtt_sender = tokio::spawn(async move {
        mqtt_sender_task(serial_queue_channel_rx, mqtt_client.clone()).await;
    });

    // Wait for all tasks
    let _ = tokio::join!(
        serial_reconnect,
        serial_listener,
        mqtt_reconnect,
        mqtt_listener,
        mqtt_to_seial,
        mqtt_sender,
    );
}

async fn mqtt_anemometer_topic_callback(json: &serde_json::Value, tx: &Sender<String>) {
    let mut json = json.clone();
    if let Some(obj) = json.as_object_mut() {
        obj.insert("topic".to_string(), json!("anm"));
        let pretty_json_string = serde_json::to_string_pretty(&json).unwrap();
        if let Err(e) = tx.send(pretty_json_string).await {
            eprintln!("Failed to send message: {e}");
        }
    }
}

async fn mqtt_sps30_topic_callback(json: &serde_json::Value, tx: &Sender<String>) {
    let mut json = json.clone();
    if let Some(obj) = json.as_object_mut() {
        obj.insert("topic".to_string(), json!("sps"));
        let pretty_json_string = serde_json::to_string_pretty(&json).unwrap();
        if let Err(e) = tx.send(pretty_json_string).await {
            eprintln!("Failed to send message: {e}");
        }
    }
}

async fn mqtt_imu_topic_callback(json: &serde_json::Value, tx: &Sender<String>) {
    let mut json = json.clone();
    if let Some(obj) = json.as_object_mut() {
        obj.insert("topic".to_string(), json!("imu"));
        let pretty_json_string = serde_json::to_string_pretty(&json).unwrap();
        if let Err(e) = tx.send(pretty_json_string).await {
            eprintln!("Failed to send message: {e}");
        }
    }
}

async fn mqtt_status_topic_callback(json: &serde_json::Value, tx: &Sender<String>) {
    let mut json = json.clone();
    if let Some(obj) = json.as_object_mut() {
        obj.insert("topic".to_string(), json!("status"));
        let pretty_json_string = serde_json::to_string_pretty(&json).unwrap();
        if let Err(e) = tx.send(pretty_json_string).await {
            eprintln!("Failed to send message: {e}");
        }
    }
}

async fn mqtt_to_serial_channel_task(
    mut rx: mpsc::Receiver<String>,
    port: Arc<Mutex<Option<Box<dyn SerialPort>>>>,
) {
    // Consumer loop: wait for messages from any worker
    while let Some(message) = rx.recv().await {
        println!("Writer sending: {message}");

        loop {
            while port.lock().await.is_none() {
                sleep(Duration::from_millis(100)).await;
            }

            loop {
                let mut port_guard = port.lock().await;

                if let Some(port) = port_guard.as_mut() {
                    if let Err(e) = port.write_all((message.clone() + "\n").as_bytes()) {
                        eprintln!("[ERROR] Failed to write: {}", e);
                    } else {
                        if let Err(e) = port.flush() {
                            eprintln!("[ERROR] Failed to flush: {}", e);
                        }
                        println!("\n[SEND] to port\n{}", message.clone());
                    }
                } else {
                    break;
                }
            }
        }
    }
}

async fn mqtt_sender_task(
    mut rx: mpsc::Receiver<String>,
    mqtt_client: Arc<Mutex<Option<AsyncClient>>>,
) {
    while let Some(json_str) = rx.recv().await {
        let topic = "command";

        loop {
            // Wait until we have a connection
            while mqtt_client.lock().await.is_none() {
                sleep(Duration::from_millis(100)).await;
            }

            let json_parsed = serde_json::from_str::<serde_json::Value>(&json_str);

            match json_parsed {
                Ok(json_value) => {
                    if let Some(command_value) = json_value.get("command") {
                        // Estraiamo la stringa interna senza virgolette
                        if let Some(command_str) = command_value.as_str() {
                            let mqtt_message = format!("{}", command_str);

                            let mqtt_client_guard = mqtt_client.lock().await;

                            if let Some(mqtt_client) = mqtt_client_guard.as_ref() {
                                if let Err(e) = mqtt_client
                                    .publish(topic, QoS::AtMostOnce, false, mqtt_message.clone())
                                    .await
                                {
                                    eprintln!("Failed to publish MQTT message: {e}");
                                } else {
                                    println!("Published to MQTT: {}", mqtt_message);
                                }
                            }
                        } else {
                            eprintln!("'command' is not a string in JSON: {}", json_str);
                            break;
                        }
                    } else {
                        eprintln!("JSON does not contain 'command' field: {}", json_str);
                        break;
                    }
                }
                Err(e) => {
                    eprintln!("Failed to parse JSON: {e}, input: {}", json_str);
                    break;
                }
            }
        }
    }
}

async fn serial_to_mqtt_channel_task(
    serial_port: Arc<Mutex<Option<Box<dyn SerialPort>>>>,
    tx: mpsc::Sender<String>,
) {
    let mut buffer = String::new();

    loop {
        // Wait until we have a connection
        while serial_port.lock().await.is_none() {
            sleep(Duration::from_millis(100)).await;
        }

        println!("📡 Serial listener task active - reading data...");

        loop {
            let mut port_guard = serial_port.lock().await;
            if let Some(port) = port_guard.as_mut() {
                let mut tmp_buf = [0u8; 1024 * 20];

                let result = { port.read(&mut tmp_buf) };

                // Release lock during read
                drop(port_guard);

                match result {
                    Ok(n) if n > 0 => {
                        buffer.push_str(&String::from_utf8_lossy(&tmp_buf[..n]));

                        while let Some(pos) = buffer.find('\n') {
                            let line = buffer.drain(..=pos).collect::<String>();
                            let line = line.trim();
                            if !line.is_empty() {
                                let json = serde_json::from_str::<serde_json::Value>(line);

                                match json {
                                    Ok(json_val) => {
                                        match tx.send(json_val.to_string()).await {
                                            Ok(_) => {}
                                            Err(e) => {
                                                println!("MQTT Queue Sending error: {:?}", e);
                                            }
                                        };
                                    }
                                    Err(_) => {
                                        eprintln!("ERROR Parse JSON from serial port.")
                                    }
                                }
                            }
                        }
                    }
                    Ok(_) => {
                        sleep(Duration::from_millis(100)).await;
                    }
                    Err(ref e) if e.kind() == std::io::ErrorKind::TimedOut => {
                        sleep(Duration::from_millis(100)).await;
                    }
                    Err(e) => {
                        eprintln!("Serial read error: {e}");
                        *serial_port.lock().await = None;
                        break;
                    }
                }
                tokio::time::sleep(Duration::from_millis(50)).await;
            } else {
                break;
            }
        }
    }
}

fn has_elapsed_between(
    old_timestamp: Instant,
    current_timestamp: Instant,
    duration: Duration,
) -> bool {
    let elapsed = current_timestamp.duration_since(old_timestamp);
    elapsed >= duration
}

async fn serial_reconnect_task(
    serial_option_port: &str,
    serial_option_baud_rate: u32,
    serial_port: Arc<Mutex<Option<Box<dyn SerialPort>>>>,
    tx: watch::Sender<bool>,
    mut rx: watch::Receiver<bool>,
    reconnection_delay: Duration,
) {
    loop {
        // Wait until disconnected
        while *rx.borrow_and_update() {
            if rx.changed().await.is_err() {
                return;
            }
        }

        println!("🔄 Serial reconnection task active - attempting to connect...");

        match open_serial_port(serial_option_port, serial_option_baud_rate) {
            Ok(port) => {
                println!("✓ Serial port connected: {}", serial_option_port);
                *serial_port.lock().await = Some(port);
                let _ = tx.send(true);
            }
            Err(e) => {
                println!(
                    "Failed to open serial port: {}. Retrying in {:?}...",
                    e, reconnection_delay
                );
                sleep(reconnection_delay).await;
            }
        }
    }
}

fn open_serial_port(
    serial_port: &str,
    baud_rate: u32,
) -> Result<Box<dyn SerialPort>, tokio_serial::Error> {
    tokio_serial::new(serial_port, baud_rate)
        .timeout(Duration::from_millis(100))
        .open()
}

async fn mqtt_reconnect_task(
    mqtt_options: &MqttOptions,
    mqtt_client: Arc<Mutex<Option<AsyncClient>>>,
    mqtt_eventloop: Arc<Mutex<Option<EventLoop>>>,
    tx: watch::Sender<bool>,
    mut rx: watch::Receiver<bool>,
    reconnection_delay: Duration,
) {
    loop {
        // Wait until disconnected
        while *rx.borrow_and_update() {
            if rx.changed().await.is_err() {
                return;
            }
        }

        println!("🔄 MQTT reconnection task active - attempting to connect...");

        match connect_mqtt(mqtt_options).await {
            Ok((client, eventloop)) => {
                println!("✓ MQTT connected");

                // Subscribe to topics
                if let Err(e) = client
                    .subscribe_many([
                        SubscribeFilter {
                            path: "anemometer".to_string(),
                            qos: QoS::AtMostOnce,
                        },
                        SubscribeFilter {
                            path: "sps30".to_string(),
                            qos: QoS::AtMostOnce,
                        },
                        SubscribeFilter {
                            path: "imu".to_string(),
                            qos: QoS::AtMostOnce,
                        },
                        SubscribeFilter {
                            path: "status".to_string(),
                            qos: QoS::AtMostOnce,
                        },
                    ])
                    .await
                {
                    println!("✗ Failed to subscribe: {}", e);
                    sleep(reconnection_delay).await;
                    continue;
                }

                *mqtt_client.lock().await = Some(client);
                *mqtt_eventloop.lock().await = Some(eventloop);
                let _ = tx.send(true);
            }
            Err(e) => {
                println!(
                    "✗ Failed to connect to MQTT: {}. Retrying in {:?}...",
                    e, reconnection_delay
                );
                sleep(reconnection_delay).await;
            }
        }
    }
}

async fn mqtt_listener_task(
    mqtt_eventloop: Arc<Mutex<Option<EventLoop>>>,
    tx: &Sender<String>,
    filter_duration: Duration,
) {
    let mut last_msg_anm_timestamp: Instant = Instant::now();

    loop {
        // Wait until we have a connection
        while mqtt_eventloop.lock().await.is_none() {
            sleep(Duration::from_millis(100)).await;
        }

        println!("📡 MQTT listener task active - polling events...");

        // Poll MQTT events
        loop {
            let result = {
                let mut eventloop_guard = mqtt_eventloop.lock().await;
                if let Some(eventloop) = eventloop_guard.as_mut() {
                    eventloop.poll().await
                } else {
                    break;
                }
            };

            match result {
                Ok(Event::Incoming(Packet::Publish(publish))) => {
                    let topic = publish.topic;
                    let payload = publish.payload;

                    let topic_type = match_topic(&topic);

                    if topic_type == TopicType::Unknown {
                        continue;
                    }

                    if topic_type == TopicType::Anemometer {
                        let current_time = Instant::now();
                        if !has_elapsed_between(
                            last_msg_anm_timestamp,
                            current_time,
                            filter_duration,
                        ) {
                            continue;
                        }

                        last_msg_anm_timestamp = current_time;
                    }

                    // Convert payload to string (if UTF-8)
                    if let Ok(text) = std::str::from_utf8(&payload) {
                        println!("📩 Message received:");
                        println!("   Topic: {topic}");
                        println!("   Payload: {text}");

                        match serde_json::from_str::<serde_json::Value>(text) {
                            Ok(json_val) => {
                                match topic_type {
                                    TopicType::Anemometer => {
                                        mqtt_anemometer_topic_callback(&json_val, &tx).await
                                    }
                                    TopicType::SPS30 => {
                                        mqtt_sps30_topic_callback(&json_val, &tx).await
                                    }
                                    TopicType::Imu => mqtt_imu_topic_callback(&json_val, &tx).await,
                                    TopicType::Status => {
                                        mqtt_status_topic_callback(&json_val, &tx).await
                                    }
                                    TopicType::Unknown => todo!(),
                                };
                            }
                            Err(_) => {
                                // not JSON
                            }
                        }
                    } else {
                        println!("Binary payload received on {topic}: {payload:?}");
                    }
                }
                Ok(Event::Incoming(i)) => {
                    println!("MQTT event: {:?}", i);
                }
                Ok(Event::Outgoing(_)) => {
                    // Outgoing packets
                }
                Err(e) => {
                    println!("MQTT connection error: {}. Connection lost.", e);
                    *mqtt_eventloop.lock().await = None;
                    break;
                }
            }
        }
    }
}

async fn connect_mqtt(
    mqtt_options: &MqttOptions,
) -> Result<(AsyncClient, EventLoop), rumqttc::ClientError> {
    let (client, eventloop) = AsyncClient::new(mqtt_options.clone(), 10);

    Ok((client, eventloop))
}
