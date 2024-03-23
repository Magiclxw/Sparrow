#include "drv_led.h"


void initLed(void)
{
    gpio_set_pin(GPIO_LED_R|GPIO_LED_G|GPIO_LED_B);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(GPIO_LED_R|GPIO_LED_G|GPIO_LED_B, GPIO_MODE_OUTPUT);
}

void setLed(uint8_t r, uint8_t g, uint8_t b)
{
    gpio_set_level(GPIO_LED_R, r);
    gpio_set_level(GPIO_LED_G, g);
    gpio_set_level(GPIO_LED_B, b);
}