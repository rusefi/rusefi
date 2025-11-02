/*
 * trigger_tritach_compact.cpp
 *
 * Compact TriTach helper + runtime decoder skeleton.
 *
 * Place in: firmware/controllers/trigger/decoders/
 *
 * Notes:
 * - Keep runtime memory usage tiny (only a few ints/floats).
 * - The exportTriTachToWaveform() function is intended only for unit test / image generation.
 * - The runtime onEdge() skeleton must be wired into TriggerCentral/TriggerDecoderBase decode path.
 */

#include "pch.h"
#include "trigger_tritach_compact.h"
#include "trigger_universal.h"
#include "trigger_decoder.h"
#include "trigger_central.h"
#include <cmath>
#include <string>
#include <algorithm>
#include <cstdint>

/*
 * Portable ticks -> microseconds conversion.
 *
 * Reason: unit tests and some host-side code define TicksToUs(...) while firmware
 * build for various boards may not expose that macro. To allow repository-wide
 * builds (CI matrix) we provide a sequence of fallbacks:
 *
 * 1) If TicksToUs macro is available (unit tests / some test files), use it.
 * 2) If TIMER_CLOCK (ticks per second) is defined by a board header, use it.
 * 3) If CORE_CLOCK + TIMER_PRESCALER or HW_TIMER_DIVIDER are available, use those.
 * 4) Otherwise provide a minimal portable fallback function ticksToUsPlatform().
 *
 * IMPORTANT: the fallback assumes 1 tick == 1 microsecond so it is only a
 * compile-friendly placeholder. Replace ticksToUsPlatform() with the correct
 * formula for each board (or include the proper header) for accurate runtime.
 */

#ifdef TicksToUs
    #define TICKS_TO_US(t) TicksToUs(t)
#elif defined(TIMER_CLOCK)
    // TIMER_CLOCK expected to be ticks-per-second (Hz)
    #define TICKS_TO_US(t) (((float)(t)) * 1000000.0f / (float)(TIMER_CLOCK))
#elif defined(CORE_CLOCK) && defined(TIMER_PRESCALER)
    // If your board defines CORE_CLOCK and a timer prescaler constant
    #define TICKS_TO_US(t) (((float)(t)) * 1000000.0f / ((float)(CORE_CLOCK) / (float)(TIMER_PRESCALER)))
#elif defined(CORE_CLOCK) && defined(HW_TIMER_DIVIDER)
    // Alternative board-specific divider macro (example)
    #define TICKS_TO_US(t) (((float)(t)) * 1000000.0f / ((float)(CORE_CLOCK) / (float)(HW_TIMER_DIVIDER)))
#else
    // Portable fallback - compiles everywhere but MUST be replaced with a board-accurate conversion.
    static inline float ticksToUsPlatform(efitick_t ticks) {
        // Placeholder conversion: assumes timer increments at 1 MHz (1 tick = 1 us).
        // For correct runtime behavior replace with actual timer frequency formula, for example:
        //   return (float)ticks * 1000000.0f / (CORE_CLOCK / TIMER_PRESCALER);
        // or include board-specific header that defines a TicksToUs/TimerClock conversion.
        return (float)ticks;
    }
    #define TICKS_TO_US(t) ticksToUsPlatform(t)
#endif

// --- Compact config helpers ---
angle_t TriTachCompactConfig::riseAngle(int toothIndex) const {
	float base = offset + static_cast<float>(toothIndex) * step();
	float rise = base + step() * (1.0f - toothWidthFraction);
	while (rise >= engineCycle) rise -= engineCycle;
	while (rise < 0.0f) rise += engineCycle;
	return angle_t(rise);
}

angle_t TriTachCompactConfig::fallAngle(int toothIndex) const {
	float base = offset + static_cast<float>(toothIndex) * step();
	float f = base + step();
	while (f >= engineCycle) f -= engineCycle;
	while (f < 0.0f) f += engineCycle;
	return angle_t(f);
}

int TriTachCompactConfig::angleToToothIndex(angle_t a) const {
	float rel = static_cast<float>(a) - offset;
	while (rel < 0.0f) rel += engineCycle;
	while (rel >= engineCycle) rel -= engineCycle;
	int idx = static_cast<int>(rel / step() + 0.5f);
	idx %= totalTeethCount;
	if (idx < 0) idx += totalTeethCount;
	return idx;
}

// --- configureTriTachCompact (keeps TriggerWaveform metadata compact) ---
void configureTriTachCompact(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);
	s->isSynchronizationNeeded = false;
	s->shapeWithoutTdc = true;
	s->useOnlyPrimaryForSync = true;
	s->useOnlyRisingEdges = true;
	s->gapTrackingLength = 1;
	// Do NOT add per-tooth events here — keep it compact.
}

// Optional test export (use for gen_trigger_images only)
void exportTriTachToWaveform(TriggerWaveform *s, int maxEvents) {
	TriTachCompactConfig cfg;
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);
	s->isSynchronizationNeeded = false;
	s->shapeWithoutTdc = false;

	int limitPairs = std::min<int>(cfg.totalTeethCount, std::max(0, maxEvents / 2));
	for (int i = 0; i < limitPairs; ++i) {
		float r = static_cast<float>(cfg.riseAngle(i));
		float f = static_cast<float>(cfg.fallAngle(i));
		s->addEvent720(r, TriggerValue::RISE);
		s->addEvent720(f, TriggerValue::FALL);
	}
	s->version++;
}

// --- Runtime decoder implementation ---
TriTachRuntimeDecoder::TriTachRuntimeDecoder() {
	config = TriTachCompactConfig();
	lastToothIndex = -1;
	lastEdgeTime = 0;
	consecutiveGapCount = 0;
}

void TriTachRuntimeDecoder::init(const TriTachCompactConfig &cfg) {
	config = cfg;
	lastToothIndex = -1;
	lastEdgeTime = 0;
	consecutiveGapCount = 0;
}

// full onEdge implementation
expected<TriggerDecodeResult> TriTachRuntimeDecoder::onEdge(
	TriggerDecoderBase* base,
	const TriggerConfiguration& triggerConfiguration,
	trigger_event_e signal,
	const efitick_t nowNt) {

	TriggerCentral *tc = getTriggerCentral();
	float rpm = 0.0f;
	if (tc) {
		rpm = tc->instantRpm.getInstantRpm();
	}

	if (rpm <= 0.0f) {
		lastEdgeTime = nowNt;
		return unexpected<std::string>("No RPM yet");
	}

	const float cycleUs = 120000000.0f / rpm; // 120e6 / rpm -> microseconds for 720° (2 revs)
	float expectedToothPeriodUs = cycleUs / static_cast<float>(config.totalTeethCount);

	efitick_t dtTicks = nowNt - lastEdgeTime;
	float dtUs = TICKS_TO_US(dtTicks);

	if (lastEdgeTime == 0) {
		lastEdgeTime = nowNt;
		lastToothIndex = -1;
		return unexpected<std::string>("Initial edge recorded");
	}

	float fraction = dtUs / cycleUs;
	if (!std::isfinite(fraction)) fraction = 0.0f;
	if (fraction < 0.0f) fraction = 0.0f;
	if (fraction > 2.0f) fraction = fmodf(fraction, 1.0f);

	float referenceAngleF = 0.0f;
	if (lastToothIndex >= 0) {
		referenceAngleF = static_cast<float>(config.riseAngle(lastToothIndex));
	} else {
		referenceAngleF = 0.0f;
	}

	float currentAngleF = referenceAngleF + fraction * config.engineCycle;
	while (currentAngleF >= config.engineCycle) currentAngleF -= config.engineCycle;
	while (currentAngleF < 0.0f) currentAngleF += config.engineCycle;
	angle_t currentAngle = angle_t(currentAngleF);

	int computedIndex = config.angleToToothIndex(currentAngle);

	float ratio = (expectedToothPeriodUs > 0.0f) ? (dtUs / expectedToothPeriodUs) : 0.0f;

	bool isGap = false;
	const float GAP_RATIO_THRESHOLD = 2.5f;
	if (ratio > GAP_RATIO_THRESHOLD) {
		isGap = true;
	} else if (lastToothIndex >= 0) {
		int delta = (computedIndex - lastToothIndex + config.totalTeethCount) % config.totalTeethCount;
		if (delta > 3 && delta < config.totalTeethCount - 3) {
			isGap = true;
		}
	}

	if (isGap) {
		if (tc) {
			TriggerWaveform &ts = tc->triggerShape;
			base->onShaftSynchronization(/*wasSynchronized=*/true, nowNt, ts);
		}
		consecutiveGapCount = 0;
	} else {
		consecutiveGapCount++;
	}

	TriggerDecodeResult res;
	res.CurrentIndex = static_cast<uint32_t>(computedIndex);

	lastToothIndex = computedIndex;
	lastEdgeTime = nowNt;

	return res;
}
