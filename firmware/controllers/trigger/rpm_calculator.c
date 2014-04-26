/**
 * @file    rpm_calculator.c
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
#include "trigger_central.h"
#include "engine_configuration.h"
#include "engine_math.h"
#if EFI_PROD_CODE || EFI_SIMULATOR
#include "analog_chart.h"
#endif /* EFI_PROD_CODE */

#include "wave_chart.h"
#include "rfiutil.h"

static rpm_s rpmState;

#define UNREALISTIC_RPM 30000

#define TOP_DEAD_CENTER_MESSAGE "r"

extern engine_configuration_s *engineConfiguration;
extern engine_configuration2_s *engineConfiguration2;
extern WaveChart waveChart;

static Logging logger;

/**
 * @return true if there was a full shaft revolution within the last second
 */
int isRunning() {
	uint64_t nowUs = getTimeNowUs();
	return nowUs - rpmState.lastRpmEventTimeUs < US_PER_SECOND;
}

uint64_t getLastRpmEventTime(void) {
	return rpmState.lastRpmEventTimeUs;
}

int isCranking(void) {
	int rpm = getRpm();
	return isCrankingR(rpm);
}

/**
 * @return -1 in case of isNoisySignal(), current RPM otherwise
 */
int getRpm() {
	if (!isRunning())
		return 0;
	return rpmState.rpm;
}

/**
 * @return Current crankshaft angle, 0 to 720 for four-stroke
 */
float getCrankshaftAngle(uint64_t timeUs) {
	uint64_t timeSinceZeroAngle = timeUs - rpmState.lastRpmEventTimeUs;

	float cRevolutionTimeMs = getCrankshaftRevolutionTimeMs(rpmState.rpm);

	return 360.0 * timeSinceZeroAngle / cRevolutionTimeMs / 1000;
}

int getRevolutionCounter(void) {
	return rpmState.revolutionCounter;
}

/**
 * Checks for noise on the trigger input line. Noise is detected by an unexpectedly small time gap between
 * current and previous trigger input events.
 *
 * @return TRUE if noise is detected
 */
static int isNoisySignal(rpm_s * rpmState, uint64_t nowUs) {
	uint64_t diff = nowUs - rpmState->lastRpmEventTimeUs;
	return diff < 1000; // that's 1ms
}

static char shaft_signal_msg_index[15];

void addWaveChartEvent(char *name, char * msg, char *msg2) {
	addWaveChartEvent3(&waveChart, name, msg, msg2);
}


/**
 * @brief Shaft position callback used by RPM calculation logic.
 *
 * This callback is invoked on interrupt thread.
 */
static void shaftPositionCallback(ShaftEvents ckpSignalType, int index) {
	itoa10(&shaft_signal_msg_index[1], index);
	if (ckpSignalType == SHAFT_PRIMARY_UP) {
		addWaveChartEvent("crank", "up", shaft_signal_msg_index);
	} else if (ckpSignalType == SHAFT_PRIMARY_DOWN) {
		addWaveChartEvent("crank", "down", shaft_signal_msg_index);
	} else if (ckpSignalType == SHAFT_SECONDARY_UP) {
		addWaveChartEvent("crank2", "up", shaft_signal_msg_index);
	} else if (ckpSignalType == SHAFT_SECONDARY_DOWN) {
		addWaveChartEvent("crank2", "down", shaft_signal_msg_index);
	}


	if (index != 0) {
#if EFI_PROD_CODE || EFI_SIMULATOR
		if (engineConfiguration->analogChartMode == AC_TRIGGER)
			acAddData(getCrankshaftAngle(getTimeNowUs()), 1000 * ckpSignalType + index);
#endif
		return;
	}
	rpmState.revolutionCounter++;

	uint64_t nowUs = getTimeNowUs();

	int hadRpmRecently = isRunning();

	if (hadRpmRecently) {
		if (isNoisySignal(&rpmState, nowUs)) {
			// unexpected state. Noise?
			rpmState.rpm = NOISY_RPM;
		} else {
			uint64_t diff = nowUs - rpmState.lastRpmEventTimeUs;
			// 60000 because per minute
			// * 2 because each revolution of crankshaft consists of two camshaft revolutions
			// / 4 because each cylinder sends a signal
			// need to measure time from the previous non-skipped event

			int rpm = (int)(60 * US_PER_SECOND / engineConfiguration->rpmMultiplier / diff);
			rpmState.rpm = rpm > UNREALISTIC_RPM ? NOISY_RPM : rpm;
		}
	}
	rpmState.lastRpmEventTimeUs = nowUs;
#if EFI_PROD_CODE || EFI_SIMULATOR
	if (engineConfiguration->analogChartMode == AC_TRIGGER)
		acAddData(getCrankshaftAngle(nowUs), index);
#endif
}

static scheduling_s tdcScheduler;

static char rpmBuffer[10];

static void onTdcCallback(void) {
	itoa10(rpmBuffer, getRpm());
	addWaveChartEvent(TOP_DEAD_CENTER_MESSAGE, rpmBuffer, "");
}

static void tdcMarkCallback(ShaftEvents ckpSignalType, int index) {
	if (index == 0) {
		scheduleByAngle(&tdcScheduler, engineConfiguration->globalTriggerAngleOffset, (schfunc_t) onTdcCallback, NULL);
	}
}

void initRpmCalculator(void) {
	initLogging(&logger, "rpm calc");

	strcpy(shaft_signal_msg_index, "_");

	rpmState.rpm = 0;

	// we need this initial to have not_running at first invocation
	rpmState.lastRpmEventTimeUs = (uint64_t)-10 * US_PER_SECOND;

	registerShaftPositionListener(&shaftPositionCallback, "rpm reporter");
	registerShaftPositionListener(&tdcMarkCallback, "chart TDC mark");
}

void scheduleByAngle(scheduling_s *timer, float angle, schfunc_t callback, void *param) {
	float delayMs = getOneDegreeTimeMs(getRpm()) * angle;
	scheduleTask(timer, MS2US(delayMs), callback, param);
}
