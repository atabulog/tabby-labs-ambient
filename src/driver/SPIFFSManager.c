#include "driver/SPIFFSManager.h"


/*****************************************/
/* private variable declarations */
/*****************************************/
static bool spiffs_is_mounted = false;
static const char* TAG = "spiffs_manager";

/*****************************************/
/* public function definitions */
/*****************************************/
void spiffs_manager_init(void) 
{
    ESP_LOGI(TAG, "Initializing SPIFFS");
    // create SPIFFS configuration
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 3,
        .format_if_mount_failed = true
    };
    // initialize SPIFFS
    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount SPIFFS (%s)", esp_err_to_name(ret));
        spiffs_is_mounted = false;
        return;
    }

    ESP_LOGI(TAG, "SPIFFS initialized successfully");
    spiffs_is_mounted = true;

}

bool spiffs_manager_is_mounted(void) 
{
    return spiffs_is_mounted;
}

FILE* spiffs_manager_open_file(const char* file_path, const char* access_mode)
{
    // exit early if SPIFFS is not mounted
    if(!spiffs_is_mounted)
    {
        ESP_LOGE(TAG, "SPIFFS is not mounted");
        return NULL;
    }

    //exit early if file path or access mode is null
    if(!file_path || !access_mode)
    {
        ESP_LOGE(TAG, "Invalid file path or access mode");
        return NULL;
    }
    // open file and return file pointer
    ESP_LOGD(TAG, "Opening file: %s", file_path);
    return fopen(file_path, access_mode);
}


int spiffs_manager_handle_data_stream(const char* file_path, int (*handler)(FILE* file_data), const char* access_mode)
{
    // open file in read mode and exit early if open fails
    FILE* file = fopen(file_path, access_mode);
    if (!file) 
    {
        return -1;
    }

    //apply handler to file data
    return handler(file);
}