#ifndef __DRV_NVS_H__
#define __DRV_NVS_H__

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"

#define USER_NAMESPACE_0 ("storage")

#define SERVER_ADDRESS  ("server_address")

esp_err_t initNvs(void);
esp_err_t nvsOpen(const char* namespace_name, nvs_open_mode_t open_mode);
void nvsClose();
esp_err_t nvsGetStr(const char* key, char* out_value, size_t* length);
esp_err_t nvsSetStr(const char* key, const char* value);
esp_err_t nvsCommit();
esp_err_t nvsEraseKey(const char* key);

#endif