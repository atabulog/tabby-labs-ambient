#include "esp_wifi.h"
#include "esp_log.h"
#include "server/ServerManager.h"
#include "NVSManager.h"

static const char* TAG = "ServerManager";

void server_manager_init(void)
{
    // Initialize the nvs manager if not already started
    if(!nvs_manager_is_ready())
    {
        nvs_manager_init();

        if(!nvs_manager_is_ready())
        {
            ESP_LOGE(TAG, "NVS Manager is not ready");
            assert(false); //exit application - error setting up NVS manager
        }
    }

    //fetch network connection data from nvs
    wifi_config_t wifi_config = nvs_manager_get_wifi_config();
}