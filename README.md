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

Este projeto utiliza um sensor ultrassônico acoplado a um ESP32 para monitorar o nível de preenchimento de uma lixeira. O ESP32 mede a distância do topo até o conteúdo da lixeira, calcula o volume ocupado e exibe essa informação em uma tela OLED. Além disso, o projeto utiliza um módulo LoRa para enviar as leituras remotamente.

FUNCIONALIDADES

Medição de Distância: Usa um sensor ultrassônico para medir a profundidade da lixeira e determinar o volume preenchido.

Display OLED: Exibe a distância medida, o volume atual e o volume máximo registrado.

Comunicação LoRa: Envia informações de volume e distância remotamente via tecnologia LoRa.

Monitor Serial: Exibe as leituras no monitor serial para depuração.

COMO USAR

Inicialização: O ESP32 tentará conectar-se ao módulo LoRa e inicializar o display OLED.

Leitura de Dados: O sensor ultrassônico mede repetidamente a distância até o conteúdo da lixeira, e o ESP32 calcula o volume preenchido como uma porcentagem.

Exibição: O display OLED mostrará as leituras atuais e o volume máximo registrado.

Transmissão via LoRa: Os dados de distância e volume são enviados remotamente via LoRa.

