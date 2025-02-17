#ifndef HARDWARE_ABSTRACTION_LAYER_H
#define HARDWARE_ABSTRACTION_LAYER_H

/* The hardware abstraction layer is responsible for handling the hardware
 * specific code and abstracting it from a standard CRUD interface.
 */

#include "driver/temperature_sensor.h"

// Function to initialize the hardware abstraction layer
void HAL_init(void);

/**
 * @brief Get system temperature in Celsius
 * @return float 
 */
float HAL_get_temperature(void);


#endif //HARDWARE_ABSTRACTION_LAYER_H