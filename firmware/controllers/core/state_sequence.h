/**
 * @file	state_sequence.h
 *
 * @date May 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "rusefi_enums.h"
#include <new>
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

template<typename base_t, unsigned n_elem, typename... tail_t>
class StaticAllocWrapper {
public:
	template<typename... Args>
	StaticAllocWrapper(Args&&... args)
	{
		// Placement new, don't worry - no dynamic memory allocated here
		// Always pass # elements to child, hopefully it can do something with it
		new (m_data) base_t(n_elem, std::forward<Args>(args)...);
	}

	base_t * operator->() {
		return reinterpret_cast<base_t *>(m_data);
	}

	base_t & operator*() {
		return *reinterpret_cast<base_t *>(m_data);
	}

	base_t const * operator->() const {
		return reinterpret_cast<base_t const *>(m_data);
	}

	base_t const & operator*() const {
		return *reinterpret_cast<base_t const *>(m_data);
	}

private:
	template<typename type_t, typename more_t, typename... rest_t>
	static constexpr unsigned get_size(unsigned base_size) {
		return get_size<more_t, rest_t...>(get_size<type_t>(base_size));
	}

	template<typename type_t>
	static constexpr unsigned get_size(unsigned base_size) {
		// realign
		base_size = (base_size + alignof(type_t) - 1U) & ~(alignof(type_t) - 1U);
		return base_size + n_elem * sizeof(type_t);
	}

	uint8_t m_data[get_size<tail_t...>(sizeof(base_t))];
};

/**
 * This class represents multi-channel logical signals with shared time axis
 *
 */
class MultiChannelStateSequence {
protected:
	explicit MultiChannelStateSequence(unsigned maxWaveCount);

	template<typename, unsigned, typename...>
	friend class StaticAllocWrapper;

public:
	void reset();
	float getSwitchTime(const int phaseIndex) const;
	void setSwitchTime(const int phaseIndex, const float value);
	void checkSwitchTimes(const float scale) const;
	pin_state_t getChannelState(const int channelIndex, const int phaseIndex) const;
	void setChannelState(const int channelIndex, const int phaseIndex, pin_state_t state);

	int findAngleMatch(const float angle) const;
	int findInsertionAngle(const float angle) const;

	/**
	 * Number of signal channels
	 */
	uint8_t * const wavePtr;
	uint16_t phaseCount;
	uint16_t waveCount;
private:
	/**
	 * values in the (0..1] range which refer to points within the period at at which pin state should be changed
	 * So, in the simplest case we turn pin off at 0.3 and turn it on at 1 - that would give us a 70% duty cycle PWM
	 */
	float switchTimes[];
	// uint8_t wave[] comes after
};

template<unsigned max_phase>
class MultiChannelStateSequenceWithData
	: public StaticAllocWrapper<MultiChannelStateSequence, max_phase, float, uint8_t> {
};

