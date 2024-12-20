#include "drv_led.h"
#include "driver/gpio.h"

void initLed(void)
{
    gpio_reset_pin(GPIO_LED_R);
    gpio_reset_pin(GPIO_LED_G);
    gpio_reset_pin(GPIO_LED_B);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(GPIO_LED_R, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_LED_G, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_LED_B, GPIO_MODE_OUTPUT);
}

void setLed(LedColorEnum color)
{
    uint8_t rgb = 0x01;
    rgb = ~(rgb << color);
    gpio_set_level(GPIO_LED_R, rgb&0x01);
    gpio_set_level(GPIO_LED_G, rgb&0x02);
    gpio_set_level(GPIO_LED_B, rgb&0x04);
}