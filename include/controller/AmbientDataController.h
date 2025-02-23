#ifndef AMBIENT_DATA_CONTROLLER_H
#define AMBIENT_DATA_CONTROLLER_H

/**
 * @file AmbientDataController.h
 * @author Austin Tabulog
 * @brief The data controller is responsible for handling the HTTP requests for the ambient data measuring system.
 * @version 0.1
 * @date 2025-02-17
 *
 * @copyright Copyright (c) 2025
 */

#include "server/WebServer.h"

/**
 * @brief Registers the controller's routes to the web server
 */
void ambient_data_controller_register_routes();

#endif // AMBIENT_DATA_CONTROLLER_H