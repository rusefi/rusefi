/*
 * @file aux_valves.cpp
 *
 *
 * Here we have two auxilary digital on/off outputs which would open once per each 360 degrees of engine crank revolution.
 * The second valve is 180 degrees after the first one.
 *
 * Valve open and close angles are taken from scriptCurve1 and scriptCurve2 tables respectively, the position depend on TPS input.
 *
 * https://github.com/rusefi/rusefi/issues/490
 *
 * @date Nov 25, 2017
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "aux_valves.h"
#include "trigger_central.h"
#include "spark_logic.h"

#if EFI_AUX_VALVES

static void plainPinTurnOff(NamedOutputPin *output) {
	output->setLow();
}


static void scheduleOpen(AuxActor *current) {
	engine->module<TriggerScheduler>()->schedule(
			"aux-valve",
			&current->open,
			current->extra + engine->engineState.auxValveStart,
			{ auxPlainPinTurnOn, current }
			);
}

void auxPlainPinTurnOn(AuxActor *current) {
	NamedOutputPin *output = &enginePins.auxValve[current->valveIndex];
	output->setHigh();

	scheduleOpen(current);

	angle_t duration = engine->engineState.auxValveEnd - engine->engineState.auxValveStart;

	wrapAngle(duration, "duration", ObdCode::CUSTOM_ERR_6557);

	engine->module<TriggerScheduler>()->schedule(
			"aux-valve",
			&current->close,
			current->extra + engine->engineState.auxValveEnd,
			{ plainPinTurnOff, output }
			);
	}

void initAuxValves() {
	if (!isBrainPinValid(engineConfiguration->auxValves[0])) {
		return;
	}

	if (!Sensor::hasSensor(SensorType::DriverThrottleIntent)) {
		firmwareError(ObdCode::CUSTOM_OBD_91, "No TPS for Aux Valves");
		return;
	}

	recalculateAuxValveTiming();

	for (int valveIndex = 0; valveIndex < AUX_DIGITAL_VALVE_COUNT; valveIndex++) {

		for (int phaseIndex = 0; phaseIndex < 2; phaseIndex++) {
			AuxActor *actor = &engine->auxValves[valveIndex][phaseIndex];
			actor->phaseIndex = phaseIndex;
			actor->valveIndex = valveIndex;
			actor->extra = phaseIndex * 360 + valveIndex * 180;

			scheduleOpen(actor);
		}
	}
}

void recalculateAuxValveTiming() {
	if (!isBrainPinValid(engineConfiguration->auxValves[0])) {
		return;
	}

	auto tps = Sensor::get(SensorType::DriverThrottleIntent);
	if (!tps) {
		// error should be already reported by now
		return;
	}

	engine->engineState.auxValveStart = interpolate2d(tps.Value,
			config->scriptCurve1Bins,
			config->scriptCurve1);

	engine->engineState.auxValveEnd = interpolate2d(tps.Value,
			config->scriptCurve2Bins,
			config->scriptCurve2);

	if (engine->engineState.auxValveStart >= engine->engineState.auxValveEnd) {
		// this is a fatal error to make this really visible
		firmwareError(ObdCode::CUSTOM_AUX_OUT_OF_ORDER, "out of order at %.2f %.2f %.2f", tps,
				engine->engineState.auxValveStart,
				engine->engineState.auxValveEnd);
	}
}

#endif // EFI_AUX_VALVES
