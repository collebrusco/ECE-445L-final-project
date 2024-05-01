#ifndef CONTROLER_BOARD_H
#define CONTROLER_BOARD_H

// all controller board interfacing 

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

typedef struct {
    uint8_t raw, rising, falling;
} Button;

void controller_init();        // init

// ******** SOUND ********
#include "DAC.h"
#include "Sound.h"

// ******** DISPLAY ********
#include "../inc/ST7735.h"

// ******** INPUT ********
void controller_sample();               // call once/frame for input   

uint32_t controller_pot();              // slidepot (4095 up, 0 down)
uint32_t controller_adc_manual();       // manually samples adc, raw value

Button* controller_lever_up();          // all button edge detection via sample()
Button* controller_lever_down();
Button* controller_button_left();
Button* controller_button_right();

// ******** LEDS ********
void controller_red_on();
void controller_red_off();
void controller_red_toggle();

void controller_yellow_on();
void controller_yellow_off();
void controller_yellow_toggle();


#ifdef __cplusplus
}
#endif

#endif
