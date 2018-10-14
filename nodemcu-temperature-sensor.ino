#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "DHT.h"

#define DHTPIN 4 // = D2 on the NodeMCU
#define DHTTYPE DHT22

const char* ssid = "SSID";
const char* password = "PASSWORD";

const char* host = "smarthomereceiversappservice.azurewebsites.net";
const char* uri = "/api/TemperatureHumidity";
const int httpsPort = 443;

const int sleepInSeconds = 20;
const char* fingerprint = "3ab0b1c27f746fd90c34f0d6a960cf73a4229de8";

DHT dht(DHTPIN, DHTTYPE);

void goToSleep() {
  Serial.printf("Going into deep sleep for %d seconds\n", sleepInSeconds);
  ESP.deepSleep(sleepInSeconds * 1e6); 
  Serial.println("------------------------------");
}

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(2000);
  while (!Serial) {}

  String macAddress = WiFi.macAddress();
  Serial.println("------------------------------");
  Serial.print("\nTemperature/humidity sensor started");
  Serial.print("MAC address: ");
  Serial.println(macAddress);


  // === READ SENSOR ===
  Serial.println("------------------------------");
  Serial.println("Reading AM2320 temperature/humidity sensor...");
  delay(2000);
  int i = 1;
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  while (isnan(t) || isnan(t)) {
    Serial.printf("Failed to read temperature/humidity from sensor (%d/10)\n", i);
    if (i < 10) {
      delay(1000);
      i++;
      t = dht.readTemperature();
      h = dht.readHumidity();
    } else {
      Serial.println("Give up sensor reading and go to deep sleep");      
      goToSleep();
      return;
    }
  }
  Serial.println("Reading completed succsssfully.");
  

  // === CREATE JSON STRING ===
  StaticJsonBuffer<200> jsonBuffer; 
  JsonObject& root = jsonBuffer.createObject(); 
  root["sensorId"] = macAddress; 
  root["temperature"] = t; 
  root["humidity"] = h; 
  String jsonStr;
  root.printTo(jsonStr);
  Serial.println(jsonStr);


  // === CONNECT TO WIFI ===
  Serial.printf("Connecting to %s ", ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  int j = 1;
  while (WiFi.status() != WL_CONNECTED)
  {
    if (j < 60) {
      delay(500);
      Serial.print(".");
      j++;
    } else {
      Serial.println("Give up WiFi connect and go to deep sleep");      
      WiFi.disconnect();
      goToSleep();
      return;
    }
  }
  Serial.print("\nConnected with ");
  Serial.println(WiFi.localIP());

  
  // === SEND TO API ===
  HTTPClient http;
  http.begin(host, httpsPort, uri, true, fingerprint);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(jsonStr);

  if (httpResponseCode == HTTP_CODE_OK) {
    Serial.printf("Successfully posted data to %s\n", host);
    http.end();
  } else {
    Serial.printf("Failed to connect to host %s with response code %d and error %s\n", host, httpResponseCode, http.errorToString(httpResponseCode).c_str());
  }
  http.end();
  WiFi.disconnect();

  // === GO TO SLEEP ===
  goToSleep();
}

void loop() {
}
