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
		float absError = absF(lhs - rhs);

		// If there's enough error, actually compare values
		if (absError > TDeadband) {
			m_lastState = lhs > rhs;
		}

		return m_lastState;
	}

private:
	bool m_lastState =false;
};
