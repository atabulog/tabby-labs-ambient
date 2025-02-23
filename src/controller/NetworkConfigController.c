#include "driver/SPIFFSManager.h"
#include "controller/NetworkConfigController.h"
#include "esp_log.h"

/*****************************************/
/* private variables */
/*****************************************/
static const char* TAG = "network_config_controller";
static const char* index_html_path = "/spiffs/index.html";
static const char* index_css_path = "/spiffs/styles.css";
static const char* index_html_content_type = "text/html";
static const char* index_css_content_type = "text/css";
static const char* network_settings_uri = "/";
static const char* styles_uri = "/styles.css";
/*****************************************/
/* private function definitions */
/*****************************************/
/**
 * @brief Serve the index.html file
 *
 * @http GET network_settings_uri
 * @param req http request packet
 * @return esp_err_t system status code
 */
static esp_err_t handle_root(httpd_req_t *req) {
    //fetch the index.html file from the SPIFFS file system-make sure its mounted
    if(!spiffs_manager_is_mounted())
    {
        ESP_LOGE(TAG, "SPIFFS is not mounted");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    //get the index.html file from the SPIFFS file system
    FILE* file_data = spiffs_manager_open_file(index_html_path, "r");
    //exit if file data is null
    if(!file_data)
    {
        ESP_LOGE(TAG, "Failed to open index.html");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }
    //serve the index.html file
    web_server_serve_file(req, file_data, index_html_content_type);
    return ESP_OK;
}

/**
 * @brief Serve the styles.css file
 *
 * @http GET styles.css
 * @param req http request packet
 * @return esp_err_t system status code
 */
static esp_err_t handle_styles(httpd_req_t *req) {
    //fetch the index.html file from the SPIFFS file system-make sure its mounted
    if(!spiffs_manager_is_mounted())
    {
        ESP_LOGE(TAG, "SPIFFS is not mounted");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    //get the index.html file from the SPIFFS file system
    FILE* file_data = spiffs_manager_open_file(index_css_path, "r");
    //exit if file data is null
    if(!file_data)
    {
        ESP_LOGE(TAG, "Failed to open styles.css");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }
    //serve the index.html file
    web_server_serve_file(req, file_data, index_css_content_type);
    return ESP_OK;
}


/*****************************************/
/* public function definitions */
/*****************************************/
void network_config_controller_register_routes(web_server_t *server)
{
    ESP_LOGI(TAG, "Registering network config controller routes");
    if(web_server_register_get(server, network_settings_uri, handle_root) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to register route: %s", network_settings_uri);
        assert(false);
    }
    if(web_server_register_get(server, styles_uri, handle_styles) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to register route: %s", styles_uri);
        assert(false);
    }
}