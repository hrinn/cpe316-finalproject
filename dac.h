/*
 * dac.h
 *
 *  Created on: May 11, 2020
 *      Author: Hayden
 */

#ifndef DAC_H_
#define DAC_H_

/* includes */
#include "msp.h"

/* macros */
#define DAC_CONFIG 0x30
#define DAC_DATA_UPPER 0x3C0
#define DAC_DATA_LOWER 0x03F
#define DAC_UPPER_SHIFT 6
#define DAC_LOWER_SHIFT 2
#define DAC_3V 930

/* function definitions */
void init_spi(void);
void dac_write(uint16_t data);

#endif /* DAC_H_ */
