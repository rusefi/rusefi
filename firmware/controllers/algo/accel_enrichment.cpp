/**
 * @file    accel_enrichment.cpp
 * @brief   Acceleration enrichment calculator
 *
 * In this file we have three strategies for acceleration/deceleration fuel correction
 *
 * 1) MAP rate-of-change correction
 * 2) TPS rate-of-change correction
 * 3) fuel film/wal wetting correction
 *   AWC Added to Wall Coefficient, %
 *   AWA Added to Wall Amount
 *   SOC Sucked Off wall Coefficient, %
 *   SOA Sucked Off wall amount
 *   WF  current on-Wall Fuel amount
 *
 *
 * http://rusefi.com/wiki/index.php?title=Manual:Software:Fuel_Control
 * @date Apr 21, 2014
 * @author Dmitry Sidin
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#include "main.h"
#include "trigger_central.h"
#include "accel_enrichment.h"
#include "engine_state.h"
#include "engine_math.h"
#include "signal_executor.h"

EXTERN_ENGINE
;

static Logging *logger = NULL;

WallFuel::WallFuel() {
	reset();
}

void WallFuel::reset() {
	memset(wallFuel, 0, sizeof(wallFuel));
}

floatms_t WallFuel::adjust(int injectorIndex, floatms_t target DECLARE_ENGINE_PARAMETER_S) {
	if (cisnan(target)) {
		return target;
	}
	float addedToWallCoef = CONFIG(addedToWallCoef);

	/**
	 * What amount of fuel is sucked of the walls, based on current amount of fuel on the wall.
	 */
	floatms_t suckedOffWallsAmount = wallFuel[injectorIndex] * CONFIG(suckedOffCoef);

	floatms_t adjustedFuelPulse = (target - suckedOffWallsAmount) / (1 - addedToWallCoef);

	float addedToWallsAmount = adjustedFuelPulse * addedToWallCoef;
	wallFuel[injectorIndex] += addedToWallsAmount - suckedOffWallsAmount;
	engine->wallFuelCorrection = adjustedFuelPulse - target;
	return adjustedFuelPulse;
}

floatms_t WallFuel::getWallFuel(int injectorIndex) {
	return wallFuel[injectorIndex];
}

float AccelEnrichmemnt::getMaxDelta() {
	if (cb.getCount() == 0)
		return 0; // no recent data
	return cb.maxValue(cb.getSize());
}

// todo: eliminate code duplication between these two methods! Some pointer magic would help.
floatms_t AccelEnrichmemnt::getTpsEnrichment(DECLARE_ENGINE_PARAMETER_F) {
	float d = getMaxDelta();
	if (d > engineConfiguration->tpsAccelEnrichmentThreshold) {
		return d * engineConfiguration->tpsAccelEnrichmentMultiplier;
	}
	if (d < -engineConfiguration->tpsDecelEnleanmentThreshold) {
		return d * engineConfiguration->tpsDecelEnleanmentMultiplier;
	}
	return 0;
}

float AccelEnrichmemnt::getEngineLoadEnrichment(DECLARE_ENGINE_PARAMETER_F) {
	float d = getMaxDelta();
	if (d > engineConfiguration->engineLoadAccelEnrichmentThreshold) {
		return d * engineConfiguration->engineLoadAccelEnrichmentMultiplier;
	}
	if (d < -engineConfiguration->engineLoadDecelEnleanmentThreshold) {
		return d * engineConfiguration->engineLoadAccelEnrichmentMultiplier;
	}
	return 0;
}

void AccelEnrichmemnt::reset() {
	cb.clear();
	previousValue = NAN;
}

void AccelEnrichmemnt::onNewValue(float currentValue DECLARE_ENGINE_PARAMETER_S) {
	if (!cisnan(previousValue)) {
		/**
		 * this could be negative, zero or positive
		 */
		float delta = currentValue - previousValue;
		FuelSchedule *fs = engine->engineConfiguration2->injectionEvents;
		cb.add(delta * fs->eventsCount);
	}

	previousValue = currentValue;
}

void AccelEnrichmemnt::onEngineCycleTps(DECLARE_ENGINE_PARAMETER_F) {
	onNewValue(getTPS(PASS_ENGINE_PARAMETER_F) PASS_ENGINE_PARAMETER);
}

void AccelEnrichmemnt::onEngineCycle(DECLARE_ENGINE_PARAMETER_F) {
	onNewValue(getEngineLoadT(PASS_ENGINE_PARAMETER_F) PASS_ENGINE_PARAMETER);
}

AccelEnrichmemnt::AccelEnrichmemnt() {
	reset();
	cb.setSize(4);
}

#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)

static void accelInfo() {
	if (logger == NULL)
		return;
//	scheduleMsg(logger, "EL accel length=%d", mapInstance.cb.getSize());
	scheduleMsg(logger, "EL accel th=%f/mult=%f", engineConfiguration->engineLoadAccelEnrichmentThreshold, engineConfiguration->engineLoadAccelEnrichmentMultiplier);
	scheduleMsg(logger, "EL decel th=%f/mult=%f", engineConfiguration->engineLoadDecelEnleanmentThreshold, engineConfiguration->engineLoadDecelEnleanmentMultiplier);

//	scheduleMsg(logger, "TPS accel length=%d", tpsInstance.cb.getSize());
	scheduleMsg(logger, "TPS accel th=%f/mult=%f", engineConfiguration->tpsAccelEnrichmentThreshold, engineConfiguration->tpsAccelEnrichmentMultiplier);

	scheduleMsg(logger, "added to wall=%f/sucked=%f", engineConfiguration->addedToWallCoef, engineConfiguration->suckedOffCoef);
}

void setEngineLoadAccelThr(float value) {
	engineConfiguration->engineLoadAccelEnrichmentThreshold = value;
	accelInfo();
}

void setEngineLoadAccelMult(float value) {
	engineConfiguration->engineLoadAccelEnrichmentMultiplier = value;
	accelInfo();
}

static void setTpsAccelThr(float value) {
	engineConfiguration->tpsAccelEnrichmentThreshold = value;
	accelInfo();
}

static void setTpsAccelMult(float value) {
	engineConfiguration->tpsAccelEnrichmentMultiplier = value;
	accelInfo();
}

static void setTpsDecelThr(float value) {
	engineConfiguration->tpsDecelEnleanmentThreshold = value;
	accelInfo();
}

static void setTpsDecelMult(float value) {
	engineConfiguration->tpsDecelEnleanmentMultiplier = value;
	accelInfo();
}

static void setDecelThr(float value) {
	engineConfiguration->engineLoadDecelEnleanmentThreshold = value;
	accelInfo();
}

static void setDecelMult(float value) {
	engineConfiguration->engineLoadDecelEnleanmentMultiplier = value;
	accelInfo();
}

static void setTpsAccelLen(int len) {
	if (len < 1) {
		scheduleMsg(logger, "Length should be positive");
		return;
	}
	engine->tpsAccelEnrichment.cb.setSize(len);
	accelInfo();
}

void setEngineLoadAccelLen(int len) {
	if (len < 1) {
		scheduleMsg(logger, "Length should be positive");
		return;
	}
	engine->engineLoadAccelEnrichment.cb.setSize(len);
	accelInfo();
}

void updateAccelParameters() {
	setEngineLoadAccelLen(engineConfiguration->engineLoadAccelLength);
	setTpsAccelLen(engineConfiguration->tpsAccelLength);
}

void initAccelEnrichment(Logging *sharedLogger) {
	logger = sharedLogger;
	addConsoleActionI("set_tps_accel_len", setTpsAccelLen);
	addConsoleActionF("set_tps_accel_threshold", setTpsAccelThr);
	addConsoleActionF("set_tps_accel_multiplier", setTpsAccelMult);
	addConsoleActionF("set_tps_decel_threshold", setTpsDecelThr);
	addConsoleActionF("set_tps_decel_multiplier", setTpsDecelMult);

	addConsoleActionI("set_engine_load_accel_len", setEngineLoadAccelLen);
	addConsoleActionF("set_engine_load_accel_threshold", setEngineLoadAccelThr);
	addConsoleActionF("set_engine_load_accel_multiplier", setEngineLoadAccelMult);
	addConsoleActionF("set_engine_decel_threshold", setDecelThr);
	addConsoleActionF("set_engine_decel_multiplier", setDecelMult);
	addConsoleAction("accelinfo", accelInfo);

	updateAccelParameters();
}
#endif /* ! EFI_UNIT_TEST */
