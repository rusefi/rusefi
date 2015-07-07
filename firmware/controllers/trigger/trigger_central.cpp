/*
 * @file	trigger_central.cpp
 *
 * @date Feb 23, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "main.h"

#if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)

#include "trigger_central.h"
#include "trigger_decoder.h"
#include "main_trigger_callback.h"
#include "engine_configuration.h"
#include "listener_array.h"
#include "data_buffer.h"
#include "histogram.h"
#include "pwm_generator_logic.h"
#include "efilib2.h"
#include "settings.h"
#include "auto_generated_enums.h"

#include "rpm_calculator.h"
#if EFI_PROD_CODE
#include "rfiutil.h"
#include "pin_repository.h"
#endif

#if EFI_WAVE_CHART
#include "wave_chart.h"
WaveChart waveChart;
#endif /* EFI_WAVE_CHART */

static histogram_s triggerCallback;

// we need this initial to have not_running at first invocation
static volatile efitime_t previousShaftEventTimeNt = (efitimems_t) -10 * US2NT(US_PER_SECOND_LL);

TriggerCentral triggerCentral;

static Logging *logger;

efitime_t getCrankEventCounter() {
	return triggerCentral.triggerState.getTotalEventCounter();
}

efitime_t getStartOfRevolutionIndex() {
	return triggerCentral.triggerState.getStartOfRevolutionIndex();
}

void TriggerCentral::addEventListener(ShaftPositionListener listener, const char *name, Engine *engine) {
	print("registerCkpListener: %s\r\n", name);
	triggerListeneres.registerCallback((VoidInt) listener, engine);
}

/**
 * @brief Adds a trigger event listener
 *
 * Trigger event listener would be invoked on each trigger event. For example, for a 60/2 wheel
 * that would be 116 events: 58 SHAFT_PRIMARY_UP and 58 SHAFT_PRIMARY_DOWN events.
 */
void addTriggerEventListener(ShaftPositionListener listener, const char *name, Engine *engine) {
	triggerCentral.addEventListener(listener, name, engine);
}

uint32_t triggerHanlderEntryTime;

#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
EXTERN_ENGINE
;

int triggerReentraint = 0;
int maxTriggerReentraint = 0;
uint32_t triggerDuration;
uint32_t triggerMaxDuration = 0;

extern bool isInsideTriggerHandler;

void hwHandleShaftSignal(trigger_event_e signal) {
	triggerHanlderEntryTime = GET_TIMESTAMP();
	isInsideTriggerHandler = true;
	if (triggerReentraint > maxTriggerReentraint)
		maxTriggerReentraint = triggerReentraint;
	triggerReentraint++;
	efiAssertVoid(getRemainingStack(chThdSelf()) > 128, "lowstck#8");
	triggerCentral.handleShaftSignal(signal PASS_ENGINE_PARAMETER);
	triggerReentraint--;
	triggerDuration = GET_TIMESTAMP() - triggerHanlderEntryTime;
	isInsideTriggerHandler = false;
	if (triggerDuration > triggerMaxDuration)
		triggerMaxDuration = triggerDuration;
}
#endif /* EFI_PROD_CODE */

TriggerCentral::TriggerCentral() {
	nowNt = 0;
	memset(hwEventCounters, 0, sizeof(hwEventCounters));
	clearCallbacks(&triggerListeneres);
}

int TriggerCentral::getHwEventCounter(int index) {
	return hwEventCounters[index];
}

void TriggerCentral::resetCounters() {
	memset(hwEventCounters, 0, sizeof(hwEventCounters));
	triggerState.resetRunningCounters();
}

static char shaft_signal_msg_index[15];

static bool_t isUpEvent[6] = { false, true, false, true, false, true };
static const char *eventId[6] = { CRANK1, CRANK1, CRANK2, CRANK2, CRANK3, CRANK3 };

static ALWAYS_INLINE void reportEventToWaveChart(trigger_event_e ckpSignalType, int index DECLARE_ENGINE_PARAMETER_S) {
	itoa10(&shaft_signal_msg_index[2], index);
	bool_t isUp = isUpEvent[(int) ckpSignalType];
	shaft_signal_msg_index[0] = isUp ? 'u' : 'd';

	addWaveChartEvent(eventId[(int )ckpSignalType], (char* ) shaft_signal_msg_index);
	if (engineConfiguration->useOnlyFrontForTrigger) {
		// let's add the opposite event right away
		shaft_signal_msg_index[0] = isUp ? 'd' : 'u';
		addWaveChartEvent(eventId[(int )ckpSignalType], (char* ) shaft_signal_msg_index);
	}
}

void TriggerCentral::handleShaftSignal(trigger_event_e signal DECLARE_ENGINE_PARAMETER_S) {
	efiAssertVoid(engine!=NULL, "configuration");

	nowNt = getTimeNowNt();

	efiAssertVoid(engine->engineConfiguration!=NULL, "engineConfiguration");
	efiAssertVoid(engine->engineConfiguration2!=NULL, "engineConfiguration2");

	engine->onTriggerEvent(nowNt);

#if EFI_HISTOGRAMS && EFI_PROD_CODE
	int beforeCallback = hal_lld_get_counter_value();
#endif
	int eventIndex = (int) signal;
	efiAssertVoid(eventIndex >= 0 && eventIndex < HW_EVENT_TYPES, "signal type");
	hwEventCounters[eventIndex]++;

	if (nowNt - previousShaftEventTimeNt > US2NT(US_PER_SECOND_LL)) {
		/**
		 * We are here if there is a time gap between now and previous shaft event - that means the engine is not runnig.
		 * That means we have lost synchronization since the engine is not running :)
		 */
		triggerState.shaft_is_synchronized = false;
	}
	previousShaftEventTimeNt = nowNt;

	/**
	 * This invocation changes the state of triggerState
	 */
	triggerState.decodeTriggerEvent(signal, nowNt PASS_ENGINE_PARAMETER);

	if (!triggerState.shaft_is_synchronized) {
		// we should not propagate event if we do not know where we are
		return;
	}

	/**
	 * If we only have a crank position sensor with four stroke, here we are extending crank revolutions with a 360 degree
	 * cycle into a four stroke, 720 degrees cycle.
	 */
	int triggerIndexForListeners;
	if (engineConfiguration->operationMode != FOUR_STROKE_CRANK_SENSOR) {
		// That's easy - trigger cycle matches engine cycle
		triggerIndexForListeners = triggerState.getCurrentIndex();
	} else {
		bool isEven = triggerState.getTotalRevolutionCounter() & 1;

		triggerIndexForListeners = triggerState.getCurrentIndex() + (isEven ? 0 : TRIGGER_SHAPE(size));
	}
	reportEventToWaveChart(signal, triggerIndexForListeners PASS_ENGINE_PARAMETER);

	if (triggerState.current_index >= TRIGGER_SHAPE(size)) {
		warning(OBD_PCM_Processor_Fault, "unexpected eventIndex=%d", triggerState.current_index);
	} else {

		/**
		 * Here we invoke all the listeners - the main engine control logic is inside these listeners
		 */
		for (int i = 0; i < triggerListeneres.currentListenersCount; i++) {
			ShaftPositionListener listener = (ShaftPositionListener) triggerListeneres.callbacks[i];
			(listener)(signal, triggerIndexForListeners PASS_ENGINE_PARAMETER);
		}

	}
#if EFI_HISTOGRAMS && EFI_PROD_CODE
	int afterCallback = hal_lld_get_counter_value();
	int diff = afterCallback - beforeCallback;
	// this counter is only 32 bits so it overflows every minute, let's ignore the value in case of the overflow for simplicity
	if (diff > 0) {
		hsAdd(&triggerCallback, diff);
	}
#endif /* EFI_HISTOGRAMS */
}

void printAllCallbacksHistogram(void) {
#if EFI_PROD_CODE
	printHistogram(logger, &triggerCallback);
#endif
}

EXTERN_ENGINE
;

static void triggerShapeInfo(void) {
#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
	TriggerShape *s = &engine->triggerShape;
	scheduleMsg(logger, "useRise=%s", boolToString(s->useRiseEdge));
	scheduleMsg(logger, "gap from %f to %f", s->syncRatioFrom, s->syncRatioTo);

	for (int i = 0; i < s->getSize(); i++) {
		scheduleMsg(logger, "event %d %f", i, s->eventAngles[i]);
	}
#endif
}

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
#include <stdio.h>
#include <stdlib.h>


/**
 * This is used to generate trigger info which is later used by TODO to generate images for documentation
 */
void printAllTriggers() {

	FILE * fp = fopen ("triggers.txt", "w+");

	for (int triggerId = 1; triggerId < TT_UNUSED; triggerId++) {
		trigger_type_e tt = (trigger_type_e) triggerId;

		printf("Exporting %s\r\n", getTrigger_type_e(tt));

		persistent_config_s pc;
		Engine e(&pc);
		Engine *engine = &e;
		persistent_config_s *config = &pc;
		engine_configuration_s *engineConfiguration = &pc.engineConfiguration;
		board_configuration_s *boardConfiguration = &engineConfiguration->bc;

		engineConfiguration->engineCycle = 720;
		engineConfiguration->trigger.type = tt;
		engineConfiguration->operationMode = FOUR_STROKE_CAM_SENSOR;

		TriggerShape *s = &engine->triggerShape;
		s->initializeTriggerShape(NULL PASS_ENGINE_PARAMETER);

		fprintf(fp, "TRIGGERTYPE %d %d %s\r\n", triggerId, s->getSize(), getTrigger_type_e(tt));
		for (int i = 0; i < s->getSize(); i++) {

			int triggerDefinitionCoordinate = (s->getTriggerShapeSynchPointIndex() + i) % s->getLength();


			fprintf(fp, "event %d %d %f\r\n", i, s->events[triggerDefinitionCoordinate], s->eventAngles[i]);
		}

	}
	fclose(fp);
	printf("All triggers exported\r\n");
}

#endif


#if EFI_PROD_CODE || defined(__DOXYGEN__)
extern PwmConfig triggerSignal;
#endif /* #if EFI_PROD_CODE */

extern uint32_t maxLockTime;
extern uint32_t maxEventQueueTime;
extern uint32_t hipLastExecutionCount;
extern uint32_t hwSetTimerTime;

extern int maxHowFarOff;
extern uint32_t *cyccnt;

void triggerInfo(void) {
#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)

	TriggerShape *ts = &engine->triggerShape;

	scheduleMsg(logger, "Template %s (%d) trigger %s (%d) useRiseEdge=%s onlyFront=%s",
			getConfigurationName(engineConfiguration->engineType), engineConfiguration->engineType,
			getTrigger_type_e(engineConfiguration->trigger.type), engineConfiguration->trigger.type,
			boolToString(TRIGGER_SHAPE(useRiseEdge)), boolToString(engineConfiguration->useOnlyFrontForTrigger));

	if (engineConfiguration->trigger.type == TT_TOOTHED_WHEEL) {
		scheduleMsg(logger, "total %d/skipped %d", engineConfiguration->trigger.customTotalToothCount,
				engineConfiguration->trigger.customSkippedToothCount);
	}

	scheduleMsg(logger, "trigger#1 event counters up=%d/down=%d", triggerCentral.getHwEventCounter(0),
			triggerCentral.getHwEventCounter(1));

	if (engine->triggerShape.needSecondTriggerInput) {
		scheduleMsg(logger, "trigger#2 event counters up=%d/down=%d", triggerCentral.getHwEventCounter(2),
				triggerCentral.getHwEventCounter(3));
	}
	scheduleMsg(logger, "expected cycle events %d/%d/%d", ts->expectedEventCount[0],
			engine->triggerShape.expectedEventCount[1], ts->expectedEventCount[2]);

	scheduleMsg(logger, "trigger type=%d/need2ndChannel=%s", engineConfiguration->trigger.type,
			boolToString(engine->triggerShape.needSecondTriggerInput));
	scheduleMsg(logger, "expected duty #0=%f/#1=%f", ts->dutyCycle[0], ts->dutyCycle[1]);

	scheduleMsg(logger, "synchronizationNeeded=%s/isError=%s/total errors=%d ord_err=%d/total revolutions=%d/self=%s",
			boolToString(ts->isSynchronizationNeeded),
			boolToString(isTriggerDecoderError()), triggerCentral.triggerState.totalTriggerErrorCounter,
			triggerCentral.triggerState.orderingErrorCounter, triggerCentral.triggerState.getTotalRevolutionCounter(),
			boolToString(engineConfiguration->directSelfStimulation));

	if (ts->isSynchronizationNeeded) {
		scheduleMsg(logger, "gap from %f to %f", ts->syncRatioFrom, ts->syncRatioTo);
	}

#endif

#if EFI_PROD_CODE || defined(__DOXYGEN__)

	scheduleMsg(logger, "primary trigger input: %s", hwPortname(boardConfiguration->triggerInputPins[0]));
	scheduleMsg(logger, "primary trigger simulator: %s %s freq=%d",
			hwPortname(boardConfiguration->triggerSimulatorPins[0]),
			getPin_output_mode_e(boardConfiguration->triggerSimulatorPinModes[0]),
			boardConfiguration->triggerSimulatorFrequency);

	if (engine->triggerShape.needSecondTriggerInput) {
		scheduleMsg(logger, "secondary trigger input: %s", hwPortname(boardConfiguration->triggerInputPins[1]));
#if EFI_EMULATE_POSITION_SENSORS || defined(__DOXYGEN__)
		scheduleMsg(logger, "secondary trigger simulator: %s %s phase=%d",
				hwPortname(boardConfiguration->triggerSimulatorPins[1]),
				getPin_output_mode_e(boardConfiguration->triggerSimulatorPinModes[1]), triggerSignal.safe.phaseIndex);
#endif /* EFI_EMULATE_POSITION_SENSORS */
	}
//	scheduleMsg(logger, "3rd trigger simulator: %s %s", hwPortname(boardConfiguration->triggerSimulatorPins[2]),
//			getPin_output_mode_e(boardConfiguration->triggerSimulatorPinModes[2]));

	scheduleMsg(logger, "trigger error extra LED: %s %s", hwPortname(boardConfiguration->triggerErrorPin),
			getPin_output_mode_e(boardConfiguration->triggerErrorPinMode));
	scheduleMsg(logger, "primary logic input: %s", hwPortname(boardConfiguration->logicAnalyzerPins[0]));
	scheduleMsg(logger, "secondary logic input: %s", hwPortname(boardConfiguration->logicAnalyzerPins[1]));

	scheduleMsg(logger, "zeroTestTime=%d maxHowFarOff=%d", engine->m.zeroTestTime, maxHowFarOff);
	maxHowFarOff = 0;

	scheduleMsg(logger, "advanceLookupTime=%d now=%d fuelCalcTime=%d",
			engine->m.advanceLookupTime, *cyccnt,
			engine->m.fuelCalcTime);

	scheduleMsg(logger,
			"ignitionMathTime=%d ignitionSchTime=%d injectonSchTime=%d",
			engine->m.ignitionMathTime, engine->m.ignitionSchTime,
			engine->m.injectonSchTime);

	scheduleMsg(logger, "mapTime=%d/hipTime=%d/rpmTime=%d/mainTriggerCallbackTime=%d",
			engine->m.mapAveragingCbTime,
			engine->m.hipCbTime,
			engine->m.rpmCbTime,
			engine->m.mainTriggerCallbackTime);

	scheduleMsg(logger, "maxLockTime=%d / maxTriggerReentraint=%d", maxLockTime, maxTriggerReentraint);
	maxLockTime = 0;
	scheduleMsg(logger, "maxEventQueueTime=%d", maxEventQueueTime);

	scheduleMsg(logger, "hipLastExecutionCount=%d", hipLastExecutionCount);
	scheduleMsg(logger, "hwSetTimerTime %d", hwSetTimerTime);

	scheduleMsg(logger, "totalTriggerHandlerMaxTime=%d", triggerMaxDuration);
	triggerMaxDuration = 0;

#endif /* EFI_PROD_CODE */
}

float getTriggerDutyCycle(int index) {
	return triggerCentral.triggerState.getTriggerDutyCycle(index);
}

static void resetRunningTriggerCounters() {
	triggerCentral.resetCounters();
#if EFI_PROD_CODE
	triggerInfo();
#endif
}

void initTriggerCentral(Logging *sharedLogger, Engine *engine) {
	logger = sharedLogger;
	strcpy((char*) shaft_signal_msg_index, "x_");

#if EFI_WAVE_CHART
	initWaveChart(&waveChart);
#endif /* EFI_WAVE_CHART */

#if EFI_PROD_CODE || EFI_SIMULATOR
	addConsoleAction("triggerinfo", triggerInfo);
	addConsoleAction("trigger_shape_info", triggerShapeInfo);
	addConsoleAction("reset_trigger", resetRunningTriggerCounters);
#endif

#if EFI_HISTOGRAMS
	initHistogram(&triggerCallback, "all callbacks");
#endif /* EFI_HISTOGRAMS */
}

#endif
