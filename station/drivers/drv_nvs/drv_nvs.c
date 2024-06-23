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
    return nvs_set_str(s_nvsHandle, key,value);
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
}