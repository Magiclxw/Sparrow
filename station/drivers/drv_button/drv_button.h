#ifndef __DRV_BUTTON_H__
#define __DRV_BUTTON_H__

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"
#include "esp_system.h"

#define GPIO_POWER  (5)
#define GPIO_BUTTON (17)



void initButton();
int buttonGetState(void);

#endif