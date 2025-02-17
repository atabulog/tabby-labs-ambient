#include "NVSManager.h"
#include "esp_log.h"

static const char* TAG = "NVSManager";
//private data declaration
static nvs_data_t nvs_data;
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

esp_err_t nvs_manager_read(const char* key, void* value) 
{
    // Function implementation goes here
    return ESP_OK;
}

esp_err_t nvs_manager_write(const char* key, const void* value) 
{
    // Function implementation goes here
    return ESP_OK;
}

esp_err_t nvs_manager_erase(const char* key) 
{
    // Function implementation goes here
    return ESP_OK;
}

esp_err_t nvs_manager_save(void) 
{
    // Function implementation goes here
    return ESP_OK;
}
