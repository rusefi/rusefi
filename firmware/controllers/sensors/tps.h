/**
 * @file    tps.h
 * @brief
 *
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"
#include "engine_configuration.h"

// Scaled to 1000 counts = 5.0 volts
#define TPS_TS_CONVERSION 200

constexpr inline int convertVoltageTo10bitADC(float voltage) {
	return (int) (voltage * TPS_TS_CONVERSION);
}

void grabTPSIsClosed();
void grabTPSIsWideOpen();
void grabPedalIsUp();
void grabPedalIsWideOpen();
