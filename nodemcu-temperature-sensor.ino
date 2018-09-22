#include "DHT.h"

#define DHTPIN 4 // = D2 on the NodeMCU
#define DHTTYPE DHT22

#define SENSOR_ID "temp-1"

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(2000);
  while (!Serial) {}

  Serial.print(SENSOR_ID);
  Serial.print(" started");
  Serial.print("------------------------------");
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
  
  delay(3000);
}
