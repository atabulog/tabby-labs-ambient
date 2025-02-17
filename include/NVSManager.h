#ifndef NVS_MANAGER_H
#define NVS_MANAGER_H

/**
 * @file NVSManager.h
 * @author Austin Tabulog
 * @brief Non-Volatile Storage Manager maintains persistent data stored in on-chip flash memory.
 * @version 0.1
 * @date 2025-02-17
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <stdbool.h>
#include "nvs_flash.h"
#include "esp_wifi.h"

/**
 * @brief Generic device information data structure.
 * 
 */
typedef struct device_config_t {
    char device_name[32]; // user defined device name
    char device_type[32]; // manufacturer defined device type
    char device_id[32]; // manufacturer defined device id specific to every device
} device_config_t;

/**
 * @brief All the data managed by the the NVS Manager.
 * 
 */
typedef struct nvs_data_t {
    wifi_config_t wifi_config;
    device_config_t device_config;
} nvs_data_t;

/**
 * @brief Initializes the NVS Manager.
 * 
 */
void nvs_manager_init(void);

/**
 * @brief Gets the NVS Manager ready status.
 * 
 * @return true if the NVS Manager is ready
 * @return false if the NVS Manager is not ready
 */
bool nvs_manager_is_ready(void);

/**
 * @brief Reads the value of a key from the NVS Manager.
 *
 * @param key The key to read.
 * @param value The value to read.
 * @return esp_err_t The error code.
 */
esp_err_t nvs_manager_read(const char* key, void* value);

/**
 * @brief Writes the value of a key to the NVS Manager.
 *
 * @param key The key to write.
 * @param value The value to write.
 * @return esp_err_t The error code.
 */
esp_err_t nvs_manager_write(const char* key, const void* value);

/**
 * @brief Erases the value of a key from the NVS Manager.
 *
 * @param key The key to erase.
 * @return esp_err_t The error code.
 */
esp_err_t nvs_manager_erase(const char* key);

/**
 * @brief Saves the changes to the NVS Manager.
 *
 * @return esp_err_t The error code.
 */
esp_err_t nvs_manager_save(void);

#endif //NVS_MANAGER_H