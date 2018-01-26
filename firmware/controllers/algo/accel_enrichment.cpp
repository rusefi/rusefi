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
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "main.h"
#include "trigger_central.h"
#include "accel_enrichment.h"
#include "engine_state.h"
#include "engine_math.h"
#include "signal_executor.h"
#if !EFI_UNIT_TEST || defined(__DOXYGEN__)
#include "tunerstudio_configuration.h"
extern TunerStudioOutputChannels tsOutputChannels;
#endif /* EFI_UNIT_TEST */

EXTERN_ENGINE
;


tps_tps_Map3D_t tpsTpsMap("tpsTps");

static Logging *logger = NULL;

WallFuel::WallFuel() {
	reset();
}

void WallFuel::reset() {
	memset(wallFuel, 0, sizeof(wallFuel));
}

floatms_t WallFuel::adjust(int injectorIndex, floatms_t target DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (cisnan(target)) {
		return target;
	}
	// disable this correction for cranking
	if (ENGINE(rpmCalculator).isCranking(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		return target;
	}
	float addedToWallCoef = CONFIG(addedToWallCoef);

	/**
	 * What amount of fuel is sucked of the walls, based on current amount of fuel on the wall.
	 */
	floatms_t suckedOffWallsAmount = wallFuel[injectorIndex] * CONFIG(suckedOffCoef);

	floatms_t adjustedFuelPulse = (target - suckedOffWallsAmount) / (1 - addedToWallCoef);

	// We can't inject a negative amount of fuel
	// If this goes below zero we will be over-fueling slightly,
	// but that's ok.
	if(adjustedFuelPulse < 0) {
		adjustedFuelPulse = 0;
	}

	float addedToWallsAmount = adjustedFuelPulse * addedToWallCoef;
	wallFuel[injectorIndex] += addedToWallsAmount - suckedOffWallsAmount;
	engine->wallFuelCorrection = adjustedFuelPulse - target;
	return adjustedFuelPulse;
}

floatms_t WallFuel::getWallFuel(int injectorIndex) {
	return wallFuel[injectorIndex];
}

int AccelEnrichmemnt::getMaxDeltaIndex(DECLARE_ENGINE_PARAMETER_SIGNATURE) {

	int len = minI(cb.getSize(), cb.getCount());
	if (len < 2)
		return 0;
	int ci = cb.currentIndex - 1;
	float maxValue = cb.get(ci) - cb.get(ci - 1);
	int resultIndex = ci;

	// todo: 'get' method is maybe a bit heavy because of the branching
	// todo: this could be optimized with some careful magic

	for (int i = 1; i<len - 1;i++) {
		float v = cb.get(ci - i) - cb.get(ci - i - 1);
		if (v > maxValue) {
			maxValue = v;
			resultIndex = ci - i;
		}
	}

	return resultIndex;
}

float AccelEnrichmemnt::getMaxDelta(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	int index = getMaxDeltaIndex(PASS_ENGINE_PARAMETER_SIGNATURE);

	return (cb.get(index) - (cb.get(index - 1))) * CONFIG(specs.cylindersCount);
}

// todo: eliminate code duplication between these two methods! Some pointer magic would help.
floatms_t AccelEnrichmemnt::getTpsEnrichment(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	int index = getMaxDeltaIndex(PASS_ENGINE_PARAMETER_SIGNATURE);

//	FuelSchedule *fs = engine->engineConfiguration2->injectionEvents;
	float tpsTo = cb.get(index);
	float tpsFrom = cb.get(index - 1);
	float d = tpsTo - tpsFrom;

	float valueFromTable = tpsTpsMap.getValue(tpsFrom, tpsTo);

	floatms_t extraFuel;
	if (d > engineConfiguration->tpsAccelEnrichmentThreshold) {
		extraFuel = valueFromTable;
	} else if (d < -engineConfiguration->tpsDecelEnleanmentThreshold) {
		extraFuel = d * engineConfiguration->tpsDecelEnleanmentMultiplier;
	} else {
		extraFuel = 0;
	}

#if !EFI_UNIT_TEST || defined(__DOXYGEN__)
	if (engineConfiguration->debugMode == DBG_TPS_ACCEL) {
		tsOutputChannels.debugFloatField1 = tpsFrom;
		tsOutputChannels.debugFloatField2 = tpsTo;
		tsOutputChannels.debugFloatField3 = valueFromTable;
		tsOutputChannels.debugFloatField4 = extraFuel;
	}
#endif


	return extraFuel;
}

float AccelEnrichmemnt::getEngineLoadEnrichment(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	int index = getMaxDeltaIndex(PASS_ENGINE_PARAMETER_SIGNATURE);

	float d = (cb.get(index) - (cb.get(index - 1))) * CONFIG(specs.cylindersCount);

	float result = 0;
	int distance = 0;
	float taper = 0;
	if (d > engineConfiguration->engineLoadAccelEnrichmentThreshold) {

		int distance = cb.currentIndex - index;
		if (distance <= 0) // checking if indexes are out of order due to circular buffer nature
			distance += minI(cb.getCount(), cb.getSize());

		taper = interpolate2d("accel", distance, engineConfiguration->mapAccelTaperBins, engineConfiguration->mapAccelTaperMult, MAP_ACCEL_TAPER);

		result = taper * d * engineConfiguration->engineLoadAccelEnrichmentMultiplier;
	} else if (d < -engineConfiguration->engineLoadDecelEnleanmentThreshold) {
		result = d * engineConfiguration->engineLoadAccelEnrichmentMultiplier;
	}

#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)
	if (engineConfiguration->debugMode == DBG_EL_ACCEL) {
		tsOutputChannels.debugIntField1 = distance;
		tsOutputChannels.debugFloatField1 = result;
		tsOutputChannels.debugFloatField2 = taper;

	}
#endif
	return result;
}

void AccelEnrichmemnt::reset() {
	cb.clear();
	previousValue = NAN;
}

void AccelEnrichmemnt::setLength(int length) {
	cb.setSize(length);
}

void AccelEnrichmemnt::onNewValue(float currentValue DECLARE_ENGINE_PARAMETER_SUFFIX) {
	cb.add(currentValue);
}

void AccelEnrichmemnt::onEngineCycleTps(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	onNewValue(getTPS(PASS_ENGINE_PARAMETER_SIGNATURE) PASS_ENGINE_PARAMETER_SUFFIX);
}

void AccelEnrichmemnt::onEngineCycle(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	onNewValue(getEngineLoadT(PASS_ENGINE_PARAMETER_SIGNATURE) PASS_ENGINE_PARAMETER_SUFFIX);
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
	scheduleMsg(logger, "EL accel th=%.2f/mult=%.2f", engineConfiguration->engineLoadAccelEnrichmentThreshold, engineConfiguration->engineLoadAccelEnrichmentMultiplier);
	scheduleMsg(logger, "EL decel th=%.2f/mult=%.2f", engineConfiguration->engineLoadDecelEnleanmentThreshold, engineConfiguration->engineLoadDecelEnleanmentMultiplier);

//	scheduleMsg(logger, "TPS accel length=%d", tpsInstance.cb.getSize());
	scheduleMsg(logger, "TPS accel th=%.2f/mult=%.2f", engineConfiguration->tpsAccelEnrichmentThreshold, -1);

	scheduleMsg(logger, "added to wall=%.2f/sucked=%.2f", engineConfiguration->addedToWallCoef, engineConfiguration->suckedOffCoef);
}

void setEngineLoadAccelThr(float value) {
	engineConfiguration->engineLoadAccelEnrichmentThreshold = value;
	accelInfo();
}

void setEngineLoadAccelMult(float value) {
	engineConfiguration->engineLoadAccelEnrichmentMultiplier = value;
	accelInfo();
}

void setTpsAccelThr(float value) {
	engineConfiguration->tpsAccelEnrichmentThreshold = value;
	accelInfo();
}

void setTpsDecelThr(float value) {
	engineConfiguration->tpsDecelEnleanmentThreshold = value;
	accelInfo();
}

void setTpsDecelMult(float value) {
	engineConfiguration->tpsDecelEnleanmentMultiplier = value;
	accelInfo();
}

void setDecelThr(float value) {
	engineConfiguration->engineLoadDecelEnleanmentThreshold = value;
	accelInfo();
}

void setDecelMult(float value) {
	engineConfiguration->engineLoadDecelEnleanmentMultiplier = value;
	accelInfo();
}

void setTpsAccelLen(int length) {
	if (length < 1) {
		scheduleMsg(logger, "Length should be positive");
		return;
	}
	engine->tpsAccelEnrichment.setLength(length);
	accelInfo();
}

void setEngineLoadAccelLen(int length) {
	if (length < 1) {
		scheduleMsg(logger, "Length should be positive");
		return;
	}
	engine->engineLoadAccelEnrichment.setLength(length);
	accelInfo();
}

void updateAccelParameters() {
	setEngineLoadAccelLen(engineConfiguration->engineLoadAccelLength);
	setTpsAccelLen(engineConfiguration->tpsAccelLength);
}

#endif /* ! EFI_UNIT_TEST */


void initAccelEnrichment(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX) {
	logger = sharedLogger;
	tpsTpsMap.init(config->tpsTpsAccelTable, config->tpsTpsAccelFromRpmBins, config->tpsTpsAccelToRpmBins);

#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)

	addConsoleAction("accelinfo", accelInfo);

	updateAccelParameters();
#endif /* ! EFI_UNIT_TEST */
}

