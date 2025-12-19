/*
 * @file	trigger_central.h
 *
 * @date Feb 23, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "rusefi_enums.h"
#include "listener_array.h"
#include "trigger_decoder.h"
#include "instant_rpm_calculator.h"
#include "trigger_central_generated.h"
#include <rusefi/timer.h>
#include "pin_repository.h"
#include "local_version_holder.h"
#include "cyclic_buffer.h"
#include <algorithm>

#define MAP_CAM_BUFFER 64

#ifndef RPM_LOW_THRESHOLD
// no idea what is the best value, 25 is as good as any other guess
#define RPM_LOW_THRESHOLD 25
#endif

class Engine;
typedef void (*ShaftPositionListener)(trigger_event_e signal, uint32_t index, efitick_t edgeTimestamp);

#define HAVE_CAM_INPUT() (isBrainPinValid(engineConfiguration->camInputs[0]))

class TriggerNoiseFilter {
public:
	void resetAccumSignalData();
	bool noiseFilter(efitick_t nowNt,
			TriggerDecoderBase* triggerState,
			trigger_event_e signal);

	efitick_t lastSignalTimes[HW_EVENT_TYPES];
	efitick_t accumSignalPeriods[HW_EVENT_TYPES];
	efitick_t accumSignalPrevPeriods[HW_EVENT_TYPES];
};

/**
 * Maybe merge TriggerCentral and TriggerState classes into one class?
 * Probably not: we have an instance of TriggerState which is used for trigger initialization,
 * also composition probably better than inheritance here
 */
class TriggerCentral final : public trigger_central_s {
public:
	TriggerCentral();
	/**
	 * we have two kinds of sync:
	 * this method is about detecting of exact engine phase with 720 degree precision usually based on cam wheel decoding
	 * not to be confused with a totally different trigger _wheel_ sync which could be either crank wheel sync or cam wheel sync
	 */
	angle_t syncEnginePhaseAndReport(int divider, int remainder);
	void handleShaftSignal(trigger_event_e signal, efitick_t timestamp);
	int getHwEventCounter(int index) const;
	void resetCounters();
	void validateCamVvtCounters();
	void applyShapesConfiguration();

  angle_t findNextTriggerToothAngle(int nextToothIndex);

	InstantRpmCalculator instantRpm;

	void prepareTriggerShape() {
#if EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT
		if (triggerShape.shapeDefinitionError) {
			// Nothing to do here if there's a problem with the trigger shape
			return;
		}

		triggerFormDetails.prepareEventAngles(&triggerShape);
#endif
	}

	// this is useful at least for real hardware integration testing - maybe a proper solution would be to simply
	// GND input pins instead of leaving them floating
	bool hwTriggerInputEnabled = true;

	cyclic_buffer<int> triggerErrorDetection;

	/**
	 * See also triggerSimulatorRpm
	 */
	bool directSelfStimulation = false;

	PrimaryTriggerConfiguration primaryTriggerConfiguration;
#if CAMS_PER_BANK == 1
	VvtTriggerConfiguration vvtTriggerConfiguration[CAMS_PER_BANK] = {{"VVT1 ", 0}};
#else
	VvtTriggerConfiguration vvtTriggerConfiguration[CAMS_PER_BANK] = {{"VVT1 ", 0}, {"VVT2 ", 1}};
#endif

	LocalVersionHolder triggerVersion;

	/**
	 * By the way:
	 * 'cranking' means engine is not stopped and the rpm are below crankingRpm
	 * 'running' means RPM are above crankingRpm
	 * 'spinning' means the engine is not stopped
	 */
	// todo: combine with other RpmCalculator fields?
	/**
	 * this is set to true each time we register a trigger tooth signal
	 */
	bool isSpinningJustForWatchdog = false;

	float mapCamPrevCycleValue = 0;
	int prevChangeAtCycle = 0;

	/**
	 * value of 'triggerShape.getLength()'
	 * pre-calculating this value is a performance optimization
	 */
	uint32_t engineCycleEventCount = 0;
	/**
	 * true if a recent configuration change has changed any of the trigger settings which
	 * we have not adjusted for yet
	 */
	bool triggerConfigChangedOnLastConfigurationChange = false;

	bool checkIfTriggerConfigChanged();
#if EFI_UNIT_TEST
	bool isTriggerConfigChanged();
#endif // EFI_UNIT_TEST

	bool isTriggerDecoderError();

	expected<float> getCurrentEnginePhase(efitick_t nowNt) const;

	float getSecondsSinceTriggerEvent(efitick_t nowNt) const {
		return m_lastEventTimer.getElapsedSeconds(nowNt);
	}

	bool engineMovedRecently(efitick_t nowNt) const {
		// todo: this user-defined property is a quick solution, proper fix https://github.com/rusefi/rusefi/issues/6593 is needed
		if (engineConfiguration->triggerEventsTimeoutMs != 0 && m_lastEventTimer.hasElapsedMs(engineConfiguration->triggerEventsTimeoutMs)) {
			return false;
		}

		constexpr float oneRevolutionLimitInSeconds = 60.0 / RPM_LOW_THRESHOLD;
		auto maxAverageToothTime = oneRevolutionLimitInSeconds / triggerShape.getSize();

		// Some triggers may have long gaps (with many teeth), don't count that as stopped!
		auto maxAllowedGap = maxAverageToothTime * 10;

		// Clamp between 0.1 seconds ("instant" for a human) and worst case of one engine cycle on low tooth count wheel
		maxAllowedGap = std::clamp(maxAllowedGap, 0.1f, oneRevolutionLimitInSeconds);

		return (getSecondsSinceTriggerEvent(nowNt) < maxAllowedGap) || directSelfStimulation;
	}

	bool engineMovedRecently() const {
		return engineMovedRecently(getTimeNowNt());
	}

	TriggerNoiseFilter noiseFilter;

	angle_t getVVTPosition(uint8_t bankIndex, uint8_t camIndex);

#if EFI_UNIT_TEST
	// latest VVT event position (could be not synchronization event)
	angle_t currentVVTEventPosition[BANKS_COUNT][CAMS_PER_BANK];
#endif // EFI_UNIT_TEST

	// synchronization event position
	angle_t vvtPosition[BANKS_COUNT][CAMS_PER_BANK];

#if EFI_SHAFT_POSITION_INPUT
	PrimaryTriggerDecoder triggerState;
#endif //EFI_SHAFT_POSITION_INPUT

	TriggerWaveform triggerShape;

	VvtTriggerDecoder vvtState[BANKS_COUNT][CAMS_PER_BANK] = {
		{
			"VVT B1 Int",
#if CAMS_PER_BANK >= 2
			"VVT B1 Exh"
#endif
		},
#if BANKS_COUNT >= 2
		{
			"VVT B2 Int",
#if CAMS_PER_BANK >= 2
			"VVT B1 Exh"
#endif
		}
#endif
	};

	TriggerWaveform vvtShape[CAMS_PER_BANK];

	TriggerFormDetails triggerFormDetails;

	// Keep track of the last time we got a valid trigger event
	Timer m_lastEventTimer;

	/**
	 * this is based on engineSnifferRpmThreshold settings and current RPM
	 */
	bool isEngineSnifferEnabled = false;

	void applyCamGapOverride();
	bool isMapCamSync(efitick_t nowNt, float currentPhase);
private:
	void decodeMapCam(int triggerIndexForListeners, efitick_t nowNt, float currentPhase);
	void applyTriggerGapOverride();

	bool isToothExpectedNow(efitick_t timestamp);

	// Time since the last tooth
	Timer m_lastToothTimer;
	// Phase of the last tooth relative to the sync point
	float m_lastToothPhaseFromSyncPoint;

	// At what engine phase do we expect the next tooth to arrive?
	// Used for checking whether your trigger pattern is correct.
	expected<float> expectedNextPhase = unexpected;
};

void triggerInfo(void);
void hwHandleShaftSignal(int signalIndex, bool isRising, efitick_t timestamp);
void handleShaftSignal(int signalIndex, bool isRising, efitick_t timestamp);
void hwHandleVvtCamSignal(TriggerValue front, efitick_t timestamp, int index);
void hwHandleVvtCamSignal(bool isRising, efitick_t timestamp, int index);
void handleVvtCamSignal(TriggerValue front, efitick_t timestamp, int index);

void validateTriggerInputs();

void initTriggerCentral();

int isSignalDecoderError(void);

void onConfigurationChangeTriggerCallback();

#define SYMMETRICAL_CRANK_SENSOR_DIVIDER (2 * 2)
#define SYMMETRICAL_THREE_TIMES_CRANK_SENSOR_DIVIDER (3 * 2)
#define SYMMETRICAL_SIX_TIMES_CRANK_SENSOR_DIVIDER (6 * 2)
#define SYMMETRICAL_TWELVE_TIMES_CRANK_SENSOR_DIVIDER (12 * 2)

TriggerCentral * getTriggerCentral();
int getCrankDivider(operation_mode_e operationMode);

constexpr bool isTriggerUpEvent(trigger_event_e event) {
	switch (event) {
		case SHAFT_PRIMARY_FALLING:
		case SHAFT_SECONDARY_FALLING:
			return false;
		case SHAFT_PRIMARY_RISING:
		case SHAFT_SECONDARY_RISING:
			return true;
	}

	return false;
}
