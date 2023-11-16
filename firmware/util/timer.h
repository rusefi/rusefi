/**
 * @file timer.h
 */

#pragma once

#include "efitime.h"

/**
 * Helper class with "has X amount of time elapsed since most recent reset" methods
 * Brand new instances have most recent reset time far in the past, i.e. "hasElapse" is true for any reasonable range
 */
class Timer final {
public:
	Timer();
	void reset();

	// Reset the timer to a known timestamp (don't take a timestamp internally)
	void reset(efitick_t nowNt);
	// returns timer to the most original-as-constructed state
	void init();

	bool hasElapsedSec(float seconds) const;
	bool hasElapsedMs(float ms) const;
	bool hasElapsedUs(float us) const;

	// Return the elapsed time since the last reset.
	// If the elapsed time is longer than 2^32 timer tick counts,
	// then a time period representing 2^32 counts will be returned.
	float getElapsedSeconds() const;
	float getElapsedSeconds(efitick_t nowNt) const;
	float getElapsedUs() const;
	// WOW yes returns US while parameter is NT
	float getElapsedUs(efitick_t nowNt) const;

	// Perform an atomic update event based on the passed timestamp,
	// returning the delta between the last reset and the provided timestamp
	float getElapsedSecondsAndReset(efitick_t nowNt);

private:
	efitick_t m_lastReset;
};
