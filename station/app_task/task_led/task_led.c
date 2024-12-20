#include "task_led.h"
#include "drv_led.h"
#include "../../sys_config.h"
#include "drv_usb.h"
#include "drv_http.h"

TaskHandle_t Led_Task__Handle = NULL;

static void ledTask();

int createLedTask()
{
    xTaskCreate((TaskFunction_t)ledTask,
                (const char*)"ledTask",
                (uint32_t )LED_TASK_STACK_SIZE,
                (void *	)NULL,
                (UBaseType_t)LED_TASK_PRIORITY,
                &Led_Task__Handle);
    return OPERATE_SUCCESS;
}

static void ledTask()
{
    initLed();

    while(1)
    {
        //setLed(1,1,0);
        //char data[12] = "hello world\r\n";
        //tud_cdc_write_str(data);
        //if (tud_cdc_connected())
        {
            // tud_cdc_write_clear();
            // tud_cdc_write(data, sizeof(data));
            // //tud_cdc_write_char(data[0]);
            // tud_cdc_write_flush();
        }
  
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}