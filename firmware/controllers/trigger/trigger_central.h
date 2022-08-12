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
#include "trigger_central_generated.h"
#include "timer.h"
#include "pin_repository.h"
#include "local_version_holder.h"

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
	void handleShaftSignal(trigger_event_e signal, efitick_t timestamp);
	int getHwEventCounter(int index) const;
	void resetCounters();
	void validateCamVvtCounters();

	LocalVersionHolder triggerVersion;

	angle_t mapCamPrevToothAngle = -1;
	float mapCamPrevCycleValue = 0;
	int prevChangeAtCycle = 0;

	/**
	 * true if a recent configuration change has changed any of the trigger settings which
	 * we have not adjusted for yet
	 */
	bool triggerConfigChanged = false;

	bool checkIfTriggerConfigChanged();
	bool isTriggerConfigChanged();

	bool isTriggerDecoderError();

	expected<float> getCurrentEnginePhase(efitick_t nowNt) const;

	float getSecondsSinceTriggerEvent(efitick_t nowNt) const {
		return m_lastEventTimer.getElapsedSeconds(nowNt);
	}

	bool engineMovedRecently(efitick_t nowNt) const {
		constexpr float oneRevolutionLimitInSeconds = 60.0 / RPM_LOW_THRESHOLD;
		auto maxAverageToothTime = oneRevolutionLimitInSeconds / triggerShape.getSize();

		// Some triggers may have long gaps (with many teeth), don't count that as stopped!
		auto maxAllowedGap = maxAverageToothTime * 10;

		// Clamp between 0.1 seconds ("instant" for a human) and worst case of one engine cycle on low tooth count wheel
		maxAllowedGap = clampF(0.1f, maxAllowedGap, oneRevolutionLimitInSeconds);

		return getSecondsSinceTriggerEvent(nowNt) < maxAllowedGap;
	}

	bool engineMovedRecently() const {
		return engineMovedRecently(getTimeNowNt());
	}

	TriggerNoiseFilter noiseFilter;

	int vvtEventRiseCounter[CAM_INPUTS_COUNT];
	int vvtEventFallCounter[CAM_INPUTS_COUNT];

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

private:
	void decodeMapCam(efitick_t nowNt, float currentPhase);

	// Time since the last tooth
	Timer m_lastToothTimer;
	// Phase of the last tooth relative to the sync point
	float m_lastToothPhaseFromSyncPoint;
};

void triggerInfo(void);
void hwHandleShaftSignal(int signalIndex, bool isRising, efitick_t timestamp);
void handleShaftSignal(int signalIndex, bool isRising, efitick_t timestamp);
void hwHandleVvtCamSignal(trigger_value_e front, efitick_t timestamp, int index);

void validateTriggerInputs();

void initTriggerCentral();

int isSignalDecoderError(void);

void onConfigurationChangeTriggerCallback();

#define SYMMETRICAL_CRANK_SENSOR_DIVIDER 4
#define SYMMETRICAL_THREE_TIMES_CRANK_SENSOR_DIVIDER 6
#define SYMMETRICAL_TWELVE_TIMES_CRANK_SENSOR_DIVIDER 24
