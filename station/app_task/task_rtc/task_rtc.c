#include "task_rtc.h"
#include "../../sys_config.h"

TaskHandle_t Rtc_Task__Handle = NULL;

static void Rtc_Task();

int Rtc_Task_Create()
{
    xTaskCreate((TaskFunction_t)Rtc_Task,
                (const char*)"Rtc_Task",
                (uint32_t )RTC_TASK_STACK_SIZE,
                (void *	)NULL,
                (UBaseType_t)RTC_TASK_PRIORITY,
                &Rtc_Task__Handle);
    return OPERATE_SUCCESS;
}

static void Rtc_Task()
{

    while(1)
    {

    }
}