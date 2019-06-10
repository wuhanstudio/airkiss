#include "netconfig.h"

extern U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2;
// extern U8G2_ST7565_JLX12864_F_4W_SW_SPI    u8g2;

String DEVICE_ID = "ESP8266--";           //define Device ID
unsigned long time1;                      //recode Connecting time
unsigned long time2;                      //recode Connecting time

void myPrint(const char* message)
{
  u8g2.clearBuffer();                     // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr);     // choose a suitable font
  u8g2.drawStr(0, 10, message);           // write something to the internal memory
  u8g2.sendBuffer();                      // transfer internal memory to the display
  delay(1000);
}

//初始化提示灯和连接上一次wifi
void initNetConfig()
{
  //初始化设备ID
  DEVICE_ID = DEVICE_ID + WiFi.softAPmacAddress().c_str();
  Serial.print("Device:");
  Serial.println(DEVICE_ID);
}

//启动airkiss
void startAirkiss()
{
  Serial.println("start airkiss...");
  WiFi.beginSmartConfig();
  while (1)
  {
    Serial.print(".");
    if (WiFi.smartConfigDone())
    {
      Serial.println("");
      Serial.println("airkiss Success");
      Serial.printf("SSID:%s\r\nPSW:%s\r\n", WiFi.SSID().c_str(), WiFi.psk().c_str());
      myPrint(WiFi.psk().c_str());
      break;
    }
    delay(500); // 这个地方一定要加延时，否则极易崩溃重启
  }
  deleteConfig(); //疯狂重启
}

//删除wifi连接记录并重启
void deleteConfig()
{
  ESP.eraseConfig();
  delay(100);
  ESP.reset();
  ESP.restart();
}

//获取mac地址
String getMacAddress()
{
  return DEVICE_ID;
}