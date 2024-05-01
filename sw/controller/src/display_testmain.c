// /** File includes. */
// #include <stdint.h>

// /* Register definitions. */
// #include "./inc/tm4c123gh6pm.h"
// /* Clocking. */
// #include "./inc/PLL.h"
// /* Clock delay and interrupt control. */
// #include "./inc/CortexM.h"
// /* Initialization of all the pins. */
// #include "./inc/Unified_Port_Init.h"
// /* Talking to PC via UART. */
// #include "./inc/UART.h"
// /* ST7735 display. */
// #include "./inc/ST7735.h"

// #include "./inc/Timer0A.h"
// #include "./inc/LED.h"

// #include "Display.h"

// /** Function Implementations. */
// void DelayWait10ms(uint32_t n) {
//     uint32_t volatile time;
//     while (n){
//         time = 727240 * 2 / 91;  // 10msec
//         while (time){
//             --time;
//         }
//         --n;
//     }
// }

// static void bumptach() {
//     static uint32_t rpm = 5000;
//     rpm = rpm > 8000 ? 0 : rpm+20;
//     update_rpm(rpm);
// }

// #define PF1                     (*((volatile uint32_t *)0x40025008))
// #define PF2                     (*((volatile uint32_t *)0x40025010))
// #define PF3                     (*((volatile uint32_t *)0x40025020))

// int main(void) {
//     /* Disable interrupts for initialization. */
//     DisableInterrupts();

//     /* Initialize clocking. */
//     PLL_Init(Bus80MHz);

//     /* Allow us to talk to the PC via PuTTy! Check device manager to see which
//        COM serial port we are on. The baud rate is 115200 chars/s. */
//     UART_Init();

//     /* Initialize all ports. */
//     LED_Init();

//     /* Allows any enabled timers to begin running. */
//     EnableInterrupts();

//     ST7735_InitR(INITR_REDTAB);
//     ST7735_FillScreen(ST7735_BLACK);
//     ST7735_SetCursor(0, 0);

//     draw_tach(0);
//     Timer0A_Init(bumptach, 80000000/60, 1);

//     while (1) {
//         PF3 ^= 0x08;
//         WaitForInterrupt();
//     }
//     return 1;
// }
