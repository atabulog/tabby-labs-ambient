#ifndef SPIFFS_MANAGER_H
#define SPIFFS_MANAGER_H

#include <stdbool.h>
#include <esp_spiffs.h>
#include <esp_log.h>

/**
 * @brief Initialize SPIFFS manager system
 * 
 */
void spiffs_manager_init(void);

/**
 * @brief Get the SPIFFS mount status
 *
 * @return true if mounted
 * @return false if not mounted
 */
bool spiffs_manager_is_mounted(void);

/**
 * @brief Open a file in SPIFFS
 * 
 * @param file_path file path to open
 * @param access_mode mode to open file in
 * @return FILE* pointer to file or null if error
 */
FILE* spiffs_manager_open_file(const char* file_path, const char* access_mode);

/**
 * @brief Handle a data stream from a file
 *
 * @param file_path path to file
 * @param handler function to handle data stream
 * @param access_mode file access mode "r" for read, "w" for write, "a" for append, etc.
 * @return int -2 if data stream error, else return value of handler
 */
int spiffs_manager_handle_data_stream(const char* file_path, 
                                      int (*handler)(FILE* file_data), 
                                      const char* access_mode);

#endif // SPIFFS_MANAGER_H