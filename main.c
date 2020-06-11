#include "msp.h"
#include "adc.h"
#include "dac.h"

/**
 * main.c
 */
void main(void) {
    // stop watchdog timer
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

	// configure ADC parameters
	setup_ADC14();

	// config SPI parameters
	init_spi();

    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;   // Wake up on exit from ISR
    __DSB(); // SLEEPONEXIT takes effect immediately

	while(1) {
	    // read ADC value
	    ADC14->CTL0 |= ADC14_CTL0_SC | ADC14_CTL0_ENC;
	    __sleep();
	}
}

void ADC14_IRQHandler(void) {
    // write ADC value to DAC
    dac_write(ADC14->MEM[0]);
}
