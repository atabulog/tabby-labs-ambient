#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <stdbool.h>
#include "esp_interface.h"

/**
 * @brief Initialize Network manager system
 */
void network_manager_init();

/**
 * @brief Get the Network connection status
 *
 * @return true if connected
 * @return false if not connected
 */
bool network_manager_is_connected(void);

/**
 * @brief Get the current network interface
 * 
 * @return esp_interface_t 
 */
esp_interface_t network_manager_get_interface(void);

/**
 * @brief Disconnect from the Wi-Fi network
 * 
 */
void network_manager_disconnect(void);

#endif // NETWORK_MANAGER_H