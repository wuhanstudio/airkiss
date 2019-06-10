#include "netconfig.h"

String DEVICE_ID = "ESP8266--";           //define Device ID
unsigned long time1;                      //recode Connecting time
unsigned long time2;                      //recode Connecting time

//初始化提示灯和连接上一次wifi
void initNetConfig() {
  //初始化设备ID
  DEVICE_ID = DEVICE_ID + WiFi.softAPmacAddress().c_str();
  Serial.print("Device:");
  Serial.println(DEVICE_ID);
}

//启动airkiss
void startAirkiss() {
Serial.println("start airkiss...");
  WiFi.mode(WIFI_STA);
  WiFi.beginSmartConfig();
  while (1) {
    Serial.print(".");
    if (WiFi.smartConfigDone()) {
      Serial.println("");
      Serial.println("airkiss Success");
      Serial.printf("SSID:%s\r\nPSW:%s\r\n", WiFi.SSID().c_str(), WiFi.psk().c_str());
      //use passwd to connect
      Serial.println("Connecting...");
      time1 = millis();
      time2 = millis();
      WiFi.begin(WiFi.SSID().c_str(), WiFi.psk().c_str());
      while (WiFi.status() != WL_CONNECTED && (time2 - time1 < 10000)) {
        delay(500);
        Serial.print(".");
        time2 = millis();
      }
      //长时间未连接
      if(time2 - time1 >= 10000){
        //重启芯片 等待重新接收包
        deleteConfig();
      }
      break;
    }
    delay(500); // 这个地方一定要加延时，否则极易崩溃重启
  }
}

//删除wifi连接记录并重启
void deleteConfig() {
  ESP.eraseConfig();
  delay(100);
  ESP.reset();
  ESP.restart();
}

//获取mac地址
String getMacAddress() {
  return DEVICE_ID;
}