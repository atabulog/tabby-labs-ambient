#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "esp_http_server.h"
#include "stdbool.h"

// Define a web struct for holding server configuration
typedef struct {
    httpd_handle_t server_handle;
    bool is_running;
} web_server_t;

/**
 * @brief Start a web server instance
 */
bool web_server_start();

/**
 * @brief Stop a web server instance
 */
bool web_server_stop();

/**
 * @brief Check if a web server instance is running
 * 
 * @return true if running
 * @return false if not running
 */
bool web_server_is_running();

/**
 * @brief register a GET handler to a web server
 * 
 * @param uri uri associated with handler
 * @param handler method to handle GET request
 * @return esp_err_t result of registration
 */
esp_err_t web_server_register_get(const char* uri, 
                                  esp_err_t (*handler)(httpd_req_t *req));

/**
 * @brief Serve a file from the file system
 * @param request http request packet
 * @param file_data file data to serve
 * @param content_type content type of file
 * @return esp_err_t result of serving file
 */
esp_err_t web_server_serve_file(httpd_req_t* request, 
                                FILE* file_data, 
                                const char* content_type);
#endif // WEB_SERVER_H