/**
 * @file	trigger_decoder.h
 *
 * @date Dec 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "trigger_structure.h"
#include "trigger_state_generated.h"
#include "trigger_state_primary_generated.h"
#include "timer.h"

const char *getTrigger_event_e(trigger_event_e value);

struct TriggerStateListener {
#if EFI_SHAFT_POSITION_INPUT
	virtual void OnTriggerStateProperState(efitick_t nowNt) = 0;
	virtual void OnTriggerSyncronization(bool wasSynchronized, bool isDecodingError) = 0;
	virtual void OnTriggerSynchronizationLost() = 0;
#endif // EFI_SHAFT_POSITION_INPUT
};

class TriggerConfiguration {
public:
	explicit TriggerConfiguration(const char* printPrefix) : PrintPrefix(printPrefix) {}
	void update();

	const char* const PrintPrefix;
	bool VerboseTriggerSynchDetails;
	trigger_config_s TriggerType;

protected:
	virtual bool isVerboseTriggerSynchDetails() const = 0;
	virtual trigger_config_s getType() const = 0;
};

class PrimaryTriggerConfiguration final : public TriggerConfiguration {
public:
	PrimaryTriggerConfiguration() : TriggerConfiguration("TRG ") {}

protected:
	bool isVerboseTriggerSynchDetails() const override;
	trigger_config_s getType() const override;
};

class VvtTriggerConfiguration final : public TriggerConfiguration {
public:
	VvtTriggerConfiguration(const char * prefix, const int index)
		: TriggerConfiguration(prefix)
		, m_index(index)
	{
	}

protected:
	bool isVerboseTriggerSynchDetails() const override;
	trigger_config_s getType() const override;

private:
	const int m_index;
};

typedef struct {
	/**
	 * index within trigger revolution, from 0 to trigger event count
	 */
	uint32_t current_index;
	/**
	 * Number of actual events of each channel within current trigger cycle, these
	 * values are used to detect trigger signal errors.
	 * see TriggerWaveform
	 */
	size_t eventCount[PWM_PHASE_MAX_WAVE_PER_PWM];

} current_cycle_state_s;

struct TriggerDecodeResult {
	uint32_t CurrentIndex;
};

/**
 * @see TriggerWaveform for trigger wheel shape definition
 */
class TriggerDecoderBase : public trigger_state_s {
public:
	TriggerDecoderBase(const char* name);
	/**
	 * current trigger processing index, between zero and #size
	 */
	int getCurrentIndex() const;
	int getCrankSynchronizationCounter() const;
	/**
	 * this is important for crank-based virtual trigger and VVT magic
	 */
	void incrementShaftSynchronizationCounter();

#if EFI_UNIT_TEST
	/**
	 * used for trigger export only
	 */
	float gapRatio[PWM_PHASE_MAX_COUNT * 6];
#endif // EFI_UNIT_TEST

	int64_t getTotalEventCounter() const;

	expected<TriggerDecodeResult> decodeTriggerEvent(
			const char *msg,
			const TriggerWaveform& triggerShape,
			TriggerStateListener* triggerStateListener,
			const TriggerConfiguration& triggerConfiguration,
			const trigger_event_e signal,
			const efitick_t nowNt);

	void onShaftSynchronization(
			bool wasSynchronized,
			const efitick_t nowNt,
			const TriggerWaveform& triggerShape);

	bool isValidIndex(const TriggerWaveform& triggerShape) const;

	/**
	 * TRUE if we know where we are
	 */
	bool shaft_is_synchronized;
	efitick_t mostRecentSyncTime;

	Timer previousEventTimer;

	void setTriggerErrorState();

	/**
	 * current duration at index zero and previous durations are following
	 */
	uint32_t toothDurations[GAP_TRACKING_LENGTH + 1];

	efitick_t toothed_previous_time;

	current_cycle_state_s currentCycle;

	/**
	 * how many times since ECU reboot we had unexpected number of teeth in trigger cycle
	 */
	uint32_t totalTriggerErrorCounter;
	uint32_t orderingErrorCounter;

	virtual void resetState();
	void setShaftSynchronized(bool value);
	bool getShaftSynchronized();

	/**
	 * this is start of real trigger cycle
	 * for virtual double trigger see timeAtVirtualZeroNt
	 */
	efitick_t startOfCycleNt;

	uint32_t findTriggerZeroEventIndex(
			TriggerWaveform& shape,
			const TriggerConfiguration& triggerConfiguration
			);

	bool someSortOfTriggerError() const {
		return !m_timeSinceDecodeError.hasElapsedSec(1);
	}

protected:
	// Called when some problem is detected with trigger decoding.
	// That means either:
	//  - Too many events without a sync point
	//  - Saw a sync point but the wrong number of events in the cycle
	virtual void onTriggerError() { }

	virtual void onNotEnoughTeeth(int, int) { }
	virtual void onTooManyTeeth(int, int) { }

	const char* const m_name;

private:
	void resetCurrentCycleState();
	bool isSyncPoint(const TriggerWaveform& triggerShape, trigger_type_e triggerType) const;

	bool validateEventCounters(const TriggerWaveform& triggerShape) const;

	trigger_event_e prevSignal;
	int64_t totalEventCountBase;

	bool isFirstEvent;

	Timer m_timeSinceDecodeError;
};

/**
 * the reason for sub-class is simply to save RAM but not having statistics in the trigger initialization instance
 */
class PrimaryTriggerDecoder : public TriggerDecoderBase, public trigger_state_primary_s {
public:
	PrimaryTriggerDecoder(const char* name);
	void resetState() override;

	void resetHasFullSync() {
		// If this trigger doesn't need disambiguation, we already have phase sync
		m_hasSynchronizedPhase = !m_needsDisambiguation;
	}

	angle_t syncEnginePhase(int divider, int remainder, angle_t engineCycle);

	bool hasSynchronizedPhase() const;

	void setNeedsDisambiguation(bool needsDisambiguation) {
		m_needsDisambiguation = needsDisambiguation;

		resetHasFullSync();
	}

	void onTriggerError() override;

	void onNotEnoughTeeth(int actual, int expected) override;
	void onTooManyTeeth(int actual, int expected) override;

private:

	bool m_needsDisambiguation = false;
};

class VvtTriggerDecoder : public TriggerDecoderBase {
public:
	VvtTriggerDecoder(const char* name) : TriggerDecoderBase(name) { }

	void onNotEnoughTeeth(int actual, int expected) override;
	void onTooManyTeeth(int actual, int expected) override;
};

angle_t getEngineCycle(operation_mode_e operationMode);
