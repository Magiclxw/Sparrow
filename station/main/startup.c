#include "startup.h"
#include "task_led.h"
#include "wifi_station.h"
#include "driver_mqtt.h"
#include "drv_nvs.h"
#include "task_bluetooth.h"
#include "task_servo.h"
#include "task_rtc.h"
#include "task_battery.h"
#include "task_display.h"
#include "drv_hid.h"

SemaphoreHandle_t preStartupSemaphore;

void preStartup()
{
    preStartupSemaphore = xSemaphoreCreateBinary();

    initNvs();
    //clearWifiData();
    //wifi初始化
    initialise_wifi();
    //mqtt初始化，建立mqtt连接
    mqtt_app_start();

    hid_test();
    //xSemaphoreTake(preStartupSemaphore,portMAX_DELAY);
}

void midStartup()
{
    //esp_deep_sleep(1000000LL * 3600);
    displayTaskCreate();
}

void postStartup()
{
    LED_Task_Create();

    Bluetooth_Task_Create();

    //启动舵机线程
    Servo_Control_TASK_Create();
    //创建电源线程
    Battery_Task_Create();

    //启动RTC线程
    ////Rtc_Task_Create();
}