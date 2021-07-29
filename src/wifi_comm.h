#ifndef WIFI_COMM_H
#define WIFI_COMM_H
#include "consts.h"
#include <WiFi.h>

extern void sendingLoop(void * parameters);
extern WiFiServer wifiServer;

#endif
