#include "systemInfo.h"

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