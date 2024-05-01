#ifndef DRIVE_SIM_H
#define DRIVE_SIM_H
#include <stdint.h>

// uses Timer1A

// init
void drive_init();
void drive_destroy();

// sim
void drive_throttle(uint32_t input);    // set 12-bit throttle
void drive_upshift();                   // upshift
void drive_downshift();                 // downshift
void drive_step();                      // step simulation (internal dt timer)

// get
uint32_t drive_rpm();                   // read rpm
uint32_t drive_gear();                  // read gear
uint32_t drive_power();                 // get sim'd 12-bit power

#endif
