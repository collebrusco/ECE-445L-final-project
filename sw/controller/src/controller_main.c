/** File includes. */
#include <stdint.h>

/* Register definitions. */
#include "../inc/tm4c123gh6pm.h"
/* Clocking. */
#include "../inc/PLL.h"
/* Clock delay and interrupt control. */
#include "../inc/CortexM.h"
/* ST7735 display. */
#include "../inc/ST7735.h"

#include "../inc/Timer5A.h"
#include "../inc/Timer3A.h"
#include "../inc/LED.h"
#include "../inc/Launchpad.h"
#include "../inc/UART5.h"
#include "../inc/esp8266_base.h"
#include "../inc/Unified_Port_Init.h"

#include "Display.h"
#include "DriveSim.h"
#include "ControllerBoard.h"
#include "EngineSound.h"


static uint32_t flag = 0;
static void frame_advance() {
    if (flag) return;
    flag = 1;
    PF1 ^= 0x02;
    controller_sample();
    drive_throttle(controller_pot());
    drive_step();
    if (drive_rpm() >= 6000 && drive_rpm() <= 6500) 
        controller_yellow_on();
    else
        controller_yellow_off();
    if (drive_rpm() >= 6501) 
        controller_red_on();
    else
        controller_red_off();
}
int main(void) {
    DisableInterrupts();

    PLL_Init(Bus80MHz);
    
    Port_E_Init();
    Reset_8266();
    UART5_Init();
    
    LED_Init();
    
    drive_init();
    controller_init();
    engine_sound_init();
    
    ST7735_InitR(INITR_REDTAB);
    ST7735_FillScreen(ST7735_BLACK);
    ST7735_SetCursor(0, 0);
    draw_tach(700);
    
    engine_sound_start(700);
    
    Timer5A_Init(frame_advance, 80000000/30, 1);
    Timer3A_Init(&tm4c_to_esp8266, 8000000, 0);
    
    EnableInterrupts();
    
    uint32_t p_board = 0;
    uint32_t sound = 1;
    
    while (1) {
        if (flag) { flag = 0;
            update_rpm(drive_rpm());
            if (sound) engine_sound_up_rpm(drive_rpm());
            uint32_t board = LaunchPad_Input();
            if (p_board && !board) {
                sound = !sound;
                if (sound) engine_sound_start(drive_rpm());
                else engine_sound_stop();
            }
            p_board = board;
            if (controller_lever_up()->falling)
                {drive_upshift(); update_gear(drive_gear());}
            if (controller_lever_down()->falling)
                {drive_downshift(); update_gear(drive_gear());}
        }

        //WaitForInterrupt();
    }
    return 1;
}
