#include "drv_http.h"
#include "esp_http_client.h"
#include "esp_netif.h"
#include "esp_crt_bundle.h"
#include "esp_tls.h"
#include "drv_nvs.h"
#include "../../sys_config.h"


static const char *TAG = "HTTP_CLIENT";
static char weatherBuffer[MAX_HTTP_WEATHER_RECV_BUFFER] = {0};
static char bilibiliBuffer[MAX_HTTP_BILIBILI_RECV_BUFFER] = {0};

static char s_weatherUrl[256] = {0};
static char s_bilibiliUrl[256] = {0};

static esp_http_client_handle_t s_weatherClient = NULL;
static esp_http_client_handle_t s_bilibiliClient = NULL;

static size_t s_weatherGetUrl();
static size_t s_bilibiliGetUrl();

static char jsonWeatherIdData[50] = {0};
static char jsonWeatherNameData[50] = {0};
static char jsonWeatheCountryData[50] = {0};
static char jsonWeathePathData[50] = {0};
static char jsonWeatheTimeZoneData[50] = {0};
static char jsonWeatheOffsetData[50] = {0};

static char jsonWeatheTextData[10] = {0};
static char jsonWeatheCodeData[10] = {0};
static char jsonWeatheTemperatureData[10] = {0};

static char jsonBilibiliData[10] = {0};

static int jsonBilibiliCodeData = 0;
static char jsonBilibiliMessageData[10] = {0};
static int jsonBilibiliTtlData = 0;
static char jsonBilibiliDataData[10] = {0};

static int jsonBilibiliMidData = 0;
static int jsonBilibiliFollowingData = 0;
static int jsonBilibiliWhisperData = 0;
static int jsonBilibiliBlackData = 0;
static int jsonBilibiliFollowerData = 0;

esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    static char *output_buffer;  // Buffer to store response of http request from event handler
    static int output_len;       // Stores number of bytes read
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            /*
             *  Check for chunked encoding is added as the URL for chunked encoding used in this example returns binary data.
             *  However, event handler can also be used in case chunked encoding is used.
             */
            if (!esp_http_client_is_chunked_response(evt->client)) {
                // If user_data buffer is configured, copy the response into the buffer
                int copy_len = 0;
                if (evt->user_data) {
                    copy_len = MIN(evt->data_len, (MAX_HTTP_OUTPUT_BUFFER - output_len));
                    if (copy_len) {
                        memcpy(evt->user_data + output_len, evt->data, copy_len);
                    }
                } else {
                    const int buffer_len = esp_http_client_get_content_length(evt->client);
                    if (output_buffer == NULL) {
                        output_buffer = (char *) malloc(buffer_len);
                        output_len = 0;
                        if (output_buffer == NULL) {
                            ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
                            return ESP_FAIL;
                        }
                    }
                    copy_len = MIN(evt->data_len, (buffer_len - output_len));
                    if (copy_len) {
                        memcpy(output_buffer + output_len, evt->data, copy_len);
                    }
                }
                output_len += copy_len;
            }

            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
            if (output_buffer != NULL) {
                // Response is accumulated in output_buffer. Uncomment the below line to print the accumulated response
                // ESP_LOG_BUFFER_HEX(TAG, output_buffer, output_len);
                free(output_buffer);
                output_buffer = NULL;
            }
            output_len = 0;
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            int mbedtls_err = 0;
            esp_err_t err = esp_tls_get_and_clear_last_error((esp_tls_error_handle_t)evt->data, &mbedtls_err, NULL);
            if (err != 0) {
                ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
                ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
            }
            if (output_buffer != NULL) {
                free(output_buffer);
                output_buffer = NULL;
            }
            output_len = 0;
            break;
        case HTTP_EVENT_REDIRECT:
            ESP_LOGD(TAG, "HTTP_EVENT_REDIRECT");
            esp_http_client_set_header(evt->client, "From", "user@example.com");
            esp_http_client_set_header(evt->client, "Accept", "text/html");
            esp_http_client_set_redirection(evt->client);
            break;
    }
    return ESP_OK;
}

esp_err_t initHttp(void)
{
    static char* weatherKey = NULL;
    static char* bilibiliMid = NULL;
    char weatherData[256] = WEATHER_KEY_HEAD;
    char bilibiliData[256] = BILIBILI_VMID;
    uint8_t weatherUrlLength = 0;
    uint8_t bilibiliUrlLength = 0;

    weatherUrlLength = s_weatherGetUrl();
    bilibiliUrlLength = s_bilibiliGetUrl();

    if (weatherUrlLength > 1)
    {
        weatherKey = pvPortMalloc(weatherUrlLength);

        memcpy(weatherKey, s_weatherUrl, weatherUrlLength);

        strcat(weatherData, weatherKey);

        strcat(weatherData, WEATHER_KEY_TAIL);

        printf("weatherUrl: %s\n", weatherData);

        esp_http_client_config_t weatherConfig = {
        .url = weatherData,
        .method = HTTP_METHOD_GET,
        .user_data = weatherBuffer,
        .event_handler = _http_event_handler,
        .crt_bundle_attach = esp_crt_bundle_attach,
        };

        s_weatherClient = esp_http_client_init(&weatherConfig);

        vPortFree(weatherKey);
    }
    else
    {
        // esp_http_client_config_t weatherConfig = {
        // .url = "https://api.seniverse.com/v3/weather/now.json?key=S5XFRDn4rnHvwDwu8&location=ip&language=zh-Hans&unit=c",
        // .method = HTTP_METHOD_GET,
        // .user_data = weatherBuffer,
        // .event_handler = _http_event_handler,
        // .crt_bundle_attach = esp_crt_bundle_attach,
        // };

        // s_weatherClient = esp_http_client_init(&weatherConfig);

        return ESP_FAIL;
    }

    if (bilibiliUrlLength > 1)
    {
        bilibiliMid = pvPortMalloc(bilibiliUrlLength);

        memcpy(bilibiliMid, s_bilibiliUrl, bilibiliUrlLength);

        strcat(bilibiliData, bilibiliMid);

        printf("bilibiliUrl: %s\n", bilibiliData);

        esp_http_client_config_t bilibiliConfig = {
            .url = bilibiliData,
            .method = HTTP_METHOD_GET,
            .user_data = bilibiliBuffer,
            .event_handler = _http_event_handler,
            .crt_bundle_attach = esp_crt_bundle_attach,
        };
        s_bilibiliClient = esp_http_client_init(&bilibiliConfig);

        vPortFree(bilibiliMid);
    }
    else
    {
        // esp_http_client_config_t bilibiliConfig = {
        // .url = "https://api.bilibili.com/x/relation/stat?vmid=151971283",
        // .method = HTTP_METHOD_GET,
        // .user_data = bilibiliBuffer,
        // .event_handler = _http_event_handler,
        // .crt_bundle_attach = esp_crt_bundle_attach,
        // };
        // s_bilibiliClient = esp_http_client_init(&bilibiliConfig);

        return ESP_FAIL;
    }

    return ESP_OK;
}

static uint8_t flag = 0;

void httpRefreshData()
{
    esp_err_t err = ESP_OK;

    if (flag == 0)
    {
        if (s_weatherClient != NULL)
        {
            err = esp_http_client_perform(s_weatherClient);

            if (err == ESP_OK) {
                ESP_LOGI(TAG, "HTTPS Status = %d, content_length = %"PRIu64,
                        esp_http_client_get_status_code(s_weatherClient),
                        esp_http_client_get_content_length(s_weatherClient));
            } else {
                ESP_LOGE(TAG, "Error perform http request %s", esp_err_to_name(err));
            }

            flag = 1;

            printf("HTTP weather response : %s\n", weatherBuffer);

            esp_http_client_close(s_weatherClient);
        }
        
    }
    else
    {
        if (s_bilibiliClient != NULL)
        {
            err = esp_http_client_perform(s_bilibiliClient);

            if (err == ESP_OK) {
                ESP_LOGI(TAG, "HTTPS Status = %d, content_length = %"PRIu64,
                        esp_http_client_get_status_code(s_bilibiliClient),
                        esp_http_client_get_content_length(s_bilibiliClient));
            } else {
                ESP_LOGE(TAG, "Error perform http request %s", esp_err_to_name(err));
            }

            flag = 0;

            printf("HTTP bilibili response : %s\n", bilibiliBuffer);

            esp_http_client_close(s_bilibiliClient);
        }
    }
}

esp_err_t jsonGetWeatherData()
{
    // {
    //     "results":
    //     [
    //         {
    //             "location":
    //             {
    //                 "id":"WTTDPCGXTWUS",
    //                 "name":"苏州",
    //                 "country":"CN",
    //                 "path":"苏州,苏州,江苏,中国",
    //                 "timezone":"Asia/Shanghai",
    //                 "timezone_offset":"+08:00"
    //             },
    //             "now":
    //             {
    //                 "text":"晴",
    //                 "code":"1",
    //                 "temperature":"28"
    //             },
    //             "last_update":"2024-09-08T22:46:52+08:00"
    //             }
    //     ]
    // }
     cJSON *jsonWeatherFormat = NULL;

     cJSON *jsonWeatherSubData = NULL;

     cJSON *jsonWeatherResult = NULL;

     cJSON *jsonWeatherLocation = NULL;
     cJSON *jsonWeatherNow = NULL;

     cJSON *jsonWeatherId = NULL;
     cJSON *jsonWeatherName = NULL;
     cJSON *jsonWeatheCountry = NULL;
     cJSON *jsonWeathePath = NULL;
     cJSON *jsonWeatheTimeZone = NULL;
     cJSON *jsonWeatheOffset = NULL;

     cJSON *jsonWeatheText = NULL;
     cJSON *jsonWeatheCode = NULL;
     cJSON *jsonWeatheTemperature = NULL;

    jsonWeatherFormat = cJSON_Parse(weatherBuffer);

    if (jsonWeatherFormat == NULL)
    {
        return ESP_FAIL;
    }

    jsonWeatherResult = cJSON_GetObjectItem(jsonWeatherFormat, "results");

    if (jsonWeatherResult == NULL)
    {
        return ESP_FAIL;
    }

    // resultSize = cJSON_GetArraySize(jsonWeatherResult);

    jsonWeatherSubData = cJSON_GetArrayItem(jsonWeatherResult, 0);

    if (jsonWeatherSubData == NULL)
    {
        return ESP_FAIL;
    }


    jsonWeatherLocation = cJSON_GetObjectItem(jsonWeatherSubData, "location");

    if (jsonWeatherLocation == NULL)
    {
        return ESP_FAIL;
    }

    jsonWeatherNow = cJSON_GetObjectItem(jsonWeatherSubData, "now");

    if (jsonWeatherNow == NULL)
    {
        return ESP_FAIL;
    }

    jsonWeatherId = cJSON_GetObjectItem(jsonWeatherLocation, "id");
    jsonWeatherName = cJSON_GetObjectItem(jsonWeatherLocation, "name");
    jsonWeatheCountry = cJSON_GetObjectItem(jsonWeatherLocation, "country");
    jsonWeathePath = cJSON_GetObjectItem(jsonWeatherLocation, "path");
    jsonWeatheTimeZone = cJSON_GetObjectItem(jsonWeatherLocation, "timezone");
    jsonWeatheOffset = cJSON_GetObjectItem(jsonWeatherLocation, "timezone_offset");

    jsonWeatheText = cJSON_GetObjectItem(jsonWeatherNow, "text");
    jsonWeatheCode = cJSON_GetObjectItem(jsonWeatherNow, "code");
    jsonWeatheTemperature = cJSON_GetObjectItem(jsonWeatherNow, "temperature");

    
    strcpy(jsonWeatherIdData, jsonWeatherId->valuestring);
    strcpy(jsonWeatherNameData, jsonWeatherName->valuestring);
    strcpy(jsonWeatheCountryData, jsonWeatheCountry->valuestring);
    strcpy(jsonWeathePathData, jsonWeathePath->valuestring);
    strcpy(jsonWeatheTimeZoneData, jsonWeatheTimeZone->valuestring);
    strcpy(jsonWeatheOffsetData, jsonWeatheOffset->valuestring);
    strcpy(jsonWeatheTextData, jsonWeatheText->valuestring);
    strcpy(jsonWeatheCodeData, jsonWeatheCode->valuestring);
    strcpy(jsonWeatheTemperatureData, jsonWeatheTemperature->valuestring);


    cJSON_Delete(jsonWeatherFormat);

    // printf("jsonWeatherId: %s\n", jsonWeatherId->valuestring);
    // printf("jsonWeatherFormat: %s\n", jsonWeatherName->valuestring);
    // printf("jsonWeatheCountry: %s\n", jsonWeatheCountry->valuestring);
    // printf("jsonWeathePath: %s\n", jsonWeathePath->valuestring);
    // printf("jsonWeatheTimeZone: %s\n", jsonWeatheTimeZone->valuestring);
    // printf("jsonWeatheOffset: %s\n", jsonWeatheOffset->valuestring);

    // printf("jsonWeatheText: %s\n", jsonWeatheText->valuestring);
    // printf("jsonWeatheCode: %s\n", jsonWeatheCode->valuestring);
    // printf("jsonWeatheTemperature: %s\n", jsonWeatheTemperature->valuestring);
    return ESP_OK;
}

esp_err_t jsonGetBilibiliData()
{
    // {
    // "code": 0,
    // "message": "0",
    // "ttl": 1,
    // "data": {
    //     "mid": 151971283,
    //     "following": 51,
    //     "whisper": 0,
    //     "black": 0,
    //     "follower": 191
    //     }
    // }

     cJSON *jsonBilibiliFormat = NULL;

     cJSON *jsonBilibiliCode = NULL;
     cJSON *jsonBilibiliMessage = NULL;
     cJSON *jsonBilibiliTtl = NULL;
     cJSON *jsonBilibiliData = NULL;

     cJSON *jsonBilibiliMid = NULL;
     cJSON *jsonBilibiliFollowing = NULL;
     cJSON *jsonBilibiliWhisper = NULL;
     cJSON *jsonBilibiliBlack = NULL;
     cJSON *jsonBilibiliFollower = NULL;

    jsonBilibiliFormat = cJSON_Parse(bilibiliBuffer);

    if (jsonBilibiliFormat == NULL)
    {
        return ESP_FAIL;
    }

    jsonBilibiliCode = cJSON_GetObjectItem(jsonBilibiliFormat, "code");

    if (jsonBilibiliCode == NULL)
    {
        return ESP_FAIL;
    }

    jsonBilibiliMessage = cJSON_GetObjectItem(jsonBilibiliFormat, "message");

    if (jsonBilibiliMessage == NULL)
    {
        return ESP_FAIL;
    }

    jsonBilibiliTtl = cJSON_GetObjectItem(jsonBilibiliFormat, "ttl");

    if (jsonBilibiliTtl == NULL)
    {
        return ESP_FAIL;
    }

    jsonBilibiliData = cJSON_GetObjectItem(jsonBilibiliFormat, "data");

    if (jsonBilibiliData == NULL)
    {
        return ESP_FAIL;
    }

    jsonBilibiliMid = cJSON_GetObjectItem(jsonBilibiliData, "mid");

    if (jsonBilibiliMid == NULL)
    {
        return ESP_FAIL;
    }

    jsonBilibiliFollowing = cJSON_GetObjectItem(jsonBilibiliData, "following");

    if (jsonBilibiliFollowing == NULL)
    {
        return ESP_FAIL;
    }

    jsonBilibiliWhisper = cJSON_GetObjectItem(jsonBilibiliData, "whisper");

    if (jsonBilibiliWhisper == NULL)
    {
        return ESP_FAIL;
    }

    jsonBilibiliBlack = cJSON_GetObjectItem(jsonBilibiliData, "black");

    if (jsonBilibiliBlack == NULL)
    {
        return ESP_FAIL;
    }

    jsonBilibiliFollower = cJSON_GetObjectItem(jsonBilibiliData, "follower");

    if (jsonBilibiliFollower == NULL)
    {
        return ESP_FAIL;
    }

    jsonBilibiliCodeData = jsonBilibiliCode->valueint;
    strcpy(jsonBilibiliMessageData, jsonBilibiliMessage->valuestring);
    jsonBilibiliTtlData = jsonBilibiliTtl->valueint;
    jsonBilibiliMidData = jsonBilibiliMid->valueint;
    jsonBilibiliFollowingData = jsonBilibiliFollowing->valueint;
    jsonBilibiliWhisperData = jsonBilibiliWhisper->valueint;
    jsonBilibiliBlackData = jsonBilibiliBlack->valueint;
    jsonBilibiliFollowerData = jsonBilibiliFollower->valueint;

    cJSON_Delete(jsonBilibiliFormat);
    // printf("jsonBilibiliCode = %d\n", jsonBilibiliCode->valueint);
    // printf("jsonBilibiliMessage = %s\n", jsonBilibiliMessage->valuestring);
    // printf("jsonBilibiliTtl = %d\n", jsonBilibiliTtl->valueint);
    // printf("jsonBilibiliMid = %d\n", jsonBilibiliMid->valueint);
    // printf("jsonBilibiliFollowing = %d\n", jsonBilibiliFollowing->valueint);
    // printf("jsonBilibiliWhisper = %d\n", jsonBilibiliWhisper->valueint);
    // printf("jsonBilibiliBlack = %d\n", jsonBilibiliBlack->valueint);
    // printf("jsonBilibiliFollower = %d\n", jsonBilibiliFollower->valueint);

    
    
    return ESP_OK;
}

// void drvHttpDeleteJsonData()
// {
//     cJSON_Delete(jsonWeatherFormat);
//     cJSON_Delete(jsonBilibiliFormat);
// }

esp_err_t jsonGetBilibiliFollowing(int *buffer)
{
    *buffer = jsonBilibiliFollowingData;
    return ESP_OK;
}

esp_err_t jsonGetBilibiliFollower(int *buffer)
{
    *buffer = jsonBilibiliFollowerData;
    return ESP_OK;
}

esp_err_t drvHttpGetWeatherName(char *buffer)
{
    if (strlen(jsonWeatherNameData) != 0)
    {
        strcpy(buffer, jsonWeatherNameData);
        return ESP_OK;
    }
    return ESP_FAIL;
}

esp_err_t drvHttpGetWeatherText(char *buffer)
{
    if (strlen(jsonWeatheTextData) != 0)
    {
        strcpy(buffer, jsonWeatheTextData);
        return ESP_OK;
    }
    return ESP_FAIL;
}

esp_err_t drvHttpGetWeatherCode(char *buffer)
{
    if (strlen(jsonWeatheCodeData) != 0)
    {
        strcpy(buffer, jsonWeatheCodeData);
        return ESP_OK;
    }
    return ESP_FAIL;
}

esp_err_t drvHttpGetWeatherTemperature(char *buffer)
{
    if (strlen(jsonWeatheTemperatureData) != 0)
    {
        strcpy(buffer, jsonWeatheTemperatureData);
        return ESP_OK;
    }
    return ESP_FAIL;
}


esp_err_t httpSetWeatherUrl(const char *url)
{
    esp_err_t ret = nvsSaveValue(USER_NAMESPACE_0, NVS_READWRITE, NVS_WEATHER, url);
    return ret;
}

esp_err_t httpSetBilibiliUrl(const char *url)
{
    esp_err_t ret = nvsSaveValue(USER_NAMESPACE_0, NVS_READWRITE, NVS_BILIBILI, url);
    return ret;
}

static size_t s_weatherGetUrl()
{
    size_t length = 0;

    esp_err_t ret = nvsLoadValue(USER_NAMESPACE_0, NVS_READWRITE, NVS_WEATHER, s_weatherUrl, &length);

    printf("Weather : %s\r\n", s_weatherUrl);

    if (ret == ESP_OK)
    {
        return length;
    }
    else
    {
        return 0;
    }
}

static size_t s_bilibiliGetUrl()
{
    size_t length = 0;

    esp_err_t ret = nvsLoadValue(USER_NAMESPACE_0, NVS_READWRITE, NVS_BILIBILI, s_bilibiliUrl, &length);

    printf("Bilibili : %s\r\n", s_bilibiliUrl);

    if (ret == ESP_OK)
    {
        return length;
    }
    else
    {
        return 0;
    }
}