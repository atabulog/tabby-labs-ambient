#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "esp_http_server.h"

// Define a struct for holding server configuration
typedef struct {
    httpd_handle_t server_handle;
} web_server_t;

// Function to start the web server
web_server_t* web_server_start(void);

// Function to stop the web server
void web_server_stop(web_server_t* server);

// Function to register URI handlers
esp_err_t web_server_register_handler(web_server_t* server, httpd_uri_t* uri_handler);

#endif // WEB_SERVER_H