#pragma once

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

extern const String WIFI_SSID;
extern const String WIFI_PASSWORD;
extern const String IFTTT_KEY;

extern const int MAX_COLS;
extern const int MAX_ROWS;

extern const int RFID_SS_PIN;
extern const int RFID_RST_PIN;

extern const bool DEBUG;

#endif