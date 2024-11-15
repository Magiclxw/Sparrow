#ifndef __DRV_POWER_H__
#define __DRV_POWER_H__

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"

#define WAKEUP_PIN  GPIO_NUM_21
#define SWTICH_PIN  GPIO_NUM_5
#define ADC_EN_PIN  GPIO_NUM_18
#define CHARGE_PIN  GPIO_NUM_36
#define STANDBY_PIN GPIO_NUM_37

void drvPowerInit();
int drvPowerGetWakeupLevel(void);
uint64_t drvPowerGetNextWakeupTime();
int drvPowergetChargeState(void);
int drvPowergetStandbyState(void);
void drvPowerOnOff();

#endif