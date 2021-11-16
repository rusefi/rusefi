/**
 * @file    rpm_calculator.cpp
 * @brief   RPM calculator
 *
 * Here we listen to position sensor events in order to figure our if engine is currently running or not.
 * Actual getRpm() is calculated once per crankshaft revolution, based on the amount of time passed
 * since the start of previous shaft revolution.
 *
 * We also have 'instant RPM' logic separate from this 'cycle RPM' logic. Open question is why do we not use
 * instant RPM instead of cycle RPM more often.
 *
 * @date Jan 1, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "os_access.h"

#include "trigger_central.h"
#include "tooth_logger.h"

#if EFI_PROD_CODE
#include "os_util.h"
#endif /* EFI_PROD_CODE */

#if EFI_SENSOR_CHART
#include "sensor_chart.h"
#endif



#if EFI_ENGINE_SNIFFER
#include "engine_sniffer.h"
extern WaveChart waveChart;
#endif /* EFI_ENGINE_SNIFFER */

// See RpmCalculator::checkIfSpinning()
#ifndef NO_RPM_EVENTS_TIMEOUT_SECS
#define NO_RPM_EVENTS_TIMEOUT_SECS 2
#endif /* NO_RPM_EVENTS_TIMEOUT_SECS */

float RpmCalculator::getRpmAcceleration() const {
	return rpmRate;
}

bool RpmCalculator::isStopped() const {
	// Spinning-up with zero RPM means that the engine is not ready yet, and is treated as 'stopped'.
	return state == STOPPED || (state == SPINNING_UP && rpmValue == 0);
}

bool RpmCalculator::isCranking() const {
	// Spinning-up with non-zero RPM is suitable for all engine math, as good as cranking
	return state == CRANKING || (state == SPINNING_UP && rpmValue > 0);
}

bool RpmCalculator::isSpinningUp() const {
	return state == SPINNING_UP;
}

uint32_t RpmCalculator::getRevolutionCounterSinceStart(void) const {
	return revolutionCounterSinceStart;
}

/**
 * @return -1 in case of isNoisySignal(), current RPM otherwise
 * See NOISY_RPM
 */
// todo: migrate to float return result or add a float version? this would have with calculations
int RpmCalculator::getRpm() const {
#if !EFI_PROD_CODE
	if (mockRpm != MOCK_UNDEFINED) {
		return mockRpm;
	}
#endif /* EFI_PROD_CODE */
	return rpmValue;
}

#if EFI_SHAFT_POSITION_INPUT

RpmCalculator::RpmCalculator() :
		StoredValueSensor(SensorType::Rpm, 0)
	{
#if !EFI_PROD_CODE
	mockRpm = MOCK_UNDEFINED;
#endif /* EFI_PROD_CODE */
	// todo: reuse assignRpmValue() method which needs
	// which we cannot provide inside this parameter-less constructor. need a solution for this minor mess
	setValidValue(0, 0);	// 0 for current time since RPM sensor never times out
}

/**
 * @return true if there was a full shaft revolution within the last second
 */
bool RpmCalculator::isRunning() const {
	return state == RUNNING;
}

/**
 * @return true if engine is spinning (cranking or running)
 */
bool RpmCalculator::checkIfSpinning(efitick_t nowNt) const {
	/**
	 * note that the result of this subtraction could be negative, that would happen if
	 * we have a trigger event between the time we've invoked 'getTimeNow' and here
	 */

	// Anything below 60 rpm is not running
	bool noRpmEventsForTooLong = lastTdcTimer.getElapsedSeconds(nowNt) > NO_RPM_EVENTS_TIMEOUT_SECS;

	/**
	 * Also check if there were no trigger events
	 */
	bool noTriggerEventsForTooLong = engine->triggerCentral.getTimeSinceTriggerEvent(nowNt) >= 1;

	if (noRpmEventsForTooLong || noTriggerEventsForTooLong) {
		return false;
	}

	return true;
}

void RpmCalculator::assignRpmValue(float floatRpmValue) {
	previousRpmValue = rpmValue;

	// Round to the nearest integer RPM - some other parts of the ECU expect integer, so that's what we hand out.
	// TODO: RPM should eventually switch to floating point across the ECU
	rpmValue = efiRound(floatRpmValue, 1);

	if (rpmValue <= 0) {
		oneDegreeUs = NAN;
		setValidValue(0, 0);	// 0 for current time since RPM sensor never times out
	} else {
		setValidValue(floatRpmValue, 0);	// 0 for current time since RPM sensor never times out

		// here it's really important to have more precise float RPM value, see #796
		oneDegreeUs = getOneDegreeTimeUs(floatRpmValue);
		if (previousRpmValue == 0) {
			/**
			 * this would make sure that we have good numbers for first cranking revolution
			 * #275 cranking could be improved
			 */
			ENGINE(periodicFastCallback());
		}
	}
}

void RpmCalculator::setRpmValue(float value) {
	assignRpmValue(value);
	spinning_state_e oldState = state;
	// Change state
	if (rpmValue == 0) {
		state = STOPPED;
	} else if (rpmValue >= CONFIG(cranking.rpm)) {
		if (state != RUNNING) {
			// Store the time the engine started
			engineStartTimer.reset();
		}

		state = RUNNING;
	} else if (state == STOPPED || state == SPINNING_UP) {
		/**
		 * We are here if RPM is above zero but we have not seen running RPM yet.
		 * This gives us cranking hysteresis - a drop of RPM during running is still running, not cranking.
		 */
		state = CRANKING;
	}
#if EFI_ENGINE_CONTROL
	// This presumably fixes injection mode change for cranking-to-running transition.
	// 'isSimultanious' flag should be updated for events if injection modes differ for cranking and running.
	if (state != oldState && CONFIG(crankingInjectionMode) != CONFIG(injectionMode)) {
		// Reset the state of all injectors: when we change fueling modes, we could
		// immediately reschedule an injection that's currently underway.  That will cause
		// the injector's overlappingCounter to get out of sync with reality.  As the fix,
		// every injector's state is forcibly reset just before we could cause that to happen.
		engine->injectionEvents.resetOverlapping();

		// reschedule all injection events now that we've reset them
		engine->injectionEvents.addFuelEvents();
	}
#endif
}

spinning_state_e RpmCalculator::getState() const {
	return state;
}

void RpmCalculator::onNewEngineCycle() {
	revolutionCounterSinceBoot++;
	revolutionCounterSinceStart++;
}

uint32_t RpmCalculator::getRevolutionCounterM(void) const {
	return revolutionCounterSinceBoot;
}

void RpmCalculator::setStopped() {
	revolutionCounterSinceStart = 0;

	rpmRate = 0;

	if (rpmValue != 0) {
		assignRpmValue(0);
		// needed by 'useNoiselessTriggerDecoder'
		engine->triggerCentral.noiseFilter.resetAccumSignalData();
		efiPrintf("engine stopped");
	}
	state = STOPPED;
}

void RpmCalculator::setStopSpinning() {
	isSpinning = false;
	setStopped();
}

void RpmCalculator::setSpinningUp(efitick_t nowNt) {
	if (!CONFIG(isFasterEngineSpinUpEnabled))
		return;
	// Only a completely stopped and non-spinning engine can enter the spinning-up state.
	if (isStopped() && !isSpinning) {
		state = SPINNING_UP;
		engine->triggerCentral.triggerState.spinningEventIndex = 0;
		isSpinning = true;
	}
	// update variables needed by early instant RPM calc.
	if (isSpinningUp()) {
		engine->triggerCentral.triggerState.setLastEventTimeForInstantRpm(nowNt);
	}
	/**
	 * Update ignition pin indices if needed. Here we potentially switch to wasted spark temporarily.
	 */
	prepareIgnitionPinIndices(getCurrentIgnitionMode());
}

/**
 * @brief Shaft position callback used by RPM calculation logic.
 *
 * This callback should always be the first of trigger callbacks because other callbacks depend of values
 * updated here.
 * This callback is invoked on interrupt thread.
 */
void rpmShaftPositionCallback(trigger_event_e ckpSignalType,
		uint32_t index, efitick_t nowNt) {

	RpmCalculator *rpmState = &engine->rpmCalculator;

	if (index == 0) {
		bool hadRpmRecently = rpmState->checkIfSpinning(nowNt);

		float periodSeconds = engine->rpmCalculator.lastTdcTimer.getElapsedSecondsAndReset(nowNt);

		if (hadRpmRecently) {
		/**
		 * Four stroke cycle is two crankshaft revolutions
		 *
		 * We always do '* 2' because the event signal is already adjusted to 'per engine cycle'
		 * and each revolution of crankshaft consists of two engine cycles revolutions
		 *
		 */
			if (periodSeconds == 0) {
				rpmState->setRpmValue(NOISY_RPM);
				rpmState->rpmRate = 0;
			} else {
				int mult = (int)getEngineCycle(engine->getOperationMode()) / 360;
				float rpm = 60 * mult / periodSeconds;

				auto rpmDelta = rpm - rpmState->previousRpmValue;
				rpmState->rpmRate = rpmDelta / (mult * periodSeconds);

				rpmState->setRpmValue(rpm > UNREALISTIC_RPM ? NOISY_RPM : rpm);
			}
		} else {
			// we are here only once trigger is synchronized for the first time
			// while transitioning  from 'spinning' to 'running'
			engine->triggerCentral.triggerState.movePreSynchTimestamps();
		}

		rpmState->onNewEngineCycle();
	}

#if EFI_SENSOR_CHART
	// this 'index==0' case is here so that it happens after cycle callback so
	// it goes into sniffer report into the first position
	if (ENGINE(sensorChartMode) == SC_TRIGGER) {
		angle_t crankAngle = getCrankshaftAngleNt(nowNt);
		int signal = 1000 * ckpSignalType + index;
		scAddData(crankAngle, signal);
	}
#endif /* EFI_SENSOR_CHART */

	// Always update instant RPM even when not spinning up
	engine->triggerCentral.triggerState.updateInstantRpm(
		engine->triggerCentral.triggerShape, &engine->triggerCentral.triggerFormDetails,
		index, nowNt);

	if (rpmState->isSpinningUp()) {
		float instantRpm = engine->triggerCentral.triggerState.getInstantRpm();

		rpmState->assignRpmValue(instantRpm);
#if 0
		efiPrintf("** RPM: idx=%d sig=%d iRPM=%d", index, ckpSignalType, instantRpm);
#endif
	}
}

float RpmCalculator::getTimeSinceEngineStart(efitick_t nowNt) const {
	return engineStartTimer.getElapsedSeconds(nowNt);
}

static char rpmBuffer[_MAX_FILLER];

/**
 * This callback has nothing to do with actual engine control, it just sends a Top Dead Center mark to the rusEfi console
 * digital sniffer.
 */
static void onTdcCallback(void *) {
#if EFI_UNIT_TEST
	if (!engine->needTdcCallback) {
		return;
	}
#endif /* EFI_UNIT_TEST */

	itoa10(rpmBuffer, GET_RPM());
#if EFI_ENGINE_SNIFFER
	waveChart.startDataCollection();
#endif
	addEngineSnifferEvent(TOP_DEAD_CENTER_MESSAGE, (char* ) rpmBuffer);
#if EFI_TOOTH_LOGGER
	LogTriggerTopDeadCenter(getTimeNowNt());
#endif /* EFI_TOOTH_LOGGER */
}

/**
 * This trigger callback schedules the actual physical TDC callback in relation to trigger synchronization point.
 */
void tdcMarkCallback(
		uint32_t index0, efitick_t edgeTimestamp) {
	bool isTriggerSynchronizationPoint = index0 == 0;
	if (isTriggerSynchronizationPoint && ENGINE(isEngineChartEnabled) && ENGINE(tdcMarkEnabled)) {
		// two instances of scheduling_s are needed to properly handle event overlap
		int revIndex2 = getRevolutionCounter() % 2;
		int rpm = GET_RPM();
		// todo: use tooth event-based scheduling, not just time-based scheduling
		if (isValidRpm(rpm)) {
			angle_t tdcPosition = tdcPosition();
			// we need a positive angle offset here
			fixAngle(tdcPosition, "tdcPosition", CUSTOM_ERR_6553);
			scheduleByAngle(&engine->tdcScheduler[revIndex2], edgeTimestamp, tdcPosition, onTdcCallback);
		}
	}
}


/**
 * @return Current crankshaft angle, 0 to 720 for four-stroke
 */
float getCrankshaftAngleNt(efitick_t timeNt) {
	float timeSinceZeroAngle = engine->rpmCalculator.lastTdcTimer.getElapsedSeconds(timeNt);

	int rpm = GET_RPM();

	float oneDegreeSeconds = (60.0f / 360) / rpm;

	return rpm == 0 ? NAN : timeSinceZeroAngle / oneDegreeSeconds;
}

void initRpmCalculator() {

#if ! HW_CHECK_MODE
	if (hasFirmwareError()) {
		return;
	}
#endif // HW_CHECK_MODE

	// Only register if not configured to read RPM over OBD2
	if (!CONFIG(consumeObdSensors)) {
		ENGINE(rpmCalculator).Register();
	}

}

/**
 * Schedules a callback 'angle' degree of crankshaft from now.
 * The callback would be executed once after the duration of time which
 * it takes the crankshaft to rotate to the specified angle.
 */
efitick_t scheduleByAngle(scheduling_s *timer, efitick_t edgeTimestamp, angle_t angle,
		action_s action) {
	float delayUs = ENGINE(rpmCalculator.oneDegreeUs) * angle;

    // 'delayNt' is below 10 seconds here so we use 32 bit type for performance reasons
	int32_t delayNt = USF2NT(delayUs);
	efitime_t delayedTime = edgeTimestamp + delayNt;

	ENGINE(executor.scheduleByTimestampNt("angle", timer, delayedTime, action));

	return delayedTime;
}

#else
RpmCalculator::RpmCalculator() {

}

#endif /* EFI_SHAFT_POSITION_INPUT */

