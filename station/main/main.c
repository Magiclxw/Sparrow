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
#include "esp_event.h"
#include "esp_log.h"
#include "drv_hid.h"
#include "drv_ble.h"
#include "task_led.h"
#include "drv_nvs.h"
#include "systemInfo.h"
#include "startup.h"
#include "lwip/err.h"
#include "lwip/sys.h"

void app_main(void)
{
  static uint16_t powerOnTimes = 0;
  preStartup();

  vTaskDelay(pdMS_TO_TICKS(100));
  sysInfoGetPowerOnTimes(&powerOnTimes);
  printf("powerOnTimes = %d\r\n",powerOnTimes);
  powerOnTimes++;
  sysInfoSetPowerOnTimes(powerOnTimes);

  midStartup();

  postStartup();
}