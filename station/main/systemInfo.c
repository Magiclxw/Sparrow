#include "systemInfo.h"
#include "task_rtc.h"


//开机次数统计
static uint16_t s_powerOnTimes = 0;

esp_err_t sysInfoGetPowerOnTimes(uint16_t *times)
{
    char strTimes[6] = {0};
    size_t length;

    esp_err_t ret = nvsOpen(USER_NAMESPACE_0,NVS_READWRITE);

    if(ret == ESP_OK)
    {
        ret = nvsGetStr("powerOnTimes", NULL, &length);
        if(ret == ESP_OK)
        {
            ret = nvsGetStr("powerOnTimes", strTimes, &length);
        }
    }

    if(ret != ESP_OK)
    {
        strTimes[0] = 0;
        nvsSetStr("powerOnTimes", strTimes);
    }

    *times = atoi(strTimes);

    nvsClose();

    return ret;
}

esp_err_t sysInfoSetPowerOnTimes(uint16_t times)
{
    char strTimes[6] = {0};

    esp_err_t ret = nvsOpen(USER_NAMESPACE_0, NVS_READWRITE);

    sprintf(strTimes, "%d", times);

    ret = nvsSetStr("powerOnTimes",strTimes);

    ret = nvsCommit();

    nvsClose();

    return ret;
}

/*
* @brief 统计开机次数
*/
esp_err_t sysInfoIncrementPowerOnTimes(void)
{
    esp_err_t ret = sysInfoGetPowerOnTimes(&s_powerOnTimes);

    //printf("powerOnTimes = %d\r\n",s_powerOnTimes);

    s_powerOnTimes++;

    ret = sysInfoSetPowerOnTimes(s_powerOnTimes);

    return ret;
}


void sysSetSleepTime(uint32_t seconds)
{
    // esp_deep_sleep(1000000LL * seconds);
    nvsSaveU32(USER_NAMESPACE_0, NVS_READWRITE, NVS_SLEEP_TIME, seconds);
}

void sysGetSleepTime(uint32_t *seconds)
{
    nvsLoadU32(USER_NAMESPACE_0, NVS_READWRITE, NVS_SLEEP_TIME, seconds);
}

void sysSetSleep()
{
    uint32_t sleepTime;
    sysGetSleepTime(&sleepTime);
    // printf("sleepTime = %d seconds\r\n", sleepTime);
    //睡眠时间需要大于60s
    if (sleepTime < 60)
    {
        esp_deep_sleep(sleepTime * 1000000LL);
    }
}