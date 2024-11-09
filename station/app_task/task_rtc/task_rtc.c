#include "task_rtc.h"
#include "../../sys_config.h"
#include "drv_power.h"

TaskHandle_t Rtc_Task__Handle = NULL;

static void rtcTask();

int createRtcTask()
{
    xTaskCreate((TaskFunction_t)rtcTask,
                (const char*)"rtcTask",
                (uint32_t )RTC_TASK_STACK_SIZE,
                (void *	)NULL,
                (UBaseType_t)RTC_TASK_PRIORITY,
                &Rtc_Task__Handle);
    return OPERATE_SUCCESS;
}

static void rtcTask()
{

    while(1)
    {
        drvPowerGetNextWakeupTime();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}