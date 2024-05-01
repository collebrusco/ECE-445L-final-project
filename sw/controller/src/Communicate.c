#include "Communicate.h"
#include "./inc/UART5.h"

// Controller TX is wired to UART5 (PE4).

void Comms_Init() {
	UART5_Init();
}

void Send_Data(uint32_t data) { // just a uart write lol
	uint8_t pwm_byte = data & 0xFF; // low byte of data is PWM val
	uint8_t byte1 = (data >> 8) & 0xFF;
	uint8_t byte2 = (data >> 16) & 0xFF;
	uint8_t byte3 = (data >> 24) & 0xFF;
	
	// we will send, IN ORDER, pwm:byte1:byte2:byte3
	UART5_OutChar(pwm_byte);
	UART5_OutChar(byte1);
	UART5_OutChar(byte2);
	UART5_OutChar(byte3);
}
