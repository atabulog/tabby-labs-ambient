#include "NetworkManager.h"
#include "NVSManager.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include <string.h>

// Bit for Wi-Fi connected event
const int WIFI_CONNECTED_BIT = BIT0;
// Event group for Wi-Fi events
static EventGroupHandle_t wifi_event_group;
// Tag for logging
static const char* TAG = "network_manager";
// Flag for network manager connection status
static bool network_manager_connected = false;
// enumeration stating current network interface
static esp_interface_t network_interface = ESP_IF_MAX;
// AP network SSID 
static const char* AP_SSID = "Tabb Labs Ambient";


/**
 * @brief Wi-Fi event handler used to manage wifi events
 *
 * @param arg  event handler argument
 * @param event_base esp derived event subsystem
 * @param event_id esp derived event id
 * @param event_data esp derived event data
 */
static void wifi_event_handler(
    void *arg,
    esp_event_base_t event_base,
    int32_t event_id,
    void *event_data)
{
    if (event_base == WIFI_EVENT)
    {
        switch (event_id)
        {
        case WIFI_EVENT_STA_START:
            esp_wifi_connect();
            break;
        
        case WIFI_EVENT_STA_DISCONNECTED:
            ESP_LOGI(TAG, "Reconnecting to Wi-Fi...");
            esp_wifi_connect();
        break;
        default:
            break;
        }
    }
    else if (event_base == IP_EVENT)
    {
        switch (event_id)
        {
        case IP_EVENT_STA_GOT_IP:
            ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
            ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
            xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
            break;
        
        default:
            break;
        }
    }
}

/**
 * @brief Start the station network
 * 
 */
void wifi_start_sta(wifi_config_t* wifi_config)
{
    // Initialize network interface and event loop for Wi-Fi
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    // Wi-Fi configuration
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    // assign wifi event signals to the wifi event handler
    esp_event_handler_instance_t instance_any_id;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_any_id));
    // assign ip event signals to the wifi event handler
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    ESP_LOGI(TAG, "Connecting to Wi-Fi SSID: %s", wifi_config->sta.ssid);
    // Set Wi-Fi configuration and attempt to connect
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_connect());
    
    // Wait for Wi-Fi connection
    EventBits_t bits = xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
    if (bits & WIFI_CONNECTED_BIT)
    {
        network_manager_connected = true;
        ESP_LOGI(TAG, "Wifi connected: %s", wifi_config->sta.ssid);
    }
    else
    {
        network_manager_connected = false;
        ESP_LOGW(TAG, "failed to connect to wifi SSID: %s", wifi_config->sta.ssid);
    }
    // start network
    ESP_ERROR_CHECK(esp_wifi_start());


}

/**
 * @brief Start the access point network
 *
 */
void wifi_start_ap(void)
{
    // Initialize network interface and event loop for Wi-Fi
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_ap();
    // Wi-Fi configuration
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    //
    wifi_config_t ap_config =
    {
        .ap = {
            .ssid = AP_SSID,
            .ssid_len = strlen(AP_SSID),
            .password = NULL,
            .max_connection = 1,
            .authmode = WIFI_AUTH_OPEN
        }
    };

    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(WIFI_IF_AP, &ap_config);
    esp_wifi_start();

    ESP_LOGI(TAG, "WiFi AP started. Connect to '%s' with no password", AP_SSID);
}

/**
 * @brief Stop the access point network
 *
 */
void wifi_stop_ap(void)
{}


void network_manager_init(void)
{
    //get network data from nvs
    if(!nvs_manager_is_ready())
    {
        ESP_LOGE(TAG, "Stopped init - NVS Manager is not ready");
        assert(false);
    }
    //create a wifi config structure and populate it with data from NVS
    wifi_config_t* wifi_config;
    esp_err_t err = nvs_manager_get_wifi_config(&wifi_config);
    //if network config stored in nvs, start the sta network
    if(err == ESP_OK)
    {    
        network_manager_start_sta(wifi_config);    
    }
    else
    {
        //if no network config stored in nvs, start the ap network
        network_manager_start_ap();    
    }
}


bool network_manager_is_connected(void)
{
    return network_manager_connected;
}

void network_manager_connect(const char* ssid, const char* password)
{
    
}

void network_manager_disconnect(void)
{
    ESP_ERROR_CHECK(esp_wifi_disconnect());
    network_manager_connected = false;
    ESP_LOGI(TAG, "Disconnected from Wi-Fi");
}