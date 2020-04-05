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
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "engine_math.h"
#include "aux_valves.h"
#include "allsensors.h"
#include "sensor.h"
#include "trigger_central.h"
#include "spark_logic.h"

EXTERN_ENGINE;

void plainPinTurnOn(AuxActor *current) {
	NamedOutputPin *output = &enginePins.auxValve[current->valveIndex];
	output->setHigh();

#if EFI_UNIT_TEST
	Engine *engine = current->engine;
	EXPAND_Engine;
#endif /* EFI_UNIT_TEST */

	scheduleOrQueue(&current->open,
			TRIGGER_EVENT_UNDEFINED,
			getTimeNowNt(),
			current->extra + engine->engineState.auxValveStart,
			{ plainPinTurnOn, current }
			PASS_ENGINE_PARAMETER_SUFFIX
			);

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

void plainPinTurnOff(NamedOutputPin *output) {
	output->setLow();
}

/*
static void auxValveTriggerCallback(trigger_event_e ckpSignalType,
		uint32_t index DECLARE_ENGINE_PARAMETER_SUFFIX) {
	UNUSED(ckpSignalType);

	if (index != engine->auxSchedulingIndex) {
		return;
	}
	int rpm = GET_RPM_VALUE;
	if (!isValidRpm(rpm)) {
		return;
	}
*/
	/**
	 * Sometimes previous event has not yet been executed by the time we are scheduling new events.
	 * We use this array alternation in order to bring events that are scheled and waiting to be executed from
	 * events which are already being scheduled
	 */
/*
	int engineCycleAlternation = engine->triggerCentral.triggerState.getTotalRevolutionCounter() % CYCLE_ALTERNATION;

	for (int valveIndex = 0; valveIndex < AUX_DIGITAL_VALVE_COUNT; valveIndex++) {

		NamedOutputPin *output = &enginePins.auxValve[valveIndex];

		for (int phaseIndex = 0; phaseIndex < 2; phaseIndex++) {
*/
/* I believe a more correct implementation is the following:
 * here we properly account for trigger angle position in engine cycle coordinates
			// todo: at the moment this logic is assuming four-stroke 720-degree engine cycle
			angle_t extra = phaseIndex * 360 // cycle opens twice per 720 engine cycle
					+ valveIndex * 180 // 2nd valve is operating at 180 offset to first
					+ tdcPosition() // engine cycle position to trigger cycle position conversion
					- ENGINE(triggerCentral.triggerShape.eventAngles[SCHEDULING_TRIGGER_INDEX])
					;
*/
/*
			angle_t extra = phaseIndex * 360 + valveIndex * 180;
			angle_t onTime = extra + engine->engineState.auxValveStart;
			scheduling_s *onEvent = &engine->auxTurnOnEvent[valveIndex][phaseIndex][engineCycleAlternation];
			scheduling_s *offEvent = &engine->auxTurnOffEvent[valveIndex][phaseIndex][engineCycleAlternation];
			bool isOverlap = onEvent->isScheduled || offEvent->isScheduled;
			if (isOverlap) {
				enginePins.debugTriggerSync.setValue(1);
			}

			fixAngle(onTime, "onTime", CUSTOM_ERR_6556);
			scheduleByAngle(onEvent,
					onTime,
					&plainPinTurnOn, output PASS_ENGINE_PARAMETER_SUFFIX);
			angle_t offTime = extra + engine->engineState.auxValveEnd;
			fixAngle(offTime, "offTime", CUSTOM_ERR_6557);
			scheduleByAngle(offEvent,
					offTime,
					&plainPinTurnOff, output PASS_ENGINE_PARAMETER_SUFFIX);
			if (isOverlap) {
				enginePins.debugTriggerSync.setValue(0);
			}
		}
	}
}
*/

void initAuxValves(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX) {
	UNUSED(sharedLogger);
	if (engineConfiguration->auxValves[0] == GPIO_UNASSIGNED) {
		return;
	}

	if (!Sensor::hasSensor(SensorType::DriverThrottleIntent)) {
		warning(CUSTOM_OBD_91, "No TPS for Aux Valves");
		return;
	}

	updateAuxValves(PASS_ENGINE_PARAMETER_SIGNATURE);

	for (int valveIndex = 0; valveIndex < AUX_DIGITAL_VALVE_COUNT; valveIndex++) {

		for (int phaseIndex = 0; phaseIndex < 2; phaseIndex++) {
			AuxActor *actor = &engine->auxValves[valveIndex][phaseIndex];
			actor->phaseIndex = phaseIndex;
			actor->valveIndex = valveIndex;
			actor->extra = phaseIndex * 360 + valveIndex * 180;

			INJECT_ENGINE_REFERENCE(actor);

			scheduleOrQueue(&actor->open,
					TRIGGER_EVENT_UNDEFINED,
					getTimeNowNt(),
					actor->extra + engine->engineState.auxValveStart,
					{ plainPinTurnOn, actor }
					PASS_ENGINE_PARAMETER_SUFFIX
					);
		}
	}


//	addTriggerEventListener(auxValveTriggerCallback, "AuxV", engine);
}

void updateAuxValves(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (engineConfiguration->auxValves[0] == GPIO_UNASSIGNED) {
		return;
	}

	auto [valid, tps] = Sensor::get(SensorType::DriverThrottleIntent);
	if (!valid) {
		// error should be already reported by now
		return;
	}

	engine->engineState.auxValveStart = interpolate2d("aux", tps,
			engineConfiguration->fsioCurve1Bins,
			engineConfiguration->fsioCurve1);

	engine->engineState.auxValveEnd = interpolate2d("aux", tps,
			engineConfiguration->fsioCurve2Bins,
			engineConfiguration->fsioCurve2);

	if (engine->engineState.auxValveStart >= engine->engineState.auxValveEnd) {
		// this is a fatal error to make this really visible
		firmwareError(CUSTOM_AUX_OUT_OF_ORDER, "out of order at %.2f %.2f %.2f", tps,
				engine->engineState.auxValveStart,
				engine->engineState.auxValveEnd);
	}
}
