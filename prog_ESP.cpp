//interface ESP32 com Telegram
#include  <Arduino.h>
#include  <WiFi.h>
#include  <WiFiClientSecure.h>
#include  <UniversalTelegramBot.h>

#define bot_token "token do seu bot"
#define bot_ID    "ID do seu chat"

WiFiClientSecure  clienteseguro;
UniversalTelegramBot  bot(bot_token,clienteseguro);

#define SSID  "NOME DA REDE"
#define SENHA "SENHA DA REDE"

#define sensor  14
#define LED     2

int botDelay = 1000;
unsigned long     ultimaVez;

void  trataNovasMsgs(int numNovasMsgs){
  Serial.println("Tratando mensagens");
  Serial.println(String(numNovasMsgs));
  for(int i=0;i<numNovasMsgs;i++){
    String chat_id = String(bot.messages[i].chat_id);
    String texto = bot.messages[i].text;
    Serial.println(texto);
    if(texto=="/ligaled")
      digitalWrite(LED,1);
    if(texto=="/desligaled")
      digitalWrite(LED,0);
  }
}

void  setup(){
  //GPIO
  pinMode(LED,OUTPUT);
  pinMode(sensor,INPUT);

  Serial.begin(9600);
  WiFi.begin(SSID,SENHA);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("Conectado a rede WiFi");
  Serial.println(WiFi.localIP());
  clienteseguro.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  bot.sendMessage(bot_ID,"Bot iniciado com sucesso","");
}

void  loop(){
  uint8_t dados_sensor = digitalRead(sensor);
  if(dados_sensor==0){
    Serial.println("Sensor detectou");
    bot.sendMessage(bot_ID,"Objeto detectado pelo sensor","");
    delay(3000);
  }

  if(millis() > ultimaVez+botDelay){
    int numNovaMsg = bot.getUpdates(bot.last_message_received + 1);
    while(numNovaMsg){
      trataNovasMsgs(numNovaMsg);
      numNovaMsg = bot.getUpdates(bot.last_message_received+1);
    }
    ultimaVez = millis();
  }
}