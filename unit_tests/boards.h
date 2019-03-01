/*
 * boards.h
 *
 *  Created on: Nov 15, 2013
 *      Author: Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef BOARDS_H_
#define BOARDS_H_

#include "rusefi_enums.h"

#define ADC_CHANNEL_VREF 0

float getVoltageDivided(const char *msg, adc_channel_e);
float getVoltage(const char *msg, int channel);
int getAdcValue(const char *msg, int channel);

#endif /* BOARDS_H_ */
