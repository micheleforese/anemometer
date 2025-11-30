# Example Mosquitto Docker Server

1. Navigate to this folder
2. Type:

    ```shell
    docker run -it -p 1883:1883 -v "$(pwd)/config:/mosquitto/config" eclipse-mosquitto:2.0.22
    docker build -t my-mosquitto .
    docker run -it -p 1883:1883 my-mosquitto
    ```
