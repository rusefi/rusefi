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

/*
 * default Volumetric Efficiency
 */
//float getDefaultVE(int rpm) {
//	if (rpm > 5000)
//		return interpolate(5000, 1.1, 8000, 1, rpm);
//	return interpolate(500, 0.5, 5000, 1.1, rpm);
//}
/**
 * @return time needed to rotate crankshaft by one degree, in milliseconds.
 * @deprecated
 */
float getOneDegreeTimeMs(int rpm) {
	return 1000.0 * 60 / 360 / rpm;
}

/**
 * @return time needed to rotate crankshaft by one degree, in microseconds.
 */
float getOneDegreeTimeUs(int rpm) {
	return 1000000.0 * 60 / 360 / rpm;
}

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
	while (angle > 720)
		angle -= 720;
	return angle;
}

/**
 * @brief Returns engine load according to selected engine_load_mode
 *
 */
float getEngineLoadT(engine_configuration_s *engineConfiguration) {
	switch (engineConfiguration->engineLoadMode) {
	case LM_MAF:
		return getMaf();
	case LM_MAP:
		return getMap();
	case LM_TPS:
		return getTPS();
	case LM_SPEED_DENSITY:
		// TODO: real implementation
		return getMap();
	default:
		firmwareError("Unexpected engine load parameter: %d", engineConfiguration->engineLoadMode);
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

OutputSignalList ignitionSignals;
OutputSignalList injectonSignals;

static void registerSparkEvent(engine_configuration_s const *engineConfiguration, trigger_shape_s * s,
		IgnitionEventList *list, OutputSignal *actuator, float localAdvance, float dwell) {

	IgnitionEvent *event = list->getNextActuatorEvent();
	if (event == NULL)
		return; // error already reported

	event->advance = localAdvance;

	registerActuatorEventExt(engineConfiguration, s, &event->actuator, actuator, localAdvance - dwell);
}

void initializeIgnitionActions(float advance, float dwellAngle, engine_configuration_s *engineConfiguration,
		engine_configuration2_s *engineConfiguration2, IgnitionEventList *list) {

	efiAssertVoid(engineConfiguration->cylindersCount > 0, "cylindersCount");
	ignitionSignals.clear();

	list->resetEventList();

	switch (engineConfiguration->ignitionMode) {
	case IM_ONE_COIL:
		for (int i = 0; i < engineConfiguration->cylindersCount; i++) {
			// todo: extract method
			float localAdvance = advance + 720.0 * i / engineConfiguration->cylindersCount;

			registerSparkEvent(engineConfiguration, &engineConfiguration2->triggerShape, list,
					ignitionSignals.add(SPARKOUT_1_OUTPUT), localAdvance, dwellAngle);
		}
		break;
	case IM_WASTED_SPARK:
		for (int i = 0; i < engineConfiguration->cylindersCount; i++) {
			float localAdvance = advance + 720.0 * i / engineConfiguration->cylindersCount;

			int wastedIndex = i % (engineConfiguration->cylindersCount / 2);

			int id = getCylinderId(engineConfiguration->firingOrder, wastedIndex) - 1;
			io_pin_e ioPin = (io_pin_e) (SPARKOUT_1_OUTPUT + id);

			registerSparkEvent(engineConfiguration, &engineConfiguration2->triggerShape, list,
					ignitionSignals.add(ioPin), localAdvance, dwellAngle);

		}

		break;
	case IM_INDIVIDUAL_COILS:
		for (int i = 0; i < engineConfiguration->cylindersCount; i++) {
			float localAdvance = advance + 720.0 * i / engineConfiguration->cylindersCount;

			io_pin_e pin = (io_pin_e) ((int) SPARKOUT_1_OUTPUT + getCylinderId(engineConfiguration->firingOrder, i) - 1);
			registerSparkEvent(engineConfiguration, &engineConfiguration2->triggerShape, list, ignitionSignals.add(pin),
					localAdvance, dwellAngle);
		}
		break;

	default:
		firmwareError("unsupported ignitionMode %d in initializeIgnitionActions()", engineConfiguration->ignitionMode);
	}
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
			registerActuatorEventExt(e, s, list->getNextActuatorEvent(), injectonSignals.add(pin), angle);
		}
		break;
	case IM_SIMULTANEOUS:
		for (int i = 0; i < e->cylindersCount; i++) {
			float angle = baseAngle + i * 720.0 / e->cylindersCount;

			for (int j = 0; j < e->cylindersCount; j++) {
				io_pin_e pin = (io_pin_e) ((int) INJECTOR_1_OUTPUT + j);
				registerActuatorEventExt(e, s, list->getNextActuatorEvent(), injectonSignals.add(pin), angle);
			}
		}
		break;
	case IM_BATCH:
		for (int i = 0; i < e->cylindersCount; i++) {
			io_pin_e pin = (io_pin_e) ((int) INJECTOR_1_OUTPUT + (i % 2));
			float angle = baseAngle + i * 720.0 / e->cylindersCount;
			registerActuatorEventExt(e, s, list->getNextActuatorEvent(), injectonSignals.add(pin), angle);
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

/**
 * Trigger event count equals engine cycle event count if we have a cam sensor.
 * Two trigger cycles make one engine cycle in case of a four stroke engine If we only have a cranksensor.
 */
int getEngineCycleEventCount(engine_configuration_s const *engineConfiguration, trigger_shape_s * s) {
	return getOperationMode(engineConfiguration) == FOUR_STROKE_CAM_SENSOR ? s->getSize() : 2 * s->getSize();
}

void findTriggerPosition(engine_configuration_s const *engineConfiguration, trigger_shape_s * s,
		event_trigger_position_s *position, float angleOffset) {

	angleOffset = fixAngle(angleOffset + engineConfiguration->globalTriggerAngleOffset);

	// todo: migrate to crankAngleRange?
	float firstAngle = s->wave.getAngle(s->triggerShapeSynchPointIndex, engineConfiguration, s);

	int engineCycleEventCount = getEngineCycleEventCount(engineConfiguration, s);

	// let's find the last trigger angle which is less or equal to the desired angle
	int i;
	for (i = 0; i < engineCycleEventCount - 1; i++) {
		// todo: we need binary search here
		float angle = fixAngle(
				s->wave.getAngle((s->triggerShapeSynchPointIndex + i + 1) % engineCycleEventCount, engineConfiguration, s)
						- firstAngle);
		if (angle > angleOffset)
			break;
	}
	// explicit check for zero to avoid issues where logical zero is not exactly zero due to float nature
	float eventAngle;
	if (i == 0) {
		eventAngle = 0;
	} else {
		eventAngle = fixAngle(
				s->wave.getAngle((s->triggerShapeSynchPointIndex + i) % engineCycleEventCount, engineConfiguration, s)
						- firstAngle);
	}

	if (angleOffset < eventAngle) {
		firmwareError("angle constraint violation in registerActuatorEventExt(): %f/%f", angleOffset, eventAngle);
		return;
	}

	position->eventIndex = i;
	position->eventAngle = eventAngle;
	position->angleOffset = angleOffset - eventAngle;
}

void registerActuatorEventExt(engine_configuration_s const *engineConfiguration, trigger_shape_s * s, ActuatorEvent *e,
		OutputSignal *actuator, float angleOffset) {
	efiAssertVoid(s->getSize() > 0, "uninitialized trigger_shape_s");

	if (e == NULL)
		return; // error already reported
	e->actuator = actuator;

	findTriggerPosition(engineConfiguration, s, &e->position, angleOffset);
}

static int order_1_THEN_3_THEN_4_THEN2[] = { 1, 3, 4, 2 };

static int order_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4[] = { 1, 5, 3, 6, 2, 4 };

static int order_1_8_4_3_6_5_7_2[] = {1, 8, 4, 3, 6, 5, 7, 2};

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
	engineConfiguration2->triggerShape.triggerShapeSynchPointIndex = findTriggerZeroEventIndex(
			&engineConfiguration2->triggerShape, &engineConfiguration->triggerConfig);

	injectonSignals.clear();
	EventHandlerConfiguration *config = &engineConfiguration2->engineEventConfiguration;
	addFuelEvents(engineConfiguration, engineConfiguration2, &config->crankingInjectionEvents,
			engineConfiguration->crankingInjectionMode);
	addFuelEvents(engineConfiguration, engineConfiguration2, &config->injectionEvents,
			engineConfiguration->injectionMode);
}

void setTableBin(float array[], int size, float l, float r) {
	for (int i = 0; i < size; i++) {
		float value = interpolate(0, l, size - 1, r, i);
		/**
		 * rounded values look nicer, also we want to avoid precision mismatch with Tuner Studio
		 */
		array[i] = efiRound(value, 0.01);
	}
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

int isInjectionEnabled(engine_configuration2_s const *engineConfiguration2) {
	return engineConfiguration2->isInjectionEnabledFlag;
}
