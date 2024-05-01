#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/SSD2119.h"
#include "../inc/PLL.h"

int main(void){
    // Set the clocking to run at 80MHz from the PLL.
    PLL_Init(Bus80MHz);

    // Initialize LCD
    LCD_Init();
 
    LCD_SetTextColor(255, 255, 255);
    LCD_SetCursor(0, 0);
    LCD_printf("Hey dipshit, implement this!");
}
