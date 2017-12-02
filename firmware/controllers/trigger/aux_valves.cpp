/*
 * aux_valves.cpp
 *
 *
 * Here we have two auxilary digital on/off outputs which would open once per each 360 degrees of engine crank revolution.
 * The second valve is 180 degrees after the first one.
 *
 * Valve open and close angles are taken from fsioCurve1 and fsioCurve2 tables respectively, the position depend on TPS input.
 *
 * https://github.com/rusefi/rusefi/issues/490
 *
 * @date Nov 25, 2017
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include "aux_valves.h"
#include "allsensors.h"
#include "trigger_central.h"
#include "engine_math.h"

EXTERN_ENGINE
;

static scheduling_s turnOnEvent[AUX_DIGITAL_VALVE_COUNT][2];
static scheduling_s turnOffEvent[AUX_DIGITAL_VALVE_COUNT][2];

static void turnOn(NamedOutputPin *output) {
	output->setHigh();
}

static void turnOff(NamedOutputPin *output) {
	output->setLow();
}

static void auxValveTriggerCallback(trigger_event_e ckpSignalType,
		uint32_t index DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if EFI_PROD_CODE || EFI_SIMULATOR || defined(__DOXYGEN__)
	if (index != 2) {
		return;
	}
	int rpm = ENGINE(rpmCalculator.rpmValue);
	if (!isValidRpm(rpm)) {
		return;
	}

	for (int valveIndex = 0; valveIndex < AUX_DIGITAL_VALVE_COUNT;
			valveIndex++) {

		NamedOutputPin *output = &enginePins.auxValve[valveIndex];

		for (int phaseIndex = 0; phaseIndex < 2; phaseIndex++) {
			angle_t extra = phaseIndex * 360 + valveIndex * 180;
			angle_t onTime = extra + engine->engineState.auxValveStart;
			fixAngle(onTime, "onTime");
			scheduleByAngle(rpm, &turnOnEvent[valveIndex][phaseIndex],
					onTime,
					(schfunc_t) &turnOn, output, &engine->rpmCalculator);
			angle_t offTime = extra + engine->engineState.auxValveEnd;
			fixAngle(offTime, "offTime");
			scheduleByAngle(rpm, &turnOffEvent[valveIndex][phaseIndex],
					offTime,
					(schfunc_t) &turnOff, output, &engine->rpmCalculator);

		}
	}

#endif /* EFI_PROD_CODE || EFI_SIMULATOR */
}

void initAuxValves(Logging *sharedLogger) {
#if EFI_PROD_CODE || EFI_SIMULATOR || defined(__DOXYGEN__)
	if (engineConfiguration->auxValves[0] == GPIO_UNASSIGNED) {
		return;
	}
	addTriggerEventListener(auxValveTriggerCallback, "tach", engine);
#endif /* EFI_PROD_CODE || EFI_SIMULATOR */
}

void updateAuxValves(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (engineConfiguration->auxValves[0] == GPIO_UNASSIGNED) {
		return;
	}

	float x = getTPS(PASS_ENGINE_PARAMETER_SIGNATURE);
	engine->engineState.auxValveStart = interpolate2d("aux", x,
			engineConfiguration->fsioCurve1Bins,
			engineConfiguration->fsioCurve1, FSIO_CURVE_16);

	engine->engineState.auxValveEnd = interpolate2d("aux", x,
			engineConfiguration->fsioCurve2Bins,
			engineConfiguration->fsioCurve2, FSIO_CURVE_16);
}

