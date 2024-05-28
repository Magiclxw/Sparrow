#include "task_led.h"
#include "drv_led.h"
#include "../../sys_config.h"

TaskHandle_t Led_Task__Handle = NULL;

static void Led_Task();

int LED_Task_Create()
{
    xTaskCreate((TaskFunction_t)Led_Task,
                (const char*)"Led_Task",
                (uint32_t )LED_TASK_STACK_SIZE,
                (void *	)NULL,
                (UBaseType_t)LED_TASK_PRIORITY,
                &Led_Task__Handle);
    return OPERATE_SUCCESS;
}

static void Led_Task()
{
    initLed();

    while(1)
    {
        //setLed(1,0,1);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}