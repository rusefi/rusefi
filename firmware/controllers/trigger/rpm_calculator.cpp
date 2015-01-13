/**
 * @file    rpm_calculator.cpp
 * @brief   RPM calculator
 *
 * Here we listen to position sensor events in order to figure our if engine is currently running or not.
 * Actual getRpm() is calculated once per crankshaft revolution, based on the amount of time passed
 * since the start of previous shaft revolution.
 *
 * @date Jan 1, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "main.h"

#if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)

#include "rpm_calculator.h"
#include "trigger_central.h"
#include "engine_configuration.h"
#include "engine_math.h"

#if EFI_PROD_CODE
#include "rfiutil.h"
#include "engine.h"
#endif

#if ! EFI_UNIT_TEST
#include "analog_chart.h"
#endif

#include "efilib2.h"

#if EFI_WAVE_CHART
#include "wave_chart.h"
extern WaveChart waveChart;
#endif /* EFI_WAVE_CHART */

EXTERN_ENGINE;

static Logging logger;

RpmCalculator::RpmCalculator() {
#if !EFI_PROD_CODE
	mockRpm = MOCK_UNDEFINED;
#endif
	setRpmValue(0);

	// we need this initial to have not_running at first invocation
	lastRpmEventTimeNt = (uint64_t) -10 * US2NT(US_PER_SECOND_LL);
	revolutionCounterSinceStart = 0;
	revolutionCounterSinceBoot = 0;

	lastRpmEventTimeNt = 0;
}

/**
 * @return true if there was a full shaft revolution within the last second
 */
bool RpmCalculator::isRunning(DECLARE_ENGINE_PARAMETER_F) {
	uint64_t nowNt = getTimeNowNt();
	if (engine->stopEngineRequestTimeNt != 0) {
		if (nowNt - lastRpmEventTimeNt < 3 * US2NT(US_PER_SECOND_LL)) {
			return false;
		}
	}

	return nowNt - lastRpmEventTimeNt < US2NT(US_PER_SECOND_LL);
}

void RpmCalculator::setRpmValue(int value) {
	rpmValue = value;
	if (rpmValue <= 0) {
		oneDegreeUs = NAN;
	} else {
		oneDegreeUs = getOneDegreeTimeUs(rpmValue);
	}
}

void RpmCalculator::onNewEngineCycle() {
	revolutionCounterSinceBoot++;
	revolutionCounterSinceStart++;
}

uint32_t RpmCalculator::getRevolutionCounter(void) {
	return revolutionCounterSinceBoot;
}

uint32_t RpmCalculator::getRevolutionCounterSinceStart(void) {
	return revolutionCounterSinceStart;
}

/**
 * WARNING: this is a heavy method because 'getRpm()' is relatively heavy
 *
 * @return -1 in case of isNoisySignal(), current RPM otherwise
 */
// todo: migrate to float return result or add a float verion? this would have with calculations
// todo: add a version which does not check time & saves time? need to profile
int RpmCalculator::rpm(DECLARE_ENGINE_PARAMETER_F) {
#if !EFI_PROD_CODE
	if (mockRpm != MOCK_UNDEFINED)
	return mockRpm;
#endif
	if (!isRunning(PASS_ENGINE_PARAMETER_F)) {
		revolutionCounterSinceStart = 0;
		rpmValue = 0;
	}
	return rpmValue;
}

#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
bool isCrankingE(Engine *engine) {
	int rpm = getRpmE(engine);
	return isCrankingR(rpm);
}

/**
 * WARNING: this is a heavy method because 'getRpm()' is relatively heavy
 */
bool isCranking(void) {
	return isCrankingE(engine);
}
#endif

/**
 * @brief Shaft position callback used by RPM calculation logic.
 *
 * This callback should always be the first of trigger callbacks because other callbacks depend of values
 * updated here.
 * This callback is invoked on interrupt thread.
 */
void rpmShaftPositionCallback(trigger_event_e ckpSignalType, uint32_t index DECLARE_ENGINE_PARAMETER_S) {
	RpmCalculator *rpmState = &engine->rpmCalculator;
	uint64_t nowNt = getTimeNowNt();
#if EFI_PROD_CODE
	efiAssertVoid(getRemainingStack(chThdSelf()) > 256, "lowstck#2z");
#endif

	if (index != 0) {
#if EFI_ANALOG_CHART || defined(__DOXYGEN__)
		if (engineConfiguration->analogChartMode == AC_TRIGGER)
			acAddData(getCrankshaftAngleNt(nowNt PASS_ENGINE_PARAMETER), 1000 * ckpSignalType + index);
#endif
		return;
	}

	bool hadRpmRecently = rpmState->isRunning(PASS_ENGINE_PARAMETER_F);

	if (hadRpmRecently) {
		uint64_t diffNt = nowNt - rpmState->lastRpmEventTimeNt;
		/**
		 * Four stroke cycle is two crankshaft revolutions
		 *
		 * We always do '* 2' because the event signal is already adjusted to 'per engine cycle'
		 * and each revolution of crankshaft consists of two engine cycles revolutions
		 *
		 */
		if (diffNt == 0) {
			rpmState->setRpmValue(NOISY_RPM);
		} else {
			// todo: interesting what is this *2 about? four stroke magic constant?
			int rpm = (int) (60 * US2NT(US_PER_SECOND_LL) * 2 / diffNt);
			rpmState->setRpmValue(rpm > UNREALISTIC_RPM ? NOISY_RPM : rpm);
		}
	}
	rpmState->onNewEngineCycle();
	rpmState->lastRpmEventTimeNt = nowNt;
#if EFI_ANALOG_CHART || defined(__DOXYGEN__)
	if (engineConfiguration->analogChartMode == AC_TRIGGER)
		acAddData(getCrankshaftAngleNt(nowNt PASS_ENGINE_PARAMETER), index);
#endif
}

static scheduling_s tdcScheduler[2];

static char rpmBuffer[10];

#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
/**
 * This callback has nothing to do with actual engine control, it just sends a Top Dead Center mark to the dev console
 * digital sniffer.
 */
static void onTdcCallback(void) {
	itoa10(rpmBuffer, getRpm());
	addWaveChartEvent(TOP_DEAD_CENTER_MESSAGE, (char* ) rpmBuffer);
}

/**
 * This trigger callback schedules the actual physical TDC callback in relation to trigger synchronization point.
 */
static void tdcMarkCallback(trigger_event_e ckpSignalType, uint32_t index0 DECLARE_ENGINE_PARAMETER_S) {
	(void) ckpSignalType;
	bool isTriggerSynchronizationPoint = index0 == 0;
	if (isTriggerSynchronizationPoint) {
		int revIndex2 = engine->rpmCalculator.getRevolutionCounter() % 2;
		int rpm = getRpm();
		// todo: use event-based scheduling, not just time-based scheduling
		scheduleByAngle(rpm, &tdcScheduler[revIndex2], engineConfiguration->globalTriggerAngleOffset,
				(schfunc_t) onTdcCallback, NULL);
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
float getCrankshaftAngleNt(uint64_t timeNt DECLARE_ENGINE_PARAMETER_S) {
	uint64_t timeSinceZeroAngleNt = timeNt - engine->rpmCalculator.lastRpmEventTimeNt;

	/**
	 * even if we use 'getOneDegreeTimeUs' macros here, it looks like the
	 * compiler is not smart enough to figure out that "A / ( B / C)" could be optimized into
	 * "A * C / B" in order to replace a slower division with a faster multiplication.
	 */
	return timeSinceZeroAngleNt / getOneDegreeTimeNt(engine->rpmCalculator.rpm(PASS_ENGINE_PARAMETER_F));
}

void initRpmCalculator(Engine *engine) {
#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
	initLogging(&logger, "rpm calc");

	tdcScheduler[0].name = "tdc0";
	tdcScheduler[1].name = "tdc1";
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
void scheduleByAngle(int rpm, scheduling_s *timer, float angle, schfunc_t callback, void *param) {
	if (!isValidRpm(rpm)) {
		/**
		 * this might happen in case of a single trigger event after a pause - this is normal, so no
		 * warning here
		 */
		return;
	}
	float delayUs = getOneDegreeTimeUs(rpm) * angle;
	if (cisnan(delayUs)) {
		firmwareError("NaN delay?");
		return;
	}
	scheduleTask("by angle", timer, (int) delayUs, callback, param);
}
#endif

#else
RpmCalculator::RpmCalculator() {

}

#endif /* EFI_SHAFT_POSITION_INPUT */

