#include "server/WebServer.h"
#include "esp_log.h"

static const char *TAG = "web_server";

bool web_server_start(web_server_t* server) 
{
    if(server)
    {
        ESP_LOGW(TAG, "Server already started.");
        return false;
    }
    // Allocate memory for the server struct
    server = calloc(1, sizeof(web_server_t));
    if (!server) 
    {
        ESP_LOGE(TAG, "Failed to allocate memory for server.");
        return false;
    }
    // Configure the web server
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    if (httpd_start(&server->server_handle, &config) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to start web server.");
        free(server);
        return false;
    }
    //log the success and return server pointer
    ESP_LOGI(TAG, "Web server started.");
    return true;
}

bool web_server_stop(web_server_t* server)
{
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
    if (!server || !server->server_handle)
    {
        return ESP_ERR_INVALID_ARG;
    }
    //register uri handler with server
    return httpd_register_uri_handler(server->server_handle, uri_handler);
}

esp_err_t web_server_register_get(web_server_t* server, const char* uri, esp_err_t (*handler)(httpd_req_t *req))
{
    //create uri handler
    httpd_uri_t uri_handler = {
        .uri = uri,
        .method = HTTP_GET,
        .handler = handler,
        .user_ctx = NULL
    };
    //register uri handler with server
    return web_server_register_handler(server, &uri_handler);
}