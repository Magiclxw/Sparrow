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
    
    //nvs_flash_erase();
    //  nvs_handle_t my_handle;
    //  ret = nvs_open("storage", NVS_READWRITE, &my_handle);
    //  nvs_erase_key(my_handle,"ssid");
    //  nvs_erase_key(my_handle,"password");
    // if (ret != ESP_OK) {
    //     printf("Error (%s) opening NVS handle!\n", esp_err_to_name(ret));
    // } else {
    //     printf("Done\n");

    //     // Read
    //     printf("Reading restart counter from NVS ... ");
    //     char restart_counter[10]; // value will default to 0, if not set yet in NVS
    //     size_t len = 10;
    //     ret = nvs_get_str(my_handle, "restart_counter", restart_counter,&len);
    //     switch (ret) {
    //         case ESP_OK:
    //             printf("Done\n");
    //             printf("Restart counter = %s /n", restart_counter);
    //             break;
    //         case ESP_ERR_NVS_NOT_FOUND:
    //             printf("The value is not initialized yet!\n");
    //             break;
    //         default :
    //             printf("Error (%s) reading!\n", esp_err_to_name(ret));
    //     }

    //     // Write
    //     printf("Updating restart counter in NVS ... ");
    //     char restart_counter1[10] = {"1234"};
    //     ret = nvs_set_str(my_handle, "restart_counter", restart_counter1);
    //     //nvs_set_str
    //     printf((ret != ESP_OK) ? "Failed!\n" : "Done\n");

    //     // Commit written value.
    //     // After setting any values, nvs_commit() must be called to ensure changes are written
    //     // to flash storage. Implementations may write to storage at other times,
    //     // but this is not guaranteed.
    //     printf("Committing updates in NVS ... ");
    //     ret = nvs_commit(my_handle);
    //     printf((ret != ESP_OK) ? "Failed!\n" : "Done\n");

    //     // Close
    //     nvs_close(my_handle);
    // }

    // ESP_ERROR_CHECK(ret);

    initialise_wifi();

    mqtt_app_start();

    Servo_Control_TASK_Create();
}
