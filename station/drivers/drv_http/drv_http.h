#ifndef __DRV_HTTP_H__
#define __DRV_HTTP_H__

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "esp_log.h"
#include "cJSON.h"

#define MAX_HTTP_WEATHER_RECV_BUFFER 300
#define MAX_HTTP_BILIBILI_RECV_BUFFER 150
#define MAX_HTTP_OUTPUT_BUFFER  512

void initHttp(void);
void httpRefreshData();
esp_err_t jsonGetWeatherData();
esp_err_t jsonGetBilibiliData();
void drvHttpDeleteJsonData();
esp_err_t jsonGetBilibiliFollowing(int *buffer);
esp_err_t jsonGetBilibiliFollower(int *buffer);
esp_err_t drvHttpGetWeatherName(char *buffer);
esp_err_t drvHttpGetWeatherText(char *buffer);
esp_err_t drvHttpGetWeatherCode(char *buffer);
esp_err_t drvHttpGetWeatherTemperature(char *buffer);

#endif