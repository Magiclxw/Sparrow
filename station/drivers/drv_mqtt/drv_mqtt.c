#include "drv_mqtt.h"
#include "esp_sleep.h"
#include "../../main/systemInfo.h"
#include "drv_jsonHandler.h"
#include "string.h"
#include "ui.h"


static const char *TAG = "MQTT station";

QueueHandle_t mqttQueueHandle = NULL;

esp_mqtt_client_handle_t client = NULL;

static uint16_t s_powerOnTimes = 0;

static MqttDataStruct s_mqttRecData = {0};

static size_t mqttGetBrokerAddr(char * addr);
static size_t mqttGetBrokerUsername(char * username);
static size_t mqttGetBrokerPassword(char * password);


static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}

/*
 * @brief Event handler registered to receive MQTT events
 *
 *  This function is called by the MQTT client event loop.
 *
 * @param handler_args user data registered to the event.
 * @param base Event base for the handler(always MQTT Base in this example).
 * @param event_id The id for the received event.
 * @param event_data The data for the event, esp_mqtt_event_handle_t.
 */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32, base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        //订阅app端发送的保留设置数据
        msg_id = esp_mqtt_client_subscribe(client, MQTT_TOPIC_APP_RETAINED_SETTINGS, 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_subscribe(client, MQTT_TOPIC_APP_DISRETAINED_SETTINGS, 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_subscribe(client, MQTT_TOPIC_DEVICE_NOTIFICATION, 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        // msg_id = esp_mqtt_client_subscribe(client, MQTT_TOPIC_DEVICE_DISRETAINED_STATE, 0);
        // ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        // int times = 0;
        // sysInfoGetPowerOnTimes(&times);
        // char powerOnTimes[5] = {0};
        // sprintf(powerOnTimes, "%d", times);
        // msg_id = esp_mqtt_client_publish(client, "/topic/power_on_times", powerOnTimes, 5, 0, 1);

        // vTaskDelay(pdMS_TO_TICKS(100));

        xSemaphoreGive(preStartupSemaphore);
        
        break;


    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        msg_id = esp_mqtt_client_publish(client, MQTT_TOPIC_DEVICE_DISRETAINED_STATE, "1", 0, 0, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");

        s_mqttRecData.topicLength = event->topic_len;
        memcpy(&s_mqttRecData.topic, event->topic, event->topic_len);
        s_mqttRecData.dataLength = event->data_len;
        memcpy(&s_mqttRecData.data, event->data, event->data_len);

        xQueueSend(mqttQueueHandle, &s_mqttRecData, 0);

        memset(&s_mqttRecData, 0 , sizeof(s_mqttRecData));
        
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
            ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));

        }
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

esp_err_t mqttSetBrokerAddr(const char *addr)
{
    esp_err_t ret = nvsSaveValue(USER_NAMESPACE_0, NVS_READWRITE, SERVER_ADDRESS, addr);
    return ret;
}

esp_err_t mqttSetBrokerUsername(const char *username)
{
    esp_err_t ret = nvsSaveValue(USER_NAMESPACE_0, NVS_READWRITE, SERVER_USERNAME, username);
    return ret;
}

esp_err_t mqttSetBrokerPassword(const char *password)
{
    esp_err_t ret = nvsSaveValue(USER_NAMESPACE_0, NVS_READWRITE, SERVER_PASSWORD, password);
    return ret;
}

static size_t mqttGetBrokerAddr(char * addr)
{
    size_t length = 0;

    esp_err_t ret = nvsLoadValue(USER_NAMESPACE_0, NVS_READWRITE, SERVER_ADDRESS, addr, &length);

    if (ret == ESP_OK)
    {
        return length;
    }
    else
    {
        return 0;
    }
}

static size_t mqttGetBrokerUsername(char * username)
{
    size_t length = 0;

    esp_err_t ret = nvsLoadValue(USER_NAMESPACE_0, NVS_READWRITE, SERVER_USERNAME, username, &length);
    
    if (ret == ESP_OK)
    {
        return length;
    }
    else
    {
        return 0;
    }
}

static size_t mqttGetBrokerPassword(char * password)
{
    size_t length = 0;

    esp_err_t ret = nvsLoadValue(USER_NAMESPACE_0, NVS_READWRITE, SERVER_PASSWORD, password, &length);
    
    if (ret == ESP_OK)
    {
        return length;
    }
    else
    {
        return 0;
    }
}


void initMqtt(void)
{
    char s_addr[256] = {0};
    char s_username[256] = {0};
    char s_password[256] = {0};

    uint8_t addrLen = 0;
    uint8_t usernameLen = 0;
    uint8_t passwordLen = 0;

    addrLen = mqttGetBrokerAddr(s_addr);
    usernameLen = mqttGetBrokerUsername(s_username);
    passwordLen = mqttGetBrokerPassword(s_password);

    printf("addrLen: %d, usernameLen: %d, passwordLen: %d\r\n", addrLen, usernameLen, passwordLen);
    printf("addr: %s, username: %s, password: %s\r\n", s_addr, s_username, s_password);

    if (addrLen != 0 && usernameLen != 0 && passwordLen != 0)
    {
        // printf("addr: %s, username: %s, password: %s\r\n", s_addr, s_username, s_password);

        const esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = s_addr,
        .credentials = {
            .username = s_username,
        .authentication = {
            .password = s_password,
        // .broker.address.uri = "mqtts://j1aa1aff.ala.cn-hangzhou.emqxsl.cn:8883",
        // .credentials = {
        //     .username = "test1",
        // .authentication = {
        //     .password = "asd13579",
                },
            },
        // 配置遗嘱消息
        .session.last_will = 
        {
            .topic = MQTT_TOPIC_DEVICE_DISRETAINED_STATE,
            .msg = "0",
            .qos = 0,
            .retain = 1,
            .msg_len = strlen(MQTT_TOPIC_DEVICE_DISRETAINED_STATE),
        }

        };

        ESP_LOGI(TAG, "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
        client = esp_mqtt_client_init(&mqtt_cfg);
        /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
        esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
        esp_mqtt_client_start(client);
    }
}

int drvMqttSendRetainedState()
{
    char retainedbuffer[1024];

    getDeviceRetainedState(retainedbuffer);

    printf("retained data = %s\n",retainedbuffer);

    return esp_mqtt_client_publish(client, MQTT_TOPIC_DEVICE_RETAINED_STATE, retainedbuffer, 0, 0, 1);
}

int drvMqttSendAppRetainedSettings()
{
    char retainedbuffer[1024];
    jsonGenerateAppRetainedSettings(retainedbuffer);
    return esp_mqtt_client_publish(client, MQTT_TOPIC_APP_RETAINED_SETTINGS, retainedbuffer, 0, 0, 1);
}