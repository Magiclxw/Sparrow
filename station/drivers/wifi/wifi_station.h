#ifndef __WIFI_STATION_H
#define __WIFI_STATION_H

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_sntp.h"

void initWifi(void);
esp_err_t clearWifiData();
uint8_t wifiGetConnectState(void);

#endif