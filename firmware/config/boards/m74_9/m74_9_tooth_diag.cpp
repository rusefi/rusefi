/**
 * @file m74_9_tooth_diag.cpp
 *
 * m74_9 trigger-wheel diagnostics: captures primary tooth periods and learns
 * the per-tooth time profile (the "wheel non-uniformity" the stock ECU also
 * learns - VR threshold offsets + compression ripple make each tooth's time
 * share of the revolution slightly but consistently different).
 *
 * Consumers:
 *  - 'toothdump' console command: prints the learned profile and the last few
 *    raw revolutions as period lists (paste into the chat, plot externally).
 *  - profileUs[]: future use for gap validation / instant rpm.
 *
 * Capture happens in boardTriggerCallback(), which the weak default in
 * trigger_central.cpp lets the board override; it fires once per decoded
 * (synchronized) primary tooth, 58 times per crank revolution.
 */

#include "pch.h"

#if EFI_PROD_CODE
#include "trigger_central.h"
#include "m74_9_tooth_diag.h"

namespace {

constexpr size_t ToothCount = 58;

// Ring capture of the last few revolutions: {timestamp, tooth index 0..57}
struct ToothEvent {
	efitick_t timestamp;
	uint8_t index;
};

// 6 revolutions of headroom
constexpr size_t RingSize = ToothCount * 6;
ToothEvent ring[RingSize];
size_t ringHead = 0;
size_t ringTotalWritten = 0;

efitick_t lastToothTimestamp = 0;
int lastToothIndex = -1;

// Learned per-tooth period profile: EMA over synchronized revolutions
float profileUs[ToothCount] = {};
uint32_t profileCount[ToothCount] = {};

constexpr float EmaAlpha = 0.05f;

bool isPlausibleToothPeriod(efitick_t period) {
	// One tooth is 6 degrees of crank. Accept roughly 6 rpm .. 6000 rpm.
	return period > NT_PER_SECOND / 6000 && period < NT_PER_SECOND / 6;
}

// Print an array of period values in chunks: efiPrintf lines are capped at
// 256 characters, 58 values do not fit one line.
void printPeriodChunks(const char* label, const float* values, size_t count) {
	constexpr size_t Chunk = 12;
	char line[224];

	for (size_t start = 0; start < count; start += Chunk) {
		size_t end = start + Chunk < count ? start + Chunk : count;
		size_t off = 0;

		off += chsnprintf(line + off, sizeof(line) - off, "%s%02d", label, (int)start);

		for (size_t i = start; i < end; i++) {
			off += chsnprintf(line + off, sizeof(line) - off, " %.0f", values[i]);
		}

		efiPrintf("%s", line);
	}
}

void printToothProfile() {
	// Mean over the regular teeth (the last tooth, index 57, carries the
	// missing-teeth gap interval and would skew the normalization).
	float sum = 0;
	int normalTeeth = 0;
	for (size_t i = 0; i < ToothCount - 1; i++) {
		if (profileCount[i] > 0) {
			sum += profileUs[i];
			normalTeeth++;
		}
	}

	uint32_t revolutionsLearned = 0;
	for (size_t i = 0; i < ToothCount; i++) {
		revolutionsLearned = maxI(revolutionsLearned, profileCount[i]);
	}

	efiPrintf("toothdump profile(us) %d revs learned:", (int)revolutionsLearned);
	printPeriodChunks(" profile", profileUs, ToothCount);

	if (normalTeeth > 0) {
		float mean = sum / normalTeeth;
		float norm[ToothCount];
		for (size_t i = 0; i < ToothCount; i++) {
			norm[i] = profileCount[i] > 0 ? profileUs[i] / mean : 0;
		}

		efiPrintf("toothdump profile(norm, mean=%.1f us):", mean);
		printPeriodChunks(" norm", norm, ToothCount);
	}
}

void printRawRevolutions() {
	// Copy the valid part of the ring into time order (oldest -> newest).
	// Fresh (never written) entries have index 0 and must not be treated as
	// revolution boundaries.
	size_t valid = ringTotalWritten < RingSize ? ringTotalWritten : RingSize;

	ToothEvent ordered[RingSize];
	for (size_t i = 0; i < valid; i++) {
		ordered[i] = ring[(ringHead + RingSize - valid + i) % RingSize];
	}

	// Revolution boundaries: events with index 0 (first tooth after the gap).
	size_t starts[8];
	size_t startCount = 0;
	for (size_t i = 0; i + 1 < valid; i++) {
		if (ordered[i].index == 0) {
			starts[startCount++] = i;
			if (startCount == efi::size(starts)) {
				break;
			}
		}
	}

	if (startCount < 2) {
		efiPrintf("toothdump: not enough captured revolutions (%d boundaries)", (int)startCount);
		return;
	}

	// Print up to 4 complete revolutions, newest last. The last start is a
	// partial revolution in progress - print from the one before it.
	size_t revsPrinted = 0;
	for (size_t b = 1; b + 1 < startCount + 1 && revsPrinted < 4; b++, revsPrinted++) {
		size_t revStart = starts[startCount - b - 1];
		size_t revEnd = starts[startCount - b];

		float periods[ToothCount];
		size_t count = 0;

		// period of tooth N = time from the event at N to the event at N+1;
		// the last tooth (57) carries the missing-teeth gap interval.
		for (size_t cur = revStart; cur < revEnd && count < ToothCount; cur++) {
			periods[count++] = (float)(ordered[cur + 1].timestamp - ordered[cur].timestamp);
		}

		if (count == ToothCount) {
			efiPrintf("toothdump rev:");
			printPeriodChunks(" rev", periods, count);
		}
	}
}

} // namespace

// Called from the trigger decoder for every synchronized primary tooth.
void boardTriggerCallback(efitick_t timestamp, float currentPhase) {
	// Tooth index within one crank revolution; the 4-stroke phase wraps
	// 0..720 and the wheel repeats every 360 degrees.
	int index = (int)(currentPhase * ToothCount / 360.0f + 0.5f) % ToothCount;

	if (lastToothIndex >= 0) {
		efitick_t period = timestamp - lastToothTimestamp;

		if (isPlausibleToothPeriod(period)) {
			float periodUs = period;

			if (profileCount[lastToothIndex] == 0) {
				profileUs[lastToothIndex] = periodUs;
			} else {
				profileUs[lastToothIndex] += EmaAlpha * (periodUs - profileUs[lastToothIndex]);
			}
			profileCount[lastToothIndex]++;
		}
	}

	lastToothTimestamp = timestamp;
	lastToothIndex = index;

	ring[ringHead].timestamp = timestamp;
	ring[ringHead].index = index;
	ringHead = (ringHead + 1) % RingSize;
	ringTotalWritten++;
}

void m74_9ToothDump() {
	efiPrintf("toothdump rpm=%.0f", Sensor::getOrZero(SensorType::Rpm));

	printToothProfile();
	printRawRevolutions();
}

#endif // EFI_PROD_CODE
