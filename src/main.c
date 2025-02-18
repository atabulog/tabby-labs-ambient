#include "NVSManager.h"
#include "HardwareAbstractionLayer.h"
#include "NetworkManager.h"
#include "esp_log.h"
#include "esp_sleep.h"
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

void app_main()
{
    setup();
    // Enable wakeup from a timer
    while (true)
    {
        // TODO: Real work
        vTaskDelay(pdMS_TO_TICKS(100)); // 100ms delay
    }
    

}