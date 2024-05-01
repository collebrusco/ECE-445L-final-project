#include <stdio.h>
#include <stdint.h>
#include <string.h>			
#include "../inc/tm4c123gh6pm.h"

#include "UART.h"
#include "UART5.h"
#include "ST7735.h"
#include "Clock.h"
#include "esp8266.h"
#include "../src/DriveSim.h"

//#define DEBUG1                // First level of Debug
//#define DEBUG2                // Second level of Debug
//#define DEBUG3                // Third level of Debug

//#undef DEBUG1               // Comment out to enable DEBUG1
//#undef DEBUG2               // Comment out to enable DEBUG2
//#undef DEBUG3               // Comment out to enable DEBUG3


#define UART5_FR_TXFF            0x00000020  // UART Transmit FIFO Full
#define UART5_FR_RXFE            0x00000010  // UART Receive FIFO Empty
#define UART5_LCRH_WLEN_8        0x00000060  // 8 bit word length
#define UART5_LCRH_FEN           0x00000010  // UART Enable FIFOs
#define UART5_CTL_UARTEN         0x00000001  // UART Enable

#define PE0   (*((volatile uint32_t *)0x40024004))    // RDY from ESP 8266
#define PE1   (*((volatile uint32_t *)0x40024008))    // RST_B to ESP8266
#define PE2   (*((volatile uint32_t *)0x40024010))    // ACK to ESP8266
#define PE3   (*((volatile uint32_t *)0x40024020))    // LED to indicate that the 8266 connected

#define PF0   (*((volatile uint32_t *) 0x40025004))

#define RDY   PE0
#define RDY1  0x1


#define LOW   0x0

#define BIT0  0x1
#define BIT1  0x2
#define BIT2  0x4
#define BIT3  0x8   

uint8_t buf[4] = {0, 0, 0, 0};

void DelayWait1ms(uint32_t n) {
    uint32_t volatile time;
    while (n){
        time = 72724 * 2 / 91;  // ~1msec
        while (time){
            --time;
        }
        --n;
    }
}

void Reset_8266(void) 
{
  PE1 = LOW;              // Reset the 8266
  DelayWait1ms(5000);     // Wait for 8266 to reset
  PE1 = BIT1;             // Enable the 8266
  DelayWait1ms(5000);     // Wait before setting up 8266
  PE3 = LOW;              // Turn off LED
  while (!RDY) {
    DelayWait1ms(300);
  }
}

void tm4c_to_esp8266(void) {
  UART5_OutChar(drive_power() >> 4);
}


