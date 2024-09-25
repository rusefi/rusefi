/**
 * @file    deadband.h
 *
 * @date April 6, 2020
 * @author Matthew Kennedy, (c) 2020
 */

#pragma once

#include "efilib.h"

template <int TDeadband>
class Deadband {
public:
	bool gt(float lhs, float rhs) {
		// If we're within the deadband, be "sticky" with the previous value
		float absError = std::abs(lhs - rhs);

		// If there's enough error, actually compare values
		if (absError > TDeadband) {
			m_lastState = lhs > rhs;
		}

		return m_lastState;
	}

	// Deadband has no concept of equal - only greater and less, so to compute gt, we just swap args
	bool lt(float lhs, float rhs) {
		return gt(rhs, lhs);
	}

private:
	bool m_lastState =false;
};
