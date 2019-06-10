#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include "netconfig.h"
#include "DHTesp.h"

#define DHT 2                                         //DHT GPIO2

String myself;                                        //myself ip
bool flag = 0;                                        //flag = 0 Keep sending package to server until received

DHTesp dht;                                           //DHT11 sensor
ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server(80);                          //open webserver on port 80

//reply to "status"
void SendStatus()
{
  server.send(200, "text/plain", "ok");
  Serial.println("...");
}
//reply to "temp"
void SendTemp()
{
  delay(dht.getMinimumSamplingPeriod());
  server.send(200, "text/plain", String(dht.getTemperature()));
}
//reply to "humid"
void Sendhumid()
{
  delay(dht.getMinimumSamplingPeriod());
  server.send(200, "text/plain", String(dht.getHumidity()));
}

void setup()
{
  Serial.begin(115200);
  //init DHT11 sensor , Connect DHT sensor to GPIO 2
  dht.setup(DHT, DHTesp::DHT11);
  Serial.println("DHT11 is ready!");
  //init airkiss
  initNetConfig();
  startAirkiss();
  myself = WiFi.localIP().toString();
  Serial.print("IP: ");
  Serial.println(myself);
  //init server
  server.on("/status", SendStatus);
  server.on("/temp", SendTemp);
  server.on("/humid", Sendhumid);
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
        if (httpCode > 0)
        {
          //HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTP] GET... code: %d\n", httpCode);
          //file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
          {
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

void loop()
{
  //waiting for get from webapp
  server.handleClient();
}