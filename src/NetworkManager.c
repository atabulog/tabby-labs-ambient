#include "NetworkManager.h"
#include "NVSManager.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include <string.h>

// Event group for Wi-Fi events
static EventGroupHandle_t wifi_event_group;
static const char *TAG = "network_manager";
const int WIFI_CONNECTED_BIT = BIT0;
static bool network_manager_connected = false;

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
    // handle STA start events
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    // handle STA disconnected events
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        ESP_LOGI(TAG, "Reconnecting to Wi-Fi...");
        esp_wifi_connect();
    }
    // handle IP assignment events
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

void network_manager_init(void)
{
    wifi_event_group = xEventGroupCreate();

    // ensure nvs manager is ready - dependent on NVS for wifi
    if (!nvs_manager_is_ready())
    {
        nvs_manager_init();
        // NVS Should be started - assert if nvs manager is not ready
        assert(nvs_manager_is_ready());
    }

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
    // set wifi mode to station mode and start | does not have connection data
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "Network initialization completed | can now be connected to network with network_manager_connect()");
}

bool network_manager_is_connected(void)
{
    return network_manager_connected;
}

void network_manager_connect(const char* ssid, const char* password)
{
    ESP_LOGI(TAG, "Connecting to Wi-Fi SSID: %s", ssid);
    wifi_config_t wifi_config = {};
    strncpy((char *)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid));
    strncpy((char *)wifi_config.sta.password, password, sizeof(wifi_config.sta.password));

    // Set Wi-Fi configuration and attepmt to connect
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_connect());

    // Wait for Wi-Fi connection
    EventBits_t bits = xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
    if (bits & WIFI_CONNECTED_BIT)
    {
        network_manager_connected = true;
        ESP_LOGI(TAG, "Wifi connected: %s", ssid);
    }
    else
    {
        network_manager_connected = false;
        ESP_LOGW(TAG, "failed to connect to wifi SSID: %s", ssid);
    }
}

void network_manager_disconnect(void)
{
    ESP_ERROR_CHECK(esp_wifi_disconnect());
    network_manager_connected = false;
    ESP_LOGI(TAG, "Disconnected from Wi-Fi");
}