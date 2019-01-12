/*
 * boards.h
 *
 *  Created on: Nov 15, 2013
 *      Author: Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef BOARDS_H_
#define BOARDS_H_

#define ADC_CHANNEL_VREF 0

#define TEST_MAF_CHANNEL 10000013
#define TEST_CLT_CHANNEL 10000014
#define TEST_IAT_CHANNEL 10000015

float getVoltageDivided(const char *msg, int);
float getVoltage(const char *msg, int channel);
int getAdcValue(const char *msg, int channel);

#endif /* BOARDS_H_ */
