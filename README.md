# IoT Waste Management System
This project is a waste management system based on the Internet of Things (IoT). It utilizes LoRa communication to transmit data between two devices: a Transmitter (sensor module) and a Receiver (data display and MQTT client). The system measures the fill level of a trash container using an ultrasonic sensor and displays the data on an OLED display. The data is also transmitted via LoRa and published to an MQTT broker for remote monitoring.

Components Used:

ESP32: Microcontroller for both transmitter and receiver

LoRa Module: For long-range communication between the transmitter and receiver

Ultrasonic Sensor: To measure the distance to the trash container’s contents

OLED Display: For local display of distance, volume, and maximum volume

MQTT: For remote data publishing and monitoring

Wi-Fi: For connecting to the MQTT broker

Transmitter Code:

The transmitter module collects distance measurements from an ultrasonic sensor, calculates the volume of waste in the container, and sends this data using the LoRa radio module.

Features:

Measures the distance to the trash container’s contents.

Calculates the volume based on the measured distance.

Sends the data via LoRa to the receiver.

Displays data on an OLED display (distance, volume, and maximum volume).

Monitors and stores the maximum volume reached in the container.

Libraries Used:

LoRa: Library for LoRa communication.

SPI: For SPI communication between the ESP32 and the LoRa module.

Ultrasonic: To interface with the ultrasonic sensor.

Adafruit SSD1306 & GFX: Libraries for displaying data on the OLED screen.

Wire: For I2C communication.

Receiver Code:

The receiver module receives data sent by the transmitter via LoRa. It then displays the data on an OLED display and publishes it to an MQTT broker for remote monitoring.


Features:

Receives distance and volume data via LoRa.

Displays the data on an OLED display.

Publishes distance and volume data to an MQTT broker for remote monitoring.

Uses Wi-Fi for MQTT communication.

Libraries Used:

LoRa: Library for LoRa communication.

SPI: For SPI communication between the ESP32 and the LoRa module.

Wi-Fi: For connecting to the Wi-Fi network.

EspMQTTClient: Library for MQTT communication.

Adafruit SSD1306 & GFX: Libraries for displaying data on the OLED screen.

Wire: For I2C communication.

How It Works:

Transmitter Module:

The transmitter uses an ultrasonic sensor to measure the distance to the contents of a trash container.

The distance is converted to volume and displayed on an OLED screen.

The data (distance, volume, and maximum volume) is sent to the receiver via LoRa.

Receiver Module:

The receiver listens for LoRa packets containing the data.

Once received, it displays the data on an OLED screen.

It then publishes the data to an MQTT broker for remote monitoring, allowing for real-time tracking of the trash container's fill status.

MQTT Integration:

MQTT Broker: The system uses the Tago.io MQTT broker for remote data publishing.

Topics:
Dados/Mqtt/distancia_lida: Distance data from the transmitter.
Dados/Mqtt/volume_lido: Volume data from the transmitter.
Dados/Mqtt/volume_maximo_registrado: Maximum volume data.

Setup and Configuration:

Transmitter:

Connect the ultrasonic sensor and LoRa module to the ESP32.
Initialize the LoRa communication and display in the setup() function.
The system continuously measures the distance, calculates the volume, and sends data via LoRa.

Receiver:

Connect the LoRa module and OLED display to the ESP32.
Configure the Wi-Fi and MQTT settings in the code (SSID, password, MQTT broker address, etc.).
The receiver listens for LoRa packets, displays the data, and sends it to the MQTT broker.

Conclusion:

This system enables efficient waste management by providing real-time monitoring of trash containers. By utilizing LoRa for long-range communication and MQTT for remote data access, the system offers a practical solution for smart waste monitoring.

