#include "dac.h"

/*
 * dac.c
 *
 *  Created on: May 9, 2020
 *      Author: Hayden
 */

void init_spi(void) {
    // P1.6 = USB0SIMO -> SDI
    // P1.5 = UCB0CLK -> SCLK
    // P4.1 -> CS

    P1->SEL0 |= BIT5 | BIT6; // Set P1.5 and P1.6 as SPI pins

    // Setup P4.1 (CS) as GPIO output
    P4->SEL0 &= ~BIT1;
    P4->SEL1 &= ~BIT1;
    P4->DIR |= BIT1;
    P4->OUT |= BIT1; // CS is high when idle

    // Initialize eUSCI
    // set UCSWRST
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;         // This puts the SPI into reset mode and allows config
    // Configure parameters
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_MST;           // master mode
    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_CKPL;         // clock is low when inactive
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_CKPH;          // Data is changed on the falling edge of the clock
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_MSB;           // MSB is transmitted first
    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SEVENBIT;     // 8 bit data length mode
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SYNC;          // Synchronous mode
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SSEL__SMCLK;   // Select SMCLK as clock source
    // Clear UCSWRST
    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;        // SPI is released for normal operation
}

void spi_write(uint8_t data) {
    while (!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG)); // wait for the SPI to finish transmitting
    EUSCI_B0->TXBUF = data; // transmit data

}

uint8_t get_upper(uint16_t data) {
    uint8_t upper = DAC_CONFIG | ((data & DAC_DATA_UPPER) >> DAC_UPPER_SHIFT);
    return upper;
}

uint8_t get_lower(uint16_t data) {
    uint8_t lower = (data & DAC_DATA_LOWER) << DAC_LOWER_SHIFT;
    return lower;
}

void dac_write(uint16_t data) {
    P4->OUT &= ~BIT1; // set CS to low, start of transmission
    spi_write(get_upper(data));
    spi_write(get_lower(data));
    P4->OUT |= BIT1; // set CS to high, end of transmission
}
