#include "driver/SPIFFSManager.h"
#include "controller/NetworkConfigController.h"
#include "esp_log.h"

/*****************************************/
/* private variables */
/*****************************************/
static const char *TAG = "network_config_controller";
// content file extensions
static const char* html_extension = ".html";
static const char* css_extension = ".css";
static const char* js_extension = ".js";
static const char* png_extension = ".png";
static const char* jpeg_extension = ".jpeg";
static const char* icon_extension = ".ico";
// content types for the web server
static const char* default_content_type = "text/plain";
static const char* html_content_type = "text/html";
static const char* script_content_type = "application/javascript";
static const char* css_content_type = "text/css";
static const char* png_content_type = "image/png";
static const char* jpeg_content_type = "image/jpeg";
static const char* icon_content_type = "image/x-icon";
// server uris
static const char* index_uri = "/";
static const char* any_uri = "/*";
// files
static const char* index_html = "/index.html";

/*****************************************/
/* private function definitions */
/*****************************************/
/**
 * @brief Handles all static file requests
 *
 * @param req http request packet
 * @return esp_err_t system status code
 */
static esp_err_t static_file_handler(httpd_req_t* req)
{
    // verify the SPIFFS file system is mounted
    if (!spiffs_manager_is_mounted())
    {
        ESP_LOGE(TAG, "SPIFFS is not mounted");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    const char* uri = req->uri;
    if (strcmp(uri, index_uri) == 0)
    {
        uri = index_html; // Handle the index URI separately
    }
    else
    {
        uri = req->uri; // Use the requested URI
    }

    // determine content type from request
    const char* content_type = NULL;
    const char* content_extension = strrchr(uri, '.');
    if (!content_extension)
    {
        ESP_LOGE(TAG, "Failed to determine content type for uri: %s", uri);
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }
    if (strcmp(content_extension, js_extension) == 0)
    {
        content_type = script_content_type;
    }
    else if (strcmp(content_extension, html_extension) == 0)
    {
        content_type = html_content_type;
    }
    else if (strcmp(content_extension, css_extension) == 0)
    {
        content_type = css_content_type;
    }
    else if (strcmp(content_extension, png_extension) == 0)
    {
        content_type = png_content_type;
    }
    else if (strcmp(content_extension, jpeg_extension) == 0)
    {
        content_type = jpeg_content_type;
    }
    else if (strcmp(content_extension, icon_extension) == 0)
    {
        content_type = icon_content_type;
    }
    else
    {
        ESP_LOGW(TAG, "Unsupported content type: %s", content_extension);
        content_type = default_content_type;
    }

    // create spiffs data filepath from request
    ESP_LOGI(TAG, "requesting data: %s", uri);
    char spiffs_data_path[520];
    snprintf(spiffs_data_path, sizeof(spiffs_data_path), "/spiffs%s", uri);

    // Open the file from SPIFFS
    FILE* file_data = spiffs_manager_open_file(spiffs_data_path, "r");
    if (!file_data)
    {
        ESP_LOGE(TAG, "Failed to open file: %s", uri);
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    // Serve the file
    web_server_serve_file(req, file_data, content_type);
    return ESP_OK;
}

/*****************************************/
/* public function definitions */
/*****************************************/
void network_config_controller_register_routes()
{
    // register routes for the main page
    ESP_LOGI(TAG, "Registering network config controller routes");
    esp_err_t result = web_server_register_get(any_uri, static_file_handler);
    if (result != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to register network settings uri route: %s", esp_err_to_name(result));
        assert(false);
    }
}