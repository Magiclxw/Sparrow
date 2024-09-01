#ifndef __DRV_LED_H
#define __DRV_LED_H

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"


#define GPIO_LED_R 33
#define GPIO_LED_G 34
#define GPIO_LED_B 35

typedef enum ledColorEnum
{
    LED_RED,
    LED_GREEN,
    LED_BLUE,
}LedColorEnum;

void initLed(void);
void setLed(LedColorEnum color);

#endif