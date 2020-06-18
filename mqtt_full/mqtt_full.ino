
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;
 

const char* mqttServer = "";
const int mqttPort = ;
const char* mqttUser = "";
const char* mqttPassword = "";
const char* ssid = "";
const char* password = "";

WiFiClient espClient;
PubSubClient client(espClient);
#include <OneWire.h>
#include <DallasTemperature.h>


#define ONE_WIRE_BUS D4
#define VccPin D6

OneWire oneWire(ONE_WIRE_BUS);


unsigned int ocitanje;
DallasTemperature sensors(&oneWire);
void setup() {

  pinMode(VccPin, OUTPUT);
  digitalWrite(VccPin, LOW);
  Serial.begin(115200);
  sensors.begin();
 unsigned status;
    status = bme.begin(0x76);

  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
 // client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client",mqttUser,mqttPassword)) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }

}

void loop() {
  
  
  digitalWrite(VccPin, HIGH);
delay(50);
ocitanje = analogRead(A0);
Serial.print("prazno");
Serial.print(ocitanje);
digitalWrite(VccPin, LOW);
if(ocitanje<1002){
ocitanje = map(ocitanje, 400, 1002, 100, 0);
ocitanje = constrain(ocitanje, 0, 100);
client.publish("s1/vltlo", String(ocitanje).c_str());
Serial.print(ocitanje);
Serial.println("%");
}else {
  client.publish("s1/vltlo", 0);
}
  
   sensors.requestTemperatures();
    client.publish("s1/tmpzrak", String(bme.readTemperature()).c_str());
  client.publish("s1/vlzrak", String(bme.readHumidity()).c_str());
 // client.publish("s1/vltlo", String(bme.readHumidity()).c_str());
   if(sensors.getTempCByIndex(0) != DEVICE_DISCONNECTED_C) 
  {
    Serial.print(sensors.getTempCByIndex(0));
    client.publish("s1/tmptlo", String(sensors.getTempCByIndex(0)).c_str());
  } 
 
  
  
  ESP.deepSleep(10000000,WAKE_RF_DISABLED);
  delay(100);
}
