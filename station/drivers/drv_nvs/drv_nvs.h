#ifndef __DRV_NVS_H__
#define __DRV_NVS_H__

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
/**
 * @attention nvs 键长最大为15字符
*/
#define USER_NAMESPACE_0 ("storage")
#define SERVER_ADDRESS  ("mqttAddr")
#define SERVER_USERNAME ("mqttUsername")
#define SERVER_PASSWORD ("mqttPassword")
#define NVS_KEY_BACKGROUND ("background")
#define NVS_WEATHER ("weather")
#define NVS_BILIBILI ("bilibili")
#define NVS_SLEEP_TIME ("sleepTime")

#define NVS_WAKEUP_INTERVAL ("wakeupInterval")
#define NVS_POWER_CTRL      ("powerCtrl")
#define NVS_PC_PASSWORD     ("pcPassword")
#define NVS_PC_PASSWORD_CTRL    ("pcPasswordCtrl")
#define NVS_PC_PASSWORD_WAIT    ("pcPasswordWait")
#define NVS_LED_CTRL            ("ledCtrl")
#define TOOLS_TOKEN_CTRL        ("tokenCtrl")
#define NVS_SERVO_IDLE_ANGLE    ("idleAngle")
#define NVS_SERVO_POS_ANGLE     ("posAngle")
#define NVS_SERVO_NEG_ANGLE     ("negAngle")
#define NVS_BEL_NAME            ("bleName")

#define NVS_KEY_BACKGROUND_INDEX     ("bgIndex")   // 背景索引

esp_err_t initNvs(void);
esp_err_t nvsOpen(const char* namespace_name, nvs_open_mode_t open_mode);
void nvsClose();
esp_err_t nvsGetStr(const char* key, char* out_value, size_t* length);
esp_err_t nvsSetStr(const char* key, const char* value);
esp_err_t nvsCommit();
esp_err_t nvsEraseKey(const char* key);
esp_err_t nvsSaveValue(const char* namespace_name, nvs_open_mode_t open_mode, const char* key, const char* value);
esp_err_t nvsLoadValue(const char* namespace_name, nvs_open_mode_t open_mode, const char* key, char* out_value, size_t *length);
esp_err_t nvsSaveU32(const char* namespace_name, nvs_open_mode_t open_mode, const char* key, const uint32_t value);
esp_err_t nvsLoadU32(const char* namespace_name, nvs_open_mode_t open_mode, const char* key, uint32_t *value);
esp_err_t nvsSaveBlobData(const char* namespace_name, nvs_open_mode_t open_mode, const char* key, uint8_t *out_value, size_t length);
esp_err_t nvsLoadBlobData(const char* namespace_name, nvs_open_mode_t open_mode, const char* key, uint8_t *out_value);

esp_err_t nvsSaveWakeupInterval(uint64_t interval);
esp_err_t nvsLoadWakeupInterval(uint64_t *out_value);
esp_err_t nvsSavePowerCtrl(uint32_t ctrl);
esp_err_t nvsSavePcPassword(char* password);
esp_err_t nvsSavePcPasswordCtrl(uint32_t ctrl);
esp_err_t nvsSavePcPasswordWait(uint32_t waitTime);
esp_err_t nvsSaveLedCtrl(uint32_t ctrl);
esp_err_t nvsSaveToolsTokenCtrl(uint32_t ctrl);
esp_err_t nvsSaveIdleAngle(uint32_t angle);
esp_err_t nvsLoadIdleAngle(uint32_t * angle);
esp_err_t nvsSavePosAngle(uint32_t angle);
esp_err_t nvsLoadPosAngle(uint32_t * angle);
esp_err_t nvsSaveNegAngle(uint32_t angle);
esp_err_t nvsLoadNegAngle(uint32_t * angle);
esp_err_t nvsSaveBackgroundIndex(uint32_t index);
esp_err_t nvsLoadBackgroundIndex(uint32_t *out_value);
esp_err_t nvsSetWeatherUrl(const char *url);
esp_err_t nvsSetBilibiliUrl(const char *url);
esp_err_t nvsSaveBleName(char* name);
esp_err_t nvsLoadBleName(char *name);

#endif