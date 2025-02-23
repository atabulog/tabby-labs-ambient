#include "server/WebServer.h"
#include "controller/AmbientDataController.h"
#include "controller/NetworkConfigController.h"
#include "driver/NVSManager.h"
#include "driver/HardwareAbstractionLayer.h"
#include "driver/NetworkManager.h"
#include "driver/SPIFFSManager.h"
#include "esp_log.h"

esp_interface_t network_interface = ESP_IF_MAX;
web_server_t* web_server = NULL;

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

    // Initialize the spiffs manager
    spiffs_manager_init();
    if(!spiffs_manager_is_mounted())
    {
        ESP_LOGE("SPIFFSManager", "SPIFFS Manager is not mounted");
        assert(false); //exit application
    }

    // Initialize the hardware abstraction layer
    tl_hal_init();
    
    // Initialize the network manager
    network_manager_init();
}

void app_main()
{
    setup();
    // Enable wakeup from a timer
    while (1)
    {
        //check if network interface has changed
        if(network_interface != network_manager_get_interface())
        {
            network_interface = network_manager_get_interface();
            //restart web server if it is running
            if(web_server)
            {
                web_server_stop(web_server);
                web_server = NULL;
                web_server_start(web_server);
            }
            //ensure appropriate web server is running for given network state
            switch(network_interface)
            {
                case ESP_IF_WIFI_STA:
                    ambient_data_controller_register_routes(web_server);
                    break;
                case ESP_IF_WIFI_AP:
                    network_config_controller_register_routes(web_server);
                    break;
                case ESP_IF_WIFI_NAN:
                    ESP_LOGE("main", "WIFI NAN intf not supported");
                    assert(false); //exit application
                    break;
                case ESP_IF_ETH:
                    ESP_LOGE("main", "IF_ETH intf not supported");
                    assert(false); //exit application
                    break;
                case ESP_IF_MAX:
                    ESP_LOGE("main", "IF_MAX intf not supported");
                    assert(false); //exit application
                    break;
                default:
                    ESP_LOGE("main", "unknown intf not supported");
                    assert(false); //exit application
                    break;
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(100)); // 100ms task delay
    }
    

}