#include "netconfig.h"

extern U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2;

String DEVICE_TYPE = "gh_5e8661030bb8";
//char* DEVICE_ID = "gh_5e8661030bb8_e282ca678250674b";
String DEVICE_ID = "4Dclass_WifiModule_";
unsigned long time1;
unsigned long time2;

void myPrint(const char* message) {
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(0, 10, message); // write something to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(1000);
}

/**
   初始化提示灯和连接上一次wifi
*/
void initNetConfig() {
  //初始化设备ID
  DEVICE_ID = DEVICE_ID + WiFi.softAPmacAddress().c_str();
  Serial.println("");
}

/**
   启动airkiss
*/
bool startAirkiss() {
  Serial.println("");
  Serial.println("start airkiss...");
  WiFi.mode(WIFI_STA);
  WiFi.beginSmartConfig();
  while (1) {
    Serial.print(".");
    if (WiFi.smartConfigDone()) {
      Serial.println(".");
      Serial.println("airkiss Success");
      Serial.printf("SSID:%s\r\nPSW:%s\r\n", WiFi.SSID().c_str(), WiFi.psk().c_str());
      myPrint(WiFi.psk().c_str());
      delay(10000);
      WiFi.setAutoConnect(false);  // 设置自动连接
      //设置连接
      Serial.println("");
      Serial.println("Connecting...");
      time1 = millis();
      time2 = millis();
      WiFi.begin(WiFi.SSID().c_str(), WiFi.psk().c_str());
      while (WiFi.status() != WL_CONNECTED && (time2 - time1 < 10000)) {
        delay(500);
        Serial.print(".");
        time2 = millis();
      }
      if (time2 - time1 >= 10000) {
        deleteConfig();
      }
      break;
    }
    delay(500); // 这个地方一定要加延时，否则极易崩溃重启
  }
//  deleteConfig(); //疯狂重启
}

/**
   删除wifi连接记录
*/
void deleteConfig() {
  ESP.eraseConfig();
  delay(100);
  ESP.reset();
  ESP.restart();
}

/**
   获取mac地址
*/
String getMacAddress() {
  return DEVICE_ID;
}
