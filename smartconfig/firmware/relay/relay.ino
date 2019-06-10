#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include "netconfig.h"

//myself ip
String myself;

//nodejs
bool flag = 0;

ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server(80);

void SendStatus() {
  String message = "ok";
  server.send(200, "text/plain", message);
}

void Switch() {
  String message = "ok";
  digitalWrite(0, LOW);  
  delay(1000);
  digitalWrite(0, HIGH);
  server.send(200, "text/plain", message);
  
}

void setup() {
  Serial.begin(115200);

  //init relay
  pinMode(0, OUTPUT);
  digitalWrite(0, HIGH);

  //init airkiss
  initNetConfig();
  startAirkiss();
  Serial.println();
  myself = WiFi.localIP().toString();
  Serial.println(myself);

  //init server
  server.on("/status", SendStatus);
  server.on("/switch", Switch);
  server.begin();
  Serial.println("HTTP server started!");

  while (flag == 0) {
    // wait for WiFi connection
    if ((WiFiMulti.run() == WL_CONNECTED)) {

      WiFiClient client;

      HTTPClient http;

      Serial.print("[HTTP] begin...\n");
      if (http.begin(client, "http://192.168.199.209:5000/register?ip=" + myself + "&type=lcd&id=8")) {  // HTTP

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
