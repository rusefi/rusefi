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

#include "pch.h"

#include "event_registry.h"
#include "fuel_math.h"
#include "advance_map.h"
#include "gppwm_channel.h"

#if EFI_UNIT_TEST
extern bool verboseMode;
#endif /* EFI_UNIT_TEST */

angle_t wrapAngleMethod(angle_t param, const char *msg, obd_code_e code) {
	fixAngle(param, msg, code);
	return param;
}

floatms_t getEngineCycleDuration(int rpm) {
	return getCrankshaftRevolutionTimeMs(rpm) * (getEngineRotationState()->getOperationMode() == TWO_STROKE ? 1 : 2);
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

float getFuelingLoad() {
	return getEngineState()->fuelingLoad;
}

float getIgnitionLoad() {
	return getEngineState()->ignitionLoad;
}

/**
 * see also setConstantDwell
 */
void setSingleCoilDwell() {
	for (int i = 0; i < DWELL_CURVE_SIZE; i++) {
		config->sparkDwellRpmBins[i] = (i + 1) * 50;
		config->sparkDwellValues[i] = 4;
	}

	config->sparkDwellRpmBins[5] = 500;
	config->sparkDwellValues[5] = 4;

	config->sparkDwellRpmBins[6] = 4500;
	config->sparkDwellValues[6] = 4;

	config->sparkDwellRpmBins[7] = 12500;
	config->sparkDwellValues[7] = 0;
}

/**
 * @return Spark dwell time, in milliseconds. 0 if tables are not ready.
 */
floatms_t IgnitionState::getSparkDwell(int rpm) {
#if EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT
	float dwellMs;
	if (engine->rpmCalculator.isCranking()) {
		dwellMs = engineConfiguration->ignitionDwellForCrankingMs;
	} else {
		efiAssert(CUSTOM_ERR_ASSERT, !cisnan(rpm), "invalid rpm", NAN);

		baseDwell = interpolate2d(rpm, config->sparkDwellRpmBins, config->sparkDwellValues);
		dwellVoltageCorrection = interpolate2d(
				Sensor::getOrZero(SensorType::BatteryVoltage),
				engineConfiguration->dwellVoltageCorrVoltBins,
				engineConfiguration->dwellVoltageCorrValues
		);

		// for compat (table full of zeroes)
		if (dwellVoltageCorrection < 0.1f) {
			dwellVoltageCorrection = 1;
		}

		dwellMs = baseDwell * dwellVoltageCorrection;
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

static const uint8_t order_1[] = {1};

static const uint8_t order_1_2[] = {1, 2};

static const uint8_t order_1_2_3[] = {1, 2, 3};
static const uint8_t order_1_3_2[] = {1, 3, 2};
// 4 cylinder

static const uint8_t order_1_THEN_3_THEN_4_THEN2[] = { 1, 3, 4, 2 };
static const uint8_t order_1_THEN_2_THEN_4_THEN3[] = { 1, 2, 4, 3 };
static const uint8_t order_1_THEN_3_THEN_2_THEN4[] = { 1, 3, 2, 4 };
static const uint8_t order_1_THEN_4_THEN_3_THEN2[] = { 1, 4, 3, 2 };

// 5 cylinder
static const uint8_t order_1_2_4_5_3[] = {1, 2, 4, 5, 3};

// 6 cylinder
static const uint8_t order_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4[] = { 1, 5, 3, 6, 2, 4 };
static const uint8_t order_1_THEN_4_THEN_2_THEN_5_THEN_3_THEN_6[] = { 1, 4, 2, 5, 3, 6 };
static const uint8_t order_1_THEN_2_THEN_3_THEN_4_THEN_5_THEN_6[] = { 1, 2, 3, 4, 5, 6 };
static const uint8_t order_1_6_3_2_5_4[] = {1, 6, 3, 2, 5, 4};
static const uint8_t order_1_4_3_6_2_5[] = {1, 4, 3, 6, 2, 5};
static const uint8_t order_1_6_2_4_3_5[] = {1, 6, 2, 4, 3, 5};
static const uint8_t order_1_6_5_4_3_2[] = {1, 6, 5, 4, 3, 2};
static const uint8_t order_1_4_5_2_3_6[] = {1, 4, 5, 2, 3, 6};

// 8 cylinder
static const uint8_t order_1_8_4_3_6_5_7_2[] = { 1, 8, 4, 3, 6, 5, 7, 2 };
static const uint8_t order_1_8_7_2_6_5_4_3[] = { 1, 8, 7, 2, 6, 5, 4, 3 };
static const uint8_t order_1_5_4_2_6_3_7_8[] = { 1, 5, 4, 2, 6, 3, 7, 8 };
static const uint8_t order_1_2_7_8_4_5_6_3[] = { 1, 2, 7, 8, 4, 5, 6, 3 };
static const uint8_t order_1_3_7_2_6_5_4_8[] = { 1, 3, 7, 2, 6, 5, 4, 8 };
static const uint8_t order_1_2_3_4_5_6_7_8[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
static const uint8_t order_1_5_4_8_6_3_7_2[] = { 1, 5, 4, 8, 6, 3, 7, 2 };
static const uint8_t order_1_8_7_3_6_5_4_2[] = { 1, 8, 7, 3, 6, 5, 4, 2 };

// 9 cylinder
static const uint8_t order_1_2_3_4_5_6_7_8_9[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

// 10 cylinder
static const uint8_t order_1_10_9_4_3_6_5_8_7_2[] = {1, 10, 9, 4, 3, 6, 5, 8, 7, 2};

// 12 cyliner
static const uint8_t order_1_7_5_11_3_9_6_12_2_8_4_10[] = {1, 7, 5, 11, 3, 9, 6, 12, 2, 8, 4, 10};
static const uint8_t order_1_7_4_10_2_8_6_12_3_9_5_11[] = {1, 7, 4, 10, 2, 8, 6, 12, 3, 9, 5, 11};
static const uint8_t order_1_12_5_8_3_10_6_7_2_11_4_9[] = {1, 12, 5, 8, 3, 10, 6, 7, 2, 11, 4, 9};
static const uint8_t order_1_2_3_4_5_6_7_8_9_10_11_12[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

// no comments
static const uint8_t order_1_14_9_4_7_12_15_6_13_8_3_16_11_2_5_10[] = {1, 14, 9, 4, 7, 12, 15, 6, 13, 8, 3, 16, 11, 2, 5, 10};

static size_t getFiringOrderLength() {

	switch (engineConfiguration->firingOrder) {
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
	case FO_1_6_2_4_3_5:
	case FO_1_6_5_4_3_2:
	case FO_1_4_5_2_3_6:
		return 6;

// 8 cylinder
	case FO_1_8_4_3_6_5_7_2:
	case FO_1_8_7_2_6_5_4_3:
	case FO_1_5_4_2_6_3_7_8:
	case FO_1_2_7_8_4_5_6_3:
	case FO_1_3_7_2_6_5_4_8:
	case FO_1_2_3_4_5_6_7_8:
	case FO_1_5_4_8_6_3_7_2:
	case FO_1_8_7_3_6_5_4_2:
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
		firmwareError(CUSTOM_OBD_UNKNOWN_FIRING_ORDER, "Invalid firing order: %d", engineConfiguration->firingOrder);
	}
	return 1;
}

static const uint8_t* getFiringOrderTable()
{
	switch (engineConfiguration->firingOrder) {
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
	case FO_1_6_2_4_3_5:
		return order_1_6_2_4_3_5;
	case FO_1_6_5_4_3_2:
		return order_1_6_5_4_3_2;
	case FO_1_4_5_2_3_6:
		return order_1_4_5_2_3_6;

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
	case FO_1_8_7_3_6_5_4_2:
		return order_1_8_7_3_6_5_4_2;


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
		firmwareError(CUSTOM_OBD_UNKNOWN_FIRING_ORDER, "Invalid firing order: %d", engineConfiguration->firingOrder);
	}

	return NULL;
}

/**
 * @param index from zero to cylindersCount - 1
 * @return cylinderId from one to cylindersCount
 */
size_t getCylinderId(size_t index) {
	const size_t firingOrderLength = getFiringOrderLength();

	if (firingOrderLength < 1 || firingOrderLength > MAX_CYLINDER_COUNT) {
		firmwareError(CUSTOM_FIRING_LENGTH, "fol %d", firingOrderLength);
		return 1;
	}
	if (engineConfiguration->cylindersCount != firingOrderLength) {
		// May 2020 this somehow still happens with functional tests, maybe race condition?
		firmwareError(CUSTOM_OBD_WRONG_FIRING_ORDER, "Wrong cyl count for firing order, expected %d cylinders", firingOrderLength);
		return 1;
	}

	if (index >= firingOrderLength) {
		// May 2020 this somehow still happens with functional tests, maybe race condition?
		warning(CUSTOM_ERR_6686, "firing order index %d", index);
		return 1;
	}

	if (auto firingOrderTable = getFiringOrderTable()) {
		return firingOrderTable[index];
	} else {
		// error already reported
		return 1;
	}
}

/**
 * @param prevCylinderId from one to cylindersCount
 * @return cylinderId from one to cylindersCount
 */
size_t getNextFiringCylinderId(size_t prevCylinderId) {
	const size_t firingOrderLength = getFiringOrderLength();
	auto firingOrderTable = getFiringOrderTable();

	if (firingOrderTable) {
		for (size_t i = 0; i < firingOrderLength; i++) {
			if (firingOrderTable[i] == prevCylinderId) {
				return firingOrderTable[(i + 1) % firingOrderLength];
			}
		}
	}

	return 1;
}

/**
 * @return IM_WASTED_SPARK if in SPINNING mode and IM_INDIVIDUAL_COILS setting
 * @return engineConfiguration->ignitionMode otherwise
 */
ignition_mode_e getCurrentIgnitionMode() {
	ignition_mode_e ignitionMode = engineConfiguration->ignitionMode;
#if EFI_SHAFT_POSITION_INPUT
	// In spin-up cranking mode we don't have full phase sync info yet, so wasted spark mode is better
	// However, only do this on even cylinder count engines: odd cyl count doesn't fire at all
	if (ignitionMode == IM_INDIVIDUAL_COILS && (engineConfiguration->cylindersCount % 2 == 0)) {
		bool missingPhaseInfoForSequential = 
			!engine->triggerCentral.triggerState.hasSynchronizedPhase();

		if (engine->rpmCalculator.isSpinningUp() || missingPhaseInfoForSequential) {
			ignitionMode = IM_WASTED_SPARK;
		}
	}
#endif /* EFI_SHAFT_POSITION_INPUT */
	return ignitionMode;
}

#if EFI_ENGINE_CONTROL

/**
 * This heavy method is only invoked in case of a configuration change or initialization.
 */
void prepareOutputSignals() {
	getEngineState()->engineCycle = getEngineCycle(getEngineRotationState()->getOperationMode());

#if EFI_UNIT_TEST
	if (verboseMode) {
		printf("prepareOutputSignals %d %s\r\n", engineConfiguration->trigger.type, getIgnition_mode_e(engineConfiguration->ignitionMode));
	}
#endif /* EFI_UNIT_TEST */

#if EFI_SHAFT_POSITION_INPUT
	engine->triggerCentral.prepareTriggerShape();
#endif // EFI_SHAFT_POSITION_INPUT

	// Fuel schedule may now be completely wrong, force a reset
	engine->injectionEvents.invalidate();
}

angle_t getCylinderAngle(uint8_t cylinderIndex, uint8_t cylinderNumber) {
	// base = position of this cylinder in the firing order.
	// We get a cylinder every n-th of an engine cycle where N is the number of cylinders
	auto base = engine->engineState.engineCycle * cylinderIndex / engineConfiguration->cylindersCount;

	// Plus or minus any adjustment if this is an odd-fire engine
	auto adjustment = engineConfiguration->timing_offset_cylinder[cylinderNumber];

	auto result = base + adjustment;

	assertAngleRange(result, "getCylinderAngle", CUSTOM_ERR_6566);

	return result;
}

void setTimingRpmBin(float from, float to) {
	setRpmBin(config->ignitionRpmBins, IGN_RPM_COUNT, from, to);
}

void setTimingLoadBin(float from, float to) {
	setLinearCurve(config->ignitionLoadBins, from, to);
}

/**
 * this method sets algorithm and ignition table scale
 */
void setAlgorithm(engine_load_mode_e algo) {
	engineConfiguration->fuelAlgorithm = algo;
}

void setFlatInjectorLag(float value) {
	setArrayValues(engineConfiguration->injector.battLagCorr, value);
}

BlendResult calculateBlend(blend_table_s& cfg, float rpm, float load) {
	// If set to 0, skip the math as its disabled
	if (cfg.blendParameter == GPPWM_Zero) {
		return { 0, 0, 0 };
	}

	auto value = readGppwmChannel(cfg.blendParameter);

	if (!value) {
		return { 0, 0, 0 };
	}

	float tableValue = interpolate3d(
		cfg.table,
		cfg.loadBins, load,
		cfg.rpmBins, rpm
	);

	float blendFactor = interpolate2d(value.Value, cfg.blendBins, cfg.blendValues);

	return { value.Value, blendFactor, 0.01f * blendFactor * tableValue };
}

#endif /* EFI_ENGINE_CONTROL */
