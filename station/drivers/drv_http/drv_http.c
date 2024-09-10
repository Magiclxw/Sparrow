#include "drv_http.h"
#include "esp_http_client.h"
#include "esp_netif.h"
#include "esp_crt_bundle.h"
#include "esp_tls.h"
#include "../../sys_config.h"


static const char *TAG = "HTTP_CLIENT";
static char weatherBuffer[MAX_HTTP_WEATHER_RECV_BUFFER] = {0};
static char bilibiliBuffer[MAX_HTTP_BILIBILI_RECV_BUFFER] = {0};

static esp_http_client_handle_t s_weatherClient = NULL;
static esp_http_client_handle_t s_bilibiliClient = NULL;

static cJSON *jsonWeatherFormat = NULL;

static cJSON *jsonWeatherSubData = NULL;

static cJSON *jsonWeatherResult = NULL;

static cJSON *jsonWeatherLocation = NULL;
static cJSON *jsonWeatherNow = NULL;

static cJSON *jsonWeatherId = NULL;
static cJSON *jsonWeatherName = NULL;
static cJSON *jsonWeatheCountry = NULL;
static cJSON *jsonWeathePath = NULL;
static cJSON *jsonWeatheTimeZone = NULL;
static cJSON *jsonWeatheOffset = NULL;

static cJSON *jsonWeatheText = NULL;
static cJSON *jsonWeatheCode = NULL;
static cJSON *jsonWeatheTemperature = NULL;

static cJSON *jsonBilibiliFormat = NULL;

static cJSON *jsonBilibiliCode = NULL;
static cJSON *jsonBilibiliMessage = NULL;
static cJSON *jsonBilibiliTtl = NULL;
static cJSON *jsonBilibiliData = NULL;

static cJSON *jsonBilibiliMid = NULL;
static cJSON *jsonBilibiliFollowing = NULL;
static cJSON *jsonBilibiliWhisper = NULL;
static cJSON *jsonBilibiliBlack = NULL;
static cJSON *jsonBilibiliFollower = NULL;

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

void initHttp(void)
{
    printf("init http\r\n");
    esp_http_client_config_t weatherConfig = {
        .url = "https://api.seniverse.com/v3/weather/now.json?key=S5XFRDn4rnHvwDwu8&location=ip&language=zh-Hans&unit=c",
        .method = HTTP_METHOD_GET,
        .user_data = weatherBuffer,
        .event_handler = _http_event_handler,
        .crt_bundle_attach = esp_crt_bundle_attach,
    };
    s_weatherClient = esp_http_client_init(&weatherConfig);

     esp_http_client_config_t bilibiliConfig = {
        .url = "https://api.bilibili.com/x/relation/stat?vmid=151971283",
        .method = HTTP_METHOD_GET,
        .user_data = bilibiliBuffer,
        .event_handler = _http_event_handler,
        .crt_bundle_attach = esp_crt_bundle_attach,
    };
    s_bilibiliClient = esp_http_client_init(&bilibiliConfig);

    // esp_err_t err = esp_http_client_perform(s_bilibiliClient);
    
    // if (err == ESP_OK) {
    //     ESP_LOGI(TAG, "HTTPS Status = %d, content_length = %"PRIu64,
    //             esp_http_client_get_status_code(s_bilibiliClient),
    //             esp_http_client_get_content_length(s_bilibiliClient));
    // } else {
    //     ESP_LOGE(TAG, "Error perform http request %s", esp_err_to_name(err));
    // }


    // err = esp_http_client_perform(s_weatherClient);

    // if (err == ESP_OK) {
    //     ESP_LOGI(TAG, "HTTPS Status = %d, content_length = %"PRIu64,
    //             esp_http_client_get_status_code(s_weatherClient),
    //             esp_http_client_get_content_length(s_weatherClient));
    // } else {
    //     ESP_LOGE(TAG, "Error perform http request %s", esp_err_to_name(err));
    // }

    // ESP_LOG_BUFFER_HEX(TAG, local_response_buffer, strlen(local_response_buffer));
    // printf("HTTP Response : %s\n", local_response_buffer);
    // esp_http_client_cleanup(client);
}

static uint8_t flag = 0;

void httpRefreshData()
{
    esp_err_t err = ESP_OK;

    // if (flag == 0)
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

        esp_http_client_close(s_weatherClient);

        // printf("HTTP weather response : %s\n", weatherBuffer);
    }
    // else
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

        esp_http_client_close(s_bilibiliClient);

        // printf("HTTP bilibili response : %s\n", bilibiliBuffer);
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

void drvHttpDeleteJsonData()
{
    cJSON_Delete(jsonWeatherFormat);
    cJSON_Delete(jsonBilibiliFormat);
}

esp_err_t jsonGetBilibiliFollowing(int *buffer)
{
    if (jsonBilibiliFollowing == NULL)
    {
        return ESP_FAIL;
    }
    *buffer = jsonBilibiliFollowing->valueint;
    return ESP_OK;
}

esp_err_t jsonGetBilibiliFollower(int *buffer)
{
    if (jsonBilibiliFollower == NULL)
    {
        return ESP_FAIL;
    }
    *buffer = jsonBilibiliFollower->valueint;
    return ESP_OK;
}

esp_err_t drvHttpGetWeatherName(char *buffer)
{
    if (jsonWeatherName == NULL)
    {
        return ESP_FAIL;
    }

    strcpy(buffer, jsonWeatherName->valuestring);

    return ESP_OK;
}

esp_err_t drvHttpGetWeatherText(char *buffer)
{
    if (jsonWeatheText == NULL)
    {
        return ESP_FAIL;
    }
    strcpy(buffer, jsonWeatheText->valuestring);
    return ESP_OK;
}

esp_err_t drvHttpGetWeatherCode(char *buffer)
{
    if (jsonWeatheCode == NULL)
    {
        return ESP_FAIL;
    }
    strcpy(buffer, jsonWeatheCode->valuestring);
    return ESP_OK;
}

esp_err_t drvHttpGetWeatherTemperature(char *buffer)
{
    if (jsonWeatheTemperature == NULL)
    {
        return ESP_FAIL;
    }
    strcpy(buffer, jsonWeatheTemperature->valuestring);
    return ESP_OK;
}