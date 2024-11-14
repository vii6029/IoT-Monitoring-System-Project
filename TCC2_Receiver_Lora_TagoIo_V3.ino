#include <LoRa.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <EspMQTTClient.h>

// Definições para comunicação LoRa
#define LORA_SCK 5
#define LORA_MISO 19
#define LORA_MOSI 27
#define LORA_RST 14
#define LORA_CS 18
#define HIGH_GAIN_LORA 20 // dB
#define BAND 915E6

// Definições do OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3c
#define OLED_SDA 4
#define OLED_SCL 15
#define OLED_RST 16

//Definições das linhas do Display
#define OLED_LINE1 0
#define OLED_LINE3 20
#define OLED_LINE4 35
#define OLED_LINE6 50

EspMQTTClient Client(
  "HOLYVERS 1",                  //nome da rede Wi-Fi
  "48484848483235313332313137",  //nome da senha da rede Wi-Fi
  "mqtt.tago.io",                //nome do servidor Broker
  "esp32tcc",                    //Username do Token
  "d6f637bf-d6ef-4704-b512-a63962a0d53b", //código do Token
  "HVMO/TccEsp32",              //Nome do cliente MQTT
  1883                           //porta MQTT para se conectar ao Broker
);

// Definições Gerais
#define DEBUG_SERIAL_BAUDRATE 115200

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

#pragma pack(1)
typedef struct {
  float distancia_lida;
  int volume_lido;
  int volume_max;
} TDadosLora;
#pragma pack()

void display_init(void) {
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("[LoRa Receiver] Falha ao inicializar comunicação com o OLED");
  }
  else {
    Serial.println("[LoRa Receiver] Comunicação com OLED inicializada com sucesso");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
  }
}

bool init_comunicacao_lora(void) {
  bool status_init = false;
  Serial.println("[LoRa Receiver] Tentando iniciar comunicação com o rádio LoRa..");
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
  LoRa.setPins(LORA_CS, LORA_RST, LORA_DEFAULT_DIO0_PIN);
  if (!LoRa.begin(BAND)) {
    Serial.println("[LoRa Receiver] Comunicação com o rádio LoRa falhou. Nova tentativa em 1 segundo..");
    delay(1000);
    status_init = false;
  }
  else {
    LoRa.setTxPower(HIGH_GAIN_LORA);
    Serial.println("[LoRa Receiver] Comunicação com o rádio LoRa OK");
    status_init = true;
  }
  return status_init;
}

void onConnectionEstablished(){
  Serial.println("[MQTT] Connected to MQTT broker successfully.");
  // Adicione o código necessário aqui para ações após a conexão MQTT ser estabelecida
}

void setup() {
  Wire.begin(OLED_SDA, OLED_SCL);
  display_init();

  display.clearDisplay();
  display.setCursor(0, OLED_LINE1);
  display.print("Aguarde...");
  display.display();
  Serial.begin(DEBUG_SERIAL_BAUDRATE);
  while (!Serial);

  while (!init_comunicacao_lora());

  // Configurar mensagem de "última vontade" para a conexão MQTT
  Client.enableLastWillMessage("Dados/Mqtt/lastwill", "Disconnected", true);

  // Definir função de callback para conexão MQTT estabelecida
  Client.isConnected();

  // Aguardar conexão MQTT ser estabelecida
  while (!Client.isConnected()) {
    Client.loop();
  }
}

void loop() {
  char byte_recebido;
  int packet_size = 0;
  int lora_rssi = 0;
  TDadosLora dados_lora;
  char* ptInformacaoRecebida = NULL;

  packet_size = LoRa.parsePacket();
  if (packet_size == sizeof(TDadosLora)) {
    Serial.print("[LoRa Receiver] Há dados a serem lidos");

    ptInformacaoRecebida = (char*)malloc(packet_size);
    while (LoRa.available()) {
      byte_recebido = (char)LoRa.read();
      *ptInformacaoRecebida = byte_recebido;
      ptInformacaoRecebida++;
    }

    //Copie os dados recebidos para a estrutura TDadosLora
    byte dados[sizeof(TDadosLora)];
    memcpy(&dados_lora, ptInformacaoRecebida - packet_size, packet_size);

    lora_rssi = LoRa.packetRssi();
    display.clearDisplay();
    display.setCursor(0, OLED_LINE1);
    display.print("Recebendo...");
    display.setCursor(0, OLED_LINE3);
    display.print("Distancia:  ");
    display.print(dados_lora.distancia_lida);
    display.println(" cm");
    display.setCursor(0, OLED_LINE4);
    display.print("Volume:  ");
    display.print(dados_lora.volume_lido);
    display.println(" %");
    display.setCursor(0, OLED_LINE6);
    display.print("Volume max:  ");
    display.print(dados_lora.volume_max);
    display.print(" %");
    display.display();


    // Publicar distância no tópico MQTT
    String topicDistancia = "Dados/Mqtt/distancia_lida";
    String payloadDistancia = "distancia_lida: " + String(dados_lora.distancia_lida) + " cm";
    if (Client.isConnected()) {
      Client.publish(topicDistancia, payloadDistancia);
    } 

    // Publicar volume no tópico MQTT
    String topicVolume = "Dados/Mqtt/volume_lido";
    String payloadVolume = "volume_lido: " + String(dados_lora.volume_lido) + " %";
    if (Client.isConnected()) {
      Client.publish(topicVolume, payloadVolume);
    }

    // Publicar volume máximo no tópico MQTT
    String topicVolumeMax = "Dados/Mqtt/volume_maximo_registrado";
    String payloadVolumeMax = "volume_maximo: " + String(dados_lora.volume_max) + " %";
    if (Client.isConnected()) {
      Client.publish(topicVolumeMax, payloadVolumeMax);
    }
  } 

  // Processar comunicação MQTT
  Client.loop();
  delay(2000);
}
