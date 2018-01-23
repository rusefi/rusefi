/**
 * @file board.c
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "boards.h"

float testMafValue = 0;

float getVoltageDivided(const char *msg, int channel) {
	switch(channel) {
	case TEST_MAF_CHANNEL:
		return testMafValue;
	}
	return 0;
}

float getVoltage(const char *msg, int channel) {
	return 0;
}

int getAdcValue(const char *msg, int channel) {
	return 0;
}


