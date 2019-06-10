#include <Arduino.h>
#include <U8g2lib.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include "netconfig.h"

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY!)
#error Select ESP8266 board.
#endif

//U8G2
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 12, /* data=*/ 13, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

//myself ip
String myself;

//nodejs
bool flag = 0;

ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server(80);



void SendStatus() {
  server.send(200, "text/plain", "ok");
  Serial.println("...");
}

void Display() {
  server.send(200, "text/plain", "ok");
  myPrint(server.arg(0).c_str());
}

void setup() {
  Serial.begin(115200);

  // init u8g2
  u8g2.begin();

  //init airkiss
  initNetConfig();
  startAirkiss();
  
  Serial.println();
  myself = WiFi.localIP().toString();
  Serial.println(myself);

  //init server
  server.on("/status", SendStatus);
  server.on("/display", Display);
  server.begin();
  Serial.println("HTTP server started!");

  while (flag == 0) {
    // wait for WiFi connection
    if ((WiFiMulti.run() == WL_CONNECTED)) {

      WiFiClient client;

      HTTPClient http;

      Serial.print("[HTTP] begin...\n");
      if (http.begin(client, "http://192.168.199.209:5000/register?ip=" + myself + "&type=lcd&id=6")) {  // HTTP

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

void loop() {
  //waiting for get
  server.handleClient();
}
