#ifndef TEMPERATURE_CONTROLLER_H
#define TEMPERATURE_CONTROLLER_H

/* The temperature controller is responsible for handling the HTTP requests 
 * for the temperature system.
 */

#include "WebServer.h"

/**
 * @brief Registers the controller's routes to the provided web server
 * @param server Web server to handle the controller's routes
 */
void temperature_controller_register_routes(web_server_t* server);

#endif // TEMPERATURE_CONTROLLER_H