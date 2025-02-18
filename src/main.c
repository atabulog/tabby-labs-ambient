#include "NVSManager.h"
#include "HardwareAbstractionLayer.h"
#include "NetworkManager.h"
#include "esp_log.h"

/**
 * @brief Setup function to initialize the system
 * 
 */
void setup(void)
{
    // Initialize NVS
    nvs_manager_init();
    if(!nvs_manager_is_ready())
    {
        ESP_LOGE("NVSManager", "NVS Manager is not ready");
        assert(false); //exit application
    }
    // Initialize the hardware abstraction layer
    tl_hal_init();
    
    // Initialize the network manager
    network_manager_init();
}

void initialize_web_server(void)
{
}


void app_main()
{
    setup();

    initialize_web_server();
    
    
    // Connect to Wi-Fi
    network_manager_connect("your_ssid", "your_password");
}