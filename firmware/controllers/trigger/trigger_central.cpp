/*
 * @file	trigger_central.cpp
 * Here we have a bunch of higher-level methods which are not directly related to actual signal decoding
 *
 * @date Feb 23, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "trigger_central.h"
#include "trigger_decoder.h"
#include "main_trigger_callback.h"
#include "listener_array.h"
#include "hip9011.h"
#include "logic_analyzer.h"

#include "local_version_holder.h"
#include "trigger_simulator.h"
#include "trigger_emulator_algo.h"

#include "map_averaging.h"
#include "main_trigger_callback.h"
#include "status_loop.h"
#include "engine_sniffer.h"
#include "auto_generated_sync_edge.h"

#if EFI_TUNER_STUDIO
#include "tunerstudio.h"
#endif /* EFI_TUNER_STUDIO */

#if EFI_ENGINE_SNIFFER
WaveChart waveChart;
#endif /* EFI_ENGINE_SNIFFER */

static scheduling_s debugToggleScheduling;
#define DEBUG_PIN_DELAY US2NT(60)

#define TRIGGER_WAVEFORM(x) getTriggerCentral()->triggerShape.x

#if EFI_SHAFT_POSITION_INPUT

TriggerCentral::TriggerCentral() :
		vvtEventRiseCounter(),
		vvtEventFallCounter(),
		vvtPosition(),
		triggerState("TRG")
{
	setArrayValues(hwEventCounters, 0);
	triggerState.resetState();
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

	if (std::isnan(oneDegreeUs)) {
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
int getCrankDivider(operation_mode_e operationMode) {
	switch (operationMode) {
	case FOUR_STROKE_CRANK_SENSOR:
		return 2;
	case FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR:
		return SYMMETRICAL_CRANK_SENSOR_DIVIDER;
	case FOUR_STROKE_THREE_TIMES_CRANK_SENSOR:
		return SYMMETRICAL_THREE_TIMES_CRANK_SENSOR_DIVIDER;
	case FOUR_STROKE_SIX_TIMES_CRANK_SENSOR:
		return SYMMETRICAL_SIX_TIMES_CRANK_SENSOR_DIVIDER;
	case FOUR_STROKE_TWELVE_TIMES_CRANK_SENSOR:
		return SYMMETRICAL_TWELVE_TIMES_CRANK_SENSOR_DIVIDER;
	case OM_NONE:
	case FOUR_STROKE_CAM_SENSOR:
	case TWO_STROKE:
		// That's easy - trigger cycle matches engine cycle
		return 1;
	/* let's NOT handle default in order to benefit from -Werror=switch	*/
	}
	/**
	 wow even while we explicitly handle all enumerations in the switch above we still need a return statement due to
	 https://stackoverflow.com/questions/34112483/gcc-how-best-to-handle-warning-about-unreachable-end-of-function-after-switch
	 */
	criticalError("unreachable getCrankDivider");
	return 1;
}

static bool vvtWithRealDecoder(vvt_mode_e vvtMode) {
	return vvtMode != VVT_INACTIVE
			&& vvtMode != VVT_TOYOTA_3_TOOTH /* VVT_2JZ is an unusual 3/0 missed tooth symmetrical wheel */
			&& vvtMode != VVT_HONDA_K_INTAKE
			&& vvtMode != VVT_MAP_V_TWIN
			&& vvtMode != VVT_SINGLE_TOOTH;
}

angle_t TriggerCentral::syncEnginePhaseAndReport(int divider, int remainder) {
	angle_t engineCycle = getEngineCycle(getEngineRotationState()->getOperationMode());

	angle_t totalShift = triggerState.syncEnginePhase(divider, remainder, engineCycle);
	if (totalShift != 0) {
		// Reset instant RPM, since the engine phase has now changed, invalidating the tooth history buffer
		// maybe TODO: could/should we rotate the buffer around to re-align it instead? Is that worth it?
		instantRpm.resetInstantRpm();
	}
	return totalShift;
}

static void turnOffAllDebugFields(void *arg) {
	(void)arg;
#if EFI_PROD_CODE
	for (int index = 0;index<TRIGGER_INPUT_PIN_COUNT;index++) {
		if (isBrainPinValid(engineConfiguration->triggerInputDebugPins[index])) {
			writePad("trigger debug", engineConfiguration->triggerInputDebugPins[index], 0);
		}
	}
	for (int index = 0;index<CAM_INPUTS_COUNT;index++) {
		if (isBrainPinValid(engineConfiguration->camInputsDebug[index])) {
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

	operation_mode_e operationMode = getEngineRotationState()->getOperationMode();

	auto crankDivider = getCrankDivider(operationMode);
	if (crankDivider == 1) {
		// Crank divider of 1 means there's no ambiguity, so don't try to resolve it
		return 0;
	}

	TriggerCentral *tc = getTriggerCentral();

	vvt_mode_e vvtMode = engineConfiguration->vvtMode[camIndex];
	switch (vvtMode) {
	case VVT_MAP_V_TWIN:
	case VVT_MITSUBISHI_4G63:
	case VVT_MITSUBISHI_4G9x:
		return tc->syncEnginePhaseAndReport(crankDivider, 1);
	case VVT_SINGLE_TOOTH:
	case VVT_NISSAN_VQ:
	case VVT_BOSCH_QUICK_START:
	case VVT_MIATA_NB:
	case VVT_TOYOTA_3TOOTH_UZ:
	case VVT_TOYOTA_3_TOOTH:
	case VVT_TOYOTA_4_1:
	case VVT_FORD_COYOTE:
	case VVT_DEV:
	case VVT_FORD_ST170:
	case VVT_BARRA_3_PLUS_1:
	case VVT_NISSAN_MR:
  case VVT_HR12DDR_IN:
	case VVT_MAZDA_SKYACTIV:
	case VVT_MAZDA_L:
	case VVT_MITSUBISHI_4G69:
	case VVT_MITSUBISHI_3A92:
	case VVT_MITSUBISHI_6G72:
	case VVT_MITSUBISHI_6G75:
	case VVT_HONDA_K_EXHAUST:
	case VVT_HONDA_CBR_600:
	case VVT_SUBARU_7TOOTH:
		return tc->syncEnginePhaseAndReport(crankDivider, 0);
	case VVT_CUSTOM_25:
	case VVT_CUSTOM_26:
	case VVT_HONDA_K_INTAKE:
	    // with 4 evenly spaced tooth we cannot use this wheel for engine sync
        criticalError("Honda K Intake is not suitable for engine sync");
        [[fallthrough]];
	case VVT_CUSTOM_1:
	case VVT_CUSTOM_2:
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

static void logVvtFront(bool useOnlyRise, bool isImportantFront, TriggerValue front, efitick_t nowNt, int index) {
	if (isImportantFront && isBrainPinValid(engineConfiguration->camInputsDebug[index])) {
#if EFI_PROD_CODE
		writePad("cam debug", engineConfiguration->camInputsDebug[index], 1);
#endif /* EFI_PROD_CODE */
		getScheduler()->schedule("dbg_on", &debugToggleScheduling, nowNt + DEBUG_PIN_DELAY, &turnOffAllDebugFields);
	}

	if (!useOnlyRise || engineConfiguration->displayLogicLevelsInEngineSniffer) {
		// If we care about both edges OR displayLogicLevel is set, log every front exactly as it is
		addEngineSnifferVvtEvent(index, front == TriggerValue::RISE ? FrontDirection::UP : FrontDirection::DOWN);

#if EFI_TOOTH_LOGGER
		LogTriggerCamTooth(front == TriggerValue::RISE, nowNt, index);
#endif /* EFI_TOOTH_LOGGER */
	} else {
		if (isImportantFront) {
			// On the important edge, log a rise+fall pair, and nothing on the real falling edge
			addEngineSnifferVvtEvent(index, FrontDirection::UP);
			addEngineSnifferVvtEvent(index, FrontDirection::DOWN);

#if EFI_TOOTH_LOGGER
			LogTriggerCamTooth(true, nowNt, index);
			LogTriggerCamTooth(false, nowNt, index);
#endif /* EFI_TOOTH_LOGGER */
		}
	}
}

static bool tooSoonToHandleSignal() {
#if EFI_PROD_CODE
extern bool main_loop_started;
	if (!main_loop_started) {
	  warning(ObdCode::CUSTOM_ERR_INPUT_DURING_INITIALISATION, "event too early");
		return true;
	}
#endif //EFI_PROD_CODE
  return false;
}

void hwHandleVvtCamSignal(bool isRising, efitick_t timestamp, int index) {
	hwHandleVvtCamSignal(isRising ? TriggerValue::RISE : TriggerValue::FALL, timestamp, index);
}

// 'invertCamVVTSignal' is already accounted by the time this method is invoked
void hwHandleVvtCamSignal(TriggerValue front, efitick_t nowNt, int index) {
  if (tooSoonToHandleSignal()) {
    return;
  }
	TriggerCentral *tc = getTriggerCentral();
	if (tc->directSelfStimulation || !tc->hwTriggerInputEnabled) {
		// sensor noise + self-stim = loss of trigger sync
		return;
	}
	handleVvtCamSignal(front, nowNt, index);
}

void handleVvtCamSignal(TriggerValue front, efitick_t nowNt, int index) {
	TriggerCentral *tc = getTriggerCentral();
	if (index == 0) {
	    engine->outputChannels.vvtChannel1 = front == TriggerValue::RISE;
	} else if (index == 1) {
	    engine->outputChannels.vvtChannel2 = front == TriggerValue::RISE;
	} else if (index == 2) {
	    engine->outputChannels.vvtChannel3 = front == TriggerValue::RISE;
	} else if (index == 3) {
	    engine->outputChannels.vvtChannel4 = front == TriggerValue::RISE;
	}

	int bankIndex = BANK_BY_INDEX(index);
	int camIndex = CAM_BY_INDEX(index);
	if (front == TriggerValue::RISE) {
		tc->vvtEventRiseCounter[index]++;
	} else {
		tc->vvtEventFallCounter[index]++;
	}
	if (engineConfiguration->vvtMode[camIndex] == VVT_INACTIVE) {
		warning(ObdCode::CUSTOM_VVT_MODE_NOT_SELECTED, "VVT: event on %d but no mode", camIndex);
	}

	const auto& vvtShape = tc->vvtShape[camIndex];

	bool isVvtWithRealDecoder = vvtWithRealDecoder(engineConfiguration->vvtMode[camIndex]);

	// Non real decoders only use the rising edge
	bool vvtUseOnlyRise = !isVvtWithRealDecoder || vvtShape.useOnlyRisingEdges;
	bool isImportantFront = !vvtUseOnlyRise || (front == TriggerValue::RISE);

	logVvtFront(vvtUseOnlyRise, isImportantFront, front, nowNt, index);

	if (!isImportantFront) {
		// This edge is unimportant, ignore it.
		return;
	}

	// If the main trigger is not synchronized, don't decode VVT yet
	if (!tc->triggerState.getShaftSynchronized()) {
		return;
	}

	TriggerDecoderBase& vvtDecoder = tc->vvtState[bankIndex][camIndex];

	if (isVvtWithRealDecoder) {
		vvtDecoder.decodeTriggerEvent(
				"vvt",
			vvtShape,
			nullptr,
			tc->vvtTriggerConfiguration[camIndex],
			front == TriggerValue::RISE ? SHAFT_PRIMARY_RISING : SHAFT_PRIMARY_FALLING, nowNt);
		vvtDecoder.vvtToothDurations0 = (uint32_t)NT2US(vvtDecoder.toothDurations[0]);
	}

    // here we count all cams together
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

	auto vvtPosition = engineConfiguration->vvtOffsets[bankIndex * CAMS_PER_BANK + camIndex] - currentPosition;
	tc->triggerState.vvtToothPosition[index] = vvtPosition;

	switch(engineConfiguration->vvtMode[camIndex]) {
	case VVT_TOYOTA_3_TOOTH:
	{
	    int from = engineConfiguration->camDecoder2jzPosition - engineConfiguration->camDecoder2jzPrecision;
	    int to   = engineConfiguration->camDecoder2jzPosition + engineConfiguration->camDecoder2jzPrecision;
		// we do not know if we are in sync or out of sync, so we have to be looking for both possibilities
		if ((currentPosition < from       || currentPosition > to) &&
		    (currentPosition < from + 360 || currentPosition > to + 360)) {
			// outside of the expected range
			return;
		}
	}
		break;
	default:
		// else, do nothing
		break;
	}

	// this could be just an 'if' but let's have it expandable for future use :)
	switch(engineConfiguration->vvtMode[camIndex]) {
	case VVT_HONDA_K_INTAKE:
		// honda K has four tooth in VVT intake trigger, so we just wrap each of those to 720 / 4
		vvtPosition = wrapVvt(vvtPosition, 180);
		break;
	default:
		// else, do nothing
		break;
	}

#if EFI_PROD_CODE
  if (!isBrainPinValid(engineConfiguration->camInputs[engineConfiguration->engineSyncCam]) &&
      engineConfiguration->vvtMode[engineConfiguration->engineSyncCam] != VVT_MAP_V_TWIN) {
    criticalError("Selected engine sync input not configured: %d", engineConfiguration->engineSyncCam);
  }
#endif // EFI_PROD_CODE

	// Only do engine sync using one cam, other cams just provide VVT position.
	if (index == engineConfiguration->engineSyncCam) {
		angle_t crankOffset = adjustCrankPhase(camIndex);
		// vvtPosition was calculated against wrong crank zero position. Now that we have adjusted crank position we
		// shall adjust vvt position as well
		vvtPosition -= crankOffset;
		vvtPosition = wrapVvt(vvtPosition, FOUR_STROKE_CYCLE_DURATION);

		if (absF(angleFromPrimarySyncPoint) < 7) {
			/**
			 * we prefer not to have VVT sync right at trigger sync so that we do not have phase detection error if things happen a bit in
			 * wrong order due to belt flex or else
			 * https://github.com/rusefi/rusefi/issues/3269
			 */
			warning(ObdCode::CUSTOM_VVT_SYNC_POSITION, "VVT sync position too close to trigger sync");
		}
	} else {
		// Not using this cam for engine sync, just wrap the value in to the reasonable range
		vvtPosition = wrapVvt(vvtPosition, FOUR_STROKE_CYCLE_DURATION);
	}

	// Only record VVT position if we have full engine sync - may be bogus before that point
	if (tc->triggerState.hasSynchronizedPhase()) {
		tc->vvtPosition[bankIndex][camIndex] = vvtPosition;
	} else {
		tc->vvtPosition[bankIndex][camIndex] = 0;
	}
}

int triggerReentrant = 0;
int maxTriggerReentrant = 0;
uint32_t triggerDuration;
uint32_t triggerMaxDuration = 0;

/**
 * This function is called by all "hardware" trigger inputs:
 *  - Hardware triggers
 *  - Trigger replay from CSV (unit tests)
 */
void hwHandleShaftSignal(int signalIndex, bool isRising, efitick_t timestamp) {
  if (tooSoonToHandleSignal()) {
    return;
  }
	TriggerCentral *tc = getTriggerCentral();
	ScopePerf perf(PE::HandleShaftSignal);

	if (tc->directSelfStimulation || !tc->hwTriggerInputEnabled) {
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
	if (isPrimary) {
		engine->outputChannels.triggerChannel1 = signal == SHAFT_PRIMARY_RISING;
	} else {
		engine->outputChannels.triggerChannel2 = signal == SHAFT_SECONDARY_RISING;
	}

	// Don't accept trigger input in case of some problems
	if (!getLimpManager()->allowTriggerInput()) {
		return;
	}

#if EFI_TOOTH_LOGGER
	// Log to the Tunerstudio tooth logger
	// We want to do this before anything else as we
	// actually want to capture any noise/jitter that may be occurring

	bool logLogicState = engineConfiguration->displayLogicLevelsInEngineSniffer && getTriggerCentral()->triggerShape.useOnlyRisingEdges;

	if (!logLogicState) {
		// we log physical state even if displayLogicLevelsInEngineSniffer if both fronts are used by decoder
		LogTriggerTooth(signal, timestamp);
	}

#endif /* EFI_TOOTH_LOGGER */

	// for effective noise filtering, we need both signal edges,
	// so we pass them to handleShaftSignal() and defer this test
	if (!engineConfiguration->useNoiselessTriggerDecoder) {
		if (!isUsefulSignal(signal, getTriggerCentral()->triggerShape)) {
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
		getScheduler()->schedule("dbg_off", &debugToggleScheduling, timestamp + DEBUG_PIN_DELAY, &turnOffAllDebugFields);
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

	getTriggerCentral()->handleShaftSignal(signal, timestamp);

	triggerReentrant--;
	triggerDuration = getTimeNowLowerNt() - triggerHandlerEntryTime;
	triggerMaxDuration = maxI(triggerMaxDuration, triggerDuration);
}

void TriggerCentral::resetCounters() {
	memset(hwEventCounters, 0, sizeof(hwEventCounters));
}

static const int wheelIndeces[4] = { 0, 0, 1, 1};

static void reportEventToWaveChart(trigger_event_e ckpSignalType, int triggerEventIndex, bool addOppositeEvent) {
	if (!getTriggerCentral()->isEngineSnifferEnabled) { // this is here just as a shortcut so that we avoid engine sniffer as soon as possible
		return; // engineSnifferRpmThreshold is accounted for inside getTriggerCentral()->isEngineSnifferEnabled
	}

	int wheelIndex = wheelIndeces[(int )ckpSignalType];

	bool isUp = isTriggerUpEvent(ckpSignalType);

	addEngineSnifferCrankEvent(wheelIndex, triggerEventIndex, isUp ? FrontDirection::UP : FrontDirection::DOWN);
	if (addOppositeEvent) {
		// let's add the opposite event right away
		addEngineSnifferCrankEvent(wheelIndex, triggerEventIndex, isUp ? FrontDirection::DOWN : FrontDirection::UP);
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
	static const TriggerWheel triggerIdx[4] = { TriggerWheel::T_PRIMARY, TriggerWheel::T_PRIMARY, TriggerWheel::T_SECONDARY, TriggerWheel:: T_SECONDARY };
	// we process all trigger channels independently
	TriggerWheel ti = triggerIdx[signal];
	// falling is opposite to rising, and vise versa
	trigger_event_e os = opposite[signal];

	// todo: currently only primary channel is filtered, because there are some weird trigger types on other channels
	if (ti != TriggerWheel::T_PRIMARY)
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
			(triggerState->currentCycle.eventCount[(int)ti] + 1) == TRIGGER_WAVEFORM(getExpectedEventCount(ti));

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
    isDecodingMapCam = engineConfiguration->vvtMode[0] == VVT_MAP_V_TWIN &&
                       			Sensor::getOrZero(SensorType::Rpm) < engineConfiguration->cranking.rpm;
	if (isDecodingMapCam) {
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
			float instantMapDiffBetweenReadoutAngles = map - mapCamPrevCycleValue;
			mapCamPrevCycleValue = map;

			if (instantMapDiffBetweenReadoutAngles > engineConfiguration->mapSyncThreshold) {
				mapVvt_sync_counter++;
				int revolutionCounter = getTriggerCentral()->triggerState.getSynchronizationCounter();
				mapVvt_MAP_AT_CYCLE_COUNT = revolutionCounter - prevChangeAtCycle;
				prevChangeAtCycle = revolutionCounter;

				hwHandleVvtCamSignal(TriggerValue::RISE, timestamp, /*index*/0);
				hwHandleVvtCamSignal(TriggerValue::FALL, timestamp, /*index*/0);
#if EFI_UNIT_TEST
				// hack? feature? existing unit test relies on VVT phase available right away
				// but current implementation which is based on periodicFastCallback would only make result available on NEXT tooth
				getLimpManager()->onFastCallback();
#endif // EFI_UNIT_TEST
			}

			mapVvt_MAP_AT_SPECIAL_POINT = map;
			mapVvt_MAP_AT_DIFF = instantMapDiffBetweenReadoutAngles;
		}

		mapCamPrevToothAngle = toothAngle360;
	}
}

bool TriggerCentral::isToothExpectedNow(efitick_t timestamp) {
	// Check that the expected next phase (from the last tooth) is close to the actual current phase:
	// basically, check that the tooth width is correct
	auto estimatedCurrentPhase = getCurrentEnginePhase(timestamp);
	auto lastToothPhase = m_lastToothPhaseFromSyncPoint;

	if (expectedNextPhase && estimatedCurrentPhase) {
		float angleError = expectedNextPhase.Value - estimatedCurrentPhase.Value;

		// Wrap around correctly at the end of the cycle
		float cycle = getEngineState()->engineCycle;
		if (angleError < -cycle / 2) {
			angleError += cycle;
		}

		triggerToothAngleError = angleError;

		// Only perform checks if engine is spinning quickly
		// All kinds of garbage happens while cranking
		if (Sensor::getOrZero(SensorType::Rpm) > 1000) {
			// Now compute how close we are to the last tooth decoded
			float angleSinceLastTooth = estimatedCurrentPhase.Value - lastToothPhase;
			if (angleSinceLastTooth < 0.5f) {
				// This tooth came impossibly early, ignore it
				// This rejects things like doubled edges, for example:
				//             |-| |----------------
				//             | | |
				// ____________| |_|
				//             1   2
				//     #1 will be decoded
				//     #2 will be ignored
				// We're not sure which edge was the "real" one, but they were close enough
				// together that it doesn't really matter.
				warning(ObdCode::CUSTOM_PRIMARY_DOUBLED_EDGE, "doubled trigger edge after %.2f deg at #%d", angleSinceLastTooth, triggerState.currentCycle.current_index);

				return false;
			}

			// Absolute error from last tooth
			float absError = absF(angleError);
			float isRpmEnough = Sensor::getOrZero(SensorType::Rpm) > 1000;
			// TODO: configurable threshold
			if (isRpmEnough && absError > 10 && absError < 180) {
				// This tooth came at a very unexpected time, ignore it
				warning(ObdCode::CUSTOM_PRIMARY_BAD_TOOTH_TIMING, "tooth #%d error of %.1f", triggerState.currentCycle.current_index, angleError);

				// TODO: this causes issues with some real engine logs, should it?
				// return false;
			}
		}
	} else {
		triggerToothAngleError = 0;
	}

	// We aren't ready to reject unexpected teeth, so accept this tooth
	return true;
}

PUBLIC_API_WEAK bool boardAllowTriggerActions() { return true; }

angle_t TriggerCentral::findNextTriggerToothAngle(int p_currentToothIndex) {
  int currentToothIndex = p_currentToothIndex;
		// TODO: is this logic to compute next trigger tooth angle correct?
		angle_t nextToothAngle = 0;

		int loopAllowance = 2 * engineCycleEventCount + 1000;
		do {
			// I don't love this.
			currentToothIndex = (currentToothIndex + 1) % engineCycleEventCount;
			nextToothAngle = getTriggerCentral()->triggerFormDetails.eventAngles[currentToothIndex] - tdcPosition();
			wrapAngle(nextToothAngle, "nextEnginePhase", ObdCode::CUSTOM_ERR_6555);
		} while (nextToothAngle == currentEngineDecodedPhase && --loopAllowance > 0); // '==' for float works here since both values come from 'eventAngles' array
		if (nextToothAngle != 0 && loopAllowance == 0) {
		  // HW CI fails here, looks like we sometimes change trigger while still handling it?
			firmwareError(ObdCode::CUSTOM_ERR_TRIGGER_ZERO, "handleShaftSignal unexpected loop end %d %d %f %f", p_currentToothIndex, engineCycleEventCount, nextToothAngle, currentEngineDecodedPhase);
		}
		return nextToothAngle;
}

/**
 * This method is NOT invoked for VR falls.
 */
void TriggerCentral::handleShaftSignal(trigger_event_e signal, efitick_t timestamp) {
	if (triggerShape.shapeDefinitionError) {
		// trigger is broken, we cannot do anything here
		warning(ObdCode::CUSTOM_ERR_UNEXPECTED_SHAFT_EVENT, "Shaft event while trigger is mis-configured");
		return;
	}

	// This code gathers some statistics on signals and compares accumulated periods to filter interference
	if (engineConfiguration->useNoiselessTriggerDecoder) {
		if (!noiseFilter.noiseFilter(timestamp, &triggerState, signal)) {
			return;
		}
		if (!isUsefulSignal(signal, triggerShape)) {
			return;
		}
	}

	if (!isToothExpectedNow(timestamp)) {
		triggerIgnoredToothCount++;
		return;
	}

	isSpinningJustForWatchdog = true;

#if EFI_HD_ACR
    bool firstEventInAWhile = m_lastEventTimer.hasElapsedSec(1);
	if (firstEventInAWhile) {
        // let's open that valve on first sign of movement
        engine->module<HarleyAcr>()->updateAcr();
	}
#endif // EFI_HD_ACR

  if (boardAllowTriggerActions()) {
	  m_lastEventTimer.reset(timestamp);
  }

	int eventIndex = (int) signal;
	efiAssertVoid(ObdCode::CUSTOM_TRIGGER_EVENT_TYPE, eventIndex >= 0 && eventIndex < HW_EVENT_TYPES, "signal type");
	hwEventCounters[eventIndex]++;

	// Decode the trigger!
	auto decodeResult = triggerState.decodeTriggerEvent(
			"trigger",
			triggerShape,
			engine,
			primaryTriggerConfiguration,
			signal, timestamp);

	// Don't propagate state if we don't know where we are
	if (decodeResult) {
		ScopePerf perf(PE::ShaftPositionListeners);

		/**
		 * If we only have a crank position sensor with four stroke, here we are extending crank revolutions with a 360 degree
		 * cycle into a four stroke, 720 degrees cycle.
		 */
		int crankDivider = getCrankDivider(triggerShape.getWheelOperationMode());
		int crankInternalIndex = triggerState.getSynchronizationCounter() % crankDivider;
		int triggerIndexForListeners = decodeResult.Value.CurrentIndex + (crankInternalIndex * triggerShape.getSize());

		reportEventToWaveChart(signal, triggerIndexForListeners, triggerShape.useOnlyRisingEdges);

		// Look up this tooth's angle from the sync point. If this tooth is the sync point, we'll get 0 here.
		auto currentPhaseFromSyncPoint = getTriggerCentral()->triggerFormDetails.eventAngles[triggerIndexForListeners];

		// Adjust so currentPhase is in engine-space angle, not trigger-space angle
		currentEngineDecodedPhase = wrapAngleMethod(currentPhaseFromSyncPoint - tdcPosition(), "currentEnginePhase", ObdCode::CUSTOM_ERR_6555);

		// Record precise time and phase of the engine. This is used for VVT decode, and to check that the
		// trigger pattern selected matches reality (ie, we check the next tooth is where we think it should be)
		{
			// under lock to avoid mismatched tooth phase and time
			chibios_rt::CriticalSectionLocker csl;

			m_lastToothTimer.reset(timestamp);
			m_lastToothPhaseFromSyncPoint = currentPhaseFromSyncPoint;
		}

#if TRIGGER_EXTREME_LOGGING
	efiPrintf("trigger %d %d %d", triggerIndexForListeners, getRevolutionCounter(), time2print(getTimeNowUs()));
#endif /* TRIGGER_EXTREME_LOGGING */

		// Update engine RPM
		rpmShaftPositionCallback(signal, triggerIndexForListeners, timestamp);

		// Schedule the TDC mark
		tdcMarkCallback(triggerIndexForListeners, timestamp);

#if EFI_MAP_AVERAGING
		mapAveragingTriggerCallback(triggerIndexForListeners, timestamp);
#endif /* EFI_MAP_AVERAGING */

#if EFI_LOGIC_ANALYZER
		waTriggerEventListener(signal, triggerIndexForListeners, timestamp);
#endif

		angle_t nextPhase = findNextTriggerToothAngle(triggerIndexForListeners);

		float expectNextPhase = nextPhase + tdcPosition();
		wrapAngle(expectNextPhase, "nextEnginePhase", ObdCode::CUSTOM_ERR_6555);
		expectedNextPhase = expectNextPhase;

#if EFI_CDM_INTEGRATION
		if (trgEventIndex == 0 && isBrainPinValid(engineConfiguration->cdmInputPin)) {
			int cdmKnockValue = getCurrentCdmValue(getTriggerCentral()->triggerState.getSynchronizationCounter());
			engine->knockLogic(cdmKnockValue);
		}
#endif /* EFI_CDM_INTEGRATION */

		if (engine->rpmCalculator.getCachedRpm() > 0 && triggerIndexForListeners == 0) {
			engine->module<TpsAccelEnrichment>()->onEngineCycleTps();
		}

		// Handle ignition and injection
		mainTriggerCallback(triggerIndexForListeners, timestamp, currentEngineDecodedPhase, nextPhase);

		// Decode the MAP based "cam" sensor
		decodeMapCam(timestamp, currentEngineDecodedPhase);
	} else {
		// We don't have sync, but report to the wave chart anyway as index 0.
		reportEventToWaveChart(signal, 0, triggerShape.useOnlyRisingEdges);

		expectedNextPhase = unexpected;
	}
}

static void triggerShapeInfo() {
#if EFI_PROD_CODE || EFI_SIMULATOR
	TriggerWaveform *shape = &getTriggerCentral()->triggerShape;
	TriggerFormDetails *triggerFormDetails = &getTriggerCentral()->triggerFormDetails;
	efiPrintf("syncEdge=%s", getSyncEdge(TRIGGER_WAVEFORM(syncEdge)));
	efiPrintf("gap from %.2f to %.2f", TRIGGER_WAVEFORM(synchronizationRatioFrom[0]), TRIGGER_WAVEFORM(synchronizationRatioTo[0]));

	for (size_t i = 0; i < shape->getSize(); i++) {
		efiPrintf("event %d %.2f", i, triggerFormDetails->eventAngles[i]);
	}
#endif
}

#if EFI_PROD_CODE
extern PwmConfig triggerEmulatorSignals[NUM_EMULATOR_CHANNELS];
#endif /* #if EFI_PROD_CODE */

void triggerInfo(void) {
#if EFI_PROD_CODE || EFI_SIMULATOR

	TriggerCentral *tc = getTriggerCentral();
	TriggerWaveform *ts = &tc->triggerShape;


#if (HAL_TRIGGER_USE_PAL == TRUE) && (PAL_USE_CALLBACKS == TRUE)
		efiPrintf("trigger PAL mode %d", tc->hwTriggerInputEnabled);
#else

#endif /* HAL_TRIGGER_USE_PAL */

	efiPrintf("Template %s (%d) trigger %s (%d) syncEdge=%s tdcOffset=%.2f",
			getEngine_type_e(engineConfiguration->engineType),
			(int)engineConfiguration->engineType,
			getTrigger_type_e(engineConfiguration->trigger.type),
			(int)engineConfiguration->trigger.type,
			getSyncEdge(TRIGGER_WAVEFORM(syncEdge)), TRIGGER_WAVEFORM(tdcPosition));

	if (engineConfiguration->trigger.type == trigger_type_e::TT_TOOTHED_WHEEL) {
		efiPrintf("total %d/skipped %d", engineConfiguration->trigger.customTotalToothCount,
				engineConfiguration->trigger.customSkippedToothCount);
	}


	efiPrintf("trigger#1 event counters up=%d/down=%d", tc->getHwEventCounter(0),
			tc->getHwEventCounter(1));

	if (ts->needSecondTriggerInput) {
		efiPrintf("trigger#2 event counters up=%d/down=%d", tc->getHwEventCounter(2),
				tc->getHwEventCounter(3));
	}
	efiPrintf("expected cycle events %d/%d",
			TRIGGER_WAVEFORM(getExpectedEventCount(TriggerWheel::T_PRIMARY)),
			TRIGGER_WAVEFORM(getExpectedEventCount(TriggerWheel::T_SECONDARY)));

	efiPrintf("trigger type=%d/need2ndChannel=%s", (int)engineConfiguration->trigger.type,
			boolToString(TRIGGER_WAVEFORM(needSecondTriggerInput)));


	efiPrintf("synchronizationNeeded=%s/isError=%s/total errors=%lu ord_err=%lu/total revolutions=%d/self=%s",
			boolToString(ts->isSynchronizationNeeded),
			boolToString(tc->isTriggerDecoderError()),
			tc->triggerState.totalTriggerErrorCounter,
			tc->triggerState.orderingErrorCounter,
			tc->triggerState.getSynchronizationCounter(),
			boolToString(tc->directSelfStimulation));

	if (TRIGGER_WAVEFORM(isSynchronizationNeeded)) {
		efiPrintf("gap from %.2f to %.2f", TRIGGER_WAVEFORM(synchronizationRatioFrom[0]), TRIGGER_WAVEFORM(synchronizationRatioTo[0]));
	}

#endif /* EFI_PROD_CODE || EFI_SIMULATOR */

#if EFI_PROD_CODE

	efiPrintf("primary trigger input: %s", hwPortname(engineConfiguration->triggerInputPins[0]));
	efiPrintf("primary trigger simulator: %s %s freq=%d",
			hwPortname(engineConfiguration->triggerSimulatorPins[0]),
			getPin_output_mode_e(engineConfiguration->triggerSimulatorPinModes[0]),
			engineConfiguration->triggerSimulatorRpm);

	if (ts->needSecondTriggerInput) {
		efiPrintf("secondary trigger input: %s", hwPortname(engineConfiguration->triggerInputPins[1]));
#if EFI_EMULATE_POSITION_SENSORS
		efiPrintf("secondary trigger simulator: %s %s phase=%d",
				hwPortname(engineConfiguration->triggerSimulatorPins[1]),
				getPin_output_mode_e(engineConfiguration->triggerSimulatorPinModes[1]), triggerEmulatorSignals[0].safe.phaseIndex);
#endif /* EFI_EMULATE_POSITION_SENSORS */
	}


	for (int camInputIndex = 0; camInputIndex<CAM_INPUTS_COUNT;camInputIndex++) {
		if (isBrainPinValid(engineConfiguration->camInputs[camInputIndex])) {
			int camLogicalIndex = camInputIndex % CAMS_PER_BANK;
			efiPrintf("VVT input: %s mode %s", hwPortname(engineConfiguration->camInputs[camInputIndex]),
					getVvt_mode_e(engineConfiguration->vvtMode[camLogicalIndex]));
			efiPrintf("VVT %d event counters: %d/%d",
					camInputIndex,
					tc->vvtEventRiseCounter[camInputIndex], tc->vvtEventFallCounter[camInputIndex]);
		}
	}

	efiPrintf("primary logic input: %s", hwPortname(engineConfiguration->logicAnalyzerPins[0]));
	efiPrintf("secondary logic input: %s", hwPortname(engineConfiguration->logicAnalyzerPins[1]));


	efiPrintf("totalTriggerHandlerMaxTime=%lu", triggerMaxDuration);

#endif /* EFI_PROD_CODE */

#if EFI_ENGINE_SNIFFER
	efiPrintf("engine sniffer current size=%d", waveChart.getSize());
#endif /* EFI_ENGINE_SNIFFER */

}

static void resetRunningTriggerCounters() {
#if !EFI_UNIT_TEST
	getTriggerCentral()->resetCounters();
	triggerInfo();
#endif
}

void onConfigurationChangeTriggerCallback() {
	bool changed = false;
	// todo: how do we static_assert here?
	criticalAssertVoid(efi::size(engineConfiguration->camInputs) == efi::size(engineConfiguration->vvtOffsets), "sizes");

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
		Gpio pin = engineConfiguration->camInputs[i];
		if (engineConfiguration->vvtMode[0] == VVT_MAP_V_TWIN && isBrainPinValid(pin)) {
		    criticalError("Please no physical sensors in CAM by MAP mode index=%d %s", i, hwPortname(pin));
		}
	}

	for (size_t i = 0; i < efi::size(engineConfiguration->vvtMode); i++) {
		changed |= isConfigurationChanged(vvtMode[i]);
	}

	changed |= isConfigurationChanged(trigger.type);
	changed |= isConfigurationChanged(skippedWheelOnCam);
	changed |= isConfigurationChanged(twoStroke);
	changed |= isConfigurationChanged(globalTriggerAngleOffset);
	changed |= isConfigurationChanged(trigger.customTotalToothCount);
	changed |= isConfigurationChanged(trigger.customSkippedToothCount);
	changed |= isConfigurationChanged(overrideTriggerGaps);
	changed |= isConfigurationChanged(gapTrackingLengthOverride);
	changed |= isConfigurationChanged(overrideVvtTriggerGaps);
	changed |= isConfigurationChanged(gapVvtTrackingLengthOverride);

	if (changed) {
	#if EFI_ENGINE_CONTROL
		engine->updateTriggerConfiguration();
		getTriggerCentral()->noiseFilter.resetAccumSignalData();
	#endif
	}
#if EFI_DEFAILED_LOGGING
	efiPrintf("isTriggerConfigChanged=%d", triggerConfigChanged);
#endif /* EFI_DEFAILED_LOGGING */

	// we do not want to miss two updates in a row
	getTriggerCentral()->triggerConfigChangedOnLastConfigurationChange = getTriggerCentral()->triggerConfigChangedOnLastConfigurationChange || changed;
}

static void initVvtShape(TriggerWaveform& shape, const TriggerConfiguration& p_config, TriggerDecoderBase &initState) {
	shape.initializeTriggerWaveform(FOUR_STROKE_CAM_SENSOR, p_config.TriggerType, /*isCrank*/ false);
	shape.initializeSyncPoint(initState, p_config);
}

void TriggerCentral::validateCamVvtCounters() {
	// micro-optimized 'synchronizationCounter % 256'
	int camVvtValidationIndex = triggerState.getSynchronizationCounter() & 0xFF;
	if (camVvtValidationIndex == 0) {
		vvtCamCounter = 0;
	} else if (camVvtValidationIndex == 0xFE && vvtCamCounter < 60) {
		// magic logic: we expect at least 60 CAM/VVT events for each 256 trigger cycles, otherwise throw a code
		warning(ObdCode::OBD_Camshaft_Position_Sensor_Circuit_Range_Performance, "No Camshaft Position Sensor signals");
	}
}
/**
 * Calculate 'shape.triggerShapeSynchPointIndex' value using 'TriggerDecoderBase *state'
 */
static void calculateTriggerSynchPoint(
		const PrimaryTriggerConfiguration &primaryTriggerConfiguration,
		TriggerWaveform& shape,
		TriggerDecoderBase& initState) {

#if EFI_PROD_CODE
	efiAssertVoid(ObdCode::CUSTOM_TRIGGER_STACK, hasLotsOfRemainingStack(), "calc s");
#endif

	shape.initializeSyncPoint(initState, primaryTriggerConfiguration);

	if (shape.getSize() >= PWM_PHASE_MAX_COUNT) {
		// todo: by the time we are here we had already modified a lot of RAM out of bounds!
		firmwareError(ObdCode::CUSTOM_ERR_TRIGGER_WAVEFORM_TOO_LONG, "Trigger length above maximum: %d", shape.getSize());
		shape.setShapeDefinitionError(true);
		return;
	}

	if (shape.getSize() == 0) {
		firmwareError(ObdCode::CUSTOM_ERR_TRIGGER_ZERO, "triggerShape size is zero");
	}
}

TriggerDecoderBase initState("init");

void TriggerCentral::applyShapesConfiguration() {
	// Re-read config in case it's changed
	primaryTriggerConfiguration.update();
	for (int camIndex = 0;camIndex < CAMS_PER_BANK;camIndex++) {
		vvtTriggerConfiguration[camIndex].update();
	}

	triggerShape.initializeTriggerWaveform(lookupOperationMode(), primaryTriggerConfiguration.TriggerType);

	/**
	 * this is only useful while troubleshooting a new trigger shape in the field
	 * in very VERY rare circumstances
	 */
	if (engineConfiguration->overrideTriggerGaps) {
		int gapIndex = 0;

		triggerShape.gapTrackingLength = engineConfiguration->gapTrackingLengthOverride;

		// copy however many the user wants
		for (; gapIndex < engineConfiguration->gapTrackingLengthOverride; gapIndex++) {
			float gapOverrideFrom = engineConfiguration->triggerGapOverrideFrom[gapIndex];
			float gapOverrideTo = engineConfiguration->triggerGapOverrideTo[gapIndex];
			triggerShape.setTriggerSynchronizationGap3(/*gapIndex*/gapIndex, gapOverrideFrom, gapOverrideTo);
		}

		// fill the remainder with the default gaps
		for (; gapIndex < GAP_TRACKING_LENGTH; gapIndex++) {
			triggerShape.synchronizationRatioFrom[gapIndex] = NAN;
			triggerShape.synchronizationRatioTo[gapIndex] = NAN;
		}
	}

	if (!triggerShape.shapeDefinitionError) {
		int length = triggerShape.getLength();
		engineCycleEventCount = length;

		efiAssertVoid(ObdCode::CUSTOM_SHAPE_LEN_ZERO, length > 0, "shapeLength=0");

		triggerErrorDetection.clear();

		/**
	 	 * 'initState' instance of TriggerDecoderBase is used only to initialize 'this' TriggerWaveform instance
	 	 * #192 BUG real hardware trigger events could be coming even while we are initializing trigger
	 	 */
		calculateTriggerSynchPoint(primaryTriggerConfiguration,
				triggerShape,
				initState);
	}

    if (engineConfiguration->overrideVvtTriggerGaps) {
        int gapIndex = 0;

        TriggerWaveform *shape = &vvtShape[0];
        shape->gapTrackingLength = engineConfiguration->gapVvtTrackingLengthOverride;

		for (; gapIndex < engineConfiguration->gapVvtTrackingLengthOverride; gapIndex++) {
			float gapOverrideFrom = engineConfiguration->triggerVVTGapOverrideFrom[gapIndex];
			float gapOverrideTo = engineConfiguration->triggerVVTGapOverrideTo[gapIndex];
			shape->synchronizationRatioFrom[gapIndex] = gapOverrideFrom;
			shape->synchronizationRatioTo[gapIndex] = gapOverrideTo;
		}
		// fill the remainder with the default gaps
		for (; gapIndex < VVT_TRACKING_LENGTH; gapIndex++) {
			shape->synchronizationRatioFrom[gapIndex] = NAN;
			shape->synchronizationRatioTo[gapIndex] = NAN;
		}
    }

	for (int camIndex = 0; camIndex < CAMS_PER_BANK; camIndex++) {
		// todo: should 'vvtWithRealDecoder' be used here?
		if (engineConfiguration->vvtMode[camIndex] != VVT_INACTIVE) {
			initVvtShape(
				vvtShape[camIndex],
				vvtTriggerConfiguration[camIndex],
				initState
			);
		}
	}

	// This is not the right place for this, but further refactoring has to happen before it can get moved.
	triggerState.setNeedsDisambiguation(engine->triggerCentral.triggerShape.needsDisambiguation());

}

/**
 * @returns true if configuration just changed, and if that change has affected trigger
 */
bool TriggerCentral::checkIfTriggerConfigChanged() {
	// we want to make sure that configuration has changed AND that change has changed trigger specifically
	bool result = triggerVersion.isOld(engine->getGlobalConfigurationVersion()) && triggerConfigChangedOnLastConfigurationChange;
	triggerConfigChangedOnLastConfigurationChange = false; // whoever has called the method is supposed to react to changes
	return result;
}

#if EFI_UNIT_TEST
bool TriggerCentral::isTriggerConfigChanged() {
	return triggerConfigChangedOnLastConfigurationChange;
}
#endif // EFI_UNIT_TEST

void validateTriggerInputs() {
	if (!isBrainPinValid(engineConfiguration->triggerInputPins[0]) && isBrainPinValid(engineConfiguration->triggerInputPins[1])) {
		criticalError("First trigger channel not configured while second one is.");
	}

	if (!isBrainPinValid(engineConfiguration->camInputs[0]) && isBrainPinValid(engineConfiguration->camInputs[2])) {
		criticalError("First bank cam input is required if second bank specified");
	}
}

void initTriggerCentral() {

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
	return triggerErrorDetection.sum(6) > 4;
}

#endif // EFI_SHAFT_POSITION_INPUT
