/**
 * @file	state_sequence.h
 *
 * @date May 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "rusefi_enums.h"
#include <stdint.h>

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
#define PWM_PHASE_MAX_COUNT 280
#endif /* PWM_PHASE_MAX_COUNT */
#define PWM_PHASE_MAX_WAVE_PER_PWM 3

/**
 * int8_t is probably less efficient then int32_t but we need
 * to reduce memory footprint
 *
 * todo: migrate to bit-array to save memory?
 * this would cost some CPU cycles. see std::vector<bool>
 */
typedef trigger_value_e pin_state_t;

/**
 * This class represents one channel of a digital signal state sequence
 * Each element represents either a HIGH or LOW state - while at the moment this
 * is not implemented using a bit array, it could absolutely be a bit array
 *
 * This sequence does not know anything about signal lengths - only signal state at a given index
 * This sequence can have consecutive zeros and ones since these sequences work as a group within MultiChannelStateSequence
 *
 * @brief   PWM configuration for the specific output pin
 */
class SingleChannelStateSequence {
public:
	SingleChannelStateSequence();
	explicit SingleChannelStateSequence(pin_state_t *pinStates);
	void init(pin_state_t *pinStates);
	/**
	 * todo: confirm that we only deal with two states here, no magic '-1'?
	 * @return HIGH or LOW state at given index
	 */
	pin_state_t getState(int switchIndex) const;
	void setState(int switchIndex, pin_state_t state);

	// todo: make this private by using 'getState' and 'setState' methods
	pin_state_t *pinStates;
};

/**
 * This class represents multi-channel logical signals with shared time axis
 *
 */
class MultiChannelStateSequence {
public:
	MultiChannelStateSequence();
	MultiChannelStateSequence(float *switchTimes, SingleChannelStateSequence *waves);
	void init(float *switchTimes, SingleChannelStateSequence *waves);
	void reset(void);
	float getSwitchTime(const int phaseIndex) const;
	void setSwitchTime(const int phaseIndex, const float value);
	void checkSwitchTimes(const float scale) const;
	pin_state_t getChannelState(const int channelIndex, const int phaseIndex) const;

	int findAngleMatch(const float angle) const;
	int findInsertionAngle(const float angle) const;

	/**
	 * Number of signal channels
	 */
	uint16_t phaseCount;
	uint16_t waveCount;
	SingleChannelStateSequence *channels = nullptr;
//private:
	/**
	 * values in the (0..1] range which refer to points within the period at at which pin state should be changed
	 * So, in the simplest case we turn pin off at 0.3 and turn it on at 1 - that would give us a 70% duty cycle PWM
	 */
	float *switchTimes = nullptr;
};


