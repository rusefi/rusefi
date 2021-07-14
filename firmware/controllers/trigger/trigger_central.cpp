/*
 * @file	trigger_central.cpp
 * Here we have a bunch of higher-level methods which are not directly related to actual signal decoding
 *
 * @date Feb 23, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "global.h"
#include "os_access.h"

#include "trigger_central.h"
#include "trigger_decoder.h"
#include "main_trigger_callback.h"
#include "engine_configuration.h"
#include "listener_array.h"
#include "pwm_generator_logic.h"
#include "tooth_logger.h"
#include "hip9011.h"
#include "logic_analyzer.h"

#include "settings.h"
#include "engine_math.h"
#include "local_version_holder.h"
#include "trigger_simulator.h"
#include "trigger_emulator_algo.h"

#include "rpm_calculator.h"
#include "tooth_logger.h"
#include "perf_trace.h"
#include "map_averaging.h"
#include "main_trigger_callback.h"


#if EFI_PROD_CODE
#include "pin_repository.h"
#endif /* EFI_PROD_CODE */

#if EFI_TUNER_STUDIO
#include "tunerstudio.h"
#endif /* EFI_TUNER_STUDIO */

#if EFI_ENGINE_SNIFFER
#include "engine_sniffer.h"
WaveChart waveChart;
#endif /* EFI_ENGINE_SNIFFER */

static scheduling_s debugToggleScheduling;
#define DEBUG_PIN_DELAY MS2NT(100)

trigger_central_s::trigger_central_s() : hwEventCounters() {
}

TriggerCentral::TriggerCentral() : trigger_central_s(),
		vvtPosition(),
		vvtSyncTimeNt() {
	triggerState.resetTriggerState();
	noiseFilter.resetAccumSignalData();
}

void TriggerCentral::init(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	INJECT_ENGINE_REFERENCE(&triggerState);
	for (int bankIndex = 0; bankIndex < BANKS_COUNT; bankIndex++) {
		for (int camIndex = 0; camIndex < CAMS_PER_BANK; camIndex++) {
			INJECT_ENGINE_REFERENCE(&vvtState[bankIndex][camIndex]);
		}
	}
}

void TriggerNoiseFilter::resetAccumSignalData() {
	memset(lastSignalTimes, 0xff, sizeof(lastSignalTimes));	// = -1
	memset(accumSignalPeriods, 0, sizeof(accumSignalPeriods));
	memset(accumSignalPrevPeriods, 0, sizeof(accumSignalPrevPeriods));
}

int TriggerCentral::getHwEventCounter(int index) const {
	return hwEventCounters[index];
}

#if EFI_SHAFT_POSITION_INPUT

EXTERN_ENGINE;

angle_t TriggerCentral::getVVTPosition(uint8_t bankIndex, uint8_t camIndex) {
	if (bankIndex >= BANKS_COUNT || camIndex >= CAMS_PER_BANK) {
		return NAN;
	}
	return vvtPosition[bankIndex][camIndex];
}

/**
 * todo: why is this method NOT reciprocal to getRpmMultiplier?!
 */
static int getCrankDivider(operation_mode_e operationMode) {
	switch (operationMode) {
	case FOUR_STROKE_CRANK_SENSOR:
		return 2;
	case FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR:
		return SYMMETRICAL_CRANK_SENSOR_DIVIDER;
	case FOUR_STROKE_THREE_TIMES_CRANK_SENSOR:
		return SYMMETRICAL_THREE_TIMES_CRANK_SENSOR_DIVIDER;
	default:
	case FOUR_STROKE_CAM_SENSOR:
	case TWO_STROKE:
		// That's easy - trigger cycle matches engine cycle
		return 1;
	}
}

#define miataNbIndex (0)

static bool vvtWithRealDecoder(vvt_mode_e vvtMode) {
	// todo: why does VVT_2JZ not use real decoder?
	return vvtMode != VVT_INACTIVE
			&& vvtMode != VVT_2JZ
			&& vvtMode != VVT_SECOND_HALF
			&& vvtMode != VVT_FIRST_HALF;
}

static void syncAndReport(TriggerCentral *tc, int mod, int remainder DECLARE_ENGINE_PARAMETER_SUFFIX) {
	bool wasChanged = tc->triggerState.syncSymmetricalCrank(mod, remainder);
	if (wasChanged && engineConfiguration->debugMode == DBG_VVT) {
#if EFI_TUNER_STUDIO
		tsOutputChannels.debugIntField1++;
#endif /* EFI_TUNER_STUDIO */
	}
}

static void turnOffAllDebugFields(void *arg) {
	(void)arg;
#if EFI_PROD_CODE
	for (int index = 0;index<TRIGGER_INPUT_PIN_COUNT;index++) {
		if (CONFIG(triggerInputDebugPins[index]) != GPIO_UNASSIGNED) {
			writePad("trigger debug", CONFIG(triggerInputDebugPins[index]), 0);
		}
	}
	for (int index = 0;index<CAM_INPUTS_COUNT;index++) {
		if (CONFIG(camInputsDebug[index]) != GPIO_UNASSIGNED) {
			writePad("cam debug", CONFIG(camInputsDebug[index]), 0);
		}
	}
#endif /* EFI_PROD_CODE */
}

void hwHandleVvtCamSignal(trigger_value_e front, efitick_t nowNt, int index DECLARE_ENGINE_PARAMETER_SUFFIX) {
	int bankIndex = index / CAMS_PER_BANK;
	int camIndex = index % CAMS_PER_BANK;
	TriggerCentral *tc = &engine->triggerCentral;
	if (front == TV_RISE) {
		tc->vvtEventRiseCounter++;
	} else {
		tc->vvtEventFallCounter++;
	}
	extern const char *vvtNames[];
	const char *vvtName = vvtNames[index];
	if (CONFIG(vvtMode[camIndex]) == VVT_INACTIVE) {
		warning(CUSTOM_VVT_MODE_NOT_SELECTED, "VVT: event on %d but no mode", camIndex);
	}

#if VR_HW_CHECK_MODE
	// some boards do not have hardware VR input LEDs which makes such boards harder to validate
	// from experience we know that assembly mistakes happen and quality control is required
	extern ioportid_t criticalErrorLedPort;
	extern ioportmask_t criticalErrorLedPin;


	for (int i = 0 ; i < 100 ; i++) {
		// turning pin ON and busy-waiting a bit
		palWritePad(criticalErrorLedPort, criticalErrorLedPin, 1);
	}

	palWritePad(criticalErrorLedPort, criticalErrorLedPin, 0);
#endif // VR_HW_CHECK_MODE

	if (!CONFIG(displayLogicLevelsInEngineSniffer)) {
		addEngineSnifferEvent(vvtName, front == TV_RISE ? PROTOCOL_ES_UP : PROTOCOL_ES_DOWN);

#if EFI_TOOTH_LOGGER
// todo: we need to start logging different VVT channels differently!!!
		trigger_event_e tooth;
		if (index == 0) {
			tooth = front == TV_RISE ? SHAFT_SECONDARY_RISING : SHAFT_SECONDARY_FALLING;
		} else {
			// todo: nicer solution is needed
			tooth = front == TV_RISE ? SHAFT_3RD_RISING : SHAFT_3RD_FALLING;
		}

		LogTriggerTooth(tooth, nowNt PASS_ENGINE_PARAMETER_SUFFIX);
#endif /* EFI_TOOTH_LOGGER */
	}


	bool isImportantFront = (CONFIG(vvtCamSensorUseRise) ^ (front == TV_FALL));
	if (!vvtWithRealDecoder(engineConfiguration->vvtMode[camIndex]) && !isImportantFront) {
		// todo: there should be a way to always use real trigger code for this logic?
		return;
	}

	if (isImportantFront && CONFIG(camInputsDebug[index]) != GPIO_UNASSIGNED) {
#if EFI_PROD_CODE
		writePad("cam debug", CONFIG(camInputsDebug[index]), 1);
#endif /* EFI_PROD_CODE */
		engine->executor.scheduleByTimestamp("dbg_on", &debugToggleScheduling, nowNt + DEBUG_PIN_DELAY, &turnOffAllDebugFields);
	}

	if (CONFIG(displayLogicLevelsInEngineSniffer) && isImportantFront) {
		if (CONFIG(vvtCamSensorUseRise)) {
			// todo: unify TS composite logger code with console Engine Sniffer
			// todo: better API to reduce copy/paste?
#if EFI_TOOTH_LOGGER
			LogTriggerTooth(SHAFT_SECONDARY_RISING, nowNt PASS_ENGINE_PARAMETER_SUFFIX);
			LogTriggerTooth(SHAFT_SECONDARY_FALLING, nowNt PASS_ENGINE_PARAMETER_SUFFIX);
#endif /* EFI_TOOTH_LOGGER */
			addEngineSnifferEvent(vvtName, PROTOCOL_ES_UP);
			addEngineSnifferEvent(vvtName, PROTOCOL_ES_DOWN);
		} else {
#if EFI_TOOTH_LOGGER
			LogTriggerTooth(SHAFT_SECONDARY_FALLING, nowNt PASS_ENGINE_PARAMETER_SUFFIX);
			LogTriggerTooth(SHAFT_SECONDARY_RISING, nowNt PASS_ENGINE_PARAMETER_SUFFIX);
#endif /* EFI_TOOTH_LOGGER */

			addEngineSnifferEvent(vvtName, PROTOCOL_ES_DOWN);
			addEngineSnifferEvent(vvtName, PROTOCOL_ES_UP);
		}
	}

	floatus_t oneDegreeUs = engine->rpmCalculator.oneDegreeUs;
	if (cisnan(oneDegreeUs)) {
		// todo: this code branch is slowing NB2 cranking since we require RPM sync for VVT sync!
		// todo: smarter code
		//
		// we are here if we are getting VVT position signals while engine is not running
		// for example if crank position sensor is broken :)
		return;
	}

	tc->vvtState[bankIndex][camIndex].decodeTriggerEvent(
			tc->vvtShape[camIndex],
			nullptr,
			nullptr,
			engine->vvtTriggerConfiguration[camIndex],
			front == TV_RISE ? SHAFT_PRIMARY_RISING : SHAFT_PRIMARY_FALLING, nowNt);


	tc->vvtCamCounter++;

	float offsetUs = tc->virtualZeroTimer.getElapsedUs(nowNt);
	angle_t currentPosition = offsetUs / oneDegreeUs;
	// convert engine cycle angle into trigger cycle angle
	currentPosition -= tdcPosition();
	// https://github.com/rusefi/rusefi/issues/1713 currentPosition could be negative that's expected

#if EFI_UNIT_TEST
	tc->currentVVTEventPosition[bankIndex][camIndex] = currentPosition;
#endif // EFI_UNIT_TEST

	if (engineConfiguration->debugMode == DBG_VVT) {
#if EFI_TUNER_STUDIO
		tsOutputChannels.debugFloatField1 = currentPosition;
#endif /* EFI_TUNER_STUDIO */
	}

	switch(engineConfiguration->vvtMode[camIndex]) {
	case VVT_2JZ:
		// we do not know if we are in sync or out of sync, so we have to be looking for both possibilities
		if ((currentPosition < engineConfiguration->fsio_setting[14]       || currentPosition > engineConfiguration->fsio_setting[15]) &&
		    (currentPosition < engineConfiguration->fsio_setting[14] + 360 || currentPosition > engineConfiguration->fsio_setting[15] + 360)) {
			// outside of the expected range
			return;
		}
		break;
	case VVT_MIATA_NB2:
	case VVT_BOSCH_QUICK_START:
	case VVT_BARRA_3_PLUS_1:
	case VVT_NISSAN_VQ:
	 {
		if (tc->vvtState[bankIndex][camIndex].currentCycle.current_index != 0) {
			// this is not sync tooth - exiting
			return;
		}
		if (engineConfiguration->debugMode == DBG_VVT) {
#if EFI_TUNER_STUDIO
			tsOutputChannels.debugIntField1++;
#endif /* EFI_TUNER_STUDIO */
		}
	}
	default:
		// else, do nothing
		break;
	}

	tc->vvtSyncTimeNt[bankIndex][camIndex] = nowNt;

	auto vvtPosition = engineConfiguration->vvtOffsets[bankIndex * CAMS_PER_BANK + camIndex] - currentPosition;

	// Wrap VVT position in to the range [-360, 360)
	while (vvtPosition < -360) {
		vvtPosition += 720;
	}
	while (vvtPosition >= 360) {
		vvtPosition -= 720;
	}

	tc->vvtPosition[bankIndex][camIndex] = vvtPosition;

	if (index != 0) {
		// at the moment we use only primary VVT to sync crank phase
		return;
	}

	operation_mode_e operationMode = engine->getOperationMode(PASS_ENGINE_PARAMETER_SIGNATURE);

	switch (engineConfiguration->vvtMode[camIndex]) {
	case VVT_FIRST_HALF:
		syncAndReport(tc, getCrankDivider(operationMode), 1 PASS_ENGINE_PARAMETER_SUFFIX);
		break;
	case VVT_SECOND_HALF:
		syncAndReport(tc, getCrankDivider(operationMode), 0 PASS_ENGINE_PARAMETER_SUFFIX);
		break;
	case VVT_MIATA_NB2:
		/**
		 * NB2 is a symmetrical crank, there are four phases total
		 */
		syncAndReport(tc, getCrankDivider(operationMode), miataNbIndex PASS_ENGINE_PARAMETER_SUFFIX);
		break;
	case VVT_NISSAN_VQ:
		syncAndReport(tc, getCrankDivider(operationMode), 0 PASS_ENGINE_PARAMETER_SUFFIX);
		break;
	default:
	case VVT_INACTIVE:
		// do nothing
		break;
	}

}

int triggerReentraint = 0;
int maxTriggerReentraint = 0;
uint32_t triggerDuration;
uint32_t triggerMaxDuration = 0;

/**
 * This function is called by all "hardaware" trigger inputs:
 *  - Hardware triggers
 *  - Trigger replay from CSV (unit tests)
 */
void hwHandleShaftSignal(int signalIndex, bool isRising, efitick_t timestamp DECLARE_ENGINE_PARAMETER_SUFFIX) {
	ScopePerf perf(PE::HandleShaftSignal);
#if VR_HW_CHECK_MODE
	// some boards do not have hardware VR input LEDs which makes such boards harder to validate
	// from experience we know that assembly mistakes happen and quality control is required
	extern ioportid_t criticalErrorLedPort;
	extern ioportmask_t criticalErrorLedPin;

#if HW_CHECK_ALWAYS_STIMULATE
	disableTriggerStimulator();
#endif // HW_CHECK_ALWAYS_STIMULATE


	for (int i = 0 ; i < 100 ; i++) {
		// turning pin ON and busy-waiting a bit
		palWritePad(criticalErrorLedPort, criticalErrorLedPin, 1);
	}

	palWritePad(criticalErrorLedPort, criticalErrorLedPin, 0);
#endif // VR_HW_CHECK_MODE

	handleShaftSignal(signalIndex, isRising, timestamp PASS_ENGINE_PARAMETER_SUFFIX);
}

// Handle all shaft signals - hardware or emulated both
void handleShaftSignal(int signalIndex, bool isRising, efitick_t timestamp DECLARE_ENGINE_PARAMETER_SUFFIX) {
	bool isPrimary = signalIndex == 0;
	if (!isPrimary && !TRIGGER_WAVEFORM(needSecondTriggerInput)) {
		return;
	}

	trigger_event_e signal;
	// todo: add support for 3rd channel
	if (isRising) {
		signal = isPrimary ?
					(engineConfiguration->invertPrimaryTriggerSignal ? SHAFT_PRIMARY_FALLING : SHAFT_PRIMARY_RISING) :
					(engineConfiguration->invertSecondaryTriggerSignal ? SHAFT_SECONDARY_FALLING : SHAFT_SECONDARY_RISING);
	} else {
		signal = isPrimary ?
					(engineConfiguration->invertPrimaryTriggerSignal ? SHAFT_PRIMARY_RISING : SHAFT_PRIMARY_FALLING) :
					(engineConfiguration->invertSecondaryTriggerSignal ? SHAFT_SECONDARY_RISING : SHAFT_SECONDARY_FALLING);
	}

	// Don't accept trigger input in case of some problems
	if (!engine->limpManager.allowTriggerInput()) {
		return;
	}

#if EFI_TOOTH_LOGGER
	// Log to the Tunerstudio tooth logger
	// We want to do this before anything else as we
	// actually want to capture any noise/jitter that may be occurring

	bool logLogicState = CONFIG(displayLogicLevelsInEngineSniffer) && CONFIG(useOnlyRisingEdgeForTrigger);

	if (!logLogicState) {
		// we log physical state even if displayLogicLevelsInEngineSniffer if both fronts are used by decoder
		LogTriggerTooth(signal, timestamp PASS_ENGINE_PARAMETER_SUFFIX);
	}

#endif /* EFI_TOOTH_LOGGER */

	// for effective noise filtering, we need both signal edges, 
	// so we pass them to handleShaftSignal() and defer this test
	if (!CONFIG(useNoiselessTriggerDecoder)) {
		if (!isUsefulSignal(signal, ENGINE(primaryTriggerConfiguration))) {
			/**
			 * no need to process VR falls further
			 */
			return;
		}
	}

	if (CONFIG(triggerInputDebugPins[signalIndex]) != GPIO_UNASSIGNED) {
#if EFI_PROD_CODE
		writePad("trigger debug", CONFIG(triggerInputDebugPins[signalIndex]), 1);
#endif /* EFI_PROD_CODE */
		engine->executor.scheduleByTimestamp("dbg_off", &debugToggleScheduling, timestamp + DEBUG_PIN_DELAY, &turnOffAllDebugFields);
	}

#if EFI_TOOTH_LOGGER
	if (logLogicState) {
		LogTriggerTooth(signal, timestamp PASS_ENGINE_PARAMETER_SUFFIX);
		if (signal == SHAFT_PRIMARY_RISING) {
			LogTriggerTooth(SHAFT_PRIMARY_FALLING, timestamp PASS_ENGINE_PARAMETER_SUFFIX);
		} else {
			LogTriggerTooth(SHAFT_SECONDARY_FALLING, timestamp PASS_ENGINE_PARAMETER_SUFFIX);
		}
	}
#endif /* EFI_TOOTH_LOGGER */

	uint32_t triggerHandlerEntryTime = getTimeNowLowerNt();
	if (triggerReentraint > maxTriggerReentraint)
		maxTriggerReentraint = triggerReentraint;
	triggerReentraint++;

	ENGINE(triggerCentral).handleShaftSignal(signal, timestamp PASS_ENGINE_PARAMETER_SUFFIX);

	triggerReentraint--;
	triggerDuration = getTimeNowLowerNt() - triggerHandlerEntryTime;
	triggerMaxDuration = maxI(triggerMaxDuration, triggerDuration);
}

void TriggerCentral::resetCounters() {
	memset(hwEventCounters, 0, sizeof(hwEventCounters));
}

static char shaft_signal_msg_index[15];

static const bool isUpEvent[6] = { false, true, false, true, false, true };
static const char *eventId[6] = { PROTOCOL_CRANK1, PROTOCOL_CRANK1, PROTOCOL_CRANK2, PROTOCOL_CRANK2, PROTOCOL_CRANK3, PROTOCOL_CRANK3 };

static void reportEventToWaveChart(trigger_event_e ckpSignalType, int index DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (!ENGINE(isEngineChartEnabled)) { // this is here just as a shortcut so that we avoid engine sniffer as soon as possible
		return; // engineSnifferRpmThreshold is accounted for inside ENGINE(isEngineChartEnabled)
	}


	itoa10(&shaft_signal_msg_index[2], index);
	bool isUp = isUpEvent[(int) ckpSignalType];
	shaft_signal_msg_index[0] = isUp ? 'u' : 'd';

	addEngineSnifferEvent(eventId[(int )ckpSignalType], (char* ) shaft_signal_msg_index);
	if (engineConfiguration->useOnlyRisingEdgeForTrigger) {
		// let's add the opposite event right away
		shaft_signal_msg_index[0] = isUp ? 'd' : 'u';
		addEngineSnifferEvent(eventId[(int )ckpSignalType], (char* ) shaft_signal_msg_index);
	}
}

/**
 * This is used to filter noise spikes (interference) in trigger signal. See 
 * The basic idea is to use not just edges, but the average amount of time the signal stays in '0' or '1'.
 * So we update 'accumulated periods' to track where the signal is. 
 * And then compare between the current period and previous, with some tolerance (allowing for the wheel speed change).
 * @return true if the signal is passed through.
 */
bool TriggerNoiseFilter::noiseFilter(efitick_t nowNt,
		TriggerState * triggerState,
		trigger_event_e signal DECLARE_ENGINE_PARAMETER_SUFFIX) {
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
	bool isGapExpected = TRIGGER_WAVEFORM(isSynchronizationNeeded) && triggerState->getShaftSynchronized() &&
			(triggerState->currentCycle.eventCount[ti] + 1) == TRIGGER_WAVEFORM(getExpectedEventCount(ti));
	
	if (isGapExpected) {
		// usually we need to extend the period for gaps, based on the trigger info
		allowedPeriod *= TRIGGER_WAVEFORM(syncRatioAvg);
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

/**
 * This method is NOT invoked for VR falls.
 */
void TriggerCentral::handleShaftSignal(trigger_event_e signal, efitick_t timestamp DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (triggerShape.shapeDefinitionError) {
		// trigger is broken, we cannot do anything here
		warning(CUSTOM_ERR_UNEXPECTED_SHAFT_EVENT, "Shaft event while trigger is mis-configured");
		// magic value to indicate a problem
		hwEventCounters[0] = 155;
		return;
	}

	// This code gathers some statistics on signals and compares accumulated periods to filter interference
	if (CONFIG(useNoiselessTriggerDecoder)) {
		if (!noiseFilter.noiseFilter(timestamp, &triggerState, signal PASS_ENGINE_PARAMETER_SUFFIX)) {
			return;
		}
		if (!isUsefulSignal(signal, ENGINE(primaryTriggerConfiguration))) {
			return;
		}
	}

	engine->onTriggerSignalEvent();

	m_lastEventTimer.reset(timestamp);

	int eventIndex = (int) signal;
	efiAssertVoid(CUSTOM_TRIGGER_EVENT_TYPE, eventIndex >= 0 && eventIndex < HW_EVENT_TYPES, "signal type");
	hwEventCounters[eventIndex]++;


	/**
	 * This invocation changes the state of triggerState
	 */
	triggerState.decodeTriggerEvent(triggerShape,
			nullptr,
			engine,
			engine->primaryTriggerConfiguration,
			signal, timestamp);

	/**
	 * If we only have a crank position sensor with four stroke, here we are extending crank revolutions with a 360 degree
	 * cycle into a four stroke, 720 degrees cycle.
	 */
	operation_mode_e operationMode = engine->getOperationMode(PASS_ENGINE_PARAMETER_SIGNATURE);
	int crankDivider = getCrankDivider(operationMode);
	int crankInternalIndex = triggerState.getTotalRevolutionCounter() % crankDivider;
	int triggerIndexForListeners = triggerState.getCurrentIndex() + (crankInternalIndex * getTriggerSize());
	if (triggerIndexForListeners == 0) {
		virtualZeroTimer.reset(timestamp);
	}
	reportEventToWaveChart(signal, triggerIndexForListeners PASS_ENGINE_PARAMETER_SUFFIX);

	if (!triggerState.getShaftSynchronized()) {
		// we should not propagate event if we do not know where we are
		return;
	}

	if (triggerState.isValidIndex(ENGINE(triggerCentral.triggerShape))) {
		ScopePerf perf(PE::ShaftPositionListeners);

#if TRIGGER_EXTREME_LOGGING
	efiPrintf("trigger %d %d %d", triggerIndexForListeners, getRevolutionCounter(), (int)getTimeNowUs());
#endif /* TRIGGER_EXTREME_LOGGING */

		rpmShaftPositionCallback(signal, triggerIndexForListeners, timestamp PASS_ENGINE_PARAMETER_SUFFIX);

		tdcMarkCallback(triggerIndexForListeners, timestamp PASS_ENGINE_PARAMETER_SUFFIX);

#if !EFI_UNIT_TEST
#if EFI_MAP_AVERAGING
		mapAveragingTriggerCallback(triggerIndexForListeners, timestamp PASS_ENGINE_PARAMETER_SUFFIX);
#endif /* EFI_MAP_AVERAGING */
#endif /* EFI_UNIT_TEST */

#if EFI_LOGIC_ANALYZER
		waTriggerEventListener(signal, triggerIndexForListeners, timestamp PASS_ENGINE_PARAMETER_SUFFIX);
#endif

		mainTriggerCallback(triggerIndexForListeners, timestamp PASS_ENGINE_PARAMETER_SUFFIX);
	}
}

EXTERN_ENGINE;

static void triggerShapeInfo(void) {
#if EFI_PROD_CODE || EFI_SIMULATOR
	TriggerWaveform *shape = &engine->triggerCentral.triggerShape;
	TriggerFormDetails *triggerFormDetails = &engine->triggerCentral.triggerFormDetails;
	efiPrintf("useRise=%s", boolToString(TRIGGER_WAVEFORM(useRiseEdge)));
	efiPrintf("gap from %.2f to %.2f", TRIGGER_WAVEFORM(syncronizationRatioFrom[0]), TRIGGER_WAVEFORM(syncronizationRatioTo[0]));

	for (size_t i = 0; i < shape->getSize(); i++) {
		efiPrintf("event %d %.2f", i, triggerFormDetails->eventAngles[i]);
	}
#endif
}

#if EFI_PROD_CODE
extern PwmConfig triggerSignal;
#endif /* #if EFI_PROD_CODE */

#if HAL_USE_ICU == TRUE
extern int icuRisingCallbackCounter;
extern int icuFallingCallbackCounter;
#endif /* HAL_USE_ICU */

void triggerInfo(void) {
#if EFI_PROD_CODE || EFI_SIMULATOR

	TriggerWaveform *ts = &engine->triggerCentral.triggerShape;


#if (HAL_TRIGGER_USE_PAL == TRUE) && (PAL_USE_CALLBACKS == TRUE)
		efiPrintf("trigger PAL mode %d", engine->hwTriggerInputEnabled);
#else

#if HAL_USE_ICU == TRUE
	efiPrintf("trigger ICU hw: %d %d %d", icuRisingCallbackCounter, icuFallingCallbackCounter, engine->hwTriggerInputEnabled);
#endif /* HAL_USE_ICU */

#endif /* HAL_TRIGGER_USE_PAL */

	efiPrintf("Template %s (%d) trigger %s (%d) useRiseEdge=%s onlyFront=%s useOnlyFirstChannel=%s tdcOffset=%.2f",
			getConfigurationName(engineConfiguration->engineType), engineConfiguration->engineType,
			getTrigger_type_e(engineConfiguration->trigger.type), engineConfiguration->trigger.type,
			boolToString(TRIGGER_WAVEFORM(useRiseEdge)), boolToString(engineConfiguration->useOnlyRisingEdgeForTrigger),
			boolToString(engineConfiguration->trigger.useOnlyFirstChannel), TRIGGER_WAVEFORM(tdcPosition));

	if (engineConfiguration->trigger.type == TT_TOOTHED_WHEEL) {
		efiPrintf("total %d/skipped %d", engineConfiguration->trigger.customTotalToothCount,
				engineConfiguration->trigger.customSkippedToothCount);
	}

	efiPrintf("trigger#1 event counters up=%d/down=%d", engine->triggerCentral.getHwEventCounter(0),
			engine->triggerCentral.getHwEventCounter(1));

	if (ts->needSecondTriggerInput) {
		efiPrintf("trigger#2 event counters up=%d/down=%d", engine->triggerCentral.getHwEventCounter(2),
				engine->triggerCentral.getHwEventCounter(3));
	}
	efiPrintf("expected cycle events %d/%d/%d",
			TRIGGER_WAVEFORM(getExpectedEventCount(0)),
			TRIGGER_WAVEFORM(getExpectedEventCount(1)),
			TRIGGER_WAVEFORM(getExpectedEventCount(2)));

	efiPrintf("trigger type=%d/need2ndChannel=%s", engineConfiguration->trigger.type,
			boolToString(TRIGGER_WAVEFORM(needSecondTriggerInput)));
	efiPrintf("expected duty #0=%.2f/#1=%.2f", TRIGGER_WAVEFORM(expectedDutyCycle[0]), TRIGGER_WAVEFORM(expectedDutyCycle[1]));

	efiPrintf("synchronizationNeeded=%s/isError=%s/total errors=%d ord_err=%d/total revolutions=%d/self=%s",
			boolToString(ts->isSynchronizationNeeded),
			boolToString(isTriggerDecoderError()), engine->triggerCentral.triggerState.totalTriggerErrorCounter,
			engine->triggerCentral.triggerState.orderingErrorCounter, engine->triggerCentral.triggerState.getTotalRevolutionCounter(),
			boolToString(engine->directSelfStimulation));

	if (TRIGGER_WAVEFORM(isSynchronizationNeeded)) {
		efiPrintf("gap from %.2f to %.2f", TRIGGER_WAVEFORM(syncronizationRatioFrom[0]), TRIGGER_WAVEFORM(syncronizationRatioTo[0]));
	}

#endif /* EFI_PROD_CODE || EFI_SIMULATOR */

#if EFI_PROD_CODE
	if (HAVE_CAM_INPUT()) {
		efiPrintf("VVT input: %s mode %s", hwPortname(engineConfiguration->camInputs[0]),
				getVvt_mode_e(engineConfiguration->vvtMode[0]));
		efiPrintf("VVT event counters: %d/%d", engine->triggerCentral.vvtEventRiseCounter, engine->triggerCentral.vvtEventFallCounter);

	}

	efiPrintf("primary trigger input: %s", hwPortname(CONFIG(triggerInputPins)[0]));
	efiPrintf("primary trigger simulator: %s %s freq=%d",
			hwPortname(CONFIG(triggerSimulatorPins)[0]),
			getPin_output_mode_e(CONFIG(triggerSimulatorPinModes)[0]),
			CONFIG(triggerSimulatorFrequency));

	if (ts->needSecondTriggerInput) {
		efiPrintf("secondary trigger input: %s", hwPortname(CONFIG(triggerInputPins)[1]));
#if EFI_EMULATE_POSITION_SENSORS
		efiPrintf("secondary trigger simulator: %s %s phase=%d",
				hwPortname(CONFIG(triggerSimulatorPins)[1]),
				getPin_output_mode_e(CONFIG(triggerSimulatorPinModes)[1]), triggerSignal.safe.phaseIndex);
#endif /* EFI_EMULATE_POSITION_SENSORS */
	}
//	efiPrintf("3rd trigger simulator: %s %s", hwPortname(CONFIG(triggerSimulatorPins)[2]),
//			getPin_output_mode_e(CONFIG(triggerSimulatorPinModes)[2]));

	efiPrintf("trigger error extra LED: %s %s", hwPortname(CONFIG(triggerErrorPin)),
			getPin_output_mode_e(CONFIG(triggerErrorPinMode)));
	efiPrintf("primary logic input: %s", hwPortname(CONFIG(logicAnalyzerPins)[0]));
	efiPrintf("secondary logic input: %s", hwPortname(CONFIG(logicAnalyzerPins)[1]));


	efiPrintf("totalTriggerHandlerMaxTime=%d", triggerMaxDuration);

#endif /* EFI_PROD_CODE */
}

static void resetRunningTriggerCounters() {
#if !EFI_UNIT_TEST
	engine->triggerCentral.resetCounters();
	triggerInfo();
#endif
}

void onConfigurationChangeTriggerCallback(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	bool changed = false;
	// todo: how do we static_assert here?
	efiAssertVoid(OBD_PCM_Processor_Fault, efi::size(CONFIG(camInputs)) == efi::size(CONFIG(vvtOffsets)), "sizes");

	for (size_t camIndex = 0; camIndex < efi::size(CONFIG(camInputs)); camIndex++) {
		changed |= isConfigurationChanged(camInputs[camIndex]);
		changed |= isConfigurationChanged(vvtOffsets[camIndex]);
	}

	for (size_t i = 0; i < efi::size(CONFIG(triggerGapOverride)); i++) {
		changed |= isConfigurationChanged(triggerGapOverride[i]);
	}

	for (size_t i = 0; i < efi::size(CONFIG(triggerInputPins)); i++) {
		changed |= isConfigurationChanged(triggerInputPins[i]);
	}

	for (size_t i = 0; i < efi::size(CONFIG(vvtMode)); i++) {
		changed |= isConfigurationChanged(vvtMode[i]);
	}

	changed |= isConfigurationChanged(trigger.type);
	changed |= isConfigurationChanged(ambiguousOperationMode);
	changed |= isConfigurationChanged(useOnlyRisingEdgeForTrigger);
	changed |= isConfigurationChanged(globalTriggerAngleOffset);
	changed |= isConfigurationChanged(trigger.customTotalToothCount);
	changed |= isConfigurationChanged(trigger.customSkippedToothCount);
	changed |= isConfigurationChanged(vvtCamSensorUseRise);
	changed |= isConfigurationChanged(overrideTriggerGaps);

	if (changed) {
		assertEngineReference();

	#if EFI_ENGINE_CONTROL
		ENGINE(initializeTriggerWaveform(PASS_ENGINE_PARAMETER_SIGNATURE));
		engine->triggerCentral.noiseFilter.resetAccumSignalData();
	#endif
	}
#if EFI_DEFAILED_LOGGING
	efiPrintf("isTriggerConfigChanged=%d", engine->isTriggerConfigChanged);
#endif /* EFI_DEFAILED_LOGGING */

	// we do not want to miss two updates in a row
	engine->isTriggerConfigChanged = engine->isTriggerConfigChanged || changed;
}

/**
 * @returns true if configuration just changed, and if that change has affected trigger
 */
bool checkIfTriggerConfigChanged(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	bool result = engine->triggerVersion.isOld(engine->getGlobalConfigurationVersion()) && engine->isTriggerConfigChanged;
	engine->isTriggerConfigChanged = false; // whoever has called the method is supposed to react to changes
	return result;
}

bool isTriggerConfigChanged(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return engine->isTriggerConfigChanged;
}

void initTriggerCentral() {
	strcpy((char*) shaft_signal_msg_index, "x_");

#if EFI_ENGINE_SNIFFER
	initWaveChart(&waveChart);
#endif /* EFI_ENGINE_SNIFFER */

#if EFI_PROD_CODE || EFI_SIMULATOR
	addConsoleAction(CMD_TRIGGERINFO, triggerInfo);
	addConsoleAction("trigger_shape_info", triggerShapeInfo);
	addConsoleAction("reset_trigger", resetRunningTriggerCounters);
#endif // EFI_PROD_CODE || EFI_SIMULATOR

}

/**
 * @return TRUE is something is wrong with trigger decoding
 */
bool isTriggerDecoderError(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return engine->triggerErrorDetection.sum(6) > 4;
}

#endif // EFI_SHAFT_POSITION_INPUT
