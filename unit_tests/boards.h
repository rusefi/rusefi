/*
 * boards.h
 *
 *  Created on: Nov 15, 2013
 *      Author: Andrey Belomutskiy, (c) 2012-2013
 */

#ifndef BOARDS_H_
#define BOARDS_H_

#define ADC_CHANNEL_VREF 0

#define TEST_MAF_CHANNEL 10000013

float getVoltageDivided(int);
float getVoltage(int channel);
int getAdcValue(int channel);

#endif /* BOARDS_H_ */
