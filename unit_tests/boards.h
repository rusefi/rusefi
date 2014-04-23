/*
 * boards.h
 *
 *  Created on: Nov 15, 2013
 *      Author: Andrey Belomutskiy, (c) 2012-2013
 */

#ifndef BOARDS_H_
#define BOARDS_H_

#define ADC_LOGIC_TPS 0
#define ADC_LOGIC_AFR 0
#define ADC_LOGIC_MAF 0
#define ADC_LOGIC_MAP 0
#define ADC_CHANNEL_VREF 0
#define ADC_CHANNEL_VBATT 0
#define ADC_LOGIC_INTAKE_AIR 0
#define ADC_LOGIC_COOLANT 0

float getVoltageDivided(int);
float getVoltage(int channel);
int getAdcValue(int channel);

#endif /* BOARDS_H_ */
