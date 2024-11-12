#include <LoRa.h>
#include <SPI.h> //biblioteca que permite a troca de dados entre o sensor e o ESP32 (configura os pinos SCL, SCK... etc)
#include <Ultrasonic.h>
#include <Wire.h> //biblioteca que permite a comunicação I2C, entre o display OLED e o microcontrolador ESP32
#include <Adafruit_GFX.h> // biblioteca para o display
#include <Adafruit_SSD1306.h> // biblioteca para o display

#define TRIGGER_PIN 33
#define ECHO_PIN 32

#define OLED_ADDR 0x3c // Endereço I2C display

// Essas linhas definem a distância, em pixels, de cada linha em relação ao topo do display
#define OLED_LINE1 0
#define OLED_LINE3 20
#define OLED_LINE4 35
#define OLED_LINE6 50


// Configuração da resolução do display (esse ESP possui display 128x64)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

//Definições para comunicação com o rádio Lora presente no ESP
#define LORA_SCK 5
#define LORA_MISO 19
#define LORA_MOSI 27
#define LORA_RST 14
#define LORA_CS 18
#define HIGH_GAIN_LORA 20 // dB
#define BAND 915E6 

// Variáveis e objetos globais
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN); //Criado o objeto ultrasonic(hcsr04) da classe Ultrasonic

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, 16); //Criado o objeto do Display

const float profundidade_coletor = 18.45; //diz a máx profundidade fisica da lixeira em cm
int volume_max;
int volume_lido;
float distancia_lida;
int contador = 0;

#pragma pack(1) // Empacotamento da estrutura

typedef struct {
  float distancia_lida;
  int volume_lido;
  int volume_max;
} TDadosLora;

#pragma pack() // Restauração das configurações padrão de empacotamento


//Funções criadas
void registra_volume_max(int volume_lido);

void escreve_distancia_volume_display(float distancia_lida, int volume_lido);

void envia_medicoes_para_serial(float distancia_lida, int volume_lido);

void envia_dados_lora(float distancia_lida, int volume_lido);

bool init_comunicacao_lora(void); //inicia com o chip LoRa

/////////////////////////////////////////////////////////////////////////////////////////
void registra_volume_max(int volume_lido){

  if(volume_lido > volume_max){
    volume_max = volume_lido;
  }
}

void envia_medicoes_para_serial(float distancia_lida, int volume_lido){
  
  char mensagem[200]; //cria um vetor chamado mensagem capaz de armazenar até 200 caracteres
  char i;

  for(i=0; i<10; i++){
    Serial.println(" "); // imprime uma mensagem numa linha e passa pra próxima linha

    //constrói mensagens e as envia

    //Distancia Atual da lixeira
    memset(mensagem, 0, sizeof(mensagem)); // preenche o array "mensagem" com valor zero: o primeiro argumento é um ponteiro para a região da memória que será preenchida, o segundo é o valor atribuído a cada byte de memória e o terceiro argumento é o tamanho da região da memória em bytes.
    sprintf(mensagem, "Distância Atual: %.1f cm", distancia_lida);
    Serial.println(mensagem);

    //Volume atual da lixeira
    memset(mensagem, 0, sizeof(mensagem)); 
    sprintf(mensagem, "Volume Atual: %d %%", volume_lido);
    Serial.println(mensagem);

    //Volume máximo registrado da lixeira
    memset(mensagem, 0, sizeof(mensagem)); 
    sprintf(mensagem, "Volume máximo registrado: %d %%", volume_max);
    Serial.println(mensagem);

  }
}

void escreve_distancia_volume_display(float distancia_lida, int volume_lido){

  //
  char str_dist[10] = {0}; //inicializa um vetor do tipo char com capacidade para armazenar 10 elementos e todos os elementos são inicializados com valor zero.
  char str_vol[10] = {0};
  char str_vol_max[10] = {0};


  //formata para o display as strings de distância e volume
  sprintf(str_dist, "%.1f cm", distancia_lida);
  sprintf(str_vol, "%.1d %%", volume_lido);
  sprintf(str_vol_max, "%.1d %%", volume_max);

  display.clearDisplay();
  display.setCursor(0, OLED_LINE1);
  display.print("Enviando...");
  display.setCursor(0, OLED_LINE3);
  display.print("Distancia: ");
  display.println(str_dist);
  display.setCursor(0, OLED_LINE4);
  display.print("Volume: ");
  display.println(str_vol);
  display.setCursor(0, OLED_LINE6);
  display.print("Volume max: ");
  display.println(str_vol_max);
  display.display();

}

void envia_dados_lora(float distancia_lida, int volume_lido){ //A partir do pacote criado, essa função define as informações de uma forma que o radio Lora entenda o que tem que ser enviado por Lora

  TDadosLora dados_lora; //Cria uma variável chamada dados_lora
  dados_lora.distancia_lida = distancia_lida;
  dados_lora.volume_lido = volume_lido;
  dados_lora.volume_max = volume_max;
  LoRa.beginPacket();
  LoRa.write((unsigned char*) &dados_lora, sizeof(TDadosLora)); //
  LoRa.endPacket();
}

bool init_comunicacao_lora(void){
  
  bool status_init = false;
  Serial.println("[LoRa Sender] Tentando iniciar comunicacao com o radio LoRa..");
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
  LoRa.setPins(LORA_CS, LORA_RST, LORA_DEFAULT_DIO0_PIN);
  if (!LoRa.begin(BAND)){
    
    Serial.println("[LoRa Sender] Comunicacao com o radio LoRa falhou. Nova tentativa em 1 segundo..");
    delay(1000);
    status_init = false;
  }
  else{
    LoRa.setTxPower(HIGH_GAIN_LORA); //Configura o ganho do transmissor LoRa para 20dBm, o maior ganho possível, visando maior alcance possível
    Serial.println("[LoRa Sender] Comunicacao com o radio LoRa OK");
    status_init = true;
  }
  return status_init;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void setup(){

  Serial.begin(115200);

  //inicializa o volume máximo da lixeira
  volume_max = volume_lido;

  //inicializa display OLED
  Wire.begin(4, 15);
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)){
    Serial.println("Display OLED: falha ao inicializar");
  }
  else{
    Serial.println("Display OLED: INICIADO");
  

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
  }

  //Tenta até obter sucesso, comunicação com o chip LoRa
  while(init_comunicacao_lora() == false);

}

void loop(){

  distancia_lida = 0; // É zero porque o sensor não leu nada ainda

  for (int i = 0; i < 20; i++){ /* conta e soma dez vezes a distancia */
    ultrasonic.measure();
    distancia_lida += ultrasonic.get_cm();
    delay(700);
  }

  distancia_lida = distancia_lida / 20; /* media das distancias para uma exatidão nos resultados */

  volume_lido = 100 - (100 * (distancia_lida)/profundidade_coletor);

  if(volume_lido < 0){
    volume_lido = 0;
  }
  if(volume_lido >= 85){ /* Pois é o máximo de range que o sensor consegue ler nesta situação do projeto */
    volume_lido = 100;
  }

  
  registra_volume_max(volume_lido);

  envia_medicoes_para_serial(distancia_lida, volume_lido);

  escreve_distancia_volume_display(distancia_lida, volume_lido);

  envia_dados_lora(distancia_lida, volume_lido);

  delay(4000);
}


