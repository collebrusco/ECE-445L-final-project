#ifndef DISPLAY_H
#define DISPLAY_H
#include <stdint.h>

/*
	tachometer will display "RPM" from 0 to 8k
	6000 to 6500 is sweet spot
	6500 to 8000 is red line
*/

void draw_tach(uint32_t rpm);

void update_rpm(uint32_t rpm);

void update_gear(uint32_t g);

void erase_tach();

#endif
