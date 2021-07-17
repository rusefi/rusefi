/*
 * @file aux_valves.cpp
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
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pin_repository.h"
#include "aux_valves.h"
#include "allsensors.h"
#include "trigger_central.h"
#include "spark_logic.h"

static void plainPinTurnOff(NamedOutputPin *output) {
	output->setLow();
}


static void scheduleOpen(AuxActor *current) {

#if EFI_UNIT_TEST
	Engine *engine = current->engine;
	EXPAND_Engine;
#endif /* EFI_UNIT_TEST */

	scheduleOrQueue(&current->open,
			TRIGGER_EVENT_UNDEFINED,
			getTimeNowNt(),
			current->extra + engine->engineState.auxValveStart,
			{ auxPlainPinTurnOn, current }
			PASS_ENGINE_PARAMETER_SUFFIX
			);
}

void auxPlainPinTurnOn(AuxActor *current) {
	NamedOutputPin *output = &enginePins.auxValve[current->valveIndex];
	output->setHigh();

#if EFI_UNIT_TEST
	Engine *engine = current->engine;
	EXPAND_Engine;
#endif /* EFI_UNIT_TEST */

	scheduleOpen(current);

	angle_t duration = engine->engineState.auxValveEnd - engine->engineState.auxValveStart;

	fixAngle(duration, "duration", CUSTOM_ERR_6557);

	scheduleOrQueue(&current->close,
			TRIGGER_EVENT_UNDEFINED,
			getTimeNowNt(),
			current->extra + engine->engineState.auxValveEnd,
			{ plainPinTurnOff, output }
			PASS_ENGINE_PARAMETER_SUFFIX
			);
	}

void initAuxValves(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (!isBrainPinValid(engineConfiguration->auxValves[0])) {
		return;
	}

	if (!Sensor::hasSensor(SensorType::DriverThrottleIntent)) {
		firmwareError(CUSTOM_OBD_91, "No TPS for Aux Valves");
		return;
	}

	recalculateAuxValveTiming(PASS_ENGINE_PARAMETER_SIGNATURE);

	for (int valveIndex = 0; valveIndex < AUX_DIGITAL_VALVE_COUNT; valveIndex++) {

		for (int phaseIndex = 0; phaseIndex < 2; phaseIndex++) {
			AuxActor *actor = &engine->auxValves[valveIndex][phaseIndex];
			actor->phaseIndex = phaseIndex;
			actor->valveIndex = valveIndex;
			actor->extra = phaseIndex * 360 + valveIndex * 180;

			INJECT_ENGINE_REFERENCE(actor);
			scheduleOpen(actor);
		}
	}
}

void recalculateAuxValveTiming(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (!isBrainPinValid(engineConfiguration->auxValves[0])) {
		return;
	}

	auto [valid, tps] = Sensor::get(SensorType::DriverThrottleIntent);
	if (!valid) {
		// error should be already reported by now
		return;
	}

	engine->engineState.auxValveStart = interpolate2d(tps,
			engineConfiguration->fsioCurve1Bins,
			engineConfiguration->fsioCurve1);

	engine->engineState.auxValveEnd = interpolate2d(tps,
			engineConfiguration->fsioCurve2Bins,
			engineConfiguration->fsioCurve2);

	if (engine->engineState.auxValveStart >= engine->engineState.auxValveEnd) {
		// this is a fatal error to make this really visible
		firmwareError(CUSTOM_AUX_OUT_OF_ORDER, "out of order at %.2f %.2f %.2f", tps,
				engine->engineState.auxValveStart,
				engine->engineState.auxValveEnd);
	}
}
