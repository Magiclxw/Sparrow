#include "drv_power.h"
#include "driver/gpio.h"
#include "esp_sntp.h"
#include "drv_nvs.h"

static void drvPowerInitWakeupGpio();
static void drvPowerInitSwitchGpio();
static void drvPowerInitBatterySignal();

void drvPowerInit()
{
    drvPowerInitWakeupGpio();
    drvPowerInitSwitchGpio();
    drvPowerInitBatterySignal();
}

static void drvPowerInitWakeupGpio()
{
    gpio_reset_pin(WAKEUP_PIN);
    gpio_set_direction(WAKEUP_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(WAKEUP_PIN, GPIO_PULLDOWN_ONLY);
}

void drvPowerInitSwitchGpio()
{
    gpio_reset_pin(SWTICH_PIN);
    gpio_set_direction(SWTICH_PIN, GPIO_MODE_OUTPUT);
    // gpio_set_pull_mode(SWTICH_PIN, GPIO_PULLDOWN_ONLY);

    gpio_set_level(SWTICH_PIN, 0);
}

static void drvPowerInitBatterySignal()
{
    // gpio_reset_pin(ADC_EN_PIN);
    // gpio_reset_pin(CHARGE_PIN);
    // gpio_reset_pin(STANDBY_PIN);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(ADC_EN_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(CHARGE_PIN,GPIO_MODE_INPUT);
    gpio_set_direction(STANDBY_PIN,GPIO_MODE_INPUT);

    gpio_set_level(ADC_EN_PIN, 1);
    gpio_set_pull_mode(CHARGE_PIN,GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(STANDBY_PIN,GPIO_PULLUP_ONLY);

} 

int drvPowergetChargeState(void)
{
    return gpio_get_level(CHARGE_PIN);
}

int drvPowergetStandbyState(void)
{
    return gpio_get_level(STANDBY_PIN);
}

int drvPowerGetWakeupLevel(void)
{
    return gpio_get_level(WAKEUP_PIN);
}

/**
 * @brief 设置下次开机时间
*/
uint64_t drvPowerGetNextWakeupTime()
{
    uint64_t wakeupInterval = 0;
    time_t now;
    time(&now);
    setenv("TZ", "CST-8", 1);
    tzset();

    nvsLoadWakeupInterval(&wakeupInterval);

    now += wakeupInterval;

    return now;
}

void drvPowerSetState(uint8_t state)
{
    gpio_set_level(SWTICH_PIN, state);
}

/**
 * @brief 控制电脑开关机
*/
void drvPowerOnOff()
{
    drvPowerSetState(1);
    vTaskDelay(pdMS_TO_TICKS(500));
    drvPowerSetState(0);
}