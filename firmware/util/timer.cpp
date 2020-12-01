#include "timer.h"
#include "global.h"

void Timer::reset() {
	m_lastReset = getTimeNowNt();
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
