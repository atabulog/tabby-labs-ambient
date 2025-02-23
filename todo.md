1. Need to flash spiffs data
1. Need to check if AP page loads as expected
1. Probably need to assign an IP to the device when loads into AP mode
1. Need to create handlers for AP form submission. This will need to save data to NVS, then try to reset the network connection. Maybe even redirect/refresh the page
1. Need to create custom page for the standard connection showing data readouts
1. Need to create a custom page for configuring device settings. This data needs to save to NVS
1. Probably need to create a physical reset capability (holding down buttons, etc.)
1. Need to enable power saving sleep cycles. Do not sleep when in AP mode.
1. Need to integrate a server driven update when in sleep mode, or leverage a different device as the data aggregating server, and create this as a client.
1. Need to integrate on-board led to indicate when in AP mode.
1. Need to encrypt NVS data
1. Need to look into support https traffic instead of raw http
1. Consider adding a MQTT client instead of just an http data server.
1. Seems like the favicon.ico is either not being requested or not being served properly. May have to do with the wildcard url approach in the handler.