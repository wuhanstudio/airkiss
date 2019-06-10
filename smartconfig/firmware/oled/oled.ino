#include <Arduino.h>
#include <U8g2lib.h>
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

//U8G2_SSD1306_128X64
#define clock 12
#define data  13
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ clock, /* data=*/ data, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

// //U8G2_ST7565_JLX12864
// #define OLED_SPI_PIN_CLK                  4           // PB3
// #define OLED_SPI_PIN_MOSI                 5           // PB4
// #define OLED_SPI_PIN_RES                  12          // PB6
// #define OLED_SPI_PIN_DC                   13          // PB5
// #define OLED_SPI_PIN_CS                   14          // PB7
// #define OLED_SPI_LIGHT                    15          // PB8
// U8G2_ST7565_JLX12864_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ OLED_SPI_PIN_CLK, /* data=*/ OLED_SPI_PIN_MOSI, /* cs=*/ OLED_SPI_PIN_CS, /* dc=*/ OLED_SPI_PIN_DC, /* reset=*/ OLED_SPI_PIN_RES);

//reply to "status"
void SendStatus() {
  server.send(200, "text/plain", "ok");
  Serial.println("...");
}
//reply to "display"
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
  myself = WiFi.localIP().toString();
  Serial.print("IP: ");
  Serial.println(myself);
  //init server
  server.on("/status", SendStatus);
  server.on("/display", Display);
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