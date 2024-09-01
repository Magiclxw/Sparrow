#include "startup.h"
#include "task_led.h"
#include "wifi_station.h"
#include "drv_mqtt.h"
#include "drv_nvs.h"
#include "task_bluetooth.h"
#include "task_servo.h"
#include "task_rtc.h"
#include "task_battery.h"
#include "task_display.h"
#include "drv_usb.h"
#include "systemInfo.h"
#include "drv_led.h"

SemaphoreHandle_t preStartupSemaphore;

void preStartup()
{
    preStartupSemaphore = xSemaphoreCreateBinary();

    initNvs();
    //clearWifiData();
    setLed(LED_RED);
    //wifi初始化
    initWifi();
    //mqtt初始化，建立mqtt连接
    initMqtt();

    initBLE();
    
    intiUsb();

    //xSemaphoreTake(preStartupSemaphore,portMAX_DELAY);
}

void midStartup()
{
    //esp_deep_sleep(1000000LL * 3600);
    vTaskDelay(pdMS_TO_TICKS(100));
    createDisplayTask();
}

void postStartup()
{
    createLedTask();

    createBleRecTask();
    createBleTransTask();

    //启动舵机线程
    createServoTask();
    //创建电源线程
    //Battery_Task_Create();

    //启动RTC线程
    //Rtc_Task_Create();

    vTaskDelay(pdMS_TO_TICKS(100));
    //记录开机次数
    sysInfoIncrementPowerOnTimes();
}