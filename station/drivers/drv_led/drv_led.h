#ifndef __DRV_LED_H
#define __DRV_LED_H

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"


#define GPIO_LED_R 35
#define GPIO_LED_G 34
#define GPIO_LED_B 33

void initLed(void);
void setLed(uint8_t r, uint8_t g, uint8_t b);

#endif