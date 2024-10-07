#include "drv_button.h"
#include "driver/gpio.h"

/**
 * @brief 按键初始化
 * @attention 该函数会在lvgl配置中初始化
 */
void initButton()
{
    gpio_reset_pin(GPIO_BUTTON);
    gpio_reset_pin(GPIO_POWER);
    //输入模式
    gpio_set_direction(GPIO_BUTTON, GPIO_MODE_INPUT);
    gpio_set_direction(GPIO_POWER, GPIO_MODE_OUTPUT);
    //上拉
    gpio_set_pull_mode(GPIO_BUTTON, GPIO_PULLUP_ONLY);
    //低电平断开通路
    gpio_set_level(GPIO_POWER, 0);
}

int buttonGetState(void)
{
    return gpio_get_level(GPIO_BUTTON);
}

void powerSetState(uint8_t state)
{
    gpio_set_level(GPIO_POWER, state);
}