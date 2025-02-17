#include "WebServer.h"
#include "esp_log.h"

static const char *TAG = "web_server";

// Function to start the web server
web_server_t* web_server_start(void) 
{
    // Allocate memory for the server struct
    web_server_t* server = calloc(1, sizeof(web_server_t));
    if (!server) 
    {
        ESP_LOGE(TAG, "Failed to allocate memory for server.");
        return NULL;
    }
    // Configure the web server
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    if (httpd_start(&server->server_handle, &config) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to start web server.");
        free(server);
        return NULL;
    }
    //log the success and return server pointer
    ESP_LOGI(TAG, "Web server started.");
    return server;
}

// Function to stop the web server
void web_server_stop(web_server_t* server)
{
    if (server && server->server_handle)
    {
        httpd_stop(server->server_handle);
        free(server);
        ESP_LOGI(TAG, "Web server stopped.");
    }
}

// Function to register URI handlers
esp_err_t web_server_register_handler(web_server_t* server, httpd_uri_t* uri_handler)
{
    if (!server || !server->server_handle)
    {
        return ESP_ERR_INVALID_ARG;
    }
    return httpd_register_uri_handler(server->server_handle, uri_handler);
}