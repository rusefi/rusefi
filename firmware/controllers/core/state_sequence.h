/**
 * @file	state_sequence.h
 *
 * @date May 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include <stdint.h>
#include "rusefi_enums.h"
#include <rusefi/expected.h>

enum class TriggerValue : uint8_t {
	FALL = 0,
	RISE = 1
};

// see also 'HW_EVENT_TYPES'
typedef enum {
	SHAFT_PRIMARY_FALLING = 0,
	SHAFT_PRIMARY_RISING = 1,
	SHAFT_SECONDARY_FALLING = 2,
	SHAFT_SECONDARY_RISING = 3,
} trigger_event_e;

/**
 * This layer has two primary usages:
 * 1) 'simple' PWM generation is used to produce actuator square control wave
 * 2) 'complex' PWM generation is used for trigger simulator.
 * Some triggers like Nissan 360 slot optical wheel need a lot of points to describe the shape of the wave.
 * Looks like 252 is explained by 60 tooth * 2 (number of fronts) * 2 (number of crank rotations within engine cycle)
 */
#ifndef PWM_PHASE_MAX_COUNT
// as of April 2020, trigger which requires most array length is REMIX_66_2_2_2
// we can probably reduce RAM usage if we have more custom logic of triggers with large number of tooth while
// pretty easy logic. like we do not need to REALLY have an array to remember the shape of evenly spaces 360 or 60/2 trigger :)
// todo https://github.com/rusefi/rusefi/issues/3003
// as of 2025, stm32f4 is kind of on the way out - we are focusing on stm32*7 where we have double the RAM amount
#define PWM_PHASE_MAX_COUNT 280
#endif /* PWM_PHASE_MAX_COUNT */
// todo: rename to TRIGGER_CHANNEL_COUNT?
#define PWM_PHASE_MAX_WAVE_PER_PWM 2

typedef TriggerValue pin_state_t;

/**
 * This class represents multi-channel logical signals with shared time axis
 *
 * This is a semi-abstract interface so that implementations can exist for either regularized
 * patterns (60-2, etc) or completely arbitrary patterns stored in arrays.
 */
class MultiChannelStateSequence {
public:
	/**
	 * values in the (0..1] range which refer to points within the period at at which pin state
	 * should be changed So, in the simplest case we turn pin off at 0.3 and turn it on at 1 -
	 * that would give us a 70% duty cycle PWM
	 */
	virtual float getSwitchTime(int phaseIndex) const = 0;
	virtual pin_state_t getChannelState(int channelIndex, int phaseIndex) const = 0;

	// Make sure the switch times are in order and end at the very end.
	void checkSwitchTimes(float scale) const;

	// Find the exact angle, or unexpected if it doesn't exist
	expected<int> findAngleMatch(float angle) const;

	// returns the index at which given value would need to be inserted into sorted array
	int findInsertionAngle(float angle) const;

	uint16_t phaseCount = 0; // Number of timestamps
	uint16_t waveCount = 0; // Number of waveforms
};

template<unsigned max_phase>
class MultiChannelStateSequenceWithData : public MultiChannelStateSequence {
public:
	float getSwitchTime(int phaseIndex) const override {
		return switchTimes[phaseIndex];
	}

	pin_state_t getChannelState(int channelIndex, int phaseIndex) const override {
		if (channelIndex >= waveCount) {
			// todo: would be nice to get this asserting working
			//criticalError("channel index %d/%d", channelIndex, waveCount);
		}
		return ((waveForm[phaseIndex] >> channelIndex) & 1) ? TriggerValue::RISE : TriggerValue::FALL;
	}

	void reset() {
		waveCount = 0;
	}

	void setSwitchTime(const int phaseIndex, const float value) {
		switchTimes[phaseIndex] = value;
	}

	void setChannelState(const int channelIndex, const int phaseIndex, pin_state_t state) {
		if (channelIndex >= waveCount) {
			// todo: would be nice to get this asserting working
			//criticalError("channel index %d/%d", channelIndex, waveCount);
		}
		uint8_t & ref = waveForm[phaseIndex];
		ref = (ref & ~(1U << channelIndex)) | ((state == TriggerValue::RISE ? 1 : 0) << channelIndex);
	}

private:
	float switchTimes[max_phase];
	uint8_t waveForm[max_phase];
};

