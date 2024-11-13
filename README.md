# Trash Bin Volume Monitoring with Ultrasonic Sensor and LoRa Communication
This project uses an ultrasonic sensor attached to an ESP32 to monitor the filling level of a trash bin. The ESP32 measures the distance from the top to the trash content, calculates the occupied volume, and displays this information on an OLED screen. Additionally, the project uses a LoRa module to remotely send the readings.

FEATURES

Distance Measurement: Uses an ultrasonic sensor to measure the bin’s depth and determine the filled volume.

OLED Display: Shows the measured distance, current volume, and maximum recorded volume.

LoRa Communication: Sends volume and distance information remotely via LoRa technology.

Serial Monitor: Displays readings in the serial monitor for debugging.

HOW TO USE

Initialization: The ESP32 will attempt to connect to the LoRa module and initialize the OLED display.

Data Reading: The ultrasonic sensor repeatedly measures the distance to the trash content, and the ESP32 calculates the filled volume as a percentage.

Display: The OLED display will show the current readings and the maximum recorded volume.

LoRa Transmission: Distance and volume data are sent remotely via LoRa.

