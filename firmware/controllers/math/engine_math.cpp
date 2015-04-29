/**
 * @file	engine_math.cpp
 * @brief
 *
 * @date Jul 13, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
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

EXTERN_ENGINE
;

extern engine_pins_s enginePins;

/**
 * @return number of milliseconds in one crankshaft revolution
 */
float getCrankshaftRevolutionTimeMs(int rpm) {
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
OutputSignalList runningInjectonSignals CCM_OPTIONAL;
OutputSignalList crankingInjectonSignals CCM_OPTIONAL;

void initializeIgnitionActions(angle_t advance, angle_t dwellAngle,
		IgnitionEventList *list DECLARE_ENGINE_PARAMETER_S) {
	efiAssertVoid(engineConfiguration->specs.cylindersCount > 0, "cylindersCount");

	list->reset();

	for (int i = 0; i < CONFIG(specs.cylindersCount); i++) {
		angle_t localAdvance = advance + ENGINE(angleExtra[i]);
		NamedOutputPin *output = ENGINE(ignitionPin[i]);

		IgnitionEvent *event = list->add();

		if (!isPinAssigned(output)) {
			// todo: extact method for this index math
			warning(OBD_PCM_Processor_Fault, "no_pin_cl #%s", output->name);
		}
		event->output = output;
		event->advance = localAdvance;

		findTriggerPosition(&event->dwellPosition, localAdvance - dwellAngle PASS_ENGINE_PARAMETER);
	}
}

void FuelSchedule::registerInjectionEvent(OutputSignalList *sourceList, NamedOutputPin *output, float angle,
		bool_t isSimultanious DECLARE_ENGINE_PARAMETER_S) {
	if (!isSimultanious && !isPinAssigned(output)) {
		// todo: extact method for this index math
		warning(OBD_PCM_Processor_Fault, "no_pin_inj #%s", output->name);
	}

	InjectionEvent *ev = events.add();
	if (ev == NULL) {
		// error already reported
		return;
	}

	OutputSignal *actuator = sourceList->add();
	actuator->output = output;

	ev->isSimultanious = isSimultanious;

	efiAssertVoid(TRIGGER_SHAPE(getSize()) > 0, "uninitialized TriggerShape");

	ev->actuator = actuator;

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

void FuelSchedule::addFuelEvents(OutputSignalList *sourceList, injection_mode_e mode DECLARE_ENGINE_PARAMETER_S) {
	clear(); // this method is relatively heavy
	sourceList->reset();

	events.reset();

	efiAssertVoid(engine!=NULL, "engine is NULL");

	if (cisnan(engine->rpmCalculator.oneDegreeUs))
		return;

	efiAssertVoid(!cisnan(engine->rpmCalculator.oneDegreeUs), "NAN one deg");

	/**
	 * injection phase is scheduled by injection end, so we need to step the angle back
	 * for the duration of the injection
	 */
	float baseAngle = ENGINE(engineState.injectionAngle)
			+ engineConfiguration->injectionAngle - MS2US(engine->fuelMs) / engine->rpmCalculator.oneDegreeUs;

	switch (mode) {
	case IM_SEQUENTIAL:
		for (int i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
			int index = getCylinderId(engineConfiguration->specs.firingOrder, i) - 1;
			float angle = baseAngle
					+ (float) engineConfiguration->engineCycle * i / engineConfiguration->specs.cylindersCount;
			registerInjectionEvent(sourceList, &enginePins.injectors[index], angle, false PASS_ENGINE_PARAMETER);
		}
		break;
	case IM_SIMULTANEOUS:
		for (int i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
			float angle = baseAngle
					+ (float) engineConfiguration->engineCycle * i / engineConfiguration->specs.cylindersCount;

			/**
			 * We do not need injector pin here because we will control all injectors
			 * simultaniously
			 */
			registerInjectionEvent(sourceList, NULL, angle, true PASS_ENGINE_PARAMETER);
		}
		break;
	case IM_BATCH:
		for (int i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
			int index = i % (engineConfiguration->specs.cylindersCount / 2);
			float angle = baseAngle
					+ i * (float) engineConfiguration->engineCycle / engineConfiguration->specs.cylindersCount;
			registerInjectionEvent(sourceList, &enginePins.injectors[index], angle, false PASS_ENGINE_PARAMETER);

			if (engineConfiguration->twoWireBatch) {

				/**
				 * also fire the 2nd half of the injectors so that we can implement a batch mode on individual wires
				 */
				index = index + (engineConfiguration->specs.cylindersCount / 2);
				registerInjectionEvent(sourceList, &enginePins.injectors[index], angle, false PASS_ENGINE_PARAMETER);
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
float getSparkDwellMsT(int rpm DECLARE_ENGINE_PARAMETER_S) {
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

static int findAngleIndex(float angleOffset DECLARE_ENGINE_PARAMETER_S) {
	/**
	 * Here we rely on this to be pre-calculated, that's a performance optimization
	 */
	int engineCycleEventCount = engine->engineCycleEventCount;

	efiAssert(engineCycleEventCount > 0, "engineCycleEventCount", 0);

	uint32_t middle;
	uint32_t left = 0;
	uint32_t right = engineCycleEventCount - 1;

	/**
	 * Let's find the last trigger angle which is less or equal to the desired angle
	 * todo: extract binary search as template method?
	 */
	while (true) {
		middle = (left + right) / 2;
		float eventAngle = TRIGGER_SHAPE(eventAngles[middle]);

		if (middle == left) {
			return middle;
		}
		if (angleOffset < eventAngle) {
			right = middle;
		} else if (angleOffset > eventAngle) {
			left = middle;
		} else {
			return middle;
		}
	}
}

void findTriggerPosition(event_trigger_position_s *position, angle_t angleOffset DECLARE_ENGINE_PARAMETER_S) {
	angleOffset += tdcPosition();
	fixAngle(angleOffset);

	int index = TRIGGER_SHAPE(triggerIndexByAngle[(int)angleOffset]);
	angle_t eventAngle = TRIGGER_SHAPE(eventAngles[index]);
	if (angleOffset < eventAngle) {
		warning(OBD_PCM_Processor_Fault, "angle constraint violation in registerActuatorEventExt(): %f/%f", angleOffset, eventAngle);
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

static int order_1_8_4_3_6_5_7_2[] = { 1, 8, 4, 3, 6, 5, 7, 2 };

static int order_1_2[] = {1, 2};

/**
 * @param index from zero to cylindersCount - 1
 * @return cylinderId from one to cylindersCount
 */
int getCylinderId(firing_order_e firingOrder, int index) {

	switch (firingOrder) {
	case FO_ONE_CYLINDER:
		return 1;

	case FO_1_THEN_2:
		return order_1_2[index];
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
// 8 cylinder
	case FO_1_8_4_3_6_5_7_2:
		return order_1_8_4_3_6_5_7_2[index];

	default:
		warning(OBD_PCM_Processor_Fault, "getCylinderId not supported for %d", firingOrder);
	}
	return 1;
}

static NamedOutputPin * getIgnitionPinForIndex(int i DECLARE_ENGINE_PARAMETER_S
) {
	switch (CONFIG(ignitionMode)) {
	case IM_ONE_COIL:
		return &enginePins.coils[0];
		break;
	case IM_WASTED_SPARK: {
		int wastedIndex = i % (CONFIG(specs.cylindersCount) / 2);
		int id = getCylinderId(CONFIG(specs.firingOrder), wastedIndex);
		return &enginePins.coils[ID2INDEX(id)];
	}
		break;
	case IM_INDIVIDUAL_COILS:
		return &enginePins.coils[ID2INDEX(getCylinderId(CONFIG(specs.firingOrder), i))];
		break;

	default:
		warning(OBD_PCM_Processor_Fault, "unsupported ignitionMode %d in initializeIgnitionActions()", engineConfiguration->ignitionMode);
		return &enginePins.coils[0];
	}
}

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)

void prepareOutputSignals(DECLARE_ENGINE_PARAMETER_F) {

	engine_configuration2_s *engineConfiguration2 = engine->engineConfiguration2;

	// todo: move this reset into decoder
	engine->triggerShape.calculateTriggerSynchPoint(PASS_ENGINE_PARAMETER_F);

	for (int i = 0; i < CONFIG(specs.cylindersCount); i++) {
		ENGINE(angleExtra[i])= (float) CONFIG(engineCycle) * i / CONFIG(specs.cylindersCount);

		ENGINE(ignitionPin[i]) = getIgnitionPinForIndex(i PASS_ENGINE_PARAMETER);

	}

	for (int angle = 0; angle < CONFIG(engineCycle); angle++) {
		int triggerShapeIndex = findAngleIndex(angle PASS_ENGINE_PARAMETER);
		if (engineConfiguration->useOnlyFrontForTrigger)
			triggerShapeIndex = triggerShapeIndex & 0xFFFFFFFE; // we need even index for front_only
		TRIGGER_SHAPE(triggerIndexByAngle[angle]) = triggerShapeIndex;
	}

	engineConfiguration2->crankingInjectionEvents.addFuelEvents(&crankingInjectonSignals,
			engineConfiguration->crankingInjectionMode PASS_ENGINE_PARAMETER);
	engineConfiguration2->injectionEvents.addFuelEvents(&runningInjectonSignals,
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
	setTableBin(config->ignitionRpmBins, IGN_RPM_COUNT, from, to);
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
		setTimingLoadBin(0, 100 PASS_ENGINE_PARAMETER);
	} else if (algo == LM_SPEED_DENSITY) {
		setTimingLoadBin(0, 160 PASS_ENGINE_PARAMETER);
	}
}
