/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/vs-code-platformio-ide-esp32-esp8266-arduino/
*********/

#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#define DHTPIN 0 //Pino digital D3 (GPTO0)
#define DHTTYPE DHT11
// #define LED 2;

const char* networkSSID = "FIORINI";
const char* networkPwd = "Fiorini09";

const char* mqttBroker = "localhost";
const char* mqttTopic = "teste";
const int mqttPort = 1883;

WiFiClient esp;
PubSubClient mqtt_client(esp);


DHT dht(DHTPIN, DHTTYPE);

float temperatura = 0;
float umidade = 0;

void connectToWifi();
void getTemperature();
void connectToMQTT();
void pubToMQTT();

void setup()
{
  
  //Iniciando a comunicação Serial
  Serial.begin(115200);
  delay(50);

  //Iniciando a comunicação com o Sensor DHT11
  Serial.println("Inicializando sensor DHT11");
  dht.begin();
  delay(1000);


  connectToWifi();
  // pinMode(LED, OUTPUT);
  
}



void loop()
{
  getTemperature();
}

void connectToWifi() {
  //Iniciando conexão com a rede Wi-Fi
  WiFi.begin(networkSSID, networkPwd);
  Serial.print("\nConectando à rede WiFi ");
  Serial.print(networkSSID);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("...");
  }

  Serial.println("\nConectado com sucesso!");
}


void getTemperature() {
  delay(1000);
  
  temperatura = dht.readTemperature();
  delay(500);
  umidade = dht.readHumidity();

  Serial.println("======================");
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println("ºC");

  Serial.print("Umidade: ");
  Serial.print(umidade);
  Serial.println("%");

  Serial.println("======================");
  
  delay(500);
}



// void blinkLed() {
// #define LED 2
// 
// void setup() {
//   // put your setup code here, to run once:
//   Serial.begin(115200);
//   pinMode(LED, OUTPUT);
// }
// 
// void loop() {
//   // put your main code here, to run repeatedly:
//   digitalWrite(LED, HIGH);
//   Serial.println("LED is on");
//   delay(1000);
//   digitalWrite(LED, LOW);
//   Serial.println("LED is off");
//   delay(1000);
// }
// }