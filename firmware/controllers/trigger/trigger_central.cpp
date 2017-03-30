/*
 * @file	trigger_central.cpp
 *
 * @date Feb 23, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
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
#include "engine_math.h"

#include "rpm_calculator.h"
#if EFI_PROD_CODE || defined(__DOXYGEN__)
#include "rfiutil.h"
#include "pin_repository.h"
#include "tunerstudio.h"
extern TunerStudioOutputChannels tsOutputChannels;
#endif /* EFI_PROD_CODE */

#if EFI_ENGINE_SNIFFER || defined(__DOXYGEN__)
#include "engine_sniffer.h"
WaveChart waveChart;
#endif /* EFI_ENGINE_SNIFFER */

EXTERN_ENGINE;

#if EFI_HISTOGRAMS || defined(__DOXYGEN__)
static histogram_s triggerCallbackHistogram;
#endif /* EFI_HISTOGRAMS */

static Logging *logger;

efitime_t getCrankEventCounter(DECLARE_ENGINE_PARAMETER_F) {
	return engine->triggerCentral.triggerState.getTotalEventCounter();
}

efitime_t getStartOfRevolutionIndex(DECLARE_ENGINE_PARAMETER_F) {
	return engine->triggerCentral.triggerState.getStartOfRevolutionIndex();
}

void TriggerCentral::addEventListener(ShaftPositionListener listener, const char *name, Engine *engine) {
	print("registerCkpListener: %s\r\n", name);
	triggerListeneres.registerCallback((VoidInt) listener, engine);
}

/**
 * @brief Adds a trigger event listener
 *
 * Trigger event listener would be invoked on each trigger event. For example, for a 60/2 wheel
 * that would be 116 events: 58 SHAFT_PRIMARY_RISING and 58 SHAFT_PRIMARY_FALLING events.
 */
void addTriggerEventListener(ShaftPositionListener listener, const char *name, Engine *engine) {
	engine->triggerCentral.addEventListener(listener, name, engine);
}

uint32_t triggerHanlderEntryTime;

#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
EXTERN_ENGINE
;

int triggerReentraint = 0;
int maxTriggerReentraint = 0;
uint32_t triggerDuration;
uint32_t triggerMaxDuration = 0;

static bool isInsideTriggerHandler = false;


efitick_t previousVvtCamTime = 0;
efitick_t previousVvtCamDuration = 0;

void hwHandleVvtCamSignal(trigger_value_e front) {
	if (ENGINE(isEngineChartEnabled)) {
		// this is a performance optimization - array index is cheaper then invoking a method with 'switch'
		addEngineSniffferEvent(VVT_NAME, front == TV_RISE ? WC_UP : WC_DOWN);
	}

	if (boardConfiguration->vvtCamSensorUseRise ^ (front != TV_FALL)) {
		return;
	}

	TriggerCentral *tc = &engine->triggerCentral;

	efitick_t nowNt = getTimeNowNt();

	if (engineConfiguration->vvtMode == MIATA_NB2) {
		uint32_t currentDuration = nowNt - previousVvtCamTime;
		float ratio = ((float) currentDuration) / previousVvtCamDuration;


		previousVvtCamDuration = currentDuration;
		previousVvtCamTime = nowNt;

		if (engineConfiguration->isPrintTriggerSynchDetails) {
			scheduleMsg(logger, "vvt ratio %f", ratio);
		}
		if (ratio < boardConfiguration->nb2ratioFrom || ratio > boardConfiguration->nb2ratioTo) {
			return;
		}
		if (engineConfiguration->isPrintTriggerSynchDetails) {
			scheduleMsg(logger, "looks good: vvt ratio %f", ratio);
		}
	}


	efitick_t offsetNt = nowNt - tc->timeAtVirtualZeroNt;

	angle_t vvtPosition = NT2US(offsetNt) / engine->rpmCalculator.oneDegreeUs;

	// convert engine cycle angle into trigger cycle angle
	vvtPosition -= tdcPosition();
	fixAngle(vvtPosition, "vvtPosition");

	tc->vvtPosition = (engineConfiguration->vvtDisplayInverted ? -vvtPosition : vvtPosition) + engineConfiguration->vvtOffset;

	if (engineConfiguration->vvtMode == VVT_FIRST_HALF) {
		bool isEven = tc->triggerState.isEvenRevolution();
		if (!isEven) {
			/**
			 * we are here if we've detected the cam sensor within the wrong crank phase
			 * let's increase the trigger event counter, that would adjust the state of
			 * virtual crank-based trigger
			 */
			tc->triggerState.intTotalEventCounter();
#if EFI_PROD_CODE || defined(__DOXYGEN__)
			if (engineConfiguration->debugMode == DBG_VVT) {
				tsOutputChannels.debugIntField1++;
			}
#endif /* EFI_PROD_CODE */
		}
	} else if (engineConfiguration->vvtMode == VVT_SECOND_HALF) {
		bool isEven = tc->triggerState.isEvenRevolution();
		if (isEven) {
			// see above comment
#if EFI_PROD_CODE || defined(__DOXYGEN__)
			tc->triggerState.intTotalEventCounter();
			if (engineConfiguration->debugMode == DBG_VVT) {
				tsOutputChannels.debugIntField1++;
			}
#endif /* EFI_PROD_CODE */
		}

	} else if (engineConfiguration->vvtMode == MIATA_NB2) {
		/**
		 * NB2 is a symmetrical crank, there are four phases total
		 */
		while (tc->triggerState.getTotalRevolutionCounter() % 4 != engineConfiguration->nbVvtIndex) {
			tc->triggerState.intTotalEventCounter();
		}
	}

}

void hwHandleShaftSignal(trigger_event_e signal) {
	triggerHanlderEntryTime = GET_TIMESTAMP();
	isInsideTriggerHandler = true;
	if (triggerReentraint > maxTriggerReentraint)
		maxTriggerReentraint = triggerReentraint;
	triggerReentraint++;
	efiAssertVoid(getRemainingStack(chThdGetSelfX()) > 128, "lowstck#8");
	engine->triggerCentral.handleShaftSignal(signal PASS_ENGINE_PARAMETER);
	triggerReentraint--;
	triggerDuration = GET_TIMESTAMP() - triggerHanlderEntryTime;
	isInsideTriggerHandler = false;
	if (triggerDuration > triggerMaxDuration)
		triggerMaxDuration = triggerDuration;
}
#endif /* EFI_PROD_CODE */

TriggerCentral::TriggerCentral() {
	nowNt = 0;
	vvtPosition = 0;
	timeAtVirtualZeroNt = 0;
	// we need this initial to have not_running at first invocation
	previousShaftEventTimeNt = (efitimems_t) -10 * US2NT(US_PER_SECOND_LL);

	memset(hwEventCounters, 0, sizeof(hwEventCounters));
	clearCallbacks(&triggerListeneres);
	triggerState.reset();
}

int TriggerCentral::getHwEventCounter(int index) {
	return hwEventCounters[index];
}

void TriggerCentral::resetCounters() {
	memset(hwEventCounters, 0, sizeof(hwEventCounters));
	triggerState.resetRunningCounters();
}

static char shaft_signal_msg_index[15];

static bool isUpEvent[6] = { false, true, false, true, false, true };
static const char *eventId[6] = { CRANK1, CRANK1, CRANK2, CRANK2, CRANK3, CRANK3 };

static ALWAYS_INLINE void reportEventToWaveChart(trigger_event_e ckpSignalType, int index DECLARE_ENGINE_PARAMETER_S) {
	itoa10(&shaft_signal_msg_index[2], index);
	bool isUp = isUpEvent[(int) ckpSignalType];
	shaft_signal_msg_index[0] = isUp ? 'u' : 'd';

	addEngineSniffferEvent(eventId[(int )ckpSignalType], (char* ) shaft_signal_msg_index);
	if (engineConfiguration->useOnlyRisingEdgeForTrigger) {
		// let's add the opposite event right away
		shaft_signal_msg_index[0] = isUp ? 'd' : 'u';
		addEngineSniffferEvent(eventId[(int )ckpSignalType], (char* ) shaft_signal_msg_index);
	}
}

void TriggerCentral::handleShaftSignal(trigger_event_e signal DECLARE_ENGINE_PARAMETER_S) {
	efiAssertVoid(engine!=NULL, "configuration");

	if (triggerShape.shapeDefinitionError) {
		// trigger is broken, we cannot do anything here
		warning(CUSTOM_ERR_UNEXPECTED_SHAFT_EVENT, "Shaft event while trigger is mis-configured");
		return;
	}

	nowNt = getTimeNowNt();

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

	/**
	 * If we only have a crank position sensor with four stroke, here we are extending crank revolutions with a 360 degree
	 * cycle into a four stroke, 720 degrees cycle.
	 */
	int triggerIndexForListeners;
	if (engineConfiguration->operationMode == FOUR_STROKE_CAM_SENSOR ||
			engineConfiguration->operationMode == TWO_STROKE) {
		// That's easy - trigger cycle matches engine cycle
		triggerIndexForListeners = triggerState.getCurrentIndex();
	} else {
		int crankDivider = engineConfiguration->operationMode == FOUR_STROKE_CRANK_SENSOR ? 2 : 4;

		int crankInternalIndex = triggerState.getTotalRevolutionCounter() % crankDivider;

		triggerIndexForListeners = triggerState.getCurrentIndex() + (crankInternalIndex * TRIGGER_SHAPE(size));
	}
	if (triggerIndexForListeners == 0) {
		timeAtVirtualZeroNt = nowNt;
	}
	reportEventToWaveChart(signal, triggerIndexForListeners PASS_ENGINE_PARAMETER);

	if (!triggerState.shaft_is_synchronized) {
		// we should not propagate event if we do not know where we are
		return;
	}

	if (triggerState.isValidIndex(PASS_ENGINE_PARAMETER_F)) {
#if TRIGGER_EXTREME_LOGGING || defined(__DOXYGEN__)
	scheduleMsg(logger, "trigger %d %d %d", triggerIndexForListeners, getRevolutionCounter(), (int)getTimeNowUs());
#endif /* FUEL_MATH_EXTREME_LOGGING */

		/**
		 * Here we invoke all the listeners - the main engine control logic is inside these listeners
		 */
		for (int i = 0; i < triggerListeneres.currentListenersCount; i++) {
			ShaftPositionListener listener = (ShaftPositionListener) triggerListeneres.callbacks[i];
			(listener)(signal, triggerIndexForListeners PASS_ENGINE_PARAMETER);
		}

	}
#if EFI_HISTOGRAMS || defined(__DOXYGEN__)
	int afterCallback = hal_lld_get_counter_value();
	int diff = afterCallback - beforeCallback;
	// this counter is only 32 bits so it overflows every minute, let's ignore the value in case of the overflow for simplicity
	if (diff > 0) {
		hsAdd(&triggerCallbackHistogram, diff);
	}
#endif /* EFI_HISTOGRAMS */
}

void printAllCallbacksHistogram(void) {
#if EFI_HISTOGRAMS || defined(__DOXYGEN__)
	printHistogram(logger, &triggerCallbackHistogram);
#endif
}

EXTERN_ENGINE
;

static void triggerShapeInfo(void) {
#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
	TriggerShape *s = &engine->triggerCentral.triggerShape;
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

#define TRIGGERS_FILE_NAME "triggers.txt"

/**
 * This is used to generate trigger info which is later used by TriggerImage java class
 * to generate images for documentation
 */
extern bool printTriggerDebug;
void printAllTriggers() {

	FILE * fp = fopen (TRIGGERS_FILE_NAME, "w+");

	fprintf(fp, "# Generated by rusEfi\r\n");
	printTriggerDebug = true;
	for (int triggerId = 1; triggerId < TT_UNUSED; triggerId++) {
		trigger_type_e tt = (trigger_type_e) triggerId;

//		if (triggerId != 20)
//			continue;

		printf("Exporting %s\r\n", getTrigger_type_e(tt));

		persistent_config_s pc;
		Engine e(&pc);
		Engine *engine = &e;
		persistent_config_s *config = &pc;
		engine_configuration_s *engineConfiguration = &pc.engineConfiguration;
		board_configuration_s *boardConfiguration = &engineConfiguration->bc;

		engineConfiguration->trigger.type = tt;
		engineConfiguration->operationMode = FOUR_STROKE_CAM_SENSOR;

		TriggerShape *s = &engine->triggerCentral.triggerShape;
		s->initializeTriggerShape(NULL PASS_ENGINE_PARAMETER);

		fprintf(fp, "TRIGGERTYPE %d %d %s %f\r\n", triggerId, s->getLength(), getTrigger_type_e(tt), s->tdcPosition);

		fprintf(fp, "# duty %f %f\r\n", s->dutyCycle[0], s->dutyCycle[1]);

		for (int i = 0; i < s->getLength(); i++) {

			int triggerDefinitionCoordinate = (s->getTriggerShapeSynchPointIndex() + i) % s->getSize();


			fprintf(fp, "event %d %d %f\r\n", i, s->triggerSignals[triggerDefinitionCoordinate], s->eventAngles[i]);
		}

	}
	fclose(fp);
	printf("All triggers exported to %s\r\n", TRIGGERS_FILE_NAME);
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

extern int vvtEventRiseCounter;
extern int vvtEventFallCounter;

void triggerInfo(void) {
#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)

	TriggerShape *ts = &engine->triggerCentral.triggerShape;

	scheduleMsg(logger, "Template %s (%d) trigger %s (%d) useRiseEdge=%s onlyFront=%s useOnlyFirstChannel=%s tdcOffset=%d",
			getConfigurationName(engineConfiguration->engineType), engineConfiguration->engineType,
			getTrigger_type_e(engineConfiguration->trigger.type), engineConfiguration->trigger.type,
			boolToString(TRIGGER_SHAPE(useRiseEdge)), boolToString(engineConfiguration->useOnlyRisingEdgeForTrigger),
			boolToString(engineConfiguration->trigger.useOnlyFirstChannel), TRIGGER_SHAPE(tdcPosition));

	if (engineConfiguration->trigger.type == TT_TOOTHED_WHEEL) {
		scheduleMsg(logger, "total %d/skipped %d", engineConfiguration->trigger.customTotalToothCount,
				engineConfiguration->trigger.customSkippedToothCount);
	}

	scheduleMsg(logger, "trigger#1 event counters up=%d/down=%d", engine->triggerCentral.getHwEventCounter(0),
			engine->triggerCentral.getHwEventCounter(1));

	if (ts->needSecondTriggerInput) {
		scheduleMsg(logger, "trigger#2 event counters up=%d/down=%d", engine->triggerCentral.getHwEventCounter(2),
				engine->triggerCentral.getHwEventCounter(3));
	}
	scheduleMsg(logger, "expected cycle events %d/%d/%d", TRIGGER_SHAPE(expectedEventCount[0]),
			TRIGGER_SHAPE(expectedEventCount[1]), TRIGGER_SHAPE(expectedEventCount[2]));

	scheduleMsg(logger, "trigger type=%d/need2ndChannel=%s", engineConfiguration->trigger.type,
			boolToString(TRIGGER_SHAPE(needSecondTriggerInput)));
	scheduleMsg(logger, "expected duty #0=%f/#1=%f", TRIGGER_SHAPE(dutyCycle[0]), TRIGGER_SHAPE(dutyCycle[1]));

	scheduleMsg(logger, "synchronizationNeeded=%s/isError=%s/total errors=%d ord_err=%d/total revolutions=%d/self=%s",
			boolToString(ts->isSynchronizationNeeded),
			boolToString(isTriggerDecoderError()), engine->triggerCentral.triggerState.totalTriggerErrorCounter,
			engine->triggerCentral.triggerState.orderingErrorCounter, engine->triggerCentral.triggerState.getTotalRevolutionCounter(),
			boolToString(engineConfiguration->directSelfStimulation));

	if (TRIGGER_SHAPE(isSynchronizationNeeded)) {
		scheduleMsg(logger, "gap from %f to %f", ts->syncRatioFrom, ts->syncRatioTo);
	}

#endif

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	if (engineConfiguration->camInput != GPIO_UNASSIGNED) {
		scheduleMsg(logger, "VVT input: %s mode %d", hwPortname(engineConfiguration->camInput),
				engineConfiguration->vvtMode);
		scheduleMsg(logger, "VVT event counters: %d/%d", vvtEventRiseCounter, vvtEventFallCounter);

	}

	scheduleMsg(logger, "primary trigger input: %s", hwPortname(boardConfiguration->triggerInputPins[0]));
	scheduleMsg(logger, "primary trigger simulator: %s %s freq=%d",
			hwPortname(boardConfiguration->triggerSimulatorPins[0]),
			getPin_output_mode_e(boardConfiguration->triggerSimulatorPinModes[0]),
			boardConfiguration->triggerSimulatorFrequency);

	if (ts->needSecondTriggerInput) {
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
			"ignitionSchTime=%d injectonSchTime=%d",
			engine->m.ignitionSchTime,
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

#if ! EFI_UNIT_TEST
float getTriggerDutyCycle(int index) {
	return engine->triggerCentral.triggerState.getTriggerDutyCycle(index);
}
#endif

static void resetRunningTriggerCounters() {
#if !EFI_UNIT_TEST || defined(__DOXYGEN__)
	engine->triggerCentral.resetCounters();
	triggerInfo();
#endif
}

void initTriggerCentral(Logging *sharedLogger, Engine *engine) {
	logger = sharedLogger;
	strcpy((char*) shaft_signal_msg_index, "x_");

#if EFI_ENGINE_SNIFFER || defined(__DOXYGEN__)
	initWaveChart(&waveChart);
#endif /* EFI_ENGINE_SNIFFER */

#if EFI_PROD_CODE || EFI_SIMULATOR || defined(__DOXYGEN__)
	addConsoleAction("triggerinfo", triggerInfo);
	addConsoleAction("trigger_shape_info", triggerShapeInfo);
	addConsoleAction("reset_trigger", resetRunningTriggerCounters);
#endif

#if EFI_HISTOGRAMS || defined(__DOXYGEN__)
	initHistogram(&triggerCallbackHistogram, "all callbacks");
#endif /* EFI_HISTOGRAMS */
}

#endif
