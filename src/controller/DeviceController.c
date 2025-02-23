#include "esp_log.h"
#include "controller/DeviceController.h"

static const char* TAG = "device_controller";
static const char* device_name_uri = "/api/device/name";

/**
 * @brief Get the device name 
 * 
 * @http GET /api/device/name
 * @param req http request packet
 * @return esp_err_t system status code
 */
static esp_err_t device_controller_get_name(httpd_req_t *req)
{
    //TODO: Get the device name from the device's memory
    const char *device_name = "Tabby Labs Ambient";

    // Create a JSON response
    char response[64];
    snprintf(response, sizeof(response), "{\"device_name\": %s}", device_name);

    // Send the response
    httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

void device_controller_register_routes()
{
    ESP_LOGI(TAG, "Registering device controller routes");
    web_server_register_get(device_name_uri, device_controller_get_name);
}
