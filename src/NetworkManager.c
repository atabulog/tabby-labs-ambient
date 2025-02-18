#include "NetworkManager.h"
#include "NVSManager.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include <string.h>

/*****************************************/
/* private variable declarations */
/*****************************************/


// Bit for Wi-Fi connected event
const int WIFI_CONNECTED_BIT = BIT0;
// Tag for logging
static const char *TAG = "network_manager";
// maximum number of retries to connect to Wi-Fi
static const int MAX_CONNECTION_RETRY = 5;
// AP network SSID
static const char *AP_SSID = "Tabby Labs Ambient";
// Event group for Wi-Fi events
static EventGroupHandle_t wifi_event_group;
// Flag for network manager connection status
static bool network_manager_connected = false;
// enumeration stating current network interface
static esp_interface_t network_interface = ESP_IF_MAX;
// retry counter for Wi-Fi connection
static int retry_num = 0;

/*****************************************/
/* private static functions declarations */
/*****************************************/

/**
 * @brief Start the access point network
 *
 */
static void network_manager_start_ap(void);

/**
 * @brief Start the station network
 *
 */
static void network_manager_start_sta(wifi_config_t *wifi_config);

/**
 * @brief Wi-Fi event handler used to manage STA network events
 *
 * @param arg  event handler argument
 * @param event_base esp derived event subsystem
 * @param event_id esp derived event id
 * @param event_data esp derived event data
 */
static void sta_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

/**
 * @brief Wi-Fi event handler used to manage AP network events
 *
 * @param arg  event handler argument
 * @param event_base esp derived event subsystem
 * @param event_id esp derived event id
 * @param event_data esp derived event data
 */
static void ap_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

/**
 * @brief Stop the access point network
 *
 */
static void wifi_stop_ap(void);

/*****************************************/
/* public function definitions */
/*****************************************/

void network_manager_init(void)
{
    // get network data from nvs
    if (!nvs_manager_is_ready())
    {
        ESP_LOGE(TAG, "Stopped init - NVS Manager is not ready");
        assert(false);
    }
    // create a wifi config structure and populate it with data from NVS
    wifi_config_t* wifi_config = NULL;
    esp_err_t err = nvs_manager_get_wifi_config(wifi_config);
    // if network config stored in nvs, start the sta network
    if (err != ESP_OK)
    {
        // if no network config stored in nvs, start the ap network
        network_manager_start_ap();
        return;
    }
    // if stored network config is empty, start the ap network
    if (wifi_config->sta.ssid[0] == '\0')
    {
        network_manager_start_ap();
        return;
    }
    // start the sta network
    network_manager_start_sta(wifi_config);
}

bool network_manager_is_connected(void)
{
    return network_manager_connected;
}

esp_interface_t network_manager_get_interface(void)
{
    return network_interface;
}

void network_manager_disconnect(void)
{
    ESP_ERROR_CHECK(esp_wifi_disconnect());
    network_manager_connected = false;
    ESP_LOGI(TAG, "Disconnected from Wi-Fi");
}


/*****************************************/
/* private static functions definitions */
/*****************************************/

static void sta_event_handler(
    void* arg,
    esp_event_base_t event_base,
    int32_t event_id,
    void* event_data)
{

    if (event_base == WIFI_EVENT)
    {
        switch (event_id)
        {
        case WIFI_EVENT_STA_START:
            esp_wifi_connect();
            break;

        case WIFI_EVENT_STA_DISCONNECTED:
            if (retry_num < MAX_CONNECTION_RETRY)
            {
                retry_num++;
                ESP_LOGI(TAG, "Reconnecting to Wi-Fi...");
                esp_wifi_connect();
            }
            else
            {
                network_manager_connected = false;
                ESP_LOGW(TAG, "Failed to connect to STA network, falling back to AP mode");
                network_manager_disconnect();
                network_manager_start_ap();
            }
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
            ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
            ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
            xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
            break;

        default:
            break;
        }
    }
}

static void ap_event_handler(
    void* arg,
    esp_event_base_t event_base,
    int32_t event_id,
    void* event_data)
{
    if (event_base == WIFI_EVENT)
    {
        switch (event_id)
        {
        case WIFI_EVENT_AP_START:
            ESP_LOGI(TAG, "Wi-Fi AP started");
            break;
        case WIFI_EVENT_AP_STOP:
            ESP_LOGI(TAG, "Wi-Fi AP stopped");
            break;
        case WIFI_EVENT_AP_STACONNECTED:
            wifi_event_ap_staconnected_t* connected_event = (wifi_event_ap_staconnected_t*) event_data;
            ESP_LOGI(TAG, "Client connected. AID=%d", connected_event->aid);
            break;
        case WIFI_EVENT_AP_STADISCONNECTED:
            wifi_event_ap_stadisconnected_t* disconnected_event = (wifi_event_ap_stadisconnected_t*) event_data;
            ESP_LOGI(TAG, "Client disconnected. AID=%d", disconnected_event->aid);
            break;
        default:
            break;
        }
    }
    else if (event_base == IP_EVENT)
    {
        switch (event_id)
        {
        case IP_EVENT_AP_STAIPASSIGNED:
            ESP_LOGI(TAG, "Wi-Fi AP client IP assigned");
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
static void network_manager_start_sta(wifi_config_t *wifi_config)
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
                                                        &sta_event_handler,
                                                        NULL,
                                                        &instance_any_id));
    // assign ip event signals to the wifi event handler
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &sta_event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    ESP_LOGI(TAG, "Connecting to Wi-Fi SSID: %s", wifi_config->sta.ssid);
    // Set Wi-Fi configuration and attempt to connect
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, wifi_config));
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
    network_interface = ESP_IF_WIFI_STA;
}

static void network_manager_start_ap(void)
{
    // Initialize network interface and event loop for Wi-Fi
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_ap();

    // Wi-Fi configuration
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Register event handlers for Wi-Fi and IP events
    esp_event_handler_instance_t instance_any_id;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &ap_event_handler,
                                                        NULL,
                                                        &instance_any_id));
    esp_event_handler_instance_t instance_ap_staipassigned;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_AP_STAIPASSIGNED,
                                                        &ap_event_handler,
                                                        NULL,
                                                        &instance_ap_staipassigned));

    wifi_config_t ap_config = {};
    strcpy((char *)ap_config.ap.ssid, AP_SSID);
    strcpy((char *)ap_config.ap.password, "");
    ap_config.ap.ssid_len = strlen(AP_SSID);
    ap_config.ap.max_connection = 1;
    ap_config.ap.authmode = WIFI_AUTH_OPEN;

    // Set Wi-Fi configuration and start AP mode
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "WiFi AP started. Connect to '%s' with no password", AP_SSID);
    network_interface = ESP_IF_WIFI_AP;
}

static void wifi_stop_ap(void)
{
}
