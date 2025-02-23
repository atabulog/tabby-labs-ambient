#include "driver/HardwareAbstractionLayer.h"
#include "esp_log.h"
#include <math.h>
#include <stdbool.h>

//tag for logging
static const char* TAG = "HardwareAbstractionLayer";
//flag to check if the temperature sensor is initialized
static bool is_temp_sensor_initialized = false;
static bool is_rh_sensor_initialized = false;

/**
 * @brief "Private" function to initialize the temperature sensor
 */
void hal_init_temperature_sensor(void)
{
    //TODO: Actually implement the hardware initialization when the hardware is available
    is_temp_sensor_initialized = true; //ESP_ERROR_CHECK is an assert, so if it fails, the code will not reach this line
}

/**
 * @brief "Private" function to initialize the humidity sensor
 */
void hal_init_humidity_sensor(void)
{
    //TODO: Actually implement the hardware initialization when the hardware is available
    is_rh_sensor_initialized = true; //ESP_ERROR_CHECK is an assert, so if it fails, the code will not reach this line
}


void tl_hal_init(void)
{
    hal_init_temperature_sensor();
}


float hal_get_temperature(void)
{
    if(!is_temp_sensor_initialized)
    {
        ESP_LOGW(TAG, "Temperature sensor is not initialized.");
        return nanf(""); //return NaN if the sensor is not initialized
    }
    // Get the temperature from the sensor
    static float temperature = 0.0; //todo: replace with actual sensor reading
    return temperature;
}

float hal_get_humidity(void)
{
    if(!is_rh_sensor_initialized)
    {
        ESP_LOGW(TAG, "Humidity sensor is not initialized.");
        return nanf(""); //return NaN if the sensor is not initialized
    }
    // Get the humidity from the sensor
    static float humidity = 0.0; //todo: replace with actual sensor reading
    return humidity;
}
