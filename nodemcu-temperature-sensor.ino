#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "DHT.h"

#define DHTPIN 4 // = D2 on the NodeMCU
#define DHTTYPE DHT22

const char* sensor_id = "temp-1";
const char* ssid = "SSID";
const char* password = "PASSWORD";

const char* host = "http://www.example.com";

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(2000);
  while (!Serial) {}

  Serial.printf("\n%s started\n", sensor_id);
  Serial.println("------------------------------");

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");      
  }
  Serial.print(" connected ");
  Serial.println(WiFi.localIP());
}

void loop() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (isnan(t) || isnan(t)) {
    Serial.println("Failed to read temperature/humidity from sensor");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");

  StaticJsonBuffer<200> jsonBuffer; 
  JsonObject& root = jsonBuffer.createObject(); 
  root["sensor"] = sensor_id; 
  root["temperature"] = t; 
  root["humidity"] = h; 
  String jsonStr;
  root.printTo(jsonStr);
  Serial.println(jsonStr);

  HTTPClient http;
  http.begin(host);
  // http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.GET();
  // int httpResponseCode = http.POST(JSONmessageBuffer);

  if (httpResponseCode == HTTP_CODE_OK) {
    Serial.printf("Successfully connected to %s\n", host);
    http.end();
  } else {
    Serial.printf("Failed to connect to host %s with response code %d and error %s\n", host, httpResponseCode, http.errorToString(httpResponseCode).c_str());
  }
  
  delay(3000);
}
