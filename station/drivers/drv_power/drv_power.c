#include "drv_power.h"
#include "driver/gpio.h"

void drvPowerInitWakeupGpio()
{
    gpio_reset_pin(WAKEUP_PIN);
    gpio_set_direction(WAKEUP_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(WAKEUP_PIN, GPIO_PULLDOWN_ONLY);
}

uint8_t drvPowerGetWakeupLevel(void)
{
    return gpio_get_level(WAKEUP_PIN);
}