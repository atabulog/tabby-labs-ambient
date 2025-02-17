#include "esp_log.h"
#include "controller/AmbientDataController.h"
#include "HardwareAbstractionLayer.h"


static const char* TAG = "ambient_data_controller";
static const char* current_temperature_uri = "/api/ambient/current";

/**
 * @brief Get the current ambient values
 * 
 * @http GET /api/ambient/current
 * @param req http request packet
 * @return esp_err_t system status code
 */
esp_err_t ambient_controller_get_current_data(httpd_req_t *req)
{
    // Get the temperature and humidity from the sensors
    float temperature = hal_get_temperature();
    float humidity = hal_get_humidity();

    // Create a JSON response
    char response[64];
    snprintf(response, sizeof(response), "{\"id\": \"CurrentAmbient\", \"temp [C]\": %.2f, \"RH [%%]\": %.2f}", temperature, humidity);
    // Send the response
    httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

void ambient_data_controller_register_routes(web_server_t* server)
{
    ESP_LOGI(TAG, "Registering device controller routes");
    if(web_server_register_get(server, current_temperature_uri, ambient_controller_get_current_data))
    {
        ESP_LOGI(TAG, "Registered route: %s", current_temperature_uri);
    }
    else
    {
        ESP_LOGE(TAG, "Failed to register route: %s", current_temperature_uri);
    }
}


