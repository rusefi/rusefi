/**
 * @file	engine_math.cpp
 * @brief
 *
 * @date Jul 13, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
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

#include "main.h"
#include "engine_math.h"
#include "engine_configuration.h"
#include "interpolation.h"
#include "allsensors.h"
#include "trigger_decoder.h"
#include "event_registry.h"
#include "efiGpio.h"
#include "fuel_math.h"
#include "advance_map.h"

EXTERN_ENGINE
;

extern EnginePins enginePins;

// Store current ignition mode for prepareIgnitionPinIndices()
static ignition_mode_e ignitionModeForPinIndices;

floatms_t getEngineCycleDuration(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX) {
	return getCrankshaftRevolutionTimeMs(rpm) * (engineConfiguration->operationMode == TWO_STROKE ? 1 : 2);
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

/**
 * @brief Returns engine load according to selected engine_load_mode
 *
 */
float getEngineLoadT(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	efiAssert(engine!=NULL, "engine 2NULL", NAN);
	efiAssert(engineConfiguration!=NULL, "engineConfiguration 2NULL", NAN);
	switch (engineConfiguration->fuelAlgorithm) {
	case LM_PLAIN_MAF:
		if (!hasMafSensor(PASS_ENGINE_PARAMETER_SIGNATURE)) {
			warning(CUSTOM_MAF_NEEDED, "MAF sensor needed for current fuel algorithm");
			return NAN;
		}
		return getMafT(engineConfiguration);
	case LM_SPEED_DENSITY:
		// SD engine load is used for timing lookup but not for fuel calculation
	case LM_MAP:
		return getMap();
	case LM_ALPHA_N:
		return getTPS(PASS_ENGINE_PARAMETER_SIGNATURE);
	case LM_REAL_MAF: {
		return getRealMaf(PASS_ENGINE_PARAMETER_SIGNATURE);
	}
	default:
		warning(CUSTOM_UNKNOWN_ALGORITHM, "Unexpected engine load parameter: %d", engineConfiguration->fuelAlgorithm);
		return -1;
	}
}

void setSingleCoilDwell(engine_configuration_s *engineConfiguration) {
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

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)

FuelSchedule::FuelSchedule() {
	clear();
}

void FuelSchedule::clear() {
	isReady = false;
}

/**
 * @returns false in case of error, true if success
 */
bool FuelSchedule::addFuelEventsForCylinder(int i  DECLARE_ENGINE_PARAMETER_SUFFIX) {
	efiAssert(engine!=NULL, "engine is NULL", false);

	floatus_t oneDegreeUs = ENGINE(rpmCalculator.oneDegreeUs); // local copy
	if (cisnan(oneDegreeUs)) {
		// in order to have fuel schedule we need to have current RPM
		// wonder if this line slows engine startup?
		return false;
	}

	/**
	 * injection phase is scheduled by injection end, so we need to step the angle back
	 * for the duration of the injection
	 *
	 * todo: since this method is not invoked within trigger event handler and
	 * engineState.injectionOffset is calculated from the same utility timer should we more that logic here?
	 */
	floatms_t fuelMs = ENGINE(injectionDuration);
	efiAssert(!cisnan(fuelMs), "NaN fuelMs", false);
	angle_t injectionDuration = MS2US(fuelMs) / oneDegreeUs;
	efiAssert(!cisnan(injectionDuration), "NaN injectionDuration", false);
	assertAngleRange(injectionDuration, "injectionDuration_r");
	floatus_t injectionOffset = ENGINE(engineState.injectionOffset);
	if (cisnan(injectionOffset)) {
		// injection offset map not ready - we are not ready to schedule fuel events
		return false;
	}
	angle_t baseAngle = injectionOffset - injectionDuration;
	efiAssert(!cisnan(baseAngle), "NaN baseAngle", false);
	assertAngleRange(baseAngle, "baseAngle_r");

	int injectorIndex;

	injection_mode_e mode = engine->getCurrentInjectionMode(PASS_ENGINE_PARAMETER_SIGNATURE);

	if (mode == IM_SIMULTANEOUS || mode == IM_SINGLE_POINT) {
		injectorIndex = 0;
	} else if (mode == IM_SEQUENTIAL) {
		injectorIndex = getCylinderId(i PASS_ENGINE_PARAMETER_SUFFIX) - 1;
	} else if (mode == IM_BATCH) {
		// does not look exactly right, not too consistent with IM_SEQUENTIAL
		injectorIndex = i % (engineConfiguration->specs.cylindersCount / 2);
	} else {
		warning(CUSTOM_OBD_UNEXPECTED_INJECTION_MODE, "Unexpected injection mode %d", mode);
		injectorIndex = 0;
	}

	bool isSimultanious = mode == IM_SIMULTANEOUS;

	assertAngleRange(baseAngle, "addFbaseAngle");

	int cylindersCount = CONFIG(specs.cylindersCount);
	if (cylindersCount < 1) {
		warning(CUSTOM_OBD_ZERO_CYLINDER_COUNT, "temp cylindersCount %d", cylindersCount);
		return false;
	}

	float angle = baseAngle
			+ i * ENGINE(engineCycle) / cylindersCount;

	InjectorOutputPin *secondOutput;
	if (mode == IM_BATCH && CONFIG(twoWireBatchInjection)) {
		/**
		 * also fire the 2nd half of the injectors so that we can implement a batch mode on individual wires
		 */
		int secondIndex = injectorIndex + (CONFIG(specs.cylindersCount) / 2);
		secondOutput = &enginePins.injectors[secondIndex];
	} else {
		secondOutput = NULL;
	}

	InjectorOutputPin *output = &enginePins.injectors[injectorIndex];

	if (!isSimultanious && !output->isInitialized()) {
		// todo: extract method for this index math
		warning(CUSTOM_OBD_INJECTION_NO_PIN_ASSIGNED, "no_pin_inj #%s", output->name);
	}

	InjectionEvent *ev = &elements[i];
	ev->ownIndex = i;
#if EFI_UNIT_TEST || defined(__DOXYGEN__)
	ev->engine = engine;
#endif
	fixAngle(angle, "addFuel#1");

	ev->outputs[0] = output;
	ev->outputs[1] = secondOutput;

	ev->isSimultanious = isSimultanious;

	if (TRIGGER_SHAPE(getSize()) < 1) {
		warning(CUSTOM_ERR_NOT_INITIALIZED_TRIGGER, "uninitialized TriggerShape");
		return false;
	}

	efiAssert(!cisnan(angle), "findAngle#3", false);
	assertAngleRange(angle, "findAngle#a33");
	TRIGGER_SHAPE(findTriggerPosition(&ev->injectionStart, angle PASS_ENGINE_PARAMETER_SUFFIX));
#if EFI_UNIT_TEST || defined(__DOXYGEN__)
	printf("registerInjectionEvent angle=%.2f trgIndex=%d inj %d\r\n", angle, ev->injectionStart.eventIndex, injectorIndex);
#endif
	return true;
}

void FuelSchedule::addFuelEvents(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	clear();

	for (int i = 0; i < CONFIG(specs.cylindersCount); i++) {
		InjectionEvent *ev = &elements[i];
		ev->ownIndex = i;
		bool result = addFuelEventsForCylinder(i PASS_ENGINE_PARAMETER_SUFFIX);
		if (!result)
			return;
	}
	isReady = true;
}

#endif

static floatms_t getCrankingSparkDwell(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (engineConfiguration->useConstantDwellDuringCranking) {
		return engineConfiguration->ignitionDwellForCrankingMs;
	} else {
		// technically this could be implemented via interpolate2d
		float angle = engineConfiguration->crankingChargeAngle;
		return getOneDegreeTimeMs(GET_RPM()) * angle;
	}
}

/**
 * @return Spark dwell time, in milliseconds. 0 if tables are not ready.
 */
floatms_t getSparkDwell(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX) {
	float dwellMs;
	if (ENGINE(rpmCalculator).isCranking(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		dwellMs = getCrankingSparkDwell(PASS_ENGINE_PARAMETER_SIGNATURE);
	} else {
		efiAssert(!cisnan(rpm), "invalid rpm", NAN);

		dwellMs = interpolate2d("dwell", rpm, engineConfiguration->sparkDwellRpmBins, engineConfiguration->sparkDwellValues, DWELL_CURVE_SIZE);
	}

	if (cisnan(dwellMs) || dwellMs <= 0) {
		// this could happen during engine configuration reset
		warning(CUSTOM_ERR_DWELL_DURATION, "invalid dwell: %.2f at rpm=%d", dwellMs, rpm);
		return 0;
	}
	return dwellMs;
}

/**
 * this method is only used on initialization
 */
int TriggerShape::findAngleIndex(float target DECLARE_ENGINE_PARAMETER_SUFFIX) {
	int engineCycleEventCount = TRIGGER_SHAPE(getLength());

	efiAssert(engineCycleEventCount > 0, "engineCycleEventCount", 0);

	uint32_t left = 0;
	uint32_t right = engineCycleEventCount - 1;

	/**
	 * Let's find the last trigger angle which is less or equal to the desired angle
	 * todo: extract binary search as template method?
	 */
    while (left <= right) {
        int middle = (left + right) / 2;
		angle_t eventAngle = TRIGGER_SHAPE(eventAngles[middle]);

        if (eventAngle < target) {
            left = middle + 1;
        } else if (eventAngle > target) {
            right = middle - 1;
        } else {
            // Values are equal
            return middle;             // Key found
        }
    }
    return left - 1;
}

void TriggerShape::findTriggerPosition(event_trigger_position_s *position, angle_t angleOffset DECLARE_ENGINE_PARAMETER_SUFFIX) {
	efiAssertVoid(!cisnan(angleOffset), "findAngle#1");
	assertAngleRange(angleOffset, "findAngle#a1");

	efiAssertVoid(!cisnan(ENGINE(triggerCentral.triggerShape.tdcPosition)), "tdcPos#1")
	assertAngleRange(ENGINE(triggerCentral.triggerShape.tdcPosition), "tdcPos#a1");

	efiAssertVoid(!cisnan(CONFIG(globalTriggerAngleOffset)), "tdcPos#2")
	assertAngleRange(CONFIG(globalTriggerAngleOffset), "tdcPos#a2");

	// convert engine cycle angle into trigger cycle angle
	angleOffset += tdcPosition();
	efiAssertVoid(!cisnan(angleOffset), "findAngle#2");
	fixAngle(angleOffset, "addFuel#2");

	int index = triggerIndexByAngle[(int)angleOffset];
	angle_t eventAngle = eventAngles[index];
	if (angleOffset < eventAngle) {
		warning(CUSTOM_OBD_ANGLE_CONSTRAINT_VIOLATION, "angle constraint violation in findTriggerPosition(): %.2f/%.2f", angleOffset, eventAngle);
		return;
	}

	position->eventIndex = index;
	position->eventAngle = eventAngle;
	position->angleOffset = angleOffset - eventAngle;
}

static int order_1_2[] = {1, 2};

static int order_1_2_3[] = {1, 2, 3};
// 4 cylinder

static int order_1_THEN_3_THEN_4_THEN2[] = { 1, 3, 4, 2 };
static int order_1_THEN_2_THEN_4_THEN3[] = { 1, 2, 4, 3 };
static int order_1_THEN_3_THEN_2_THEN4[] = { 1, 3, 2, 4 };

// 5 cylinder
static int order_1_2_4_5_3[] = {1, 2, 4, 5, 3};

// 6 cylinder
static int order_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4[] = { 1, 5, 3, 6, 2, 4 };
static int order_1_THEN_4_THEN_2_THEN_5_THEN_3_THEN_6[] = { 1, 4, 2, 5, 3, 6 };
static int order_1_THEN_2_THEN_3_THEN_4_THEN_5_THEN_6[] = { 1, 2, 3, 4, 5, 6 };
static int order_1_6_3_2_5_4[] = {1, 6, 3, 2, 5, 4};

// 8 cylinder
static int order_1_8_4_3_6_5_7_2[] = { 1, 8, 4, 3, 6, 5, 7, 2 };

static int order_1_8_7_2_6_5_4_3[] = { 1, 8, 7, 2, 6, 5, 4, 3 };
static int order_1_5_4_2_6_3_7_8[] = { 1, 5, 4, 2, 6, 3, 7, 8 };

// 10 cylinder
static int order_1_10_9_4_3_6_5_8_7_2[] = {1, 10, 9, 4, 3, 6, 5, 8, 7, 2};

// 12 cyliner
static int order_1_7_5_11_3_9_6_12_2_8_4_10[] = {1, 7, 5, 11, 3, 9, 6, 12, 2, 8, 4, 10};
static int order_1_7_4_10_2_8_6_12_3_9_5_11[] = {1, 7, 4, 10, 2, 8, 6, 12, 3, 9, 5, 11};

static int getFiringOrderLength(DECLARE_ENGINE_PARAMETER_SIGNATURE) {

	switch (CONFIG(specs.firingOrder)) {
	case FO_1:
		return 1;
// 2 cylinder
	case FO_1_2:
		return 2;
// 3 cylinder
	case FO_1_2_3:
		return 3;
// 4 cylinder
	case FO_1_3_4_2:
	case FO_1_2_4_3:
	case FO_1_3_2_4:
		return 4;
// 5 cylinder
	case FO_1_2_4_5_3:
		return 5;

// 6 cylinder
	case FO_1_5_3_6_2_4:
	case FO_1_4_2_5_3_6:
	case FO_1_2_3_4_5_6:
	case FO_1_6_3_2_5_4:
		return 6;

// 8 cylinder
	case FO_1_8_4_3_6_5_7_2:
	case FO_1_8_7_2_6_5_4_3:
	case FO_1_5_4_2_6_3_7_8:
		return 8;

// 10 cylinder
	case FO_1_10_9_4_3_6_5_8_7_2:
		return 10;

// 12 cylinder
	case FO_1_7_5_11_3_9_6_12_2_8_4_10:
	case FO_1_7_4_10_2_8_6_12_3_9_5_11:
		return 12;

	default:
		warning(CUSTOM_OBD_UNKNOWN_FIRING_ORDER, "getCylinderId not supported for %d", CONFIG(specs.firingOrder));
	}
	return 1;
}


/**
 * @param index from zero to cylindersCount - 1
 * @return cylinderId from one to cylindersCount
 */
int getCylinderId(int index DECLARE_ENGINE_PARAMETER_SUFFIX) {

	const int foLength = getFiringOrderLength(PASS_ENGINE_PARAMETER_SIGNATURE);
	if (engineConfiguration->specs.cylindersCount != foLength) {
		warning(CUSTOM_OBD_WRONG_FIRING_ORDER, "Wrong firing order %d/%d", engineConfiguration->specs.cylindersCount, foLength);
		return 1;
	}

	switch (CONFIG(specs.firingOrder)) {
	case FO_1:
		return 1;
// 2 cylinder
	case FO_1_2:
		return order_1_2[index];
// 3 cylinder
	case FO_1_2_3:
		return order_1_2_3[index];
// 4 cylinder
	case FO_1_3_4_2:
		return order_1_THEN_3_THEN_4_THEN2[index];
	case FO_1_2_4_3:
		return order_1_THEN_2_THEN_4_THEN3[index];
	case FO_1_3_2_4:
		return order_1_THEN_3_THEN_2_THEN4[index];
// 5 cylinder
	case FO_1_2_4_5_3:
		return order_1_2_4_5_3[index];

// 6 cylinder
	case FO_1_5_3_6_2_4:
		return order_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4[index];
	case FO_1_4_2_5_3_6:
		return order_1_THEN_4_THEN_2_THEN_5_THEN_3_THEN_6[index];
	case FO_1_2_3_4_5_6:
		return order_1_THEN_2_THEN_3_THEN_4_THEN_5_THEN_6[index];
	case FO_1_6_3_2_5_4:
		return order_1_6_3_2_5_4[index];

// 8 cylinder
	case FO_1_8_4_3_6_5_7_2:
		return order_1_8_4_3_6_5_7_2[index];
	case FO_1_8_7_2_6_5_4_3:
		return order_1_8_7_2_6_5_4_3[index];
	case FO_1_5_4_2_6_3_7_8:
		return order_1_5_4_2_6_3_7_8[index];

// 10 cylinder
	case FO_1_10_9_4_3_6_5_8_7_2:
		return order_1_10_9_4_3_6_5_8_7_2[index];

// 12 cylinder
	case FO_1_7_5_11_3_9_6_12_2_8_4_10:
		return order_1_7_5_11_3_9_6_12_2_8_4_10[index];
	case FO_1_7_4_10_2_8_6_12_3_9_5_11:
		return order_1_7_4_10_2_8_6_12_3_9_5_11[index];

	default:
		warning(CUSTOM_OBD_UNKNOWN_FIRING_ORDER, "getCylinderId not supported for %d", CONFIG(specs.firingOrder));
	}
	return 1;
}

static int getIgnitionPinForIndex(int i DECLARE_ENGINE_PARAMETER_SUFFIX) {
	switch (getIgnitionMode(PASS_ENGINE_PARAMETER_SIGNATURE)) {
	case IM_ONE_COIL:
		return 0;
		break;
	case IM_WASTED_SPARK: {
		return i % (CONFIG(specs.cylindersCount) / 2);
	}
		break;
	case IM_INDIVIDUAL_COILS:
		return i;
		break;

	default:
		warning(CUSTOM_OBD_IGNITION_MODE, "unsupported ignitionMode %d in initializeIgnitionActions()", engineConfiguration->ignitionMode);
		return 0;
	}
}

void prepareIgnitionPinIndices(ignition_mode_e ignitionMode DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (ignitionMode != ignitionModeForPinIndices) {
#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
		for (int i = 0; i < CONFIG(specs.cylindersCount); i++) {
			ENGINE(ignitionPin[i]) = getIgnitionPinForIndex(i PASS_ENGINE_PARAMETER_SUFFIX);
		}
#endif /* EFI_ENGINE_CONTROL */
		ignitionModeForPinIndices = ignitionMode;
	}
}

ignition_mode_e getIgnitionMode(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	ignition_mode_e ignitionMode = CONFIG(ignitionMode);
	// In spin-up cranking mode we don't have full phase sync. info yet, so wasted spark mode is better
	if (ignitionMode == IM_INDIVIDUAL_COILS && ENGINE(rpmCalculator.isSpinningUp(PASS_ENGINE_PARAMETER_SIGNATURE)))
		ignitionMode = IM_WASTED_SPARK;
	return ignitionMode;
}

void TriggerShape::prepareShape(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	int engineCycleInt = (int) getEngineCycle(CONFIG(operationMode));
	for (int angle = 0; angle < engineCycleInt; angle++) {
		int triggerShapeIndex = findAngleIndex(angle PASS_ENGINE_PARAMETER_SUFFIX);
		if (engineConfiguration->useOnlyRisingEdgeForTrigger) {
			// we need even index for front_only mode - so if odd indexes are rounded down
			triggerShapeIndex = triggerShapeIndex & 0xFFFFFFFE;
		}
		triggerIndexByAngle[angle] = triggerShapeIndex;
	}
}

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)

/**
 * This heavy method is only invoked in case of a configuration change or initialization.
 */
void prepareOutputSignals(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	ENGINE(engineCycle) = getEngineCycle(CONFIG(operationMode));

	angle_t maxTimingCorrMap = -720.0f;
	angle_t maxTimingMap = -720.0f;
	for (int rpmIndex = 0;rpmIndex<IGN_RPM_COUNT;rpmIndex++) {
		for (int l = 0;l<IGN_LOAD_COUNT;l++) {
			maxTimingCorrMap = maxF(maxTimingCorrMap, config->ignitionIatCorrTable[l][rpmIndex]);
			maxTimingMap = maxF(maxTimingMap, config->ignitionTable[l][rpmIndex]);
		}
	}

#if EFI_UNIT_TEST
	printf("prepareOutputSignals %d onlyEdge=%s %s\r\n", engineConfiguration->trigger.type, boolToString(engineConfiguration->useOnlyRisingEdgeForTrigger),
			getIgnition_mode_e(engineConfiguration->ignitionMode));
#endif /* EFI_UNIT_TEST */

	for (int i = 0; i < CONFIG(specs.cylindersCount); i++) {
		ENGINE(angleExtra[i])= ENGINE(engineCycle) * i / CONFIG(specs.cylindersCount);
	}

	prepareIgnitionPinIndices(CONFIG(ignitionMode) PASS_ENGINE_PARAMETER_SUFFIX);

	TRIGGER_SHAPE(prepareShape(PASS_ENGINE_PARAMETER_SIGNATURE));
}

#endif

void setFuelRpmBin(float from, float to DECLARE_ENGINE_PARAMETER_SUFFIX) {
	setTableBin(config->fuelRpmBins, FUEL_RPM_COUNT, from, to);
}

void setFuelLoadBin(float from, float to DECLARE_ENGINE_PARAMETER_SUFFIX) {
	setTableBin(config->fuelLoadBins, FUEL_LOAD_COUNT, from, to);
}

void setTimingRpmBin(float from, float to DECLARE_ENGINE_PARAMETER_SUFFIX) {
	setRpmBin(config->ignitionRpmBins, IGN_RPM_COUNT, from, to);
}

void setTimingLoadBin(float from, float to DECLARE_ENGINE_PARAMETER_SUFFIX) {
	setTableBin(config->ignitionLoadBins, IGN_LOAD_COUNT, from, to);
}

/**
 * this method sets algorithm and ignition table scale
 */
void setAlgorithm(engine_load_mode_e algo DECLARE_ENGINE_PARAMETER_SUFFIX) {
	engineConfiguration->fuelAlgorithm = algo;
	if (algo == LM_ALPHA_N) {
		setTimingLoadBin(20, 120 PASS_ENGINE_PARAMETER_SUFFIX);
	} else if (algo == LM_SPEED_DENSITY) {
		setLinearCurve(config->ignitionLoadBins, IGN_LOAD_COUNT, 20, 120, 3);
		buildTimingMap(35 PASS_ENGINE_PARAMETER_SUFFIX);
	}
}

void setFlatInjectorLag(float value DECLARE_ENGINE_PARAMETER_SUFFIX) {
	setArrayValues(engineConfiguration->injector.battLagCorr, VBAT_INJECTOR_CURVE_SIZE, value);
}
