#include <Arduino.h>
#include <U8g2lib.h>
#include <ESP8266WiFi.h>

#include "netconfig.h"

String myself;                                        //myself ip
bool flag = 0;                                        //flag = 0 Keep sending package to server until received

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

void setup()
{
  Serial.begin(115200);
  // init u8g2
  u8g2.begin();
  //init airkiss
  initNetConfig();
  startAirkiss();
  myself = WiFi.localIP().toString();
  Serial.print("IP: ");
  Serial.println(myself);
}

void loop() {
}