# Server Instructions

```shell
server --port /dev/ttyACM1 --mqtt-id "test" --mqtt-host localhost --mqtt-port 1883
```

## Cross Compiling

> cross comes with prebuilt Docker images containing the ARM toolchain, so you usually don't need to install gcc-arm-linux-gnueabihf or .cargo/config.toml

```shell
cargo install cross
cross build --release --target armv7-unknown-linux-gnueabihf
```

## Example commands

1. Install the utility

    ```shell
    sudo apt update
    sudo apt install mosquitto-clients
    ```

2. Try Those Commands

    ```shell
    mosquitto_pub -h localhost -p 1883 -t "anemometer" -m '{"x":0.0124,"y":-0.0421,"z":-0.0041,"timestamp":1763924107.7981,"x_kalman":-0.0054,"autocalibrazione_asse_x":true,"autocalibrazione_misura_x":false,"temp_sonica_x":20.2112,"y_kalman":-0.004,"autocalibrazione_asse_y":true,"autocalibrazione_misura_y":false,"temp_sonica_y":20.1449,"z_kalman":0.0,"autocalibrazione_asse_z":true,"autocalibrazione_misura_z":false,"temp_sonica_z":20.2773}'
    ```

    ```shell
    mosquitto_pub -h localhost -p 1883 -t "sps30" -m '{"timestamp":1762784698,"sensor_data":{"mass_density":{"pm1.0":7.512,"pm2.5":7.944,"pm4.0":7.944,"pm10":7.944},"particle_count":{"pm0.5":51.835,"pm1.0":59.757,"pm2.5":59.954,"pm4.0":59.968,"pm10":59.98},"particle_size":0.443,"mass_density_unit":"ug/m3","particle_count_unit":"#/cm3","particle_size_unit":"um"}}'
    ```
