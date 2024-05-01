//***********************  ESP8266.h  ***********************
// Program written by:
// - Steven Prickett  steven.prickett@gmail.com
//
// Driver for ESP8266 module to act as a WiFi client or server
//
//*********************************************************
/* Modified by Jonathan Valvano, Sept 19, 2015
   Modified by Andreas Gerstlauer, Apr 13, 2020 
 */

#ifndef ESP8266_H
#define ESP8266_H

#include <stdint.h>

#define ESP8266_ENCRYPT_MODE_OPEN            0
#define ESP8266_ENCRYPT_MODE_WEP             1
#define ESP8266_ENCRYPT_MODE_WPA_PSK         2
#define ESP8266_ENCRYPT_MODE_WPA2_PSK        3
#define ESP8266_ENCRYPT_MODE_WPA_WPA2_PSK    4

#define ESP8266_WIFI_MODE_CLIENT            1
#define ESP8266_WIFI_MODE_AP                2
#define ESP8266_WIFI_MODE_AP_AND_CLIENT     3


//------------------- Reset_8266 --------------
// Commands the ESP to reset, determines completion by waiting
void Reset_8266(void);

void Await_ESP_Ready(void);

void tm4c_to_esp8266(void);


#endif
