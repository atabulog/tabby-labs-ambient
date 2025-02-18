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
 * @brief Connect to a Wi-Fi network
 * 
 * @param ssid Wi-Fi network SSID
 * @param password Wi-Fi network password
 */
void network_manager_connect(const char* ssid, const char* password);

/**
 * @brief Disconnect from the Wi-Fi network
 * 
 */
void network_manager_disconnect(void);

#endif // NETWORK_MANAGER_H