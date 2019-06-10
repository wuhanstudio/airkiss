#ifndef __NETCONFIG_H__
#define __NETCONFIG_H__

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <airkiss.h>

#define DEFAULT_LAN_PORT  12476

void initNetConfig();
bool startAirkiss();
void deleteConfig();
String getMacAddress();
#endif
