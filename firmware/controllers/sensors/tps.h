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

// todo: '_TS_' part here no longer makes sense now that we use volts!
// Scaled to 1000 counts = 5.0 volts
#define TPS_TS_CONVERSION (1 / PACK_MULT_RAW_VOLTAGE)

// we have this '100'  magic constant too often for two many other reasons todo: refactor further?
#define POSITION_FULLY_OPEN 100

// todo: looks like we need to rename this method since we no longer store sensors in ADC units?
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
