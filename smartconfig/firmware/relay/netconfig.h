#ifndef __NETCONFIG_H__
#define __NETCONFIG_H__

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <airkiss.h>

void initNetConfig();
void startAirkiss();
void deleteConfig();
String getMacAddress();

#endif