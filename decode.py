import paho.mqtt.client as mqtt
import struct
import base64

# MQTT settings
MQTT_BROKER = "192.168.1.105"  # Replace with your MQTT broker's IP
MQTT_PORT = 1883               # Replace with your MQTT broker's port
MQTT_TOPIC = "esp32/test"      # Replace with your MQTT topic

# Decoding function
def decode_uplink(input_bytes):
    first_byte = input_bytes[0]
    battery_voltage = (first_byte / 254) * 1.2 + 3.0
    battery_voltage_number = round(battery_voltage, 2)

    hdc1080_temperature = struct.unpack_from('<f', input_bytes, 1)[0]
    hdc1080_humidity = struct.unpack_from('<f', input_bytes, 5)[0]
    bmp280_internal_temperature = struct.unpack_from('<f', input_bytes, 9)[0]
    bmp280_pressure = struct.unpack_from('<f', input_bytes, 13)[0]

    return {
        'battery_voltage': battery_voltage_number,
        'hdc1080_temperature': hdc1080_temperature,
        'hdc1080_humidity': hdc1080_humidity,
        'bmp280_internal_temperature': bmp280_internal_temperature,
        'bmp280_pressure': bmp280_pressure,
    }

# MQTT Client callbacks
def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))
    client.subscribe(MQTT_TOPIC)

def on_message(client, userdata, msg):
    print(f"Message received on topic {msg.topic}:: {msg.payload}")
    try:

        # Decode the Base64 string to a byte array
        decoded_base64 = base64.b64decode(msg.payload)
        print("Decoded Base64 to bytes:", decoded_base64)

        # Pass the byte array to the decode_uplink function
        decoded_data = decode_uplink(decoded_base64)
        print("Decoded Data:", decoded_data)

        #input_bytes = bytearray(msg.payload)
        #decoded_data = decode_uplink(input_bytes)
        #print("Decoded Data:", decoded_data)
    except Exception as e:
        print("Error in decoding the message:", e)

# MQTT Client setup
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect(MQTT_BROKER, MQTT_PORT, 60)
client.loop_forever()