#ifndef COMMS_H
#define COMMS_H

#include "./inc/tm4c123gh6pm.h"
#include <stdint.h>

// Controller TX is wired to UART5 (PE4).

void Comms_Init();

void Send_Data(uint32_t data); // just a uart write lol

#endif
