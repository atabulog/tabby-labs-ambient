#include "hardwareAbstractionLayer.h"
#include "esp_log.h"
#include <math.h>

//tag for logging
static const char* TAG = "HardwareAbstractionLayer";
//temperature sensor handle
temperature_sensor_handle_t temp_sensor = NULL;
//flag to check if the temperature sensor is initialized
bool is_temp_sensor_initialized = false;


/**
 * @brief "Private" function to initialize the temperature sensor
 * 
 */
void HAL_init_temperature_sensor(void)
{
    //TODO: This seems to be broken for this version of ESP-IDF. Will fall back to external sensor approach when the board is designed
    // Setup configuration for the temperature sensor
    //temperature_sensor_config_t temp_sensor_config = TEMPERATURE_SENSOR_CONFIG_DEFAULT(20, 100);
    ////Initialize the temperature sensor with this config
    //ESP_ERROR_CHECK(temperature_sensor_install(&temp_sensor_config, &temp_sensor));
    //ESP_ERROR_CHECK(temperature_sensor_enable(temp_sensor));
    is_temp_sensor_initialized = true; //ESP_ERROR_CHECK is an assert, so if it fails, the code will not reach this line
}

void HAL_init(void)
{
    HAL_init_temperature_sensor();
}


float HAL_get_temperature(void)
{
    if(!is_temp_sensor_initialized)
    {
        ESP_LOGW(TAG, "Temperature sensor is not initialized.");
        return nanf(""); //return NaN if the sensor is not initialized
    }
    // Get the temperature from the sensor
    static float temperature = 0.0;
    ESP_ERROR_CHECK(temperature_sensor_get_celsius(temp_sensor, &temperature));
    return temperature;
}
