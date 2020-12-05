#pragma once

#include "efitime.h"

class Timer {
public:
	void reset();

	// Reset the timer to a known timestamp (don't take a timestamp internally)
	void reset(efitick_t nowNt);

	bool hasElapsedSec(float seconds) const;
	bool hasElapsedMs(float ms) const;
	bool hasElapsedUs(float us) const;

	// Return the elapsed time since the last reset.
	// If the elapsed time is longer than 2^32 timer tick counts,
	// then a time period representing 2^32 counts will be returned.
	float getElapsedSeconds() const;

private:
	efitick_t m_lastReset = INT64_MIN;
};
