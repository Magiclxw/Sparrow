#ifndef __DRV_POWER_H__
#define __DRV_POWER_H__

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"

#define WAKEUP_PIN  (21)

void drvPowerInitWakeupGpio();
uint8_t drvPowerGetWakeupLevel(void);

#endif