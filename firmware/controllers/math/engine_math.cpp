/**
 * @file	engine_math.cpp
 * @brief
 *
 * @date Jul 13, 2013
 * @author Andrey Belomutskiy, (c) 2012-2016
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
#include "io_pins.h"
#include "trigger_decoder.h"
#include "event_registry.h"
#include "efiGpio.h"
#include "fuel_math.h"
#include "advance_map.h"

EXTERN_ENGINE
;

extern engine_pins_s enginePins;

/**
 * @return number of milliseconds in one crank shaft revolution
 */
floatms_t getCrankshaftRevolutionTimeMs(int rpm) {
	return 360 * getOneDegreeTimeMs(rpm);
}

/**
 * @brief Returns engine load according to selected engine_load_mode
 *
 */
float getEngineLoadT(DECLARE_ENGINE_PARAMETER_F) {
	efiAssert(engine!=NULL, "engine 2NULL", NAN);
	efiAssert(engineConfiguration!=NULL, "engineConfiguration 2NULL", NAN);
	switch (engineConfiguration->algorithm) {
	case LM_PLAIN_MAF:
		if (!hasMafSensor(PASS_ENGINE_PARAMETER_F)) {
			warning(OBD_PCM_Processor_Fault, "MAF sensor needed for current fuel algorithm");
			return NAN;
		}
		return getMafT(engineConfiguration);
	case LM_SPEED_DENSITY:
		// SD engine load is used for timing lookup but not for fuel calculation
	case LM_MAP:
		return getMap();
	case LM_ALPHA_N:
		return getTPS(PASS_ENGINE_PARAMETER_F);
	case LM_REAL_MAF: {
		return getRealMaf(PASS_ENGINE_PARAMETER_F);
	}
	default:
		warning(OBD_PCM_Processor_Fault, "Unexpected engine load parameter: %d", engineConfiguration->algorithm);
		return -1;
	}
}

void setSingleCoilDwell(engine_configuration_s *engineConfiguration) {
	for (int i = 0; i < DWELL_CURVE_SIZE; i++) {
		engineConfiguration->sparkDwellBins[i] = -10 + i;
		engineConfiguration->sparkDwell[i] = -1;
	}

	engineConfiguration->sparkDwellBins[5] = 1;
	engineConfiguration->sparkDwell[5] = 4;

	engineConfiguration->sparkDwellBins[6] = 4500;
	engineConfiguration->sparkDwell[6] = 4;

	engineConfiguration->sparkDwellBins[7] = 12500;
	engineConfiguration->sparkDwell[7] = 0;
}

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)


static void addIgnitionEvent(angle_t localAdvance, angle_t dwellAngle, IgnitionEventList *list, NamedOutputPin *output DECLARE_ENGINE_PARAMETER_S) {
	IgnitionEvent *event = list->add();

	if (!isPinAssigned(output)) {
		// todo: extact method for this index math
		warning(OBD_PCM_Processor_Fault, "no_pin_cl #%s", output->name);
	}
	event->output = output;
	event->advance = localAdvance;

	findTriggerPosition(&event->dwellPosition, localAdvance - dwellAngle PASS_ENGINE_PARAMETER);
}

void initializeIgnitionActions(angle_t advance, angle_t dwellAngle,
		IgnitionEventList *list DECLARE_ENGINE_PARAMETER_S) {
	efiAssertVoid(engineConfiguration->specs.cylindersCount > 0, "cylindersCount");

	list->reset();

	for (int i = 0; i < CONFIG(specs.cylindersCount); i++) {
		// todo: clean up this implementation? does not look too nice as is.

		// change of sign here from 'before TDC' to 'after TDC'
		angle_t localAdvance = -advance + ENGINE(angleExtra[i]);
		int index = ENGINE(ignitionPin[i]);
		int cylinderIndex = ID2INDEX(getCylinderId(CONFIG(specs.firingOrder), index));
		NamedOutputPin *output = &enginePins.coils[cylinderIndex];

		addIgnitionEvent(localAdvance, dwellAngle, list, output PASS_ENGINE_PARAMETER);

		if (CONFIG(ignitionMode) == IM_WASTED_SPARK && CONFIG(twoWireBatchIgnition)) {
			index += CONFIG(specs.cylindersCount) / 2;
			cylinderIndex = ID2INDEX(getCylinderId(CONFIG(specs.firingOrder), index));
			output = &enginePins.coils[cylinderIndex];

			addIgnitionEvent(localAdvance, dwellAngle, list, output PASS_ENGINE_PARAMETER);
		}

	}
}

void FuelSchedule::registerInjectionEvent(int injectorIndex, float angle,
		bool isSimultanious DECLARE_ENGINE_PARAMETER_S) {

	NamedOutputPin *output = &enginePins.injectors[injectorIndex];

	if (!isSimultanious && !isPinAssigned(output)) {
		// todo: extact method for this index math
		warning(OBD_PCM_Processor_Fault, "no_pin_inj #%s", output->name);
	}

	InjectionEvent *ev = injectionEvents.add();
	if (ev == NULL) {
		// error already reported
		return;
	}

	ev->injectorIndex = injectorIndex;
	ev->actuator.output = output;

	ev->isSimultanious = isSimultanious;

	efiAssertVoid(TRIGGER_SHAPE(getSize()) > 0, "uninitialized TriggerShape");

	findTriggerPosition(&ev->injectionStart, angle PASS_ENGINE_PARAMETER);
	if (!hasEvents[ev->injectionStart.eventIndex]) {
		hasEvents[ev->injectionStart.eventIndex] = true;
		eventsCount++;
	}
}

FuelSchedule::FuelSchedule() {
	clear();
}

void FuelSchedule::clear() {
	memset(hasEvents, 0, sizeof(hasEvents));
	eventsCount = 0;
}

void FuelSchedule::addFuelEvents(injection_mode_e mode DECLARE_ENGINE_PARAMETER_S) {
	clear(); // this method is relatively heavy
//	sourceList->reset();

	injectionEvents.reset();

	efiAssertVoid(engine!=NULL, "engine is NULL");

	if (cisnan(engine->rpmCalculator.oneDegreeUs))
		return;

	efiAssertVoid(!cisnan(engine->rpmCalculator.oneDegreeUs), "NAN one deg");

	/**
	 * injection phase is scheduled by injection end, so we need to step the angle back
	 * for the duration of the injection
	 */
	angle_t baseAngle = ENGINE(engineState.injectionOffset)
			+ CONFIG(injectionOffset) - MS2US(ENGINE(fuelMs)) / ENGINE(rpmCalculator.oneDegreeUs);

	switch (mode) {
	case IM_SEQUENTIAL:
		for (int i = 0; i < CONFIG(specs.cylindersCount); i++) {
			int index = getCylinderId(engineConfiguration->specs.firingOrder, i) - 1;
			float angle = baseAngle
					+ ENGINE(engineCycle) * i / CONFIG(specs.cylindersCount);
			registerInjectionEvent(index, angle, false PASS_ENGINE_PARAMETER);
		}
		break;
	case IM_SIMULTANEOUS:
		for (int i = 0; i < CONFIG(specs.cylindersCount); i++) {
			float angle = baseAngle
					+ ENGINE(engineCycle) * i / CONFIG(specs.cylindersCount);

			/**
			 * We do not need injector pin here because we will control all injectors
			 * simultaneously
			 */
			registerInjectionEvent(0, angle, true PASS_ENGINE_PARAMETER);
		}
		break;
	case IM_BATCH:
		for (int i = 0; i < CONFIG(specs.cylindersCount); i++) {
			int index = i % (engineConfiguration->specs.cylindersCount / 2);
			float angle = baseAngle
					+ i * ENGINE(engineCycle) / CONFIG(specs.cylindersCount);
			registerInjectionEvent(index, angle, false PASS_ENGINE_PARAMETER);

			if (CONFIG(twoWireBatchInjection)) {

				/**
				 * also fire the 2nd half of the injectors so that we can implement a batch mode on individual wires
				 */
				index = index + (CONFIG(specs.cylindersCount) / 2);
				registerInjectionEvent(index, angle, false PASS_ENGINE_PARAMETER);
			}
		}
		break;
	default:
		warning(OBD_PCM_Processor_Fault, "Unexpected injection mode %d", mode);
	}
}

#endif

/**
 * @return Spark dwell time, in milliseconds.
 */
floatms_t getSparkDwell(int rpm DECLARE_ENGINE_PARAMETER_S) {
	if (isCrankingR(rpm)) {
		if (engineConfiguration->useConstantDwellDuringCranking) {
			return engineConfiguration->ignitionDwellForCrankingMs;
		} else {
			// technically this could be implemented via interpolate2d
			float angle = engineConfiguration->crankingChargeAngle;
			return getOneDegreeTimeMs(rpm) * angle;
		}
	}
	efiAssert(!cisnan(rpm), "invalid rpm", NAN);

	return interpolate2d(rpm, engineConfiguration->sparkDwellBins, engineConfiguration->sparkDwell, DWELL_CURVE_SIZE);
}

static int findAngleIndex(float target DECLARE_ENGINE_PARAMETER_S) {
	/**
	 * Here we rely on this to be pre-calculated, that's a performance optimization
	 */
	int engineCycleEventCount = engine->engineCycleEventCount;

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

void findTriggerPosition(event_trigger_position_s *position, angle_t angleOffset DECLARE_ENGINE_PARAMETER_S) {
	// convert engine cycle angle into trigger cycle angle
	angleOffset += tdcPosition();
	fixAngle(angleOffset);

	int index = TRIGGER_SHAPE(triggerIndexByAngle[(int)angleOffset]);
	angle_t eventAngle = TRIGGER_SHAPE(eventAngles[index]);
	if (angleOffset < eventAngle) {
		warning(OBD_PCM_Processor_Fault, "angle constraint violation in findTriggerPosition(): %f/%f", angleOffset, eventAngle);
		return;
	}

	position->eventIndex = index;
	position->eventAngle = eventAngle;
	position->angleOffset = angleOffset - eventAngle;
}

static int order_1_THEN_3_THEN_4_THEN2[] = { 1, 3, 4, 2 };
static int order_1_THEN_2_THEN_4_THEN3[] = { 1, 2, 4, 3 };
static int order_1_THEN_3_THEN_2_THEN4[] = { 1, 3, 2, 4 };

static int order_1_2_4_5_3[] = {1, 2, 4, 5, 3};

static int order_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4[] = { 1, 5, 3, 6, 2, 4 };
static int order_1_THEN_4_THEN_2_THEN_5_THEN_3_THEN_6[] = { 1, 4, 2, 5, 3, 6 };
static int order_1_THEN_2_THEN_3_THEN_4_THEN_5_THEN_6[] = { 1, 2, 3, 4, 5, 6 };

static int order_1_8_4_3_6_5_7_2[] = { 1, 8, 4, 3, 6, 5, 7, 2 };

static int order_1_2[] = {1, 2};

static int order_1_2_3[] = {1, 2, 3};

/**
 * @param index from zero to cylindersCount - 1
 * @return cylinderId from one to cylindersCount
 */
int getCylinderId(firing_order_e firingOrder, int index) {

	switch (firingOrder) {
	case FO_ONE_CYLINDER:
		return 1;
// 2 cylinder
	case FO_1_THEN_2:
		return order_1_2[index];
// 3 cylinder
	case FO_1_2_3:
		return order_1_2_3[index];
// 4 cylinder
	case FO_1_THEN_3_THEN_4_THEN2:
		return order_1_THEN_3_THEN_4_THEN2[index];
	case FO_1_THEN_2_THEN_4_THEN3:
		return order_1_THEN_2_THEN_4_THEN3[index];
	case FO_1_THEN_3_THEN_2_THEN4:
		return order_1_THEN_3_THEN_2_THEN4[index];
// 5 cylinder
	case FO_1_2_4_5_3:
		return order_1_2_4_5_3[index];

// 6 cylinder
	case FO_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4:
		return order_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4[index];
	case FO_1_THEN_4_THEN_2_THEN_5_THEN_3_THEN_6:
		return order_1_THEN_4_THEN_2_THEN_5_THEN_3_THEN_6[index];
	case FO_1_THEN_2_THEN_3_THEN_4_THEN_5_THEN_6:
		return order_1_THEN_2_THEN_3_THEN_4_THEN_5_THEN_6[index];

// 8 cylinder
	case FO_1_8_4_3_6_5_7_2:
		return order_1_8_4_3_6_5_7_2[index];

	default:
		warning(OBD_PCM_Processor_Fault, "getCylinderId not supported for %d", firingOrder);
	}
	return 1;
}

static int getIgnitionPinForIndex(int i DECLARE_ENGINE_PARAMETER_S
) {
	switch (CONFIG(ignitionMode)) {
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
		warning(OBD_PCM_Processor_Fault, "unsupported ignitionMode %d in initializeIgnitionActions()", engineConfiguration->ignitionMode);
		return 0;
	}
}

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)

/**
 * This heavy method is only invoked in case of a configuration change or initialization.
 */
void prepareOutputSignals(DECLARE_ENGINE_PARAMETER_F) {
	ENGINE(engineCycle) = getEngineCycle(CONFIG(operationMode));

	engine_configuration2_s *engineConfiguration2 = engine->engineConfiguration2;

	for (int i = 0; i < CONFIG(specs.cylindersCount); i++) {
		ENGINE(angleExtra[i])= ENGINE(engineCycle) * i / CONFIG(specs.cylindersCount);



		ENGINE(ignitionPin[i]) = getIgnitionPinForIndex(i PASS_ENGINE_PARAMETER);

	}

	int engineCycleInt = (int) ENGINE(engineCycle);
	for (int angle = 0; angle < engineCycleInt; angle++) {
		int triggerShapeIndex = findAngleIndex(angle PASS_ENGINE_PARAMETER);
		if (engineConfiguration->useOnlyFrontForTrigger)
			triggerShapeIndex = triggerShapeIndex & 0xFFFFFFFE; // we need even index for front_only
		TRIGGER_SHAPE(triggerIndexByAngle[angle]) = triggerShapeIndex;
	}

	engineConfiguration2->crankingInjectionEvents.addFuelEvents(
			engineConfiguration->crankingInjectionMode PASS_ENGINE_PARAMETER);
	engineConfiguration2->injectionEvents.addFuelEvents(
			engineConfiguration->injectionMode PASS_ENGINE_PARAMETER);
}

#endif

void setFuelRpmBin(float from, float to DECLARE_ENGINE_PARAMETER_S) {
	setTableBin(config->fuelRpmBins, FUEL_RPM_COUNT, from, to);
}

void setFuelLoadBin(float from, float to DECLARE_ENGINE_PARAMETER_S) {
	setTableBin(config->fuelLoadBins, FUEL_LOAD_COUNT, from, to);
}

void setTimingRpmBin(float from, float to DECLARE_ENGINE_PARAMETER_S) {
	setRpmBin(config->ignitionRpmBins, IGN_RPM_COUNT, from, to);
}

void setTimingLoadBin(float from, float to DECLARE_ENGINE_PARAMETER_S) {
	setTableBin(config->ignitionLoadBins, IGN_LOAD_COUNT, from, to);
}

int isInjectionEnabled(engine_configuration_s *engineConfiguration) {
	// todo: is this worth a method? should this be inlined?
	return engineConfiguration->isInjectionEnabled;
}

/**
 * this method sets algorithm and ignition table scale
 */
void setAlgorithm(engine_load_mode_e algo DECLARE_ENGINE_PARAMETER_S) {
	engineConfiguration->algorithm = algo;
	if (algo == LM_ALPHA_N) {
		setTimingLoadBin(20, 120 PASS_ENGINE_PARAMETER);
	} else if (algo == LM_SPEED_DENSITY) {
		setTableBin2(config->ignitionLoadBins, IGN_LOAD_COUNT, 20, 120, 3);
		buildTimingMap(35 PASS_ENGINE_PARAMETER);
	}
}
