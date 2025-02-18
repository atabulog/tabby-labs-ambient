#include "NVSManager.h"
#include "esp_log.h"

static const char* TAG = "NVSManager";
static const char* wifi_config_key = "wifi_config";
//private status flag
static bool nvs_manager_ready = false;

void nvs_manager_init(void) 
{
    esp_err_t err = nvs_flash_init();
    //if NVS is full or a new version is found, erase and reinitialize
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    if (err == ESP_OK) 
    {
        nvs_manager_ready = true;
        ESP_LOGI(TAG, "Encrypted NVS initialized.");
    } 
    else 
    {
        ESP_LOGE(TAG, "Failed to initialize encrypted NVS: %s", esp_err_to_name(err));
    }
}

bool nvs_manager_is_ready(void) 
{
    return nvs_manager_ready;
}

esp_err_t nvs_manager_get_wifi_config(wifi_config_t* wifi_config)
{
    return nvs_manager_read(wifi_config_key, wifi_config, sizeof(wifi_config_t));
}

esp_err_t nvs_manager_set_wifi_config(wifi_config_t* wifi_config)
{
    return nvs_manager_write(wifi_config_key, wifi_config);
}



esp_err_t nvs_manager_read(const char* key, void* value, size_t read_size) 
{
    //Open NVS at key location with read/write access
    nvs_handle_t nvs;
    esp_err_t err = nvs_open(key, NVS_READWRITE, &nvs);
    //if NVS cannot open open, return error
    if(err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed nvs read - could not open NVS at key: %s | %s", key, esp_err_to_name(err));
        return err;
    }
    //read value from NVS
    err = nvs_get_blob(nvs, key, value, &read_size);
    if(err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed nvs read - could not read data of size %d at key: %s | %s", read_size, key, esp_err_to_name(err));
        return err;
    }
    nvs_close(nvs);
    //return result of read operation
    return err;
}

esp_err_t nvs_manager_write(const char* key, const void* value) 
{
    //Open NVS at key location with read/write access
    nvs_handle_t nvs;
    esp_err_t err = nvs_open(key, NVS_READWRITE, &nvs);
    //if NVS cannot open open, return error
    if(err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed nvs write - could not open NVS at key: %s | %s", key, esp_err_to_name(err));
        return err;
    }

    //write value to NVS
    err = nvs_set_blob(nvs, key, value, sizeof(value));
    nvs_commit(nvs);
    nvs_close(nvs);
    //return result of write operation
    return err;
}

esp_err_t nvs_manager_erase(const char* key) 
{
    //Open NVS at key location with read/write access
    nvs_handle_t nvs;
    esp_err_t err = nvs_open(key, NVS_READWRITE, &nvs);
    //if NVS cannot open open, return error
    if(err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed nvs erase - could not open NVS at key: %s | %s", key, esp_err_to_name(err));
        return err;
    }
    //erase value from NVS
    err = nvs_erase_key(nvs, key);
    nvs_commit(nvs);
    nvs_close(nvs);
    //return result of erase operation
    return err;

}

