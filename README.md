# nodemcu-temperature-sensor
NodeMCU/ESP822 software that reads an AM2320 sensor connected via 1-wire and sends the temperature and humidity using HTTP over Wi-Fi.

## Hardware components
The following hardware components are used

* NodeMCU V2 Development board WiFi ESP8266-12E ()
* Temperatue and humidity sensor ASAIR AM2320 (https://www.adafruit.com/product/3721)
* Breadboard

## Setup
The setup instructions is loosely based on the Losant tutorial (https://www.losant.com/blog/getting-started-with-the-esp8266-and-dht22-sensor) and here is a summary of the steps performed:

### Drivers
* Install USB drivers for the NodeMcu UART found at https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers.

### Arduino IDE
* Install the Arduino IDE version 1.8.0 found at https://www.arduino.cc/en/Main/Software.
* Add the ESP8266 board manager URL (http://arduino.esp8266.com/stable/package_esp8266com_index.json) in Arduino IDE preferences, Arduino->Preferences->"Additional Boards Maanger URLs".
* Install version 2.4.2 of the ESP8266 Board Manager by searching for "esp8266" in the "Board Manager" found at "Tools->Board->Boards Manager" and install the "esp8266 by ESP8266 Community" package.
* Restart the Arduino IDE.
* Select the "NodeMCU 1.0 (ESP-12E Module) in the Tools->Board menu.

### Libraries
The following libraries need to be installed in the Arduio IDE:

* ArduinoJson by Benoit Blanchon (version 5.13.2)
* Adafruit Unified Sensor by Adafruit (version 1.0.2)
* DHT Sensor library by Adafruit (version 1.2.3)
* Adafruit AM2320 sensor library by Adafruit (1.1.1)
