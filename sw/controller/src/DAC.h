#ifndef DAC_DRIVER_H
#define DAC_DRIVER_H
#include "./inc/tm4c123gh6pm.h"

void DAC_init();

void DAC_out(uint16_t val);

#define DAC_out_blind(val) SSI2_DR_R = (uint32_t)(val);

#endif

