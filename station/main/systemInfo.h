#ifndef __SYSTEM_INFO_H__
#define __SYSTEM_INFO_H__

#include "../drivers/drv_nvs/drv_nvs.h"

esp_err_t sysInfoGetPowerOnTimes(uint16_t *times);
esp_err_t sysInfoSetPowerOnTimes(uint16_t times);
esp_err_t sysInfoIncrementPowerOnTimes(void);

#endif