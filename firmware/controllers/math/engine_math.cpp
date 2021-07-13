/**
 * @file	engine_math.cpp
 * @brief
 *
 * @date Jul 13, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 * This file is part of rusEfi - see http://rusefi.com
 *
 * rusEfi is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "global.h"
#include "engine_math.h"
#include "engine_configuration.h"
#include "interpolation.h"
#include "allsensors.h"
#include "sensor.h"
#include "event_registry.h"
#include "efi_gpio.h"
#include "fuel_math.h"
#include "advance_map.h"

EXTERN_ENGINE;
#if EFI_UNIT_TEST
extern bool verboseMode;
#endif /* EFI_UNIT_TEST */

floatms_t getEngineCycleDuration(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX) {
	return getCrankshaftRevolutionTimeMs(rpm) * (engine->getOperationMode(PASS_ENGINE_PARAMETER_SIGNATURE) == TWO_STROKE ? 1 : 2);
}

/**
 * @return number of milliseconds in one crank shaft revolution
 */
floatms_t getCrankshaftRevolutionTimeMs(int rpm) {
	if (rpm == 0) {
		return NAN;
	}
	return 360 * getOneDegreeTimeMs(rpm);
}

float getFuelingLoad(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return ENGINE(engineState.fuelingLoad);
}

float getIgnitionLoad(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return ENGINE(engineState.ignitionLoad);
}

/**
 * see also setConstantDwell
 */
void setSingleCoilDwell(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	for (int i = 0; i < DWELL_CURVE_SIZE; i++) {
		engineConfiguration->sparkDwellRpmBins[i] = i + 1;
		engineConfiguration->sparkDwellValues[i] = 4;
	}

	engineConfiguration->sparkDwellRpmBins[5] = 10;
	engineConfiguration->sparkDwellValues[5] = 4;

	engineConfiguration->sparkDwellRpmBins[6] = 4500;
	engineConfiguration->sparkDwellValues[6] = 4;

	engineConfiguration->sparkDwellRpmBins[7] = 12500;
	engineConfiguration->sparkDwellValues[7] = 0;
}

/**
 * @return Spark dwell time, in milliseconds. 0 if tables are not ready.
 */
floatms_t getSparkDwell(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT
	float dwellMs;
	if (ENGINE(rpmCalculator).isCranking()) {
		dwellMs = CONFIG(ignitionDwellForCrankingMs);
	} else {
		efiAssert(CUSTOM_ERR_ASSERT, !cisnan(rpm), "invalid rpm", NAN);

		dwellMs = interpolate2d(rpm, engineConfiguration->sparkDwellRpmBins, engineConfiguration->sparkDwellValues);
	}

	if (cisnan(dwellMs) || dwellMs <= 0) {
		// this could happen during engine configuration reset
		warning(CUSTOM_ERR_DWELL_DURATION, "invalid dwell: %.2f at rpm=%d", dwellMs, rpm);
		return 0;
	}
	return dwellMs;
#else
	return 0;
#endif
}

static const int order_1[] = {1};

static const int order_1_2[] = {1, 2};

static const int order_1_2_3[] = {1, 2, 3};
static const int order_1_3_2[] = {1, 3, 2};
// 4 cylinder

static const int order_1_THEN_3_THEN_4_THEN2[] = { 1, 3, 4, 2 };
static const int order_1_THEN_2_THEN_4_THEN3[] = { 1, 2, 4, 3 };
static const int order_1_THEN_3_THEN_2_THEN4[] = { 1, 3, 2, 4 };
static const int order_1_THEN_4_THEN_3_THEN2[] = { 1, 4, 3, 2 };

// 5 cylinder
static const int order_1_2_4_5_3[] = {1, 2, 4, 5, 3};

// 6 cylinder
static const int order_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4[] = { 1, 5, 3, 6, 2, 4 };
static const int order_1_THEN_4_THEN_2_THEN_5_THEN_3_THEN_6[] = { 1, 4, 2, 5, 3, 6 };
static const int order_1_THEN_2_THEN_3_THEN_4_THEN_5_THEN_6[] = { 1, 2, 3, 4, 5, 6 };
static const int order_1_6_3_2_5_4[] = {1, 6, 3, 2, 5, 4};
static const int order_1_4_3_6_2_5[] = {1, 4, 3, 6, 2, 5};

// 8 cylinder
static const int order_1_8_4_3_6_5_7_2[] = { 1, 8, 4, 3, 6, 5, 7, 2 };
static const int order_1_8_7_2_6_5_4_3[] = { 1, 8, 7, 2, 6, 5, 4, 3 };
static const int order_1_5_4_2_6_3_7_8[] = { 1, 5, 4, 2, 6, 3, 7, 8 };
static const int order_1_2_7_8_4_5_6_3[] = { 1, 2, 7, 8, 4, 5, 6, 3 };
static const int order_1_3_7_2_6_5_4_8[] = { 1, 3, 7, 2, 6, 5, 4, 8 };
static const int order_1_2_3_4_5_6_7_8[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
static const int order_1_5_4_8_6_3_7_2[] = { 1, 5, 4, 8, 6, 3, 7, 2 };

// 9 cylinder
static const int order_1_2_3_4_5_6_7_8_9[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

// 10 cylinder
static const int order_1_10_9_4_3_6_5_8_7_2[] = {1, 10, 9, 4, 3, 6, 5, 8, 7, 2};

// 12 cyliner
static const int order_1_7_5_11_3_9_6_12_2_8_4_10[] = {1, 7, 5, 11, 3, 9, 6, 12, 2, 8, 4, 10};
static const int order_1_7_4_10_2_8_6_12_3_9_5_11[] = {1, 7, 4, 10, 2, 8, 6, 12, 3, 9, 5, 11};
static const int order_1_12_5_8_3_10_6_7_2_11_4_9[] = {1, 12, 5, 8, 3, 10, 6, 7, 2, 11, 4, 9};
static const int order_1_2_3_4_5_6_7_8_9_10_11_12[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

// no comments
static const int order_1_14_9_4_7_12_15_6_13_8_3_16_11_2_5_10[] = {1, 14, 9, 4, 7, 12, 15, 6, 13, 8, 3, 16, 11, 2, 5, 10};

static int getFiringOrderLength(DECLARE_ENGINE_PARAMETER_SIGNATURE) {

	switch (CONFIG(specs.firingOrder)) {
	case FO_1:
		return 1;
// 2 cylinder
	case FO_1_2:
		return 2;
// 3 cylinder
	case FO_1_2_3:
	case FO_1_3_2:
		return 3;
// 4 cylinder
	case FO_1_3_4_2:
	case FO_1_2_4_3:
	case FO_1_3_2_4:
	case FO_1_4_3_2:
		return 4;
// 5 cylinder
	case FO_1_2_4_5_3:
		return 5;

// 6 cylinder
	case FO_1_5_3_6_2_4:
	case FO_1_4_2_5_3_6:
	case FO_1_2_3_4_5_6:
	case FO_1_6_3_2_5_4:
	case FO_1_4_3_6_2_5:
		return 6;

// 8 cylinder
	case FO_1_8_4_3_6_5_7_2:
	case FO_1_8_7_2_6_5_4_3:
	case FO_1_5_4_2_6_3_7_8:
	case FO_1_2_7_8_4_5_6_3:
	case FO_1_3_7_2_6_5_4_8:
	case FO_1_2_3_4_5_6_7_8:
	case FO_1_5_4_8_6_3_7_2:
		return 8;

// 9 cylinder radial
	case FO_1_2_3_4_5_6_7_8_9:
		return 9;

// 10 cylinder
	case FO_1_10_9_4_3_6_5_8_7_2:
		return 10;

// 12 cylinder
	case FO_1_7_5_11_3_9_6_12_2_8_4_10:
	case FO_1_7_4_10_2_8_6_12_3_9_5_11:
	case FO_1_12_5_8_3_10_6_7_2_11_4_9:
	case FO_1_2_3_4_5_6_7_8_9_10_11_12:
		return 12;

	case FO_1_14_9_4_7_12_15_6_13_8_3_16_11_2_5_10:
		return 16;

	default:
		firmwareError(CUSTOM_OBD_UNKNOWN_FIRING_ORDER, "Invalid firing order: %d", CONFIG(specs.firingOrder));
	}
	return 1;
}

static const int *getFiringOrderTable(DECLARE_ENGINE_PARAMETER_SIGNATURE)
{
	switch (CONFIG(specs.firingOrder)) {
	case FO_1:
		return order_1;
// 2 cylinder
	case FO_1_2:
		return order_1_2;
// 3 cylinder
	case FO_1_2_3:
		return order_1_2_3;
	case FO_1_3_2:
		return order_1_3_2;
// 4 cylinder
	case FO_1_3_4_2:
		return order_1_THEN_3_THEN_4_THEN2;
	case FO_1_2_4_3:
		return order_1_THEN_2_THEN_4_THEN3;
	case FO_1_3_2_4:
		return order_1_THEN_3_THEN_2_THEN4;
	case FO_1_4_3_2:
		return order_1_THEN_4_THEN_3_THEN2;
// 5 cylinder
	case FO_1_2_4_5_3:
		return order_1_2_4_5_3;

// 6 cylinder
	case FO_1_5_3_6_2_4:
		return order_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4;
	case FO_1_4_2_5_3_6:
		return order_1_THEN_4_THEN_2_THEN_5_THEN_3_THEN_6;
	case FO_1_2_3_4_5_6:
		return order_1_THEN_2_THEN_3_THEN_4_THEN_5_THEN_6;
	case FO_1_6_3_2_5_4:
		return order_1_6_3_2_5_4;
	case FO_1_4_3_6_2_5:
		return order_1_4_3_6_2_5;

// 8 cylinder
	case FO_1_8_4_3_6_5_7_2:
		return order_1_8_4_3_6_5_7_2;
	case FO_1_8_7_2_6_5_4_3:
		return order_1_8_7_2_6_5_4_3;
	case FO_1_5_4_2_6_3_7_8:
		return order_1_5_4_2_6_3_7_8;
	case FO_1_2_7_8_4_5_6_3:
		return order_1_2_7_8_4_5_6_3;
	case FO_1_3_7_2_6_5_4_8:
		return order_1_3_7_2_6_5_4_8;
	case FO_1_2_3_4_5_6_7_8:
		return order_1_2_3_4_5_6_7_8;
	case FO_1_5_4_8_6_3_7_2:
		return order_1_5_4_8_6_3_7_2;

// 9 cylinder
	case FO_1_2_3_4_5_6_7_8_9:
		return order_1_2_3_4_5_6_7_8_9;


// 10 cylinder
	case FO_1_10_9_4_3_6_5_8_7_2:
		return order_1_10_9_4_3_6_5_8_7_2;

// 12 cylinder
	case FO_1_7_5_11_3_9_6_12_2_8_4_10:
		return order_1_7_5_11_3_9_6_12_2_8_4_10;
	case FO_1_7_4_10_2_8_6_12_3_9_5_11:
		return order_1_7_4_10_2_8_6_12_3_9_5_11;
	case FO_1_12_5_8_3_10_6_7_2_11_4_9:
		return order_1_12_5_8_3_10_6_7_2_11_4_9;
	case FO_1_2_3_4_5_6_7_8_9_10_11_12:
		return order_1_2_3_4_5_6_7_8_9_10_11_12;

// do not ask
	case FO_1_14_9_4_7_12_15_6_13_8_3_16_11_2_5_10:
		return order_1_14_9_4_7_12_15_6_13_8_3_16_11_2_5_10;

	default:
		firmwareError(CUSTOM_OBD_UNKNOWN_FIRING_ORDER, "Invalid firing order: %d", CONFIG(specs.firingOrder));
	}

	return NULL;
}

/**
 * @param index from zero to cylindersCount - 1
 * @return cylinderId from one to cylindersCount
 */
int getCylinderId(int index DECLARE_ENGINE_PARAMETER_SUFFIX) {

	const int firingOrderLength = getFiringOrderLength(PASS_ENGINE_PARAMETER_SIGNATURE);

	if (firingOrderLength < 1 || firingOrderLength > MAX_CYLINDER_COUNT) {
		firmwareError(CUSTOM_ERR_6687, "fol %d", firingOrderLength);
		return 1;
	}
	if (engineConfiguration->specs.cylindersCount != firingOrderLength) {
		// May 2020 this somehow still happens with functional tests, maybe race condition?
		warning(CUSTOM_OBD_WRONG_FIRING_ORDER, "Wrong cyl count for firing order, expected %d cylinders", firingOrderLength);
		return 1;
	}

	if (index < 0 || index >= firingOrderLength) {
		// May 2020 this somehow still happens with functional tests, maybe race condition?
		warning(CUSTOM_ERR_6686, "firing order index %d", index);
		return 1;
	}

	const int *firingOrderTable = getFiringOrderTable(PASS_ENGINE_PARAMETER_SIGNATURE);
	if (firingOrderTable)
		return firingOrderTable[index];
	/* else
		error already reported */

	return 1;
}

/**
 * @param prevCylinderId from one to cylindersCount
 * @return cylinderId from one to cylindersCount
 */
int getNextFiringCylinderId(int prevCylinderId DECLARE_ENGINE_PARAMETER_SUFFIX) {
	const int firingOrderLength = getFiringOrderLength(PASS_ENGINE_PARAMETER_SIGNATURE);
	const int *firingOrderTable = getFiringOrderTable(PASS_ENGINE_PARAMETER_SIGNATURE);

	if (firingOrderTable) {
		for (size_t i = 0; i < firingOrderLength; i++)
			if (firingOrderTable[i] == prevCylinderId)
				return firingOrderTable[(i + 1) % firingOrderLength];
	}
	return 1;
}

/**
 * @param cylinderIndex from 0 to cylinderCount, not cylinder number
 */
static int getIgnitionPinForIndex(int cylinderIndex DECLARE_ENGINE_PARAMETER_SUFFIX) {
	switch (getCurrentIgnitionMode(PASS_ENGINE_PARAMETER_SIGNATURE)) {
	case IM_ONE_COIL:
		return 0;
	case IM_WASTED_SPARK: {
		if (CONFIG(specs.cylindersCount) == 1) {
			// we do not want to divide by zero
			return 0;
		}
		return cylinderIndex % (CONFIG(specs.cylindersCount) / 2);
	}
	case IM_INDIVIDUAL_COILS:
		return cylinderIndex;
	case IM_TWO_COILS:
		return cylinderIndex % 2;

	default:
		firmwareError(CUSTOM_OBD_IGNITION_MODE, "Invalid ignition mode getIgnitionPinForIndex(): %d", engineConfiguration->ignitionMode);
		return 0;
	}
}

void prepareIgnitionPinIndices(ignition_mode_e ignitionMode DECLARE_ENGINE_PARAMETER_SUFFIX) {
	(void)ignitionMode;
#if EFI_ENGINE_CONTROL
	for (size_t cylinderIndex = 0; cylinderIndex < CONFIG(specs.cylindersCount); cylinderIndex++) {
		ENGINE(ignitionPin[cylinderIndex]) = getIgnitionPinForIndex(cylinderIndex PASS_ENGINE_PARAMETER_SUFFIX);
	}
#endif /* EFI_ENGINE_CONTROL */
}

/**
 * @return IM_WASTED_SPARK if in SPINNING mode and IM_INDIVIDUAL_COILS setting
 * @return CONFIG(ignitionMode) otherwise
 */
ignition_mode_e getCurrentIgnitionMode(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	ignition_mode_e ignitionMode = CONFIG(ignitionMode);
#if EFI_SHAFT_POSITION_INPUT
	// In spin-up cranking mode we don't have full phase sync. info yet, so wasted spark mode is better
	if (ignitionMode == IM_INDIVIDUAL_COILS && ENGINE(rpmCalculator.isSpinningUp()))
		ignitionMode = IM_WASTED_SPARK;
#endif /* EFI_SHAFT_POSITION_INPUT */
	return ignitionMode;
}

#if EFI_ENGINE_CONTROL

/**
 * This heavy method is only invoked in case of a configuration change or initialization.
 */
void prepareOutputSignals(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	ENGINE(engineCycle) = getEngineCycle(engine->getOperationMode(PASS_ENGINE_PARAMETER_SIGNATURE));

	angle_t maxTimingCorrMap = -720.0f;
	angle_t maxTimingMap = -720.0f;
	for (int rpmIndex = 0;rpmIndex<IGN_RPM_COUNT;rpmIndex++) {
		for (int l = 0;l<IGN_LOAD_COUNT;l++) {
			maxTimingCorrMap = maxF(maxTimingCorrMap, config->ignitionIatCorrTable[l][rpmIndex]);
			maxTimingMap = maxF(maxTimingMap, config->ignitionTable[l][rpmIndex]);
		}
	}

#if EFI_UNIT_TEST
	if (verboseMode) {
		printf("prepareOutputSignals %d onlyEdge=%s %s\r\n", engineConfiguration->trigger.type, boolToString(engineConfiguration->useOnlyRisingEdgeForTrigger),
				getIgnition_mode_e(engineConfiguration->ignitionMode));
	}
#endif /* EFI_UNIT_TEST */

	for (size_t i = 0; i < CONFIG(specs.cylindersCount); i++) {
		ENGINE(ignitionPositionWithinEngineCycle[i]) = ENGINE(engineCycle) * i / CONFIG(specs.cylindersCount);
	}

	prepareIgnitionPinIndices(CONFIG(ignitionMode) PASS_ENGINE_PARAMETER_SUFFIX);

	TRIGGER_WAVEFORM(prepareShape(&ENGINE(triggerCentral.triggerFormDetails) PASS_ENGINE_PARAMETER_SUFFIX));

	// Fuel schedule may now be completely wrong, force a reset
	ENGINE(injectionEvents).invalidate();
}

void setTimingRpmBin(float from, float to DECLARE_CONFIG_PARAMETER_SUFFIX) {
	setRpmBin(config->ignitionRpmBins, IGN_RPM_COUNT, from, to);
}

void setTimingLoadBin(float from, float to DECLARE_CONFIG_PARAMETER_SUFFIX) {
	setLinearCurve(config->ignitionLoadBins, from, to);
}

/**
 * this method sets algorithm and ignition table scale
 */
void setAlgorithm(engine_load_mode_e algo DECLARE_CONFIG_PARAMETER_SUFFIX) {
	engineConfiguration->fuelAlgorithm = algo;
	if (algo == LM_SPEED_DENSITY) {
		setLinearCurve(config->ignitionLoadBins, 20, 120, 3);
		buildTimingMap(35 PASS_CONFIG_PARAMETER_SUFFIX);
	}
}

void setFlatInjectorLag(float value DECLARE_CONFIG_PARAMETER_SUFFIX) {
	setArrayValues(engineConfiguration->injector.battLagCorr, value);
}

#endif /* EFI_ENGINE_CONTROL */
