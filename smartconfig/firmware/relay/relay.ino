#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include "netconfig.h"

String myself;                                        //myself ip
bool flag = 0;                                        //flag = 0 Keep sending package to server until received

ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server(80);                          //open webserver on port 80

//reply to "status"
void SendStatus() {
  String message = "ok";
  server.send(200, "text/plain", message);
}
//reply to "switch"
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
  myself = WiFi.localIP().toString();
  Serial.print("IP: ");
  Serial.println(myself);
  //init server
  server.on("/status", SendStatus);
  server.on("/switch", Switch);
  server.begin();
  Serial.println("HTTP server started!");
  while (flag == 0)
  {
    //wait for WiFi connection
    if ((WiFiMulti.run() == WL_CONNECTED))
    {
      WiFiClient client;
      HTTPClient http;
      Serial.print("[HTTP] begin...\n");
      //向服务器注册上线
      if (http.begin(client, "http://192.168.199.209:5000/register?ip=" + myself + "&type=dht11&id=4"))
      {
        Serial.print("[HTTP] GET...\n");
        //start connection and send HTTP header
        int httpCode = http.GET();
        //httpCode will be negative on error
        if (httpCode > 0) {
          //HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTP] GET... code: %d\n", httpCode);
          //file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = http.getString();
            Serial.println(payload);
          }
          //完成注册，退出循环
          flag = 1;
        }
        else
        {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
          delay(2000);
        }
        http.end();
      }
      else
      {
        Serial.println("[HTTP} Unable to connect");
        //未知错误 重启芯片
        deleteConfig();
      }
    }
  }
}

void loop() {
  //waiting for get
  server.handleClient();
}