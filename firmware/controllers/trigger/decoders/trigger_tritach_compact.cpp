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
#include <algorithm>

angle_t TriTachCompactConfig::riseAngle(int toothIndex) const {
	float base = offset + static_cast<float>(toothIndex) * step();
	float rise = base + step() * (1.0f - toothWidthFraction);
	// normalize to [0, engineCycle)
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

void configureTriTachCompact(TriggerWaveform *s) {
	// follow existing configureTriTach behavior in terms of meta flags
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);
	s->isSynchronizationNeeded = false;
	s->shapeWithoutTdc = true;
	s->useOnlyPrimaryForSync = true;
	s->useOnlyRisingEdges = true;
	s->gapTrackingLength = 1;
	// Do NOT add per-tooth events here — keep it compact.
}

void exportTriTachToWaveform(TriggerWaveform *s, int maxEvents) {
	// This function is intended for host/test builds only (gen_trigger_images.bat),
	// and will populate up to maxEvents events so you can visualize shape.
	TriTachCompactConfig cfg;
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);
	s->isSynchronizationNeeded = false;
	s->shapeWithoutTdc = false;
	// If TriggerWaveform exposes a way to clear existing events, use it; otherwise the caller
	// should pass a fresh instance. We attempt best-effort only.
	// s->clear(); // uncomment if available

	int limitPairs = std::min<int>(cfg.totalTeethCount, std::max(0, maxEvents / 2));
	for (int i = 0; i < limitPairs; ++i) {
		// addEvent720 takes an angle in engine-cycle degrees (0..720)
		float r = static_cast<float>(cfg.riseAngle(i));
		float f = static_cast<float>(cfg.fallAngle(i));
		s->addEvent720(r, TriggerValue::RISE);
		s->addEvent720(f, TriggerValue::FALL);
	}
	// mark version bump so other tools notice
	s->version++;
}

/*
 * Runtime decoder skeleton:
 *
 * - The main missing piece for complete runtime integration is timestamp -> estimated cycle angle conversion.
 *   rusefi provides timer/instant RPM helpers (InstantRpmCalculator / timer utilities). Use those to estimate
 *   where in the engine cycle the current edge landed.
 * - Once you have angle, use config.angleToToothIndex(angle) to find the nearest tooth and compare
 *   with lastToothIndex for gap detection (a large delta = sync gap).
 *
 * TODO: wire computeAngleFromTimestamp using platform timer + instant RPM logic.
 */
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

expected<TriggerDecodeResult> TriTachRuntimeDecoder::onEdge(
	TriggerDecoderBase* base,
	const TriggerConfiguration& triggerConfiguration,
	trigger_event_e signal,
	const efitick_t nowNt) {

	// IMPORTANT: The next step requires mapping nowNt (ticks) -> engine-cycle angle.
	// Two common approaches:
	//  - Use instant RPM (TriggerCentral::instantRpm or InstantRpmCalculator) to convert delta-time to fraction of cycle.
	//  - Use pre-existing trigger timing machinery from TriggerDecoderBase if available.
	//
	// PSEUDO:
	//   float angle = computeAngleFromTimestamp(nowNt, base->...);
	// For now we'll leave a clear TODO showing where to integrate.

	// TODO: convert timestamp -> angle (engine-cycle degrees), using rusefi timing utilities
	// Example pseudo-call (replace with real API):
	// angle_t angle = computeCycleAngleFromTimestamp(nowNt);

	// Temporary placeholder: we cannot compute angle without engine timing utilities.
	// Return an error to indicate the decoder must be wired.
	(void)base; (void)triggerConfiguration; (void)signal; (void)nowNt;
	return unexpected<std::string>("TriTachRuntimeDecoder::onEdge: timestamp->angle conversion not implemented");
}
