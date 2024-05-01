// -------------------------------------------------
// UART5.c
// Runs on TM4C123
// Simple device driver for the UART5 which is connected to ESP8266
//
// Author:      Daniel Valvano
// Date:        May 23, 2014
// Modified by: Mark McDermott
// Date:        May 25, 2018 
//
//-------------------------------------------------
// ESP8266 Pin configuration:
//
// U5Rx   connected to PE4
// U5Tx   connected to PE5
// GPIO_0 
// GPIO_2
// RST_B


#include <stdio.h>
#include <stdint.h>
#include "UART5.h"

#include "inc/tm4c123gh6pm.h"

#define UART5_FR_TXFF            0x00000020  // UART Transmit FIFO Full
#define UART5_FR_RXFE            0x00000010  // UART Receive FIFO Empty
#define UART5_LCRH_WLEN_8        0x00000060  // 8 bit word length
#define UART5_LCRH_FEN           0x00000010  // UART Enable FIFOs
#define UART5_CTL_UARTEN         0x00000001  // UART Enable
#define CR 0x0D
#define LR 0x0A

#define UART5_BAUD_RATE 115200
#define SYS_CLOCK_FREQ 80000000
void UART5_Init(void){
    SYSCTL_RCGCUART_R |= 0x20;            // activate UART5
    volatile uint32_t delay = SYSCTL_RCGCUART_R;

    SYSCTL_RCGCGPIO_R |= 0x10;            // activate port E
    delay = SYSCTL_RCGCUART_R;

    UART5_CTL_R &= ~UART_CTL_UARTEN;      // disable UART
    UART5_IBRD_R = 43;                    // IBRD = int(80,000,000 / (16 * 115200)) = int(43.402778)
    UART5_FBRD_R = 26;                    // FBRD = round(0.402778 * 64) = 26
                                        // 8 bit word length (no parity bits, one stop bit, FIFOs)
    UART5_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
    UART5_IFLS_R &= ~0x3F;                // clear TX and RX interrupt FIFO level fields
                                        // configure interrupt for TX FIFO <= 1/8 full
                                        // configure interrupt for RX FIFO >= 1/8 full
    UART5_IFLS_R += (UART_IFLS_TX1_8|UART_IFLS_RX1_8);
                                        // enable RX FIFO interrupts and RX time-out interrupt
    UART5_IM_R |= (UART_IM_RXIM|UART_IM_RTIM);
    UART5_CTL_R |= UART_CTL_RXE | UART_CTL_UARTEN;
    GPIO_PORTE_AFSEL_R = 0x30;           // enable alt funct on PE4-5
    GPIO_PORTE_DEN_R = 0x37;             // enable digital I/O on PE4-5
    GPIO_PORTE_AMSEL_R = ~0x37;          // disable analog functionality on PE
    GPIO_PORTE_DIR_R |= ~0x04;           // input digital I/O on PE2
    GPIO_PORTE_DIR_R |=  0x01;           // output digital I/O on PE0

    // modifying PE4-5
    GPIO_PORTE_PCTL_R = (GPIO_PORTE_PCTL_R&0xFF00F0F0)+0x00110000;
    NVIC_PRI15_R = (NVIC_PRI15_R&0xFF00FFFF)|0x00110000; // bits 21-23
    NVIC_EN1_R = (1 << 29); // interupt 61
}



//------------   UART5_InChar   ------------
// Wait for new serial port input
// Input: none
// Output: ASCII code for key typed
//
char UART5_InChar(void){
	uint32_t val = UART5_FR_R;
  while((val&UART5_FR_RXFE) != 0);
  return((char)(UART5_DR_R & 0xFF));
}


//------------UART5_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
//
void UART5_OutChar(char data){
  while((UART5_FR_R&UART5_FR_TXFF) != 0);
  UART5_DR_R = data;
}


//------------UART_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
//
void UART5_OutString(char *pt){
  while(*pt){
    UART5_OutChar(*pt);
    pt++;
  }
}

//---------------------UART5_Out_CRLF---------------------
// Output a CR,LF to UART to go to a new line
// Input: none
// Output: none
//
void UART5_Out_CRLF(void){
  UART5_OutChar(CR);
  UART5_OutChar(LF);
}


//------------UART5_InUDec------------
// InUDec accepts ASCII input in unsigned decimal format
// and converts to a 32-bit unsigned number
// valid range is 0 to 4294967295 (2^32-1)
// Input: none
// Output: 32-bit unsigned number
//
// If you enter a number above 4294967295, it will return an incorrect value
// Backspace will remove last digit typed
//

uint32_t UART5_InUDec(void){
uint32_t number=0, length=0;
char character;
  character = UART5_InChar();
  while(character != CR){             // accepts until <enter> is typed
    
// The next line checks that the input is a digit, 0-9.
// If the character is not 0-9, it is ignored and not echoed
    
    if((character>='0') && (character<='9')) {
      number = 10*number+(character-'0');   // this line overflows if above 4294967295
      length++;
      UART5_OutChar(character);
    }
    
// If the input is a backspace, then the return number is
// changed and a backspace is outputted to the screen
    
    else if((character==BS) && length){
      number /= 10;
      length--;
      UART5_OutChar(character);
    }
    character = UART5_InChar();
  }
  return number;
}

//-----------------------UART5_OutUDec-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after
//

void UART5_OutUDec(uint32_t n){
// This function uses recursion to convert decimal number
//   of unspecified length as an ASCII string
//
  if(n >= 10){
    UART5_OutUDec(n/10);
    n = n%10;
  }
  UART5_OutChar(n+'0'); /* n is between 0 and 9 */
}


//---------------------UART5_InUHex----------------------------------------
// Accepts ASCII input in unsigned hexadecimal (base 16) format
// Input: none
// Output: 32-bit unsigned number
// No '$' or '0x' need be entered, just the 1 to 8 hex digits
// It will convert lower case a-f to uppercase A-F
//     and converts to a 16 bit unsigned number
//     value range is 0 to FFFFFFFF
// If you enter a number above FFFFFFFF, it will return an incorrect value
// Backspace will remove last digit typed
//
uint32_t UART5_InUHex(void){
uint32_t number=0, digit, length=0;
char character;
  character = UART5_InChar();
  while(character != CR){
    digit = 0x10; // assume bad
    if((character>='0') && (character<='9')){
      digit = character-'0';
    }
    else if((character>='A') && (character<='F')){
      digit = (character-'A')+0xA;
    }
    else if((character>='a') && (character<='f')){
      digit = (character-'a')+0xA;
    }
// If the character is not 0-9 or A-F, it is ignored and not echoed
    if(digit <= 0xF){
      number = number*0x10+digit;
      length++;
      UART5_OutChar(character);
    }
// Backspace outputted and return value changed if a backspace is inputted
    else if((character==BS) && length){
      number /= 0x10;
      length--;
      UART5_OutChar(character);
    }
    character = UART5_InChar();
  }
  return number;
}

//--------------------------UART5_OutUHex----------------------------
// Output a 32-bit number in unsigned hexadecimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1 to 8 digits with no space before or after
//

void UART5_OutUHex(uint32_t number){

	// This function uses recursion to convert the number of
	//   unspecified length as an ASCII string
  
  if(number >= 0x10){
    UART5_OutUHex(number/0x10);
    UART5_OutUHex(number%0x10);
  }
  else{
    if(number < 0xA){
      UART5_OutChar(number+'0');
     }
    else{
      UART5_OutChar((number-0x0A)+'A');
    }
  }
}


//------------UART5_InString------------
// Accepts ASCII characters from the serial port
//    and adds them to a string until <enter> is typed
//    or until max length of the string is reached.
// It echoes each character as it is inputted.
// If a backspace is inputted, the string is modified
//    and the backspace is echoed
// terminates the string with a null character
// uses busy-waiting synchronization on RDRF
// Input: pointer to empty buffer, size of buffer
// Output: Null terminated string
// -- Modified by Agustinus Darmawan + Mingjie Qiu --

void UART5_InString(char *bufPt, uint16_t max) {
int length=0;
char character;
  character = UART5_InChar();
  while(character != CR){
    if(character == BS){
      if(length){
        bufPt--;
        length--;
        UART5_OutChar(BS);
      }
    }
    else if(length < max){
      *bufPt = character;
      bufPt++;
      length++;
      UART5_OutChar(character);
    }
    character = UART5_InChar();
  }
  *bufPt = 0;
}


