#include "task_http.h"
#include "../../sys_config.h"
#include "drv_http.h"
#include "wifi_station.h"
#include "drv_jsonHandler.h"
#include "ui.h"

TaskHandle_t taskHttpHandle = NULL;

const char* bilibiliFollowerTxt = "bilibili:";

static void httpTask();

int createHttpTask()
{
    xTaskCreate((TaskFunction_t)httpTask,
                (const char*)"httpTask",
                (uint32_t )HTTP_TASK_STACK_SIZE,
                (void *	)NULL,
                (UBaseType_t)HTTP_TASK_PRIORITY,
                &taskHttpHandle);
    return OPERATE_SUCCESS;
}

static void httpTask()
{
    char weatherName[20];
    char weatherText[20];
    char weatherCode[5];
    char weatherTemperature[5];
    char strBilibiliFollower[30];

    int bilibiliFollowing = 0;
    int tempBililiFollowing = 0;
    int bilibiliFollower = 0;


    // httpSetWeatherUrl("0");
    // httpSetBilibiliUrl("0");

    //初始化http
    esp_err_t ret = initHttp();

    if (ret == ESP_FAIL)
    {
        vTaskDelete(NULL);
    }

    while (1)
    {
        //等待wifi连接
        if (wifiGetConnectState())
        {
            httpRefreshData();

            jsonGetWeatherData();
            jsonGetBilibiliData();
            // printf("城市：%s \r\n", name);
            jsonGetBilibiliFollowing(&bilibiliFollowing);
            // printf("bilibiliFollowing = %d\r\n", bilibiliFollowing);
            jsonGetBilibiliFollower(&bilibiliFollower);
            // printf("bilibiliFollower = %d\r\n", bilibiliFollower);
            drvHttpGetWeatherName(weatherName);
            // printf("weatherName = %s\r\n", weatherName);
            drvHttpGetWeatherText(weatherText);
            // printf("weatherText = %s\r\n", weatherText);
            drvHttpGetWeatherCode(weatherCode);
            // printf("weatherCode = %s\r\n", weatherCode);
            drvHttpGetWeatherTemperature(weatherTemperature);
            // printf("weatherTemperature = %s\r\n", weatherTemperature);
            
            ui_Screen_Main_SetWeatherIcon(atoi(weatherCode));
            ui_Screen_Main_SetWeatherTemperature(weatherTemperature);

            if (tempBililiFollowing != bilibiliFollowing)
            {
                strcpy(strBilibiliFollower, bilibiliFollowerTxt);
                itoa(bilibiliFollower, &strBilibiliFollower[9], 10);
                ui_Screen_Main_SetBilibiliFollower(strBilibiliFollower);
                tempBililiFollowing = bilibiliFollowing;
            }
            

            //释放json数据
            // drvHttpDeleteJsonData();
            // ESP_LOGI("TAG", "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
        }
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}
