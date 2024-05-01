#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/* Register definitions. */
#include "../inc/tm4c123gh6pm.h"
/* Clocking. */
#include "../inc/PLL.h"
/* Clock delay and interrupt control. */
#include "../inc/CortexM.h"
/* ST7735 display. */
#include "../inc/ST7735.h"

#include "../inc/UART5.h"
#include "../inc/esp8266_base.h"
#include "../inc/PWM.h"
#include "../lib/RGB/RGB.h"

#define PD1       (*((volatile uint32_t *)0x40007008))

uint8_t power_from_UART = 0;
uint8_t last_power = 0;

#define abs(x) \
    (((x) < 0) ? (-x) : (x))
#define min(x, y) \
	(((x) < (y)) ? (x) : (y))

void Port_D_Init(){
    SYSCTL_RCGCGPIO_R |= 0x08;
    while((SYSCTL_PRGPIO_R & 0x08)==0){};
    GPIO_PORTD_AMSEL_R &= ~0x3F;
    GPIO_PORTD_DIR_R |= 0xF;
    GPIO_PORTD_DEN_R |= 0x3F;
    GPIO_PORTD_DR8R_R |= 0xF;
}

int main(void) {
    DisableInterrupts();
    
    PLL_Init(Bus80MHz);
    
    //Port E GPIO also occurs in UART5_Init, both are port E
    UART5_Init();
    Reset_8266();
    SetupWiFi();

    Port_D_Init();

	PWM0A_Init(40000, 0);

    EnableInterrupts();
						
	const uint32_t slope = 40000 / 256;
    while (1) {
        uint32_t duty = power_from_UART * slope;
        PWM0A_Duty(min(duty, 39990));
        last_power = power_from_UART;
    }
    return 0;
}

void UART5_Handler() {
	UART5_ICR_R &= ~0x10;
	PD1 ^= 0x2;
    power_from_UART = UART5_InChar();
}