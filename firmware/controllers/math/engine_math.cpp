/**
 * @file	engine_math.cpp
 * @brief
 *
 * @date Jul 13, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
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
#include "OutputSignalList.h"
#include "trigger_decoder.h"
#include "event_registry.h"
#include "efiGpio.h"

/*
 * default Volumetric Efficiency
 */
//float getDefaultVE(int rpm) {
//	if (rpm > 5000)
//		return interpolate(5000, 1.1, 8000, 1, rpm);
//	return interpolate(500, 0.5, 5000, 1.1, rpm);
//}

/**
 * @return number of milliseconds in one crankshaft revolution
 */
float getCrankshaftRevolutionTimeMs(int rpm) {
	return 360 * getOneDegreeTimeMs(rpm);
}

/**
 * @brief Shifts angle into the [0..720) range
 * TODO: should be 'crankAngleRange' range?
 */
float fixAngle(float angle) {
	// I guess this implementation would be faster than 'angle % 720'
	while (angle < 0)
		angle += 720;
	while (angle >= 720)
		angle -= 720;
	return angle;
}

/**
 * @brief Returns engine load according to selected engine_load_mode
 *
 */
float getEngineLoadT(Engine *engine) {
	efiAssert(engine!=NULL, "engine 2NULL", NAN);
	engine_configuration_s *engineConfiguration = engine->engineConfiguration;
	efiAssert(engineConfiguration!=NULL, "engineConfiguration 2NULL", NAN);
	switch (engineConfiguration->algorithm) {
	case LM_MAF:
		return getMafT(engineConfiguration);
	case LM_SPEED_DENSITY:
		// SD engine load is used for timing lookup but not for fuel calculation
	case LM_MAP:
		return getMap();
	case LM_ALPHA_N:
		return getTPS();
	default:
		firmwareError("Unexpected engine load parameter: %d", engineConfiguration->algorithm);
		return -1;
	}
}

void setSingleCoilDwell(engine_configuration_s *engineConfiguration) {
	for (int i = 0; i < DWELL_CURVE_SIZE; i++) {
		engineConfiguration->sparkDwellBins[i] = 0;
		engineConfiguration->sparkDwell[i] = -1;
	}

	engineConfiguration->sparkDwellBins[5] = 1;
	engineConfiguration->sparkDwell[5] = 4;

	engineConfiguration->sparkDwellBins[6] = 4500;
	engineConfiguration->sparkDwell[6] = 4;

	engineConfiguration->sparkDwellBins[7] = 12500;
	engineConfiguration->sparkDwell[7] = 0;
}

int isCrankingRT(engine_configuration_s *engineConfiguration, int rpm) {
	return rpm > 0 && rpm < engineConfiguration->crankingSettings.crankingRpm;
}

OutputSignalList injectonSignals CCM_OPTIONAL
;

static void registerSparkEvent(engine_configuration_s const *engineConfiguration, trigger_shape_s * s,
		IgnitionEventList *list, io_pin_e pin, float localAdvance, float dwell) {

	IgnitionEvent *event = list->getNextActuatorEvent();
	if (event == NULL)
		return; // error already reported

	if (!isPinAssigned(pin)) {
		// todo: extact method for this index math
		warning(OBD_PCM_Processor_Fault, "pin not assigned for coil #%d", (int) pin - (int) SPARKOUT_1_OUTPUT + 1);
	}
	event->io_pin = pin;

	event->advance = localAdvance;

	findTriggerPosition(engineConfiguration, s, &event->dwellPosition, localAdvance - dwell);
}

void initializeIgnitionActions(float advance, float dwellAngle, engine_configuration_s *engineConfiguration,
		engine_configuration2_s *engineConfiguration2, IgnitionEventList *list) {

	efiAssertVoid(engineConfiguration->cylindersCount > 0, "cylindersCount");

	list->resetEventList();

	switch (engineConfiguration->ignitionMode) {
	case IM_ONE_COIL:
		for (int i = 0; i < engineConfiguration->cylindersCount; i++) {
			// todo: extract method
			float localAdvance = advance + 720.0f * i / engineConfiguration->cylindersCount;

			registerSparkEvent(engineConfiguration, &engineConfiguration2->triggerShape, list, SPARKOUT_1_OUTPUT,
					localAdvance, dwellAngle);
		}
		break;
	case IM_WASTED_SPARK:
		for (int i = 0; i < engineConfiguration->cylindersCount; i++) {
			float localAdvance = advance + 720.0f * i / engineConfiguration->cylindersCount;

			int wastedIndex = i % (engineConfiguration->cylindersCount / 2);

			int id = getCylinderId(engineConfiguration->firingOrder, wastedIndex) - 1;
			io_pin_e ioPin = (io_pin_e) (SPARKOUT_1_OUTPUT + id);

			registerSparkEvent(engineConfiguration, &engineConfiguration2->triggerShape, list, ioPin, localAdvance,
					dwellAngle);

		}

		break;
	case IM_INDIVIDUAL_COILS:
		for (int i = 0; i < engineConfiguration->cylindersCount; i++) {
			float localAdvance = advance + 720.0f * i / engineConfiguration->cylindersCount;

			io_pin_e pin = (io_pin_e) ((int) SPARKOUT_1_OUTPUT + getCylinderId(engineConfiguration->firingOrder, i) - 1);
			registerSparkEvent(engineConfiguration, &engineConfiguration2->triggerShape, list, pin, localAdvance,
					dwellAngle);
		}
		break;

	default:
		firmwareError("unsupported ignitionMode %d in initializeIgnitionActions()", engineConfiguration->ignitionMode);
	}
}

static void registerInjectionEvent(engine_configuration_s const *e, trigger_shape_s *s, ActuatorEventList *list,
		io_pin_e pin, float angle) {

	if (!isPinAssigned(pin)) {
		// todo: extact method for this index math
		warning(OBD_PCM_Processor_Fault, "pin not assigned for injector #%d", (int) pin - (int) INJECTOR_1_OUTPUT + 1);
	}

	registerActuatorEventExt(e, s, list->getNextActuatorEvent(), injectonSignals.add(pin), angle);
}

void addFuelEvents(engine_configuration_s const *e, engine_configuration2_s *engineConfiguration2,
		ActuatorEventList *list, injection_mode_e mode) {
	list->resetEventList();

	trigger_shape_s *s = &engineConfiguration2->triggerShape;

	float baseAngle = e->globalTriggerAngleOffset + e->injectionOffset;

	switch (mode) {
	case IM_SEQUENTIAL:
		for (int i = 0; i < e->cylindersCount; i++) {
			io_pin_e pin = (io_pin_e) ((int) INJECTOR_1_OUTPUT + getCylinderId(e->firingOrder, i) - 1);
			float angle = baseAngle + i * 720.0 / e->cylindersCount;
			registerInjectionEvent(e, s, list, pin, angle);
		}
		break;
	case IM_SIMULTANEOUS:
		for (int i = 0; i < e->cylindersCount; i++) {
			float angle = baseAngle + i * 720.0 / e->cylindersCount;

			for (int j = 0; j < e->cylindersCount; j++) {
				io_pin_e pin = (io_pin_e) ((int) INJECTOR_1_OUTPUT + j);
				registerInjectionEvent(e, s, list, pin, angle);
			}
		}
		break;
	case IM_BATCH:
		for (int i = 0; i < e->cylindersCount; i++) {
			int index = i % (e->cylindersCount / 2);
			io_pin_e pin = (io_pin_e) ((int) INJECTOR_1_OUTPUT + index);
			float angle = baseAngle + i * 720.0 / e->cylindersCount;
			registerInjectionEvent(e, s, list, pin, angle);

			/**
			 * also fire the 2nd half of the injectors so that we can implement a batch mode on individual wires
			 */
			pin = (io_pin_e) ((int) INJECTOR_1_OUTPUT + index + (e->cylindersCount / 2));
			registerInjectionEvent(e, s, list, pin, angle);
		}
		break;
	default:
		firmwareError("Unexpected injection mode %d", mode);
	}
}

/**
 * @return Spark dwell time, in milliseconds.
 */
float getSparkDwellMsT(engine_configuration_s *engineConfiguration, int rpm) {
	if (isCrankingR(rpm)) {
		// technically this could be implemented via interpolate2d
		float angle = engineConfiguration->crankingChargeAngle;
		return getOneDegreeTimeMs(rpm) * angle;
	}
	efiAssert(!cisnan(rpm), "invalid rpm", NAN);

	return interpolate2d(rpm, engineConfiguration->sparkDwellBins, engineConfiguration->sparkDwell, DWELL_CURVE_SIZE);
}

int getEngineCycleEventCount2(operation_mode_e mode, trigger_shape_s * s) {
	return mode == FOUR_STROKE_CAM_SENSOR ? s->getSize() : 2 * s->getSize();
}

/**
 * Trigger event count equals engine cycle event count if we have a cam sensor.
 * Two trigger cycles make one engine cycle in case of a four stroke engine If we only have a cranksensor.
 */
int getEngineCycleEventCount(engine_configuration_s const *engineConfiguration, trigger_shape_s * s) {
	return getEngineCycleEventCount2(getOperationMode(engineConfiguration), s);
}

void findTriggerPosition(engine_configuration_s const *engineConfiguration, trigger_shape_s * s,
		event_trigger_position_s *position, float angleOffset) {

	angleOffset = fixAngle(angleOffset + engineConfiguration->globalTriggerAngleOffset);

	int engineCycleEventCount = getEngineCycleEventCount(engineConfiguration, s);

	efiAssertVoid(engineCycleEventCount > 0, "engineCycleEventCount");

	uint32_t middle;
	uint32_t left = 0;
	uint32_t right = engineCycleEventCount - 1;

	/**
	 * Let's find the last trigger angle which is less or equal to the desired angle
	 * todo: extract binary search as template method?
	 */
	while (true) {
		middle = (left + right) / 2;

		if (middle == left) {
			break;
		}

		if (angleOffset < s->eventAngles[middle]) {
			right = middle;
		} else if (angleOffset > s->eventAngles[middle]) {
			left = middle;
		} else {
			break;
		}

	}

	float eventAngle = s->eventAngles[middle];

	if (angleOffset < eventAngle) {
		firmwareError("angle constraint violation in registerActuatorEventExt(): %f/%f", angleOffset, eventAngle);
		return;
	}

	position->eventIndex = middle;
	position->eventAngle = eventAngle;
	position->angleOffset = angleOffset - eventAngle;
}

void registerActuatorEventExt(engine_configuration_s const *engineConfiguration, trigger_shape_s * s, ActuatorEvent *e,
		OutputSignal *actuator, float angleOffset) {
	efiAssertVoid(s->getSize() > 0, "uninitialized trigger_shape_s");

	if (e == NULL) {
		// error already reported
		return;
	}
	e->actuator = actuator;

	findTriggerPosition(engineConfiguration, s, &e->position, angleOffset);
}

static int order_1_THEN_3_THEN_4_THEN2[] = { 1, 3, 4, 2 };

static int order_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4[] = { 1, 5, 3, 6, 2, 4 };

static int order_1_8_4_3_6_5_7_2[] = { 1, 8, 4, 3, 6, 5, 7, 2 };

/**
 * @param index from zero to cylindersCount - 1
 * @return cylinderId from one to cylindersCount
 */
int getCylinderId(firing_order_e firingOrder, int index) {

	switch (firingOrder) {
	case FO_ONE_CYLINDER:
		return 1;
	case FO_1_THEN_3_THEN_4_THEN2:
		return order_1_THEN_3_THEN_4_THEN2[index];
	case FO_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4:
		return order_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4[index];
	case FO_1_8_4_3_6_5_7_2:
		return order_1_8_4_3_6_5_7_2[index];

	default:
		firmwareError("getCylinderId not supported for %d", firingOrder);
	}
	return -1;
}

void prepareOutputSignals(engine_configuration_s *engineConfiguration, engine_configuration2_s *engineConfiguration2) {

	// todo: move this reset into decoder
	engineConfiguration2->triggerShape.calculateTriggerSynchPoint(&engineConfiguration->triggerConfig);

	injectonSignals.clear();
	EventHandlerConfiguration *config = &engineConfiguration2->engineEventConfiguration;
	addFuelEvents(engineConfiguration, engineConfiguration2, &config->crankingInjectionEvents,
			engineConfiguration->crankingInjectionMode);
	addFuelEvents(engineConfiguration, engineConfiguration2, &config->injectionEvents,
			engineConfiguration->injectionMode);
}

void setFuelRpmBin(engine_configuration_s *engineConfiguration, float l, float r) {
	setTableBin(engineConfiguration->fuelRpmBins, FUEL_RPM_COUNT, l, r);
}

void setFuelLoadBin(engine_configuration_s *engineConfiguration, float l, float r) {
	setTableBin(engineConfiguration->fuelLoadBins, FUEL_LOAD_COUNT, l, r);
}

void setTimingRpmBin(engine_configuration_s *engineConfiguration, float l, float r) {
	setTableBin(engineConfiguration->ignitionRpmBins, IGN_RPM_COUNT, l, r);
}

void setTimingLoadBin(engine_configuration_s *engineConfiguration, float l, float r) {
	setTableBin(engineConfiguration->ignitionLoadBins, IGN_LOAD_COUNT, l, r);
}

int isInjectionEnabled(engine_configuration_s *engineConfiguration) {
	// todo: is this worth a method? should this be inlined?
	return engineConfiguration->isInjectionEnabled;
}
