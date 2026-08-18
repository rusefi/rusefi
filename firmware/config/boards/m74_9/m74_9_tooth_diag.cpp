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
#include "trigger_decoder.h"
#include "storage.h"
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

// Persistent record in MFS (storage ID EFI_TOOTH_PROFILE_RECORD_ID).
constexpr uint32_t ProfileMagic = 0x544F4F54; // 'TOOT'
constexpr uint32_t ProfileVersion = 1;

struct ToothProfileRecord {
	uint32_t magic;
	uint32_t version;
	uint32_t revolutionsLearned;
	float profileUs[ToothCount];
	uint32_t crc;
};

ToothProfileRecord storedRecord;
bool profileLoaded = false;
bool profileDirtySinceSave = false;
bool seenEngineRunning = false;
bool readRequested = false;

uint32_t minLearnedRevs() {
	uint32_t minRevs = UINT32_MAX;
	for (size_t i = 0; i < ToothCount; i++) {
		if (profileCount[i] < minRevs) {
			minRevs = profileCount[i];
		}
	}
	return minRevs == UINT32_MAX ? 0 : minRevs;
}

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
			profileDirtySinceSave = true;
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
	efiPrintf("toothdump rpm=%.0f stored=%s minRev=%d",
		Sensor::getOrZero(SensorType::Rpm),
		profileLoaded ? "yes" : "no",
		(int)minLearnedRevs());

	printToothProfile();
	printRawRevolutions();
}

// ---- persistent storage: learned profile in MFS, like the stock ECU ----

// Strong override of the weak decoder hook: regularized per-tooth profile
// factors for the sync gap check. The gap tooth (57) keeps factor 1.0 so the
// missing-teeth gap still reads ~3x; regular teeth are normalized to their
// learned share of the revolution, which divides out the systematic
// compression ripple - the digital equivalent of the stock ECU's adaptive
// VR conditioning.
float triggerGetToothProfileFactor(int toothIndex) {
	if (toothIndex < 0 || toothIndex >= (int)ToothCount) {
		return 1.0f;
	}

	// Mean over the regular teeth (excluding the gap tooth 57).
	float sum = 0;
	int count = 0;
	for (size_t i = 0; i < ToothCount - 1; i++) {
		if (profileCount[i] > 0) {
			sum += profileUs[i];
			count++;
		}
	}

	if (count == 0 || toothIndex == (int)ToothCount - 1) {
		return 1.0f;
	}

	float factor = profileUs[toothIndex] / (sum / count);

	// Sanity clamp: no real wheel tooth deviates more than this from the mean.
	if (factor < 0.5f) {
		factor = 0.5f;
	}
	if (factor > 1.5f) {
		factor = 1.5f;
	}

	return factor;
}

// Called from the storage manager thread (serialized with settings writes).
bool toothProfileStorageWrite() {
	storedRecord.magic = ProfileMagic;
	storedRecord.version = ProfileVersion;
	storedRecord.revolutionsLearned = minLearnedRevs();
	for (size_t i = 0; i < ToothCount; i++) {
		storedRecord.profileUs[i] = profileUs[i];
	}
	storedRecord.crc = crc32(&storedRecord, offsetof(ToothProfileRecord, crc));

	StorageStatus status = storageWrite(EFI_TOOTH_PROFILE_RECORD_ID, (uint8_t*)&storedRecord, sizeof(storedRecord));

	if (status == StorageStatus::Ok) {
		profileDirtySinceSave = false;
		efiPrintf("tooth: profile stored (%d revs)", (int)storedRecord.revolutionsLearned);
		return true;
	}

	return false;
}

// Called from the storage manager thread.
bool toothProfileStorageRead() {
	StorageStatus status = storageRead(EFI_TOOTH_PROFILE_RECORD_ID, (uint8_t*)&storedRecord, sizeof(storedRecord));

	if (status != StorageStatus::Ok) {
		efiPrintf("tooth: no stored profile (%d)", (int)status);
		return true; // nothing stored - learning starts from scratch
	}

	uint32_t crc = crc32(&storedRecord, offsetof(ToothProfileRecord, crc));
	if (storedRecord.magic != ProfileMagic || storedRecord.version != ProfileVersion || crc != storedRecord.crc) {
		efiPrintf("tooth: stored profile invalid (magic=%lx crc=%lx)",
			(unsigned long)storedRecord.magic, (unsigned long)crc);
		return true; // corrupted - learning starts from scratch
	}

	// Seed the EMA state so learning continues from the stored profile.
	for (size_t i = 0; i < ToothCount; i++) {
		if (storedRecord.profileUs[i] > 0) {
			profileUs[i] = storedRecord.profileUs[i];
			profileCount[i] = storedRecord.revolutionsLearned;
		}
	}

	profileLoaded = true;
	efiPrintf("tooth: stored profile loaded (%d revs)", (int)storedRecord.revolutionsLearned);
	return true;
}

// Runs on the slow (20 Hz) board callback: lazy load + auto-save on engine stop.
void m74_9ToothPeriodic() {
	if (!readRequested) {
		readRequested = true;
		storageReqestReadID(EFI_TOOTH_PROFILE_RECORD_ID);
	}

	float rpm = Sensor::getOrZero(SensorType::Rpm);
	if (rpm > 0) {
		seenEngineRunning = true;
	}

	if (seenEngineRunning && rpm == 0 && profileDirtySinceSave && minLearnedRevs() >= 5) {
		// Engine just stopped: persist the learned profile (the storage manager
		// serializes this with settings writes). Debounced by the manager's
		// pendingWrites flag, so this fires once per stop.
		seenEngineRunning = false;
		storageRequestWriteID(EFI_TOOTH_PROFILE_RECORD_ID, true);
	}
}

// Manual save from the console.
void m74_9ToothSave() {
	if (minLearnedRevs() < 1) {
		efiPrintf("tooth: nothing learned yet");
		return;
	}
	storageRequestWriteID(EFI_TOOTH_PROFILE_RECORD_ID, true);
	efiPrintf("tooth: save requested (%d revs)", (int)minLearnedRevs());
}

#endif // EFI_PROD_CODE
