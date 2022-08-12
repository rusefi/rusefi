/*
 * @file	trigger_central.cpp
 * Here we have a bunch of higher-level methods which are not directly related to actual signal decoding
 *
 * @date Feb 23, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "os_access.h"

#include "trigger_central.h"
#include "trigger_decoder.h"
#include "main_trigger_callback.h"
#include "listener_array.h"
#include "tooth_logger.h"
#include "hip9011.h"
#include "logic_analyzer.h"

#include "local_version_holder.h"
#include "trigger_simulator.h"
#include "trigger_emulator_algo.h"

#include "tooth_logger.h"
#include "map_averaging.h"
#include "main_trigger_callback.h"
#include "status_loop.h"

#if EFI_TUNER_STUDIO
#include "tunerstudio.h"
#endif /* EFI_TUNER_STUDIO */

#if EFI_ENGINE_SNIFFER
#include "engine_sniffer.h"
WaveChart waveChart;
#endif /* EFI_ENGINE_SNIFFER */

static scheduling_s debugToggleScheduling;
#define DEBUG_PIN_DELAY US2NT(60)

#if EFI_SHAFT_POSITION_INPUT

TriggerCentral::TriggerCentral() :
		vvtEventRiseCounter(),
		vvtEventFallCounter(),
		vvtPosition(),
		triggerState("TRG")
{
	memset(&hwEventCounters, 0, sizeof(hwEventCounters));
	triggerState.resetTriggerState();
	noiseFilter.resetAccumSignalData();
}

void TriggerNoiseFilter::resetAccumSignalData() {
	memset(lastSignalTimes, 0xff, sizeof(lastSignalTimes));	// = -1
	memset(accumSignalPeriods, 0, sizeof(accumSignalPeriods));
	memset(accumSignalPrevPeriods, 0, sizeof(accumSignalPrevPeriods));
}

int TriggerCentral::getHwEventCounter(int index) const {
	return hwEventCounters[index];
}


angle_t TriggerCentral::getVVTPosition(uint8_t bankIndex, uint8_t camIndex) {
	if (bankIndex >= BANKS_COUNT || camIndex >= CAMS_PER_BANK) {
		return NAN;
	}
	return vvtPosition[bankIndex][camIndex];
}

/**
 * @return angle since trigger synchronization point, NOT angle since TDC.
 */
expected<float> TriggerCentral::getCurrentEnginePhase(efitick_t nowNt) const {
	floatus_t oneDegreeUs = engine->rpmCalculator.oneDegreeUs;

	if (cisnan(oneDegreeUs)) {
		return unexpected;
	}

	float elapsed;
	float toothPhase;

	{
		// under lock to avoid mismatched tooth phase and time
		chibios_rt::CriticalSectionLocker csl;

		elapsed = m_lastToothTimer.getElapsedUs(nowNt);
		toothPhase = m_lastToothPhaseFromSyncPoint;
	}

	return toothPhase + elapsed / oneDegreeUs;
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
	case FOUR_STROKE_TWELVE_TIMES_CRANK_SENSOR:
		return SYMMETRICAL_TWELVE_TIMES_CRANK_SENSOR_DIVIDER;
	default:
	case FOUR_STROKE_CAM_SENSOR:
	case TWO_STROKE:
		// That's easy - trigger cycle matches engine cycle
		return 1;
	}
}

static bool vvtWithRealDecoder(vvt_mode_e vvtMode) {
	// todo: why does VVT_2JZ not use real decoder?
	return vvtMode != VVT_INACTIVE
			&& vvtMode != VVT_2JZ
			&& vvtMode != VVT_HONDA_K
			&& vvtMode != VVT_MAP_V_TWIN_ANOTHER
			&& vvtMode != VVT_SECOND_HALF
			&& vvtMode != VVT_FIRST_HALF;
}

static angle_t syncAndReport(TriggerCentral *tc, int divider, int remainder) {
	angle_t engineCycle = getEngineCycle(engine->getOperationMode());

	return tc->triggerState.syncEnginePhase(divider, remainder, engineCycle);
}

static void turnOffAllDebugFields(void *arg) {
	(void)arg;
#if EFI_PROD_CODE
	for (int index = 0;index<TRIGGER_INPUT_PIN_COUNT;index++) {
		if (engineConfiguration->triggerInputDebugPins[index] != Gpio::Unassigned) {
			writePad("trigger debug", engineConfiguration->triggerInputDebugPins[index], 0);
		}
	}
	for (int index = 0;index<CAM_INPUTS_COUNT;index++) {
		if (engineConfiguration->camInputsDebug[index] != Gpio::Unassigned) {
			writePad("cam debug", engineConfiguration->camInputsDebug[index], 0);
		}
	}
#endif /* EFI_PROD_CODE */
}

static angle_t adjustCrankPhase(int camIndex) {
	float maxSyncThreshold = engineConfiguration->maxCamPhaseResolveRpm;
	if (maxSyncThreshold != 0 && Sensor::getOrZero(SensorType::Rpm) > maxSyncThreshold) {
		// The user has elected to stop trying to resolve crank phase after some RPM.
		// Maybe their cam sensor only works at low RPM or something.
		// Anyway, don't try to change crank phase at all, and return that we made no change.
		return 0;
	}

	TriggerCentral *tc = &engine->triggerCentral;
	operation_mode_e operationMode = engine->getOperationMode();

	vvt_mode_e vvtMode = engineConfiguration->vvtMode[camIndex];
	switch (vvtMode) {
	case VVT_FIRST_HALF:
	case VVT_MAP_V_TWIN_ANOTHER:
		return syncAndReport(tc, getCrankDivider(operationMode), 1);
	case VVT_SECOND_HALF:
	case VVT_NISSAN_VQ:
	case VVT_BOSCH_QUICK_START:
		return syncAndReport(tc, getCrankDivider(operationMode), 0);
	case VVT_MIATA_NB:
		/**
		 * NB2 is a symmetrical crank, there are four phases total
		 */
		return syncAndReport(tc, getCrankDivider(operationMode), 0);
	case VVT_2JZ:
	case VVT_TOYOTA_4_1:
	case VVT_FORD_ST170:
	case VVT_BARRA_3_PLUS_1:
	case VVT_NISSAN_MR:
	case VVT_MITSUBISHI_3A92:
	case VVT_MITSUBISHI_6G75:
		return syncAndReport(tc, getCrankDivider(operationMode), engineConfiguration->tempBooleanForVerySpecialCases ? 1 : 0);
	case VVT_HONDA_K:
		firmwareError(OBD_PCM_Processor_Fault, "Undecided on VVT phase of %s", getVvt_mode_e(vvtMode));
		return 0;
	case VVT_INACTIVE:
		// do nothing
		return 0;
	}
	return 0;
}

/**
 * See also wrapAngle
 */
static angle_t wrapVvt(angle_t vvtPosition, int period) {
	// Wrap VVT position in to the range [-360, 360)
	while (vvtPosition < -period / 2) {
		vvtPosition += period;
	}
	while (vvtPosition >= period / 2) {
		vvtPosition -= period;
	}
	return vvtPosition;
}

static void logFront(bool isImportantFront, efitick_t nowNt, int index) {
	extern const char *vvtNames[];
	const char *vvtName = vvtNames[index];

	if (isImportantFront && engineConfiguration->camInputsDebug[index] != Gpio::Unassigned) {
#if EFI_PROD_CODE
		writePad("cam debug", engineConfiguration->camInputsDebug[index], 1);
#endif /* EFI_PROD_CODE */
		engine->executor.scheduleByTimestampNt("dbg_on", &debugToggleScheduling, nowNt + DEBUG_PIN_DELAY, &turnOffAllDebugFields);
	}

	if (engineConfiguration->displayLogicLevelsInEngineSniffer && isImportantFront) {
		if (engineConfiguration->vvtCamSensorUseRise) {
			// todo: unify TS composite logger code with console Engine Sniffer
			// todo: better API to reduce copy/paste?
#if EFI_TOOTH_LOGGER
			LogTriggerTooth(SHAFT_SECONDARY_RISING, nowNt);
			LogTriggerTooth(SHAFT_SECONDARY_FALLING, nowNt);
#endif /* EFI_TOOTH_LOGGER */
			addEngineSnifferEvent(vvtName, PROTOCOL_ES_UP);
			addEngineSnifferEvent(vvtName, PROTOCOL_ES_DOWN);
		} else {
#if EFI_TOOTH_LOGGER
			LogTriggerTooth(SHAFT_SECONDARY_FALLING, nowNt);
			LogTriggerTooth(SHAFT_SECONDARY_RISING, nowNt);
#endif /* EFI_TOOTH_LOGGER */

			addEngineSnifferEvent(vvtName, PROTOCOL_ES_DOWN);
			addEngineSnifferEvent(vvtName, PROTOCOL_ES_UP);
		}
	}
}

void hwHandleVvtCamSignal(trigger_value_e front, efitick_t nowNt, int index) {
	if (engine->directSelfStimulation || !engine->hwTriggerInputEnabled) {
		// sensor noise + self-stim = loss of trigger sync
		return;
	}

	int bankIndex = index / CAMS_PER_BANK;
	int camIndex = index % CAMS_PER_BANK;
	TriggerCentral *tc = &engine->triggerCentral;
	if (front == TV_RISE) {
		tc->vvtEventRiseCounter[index]++;
	} else {
		tc->vvtEventFallCounter[index]++;
	}
	extern const char *vvtNames[];
	const char *vvtName = vvtNames[index];
	if (engineConfiguration->vvtMode[camIndex] == VVT_INACTIVE) {
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

	if (!engineConfiguration->displayLogicLevelsInEngineSniffer) {
		addEngineSnifferEvent(vvtName, front == TV_RISE ? PROTOCOL_ES_UP : PROTOCOL_ES_DOWN);

#if EFI_TOOTH_LOGGER
// todo: we need to start logging different VVT channels differently!!!
		trigger_event_e tooth = front == TV_RISE ? SHAFT_SECONDARY_RISING : SHAFT_SECONDARY_FALLING;

		LogTriggerTooth(tooth, nowNt);
#endif /* EFI_TOOTH_LOGGER */
	}


	bool isImportantFront = (engineConfiguration->vvtCamSensorUseRise ^ (front == TV_FALL));
	bool isVvtWithRealDecoder = vvtWithRealDecoder(engineConfiguration->vvtMode[camIndex]);
	if (!isVvtWithRealDecoder && !isImportantFront) {
		// todo: there should be a way to always use real trigger code for this logic?
		return;
	}

	logFront(isImportantFront, nowNt, index);

	// If the main trigger is not synchronized, don't decode VVT yet
	if (!tc->triggerState.getShaftSynchronized()) {
		return;
	}

	TriggerDecoderBase& vvtDecoder = tc->vvtState[bankIndex][camIndex];

	if (isVvtWithRealDecoder) {
		vvtDecoder.decodeTriggerEvent(
				"vvt",
			tc->vvtShape[camIndex],
			nullptr,
			engine->vvtTriggerConfiguration[camIndex],
			front == TV_RISE ? SHAFT_PRIMARY_RISING : SHAFT_PRIMARY_FALLING, nowNt);
		// yes we log data from all VVT channels into same fields for now
		tc->triggerState.vvtSyncGapRatio = vvtDecoder.triggerSyncGapRatio;
		tc->triggerState.vvtStateIndex = vvtDecoder.currentCycle.current_index;
	}

	tc->vvtCamCounter++;

	auto currentPhase = tc->getCurrentEnginePhase(nowNt);
	if (!currentPhase) {
		// If we couldn't resolve engine speed (yet primary trigger is sync'd), this
		// probably means that we have partial crank sync, but not RPM information yet
		return;
	}

	angle_t angleFromPrimarySyncPoint = currentPhase.Value;
	// convert trigger cycle angle into engine cycle angle
	angle_t currentPosition = angleFromPrimarySyncPoint - tdcPosition();
	// https://github.com/rusefi/rusefi/issues/1713 currentPosition could be negative that's expected

#if EFI_UNIT_TEST
	tc->currentVVTEventPosition[bankIndex][camIndex] = currentPosition;
#endif // EFI_UNIT_TEST

	tc->triggerState.vvtCurrentPosition = currentPosition;

	if (isVvtWithRealDecoder && vvtDecoder.currentCycle.current_index != 0) {
		// this is not sync tooth - exiting
		return;
	}

	switch(engineConfiguration->vvtMode[camIndex]) {
	case VVT_2JZ:
		// we do not know if we are in sync or out of sync, so we have to be looking for both possibilities
		if ((currentPosition < engineConfiguration->scriptSetting[4]       || currentPosition > engineConfiguration->scriptSetting[5]) &&
		    (currentPosition < engineConfiguration->scriptSetting[4] + 360 || currentPosition > engineConfiguration->scriptSetting[5] + 360)) {
			// outside of the expected range
			return;
		}
		break;
	default:
		// else, do nothing
		break;
	}

	tc->triggerState.vvtCounter++;

	auto vvtPosition = engineConfiguration->vvtOffsets[bankIndex * CAMS_PER_BANK + camIndex] - currentPosition;

	if (index != 0) {
		// todo: only assign initial position of not first cam once cam was synchronized
		tc->vvtPosition[bankIndex][camIndex] = wrapVvt(vvtPosition, FOUR_STROKE_CYCLE_DURATION);
		// at the moment we use only primary VVT to sync crank phase
		return;
	}

	angle_t crankOffset = adjustCrankPhase(camIndex);
	// vvtPosition was calculated against wrong crank zero position. Now that we have adjusted crank position we
	// shall adjust vvt position as well
	vvtPosition -= crankOffset;
	vvtPosition = wrapVvt(vvtPosition, FOUR_STROKE_CYCLE_DURATION);

	// this could be just an 'if' but let's have it expandable for future use :)
	switch(engineConfiguration->vvtMode[camIndex]) {
	case VVT_HONDA_K:
		// honda K has four tooth in VVT intake trigger, so we just wrap each of those to 720 / 4
		vvtPosition = wrapVvt(vvtPosition, 180);
		break;
	default:
		// else, do nothing
		break;
	}

	if (absF(angleFromPrimarySyncPoint) < 7) {
		/**
		 * we prefer not to have VVT sync right at trigger sync so that we do not have phase detection error if things happen a bit in
		 * wrong order due to belt flex or else
		 * https://github.com/rusefi/rusefi/issues/3269
		 */
		warning(CUSTOM_VVT_SYNC_POSITION, "VVT sync position too close to trigger sync");
	}

	tc->vvtPosition[bankIndex][camIndex] = vvtPosition;
}

int triggerReentrant = 0;
int maxTriggerReentrant = 0;
uint32_t triggerDuration;
uint32_t triggerMaxDuration = 0;

/**
 * This function is called by all "hardaware" trigger inputs:
 *  - Hardware triggers
 *  - Trigger replay from CSV (unit tests)
 */
void hwHandleShaftSignal(int signalIndex, bool isRising, efitick_t timestamp) {
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

	if (engine->directSelfStimulation || !engine->hwTriggerInputEnabled) {
		// sensor noise + self-stim = loss of trigger sync
		return;
	}

	handleShaftSignal(signalIndex, isRising, timestamp);
}

// Handle all shaft signals - hardware or emulated both
void handleShaftSignal(int signalIndex, bool isRising, efitick_t timestamp) {
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

	bool logLogicState = engineConfiguration->displayLogicLevelsInEngineSniffer && engineConfiguration->useOnlyRisingEdgeForTrigger;

	if (!logLogicState) {
		// we log physical state even if displayLogicLevelsInEngineSniffer if both fronts are used by decoder
		LogTriggerTooth(signal, timestamp);
	}

#endif /* EFI_TOOTH_LOGGER */

	// for effective noise filtering, we need both signal edges, 
	// so we pass them to handleShaftSignal() and defer this test
	if (!engineConfiguration->useNoiselessTriggerDecoder) {
		if (!isUsefulSignal(signal, engine->primaryTriggerConfiguration)) {
			/**
			 * no need to process VR falls further
			 */
			return;
		}
	}

	if (engineConfiguration->triggerInputDebugPins[signalIndex] != Gpio::Unassigned) {
#if EFI_PROD_CODE
		writePad("trigger debug", engineConfiguration->triggerInputDebugPins[signalIndex], 1);
#endif /* EFI_PROD_CODE */
		engine->executor.scheduleByTimestampNt("dbg_off", &debugToggleScheduling, timestamp + DEBUG_PIN_DELAY, &turnOffAllDebugFields);
	}

#if EFI_TOOTH_LOGGER
	if (logLogicState) {
		// first log rising normally
		LogTriggerTooth(signal, timestamp);
		// in 'logLogicState' mode we log opposite front right after logical rising away
		if (signal == SHAFT_PRIMARY_RISING) {
			LogTriggerTooth(SHAFT_PRIMARY_FALLING, timestamp);
		} else {
			LogTriggerTooth(SHAFT_SECONDARY_FALLING, timestamp);
		}
	}
#endif /* EFI_TOOTH_LOGGER */

	uint32_t triggerHandlerEntryTime = getTimeNowLowerNt();
	if (triggerReentrant > maxTriggerReentrant)
		maxTriggerReentrant = triggerReentrant;
	triggerReentrant++;

	engine->triggerCentral.handleShaftSignal(signal, timestamp);

	triggerReentrant--;
	triggerDuration = getTimeNowLowerNt() - triggerHandlerEntryTime;
	triggerMaxDuration = maxI(triggerMaxDuration, triggerDuration);
}

void TriggerCentral::resetCounters() {
	memset(hwEventCounters, 0, sizeof(hwEventCounters));
}

static char shaft_signal_msg_index[15];

static const bool isUpEvent[6] = { false, true, false, true, false, true };
static const char *eventId[6] = { PROTOCOL_CRANK1, PROTOCOL_CRANK1, PROTOCOL_CRANK2, PROTOCOL_CRANK2, PROTOCOL_CRANK3, PROTOCOL_CRANK3 };

static void reportEventToWaveChart(trigger_event_e ckpSignalType, int index) {
	if (!engine->isEngineSnifferEnabled) { // this is here just as a shortcut so that we avoid engine sniffer as soon as possible
		return; // engineSnifferRpmThreshold is accounted for inside engine->isEngineSnifferEnabled
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
		TriggerDecoderBase * triggerState,
		trigger_event_e signal) {
	// todo: find a better place for these defs
	static const trigger_event_e opposite[4] = { SHAFT_PRIMARY_RISING, SHAFT_PRIMARY_FALLING, SHAFT_SECONDARY_RISING, SHAFT_SECONDARY_FALLING };
	static const trigger_wheel_e triggerIdx[4] = { T_PRIMARY, T_PRIMARY, T_SECONDARY, T_SECONDARY };
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

void TriggerCentral::decodeMapCam(efitick_t timestamp, float currentPhase) {
#if WITH_TS_STATE
	if (engineConfiguration->vvtMode[0] == VVT_MAP_V_TWIN_ANOTHER &&
			Sensor::getOrZero(SensorType::Rpm) < engineConfiguration->cranking.rpm) {
		// we are trying to figure out which 360 half of the total 720 degree cycle is which, so we compare those in 360 degree sense.
		auto toothAngle360 = currentPhase;
		while (toothAngle360 >= 360) {
			toothAngle360 -= 360;
		}

		if (mapCamPrevToothAngle < engineConfiguration->mapCamDetectionAnglePosition && toothAngle360 > engineConfiguration->mapCamDetectionAnglePosition) {
			// we are somewhere close to 'mapCamDetectionAnglePosition'

			// warning: hack hack hack
			float map = engine->outputChannels.instantMAPValue;

			// Compute diff against the last time we were here
			float diff = map - mapCamPrevCycleValue;
			mapCamPrevCycleValue = map;

			if (diff > 0) {
				mapVvt_map_peak++;
				int revolutionCounter = engine->triggerCentral.triggerState.getTotalRevolutionCounter();
				mapVvt_MAP_AT_CYCLE_COUNT = revolutionCounter - prevChangeAtCycle;
				prevChangeAtCycle = revolutionCounter;

				hwHandleVvtCamSignal(TV_RISE, timestamp, /*index*/0);
				hwHandleVvtCamSignal(TV_FALL, timestamp, /*index*/0);
#if EFI_UNIT_TEST
				// hack? feature? existing unit test relies on VVT phase available right away
				// but current implementation which is based on periodicFastCallback would only make result available on NEXT tooth
				int rpm = Sensor::getOrZero(SensorType::Rpm);
				efitick_t nowNt = getTimeNowNt();
				engine->limpManager.updateState(rpm, nowNt);
#endif // EFI_UNIT_TEST
			}

			mapVvt_MAP_AT_SPECIAL_POINT = map;
			mapVvt_MAP_AT_DIFF = diff;
		}

		mapCamPrevToothAngle = toothAngle360;
	}
#endif // WITH_TS_STATE
}

/**
 * This method is NOT invoked for VR falls.
 */
void TriggerCentral::handleShaftSignal(trigger_event_e signal, efitick_t timestamp) {
	if (triggerShape.shapeDefinitionError) {
		// trigger is broken, we cannot do anything here
		warning(CUSTOM_ERR_UNEXPECTED_SHAFT_EVENT, "Shaft event while trigger is mis-configured");
		// magic value to indicate a problem
		hwEventCounters[0] = 155;
		return;
	}

	// This code gathers some statistics on signals and compares accumulated periods to filter interference
	if (engineConfiguration->useNoiselessTriggerDecoder) {
		if (!noiseFilter.noiseFilter(timestamp, &triggerState, signal)) {
			return;
		}
		if (!isUsefulSignal(signal, engine->primaryTriggerConfiguration)) {
			return;
		}
	}

	engine->onTriggerSignalEvent();

	m_lastEventTimer.reset(timestamp);

	int eventIndex = (int) signal;
	efiAssertVoid(CUSTOM_TRIGGER_EVENT_TYPE, eventIndex >= 0 && eventIndex < HW_EVENT_TYPES, "signal type");
	hwEventCounters[eventIndex]++;

	// Decode the trigger!
	auto decodeResult = triggerState.decodeTriggerEvent(
			"trigger",
			triggerShape,
			engine,
			engine->primaryTriggerConfiguration,
			signal, timestamp);

	// Don't propagate state if we don't know where we are
	if (decodeResult) {
		ScopePerf perf(PE::ShaftPositionListeners);

		/**
		 * If we only have a crank position sensor with four stroke, here we are extending crank revolutions with a 360 degree
		 * cycle into a four stroke, 720 degrees cycle.
		 */
		int crankDivider = getCrankDivider(triggerShape.getOperationMode());
		int crankInternalIndex = triggerState.getTotalRevolutionCounter() % crankDivider;
		int triggerIndexForListeners = decodeResult.Value.CurrentIndex + (crankInternalIndex * triggerShape.getSize());

		reportEventToWaveChart(signal, triggerIndexForListeners);

		// Look up this tooth's angle from the sync point. If this tooth is the sync point, we'll get 0 here.
		auto currentPhaseFromSyncPoint = engine->triggerCentral.triggerFormDetails.eventAngles[triggerIndexForListeners];

		// Adjust so currentPhase is in engine-space angle, not trigger-space angle
		auto currentPhase = currentPhaseFromSyncPoint - tdcPosition();
		wrapAngle(currentPhase, "currentEnginePhase", CUSTOM_ERR_6555);
#if EFI_TUNER_STUDIO
		engine->outputChannels.currentEnginePhase = currentPhase;
#endif // EFI_TUNER_STUDIO

		// Record precise time and phase of the engine. This is used for VVT decode.
		{
			// under lock to avoid mismatched tooth phase and time
			chibios_rt::CriticalSectionLocker csl;

			m_lastToothTimer.reset(timestamp);
			m_lastToothPhaseFromSyncPoint = currentPhaseFromSyncPoint;
		}

#if TRIGGER_EXTREME_LOGGING
	efiPrintf("trigger %d %d %d", triggerIndexForListeners, getRevolutionCounter(), (int)getTimeNowUs());
#endif /* TRIGGER_EXTREME_LOGGING */

		// Update engine RPM
		rpmShaftPositionCallback(signal, triggerIndexForListeners, timestamp);

		// Schedule the TDC mark
		tdcMarkCallback(triggerIndexForListeners, timestamp);

#if !EFI_UNIT_TEST
#if EFI_MAP_AVERAGING
		mapAveragingTriggerCallback(triggerIndexForListeners, timestamp);
#endif /* EFI_MAP_AVERAGING */
#endif /* EFI_UNIT_TEST */

#if EFI_LOGIC_ANALYZER
		waTriggerEventListener(signal, triggerIndexForListeners, timestamp);
#endif

		// TODO: is this logic to compute next trigger tooth angle correct?
		auto nextToothIndex = triggerIndexForListeners;
		float nextPhase = 0;

		do {
			// I don't love this.
			nextToothIndex = (nextToothIndex + 1) % engine->engineCycleEventCount;
			nextPhase = engine->triggerCentral.triggerFormDetails.eventAngles[nextToothIndex] - tdcPosition();
			wrapAngle(nextPhase, "nextEnginePhase", CUSTOM_ERR_6555);
		} while (nextPhase == currentPhase);

		// Handle ignition and injection
		mainTriggerCallback(triggerIndexForListeners, timestamp, currentPhase, nextPhase);

		// Decode the MAP based "cam" sensor
		decodeMapCam(timestamp, currentPhase);
	} else {
		// We don't have sync, but report to the wave chart anyway as index 0.
		reportEventToWaveChart(signal, 0);
	}
}

static void triggerShapeInfo() {
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

void triggerInfo(void) {
#if EFI_PROD_CODE || EFI_SIMULATOR

	TriggerWaveform *ts = &engine->triggerCentral.triggerShape;


#if (HAL_TRIGGER_USE_PAL == TRUE) && (PAL_USE_CALLBACKS == TRUE)
		efiPrintf("trigger PAL mode %d", engine->hwTriggerInputEnabled);
#else

#endif /* HAL_TRIGGER_USE_PAL */

	efiPrintf("Template %s (%d) trigger %s (%d) useRiseEdge=%s onlyFront=%s tdcOffset=%.2f",
			getEngine_type_e(engineConfiguration->engineType), engineConfiguration->engineType,
			getTrigger_type_e(engineConfiguration->trigger.type), engineConfiguration->trigger.type,
			boolToString(TRIGGER_WAVEFORM(useRiseEdge)), boolToString(engineConfiguration->useOnlyRisingEdgeForTrigger),
			TRIGGER_WAVEFORM(tdcPosition));

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

	efiPrintf("synchronizationNeeded=%s/isError=%s/total errors=%d ord_err=%d/total revolutions=%d/self=%s",
			boolToString(ts->isSynchronizationNeeded),
			boolToString(engine->triggerCentral.isTriggerDecoderError()),
			engine->triggerCentral.triggerState.totalTriggerErrorCounter,
			engine->triggerCentral.triggerState.orderingErrorCounter,
			engine->triggerCentral.triggerState.getTotalRevolutionCounter(),
			boolToString(engine->directSelfStimulation));

	if (TRIGGER_WAVEFORM(isSynchronizationNeeded)) {
		efiPrintf("gap from %.2f to %.2f", TRIGGER_WAVEFORM(syncronizationRatioFrom[0]), TRIGGER_WAVEFORM(syncronizationRatioTo[0]));
	}

#endif /* EFI_PROD_CODE || EFI_SIMULATOR */

#if EFI_PROD_CODE

	efiPrintf("primary trigger input: %s", hwPortname(engineConfiguration->triggerInputPins[0]));
	efiPrintf("primary trigger simulator: %s %s freq=%d",
			hwPortname(engineConfiguration->triggerSimulatorPins[0]),
			getPin_output_mode_e(engineConfiguration->triggerSimulatorPinModes[0]),
			engineConfiguration->triggerSimulatorFrequency);

	if (ts->needSecondTriggerInput) {
		efiPrintf("secondary trigger input: %s", hwPortname(engineConfiguration->triggerInputPins[1]));
#if EFI_EMULATE_POSITION_SENSORS
		efiPrintf("secondary trigger simulator: %s %s phase=%d",
				hwPortname(engineConfiguration->triggerSimulatorPins[1]),
				getPin_output_mode_e(engineConfiguration->triggerSimulatorPinModes[1]), triggerSignal.safe.phaseIndex);
#endif /* EFI_EMULATE_POSITION_SENSORS */
	}


	for (int camInputIndex = 0; camInputIndex<CAM_INPUTS_COUNT;camInputIndex++) {
		if (isBrainPinValid(engineConfiguration->camInputs[camInputIndex])) {
			int camLogicalIndex = camInputIndex % CAMS_PER_BANK;
			efiPrintf("VVT input: %s mode %s", hwPortname(engineConfiguration->camInputs[camInputIndex]),
					getVvt_mode_e(engineConfiguration->vvtMode[camLogicalIndex]));
			efiPrintf("VVT %d event counters: %d/%d",
					camInputIndex,
					engine->triggerCentral.vvtEventRiseCounter[camInputIndex], engine->triggerCentral.vvtEventFallCounter[camInputIndex]);
		}
	}


//	efiPrintf("3rd trigger simulator: %s %s", hwPortname(engineConfiguration->triggerSimulatorPins[2]),
//			getPin_output_mode_e(engineConfiguration->triggerSimulatorPinModes[2]));

	efiPrintf("trigger error extra LED: %s %s", hwPortname(engineConfiguration->triggerErrorPin),
			getPin_output_mode_e(engineConfiguration->triggerErrorPinMode));
	efiPrintf("primary logic input: %s", hwPortname(engineConfiguration->logicAnalyzerPins[0]));
	efiPrintf("secondary logic input: %s", hwPortname(engineConfiguration->logicAnalyzerPins[1]));


	efiPrintf("totalTriggerHandlerMaxTime=%d", triggerMaxDuration);

#endif /* EFI_PROD_CODE */

#if EFI_ENGINE_SNIFFER
	efiPrintf("engine sniffer current size=%d", waveChart.getSize());
#endif /* EFI_ENGINE_SNIFFER */

}

static void resetRunningTriggerCounters() {
#if !EFI_UNIT_TEST
	engine->triggerCentral.resetCounters();
	triggerInfo();
#endif
}

void onConfigurationChangeTriggerCallback() {
	bool changed = false;
	// todo: how do we static_assert here?
	efiAssertVoid(OBD_PCM_Processor_Fault, efi::size(engineConfiguration->camInputs) == efi::size(engineConfiguration->vvtOffsets), "sizes");

	for (size_t camIndex = 0; camIndex < efi::size(engineConfiguration->camInputs); camIndex++) {
		changed |= isConfigurationChanged(camInputs[camIndex]);
		changed |= isConfigurationChanged(vvtOffsets[camIndex]);
	}

	for (size_t i = 0; i < efi::size(engineConfiguration->triggerGapOverrideFrom); i++) {
		changed |= isConfigurationChanged(triggerGapOverrideFrom[i]);
		changed |= isConfigurationChanged(triggerGapOverrideTo[i]);
	}

	for (size_t i = 0; i < efi::size(engineConfiguration->triggerInputPins); i++) {
		changed |= isConfigurationChanged(triggerInputPins[i]);
	}

	for (size_t i = 0; i < efi::size(engineConfiguration->vvtMode); i++) {
		changed |= isConfigurationChanged(vvtMode[i]);
	}

	changed |= isConfigurationChanged(trigger.type);
	changed |= isConfigurationChanged(skippedWheelOnCam);
	changed |= isConfigurationChanged(twoStroke);
	changed |= isConfigurationChanged(useOnlyRisingEdgeForTrigger);
	changed |= isConfigurationChanged(globalTriggerAngleOffset);
	changed |= isConfigurationChanged(trigger.customTotalToothCount);
	changed |= isConfigurationChanged(trigger.customSkippedToothCount);
	changed |= isConfigurationChanged(vvtCamSensorUseRise);
	changed |= isConfigurationChanged(overrideTriggerGaps);

	if (changed) {
	#if EFI_ENGINE_CONTROL
		engine->updateTriggerWaveform();
		engine->triggerCentral.noiseFilter.resetAccumSignalData();
	#endif
	}
#if EFI_DEFAILED_LOGGING
	efiPrintf("isTriggerConfigChanged=%d", triggerConfigChanged);
#endif /* EFI_DEFAILED_LOGGING */

	// we do not want to miss two updates in a row
	engine->triggerCentral.triggerConfigChanged = engine->triggerCentral.triggerConfigChanged || changed;
}

/**
 * @returns true if configuration just changed, and if that change has affected trigger
 */
bool TriggerCentral::checkIfTriggerConfigChanged() {
	bool result = triggerVersion.isOld(engine->getGlobalConfigurationVersion()) && triggerConfigChanged;
	triggerConfigChanged = false; // whoever has called the method is supposed to react to changes
	return result;
}

bool TriggerCentral::isTriggerConfigChanged() {
	return triggerConfigChanged;
}

void validateTriggerInputs() {
	if (engineConfiguration->triggerInputPins[0] == Gpio::Unassigned && engineConfiguration->triggerInputPins[1] != Gpio::Unassigned) {
		firmwareError(OBD_PCM_Processor_Fault, "First trigger channel is missing");
	}

	if (engineConfiguration->camInputs[0] == Gpio::Unassigned && engineConfiguration->camInputs[1] != Gpio::Unassigned) {
		firmwareError(OBD_PCM_Processor_Fault, "If you only have cam on exhaust please pretend that it's on intake in configuration");
	}

	if (engineConfiguration->camInputs[0] == Gpio::Unassigned && engineConfiguration->camInputs[2] != Gpio::Unassigned) {
		firmwareError(OBD_PCM_Processor_Fault, "First bank cam input is required if second bank specified");
	}
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
bool TriggerCentral::isTriggerDecoderError() {
	return engine->triggerErrorDetection.sum(6) > 4;
}

#endif // EFI_SHAFT_POSITION_INPUT
