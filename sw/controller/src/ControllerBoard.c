#ifdef __cplusplus
extern "C" {
#endif

#include "ControllerBoard.h"
#include "./inc/ADCSWTrigger.h"
#include "./inc/tm4c123gh6pm.h"

static uint32_t adc = 0;
static Button lever_up = {.raw = 0, .rising = 0, .falling = 0};
static Button lever_down = {.raw = 0, .rising = 0, .falling = 0};
static Button button_left = {.raw = 0, .rising = 0, .falling = 0};
static Button button_right = {.raw = 0, .rising = 0, .falling = 0};

#define LEVER_PINS (0xF0)
#define LED_PINS (0x03)
#define LEVER_UP_BIT (0x80)
#define LEVER_DOWN_BIT (0x10)
#define GP_LEFT_BIT (0x40)
#define GP_RIGHT_BIT (0x20)
#define LED_RED_BIT (0x02)
#define LED_YELLOW_BIT (0x01)

#define LEVER_UP (GPIO_PORTC_DATA_R & LEVER_UP_BIT)
#define LEVER_DOWN (GPIO_PORTC_DATA_R & LEVER_DOWN_BIT)
#define GP_LEFT (GPIO_PORTC_DATA_R & GP_LEFT_BIT)
#define GP_RIGHT (GPIO_PORTC_DATA_R & GP_RIGHT_BIT)
#define LED_RED_PORT (GPIO_PORTD_DATA_R)
#define LED_YELLOW_PORT (GPIO_PORTD_DATA_R)

static void port_init() {
    SYSCTL_RCGCGPIO_R |= 0x04|0x08;
    { int d; volatile uint32_t delay = (*((volatile uint32_t*)(&d))); } // silly
    GPIO_PORTC_DEN_R |= LEVER_PINS;
    GPIO_PORTC_DIR_R &= ~LEVER_PINS;
    GPIO_PORTC_AMSEL_R &= ~LEVER_PINS;
    GPIO_PORTC_AFSEL_R &= ~LEVER_PINS;
    GPIO_PORTC_PUR_R |= 0x60;

    GPIO_PORTD_DEN_R |= LED_PINS;
    GPIO_PORTD_DIR_R |= LED_PINS;
    GPIO_PORTD_AMSEL_R &= ~LEVER_PINS;
    GPIO_PORTD_AFSEL_R &= ~LEVER_PINS;
}

static void update_butt(Button* butt, uint32_t pin) {
    butt->falling = butt->raw && !pin;
    butt->rising  = !butt->raw && pin;
    butt->raw     = !!pin;
}

void controller_init() {
    ADC0_InitSWTriggerSeq3(5); // PD2
    port_init();
}

// #define DEBUG 

#ifdef DEBUG
#include "./inc/ST7735.h"
#endif

void controller_sample() {
    adc = ADC0_InSeq3();
    update_butt(&lever_up, LEVER_UP);
    update_butt(&lever_down, LEVER_DOWN);
    update_butt(&button_left, GP_LEFT);
    update_butt(&button_right, GP_RIGHT);

    #ifdef DEBUG
    ST7735_SetCursor(0,15);
    ST7735_OutString("    ");
    ST7735_SetCursor(0,15);
    ST7735_OutUDec(adc);
    #endif
}

uint32_t controller_adc_manual() {
    return ADC0_InSeq3();
}

uint32_t controller_pot() {
    return adc;
}

Button* controller_lever_up() {
    return &lever_up;
}

Button* controller_lever_down() {
    return &lever_down;
}

Button* controller_button_left() {
    return &button_left;
}

Button* controller_button_right() {
    return &button_right;
}

void controller_red_on() {
    LED_RED_PORT |= LED_RED_BIT;
}

void controller_red_off() {
    LED_RED_PORT &= ~LED_RED_BIT;
}

void controller_red_toggle() {
    LED_RED_PORT ^= LED_RED_BIT;
}

void controller_yellow_on() {
    LED_YELLOW_PORT |= LED_YELLOW_BIT;
}

void controller_yellow_off() {
    LED_YELLOW_PORT &= ~LED_YELLOW_BIT;
}

void controller_yellow_toggle() {
    LED_YELLOW_PORT ^= LED_YELLOW_BIT;
}

#ifdef __cplusplus
}
#endif

// ******** Testmain: ********

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

// #include "ControllerBoard.h"

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
// static void debug_up() {
//     if (!flag) controller_sample();
//     flag = 1;
// }

// int main(void) {
//     DisableInterrupts();

//     PLL_Init(Bus80MHz);
//     UART_Init();
//     LED_Init();

//     controller_init();

//     EnableInterrupts();

//     ST7735_InitR(INITR_REDTAB);
//     ST7735_FillScreen(ST7735_BLACK);
//     ST7735_SetCursor(0, 0);

//     Timer0A_Init(debug_up, 80000000/30, 1);

//     ST7735_SetCursor(0,0);
//     ST7735_OutString("pot:     ");

//     ST7735_SetCursor(0,1);
//     ST7735_OutString("up: raw ris fal");

//     ST7735_SetCursor(0,4);
//     ST7735_OutString("dn: raw ris fal");
    
//     while (1) {

//         if (flag) { flag = 0;
//             ST7735_SetCursor(5,0);
//             ST7735_OutString("    ");
//             ST7735_SetCursor(5,0);
//             ST7735_OutUDec(controller_pot());
//             ST7735_SetCursor(6,2); ST7735_OutUDec(controller_lever_up()->raw);
//             ST7735_SetCursor(10,2); ST7735_OutUDec(controller_lever_up()->rising);
//             ST7735_SetCursor(14,2); ST7735_OutUDec(controller_lever_up()->falling);

//             ST7735_SetCursor(6,5); ST7735_OutUDec(controller_lever_down()->raw);
//             ST7735_SetCursor(10,5); ST7735_OutUDec(controller_lever_down()->rising);
//             ST7735_SetCursor(14,5); ST7735_OutUDec(controller_lever_down()->falling);
        
//         }

//         WaitForInterrupt();
//     }
//     return 1;
// }

