/**
 * @file    rpm_calculator.cpp
 * @brief   RPM calculator
 *
 * Here we listen to position sensor events in order to figure our if engine is currently running or not.
 * Actual getRpm() is calculated once per crankshaft revolution, based on the amount of time passed
 * since the start of previous shaft revolution.
 *
 * @date Jan 1, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"

#include "rpm_calculator.h"

#if EFI_WAVE_CHART
#include "wave_chart.h"
extern WaveChart waveChart;
#endif /* EFI_WAVE_CHART */

#if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)

#include "trigger_central.h"
#include "engine_configuration.h"
#include "ec2.h"
#include "engine_math.h"
#if EFI_PROD_CODE
#include "rfiutil.h"
#include "engine.h"
#endif

#if EFI_ANALOG_CHART
#include "analog_chart.h"
#endif /* EFI_PROD_CODE */

#define UNREALISTIC_RPM 30000

#define TOP_DEAD_CENTER_MESSAGE "r"

/**
 * @return -1 in case of isNoisySignal(), current RPM otherwise
 */
int getRpmE(Engine *engine) {
	efiAssert(engine->rpmCalculator!=NULL, "rpmCalculator not assigned", -1);
	return engine->rpmCalculator->rpm();
}

extern engine_configuration_s *engineConfiguration;
extern engine_configuration2_s *engineConfiguration2;

#if EFI_PROD_CODE || EFI_SIMULATOR
static Logging logger;
extern Engine engine;
#endif

RpmCalculator::RpmCalculator() {
	rpmValue = 0;

	// we need this initial to have not_running at first invocation
	lastRpmEventTimeUs = (uint64_t) -10 * US_PER_SECOND;
}

/**
 * @return true if there was a full shaft revolution within the last second
 */
bool RpmCalculator::isRunning(void) {
	uint64_t nowUs = getTimeNowUs();
	return nowUs - lastRpmEventTimeUs < US_PER_SECOND;
}

// todo: migrate to float return result or add a float verion? this would have with calculations
// todo: add a version which does not check time & saves time? need to profile
int RpmCalculator::rpm(void) {
	if (!isRunning()) {
		return 0;
	}
	return rpmValue;
}

bool isValidRpm(int rpm) {
	return rpm > 0 && rpm < UNREALISTIC_RPM;
}

#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
bool isCranking(void) {
	int rpm = getRpm();
	return isCrankingR(rpm);
}
#endif

/**
 * @brief Shaft position callback used by RPM calculation logic.
 *
 * This callback should always be the first of trigger callbacks because other callbacks depend of values
 * updated here.
 * This callback is invoked on interrupt thread.
 */
void rpmShaftPositionCallback(trigger_event_e ckpSignalType, uint32_t index, RpmCalculator *rpmState) {

	if (index != 0) {
#if EFI_ANALOG_CHART || defined(__DOXYGEN__)
		if (engineConfiguration->analogChartMode == AC_TRIGGER)
			acAddData(getCrankshaftAngle(getTimeNowUs()), 1000 * ckpSignalType + index);
#endif
		return;
	}
	rpmState->revolutionCounter++;

	uint64_t nowUs = getTimeNowUs();

	bool hadRpmRecently = rpmState->isRunning();

	if (hadRpmRecently) {
		uint64_t diff = nowUs - rpmState->lastRpmEventTimeUs;
		/**
		 * Four stroke cycle is two crankshaft revolutions
		 *
		 * We always do '* 2' because the event signal is already adjusted to 'per engine cycle'
		 * and each revolution of crankshaft consists of two engine cycles revolutions
		 *
		 */
		if (diff == 0) {
			rpmState->rpmValue = NOISY_RPM;
		} else {
			int rpm = (int) (60 * US_PER_SECOND * 2 / diff);
			rpmState->rpmValue = rpm > UNREALISTIC_RPM ? NOISY_RPM : rpm;
		}
	}
	rpmState->lastRpmEventTimeUs = nowUs;
#if EFI_ANALOG_CHART || defined(__DOXYGEN__)
	if (engineConfiguration->analogChartMode == AC_TRIGGER)
		acAddData(getCrankshaftAngle(nowUs), index);
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
	addWaveChartEvent(TOP_DEAD_CENTER_MESSAGE, (char*) rpmBuffer, "");
}

/**
 * This trigger callback schedules the actual physical TDC callback in relation to trigger synchronization point.
 */
static void tdcMarkCallback(trigger_event_e ckpSignalType, uint32_t index0, void *arg) {
	(void)ckpSignalType;
	bool isTriggerSynchronizationPoint = index0 == 0;
	if (isTriggerSynchronizationPoint) {
		int revIndex2 = getRevolutionCounter() % 2;
		// todo: use event-based scheduling, not just time-based scheduling
		scheduleByAngle(&tdcScheduler[revIndex2], engineConfiguration->globalTriggerAngleOffset,
				(schfunc_t) onTdcCallback, NULL);
	}
}
#endif

static RpmCalculator rpmState;

uint64_t getLastRpmEventTime(void) {
	return rpmState.lastRpmEventTimeUs;
}

int getRevolutionCounter(void) {
	return rpmState.revolutionCounter;
}

/**
 * @return Current crankshaft angle, 0 to 720 for four-stroke
 */
float getCrankshaftAngle(uint64_t timeUs) {
	uint64_t timeSinceZeroAngleUs = timeUs - rpmState.lastRpmEventTimeUs;

	/**
	 * even if we use 'getOneDegreeTimeUs' macros here, it looks like the
	 * compiler is not smart enough to figure out that "A / ( B / C)" could be optimized into
	 * "A * C / B" in order to replace a slower division with a faster multiplication.
	 */
	return timeSinceZeroAngleUs / getOneDegreeTimeUs(rpmState.rpm());
}

void initRpmCalculator(void) {
#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
	initLogging(&logger, "rpm calc");
	engine.rpmCalculator = &rpmState;

	tdcScheduler[0].name = "tdc0";
	tdcScheduler[1].name = "tdc1";
	addTriggerEventListener(&tdcMarkCallback, "chart TDC mark", NULL);
#endif

	addTriggerEventListener((ShaftPositionListener) &rpmShaftPositionCallback, "rpm reporter", &rpmState);
}

#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
/**
 * Schedules a callback 'angle' degree of crankshaft from now.
 * The callback would be executed once after the duration of time which
 * it takes the crankshaft to rotate to the specified angle.
 */
void scheduleByAngle(scheduling_s *timer, float angle, schfunc_t callback, void *param) {
	int rpm = getRpm();
	if (!isValidRpm(rpm)) {
		// this might happen in case of a single trigger event after a pause
		warning(OBD_PCM_Processor_Fault, "Invalid rpm: %d", rpm);
		return;
	}
	float delayMs = getOneDegreeTimeMs(rpm) * angle;
	if (cisnan(delayMs)) {
		firmwareError("NaN delay?");
		return;
	}
	scheduleTask("by angle", timer, (int) MS2US(delayMs), callback, param);
}
#endif

#endif /* EFI_SHAFT_POSITION_INPUT */

void addWaveChartEvent(const char *name, const char * msg, const char *msg2) {
#if EFI_WAVE_CHART
	waveChart.addWaveChartEvent3(name, msg, msg2);
#endif /* EFI_WAVE_CHART */
}
