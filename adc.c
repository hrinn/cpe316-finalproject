/*
 * adc.c
 *
 *  Created on: May 21, 2020
 *      Authors: Kattia, Hayden
 */

#include "adc.h"

void setup_ADC14(void) {
    /* ADC 14 ON, S&H, Sampling Time 128 Cycles,
     * MCLK as source, enable conversions, 10-bit conversion results */
    ADC14->CTL0 = ADC14_CTL0_ON | ADC14_CTL0_SHP | ADC14_CTL0_SHT0__128
            | ADC14_CTL0_SSEL__MCLK | ADC14_CTL0_ENC | ADC14_CTL1_RES__10BIT;

    // configure P4.7 as ADC Channel 6
    P4->SEL1 |= BIT7;
    P4->SEL0 |= BIT7;

    // use Channel 6 Avcc is by default 3.3v
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_6;

    // enable ADC conv complete interrupt
    ADC14->IER0 |= ADC14_IER0_IE0;

    // enable global interrupts
    __enable_irq();

    // enable ADC interrupt in NVIC module
    NVIC->ISER[0] = 1 << ((ADC14_IRQn) & 0x1F);
}
