/*
 * trigger_tritach_compact.h
 *
 * Memory-efficient TriTach configuration, export helper and runtime decoder skeleton.
 *
 * Add to: firmware/controllers/trigger/decoders/
 */

#pragma once

#include "trigger_structure.h"

struct TriTachCompactConfig {
	int totalTeethCount = 135;                 // TriTach default
	float toothWidthFraction = 0.5f;           // fraction of tooth angular step (0..1)
	float offset = 0.0f;                       // angular offset (degrees) in engine cycle
	float engineCycle = FOUR_STROKE_ENGINE_CYCLE; // 720 normally

	inline float step() const { return engineCycle / static_cast<float>(totalTeethCount); }

	// compute rise/fall angle for a tooth index (normalized into [0, engineCycle))
	angle_t riseAngle(int toothIndex) const;
	angle_t fallAngle(int toothIndex) const;

	// convert angle -> nearest tooth index
	int angleToToothIndex(angle_t a) const;
};

// configure minimal TriggerWaveform metadata in compact mode (no event allocation)
void configureTriTachCompact(TriggerWaveform *s);

// Export helper ONLY for unit tests / image generation (not for runtime).
// Fills up to maxEvents (pairs) into the TriggerWaveform for visualization.
void exportTriTachToWaveform(TriggerWaveform *s, int maxEvents = 200);

/*
 * Runtime decoder skeleton:
 * Implements a small, self-contained decoder that uses the compact configuration
 * to detect index/gaps on incoming edges. Integrate onEdge into TriggerCentral or
 * adapt to TriggerDecoderBase::decodeTriggerEvent as appropriate.
 */
class TriTachRuntimeDecoder {
public:
	TriTachRuntimeDecoder();

	// Called once at init to configure compact params (or set them directly)
	void init(const TriTachCompactConfig &cfg);

	// Called on each hardware edge. You must provide a way to convert timestamp->cycle angle;
	// TODO: wire computeAngleFromTimestamp into platform instant RPM / timer utilities.
	expected<TriggerDecodeResult> onEdge(
		TriggerDecoderBase* base,
		const TriggerConfiguration& triggerConfiguration,
		trigger_event_e signal,
		const efitick_t nowNt);

private:
	TriTachCompactConfig config;
	int lastToothIndex = -1;
	efitick_t lastEdgeTime = 0;
	// small internal counters for sync/gap detection
	int consecutiveGapCount = 0;
};
