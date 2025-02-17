#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "esp_http_server.h"
#include "stdbool.h"

// Define a web struct for holding server configuration
typedef struct {
    httpd_handle_t server_handle;
} web_server_t;

/**
 * @brief Start a web server instance
 * @param server web server to start
 */
bool web_server_start(web_server_t* server);

/**
 * @brief Stop a web server instance
 * @param server web server to stop
 */
bool web_server_stop(web_server_t* server);

/**
 * @brief register a GET handler to a web server
 * 
 * @param server server to register handler to
 * @param uri uri associated with handler
 * @param handler method to handle GET request
 * @return esp_err_t result of registration
 */
esp_err_t web_server_register_get(web_server_t* server, const char* uri, esp_err_t (*handler)(httpd_req_t *req));

#endif // WEB_SERVER_H