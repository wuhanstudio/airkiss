#ifndef __NETCONFIG_H__
#define __NETCONFIG_H__

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <airkiss.h>
#include <U8g2lib.h>
#include <Arduino.h>

#define DEFAULT_LAN_PORT  12476

void initNetConfig();
bool startAirkiss();
void deleteConfig();
void myPrint(const char* message);

String getMacAddress();

//void startDiscover();

#endif
