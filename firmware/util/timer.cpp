#include "timer.h"
#include "global.h"

void Timer::reset() {
	m_lastReset = getTimeNowNt();
}

void Timer::reset(efitick_t nowNt) {
	m_lastReset = nowNt;
}

bool Timer::hasElapsedSec(float seconds) const {
	return hasElapsedMs(seconds * 1e3);
}

bool Timer::hasElapsedMs(float milliseconds) const {
	return hasElapsedUs(milliseconds * 1e3);
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
	auto delta = getTimeNowNt() - m_lastReset;

	if (delta > UINT32_MAX - 1) {
		delta = UINT32_MAX - 1;
	}

	auto delta32 = (uint32_t)delta;

	return NT2US(delta32);
}
