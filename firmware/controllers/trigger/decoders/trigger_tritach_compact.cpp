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

// Map project tick->microsecond macro to internal name for clarity.
// Unit tests use a TicksToUs macro; adapt if your project uses something else.
#ifdef TicksToUs
    #define TICKS_TO_US(t) TicksToUs(t)
#else
    #error "TicksToUs(...) macro not defined for your build. Replace TICKS_TO_US(...) usages with your tick->us conversion."
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
