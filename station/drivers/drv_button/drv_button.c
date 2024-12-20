#include "drv_button.h"
#include "driver/gpio.h"

/**
 * @brief 按键初始化
 * @attention 该函数会在lvgl配置中初始化
 */
void initButton()
{
    gpio_reset_pin(GPIO_BUTTON);
    //输入模式
    gpio_set_direction(GPIO_BUTTON, GPIO_MODE_INPUT);
    //上拉
    gpio_set_pull_mode(GPIO_BUTTON, GPIO_PULLUP_ONLY);
}

int buttonGetState(void)
{
    return gpio_get_level(GPIO_BUTTON);
}