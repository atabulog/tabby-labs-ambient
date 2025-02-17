#ifndef DEVICE_CONTROLLER_H
#define DEVICE_CONTROLLER_H

/**
 * @file DeviceController.h
 * @author Austin Tabulog
 * @brief The device controller is responsible for handling the HTTP requests for the device's general data.
 * @version 0.1
 * @date 2025-02-17
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "server/WebServer.h"

/**
 * @brief Registers the controller's routes to the provided web server
 * @param server Web server to handle the controller's routes
 */
void device_controller_register_routes(web_server_t* server);

#endif // DEVICE_CONTROLLER_H