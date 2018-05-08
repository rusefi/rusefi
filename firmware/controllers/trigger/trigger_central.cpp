/*
 * @file	trigger_central.cpp
 * Here we have a bunch of higher-level methods which are not directly related to actual signal decoding
 *
 * @date Feb 23, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
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
#include "LocalVersionHolder.h"
#include "trigger_simulator.h"

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

/**
 * true if a recent configuration change has changed any of the trigger settings which
 * we have not adjusted for yet
 */
static bool isTriggerConfigChanged = false;

#if EFI_HISTOGRAMS || defined(__DOXYGEN__)
static histogram_s triggerCallbackHistogram;
#endif /* EFI_HISTOGRAMS */

static Logging *logger;
static LocalVersionHolder triggerVersion;

efitime_t getCrankEventCounter(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return engine->triggerCentral.triggerState.getTotalEventCounter();
}

efitime_t getStartOfRevolutionIndex(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
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

#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)

int triggerReentraint = 0;
int maxTriggerReentraint = 0;
uint32_t triggerDuration;
uint32_t triggerMaxDuration = 0;

static bool isInsideTriggerHandler = false;

static efitick_t previousVvtCamTime = 0;
static efitick_t previousVvtCamDuration = 0;

void hwHandleVvtCamSignal(trigger_value_e front) {
	if (ENGINE(isEngineChartEnabled)) {
		// this is a performance optimization - array index is cheaper then invoking a method with 'switch'
		addEngineSniffferEvent(VVT_NAME, front == TV_RISE ? WC_UP : WC_DOWN);
	}

	if (boardConfiguration->vvtCamSensorUseRise ^ (front != TV_FALL)) {
		return;
	}

	floatus_t oneDegreeUs = engine->rpmCalculator.oneDegreeUs;
	if (cisnan(oneDegreeUs)) {
		// we are here if we are getting VVT position signals while engine is not running
		// for example if crank position sensor is broken :)
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
			scheduleMsg(logger, "vvt ratio %.2f", ratio);
		}
		if (ratio < boardConfiguration->nb2ratioFrom || ratio > boardConfiguration->nb2ratioTo) {
			return;
		}
		if (engineConfiguration->isPrintTriggerSynchDetails) {
			scheduleMsg(logger, "looks good: vvt ratio %.2f", ratio);
		}
	}


	efitick_t offsetNt = nowNt - tc->timeAtVirtualZeroNt;

	angle_t vvtPosition = NT2US(offsetNt) / oneDegreeUs;

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
			tc->triggerState.incrementTotalEventCounter();
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
			tc->triggerState.incrementTotalEventCounter();
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
			tc->triggerState.incrementTotalEventCounter();
		}
	}

}

void hwHandleShaftSignal(trigger_event_e signal) {
	// for effective noise filtering, we need both signal edges, 
	// so we pass them to handleShaftSignal() and defer this test
	if (!boardConfiguration->useNoiselessTriggerDecoder) {
		if (!isUsefulSignal(signal, engineConfiguration)) {
			return;
		}
	}
	uint32_t triggerHandlerEntryTime = GET_TIMESTAMP();
	isInsideTriggerHandler = true;
	if (triggerReentraint > maxTriggerReentraint)
		maxTriggerReentraint = triggerReentraint;
	triggerReentraint++;
	efiAssertVoid(getRemainingStack(chThdGetSelfX()) > 128, "lowstck#8");
	engine->triggerCentral.handleShaftSignal(signal PASS_ENGINE_PARAMETER_SUFFIX);
	triggerReentraint--;
	triggerDuration = GET_TIMESTAMP() - triggerHandlerEntryTime;
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
	resetAccumSignalData();
}

int TriggerCentral::getHwEventCounter(int index) {
	return hwEventCounters[index];
}

void TriggerCentral::resetCounters() {
	memset(hwEventCounters, 0, sizeof(hwEventCounters));
	triggerState.resetRunningCounters();
}

void TriggerCentral::resetAccumSignalData() {
	memset(lastSignalTimes, 0xff, sizeof(lastSignalTimes));	// = -1
	memset(accumSignalPeriods, 0, sizeof(accumSignalPeriods));
	memset(accumSignalPrevPeriods, 0, sizeof(accumSignalPrevPeriods));
}

static char shaft_signal_msg_index[15];

static bool isUpEvent[6] = { false, true, false, true, false, true };
static const char *eventId[6] = { CRANK1, CRANK1, CRANK2, CRANK2, CRANK3, CRANK3 };

static ALWAYS_INLINE void reportEventToWaveChart(trigger_event_e ckpSignalType, int index DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (!ENGINE(isEngineChartEnabled)) { // this is here just as a shortcut so that we avoid engine sniffer as soon as possible
		return; // engineSnifferRpmThreshold is accounted for inside ENGINE(isEngineChartEnabled)
	}


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

/**
 * This is used to filter noise spikes (interference) in trigger signal. See 
 * The basic idea is to use not just edges, but the average amount of time the signal stays in '0' or '1'.
 * So we update 'accumulated periods' to track where the signal is. 
 * And then compare between the current period and previous, with some tolerance (allowing for the wheel speed change).
 * @return true if the signal is passed through.
 */
bool TriggerCentral::noiseFilter(efitick_t nowNt, trigger_event_e signal DECLARE_ENGINE_PARAMETER_SUFFIX) {
	// todo: find a better place for these defs
	static const trigger_event_e opposite[6] = { SHAFT_PRIMARY_RISING, SHAFT_PRIMARY_FALLING, SHAFT_SECONDARY_RISING, SHAFT_SECONDARY_FALLING, 
			SHAFT_3RD_RISING, SHAFT_3RD_FALLING };
	static const trigger_wheel_e triggerIdx[6] = { T_PRIMARY, T_PRIMARY, T_SECONDARY, T_SECONDARY, T_CHANNEL_3, T_CHANNEL_3 };
	// we process all trigger channels independently
	trigger_wheel_e ti = triggerIdx[signal];
	// falling is opposite to rising, and vise versa
	trigger_event_e os = opposite[signal];
	
	// todo: currently only primary channel is filtered, because there are some weird trigger types on other channels
	if (ti != T_PRIMARY)
		return true;
	
	// update period accumulator: for rising signal, we update '0' accumulator, and for falling - '1'
	if (lastSignalTimes[signal] != -1)
		accumSignalPeriods[signal] += nowNt - lastSignalTimes[signal];
	// save current time for this trigger channel
	lastSignalTimes[signal] = nowNt;
	
	// now we want to compare current accumulated period to the stored one 
	efitick_t currentPeriod = accumSignalPeriods[signal];
	// the trick is to compare between different
	efitick_t allowedPeriod = accumSignalPrevPeriods[os];

	// but first check if we're expecting a gap
	bool isGapExpected = TRIGGER_SHAPE(isSynchronizationNeeded) && triggerState.shaft_is_synchronized && 
			(triggerState.currentCycle.eventCount[ti] + 1) == TRIGGER_SHAPE(expectedEventCount[ti]);
	
	if (isGapExpected) {
		// usually we need to extend the period for gaps, based on the trigger info
		allowedPeriod *= TRIGGER_SHAPE(syncRatioAvg);
	}
	
	// also we need some margin for rapidly changing trigger-wheel speed,
	// that's why we expect the period to be no less than 2/3 of the previous period (this is just an empirical 'magic' coef.)
	efitick_t minAllowedPeriod = 2 * allowedPeriod / 3;
	// but no longer than 5/4 of the previous 'normal' period
	efitick_t maxAllowedPeriod = 5 * allowedPeriod / 4;
	
	// above all, check if the signal comes not too early
	if (currentPeriod >= minAllowedPeriod) {
		// now we store this period as a reference for the next time,
		// BUT we store only 'normal' periods, and ignore too long periods (i.e. gaps)
		if (!isGapExpected && (maxAllowedPeriod == 0 || currentPeriod <= maxAllowedPeriod)) {
			accumSignalPrevPeriods[signal] = currentPeriod;
		}
		// reset accumulator
		accumSignalPeriods[signal] = 0;
		return true;
	}
	// all premature or extra-long events are ignored - treated as interference
	return false;
}

void TriggerCentral::handleShaftSignal(trigger_event_e signal DECLARE_ENGINE_PARAMETER_SUFFIX) {
	efiAssertVoid(engine!=NULL, "configuration");

	if (triggerShape.shapeDefinitionError) {
		// trigger is broken, we cannot do anything here
		warning(CUSTOM_ERR_UNEXPECTED_SHAFT_EVENT, "Shaft event while trigger is mis-configured");
		return;
	}

	nowNt = getTimeNowNt();

	// This code gathers some statistics on signals and compares accumulated periods to filter interference
	if (boardConfiguration->useNoiselessTriggerDecoder) {
		if (!noiseFilter(nowNt, signal PASS_ENGINE_PARAMETER_SUFFIX)) {
			return;
		}
		// moved here from hwHandleShaftSignal()
		if (!isUsefulSignal(signal, engineConfiguration)) {
			return;
		}
	}

	engine->onTriggerSignalEvent(nowNt);

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
		triggerState.onSynchronizationLost(PASS_ENGINE_PARAMETER_SIGNATURE);
	}
	previousShaftEventTimeNt = nowNt;

	/**
	 * This invocation changes the state of triggerState
	 */
	triggerState.decodeTriggerEvent(signal, nowNt PASS_ENGINE_PARAMETER_SUFFIX);

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
	reportEventToWaveChart(signal, triggerIndexForListeners PASS_ENGINE_PARAMETER_SUFFIX);

	if (!triggerState.shaft_is_synchronized) {
		// we should not propagate event if we do not know where we are
		return;
	}

	if (triggerState.isValidIndex(PASS_ENGINE_PARAMETER_SIGNATURE)) {
#if TRIGGER_EXTREME_LOGGING || defined(__DOXYGEN__)
	scheduleMsg(logger, "trigger %d %d %d", triggerIndexForListeners, getRevolutionCounter(), (int)getTimeNowUs());
#endif /* FUEL_MATH_EXTREME_LOGGING */

		/**
		 * Here we invoke all the listeners - the main engine control logic is inside these listeners
		 */
		for (int i = 0; i < triggerListeneres.currentListenersCount; i++) {
			ShaftPositionListener listener = (ShaftPositionListener) triggerListeneres.callbacks[i];
			(listener)(signal, triggerIndexForListeners PASS_ENGINE_PARAMETER_SUFFIX);
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
	scheduleMsg(logger, "gap from %.2f to %.2f", s->syncRatioFrom, s->syncRatioTo);

	for (int i = 0; i < s->getSize(); i++) {
		scheduleMsg(logger, "event %d %.2f", i, s->eventAngles[i]);
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
		s->initializeTriggerShape(NULL PASS_ENGINE_PARAMETER_SUFFIX);

		efiAssertVoid(!s->shapeDefinitionError, "trigger error");

		fprintf(fp, "TRIGGERTYPE %d %d %s %.2f\n", triggerId, s->getLength(), getTrigger_type_e(tt), s->tdcPosition);

		fprintf(fp, "# duty %.2f %.2f\n", s->expectedDutyCycle[0], s->expectedDutyCycle[1]);

		for (int i = 0; i < s->getLength(); i++) {

			int triggerDefinitionCoordinate = (s->getTriggerShapeSynchPointIndex() + i) % s->getSize();


			fprintf(fp, "event %d %d %.2f\n", i, s->triggerSignals[triggerDefinitionCoordinate], s->eventAngles[i]);
		}

	}
	fclose(fp);
	printf("All triggers exported to %s\n", TRIGGERS_FILE_NAME);
}

#endif


#if EFI_PROD_CODE || defined(__DOXYGEN__)
extern PwmConfig triggerSignal;
#endif /* #if EFI_PROD_CODE */

extern uint32_t hipLastExecutionCount;
extern uint32_t hwSetTimerDuration;

extern uint32_t maxLockedDuration;
extern uint32_t maxEventCallbackDuration;

#if (EFI_PROD_CODE) || defined(__DOXYGEN__)
extern uint32_t maxPrecisionCallbackDuration;
#endif /* EFI_PROD_CODE  */

extern uint32_t maxSchedulingPrecisionLoss;
extern uint32_t *cyccnt;

extern int vvtEventRiseCounter;
extern int vvtEventFallCounter;

void resetMaxValues() {
#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
	maxEventCallbackDuration = triggerMaxDuration = 0;
#endif /* EFI_PROD_CODE || EFI_SIMULATOR */

	maxSchedulingPrecisionLoss = 0;

#if EFI_CLOCK_LOCKS || defined(__DOXYGEN__)
	maxLockedDuration = 0;
#endif /* EFI_CLOCK_LOCKS */

#if (EFI_PROD_CODE) || defined(__DOXYGEN__)
	maxPrecisionCallbackDuration = 0;
#endif /* EFI_PROD_CODE  */
}

void triggerInfo(void) {
#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)

	TriggerShape *ts = &engine->triggerCentral.triggerShape;

	scheduleMsg(logger, "Template %s (%d) trigger %s (%d) useRiseEdge=%s onlyFront=%s useOnlyFirstChannel=%s tdcOffset=%.2f",
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
	scheduleMsg(logger, "expected duty #0=%.2f/#1=%.2f", TRIGGER_SHAPE(expectedDutyCycle[0]), TRIGGER_SHAPE(expectedDutyCycle[1]));

	scheduleMsg(logger, "synchronizationNeeded=%s/isError=%s/total errors=%d ord_err=%d/total revolutions=%d/self=%s",
			boolToString(ts->isSynchronizationNeeded),
			boolToString(isTriggerDecoderError()), engine->triggerCentral.triggerState.totalTriggerErrorCounter,
			engine->triggerCentral.triggerState.orderingErrorCounter, engine->triggerCentral.triggerState.getTotalRevolutionCounter(),
			boolToString(engineConfiguration->directSelfStimulation));

	if (TRIGGER_SHAPE(isSynchronizationNeeded)) {
		scheduleMsg(logger, "gap from %.2f to %.2f", ts->syncRatioFrom, ts->syncRatioTo);
	}

#endif /* EFI_PROD_CODE || EFI_SIMULATOR */

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	if (engineConfiguration->camInput != GPIO_UNASSIGNED) {
		scheduleMsg(logger, "VVT input: %s mode %s", hwPortname(engineConfiguration->camInput),
				getVvt_mode_e(engineConfiguration->vvtMode));
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

	scheduleMsg(logger, "zeroTestTime=%d maxSchedulingPrecisionLoss=%d", engine->m.zeroTestTime, maxSchedulingPrecisionLoss);

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

#if EFI_CLOCK_LOCKS || defined(__DOXYGEN__)
	scheduleMsg(logger, "maxLockedDuration=%d / maxTriggerReentraint=%d", maxLockedDuration, maxTriggerReentraint);
#endif /* EFI_CLOCK_LOCKS */

	scheduleMsg(logger, "maxEventCallbackDuration=%d", maxEventCallbackDuration);

	scheduleMsg(logger, "hipLastExecutionCount=%d", hipLastExecutionCount);
	scheduleMsg(logger, "hwSetTimerDuration=%d", hwSetTimerDuration);

	scheduleMsg(logger, "totalTriggerHandlerMaxTime=%d", triggerMaxDuration);
	scheduleMsg(logger, "maxPrecisionCallbackDuration=%d", maxPrecisionCallbackDuration);
	resetMaxValues();

#endif /* EFI_PROD_CODE */
}

static void resetRunningTriggerCounters() {
#if !EFI_UNIT_TEST || defined(__DOXYGEN__)
	engine->triggerCentral.resetCounters();
	triggerInfo();
#endif
}

#define COMPARE_CONFIG_PARAMS(param) (engineConfiguration->param != previousConfiguration->param)

void onConfigurationChangeTriggerCallback(engine_configuration_s *previousConfiguration DECLARE_ENGINE_PARAMETER_SUFFIX) {
	bool changed = COMPARE_CONFIG_PARAMS(trigger.type) ||
		COMPARE_CONFIG_PARAMS(operationMode) ||
		COMPARE_CONFIG_PARAMS(useOnlyRisingEdgeForTrigger) ||
		COMPARE_CONFIG_PARAMS(globalTriggerAngleOffset) ||
		COMPARE_CONFIG_PARAMS(trigger.customTotalToothCount) ||
		COMPARE_CONFIG_PARAMS(trigger.customSkippedToothCount) ||
		COMPARE_CONFIG_PARAMS(bc.triggerInputPins[0]) ||
		COMPARE_CONFIG_PARAMS(bc.triggerInputPins[1]) ||
		COMPARE_CONFIG_PARAMS(bc.triggerInputPins[2]) ||
		COMPARE_CONFIG_PARAMS(camInput) ||
		COMPARE_CONFIG_PARAMS(vvtMode) ||
		COMPARE_CONFIG_PARAMS(bc.vvtCamSensorUseRise) ||
		COMPARE_CONFIG_PARAMS(vvtOffset) ||
		COMPARE_CONFIG_PARAMS(vvtDisplayInverted) ||
		COMPARE_CONFIG_PARAMS(bc.nb2ratioFrom) ||
		COMPARE_CONFIG_PARAMS(bc.nb2ratioTo) ||
		COMPARE_CONFIG_PARAMS(nbVvtIndex);
	if (changed) {
		assertEngineReference();

	#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
		engine->triggerCentral.triggerShape.initializeTriggerShape(logger PASS_ENGINE_PARAMETER_SUFFIX);
		engine->triggerCentral.resetAccumSignalData();
	#endif
	}
#if EFI_DEFAILED_LOGGING
	scheduleMsg(logger, "isTriggerConfigChanged=%d", isTriggerConfigChanged);
#endif /* EFI_DEFAILED_LOGGING */

	// we do not want to miss two updates in a row
	isTriggerConfigChanged = isTriggerConfigChanged || changed;
}

/**
 * @returns true if configuration just changed, and if that change has affected trigger
 */
bool checkIfTriggerConfigChanged(void) {
	bool result = triggerVersion.isOld() && isTriggerConfigChanged;
	isTriggerConfigChanged = false; // whoever has called the method is supposed to react to changes
	return result;
}

bool readIfTriggerConfigChangedForUnitTest(void) {
	return isTriggerConfigChanged;
}

void resetTriggerConfigChangedForUnitTest(void) {
	isTriggerConfigChanged = false;
}

void initTriggerCentral(Logging *sharedLogger) {
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
