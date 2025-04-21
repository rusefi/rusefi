#pragma once

void initMainLoop();

enum class LoopPeriod : uint8_t {
	None = 0,
	Period1000hz = 1 << 0,
	Period500hz = 1 << 1,
	Period250hz = 1 << 2,
	Period20hz = 1 << 3,
};

inline constexpr LoopPeriod& operator|=(LoopPeriod& a, const LoopPeriod& b) {
	a = static_cast<LoopPeriod>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	return a;
}

inline constexpr bool operator&(LoopPeriod a, LoopPeriod b) {
	return 0 != (static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

constexpr int hzForPeriod(LoopPeriod p) {
	switch (p) {
		case LoopPeriod::None: return 0;
		case LoopPeriod::Period1000hz: return 1000;
		case LoopPeriod::Period500hz: return 500;
		case LoopPeriod::Period250hz: return 250;
		case LoopPeriod::Period20hz: return 20;
	}

	return 0;
}

constexpr float loopPeriodMs(LoopPeriod p) {
	return 1000.0f / hzForPeriod(p);
}

#ifndef ADC_UPDATE_RATE
#define ADC_UPDATE_RATE LoopPeriod::Period500hz
#endif

#define ETB_UPDATE_RATE LoopPeriod::Period500hz
#define FAST_CALLBACK_RATE LoopPeriod::Period250hz
#define SLOW_CALLBACK_RATE LoopPeriod::Period20hz

#define FAST_CALLBACK_PERIOD_MS loopPeriodMs(FAST_CALLBACK_RATE)
#define SLOW_CALLBACK_PERIOD_MS loopPeriodMs(SLOW_CALLBACK_RATE)
