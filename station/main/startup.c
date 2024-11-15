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
#include "drv_http.h"
#include "task_http.h"
#include "task_usb.h"
#include "task_mqtt.h"
#include "drv_servo.h"
#include "drv_power.h"

SemaphoreHandle_t preStartupSemaphore;

void preStartup()
{
    uint32_t sleepTime = 0;
    preStartupSemaphore = xSemaphoreCreateBinary();

    initNvs();
    //clearWifiData();

    sysGetSleepTime(&sleepTime);
    // 初始化电源引脚
    drvPowerInit();

    //wifi初始化
    initWifi();

    initBLE();

    initServo();

    //xSemaphoreTake(preStartupSemaphore,portMAX_DELAY);
}

void midStartup()
{
    //esp_deep_sleep(1000000LL * 3600);
    // esp_deep_sleep(1000000LL * 60);
    vTaskDelay(pdMS_TO_TICKS(100));
    createDisplayTask();
}

void postStartup()
{
    //mqtt初始化，建立mqtt连接
    createMqttTask();
    
    // createUsbTask();

    createLedTask();
    createHttpTask();

    createBleRecTask();
    createBleTransTask();

    //启动舵机线程
    // createServoTask();
    //创建电源线程
    createBatteryTask();

    //启动RTC线程
    createRtcTask();

    vTaskDelay(pdMS_TO_TICKS(100));
    //记录开机次数
    sysInfoIncrementPowerOnTimes();
}