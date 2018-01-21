/**
 * @file    rpm_calculator.cpp
 * @brief   RPM calculator
 *
 * Here we listen to position sensor events in order to figure our if engine is currently running or not.
 * Actual getRpm() is calculated once per crankshaft revolution, based on the amount of time passed
 * since the start of previous shaft revolution.
 *
 * @date Jan 1, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "main.h"
#include "rpm_calculator.h"

#if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)

#include "trigger_central.h"
#include "engine_configuration.h"
#include "engine_math.h"

#if EFI_PROD_CODE
#include "rfiutil.h"
#include "engine.h"
#endif

#if EFI_SENSOR_CHART || defined(__DOXYGEN__)
#include "sensor_chart.h"
#endif

#include "efilib2.h"

#if EFI_ENGINE_SNIFFER
#include "engine_sniffer.h"
extern WaveChart waveChart;
#endif /* EFI_ENGINE_SNIFFER */

EXTERN_ENGINE
;

extern bool hasFirmwareErrorFlag;

static Logging * logger;

int revolutionCounterSinceBootForUnitTest = 0;

RpmCalculator::RpmCalculator() {
#if !EFI_PROD_CODE
	mockRpm = MOCK_UNDEFINED;
#endif /* EFI_PROD_CODE */
	rpmValue = 0;
	assignRpmValue(0);
	state = STOPPED;

	// we need this initial to have not_running at first invocation
	lastRpmEventTimeNt = (efitime_t) -10 * US2NT(US_PER_SECOND_LL);
	revolutionCounterSinceStart = 0;
	revolutionCounterSinceBootForUnitTest = revolutionCounterSinceBoot = 0;

	lastRpmEventTimeNt = 0;
	oneDegreeUs = NAN;
}

bool RpmCalculator::isStopped(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return state == STOPPED;
}

bool RpmCalculator::isCranking(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return state == CRANKING;
}

/**
 * @return true if there was a full shaft revolution within the last second
 */
bool RpmCalculator::isRunning(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return state == RUNNING;
}

bool RpmCalculator::checkIfSpinning(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (lastRpmEventTimeNt == 0) {
		// here we assume 64 bit time does not overflow
		// zero value is the default meaning no RPM events since reboot
		return false;
	}
	efitick_t nowNt = getTimeNowNt();
	if (ENGINE(stopEngineRequestTimeNt) != 0) {
		if (nowNt - ENGINE(stopEngineRequestTimeNt)	< 3 * US2NT(US_PER_SECOND_LL)) {
			// 'stopengine' command implementation
			setStopped(PASS_ENGINE_PARAMETER_SIGNATURE);
			return false;
		}
	}

	/**
	 * note that the result of this subtraction could be negative, that would happen if
	 * we have a trigger event between the time we've invoked 'getTimeNow' and here
	 */
	bool noEventsForTooLong = nowNt - lastRpmEventTimeNt >= US2NT(2 * US_PER_SECOND_LL); // Anything below 60 rpm is not running
	if (noEventsForTooLong) {
		setStopped(PASS_ENGINE_PARAMETER_SIGNATURE);
		return false;
	}

	return true;
}

void RpmCalculator::assignRpmValue(int value) {
	previousRpmValue = rpmValue;
	rpmValue = value;
	if (rpmValue <= 0) {
		oneDegreeUs = NAN;
	} else {
		oneDegreeUs = getOneDegreeTimeUs(rpmValue);
	}
}

void RpmCalculator::setRpmValue(int value DECLARE_ENGINE_PARAMETER_SUFFIX) {
	assignRpmValue(value);
	if (previousRpmValue == 0 && rpmValue > 0) {
		/**
		 * this would make sure that we have good numbers for first cranking revolution
		 * #275 cranking could be improved
		 */
		ENGINE(periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE));
	}
	if (rpmValue == 0) {
		state = STOPPED;
	} else if (rpmValue >= CONFIG(cranking.rpm)) {
		state = RUNNING;
	} else if (state == STOPPED) {
		/**
		 * We are here if RPM is above zero but we have not seen running RPM yet.
		 * This gives us cranking hysteresis - a drop of RPM during running is still running, not cranking.
		 */
		state = CRANKING;
	}
}

void RpmCalculator::onNewEngineCycle() {
	revolutionCounterSinceBoot++;
	revolutionCounterSinceStart++;
#if EFI_UNIT_TEST
	revolutionCounterSinceBootForUnitTest = revolutionCounterSinceBoot;
#endif /* EFI_UNIT_TEST */
}

uint32_t RpmCalculator::getRevolutionCounter(void) {
	return revolutionCounterSinceBoot;
}

uint32_t RpmCalculator::getRevolutionCounterSinceStart(void) {
	return revolutionCounterSinceStart;
}

float RpmCalculator::getRpmAcceleration() {
	return 1.0 * previousRpmValue / rpmValue;
}

void RpmCalculator::setStopped(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	revolutionCounterSinceStart = 0;
	if (rpmValue != 0) {
		assignRpmValue(0);
		scheduleMsg(logger, "engine stopped");
	}
	state = STOPPED;
}

/**
 * WARNING: this is a heavy method because 'getRpm()' is relatively heavy
 *
 * @return -1 in case of isNoisySignal(), current RPM otherwise
 */
// todo: migrate to float return result or add a float version? this would have with calculations
// todo: add a version which does not check time & saves time? need to profile
int RpmCalculator::getRpm(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if !EFI_PROD_CODE
	if (mockRpm != MOCK_UNDEFINED) {
		return mockRpm;
	}
#endif /* EFI_PROD_CODE */
	return rpmValue;
}

/**
 * @brief Shaft position callback used by RPM calculation logic.
 *
 * This callback should always be the first of trigger callbacks because other callbacks depend of values
 * updated here.
 * This callback is invoked on interrupt thread.
 */
void rpmShaftPositionCallback(trigger_event_e ckpSignalType,
		uint32_t index DECLARE_ENGINE_PARAMETER_SUFFIX) {
	efitick_t nowNt = getTimeNowNt();
#if EFI_PROD_CODE
	efiAssertVoid(getRemainingStack(chThdGetSelfX()) > 256, "lowstckRCL");
#endif

	if (index == 0) {
		ENGINE(m.beforeRpmCb) = GET_TIMESTAMP();
		RpmCalculator *rpmState = &engine->rpmCalculator;

		bool hadRpmRecently = rpmState->checkIfSpinning(PASS_ENGINE_PARAMETER_SIGNATURE);

		if (hadRpmRecently) {
			efitime_t diffNt = nowNt - rpmState->lastRpmEventTimeNt;
		/**
		 * Four stroke cycle is two crankshaft revolutions
		 *
		 * We always do '* 2' because the event signal is already adjusted to 'per engine cycle'
		 * and each revolution of crankshaft consists of two engine cycles revolutions
		 *
		 */
			if (diffNt == 0) {
				rpmState->setRpmValue(NOISY_RPM PASS_ENGINE_PARAMETER_SUFFIX);
			} else {
				int mult = (int)getEngineCycle(engineConfiguration->operationMode) / 360;
				int rpm = (int) (60 * US2NT(US_PER_SECOND_LL) * mult / diffNt);
				rpmState->setRpmValue(rpm > UNREALISTIC_RPM ? NOISY_RPM : rpm PASS_ENGINE_PARAMETER_SUFFIX);
			}
		}
		rpmState->onNewEngineCycle();
		rpmState->lastRpmEventTimeNt = nowNt;
		ENGINE(m.rpmCbTime) = GET_TIMESTAMP() - ENGINE(m.beforeRpmCb);
	}


#if EFI_SENSOR_CHART || defined(__DOXYGEN__)
	// this 'index==0' case is here so that it happens after cycle callback so
	// it goes into sniffer report into the first position
	if (ENGINE(sensorChartMode) == SC_TRIGGER) {
		angle_t crankAngle = getCrankshaftAngleNt(nowNt PASS_ENGINE_PARAMETER_SUFFIX);
		int signal = 1000 * ckpSignalType + index;
		scAddData(crankAngle, signal);
	}
#endif

}

static scheduling_s tdcScheduler[2];

static char rpmBuffer[_MAX_FILLER];

#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
/**
 * This callback has nothing to do with actual engine control, it just sends a Top Dead Center mark to the dev console
 * digital sniffer.
 */
static void onTdcCallback(void) {
	itoa10(rpmBuffer, getRpmE(engine));
	addEngineSniffferEvent(TOP_DEAD_CENTER_MESSAGE, (char* ) rpmBuffer);
}

/**
 * This trigger callback schedules the actual physical TDC callback in relation to trigger synchronization point.
 */
static void tdcMarkCallback(trigger_event_e ckpSignalType,
		uint32_t index0 DECLARE_ENGINE_PARAMETER_SUFFIX) {
	(void) ckpSignalType;
	bool isTriggerSynchronizationPoint = index0 == 0;
	if (isTriggerSynchronizationPoint && ENGINE(isEngineChartEnabled)) {
		int revIndex2 = engine->rpmCalculator.getRevolutionCounter() % 2;
		int rpm = ENGINE(rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE));
		// todo: use event-based scheduling, not just time-based scheduling
		if (isValidRpm(rpm)) {
			scheduleByAngle(rpm, &tdcScheduler[revIndex2], tdcPosition(),
					(schfunc_t) onTdcCallback, NULL, &engine->rpmCalculator);
		}
	}
}
#endif

#if EFI_PROD_CODE || EFI_SIMULATOR
int getRevolutionCounter() {
	return engine->rpmCalculator.getRevolutionCounter();
}
#endif

/**
 * @return Current crankshaft angle, 0 to 720 for four-stroke
 */
float getCrankshaftAngleNt(efitime_t timeNt DECLARE_ENGINE_PARAMETER_SUFFIX) {
	efitime_t timeSinceZeroAngleNt = timeNt
			- engine->rpmCalculator.lastRpmEventTimeNt;

	/**
	 * even if we use 'getOneDegreeTimeUs' macros here, it looks like the
	 * compiler is not smart enough to figure out that "A / ( B / C)" could be optimized into
	 * "A * C / B" in order to replace a slower division with a faster multiplication.
	 */
	int rpm = engine->rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE);
	return rpm == 0 ? NAN : timeSinceZeroAngleNt / getOneDegreeTimeNt(rpm);
}

void initRpmCalculator(Logging *sharedLogger, Engine *engine) {
	logger = sharedLogger;
	if (hasFirmwareError()) {
		return;
	}
#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)

//	tdcScheduler[0].name = "tdc0";
//	tdcScheduler[1].name = "tdc1";
	addTriggerEventListener(tdcMarkCallback, "chart TDC mark", engine);
#endif

	addTriggerEventListener(rpmShaftPositionCallback, "rpm reporter", engine);
}

#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
/**
 * Schedules a callback 'angle' degree of crankshaft from now.
 * The callback would be executed once after the duration of time which
 * it takes the crankshaft to rotate to the specified angle.
 */
void scheduleByAngle(int rpm, scheduling_s *timer, angle_t angle,
		schfunc_t callback, void *param, RpmCalculator *calc) {
	efiAssertVoid(!cisnan(angle), "NaN angle?");
	efiAssertVoid(isValidRpm(rpm), "RPM check expected");
	float delayUs = calc->oneDegreeUs * angle;
	efiAssertVoid(!cisnan(delayUs), "NaN delay?");
	scheduleTask(timer, (int) delayUs, callback, param);
}
#endif

#else
RpmCalculator::RpmCalculator() {

}

#endif /* EFI_SHAFT_POSITION_INPUT */

