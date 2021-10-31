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
 * @author Andrey Belomutskiy, (c) 2012-2020
 * @author Matthew Kennedy
 */

#include "pch.h"
#include "accel_enrichment.h"

static tps_tps_Map3D_t tpsTpsMap;

int AccelEnrichment::getMaxDeltaIndex(DECLARE_ENGINE_PARAMETER_SIGNATURE) {

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

float AccelEnrichment::getMaxDelta(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	int index = getMaxDeltaIndex(PASS_ENGINE_PARAMETER_SIGNATURE);

	return (cb.get(index) - (cb.get(index - 1)));
}

// todo: eliminate code duplication between these two methods! Some pointer magic would help.
floatms_t TpsAccelEnrichment::getTpsEnrichment(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	ScopePerf perf(PE::GetTpsEnrichment);

	int maxDeltaIndex = getMaxDeltaIndex(PASS_ENGINE_PARAMETER_SIGNATURE);

//	FuelSchedule *fs = engineConfiguration->injectionEvents;
	percent_t tpsTo = cb.get(maxDeltaIndex);
	percent_t tpsFrom = cb.get(maxDeltaIndex - 1);
	percent_t deltaTps = tpsTo - tpsFrom;

	isAboveAccelThreshold = deltaTps > engineConfiguration->tpsAccelEnrichmentThreshold;
	isBelowDecelThreshold = deltaTps < -engineConfiguration->tpsDecelEnleanmentThreshold;
	if (isAboveAccelThreshold) {
		valueFromTable = tpsTpsMap.getValue(tpsFrom, tpsTo);
		extraFuel = valueFromTable;
	} else if (isBelowDecelThreshold) {
		extraFuel = deltaTps * engineConfiguration->tpsDecelEnleanmentMultiplier;
	} else {
		extraFuel = 0;
	}

	// Fractional enrichment (fuel portions are accumulated and split between several engine cycles.
	// This is a crude imitation of carburetor's acceleration pump.
	isFractionalEnrichment = CONFIG(tpsAccelFractionPeriod) > 1 || CONFIG(tpsAccelFractionDivisor) > 1.0f;
	if (isFractionalEnrichment) {
		// make sure both values are non-zero
		float periodF = (float)maxI(CONFIG(tpsAccelFractionPeriod), 1);
		float divisor = maxF(CONFIG(tpsAccelFractionDivisor), 1.0f);

		// if current extra fuel portion is not "strong" enough, then we keep up the "pump pressure" with the accumulated portion
		floatms_t maxExtraFuel = maxF(extraFuel, accumulatedValue);
		// use only a fixed fraction of the accumulated portion
		fractionalInjFuel = maxExtraFuel / divisor;

		// update max counters
		maxExtraPerCycle = maxF(extraFuel, maxExtraPerCycle);
		maxInjectedPerPeriod = maxF(fractionalInjFuel, maxInjectedPerPeriod);

		// evenly split it between several engine cycles
		extraFuel = fractionalInjFuel / periodF;
	} else {
		resetFractionValues();
	}

#if EFI_TUNER_STUDIO
	if (engineConfiguration->debugMode == DBG_TPS_ACCEL) {
		tsOutputChannels.debugFloatField1 = tpsFrom;
		tsOutputChannels.debugFloatField2 = tpsTo;
		tsOutputChannels.debugFloatField3 = valueFromTable;
		tsOutputChannels.debugFloatField4 = extraFuel;
		tsOutputChannels.debugFloatField5 = accumulatedValue;
		tsOutputChannels.debugFloatField6 = maxExtraPerPeriod;
		tsOutputChannels.debugFloatField7 = maxInjectedPerPeriod;
		tsOutputChannels.debugIntField1 = cycleCnt;
	}
#endif /* EFI_TUNER_STUDIO */

	return extraFuel;
}

void AccelEnrichment::resetAE() {
	cb.clear();
}

void TpsAccelEnrichment::resetAE() {
	AccelEnrichment::resetAE();
	resetFractionValues();
}

void TpsAccelEnrichment::resetFractionValues() {
	accumulatedValue = 0;
	maxExtraPerCycle = 0;
	maxExtraPerPeriod = 0;
	maxInjectedPerPeriod = 0;
	cycleCnt = 0;
}

void AccelEnrichment::setLength(int length) {
	cb.setSize(length);
}

void AccelEnrichment::onNewValue(float currentValue DECLARE_ENGINE_PARAMETER_SUFFIX) {
	cb.add(currentValue);
}

void TpsAccelEnrichment::onEngineCycleTps(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	// we update values in handleFuel() directly by calling onNewValue()

	onUpdateInvocationCounter++;

	// we used some extra fuel during the current cycle, so we "charge" our "acceleration pump" with it
	accumulatedValue -= maxExtraPerPeriod;
	maxExtraPerPeriod = maxF(maxExtraPerCycle, maxExtraPerPeriod);
	maxExtraPerCycle = 0;
	accumulatedValue += maxExtraPerPeriod;

	// update the accumulated value every 'Period' engine cycles
	if (--cycleCnt <= 0) {
		maxExtraPerPeriod = 0;

		// we've injected this portion during the cycle, so we set what's left for the next cycle
		accumulatedValue -= maxInjectedPerPeriod;
		maxInjectedPerPeriod = 0;

		// it's an infinitely convergent series, so we set a limit at some point
		// (also make sure that accumulatedValue is positive, for safety)
		static const floatms_t smallEpsilon = 0.001f;
		if (accumulatedValue < smallEpsilon)
			accumulatedValue = 0;

		// reset the counter
		cycleCnt = CONFIG(tpsAccelFractionPeriod);
	}
}

AccelEnrichment::AccelEnrichment() {
	resetAE();
	cb.setSize(4);
}

#if ! EFI_UNIT_TEST

static void accelInfo() {
//	efiPrintf("TPS accel length=%d", tpsInstance.cb.getSize());
	efiPrintf("TPS accel th=%.2f/mult=%.2f", engineConfiguration->tpsAccelEnrichmentThreshold, -1);

	efiPrintf("beta=%.2f/tau=%.2f", engineConfiguration->wwaeBeta, engineConfiguration->wwaeTau);
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

void setTpsAccelLen(int length) {
	if (length < 1) {
		efiPrintf("Length should be positive");
		return;
	}
	engine->tpsAccelEnrichment.setLength(length);
	accelInfo();
}

void updateAccelParameters() {
	setTpsAccelLen(engineConfiguration->tpsAccelLength);
}

#endif /* ! EFI_UNIT_TEST */


void initAccelEnrichment(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	tpsTpsMap.init(config->tpsTpsAccelTable, config->tpsTpsAccelFromRpmBins, config->tpsTpsAccelToRpmBins);

#if ! EFI_UNIT_TEST

	addConsoleAction("accelinfo", accelInfo);

	updateAccelParameters();
#endif /* ! EFI_UNIT_TEST */
}

