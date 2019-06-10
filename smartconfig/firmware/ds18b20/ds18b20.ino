#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "netconfig.h"
#include "DHTesp.h"

#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer;

#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY!)
#error Select ESP8266 board.
#endif

//myself ip
String myself;

//nodejs
bool flag = 0;

//DHT11 sensor
DHTesp dht;

ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server(80);

void SendStatus() {
  String message = "ok";
  server.send(200, "text/plain", message);
  Serial.println("...");
}

void SendTemp_DS18B20() {
  sensors.requestTemperatures(); // Send the command to get temperatures
  float tempC = sensors.getTempC(insideThermometer);
  server.send(200, "text/plain", String(tempC));
}

void setup()
{
  Serial.begin(115200);

  //init DS18B20 sensor
  sensors.begin();
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0");
  sensors.setResolution(insideThermometer, 9);

  //init airkiss
  initNetConfig();
  startAirkiss();
  Serial.println();
  myself = WiFi.localIP().toString();
  Serial.println(myself);

  //init server
  server.on("/status", SendStatus);
  server.on("/temp", SendTemp_DS18B20);
  server.begin();
  Serial.println("HTTP server started!");

  while (flag == 0) {
    // wait for WiFi connection
    if ((WiFiMulti.run() == WL_CONNECTED)) {

      WiFiClient client;

      HTTPClient http;

      Serial.print("[HTTP] begin...\n");
      if (http.begin(client, "http://192.168.199.209:5000/register?ip=" + myself + "&type=ds18b20&id=5")) {  // HTTP

        Serial.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if (httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTP] GET... code: %d\n", httpCode);

          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = http.getString();
            Serial.println(payload);
          }
          flag = 1;
        } else {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
          flag = 0;
          delay(2000);
        }

        http.end();
      } else {
        Serial.printf("[HTTP} Unable to connect\n");
        deleteConfig();
      }
    }
  }
}

void loop()
{
  //waiting for get
  server.handleClient();
}
