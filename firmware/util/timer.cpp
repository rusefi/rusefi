#include "pch.h"

#include "timer.h"

void Timer::reset() {
	m_lastReset = getTimeNowNt();
}

void Timer::reset(efitick_t nowNt) {
	m_lastReset = nowNt;
}

bool Timer::hasElapsedSec(float seconds) const {
	return hasElapsedMs(seconds * 1000);
}

bool Timer::hasElapsedMs(float milliseconds) const {
	return hasElapsedUs(milliseconds * 1000);
}

bool Timer::hasElapsedUs(float microseconds) const {
	auto delta = getTimeNowNt() - m_lastReset;

	// If larger than 32 bits, timer has certainly expired
	if (delta >= UINT32_MAX) {
		return true;
	}

	auto delta32 = (uint32_t)delta;

	return delta32 > USF2NT(microseconds);
}

float Timer::getElapsedSeconds() const {
	return getElapsedSeconds(getTimeNowNt());
}

float Timer::getElapsedSeconds(efitick_t nowNt) const {
	return 1 / US_PER_SECOND_F * getElapsedUs(nowNt);
}

float Timer::getElapsedUs() const {
	return getElapsedUs(getTimeNowNt());
}

float Timer::getElapsedUs(efitick_t nowNt) const {
	auto deltaNt = nowNt - m_lastReset;

	// Yes, things can happen slightly in the future if we get a lucky interrupt between
	// the timestamp and this subtraction, that updates m_lastReset to what's now "the future",
	// resulting in a negative delta.
	if (deltaNt < 0) {
		return 0;
	}

	if (deltaNt > UINT32_MAX - 1) {
		deltaNt = UINT32_MAX - 1;
	}

	auto delta32 = (uint32_t)deltaNt;

	return NT2US(delta32);
}

float Timer::getElapsedSecondsAndReset(efitick_t nowNt) {
	float result = getElapsedSeconds(nowNt);

	reset(nowNt);

	return result;
}
