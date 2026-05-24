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
#include <rusefi/timer.h>

const char *getTrigger_event_e(trigger_event_e value);
const char *getTrigger_value_e(TriggerValue value);

struct TriggerStateListener {
#if EFI_SHAFT_POSITION_INPUT
	virtual void OnTriggerStateProperState(efitick_t nowNt, size_t triggerStateIndex) = 0;
	virtual void OnTriggerSynchronization(bool wasSynchronized, bool isDecodingError) = 0;
	virtual void OnTriggerSynchronizationLost() = 0;
	// todo: replace this dirty hack with proper collection (linked list?) of listeners
	virtual TriggerStateListener* nextListener() = 0;
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
public:
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
	const int index;

	VvtTriggerConfiguration(const char * prefix, const int p_index) : TriggerConfiguration(prefix), index(p_index) {
	}

protected:
	bool isVerboseTriggerSynchDetails() const override;
	trigger_config_s getType() const override;
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

  void printGaps(const char * prefix,
    const TriggerConfiguration& triggerConfiguration,
    const TriggerWaveform& triggerShape);

	/**
	 * current trigger processing index, between zero and #size
	 */
	int getCurrentIndex() const;
	int getSynchronizationCounter() const;
	/**
	 * this is important for crank-based virtual trigger and VVT magic
	 */
	void incrementShaftSynchronizationCounter();

#if EFI_UNIT_TEST
	/**
	 * used only for trigger export
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
	bool shaft_is_synchronized = false;
	efitick_t mostRecentSyncTime;

	Timer previousEventTimer;

	/**
	 * current duration at index zero and previous durations are following
	 */
	uint32_t toothDurations[GAP_TRACKING_LENGTH + 1];

	efitick_t toothed_previous_time;

	current_cycle_state_s currentCycle;
	const char* const name;

	/**
	 * how many times since ECU reboot we had unexpected number of teeth in trigger cycle
	 */
	uint32_t totalTriggerErrorCounter;
	uint32_t orderingErrorCounter;

	/**
	 * Resets the decoder state to its initial, unsynchronized condition.
	 * This includes clearing synchronization status, error counters, tooth duration history,
	 * and internal event tracking.
	 *
	 * Invoked:
	 *  - During object construction.
	 *  - When trigger synchronization is lost (e.g., engine stopped for >1 second).
	 *  - During trigger initialization/re-initialization when searching for the sync point.
	 */
	virtual void resetState();
	void setShaftSynchronized(bool value);
	bool getShaftSynchronized() const;

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
		return !m_timeSinceDecodeError.hasElapsedSec(0.3);
	}

protected:
	// Called when some problem is detected with trigger decoding.
	// That means either:
	//  - Too many events without a sync point
	//  - Saw a sync point but the wrong number of events in the cycle
	virtual void onTriggerError() { }

	// Called when shaft synchronization state changes.
	virtual void onShaftSynchronized(bool /*value*/) { }

	virtual void onNotEnoughTeeth(int, int) { }
	virtual void onTooManyTeeth(int, int) { }

private:
	void setTriggerErrorState(int errorIncrement = 1);
	void resetCurrentCycleState();
	bool isSyncPoint(const TriggerWaveform& triggerShape, trigger_type_e triggerType) const;

	int getEventCountersError(const TriggerWaveform& triggerShape) const;

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
	/**
	 * Resets the primary decoder state, including its base state and phase synchronization.
	 *
	 * Invoked:
	 *  - During object construction (via TriggerDecoderBase).
	 *  - When trigger synchronization is lost (e.g., engine stopped for >1 second).
	 *  - During trigger initialization/re-initialization when searching for the sync point.
	 */
	void resetState() override;

	void resetHasFullSync() {
		// If this trigger doesn't need disambiguation, we already have phase sync
		m_hasSynchronizedPhase = !m_needsDisambiguation;
	}

	/**
	  * returns zero if we were lucky to have correct engine phase, otherwise angle of engine phase correction which was applied.
	  */
	angle_t syncEnginePhase(int divider, int remainder, angle_t engineCycle);

	// Returns true if syncEnginePhase has been called,
	// i.e. if we have enough VVT information to have full sync on
	// an indeterminate crank pattern
	bool hasSynchronizedPhase() const {
		return m_hasSynchronizedPhase;
	}

	void setNeedsDisambiguation(bool needsDisambiguation) {
		m_needsDisambiguation = needsDisambiguation;

		resetHasFullSync();
	}

	void onTriggerError() override;

	void onShaftSynchronized(bool value) override;

	void onNotEnoughTeeth(int actual, int expected) override;
	void onTooManyTeeth(int actual, int expected) override;

private:

	/**
	 * True when the crank trigger pattern alone is ambiguous and additional information
	 * (typically a cam/VVT signal) is required to determine the engine's true phase within
	 * the full 720-degree four-stroke cycle.
	 *
	 * Many crank-only wheels (e.g. symmetric 60-2) repeat their pattern every 360 crank
	 * degrees, so the decoder can find a sync point on the crank wheel but cannot tell
	 * which of the two engine revolutions of the four-stroke cycle it is currently in.
	 * In that case m_needsDisambiguation is set to true, and m_hasSynchronizedPhase will
	 * only become true once syncEnginePhase() has been called based on a cam signal.
	 *
	 * When false (e.g. asymmetric crank patterns that are unique over the full engine
	 * cycle, or when running in a half-cycle/wasted-spark mode where 360 deg of ambiguity
	 * is acceptable), the decoder is considered fully phase-synchronized as soon as the
	 * crank sync point is found - see resetHasFullSync().
	 */
	bool m_needsDisambiguation = false;
};

class VvtTriggerDecoder : public TriggerDecoderBase {
public:
	VvtTriggerDecoder(const char* p_name) : TriggerDecoderBase(p_name) { }

	void onNotEnoughTeeth(int actual, int expected) override;
	void onTooManyTeeth(int actual, int expected) override;
};

angle_t getEngineCycle(operation_mode_e operationMode);
