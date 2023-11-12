# SCT6201H PCB Board 
for SiliQ SCT6201H wifi demo with MQTT

## Overview
This project involves the SCT6201H PCB board, focusing on using Wi-Fi to connect to an MQTT server and transmit sensor data. The data is sent in Base64 encoded format. A Python script is also provided to decode this data at the subscriber's end.

## Features
- Connects to Wi-Fi.
- Fetches sensor data and sends it to an MQTT server in Base64 encoded format.
- Python script for decoding the received data on the subscriber side.

## Hardware Requirements
- SCT6201H PCB Board
- Sensors compatible with the board

## Software Requirements
- Arduino IDE for programming the SCT6201H board.
- Python 3 for running the decoding script.
- Paho MQTT Python client.

## Arduino Code Overview
The provided Arduino code performs the following functions:
- Connects to a specified Wi-Fi network.
- Fetches sensor data.
- Encodes the data using Base64.
- Publishes the data to an MQTT topic.
- Manages deep sleep for power efficiency.

The code snippet includes methods for connecting to Wi-Fi, handling MQTT connections, reading sensor data, encoding it, and publishing to an MQTT server.

### Key Functions
- `setup_wifi()`: Establishes a Wi-Fi connection.
- `reconnect()`: Handles reconnection to the MQTT server if the connection is lost.
- `prepareTxFrame()`: Prepares and encodes the sensor data.
- `getBatVolt()` and `GetBatteryLevel()`: Functions for reading and calculating the battery level.
- `print_wakeup_reason()`: Prints the reason for waking up from deep sleep (useful for debugging).

### Setup and Loop
- `setup()`: Initializes serial communication, sensor fetching, Wi-Fi, and MQTT settings. It also configures deep sleep settings.
- `loop()`: Manages MQTT connection, sends data, and then puts the board into deep sleep.

## MQTT and Wi-Fi Configuration
Replace the following placeholders with your actual Wi-Fi and MQTT server details:
- `ssid`: Your Wi-Fi network SSID.
- `password`: Your Wi-Fi network password.
- `mqtt_server`: IP address of your MQTT broker.

## Python Decoding Script
Refer to the separate `README.md` in the Python script directory for details on running the Python decoding script.

## Installation and Running
1. Flash the Arduino code onto the SCT6201H board using the Arduino IDE.
2. Set up the MQTT broker and ensure it's running.
3. Run the Python script on a device in the same network as the MQTT broker.

## Contributing
Contributions to this project are welcome. Please ensure to follow the existing code style and add comments for clarity.

## License
[Specify the license here, if applicable]

## Contact
For any queries or contributions, feel free to contact [Your Name/Contact Information].

---
