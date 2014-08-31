/*
 * @file	trigger_central.cpp
 *
 * @date Feb 23, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "trigger_central.h"
#include "trigger_decoder.h"
#include "main_trigger_callback.h"
#include "engine_configuration.h"
#include "listener_array.h"
#include "data_buffer.h"
#include "histogram.h"
#include "wave_chart.h"

#include "rpm_calculator.h"
#if EFI_PROD_CODE
#include "rfiutil.h"
#include "pin_repository.h"
#endif

WaveChart waveChart;

static histogram_s triggerCallback;

// we need this initial to have not_running at first invocation
static volatile uint64_t previousShaftEventTime = (efitimems_t) -10 * US_PER_SECOND;

TriggerCentral triggerCentral;

static Logging logger;

uint64_t getCrankEventCounter() {
	return triggerCentral.triggerState.getTotalEventCounter();
}

uint64_t getStartOfRevolutionIndex() {
	return triggerCentral.triggerState.getStartOfRevolutionIndex();
}

void TriggerCentral::addEventListener(ShaftPositionListener listener, const char *name, void *arg) {
	print("registerCkpListener: %s\r\n", name);
	registerCallback(&triggerListeneres, (IntListener) listener, arg);
}

/**
 * @brief Adds a trigger event listener
 *
 * Trigger event listener would be invoked on each trigger event. For example, for a 60/2 wheel
 * that would be 116 events: 58 SHAFT_PRIMARY_UP and 58 SHAFT_PRIMARY_DOWN events.
 */
void addTriggerEventListener(ShaftPositionListener listener, const char *name, void *arg) {
	triggerCentral.addEventListener(listener, name, arg);
}

#if EFI_PROD_CODE || EFI_SIMULATOR
extern configuration_s *configuration;

void hwHandleShaftSignal(trigger_event_e signal) {
	triggerCentral.handleShaftSignal(configuration, signal, getTimeNowUs());
}
#endif /* EFI_PROD_CODE */

TriggerCentral::TriggerCentral() {
	memset(hwEventCounters, 0, sizeof(hwEventCounters));
	clearCallbacks(&triggerListeneres);
}

int TriggerCentral::getHwEventCounter(int index) {
	return hwEventCounters[index];
}

static char shaft_signal_msg_index[15];

static void reportEventToWaveChart(trigger_event_e ckpSignalType, int index) {
	itoa10(&shaft_signal_msg_index[1], index);
	if (ckpSignalType == SHAFT_PRIMARY_UP) {
		addWaveChartEvent(WC_CRANK1, WC_UP, (char*) shaft_signal_msg_index);
	} else if (ckpSignalType == SHAFT_PRIMARY_DOWN) {
		addWaveChartEvent(WC_CRANK1, WC_DOWN, (char*) shaft_signal_msg_index);
	} else if (ckpSignalType == SHAFT_SECONDARY_UP) {
		addWaveChartEvent(WC_CRANK2, WC_UP, (char*) shaft_signal_msg_index);
	} else if (ckpSignalType == SHAFT_SECONDARY_DOWN) {
		addWaveChartEvent(WC_CRANK2, WC_DOWN, (char*) shaft_signal_msg_index);
	} else if (ckpSignalType == SHAFT_3RD_UP) {
		addWaveChartEvent(WC_CRANK3, WC_UP, (char*) shaft_signal_msg_index);
	} else if (ckpSignalType == SHAFT_3RD_DOWN) {
		addWaveChartEvent(WC_CRANK3, WC_DOWN, (char*) shaft_signal_msg_index);
	}
}


void TriggerCentral::handleShaftSignal(configuration_s *configuration, trigger_event_e signal, uint64_t nowUs) {
	efiAssertVoid(configuration!=NULL, "configuration");

	efiAssertVoid(configuration->engineConfiguration!=NULL, "engineConfiguration");
	efiAssertVoid(configuration->engineConfiguration2!=NULL, "engineConfiguration2");

#if EFI_HISTOGRAMS && EFI_PROD_CODE
	int beforeCallback = hal_lld_get_counter_value();
#endif
	int eventIndex = (int) signal;
	efiAssertVoid(eventIndex >= 0 && eventIndex < HW_EVENT_TYPES, "signal type");
	hwEventCounters[eventIndex]++;

	if (nowUs - previousShaftEventTime > US_PER_SECOND) {
		/**
		 * We are here if there is a time gap between now and previous shaft event - that means the engine is not runnig.
		 * That means we have lost synchronization since the engine is not running :)
		 */
		triggerState.shaft_is_synchronized = false;
	}
	previousShaftEventTime = nowUs;

	trigger_shape_s * triggerShape = &configuration->engineConfiguration2->triggerShape;

	/**
	 * This invocation changes the state of triggerState
	 */
	triggerState.decodeTriggerEvent(triggerShape, &configuration->engineConfiguration->triggerConfig, signal, nowUs);

	if (!triggerState.shaft_is_synchronized) {
		// we should not propagate event if we do not know where we are
		return;
	}

	/**
	 * If we only have a crank position sensor, here we are extending crank revolutions with a 360 degree
	 * cycle into a four stroke, 720 degrees cycle. TODO
	 */
	int triggerIndexForListeners;
	if (getOperationMode(configuration->engineConfiguration) == FOUR_STROKE_CAM_SENSOR) {
		// That's easy - trigger cycle matches engine cycle
		triggerIndexForListeners = triggerState.getCurrentIndex();
	} else {
		bool isEven = (triggerState.getTotalRevolutionCounter() & 1) == 0;

		triggerIndexForListeners = triggerState.getCurrentIndex() + (isEven ? 0 : triggerShape->getSize());
	}
	reportEventToWaveChart(signal, triggerIndexForListeners);

	if (triggerState.getCurrentIndex() >= configuration->engineConfiguration2->triggerShape.shaftPositionEventCount) {
		warning(OBD_PCM_Processor_Fault, "unexpected eventIndex=%d", triggerState.getCurrentIndex());
	} else {

		/**
		 * Here we invoke all the listeners - the main engine control logic is inside these listeners
		 */
		invokeIntIntVoidCallbacks(&triggerListeneres, signal, triggerIndexForListeners);
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
	printHistogram(&logger, &triggerCallback);
#endif
}

#if EFI_PROD_CODE || EFI_SIMULATOR
// todo: eliminate this extern which is needed by 'triggerInfo'
extern engine_configuration_s *engineConfiguration;
extern engine_configuration2_s * engineConfiguration2;
extern board_configuration_s *boardConfiguration;
#endif

static void triggerInfo() {
#if EFI_PROD_CODE || EFI_SIMULATOR
	scheduleMsg(&logger, "Template %s/%d trigger %d", getConfigurationName(engineConfiguration),
			engineConfiguration->engineType, engineConfiguration->triggerConfig.triggerType);

	scheduleMsg(&logger, "trigger event counters %d/%d/%d/%d", triggerCentral.getHwEventCounter(0),
			triggerCentral.getHwEventCounter(1), triggerCentral.getHwEventCounter(2),
			triggerCentral.getHwEventCounter(3));
	scheduleMsg(&logger, "expected cycle events %d/%d/%d", engineConfiguration2->triggerShape.expectedEventCount[0],
			engineConfiguration2->triggerShape.expectedEventCount[1],
			engineConfiguration2->triggerShape.expectedEventCount[2]);

	scheduleMsg(&logger, "trigger type=%d/need2ndChannel=%s", engineConfiguration->triggerConfig.triggerType,
			boolToString(engineConfiguration->needSecondTriggerInput));
	scheduleMsg(&logger, "expected duty #0=%f/#1=%f", engineConfiguration2->triggerShape.dutyCycle[0],
			engineConfiguration2->triggerShape.dutyCycle[1]);
#endif

#if EFI_PROD_CODE
	scheduleMsg(&logger, "primary trigger simulator: %s %s", hwPortname(boardConfiguration->triggerSimulatorPins[0]),
			pinModeToString(boardConfiguration->triggerSimulatorPinModes[0]));
	scheduleMsg(&logger, "secondary trigger simulator: %s %s", hwPortname(boardConfiguration->triggerSimulatorPins[1]),
			pinModeToString(boardConfiguration->triggerSimulatorPinModes[1]));
	scheduleMsg(&logger, "3rd trigger simulator: %s %s", hwPortname(boardConfiguration->triggerSimulatorPins[2]),
			pinModeToString(boardConfiguration->triggerSimulatorPinModes[2]));

	scheduleMsg(&logger, "primary trigger input: %s", hwPortname(boardConfiguration->triggerInputPins[0]));
	scheduleMsg(&logger, "secondary trigger input: %s", hwPortname(boardConfiguration->triggerInputPins[1]));
	scheduleMsg(&logger, "primary logic input: %s", hwPortname(boardConfiguration->logicAnalyzerPins[0]));
	scheduleMsg(&logger, "secondary logic input: %s", hwPortname(boardConfiguration->logicAnalyzerPins[1]));

#endif /* EFI_PROD_CODE */
}

float getTriggerDutyCycle(int index) {
	return triggerCentral.triggerState.getTriggerDutyCycle(index);
}

void initTriggerCentral(void) {
	strcpy((char*) shaft_signal_msg_index, "_");

#if EFI_WAVE_CHART
	initWaveChart(&waveChart);
#endif

#if EFI_PROD_CODE || EFI_SIMULATOR
	initLogging(&logger, "ShaftPosition");
	addConsoleAction("triggerinfo", triggerInfo);
#endif

#if EFI_HISTOGRAMS
	initHistogram(&triggerCallback, "all callbacks");
#endif /* EFI_HISTOGRAMS */
	initTriggerDecoder();
}
