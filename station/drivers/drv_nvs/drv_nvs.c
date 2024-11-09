#include "drv_nvs.h"

static nvs_handle_t s_nvsHandle;

esp_err_t initNvs(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    return ret;
}

esp_err_t nvsOpen(const char* namespace_name, nvs_open_mode_t open_mode)
{
    return nvs_open(namespace_name, open_mode, &s_nvsHandle);
}

void nvsClose()
{
    nvs_close(s_nvsHandle);
}

esp_err_t nvsGetStr(const char* key, char* out_value, size_t* length)
{
    return nvs_get_str(s_nvsHandle, key, out_value, length);
}

esp_err_t nvsSetStr(const char* key, const char* value)
{
    return nvs_set_str(s_nvsHandle, key, value);
}

esp_err_t nvsGetU32(const char* key, uint32_t* out_value)
{
    return nvs_get_u32(s_nvsHandle, key, out_value);
}

esp_err_t nvsSetU32(const char* key, uint32_t value)
{
    return nvs_set_u32(s_nvsHandle, key, value);
}

esp_err_t nvsGetU64(const char* key, uint64_t* out_value)
{
    return nvs_get_u64(s_nvsHandle, key, out_value);
}

esp_err_t nvsSetU64(const char* key, uint64_t value)
{
    return nvs_set_u64(s_nvsHandle, key, value);
}

esp_err_t nvsCommit()
{
    return nvs_commit(s_nvsHandle);
}

esp_err_t nvsEraseKey(const char* key)
{
    return nvs_erase_key(s_nvsHandle, key);
}

esp_err_t nvsSaveValue(const char* namespace_name, nvs_open_mode_t open_mode, const char* key, const char* value)
{
    esp_err_t ret = ESP_OK;

    ret = nvsOpen(namespace_name, open_mode);

    if(ret != ESP_OK) return ret;

    ret = nvsSetStr(key, value);

    if(ret != ESP_OK) return ret;

    ret = nvsCommit();

    if(ret != ESP_OK) return ret;

    nvsClose();
    
    return ret;
}

esp_err_t nvsLoadValue(const char* namespace_name, nvs_open_mode_t open_mode, const char* key, char* out_value, size_t *length)
{
    esp_err_t ret = ESP_OK;

    ret = nvsOpen(namespace_name, open_mode);

    if(ret != ESP_OK) return ret;

    ret = nvsGetStr(key, NULL, length);

    if (ret == ESP_OK)
    {
        ret = nvsGetStr(key, out_value, length);
    }

    nvsClose();

    return ret;
}

esp_err_t nvsSaveU32(const char* namespace_name, nvs_open_mode_t open_mode, const char* key, const uint32_t value)
{
    esp_err_t ret = ESP_OK;

    ret = nvsOpen(namespace_name, open_mode);

    if(ret != ESP_OK) return ret;

    ret = nvsSetU32(key, value);

    if(ret != ESP_OK) return ret;

    ret = nvsCommit();

    if(ret != ESP_OK) return ret;

    nvsClose();
    
    return ret;
}

esp_err_t nvsLoadU32(const char* namespace_name, nvs_open_mode_t open_mode, const char* key, uint32_t *value)
{
    esp_err_t ret = ESP_OK;

    ret = nvsOpen(namespace_name, open_mode);

    if(ret != ESP_OK) return ret;

    ret = nvsGetU32(key, value);

    nvsClose();

    return ret;
}

esp_err_t nvsSaveU64(const char* namespace_name, nvs_open_mode_t open_mode, const char* key, const uint64_t value)
{
    esp_err_t ret = ESP_OK;

    ret = nvsOpen(namespace_name, open_mode);

    if(ret != ESP_OK) return ret;

    ret = nvsSetU64(key, value);

    if(ret != ESP_OK) return ret;

    ret = nvsCommit();

    if(ret != ESP_OK) return ret;

    nvsClose();
    
    return ret;
}

esp_err_t nvsLoadU64(const char* namespace_name, nvs_open_mode_t open_mode, const char* key, uint64_t *value)
{
    esp_err_t ret = ESP_OK;

    ret = nvsOpen(namespace_name, open_mode);

    if(ret != ESP_OK) return ret;

    ret = nvsGetU64(key, value);

    nvsClose();

    return ret;
}

esp_err_t nvsSaveBlobData(const char* namespace_name, nvs_open_mode_t open_mode, const char* key, uint8_t *out_value, size_t length)
{
    esp_err_t ret = ESP_OK;

    ret = nvsOpen(namespace_name, open_mode);

    ret = nvs_set_blob(s_nvsHandle, key, out_value, length);

    ret = nvs_commit(s_nvsHandle);
    if (ret != ESP_OK) return ret;

    // Close
    nvs_close(s_nvsHandle);

    return ret;
}

esp_err_t nvsLoadBlobData(const char* namespace_name, nvs_open_mode_t open_mode, const char* key, uint8_t *out_value)
{
    esp_err_t ret = ESP_OK;

    ret = nvsOpen(namespace_name, open_mode);
    
    if(ret != ESP_OK) return ret;

    size_t length = 0;

    ret = nvs_get_blob(s_nvsHandle, key, NULL, &length);

    if (ret != ESP_OK && ret != ESP_ERR_NVS_NOT_FOUND) return ret;

    if (length > 0)
    {
        key = malloc(length);

        ret = nvs_get_blob(s_nvsHandle, key, out_value, &length);
    }

    // Close
    nvs_close(s_nvsHandle);

    return ret;
}

esp_err_t nvsSaveWakeupInterval(uint64_t interval)
{
    esp_err_t ret = nvsSaveU64(USER_NAMESPACE_0, NVS_READWRITE, NVS_WAKEUP_INTERVAL, interval);
    return ret;
}

esp_err_t nvsLoadWakeupInterval(uint64_t *out_value)
{
    esp_err_t ret = ESP_OK;

    ret = nvsLoadU64(USER_NAMESPACE_0, NVS_READWRITE, NVS_WAKEUP_INTERVAL, out_value);

    return ret;
}

esp_err_t nvsSavePowerCtrl(uint32_t ctrl)
{
    esp_err_t ret = nvsSaveU32(USER_NAMESPACE_0, NVS_READWRITE, NVS_POWER_CTRL, ctrl);
    return ret;
}

esp_err_t nvsLoadPowerCtrl(uint32_t *out_value)
{
    esp_err_t ret = ESP_OK;
    
    return ret;
}

esp_err_t nvsSavePcPassword(char* password)
{
    esp_err_t ret = nvsSaveValue(USER_NAMESPACE_0, NVS_READWRITE, NVS_PC_PASSWORD, password);
    return ret;
}

esp_err_t nvsSavePcPasswordCtrl(uint32_t ctrl)
{
    esp_err_t ret = nvsSaveU32(USER_NAMESPACE_0, NVS_READWRITE, NVS_PC_PASSWORD_CTRL, ctrl);
    return ret;
}

esp_err_t nvsSavePcPasswordWait(uint32_t waitTime)
{
    esp_err_t ret = nvsSaveU32(USER_NAMESPACE_0, NVS_READWRITE, NVS_PC_PASSWORD_WAIT, waitTime);
    return ret;
}

esp_err_t nvsSaveLedCtrl(uint32_t ctrl)
{
    esp_err_t ret = nvsSaveU32(USER_NAMESPACE_0, NVS_READWRITE, NVS_LED_CTRL, ctrl);
    return ret;
}

esp_err_t nvsSaveToolsTokenCtrl(uint32_t ctrl)
{
    esp_err_t ret = nvsSaveU32(USER_NAMESPACE_0, NVS_READWRITE, TOOLS_TOKEN_CTRL, ctrl);
    return ret;
}

esp_err_t nvsSaveIdleAngle(uint32_t angle)
{
    esp_err_t ret = nvsSaveU32(USER_NAMESPACE_0, NVS_READWRITE, NVS_SERVO_IDLE_ANGLE, angle);
    return ret;
}

esp_err_t nvsSavePosAngle(uint32_t angle)
{
    esp_err_t ret = nvsSaveU32(USER_NAMESPACE_0, NVS_READWRITE, NVS_SERVO_POS_ANGLE, angle);
    return ret;
}

esp_err_t nvsSaveNegAngle(uint32_t angle)
{
    esp_err_t ret = nvsSaveU32(USER_NAMESPACE_0, NVS_READWRITE, NVS_SERVO_NEG_ANGLE, angle);
    return ret;
}

esp_err_t nvsSaveBackgroundIndex(uint32_t index)
{
    esp_err_t ret = nvsSaveU32(USER_NAMESPACE_0, NVS_READWRITE, NVS_KEY_BACKGROUND, index);
    return ret;
}

esp_err_t nvsLoadBackgroundIndex(uint32_t *out_value)
{
    esp_err_t ret = ESP_OK;

    ret = nvsLoadU32(USER_NAMESPACE_0, NVS_READWRITE, NVS_KEY_BACKGROUND, out_value);

    return ret;
}