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

// we have this '100'  magic constant too often for two many other reasons todo: refactor further?
#define POSITION_FULLY_OPEN 100

constexpr inline int convertVoltageTo10bitADC(float voltage) {
	return (int) (voltage * TPS_TS_CONVERSION);
}

void grabTPSIsClosed();
void grabTPSIsWideOpen();
void grabPedalIsUp();
void grapTps1PrimaryIsClosed();
void grapTps1PrimaryIsOpen();
void grabPedalIsWideOpen();

#if EFI_SENT_SUPPORT

struct SentTps : public StoredValueSensor {
	SentTps() : StoredValueSensor(SensorType::Tps1, MS2NT(200)) {
	}

	bool isRedundant() const override {
		return true;
	}
};

void sentTpsDecode(SentInput sentCh);
float decodeTpsSentValue(float sentValue);
bool isDigitalTps1();

#endif
