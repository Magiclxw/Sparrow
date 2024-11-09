#ifndef __DRV_NVS_H__
#define __DRV_NVS_H__

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"

#define USER_NAMESPACE_0 ("storage")
#define SERVER_ADDRESS  ("server_address")
#define SERVER_USERNAME ("server_username")
#define SERVER_PASSWORD ("server_password")
#define NVS_KEY_BACKGROUND ("nvs_key_background")
#define NVS_WEATHER ("weather")
#define NVS_BILIBILI ("bilibili")
#define NVS_SLEEP_TIME ("sleepTime")

#define NVS_WAKEUP_INTERVAL ("wakeup_interval")
#define NVS_POWER_CTRL      ("power_ctrl")
#define NVS_PC_PASSWORD     ("pc_password")
#define NVS_PC_PASSWORD_CTRL    ("pc_password_ctrl")
#define NVS_PC_PASSWORD_WAIT    ("pc_password_wait")
#define NVS_LED_CTRL            ("led_ctrl")
#define TOOLS_TOKEN_CTRL        ("tools_token_ctrl")
#define NVS_SERVO_IDLE_ANGLE    ("servo_idle_angle")
#define NVS_SERVO_POS_ANGLE     ("servo_pos_angle")
#define NVS_SERVO_NEG_ANGLE     ("servo_neg_angle")

#define NVS_KEY_BACKGROUND      ("background")

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
esp_err_t nvsSavePosAngle(uint32_t angle);
esp_err_t nvsSaveNegAngle(uint32_t angle);
esp_err_t nvsSaveBackgroundIndex(uint32_t index);
esp_err_t nvsLoadBackgroundIndex(uint32_t *out_value);

#endif