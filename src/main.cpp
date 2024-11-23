/*********
  Developer: Joao Victor Fiorini de Souza
  This software is used to collect temperature and humidity data from the DHT11 sensor connected to a ESP8266.
  The data collected is sent to a MQTT broker using WiFi.
*********/



#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <string>

#define DHTPIN 0      // Pino digital D3 (GPTO0) do ESP8266
#define DHTTYPE DHT11 // Versão do sensor DHT

//Configurações da Rede WiFi
const char *networkSSID = "FIORINI";
const char *networkPwd = "0";

//Configurações do broker
const char *mqttBroker = "192.168.1.13";
const char *mqttTopic = "lan";
const int mqttPort = 1883;

DHT          dht(DHTPIN, DHTTYPE);
WiFiClient   espWiFi;
PubSubClient mqtt_client(espWiFi);

void connectToWifi();
void publishToMQTT();
boolean connectToMQTT();
boolean isConnectedToBroker;

const char *getTemperature();
const char *getHumidity();

void pubTemperature();
void pubHumidity();

void printSemsorData();
void welcome();

float temperature = 0;
float humidity = 0;

void setup()
{

  // Inicia a comunicação Serial
  Serial.begin(115200);
  delay(50);

  welcome();

  // Inicia a comunicação com o Sensor DHT11
  Serial.println("1) Iniciando o sensor DHT11");
  dht.begin();
  delay(1000);

  connectToWifi();

  isConnectedToBroker = connectToMQTT();
}

void loop()
{

  if (isConnectedToBroker)
  {
    pubTemperature();
    pubHumidity();
    Serial.println("Publicado nos tópicos univali/iot/<temperatura|umidade> com sucesso! :D");
  }
  else
  {
    Serial.println(" ): Não foi possível publicar nos tópicos pois o microcontrolador não está conectado ao broker MQTT.");
  }
}

void connectToWifi()
{
  // Iniciando conexão com a rede Wi-Fi
  WiFi.begin(networkSSID, networkPwd);

  Serial.printf("\n2) Conectando à rede WiFi %s", networkSSID);
  // Serial.print(networkSSID);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print("...");
  }

  Serial.println("\n3)Conectado à internet com sucesso! :)\n");
}

boolean connectToMQTT()
{

  // Define as configurações do servidor MQTT
  mqtt_client.setServer(mqttBroker, mqttPort);

  while (!mqtt_client.connected())
  {
    String client_id = "esp8266-client-" + String(WiFi.macAddress());

    Serial.println("4) Conectando ao broker MQTT...\n");

    if (mqtt_client.connect(client_id.c_str()))
    {
      Serial.println("5) Conectado ao broket MQTT com sucesso! :)\n\n");

      return true;
    }
    else
    {
      Serial.println("Falha ao se connectar ao broker MQTT");
      Serial.println(mqtt_client.state());
      Serial.println("Tentando novamente em 5 segundos...");
      delay(5000);

      return false;
    }
  }

  return false;
}


const char *getTemperature()
{
  delay(1000);
  temperature = dht.readTemperature();

  static std::string temperaturaStr;
  temperaturaStr = std::to_string(temperature);
  // const char * temperaturaChar = temperaturaStr.c_str();

  return temperaturaStr.c_str();
}

const char *getHumidity()
{
  delay(1000);
  humidity = dht.readHumidity();

  static std::string umidadeStr;
  umidadeStr = std::to_string(humidity);

  return umidadeStr.c_str();
}

void pubTemperature()
{
  mqttTopic = "univali/iot/temperatura";

  const char *response = getTemperature();

  mqtt_client.publish(mqttTopic, response);

  delay(5000);
}

void pubHumidity()
{
  mqttTopic = "univali/iot/umidade";

  const char *response = getHumidity();

  mqtt_client.publish(mqttTopic, response);

  delay(5000);
}

void printSemsorData()
{
  delay(1000);

  temperature = dht.readTemperature();
  delay(500);
  humidity = dht.readHumidity();

  Serial.println("======================");
  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.println("ºC");

  Serial.print("Umidade: ");
  Serial.print(humidity);
  Serial.println("%");

  Serial.println("======================");

  delay(500);
}

void welcome() {
  
    Serial.print("  _    _ _   _ _______      __     _      _____          \n");
    Serial.print(" | |  | | \\ | |_   _\\ \\    / /\\   | |    |_   _|         \n");
    Serial.print(" | |  | |  \\| | | |  \\ \\  / /  \\  | |      | |           \n");
    Serial.print(" | |  | | . ` | | |   \\ \\/ / /\\ \\ | |      | |           \n");
    Serial.print(" | |__| | |\\  |_| |_   \\  / ____ \\| |____ _| |_          \n");
    Serial.print("  \\____/|_| \\_|_____|   \\/_/    \\_\\______|_____|         \n");
    Serial.print("  _____ _   _ _______ ______ _____  _   _ ______ _______ \n");
    Serial.print(" |_   _| \\ | |__   __|  ____|  __ \\| \\ | |  ____|__   __|\n");
    Serial.print("   | | |  \\| |  | |  | |__  | |__) |  \\| | |__     | |   \n");
    Serial.print("   | | | . ` |  | |  |  __| |  _  /| . ` |  __|    | |   \n");
    Serial.print("  _| |_| |\\  |  | |  | |____| | \\ \\| |\\  | |____   | |   \n");
    Serial.print(" |_____|_| \\_|  |_|  |______|_|  \\_\\_| \\_|______|  |_|   \n");
    Serial.print("  _____           _____                                  \n");
    Serial.print(" |  __ \\   /\\    / ____|                                 \n");
    Serial.print(" | |  | | /  \\  | (___                                   \n");
    Serial.print(" | |  | |/ /\\ \\  \\___ \\                                  \n");
    Serial.print(" | |__| / ____ \\ ____) |                                 \n");
    Serial.print(" |_____/_/    \\_\\_____/                                  \n");
    Serial.print("   _____ ____ _____  _____          _____                \n");
    Serial.print("  / ____/ __ \\_   _|/ ____|  /\\    / ____|               \n");
    Serial.print(" | |   | |  | || | | (___   /  \\  | (___                 \n");
    Serial.print(" | |   | |  | || |  \\___ \\ / /\\ \\  \\___ \\               \n");
    Serial.print(" | |___| |__| || |_ ____) / ____ \\ ____) |               \n");
    Serial.print("  \\_____\\____/_____|_____/_/    \\_\\_____/               \n");
    Serial.print("                                                       \n");
    Serial.print("                                                       \n");
}
