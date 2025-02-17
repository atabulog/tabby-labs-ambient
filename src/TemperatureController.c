#include "esp_log.h"
#include "TemperatureController.h"
#include "hardwareAbstractionLayer.h"


static const char *TAG = "temperature_controller";

static esp_err_t get_temperature_handler(httpd_req_t *req)
{
    // Get the temperature from the sensor
    float temperature = HAL_get_temperature();

    // Create a JSON response
    char response[64];
    snprintf(response, sizeof(response), "{\"temp C\": %.2f}", temperature);

    // Send the response
    httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

void temperature_controller_register_routes(web_server_t* server)
{
    // Register the get temperature route
    httpd_uri_t get_temperature_route = {
        .uri = "/api/temperature",
        .method = HTTP_GET,
        .handler = get_temperature_handler,
        .user_ctx = NULL
    };
    
    if (web_server_register_handler(server, &get_temperature_route) == ESP_OK) {
        ESP_LOGI(TAG, "Registered route: %s", get_temperature_route.uri);
    } else {
        ESP_LOGE(TAG, "Failed to register route: %s", get_temperature_route.uri);
    }
}


