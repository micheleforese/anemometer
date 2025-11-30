# Use official Mosquitto image as base
FROM eclipse-mosquitto:2.0.22

# Copy custom configuration file into the container
COPY mosquitto.conf /mosquitto/config/mosquitto.conf

# Expose MQTT port
EXPOSE 1883

# Start Mosquitto
CMD ["mosquitto", "-c", "/mosquitto/config/mosquitto.conf"]
