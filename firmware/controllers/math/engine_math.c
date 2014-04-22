/**
 * @file	engine_math.c
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

//#include <stdio.h>

#include "engine_math.h"
#include "main.h"
#include "engine_configuration.h"
#include "interpolation.h"
#include "allsensors.h"
#include "io_pins.h"

/*
 * default Volumetric Efficiency
 */
//float getDefaultVE(int rpm) {
//	if (rpm > 5000)
//		return interpolate(5000, 1.1, 8000, 1, rpm);
//	return interpolate(500, 0.5, 5000, 1.1, rpm);
//}
//#define K_AT_MIN_RPM_MIN_TPS 0.25
//#define K_AT_MIN_RPM_MAX_TPS 0.25
//#define K_AT_MAX_RPM_MIN_TPS 0.25
//#define K_AT_MAX_RPM_MAX_TPS 0.9
//
//#define rpmMin 500
//#define rpmMax 8000
//
//#define tpMin 0
//#define tpMax 100
//
//  http://rusefi.com/math/t_charge.html
// /
//float getTCharge(int rpm, int tps, float coolantTemp, float airTemp) {
//	float minRpmKcurrentTPS = interpolate(tpMin, K_AT_MIN_RPM_MIN_TPS, tpMax,
//	K_AT_MIN_RPM_MAX_TPS, tps);
//	float maxRpmKcurrentTPS = interpolate(tpMin, K_AT_MAX_RPM_MIN_TPS, tpMax,
//	K_AT_MAX_RPM_MAX_TPS, tps);
//
//	float Tcharge_coff = interpolate(rpmMin, minRpmKcurrentTPS, rpmMax,
//			maxRpmKcurrentTPS, rpm);
//
//	float Tcharge = coolantTemp * (1 - Tcharge_coff) + airTemp * Tcharge_coff;
//
//	return Tcharge;
//}
#define MAX_STARTING_FUEL 15
#define MIN_STARTING_FUEL 8

/**
 * @return time needed to rotate crankshaft by one degree, in milliseconds.
 */
float getOneDegreeTimeMs(int rpm) {
	return 1000.0 * 60 / 360 / rpm;
}

/**
 * @return time needed to rotate crankshaft by one degree, in systicks
 */
float getOneDegreeTime(int rpm) {
	return getOneDegreeTimeMs(rpm) * TICKS_IN_MS;
}

/**
 * @return number of system it needed for one crankshaft revolution
 */
float getCrankshaftRevolutionTime(int rpm) {
	return 360 * getOneDegreeTime(rpm);
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
		fatal("Unexpected engine load parameter");
		return -1;
	}
}

int isCrankingRT(engine_configuration_s *engineConfiguration, int rpm) {
	return rpm > 0 && rpm < engineConfiguration->crankingSettings.crankingRpm;
}

void initializeIgnitionActions(engine_configuration_s *engineConfiguration, engine_configuration2_s *engineConfiguration2) {
	EventHandlerConfiguration *config = &engineConfiguration2->engineEventConfiguration;
	resetEventList(&config->ignitionEvents);
	chDbgCheck(engineConfiguration->cylindersCount > 0, "cylindersCount");

	int x = 13; //todo

	if (engineConfiguration->ignitionMode == IM_ONE_COIL) {

		for (int i = 0; i < engineConfiguration->cylindersCount; i++) {
			float angle = x + 720.0 * i / engineConfiguration->cylindersCount;

			registerActuatorEventExt(engineConfiguration,
					&engineConfiguration2->triggerShape,
					&config->ignitionEvents, addOutputSignal(SPARKOUT_1_OUTPUT), angle);
		}

	} else
		fatal("unfinished initializeIgnitionActions");

}

void addFuelEvents(engine_configuration_s *e,  trigger_shape_s * s, EventHandlerConfiguration *config) {
	resetEventList(&config->injectionEvents);

	for(int i = 0;i < e->cylindersCount;i++) {
		io_pin_e pin = (io_pin_e)((int)INJECTOR_1_OUTPUT + getCylinderId(e->firingOrder, i) - 1);
		float angle = e->injectionOffset + i * 720.0 / e->cylindersCount;
		registerActuatorEventExt(e, s, &config->injectionEvents, addOutputSignal(pin), angle);
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

	if (rpm > engineConfiguration->rpmHardLimit) {
		// technically this could be implemented via interpolate2d by setting everything above rpmHardLimit to zero
		warning("skipping spark due to rpm=%d", rpm);
		return 0;
	}

	return interpolate2d(rpm, engineConfiguration->sparkDwellBins, engineConfiguration->sparkDwell, DWELL_CURVE_SIZE);
}

void registerActuatorEventExt(engine_configuration_s *engineConfiguration, trigger_shape_s * s, ActuatorEventList *list, OutputSignal *actuator, float angleOffset) {
	chDbgCheck(s->size > 0, "uninitialized trigger_shape_s");

	angleOffset = fixAngle(angleOffset + engineConfiguration->globalTriggerAngleOffset);

	// todo: migrate to crankAngleRange?
	float firstAngle = s->wave.switchTimes[0] * 720;

	// let's find the last trigger angle which is less or equal to the desired angle
	int i;
	for (i = 0; i < s->size - 1; i++) {
		// todo: we need binary search here
		float angle = s->wave.switchTimes[i + 1] * 720 - firstAngle;
		if (angle > angleOffset)
			break;
	}
	float angle = s->wave.switchTimes[i] * 720 - firstAngle;

	registerActuatorEvent(list, i, actuator, angleOffset - angle);
}


//float getTriggerEventAngle(int triggerEventIndex) {
//	return 0;
//}

/**
 * there is some BS related to isnan in MinGW, so let's have all the issues in one place
 */
int cisnan(float f) {
	return *(((int*)(&f)))  == 0x7FC00000;
}

static int order_1_THEN_3_THEN_4_THEN2[] = {1, 3, 4, 2};

/**
 * @param index from zero to cylindersCount - 1
 * @return cylinderId from one to cylindersCount
 */
int getCylinderId(firing_order_e firingOrder, int index) {

	switch(firingOrder) {
	case FO_ONE_CYLINDER:
		return 1;
	case FO_1_THEN_3_THEN_4_THEN2:
		return order_1_THEN_3_THEN_4_THEN2[index];

	default:
		firmwareError("getCylinderId not supported for %d", firingOrder);
	}
	return -1;
}

