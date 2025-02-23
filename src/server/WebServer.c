#include "server/WebServer.h"
#include "esp_log.h"

/*****************************************/
/* private variables */
/*****************************************/
static const char *TAG = "web_server";
static web_server_t* server = NULL;

/*****************************************/
/* public function definitions */
/*****************************************/
bool web_server_start() 
{
    if(server)
    {
        ESP_LOGW(TAG, "Server already started.");
        server->is_running = true;
        return false;
    }
    // Allocate memory for the server struct
    server = calloc(1, sizeof(web_server_t));
    if (!server) 
    {
        ESP_LOGE(TAG, "Failed to allocate memory for server.");
        server->is_running = false;
        return false;
    }
    // Configure the web server
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    if (httpd_start(&server->server_handle, &config) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to start web server.");
        server->is_running = false;
        free(server);
        server = NULL;
        return false;
    }
    server->is_running = true;
    ESP_LOGI(TAG, "Web server started.");
    return true;
}

bool web_server_stop()
{
    server->is_running = false;
    if (server && server->server_handle)
    {
        httpd_stop(server->server_handle);
        free(server);
        ESP_LOGI(TAG, "Web server stopped.");
        return true;
    }
    ESP_LOGW(TAG, "failed to stop server, Web server already stopped.");
    return false;
}

/**
 * @brief Private function to register a uri handler with the server
 * 
 * @param server pointer to web server struct
 * @param uri_handler uri handler to register
 * @return esp_err_t result of registration
 */
esp_err_t web_server_register_handler(web_server_t* server, httpd_uri_t* uri_handler)
{
    //exit if server is null or server handle is null
    if (!server || !server->server_handle || !uri_handler)
    {
        return ESP_ERR_INVALID_ARG;
    }
    //register uri handler with server
    return httpd_register_uri_handler(server->server_handle, uri_handler);
}

esp_err_t web_server_register_get(const char* uri, esp_err_t (*handler)(httpd_req_t *req))
{
    //exit if server, uri, or provided handler is null
    if (!server || !uri)
    {
        if (!server) 
        {
            ESP_LOGE(TAG, "Invalid registration arguments - Server is null.");
        }
        if (!uri) 
        {
            ESP_LOGE(TAG, "Invalid registration arguments - URI is null.");
        }
        if (!handler)
        {
            ESP_LOGE(TAG, "Invalid registration arguments - Handler is null.");
        }
        return ESP_ERR_INVALID_ARG;
    }
    //create uri handler
    httpd_uri_t uri_handler = {
        .uri = uri,
        .method = HTTP_GET,
        .handler = handler,
        .user_ctx = NULL
    };
    //register uri handler with server
    esp_err_t result = web_server_register_handler(server, &uri_handler);
    if(result == ESP_OK)
    {
        ESP_LOGI(TAG, "Registered route: %s", uri);
    }
    else
    {
        ESP_LOGE(TAG, "Failed to register route: %s", uri);
    }
    //register uri handler with server
    return result;
}

esp_err_t web_server_serve_file(httpd_req_t* request, FILE* file_data, const char* content_type)
{
    //exit if file data, httpd request, or content type is null
    if (!file_data || !request || !content_type)
    {
        return ESP_ERR_INVALID_ARG;
    }

    //set the content type in the request
    httpd_resp_set_type(request, content_type);
    //create a buffer and index to read the file data
    static char buffer[128] = {0};
    static size_t bytes_read = 0;
    //read the file data and send it to the client in chunks
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file_data)) > 0)
    {
        httpd_resp_send_chunk(request, buffer, bytes_read);
    }
    //close the file and send the last chunk
    fclose(file_data);
    httpd_resp_send_chunk(request, NULL, 0);
    return ESP_OK;
}