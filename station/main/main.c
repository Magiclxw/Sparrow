/* WiFi station Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver_mqtt.h"
#include "wifi_station.h"
#include "task_servo.h"
#include "task_rtc.h"
#include "task_battery.h"
#include "drv_hid.h"
#include "drv_ble.h"
#include "task_bluetooth.h"
#include "task_led.h"

#include "lwip/err.h"
#include "lwip/sys.h"

void app_main(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    //hid_test();
    LED_Task_Create();
    clearWifiData();
    //wifi初始化
    initialise_wifi();
    initBLE();
    Bluetooth_Task_Create();
    //mqtt初始化，建立mqtt连接
    mqtt_app_start();
    //启动舵机线程
    Servo_Control_TASK_Create();
    //创建电源线程
    Battery_Task_Create();

    //启动RTC线程
    ////Rtc_Task_Create();
}