#include "DAC.h"

/*
    DAC runs on SSI2
    Clock   --  PB4
    Fss     --  PB5
    Rx      unused
    Tx      --  PB7

*/
void DAC_init() {
    SYSCTL_RCGCSSI_R |= 0x04;
    SYSCTL_RCGCGPIO_R |= 0x02;
    while (!(SYSCTL_PRGPIO_R & 0x02));

    SSI2_CR1_R = 0x00000000;
    GPIO_PORTB_AFSEL_R |= 0xB0;
    GPIO_PORTB_DEN_R |= 0xB0;
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0x0F00FFFF) | 0x20220000;
    GPIO_PORTB_AMSEL_R &= ~ 0xB0;

    SSI2_CPSR_R = 0x08;
    SSI2_CR0_R = (SSI2_CR0_R & ~0x0000FFF0) | SSI_CR0_SPO | SSI_CR0_DSS_16;
    SSI2_DR_R = 0;
    SSI2_CR1_R |= SSI_CR1_SSE;
}

void DAC_out(uint16_t val) {
    while (!(SSI2_SR_R&0x02)); // wait for room in fifo but we will never need this
    SSI2_DR_R = (uint32_t)val;
}

