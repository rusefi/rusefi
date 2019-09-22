/**
 * @file board.c
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "boards.h"

float testMafValue = 0;
float testCltValue = 0;
float testIatValue = 0;

float getVoltageDivided(const char *msg, adc_channel_e channel DECLARE_ENGINE_PARAMETER_SUFFIX) {
	switch(channel) {
	case TEST_MAF_CHANNEL:
		return testMafValue;
	case TEST_CLT_CHANNEL:
		return testCltValue;
	case TEST_IAT_CHANNEL:
		return testIatValue;
	}
	return 0;
}

float getVoltage(const char *msg, int channel) {
	return 0;
}

int getAdcValue(const char *msg, int channel) {
	return 0;
}


