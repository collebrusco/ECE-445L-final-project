// /** File includes. */
// #include <stdint.h>

// /* Register definitions. */
// #include "../inc/tm4c123gh6pm.h"
// /* Clocking. */
// #include "../inc/PLL.h"
// /* Clock delay and interrupt control. */
// #include "../inc/CortexM.h"
// /* Initialization of all the pins. */
// #include "../inc/Unified_Port_Init.h"
// /* Talking to PC via UART. */
// #include "../inc/UART.h"
// /* ST7735 display. */
// #include "../inc/ST7735.h"

// #include "../inc/Timer5A.h"
// #include "../inc/Timer4A.h"
// #include "../inc/LED.h"
// #include "ControllerBoard.h"
// #include "Display.h"
// #include "DriveSim.h"
// #include "../inc/UART5.h"
// #include "../inc/esp8266_base.h"
// #include "EngineSound.h"


// /** Function Implementations. */
// static void DelayWait10ms(uint32_t n) {
//     uint32_t volatile time;
//     while (n){
//         time = 727240 * 2 / 91;  // 10msec
//         while (time){
//             --time;
//         }
//         --n;
//     }
// }

// #define PF1                     (*((volatile uint32_t *)0x40025008))
// #define PF2                     (*((volatile uint32_t *)0x40025010))
// #define PF3                     (*((volatile uint32_t *)0x40025020))

// static uint32_t flag = 0;
// static void frame_advance() {
//     if (flag) return;
//     flag = 1;
//     PF1 ^= 0x02;
//     controller_sample();
//     drive_throttle(controller_pot());
//     drive_step();
//     if (drive_rpm() >= 6000 && drive_rpm() <= 6500) 
//         controller_yellow_on();
//     else
//         controller_yellow_off();
//     if (drive_rpm() >= 6501) 
//         controller_red_on();
//     else
//         controller_red_off();
// }
// extern void drive_sim_dbgout();
// int testmain(void) {
//     DisableInterrupts();

//     PLL_Init(Bus80MHz);
//     UART_Init();
//     LED_Init();
//     draw_tach(700);
//     drive_init();
//     controller_init();
//     UART_Init();
//     UART5_Init();
//     Timer5A_Init(frame_advance, 80000000/30, 1);
//     Timer4A_Init(&tm4c_to_esp8266, 80000000, 4);
//     ST7735_InitR(INITR_REDTAB);
//     ST7735_FillScreen(ST7735_BLACK);
//     ST7735_SetCursor(0, 0);

//     EnableInterrupts();

//     EnableInterrupts();
//     uint32_t p_board = 0;
//     uint32_t sound = 1;
//     while (1) {
//         // PF3 ^= 0x08
//         if (flag) { flag = 0;
//             update_rpm(drive_rpm());
//             if (sound) engine_sound_up_rpm(drive_rpm());
//             uint32_t board = LaunchPad_Input();
//             if (p_board && !board) {
//                 sound = !sound;
//                 if (sound) engine_sound_start(drive_rpm());
//                 else engine_sound_stop();
//             }
//             p_board = board;
//             if (controller_lever_up()->falling)
//                 {drive_upshift(); update_gear(drive_gear());}
//             if (controller_lever_down()->falling)
//                 {drive_downshift(); update_gear(drive_gear());}
//                 drive_sim_dbgout();
//         }

//         WaitForInterrupt();
//     }
//     return 1;
// }
