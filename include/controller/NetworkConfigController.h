#ifndef NETWORK_CONFIG_CONTROLLER_H
#define NETWORK_CONFIG_CONTROLLER_H

/**
 * @file AmbientDataController.h
 * @author Austin Tabulog
 * @brief The Network Config Controller is responsible for handling the HTTP requests for the network configuration app.
 * @version 0.1
 * @date 2025-02-20
 *
 * @copyright Copyright (c) 2025
 */

#include "server/WebServer.h"

/**
* @brief Registers the controller's routes to the provided web server
* @param server Web server to handle the controller's routes
*/
void network_config_controller_register_routes(web_server_t *server);
 
#endif //NETWORK_CONFIG_CONTROLLER_H