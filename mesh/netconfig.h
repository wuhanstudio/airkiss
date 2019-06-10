#ifndef __NETCONFIG_H__
#define __NETCONFIG_H__

#include <ESP8266WiFi.h>
#include <airkiss.h>
#include <U8g2lib.h>
#include <Arduino.h>

void initNetConfig();
void startAirkiss();
void deleteConfig();
void myPrint(const char* message);
String getMacAddress();

#endif