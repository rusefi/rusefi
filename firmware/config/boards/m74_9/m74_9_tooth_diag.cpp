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

/**
 * RPM-adaptive trigger input debounce - the software half of the stock ECU's
 * adaptive VR conditioning. The stock ECU moves the conditioner's THRESHOLD
 * with signal amplitude; we cannot move the board's fixed comparator
 * threshold, but we can adapt the debounce to the tooth rate:
 *  - cranking (270 rpm): tooth period ~3.8 ms, debounce = 1.27 ms - dense
 *    noise storms (edges 100-500 us apart) are thinned to ~1 edge per
 *    debounce window instead of passing in full;
 *  - running: the debounce shrinks proportionally, never below the static
 *    100 us floor (noise bursts measure <50 us) and never above 1.5 ms
 *    (a real 60-2 tooth cannot arrive sooner at any cranking speed; the
 *    compression ripple at 270 rpm moves teeth 2.7-5 ms).
 * Below 50 rpm the engine is not rotating and the static floor applies.
 */
float m74_9TriggerDebounceUs() {
	float rpm = Sensor::getOrZero(SensorType::Rpm);
	if (rpm < 50) {
		return 100.0f;
	}

	// mean tooth period at this rpm (58 teeth per crank revolution)
	float toothUs = 60.0f * 1000000.0f / (58.0f * rpm);
	return clampF(100.0f, toothUs / 3.0f, 1500.0f);
}

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
// v2: tooth index now comes from the decoder index (sync tooth = 0); the v1
// records used the engine-phase mapping which was shifted by ~39 teeth.
// v3: records polluted by pre-position-gate false syncs (misattributed tooth
// periods smeared into the EMA) are rejected - learning starts clean.
constexpr uint32_t ProfileVersion = 3;

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
bool readAttempted = false;
efitick_t lastReadRequestNt = 0;
efitick_t lastSaveRequestNt = 0;

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
// 256 characters, 58 values do not fit one line. Buffers are static - the
// console command thread stack is small (a 2.7 KB local array here crashed
// the ECU with a hard fault on the first on-car toothdump).
static char chunkLine[224];

void printPeriodChunks(const char* label, const float* values, size_t count) {
	constexpr size_t Chunk = 12;

	for (size_t start = 0; start < count; start += Chunk) {
		size_t end = start + Chunk < count ? start + Chunk : count;
		size_t off = 0;

		off += chsnprintf(chunkLine + off, sizeof(chunkLine) - off, "%s%02d", label, (int)start);

		for (size_t i = start; i < end; i++) {
			off += chsnprintf(chunkLine + off, sizeof(chunkLine) - off, " %.0f", values[i]);
		}

		efiPrintf("%s", chunkLine);
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
		static float norm[ToothCount];
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
	// revolution boundaries. Static buffer: 348 x 8 = ~2.7 KB does not fit the
	// console command thread stack (the first on-car toothdump hard-faulted
	// because of exactly this).
	size_t valid = ringTotalWritten < RingSize ? ringTotalWritten : RingSize;

	static ToothEvent ordered[RingSize];
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
	static float periods[ToothCount];
	for (size_t b = 1; b + 1 < startCount + 1 && revsPrinted < 4; b++, revsPrinted++) {
		size_t revStart = starts[startCount - b - 1];
		size_t revEnd = starts[startCount - b];

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

// ---- raw trigger edge capture: a digital oscilloscope of the comparator
// output. Every hardware edge on the primary crank input is recorded with its
// NT timestamp BEFORE the debounce and the decoder see it - the true noise
// picture (what the debounce drops included). Dumped with 'rawtrg'.

constexpr size_t RawEdgeRingSize = 2048;

struct RawEdgeRecord {
	efitick_t timestamp;
	bool rising;
};

static RawEdgeRecord rawEdgeRing[RawEdgeRingSize];
static size_t rawEdgeHead = 0;
static size_t rawEdgeCount = 0;

TRIGGER_RAM_CODE void boardRawTriggerEdge(int signalIndex, bool isRising, efitick_t timestamp) {
	if (signalIndex != 0) {
		return; // primary crank input only
	}

	rawEdgeRing[rawEdgeHead] = {timestamp, isRising};
	rawEdgeHead = (rawEdgeHead + 1) % RawEdgeRingSize;
	rawEdgeCount++;
}

void m74_9RawTriggerDump() {
	size_t valid = rawEdgeCount < RawEdgeRingSize ? rawEdgeCount : RawEdgeRingSize;
	if (valid < 2) {
		efiPrintf("rawtrg: no edges captured");
		return;
	}

	// copy the valid part of the ring into time order (oldest -> newest);
	// static buffers - the console command thread stack is small (the first
	// on-car toothdump hard-faulted on a big local array). Snapshot the head
	// once so edges arriving during the dump do not shift the copy base.
	static RawEdgeRecord ordered[RawEdgeRingSize];
	size_t headSnapshot = rawEdgeHead;
	for (size_t i = 0; i < valid; i++) {
		ordered[i] = rawEdgeRing[(headSnapshot + RawEdgeRingSize - valid + i) % RawEdgeRingSize];
	}

	static int32_t deltas[RawEdgeRingSize];
	int32_t minDelta = INT32_MAX;
	int32_t maxDelta = 0;
	for (size_t i = 0; i + 1 < valid; i++) {
		int32_t d = (int32_t)(ordered[i + 1].timestamp - ordered[i].timestamp);
		deltas[i] = d;
		if (d < minDelta) minDelta = d;
		if (d > maxDelta) maxDelta = d;
	}
	size_t deltaCount = valid - 1;

	efitick_t span = ordered[valid - 1].timestamp - ordered[0].timestamp;
	// timestamps are NT ticks at US_TO_NT_MULTIPLIER per us (4 MHz); convert to
	// real us/ms for the dump (the first on-car dumps printed raw ticks labeled
	// as us/ms - everything 4x too large, which looked like a phantom 251 s
	// inter-edge gap and sent the time-jump investigation down the wrong path).
	efiPrintf("rawtrg: %d edges span %.1f ms min %d us max %d us",
		(int)deltaCount, span / (US_TO_NT_MULTIPLIER * 1000.0f),
		(int)(minDelta / US_TO_NT_MULTIPLIER), (int)(maxDelta / US_TO_NT_MULTIPLIER));

	// inter-edge deltas in us, 16 per line, NEWEST FIRST: a truncated console
	// capture (the user closing the console mid-dump) then still contains the
	// most recent edges - the interesting end of the capture. Lines keep their
	// real edge indices; parse and sort by index when reconstructing the
	// stream.
	static char deltaLine[224];
	static char dirLine[224];
	/* Walk the chunks newest first. 'start' is decremented by 16 per chunk;
	 * the LAST chunk is partial (start < 16), so 'start -= 16' on a size_t
	 * would wrap to a huge value and the loop would walk the whole address
	 * space printing garbage until it hits an unmapped page - the on-car
	 * hard fault (PRECISERR at 0x1FFEFF00, 2026-08-20). Terminate explicitly
	 * after the partial chunk instead. */
	for (size_t start = deltaCount; start > 0; ) {
		size_t begin = start >= 16 ? start - 16 : 0;
		size_t off = 0;
		off += chsnprintf(deltaLine + off, sizeof(deltaLine) - off, "raw d%03d", (int)begin);
		for (size_t i = begin; i < start; i++) {
			off += chsnprintf(deltaLine + off, sizeof(deltaLine) - off, " %6d", (int)(deltas[i] / US_TO_NT_MULTIPLIER));
		}
		efiPrintf("%s", deltaLine);

		off = 0;
		off += chsnprintf(dirLine + off, sizeof(dirLine) - off, "raw r%03d", (int)begin);
		for (size_t i = begin; i < start; i++) {
			off += chsnprintf(dirLine + off, sizeof(dirLine) - off, " %6c", ordered[i].rising ? 'R' : 'F');
		}
		efiPrintf("%s", dirLine);

		if (begin == 0) {
			break;
		}
		start = begin;
	}

	// histogram: comparator noise bursts live in the sub-125 us buckets
	// (the earlier "50-500 us" readings were raw 4 MHz ticks, 4x the real
	// us); the debounce drops <100 us. Real teeth at cranking are 2-5 ms.
	// Buckets are real us - the deltas array holds NT ticks, divide by
	// US_TO_NT_MULTIPLIER first.
	size_t h50 = 0, h100 = 0, h200 = 0, h500 = 0, h1m = 0, h2m = 0, h4m = 0, hUp = 0;
	for (size_t i = 0; i < deltaCount; i++) {
		int32_t d = deltas[i] / US_TO_NT_MULTIPLIER;
		if (d < 50) h50++;
		else if (d < 100) h100++;
		else if (d < 200) h200++;
		else if (d < 500) h500++;
		else if (d < 1000) h1m++;
		else if (d < 2000) h2m++;
		else if (d < 4000) h4m++;
		else hUp++;
	}
	efiPrintf("rawtrg hist: <50us=%d 50-100=%d 100-200=%d 200-500=%d 500-1k=%d 1-2k=%d 2-4k=%d >4k=%d",
		(int)h50, (int)h100, (int)h200, (int)h500, (int)h1m, (int)h2m, (int)h4m, (int)hUp);
}

// ---- decoder sync-event trace ----
// Ring of the decoder's last sync/desync events: the decoder-side view that
// correlates with the pin-side 'rawtrg' capture. Filled by
// boardTriggerSyncEvent() (strong override of the weak trigger hook) at every
// sync point; dumped with 'synctrace'.

constexpr size_t SyncTraceRingSize = 32;

struct SyncTraceEvent {
	uint32_t timeMs;      // ChibiOS system tick ms (chVTGetSystemTimeX) at the event - reliable clock, unlike the NT clock
	uint32_t ntMs;        // NT clock ms (getTimeNowMs, TIM5 4 MHz) at the SAME event - printed so the two
	                      // timebases can be compared inside one event (equal deltas = both run at the right rate)
	char kind;            // 'S' validated sync, 'R' first sync/re-sync, 'E' count-error desync, 'A' early-gap acceptance
	                      // (cranking), 'T' tooth-loss acceptance (running band, L9779 eats an edge)
	int8_t countersError; // eventCount - expectedEventCount at the sync (0 for S/R)
	float gap0;
	float gap1;
	uint16_t rpm;
};

static SyncTraceEvent syncTraceRing[SyncTraceRingSize];
static size_t syncTraceHead = 0;
static size_t syncTraceTotal = 0;

TRIGGER_RAM_CODE void boardTriggerSyncEvent(char kind, int countersError, float gap0, float gap1) {
	int8_t clampedError = (int8_t)(countersError < -127 ? -127 : (countersError > 127 ? 127 : countersError));

	/* The ChibiOS system tick (1 kHz) is the reliable clock for diagnostics:
	 * the NT-based getTimeNowMs() can drift between cranks (WrapAround62
	 * sampling gap misclassification) and produced 40-65 s phantom jumps in
	 * the timestamps (m74_9, 2026-08-20). */
	syncTraceRing[syncTraceHead] = {
		(uint32_t)TIME_I2MS(chVTGetSystemTimeX()),
		(uint32_t)getTimeNowMs(),
		kind,
		clampedError,
		gap0,
		gap1,
		(uint16_t)Sensor::getOrZero(SensorType::Rpm)
	};
	syncTraceHead = (syncTraceHead + 1) % SyncTraceRingSize;
	syncTraceTotal++;
}

void m74_9SyncTrace() {
	size_t valid = syncTraceTotal < SyncTraceRingSize ? syncTraceTotal : SyncTraceRingSize;

	efiPrintf("synctrace: %d sync events (newest last), drops: debounce=%u noiseFilter=%u ignoredTooth=%u ordering=%u",
		(int)valid,
		(unsigned)getTriggerCentral()->triggerDebounceDropCount,
		(unsigned)getTriggerCentral()->triggerNoiseFilterDropCount,
		(unsigned)getTriggerCentral()->triggerIgnoredToothCount,
		(unsigned)getTriggerCentral()->triggerState.orderingErrorCounter);

	for (size_t i = 0; i < valid; i++) {
		const SyncTraceEvent& e = syncTraceRing[(syncTraceHead + SyncTraceRingSize - valid + i) % SyncTraceRingSize];
		efiPrintf(" sync %c t=%u ms nt=%u ms countErr=%d gap0=%.3f gap1=%.3f rpm=%u",
			e.kind, (unsigned)e.timeMs, (unsigned)e.ntMs, (int)e.countersError, e.gap0, e.gap1, (unsigned)e.rpm);
	}
}

// 'timecheck' - dump every timebase in one shot. Run it twice with a known
// wall-clock gap in between (e.g. 60 s): systick_ms and nt_ms must each
// advance by the same amount. If one advances and the other does not (or
// jumps), that is the broken clock. nt_lower_ticks is the raw TIM5->CNT
// (4 MHz) and must also advance at ~4e6 per wall second.
void m74_9TimeCheck() {
	uint32_t systickMs = TIME_I2MS(chVTGetSystemTimeX());
	uint32_t ntLower = getTimeNowLowerNt();
	uint32_t ntMs = (uint32_t)(NT2US(getTimeNowNt()) / 1000);
	efiPrintf("timecheck: systick_ms=%u nt_lower_ticks=%u nt_ms=%u",
		(unsigned)systickMs, (unsigned)ntLower, (unsigned)ntMs);
}

// Called from the trigger decoder for every synchronized primary tooth.
TRIGGER_RAM_CODE void boardTriggerCallback(efitick_t timestamp, float) {
	// Authoritative tooth position comes from the decoder index (0 = the sync
	// tooth, 2 index units per tooth for RiseOnly wheels), NOT from the engine
	// phase float: the phase wraps around tdcPosition() and mapped the sync
	// tooth to ~39 instead of 0 on this setup (the first capture showed the
	// gap at profile slot 37 and two teeth never learned).
	int index = (engine->triggerCentral.triggerState.getCurrentIndex() / 2) % ToothCount;

	// A mid-rev false sync resets the decoder's index basis, so the next tooth
	// arrives with a non-sequential index (e.g. 30 -> 0 instead of 30 -> 31).
	// The period across such a jump spans the false sync point and belongs to
	// no tooth - recording it into the profile would smear misattributed
	// periods across the slots (observed on the car: a 3x gap period landed at
	// slot 26 and diluted the real gap slot). The position gate
	// (trigger_decoder.cpp) prevents the false sync itself; this check is the
	// belt-and-braces guard for the learned profile.
	if (lastToothIndex >= 0) {
		int expectedIndex = (lastToothIndex + 1) % ToothCount;

		if (index == expectedIndex) {
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

// Strong override of the weak decoder hook.
//
// DISABLED 2026-08-23: the decoder CONSUMED the RAM-learned EMA profile at
// rpm >= 2 * crankingRpm (useProfile gate in trigger_decoder.cpp) even
// though the board's load/save housekeeping (custom_board_periodicSlowCallback)
// is switched off - disabling the periodic load stops neither the learner
// (trigger hook, boardTriggerCallback) nor the consumer. On the car this
// periodically warped the sync gap ratio at STABLE high rpm: smooth teeth
// (real jitter ~1%) divided by cranking-learned factors (0.79..1.51x)
// produced a fake ripple -> false sync decisions -> C9002 + C9007 + coil
// recharge jerks at constant rpm, recovering on the next revolution.
// The decoder now compares RAW durations on this board; the learner keeps
// running to feed the toothdump/rawtrg diagnostics only.
TRIGGER_RAM_CODE float triggerGetToothProfileFactor(int toothIndex) {
	(void)toothIndex;
	return 1.0f;
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
		// The RAM profile is the authoritative copy after a save: the boot-time
		// read runs once and (by design) does not re-read after a mid-boot
		// save, so mark it loaded here - otherwise 'toothdump' reports
		// stored=no until the next power cycle even though the record is in MFS.
		profileLoaded = true;
		efiPrintf("tooth: profile stored (%d revs)", (int)storedRecord.revolutionsLearned);
		return true;
	}

	return false;
}

// Called from the storage manager thread.
bool toothProfileStorageRead() {
	readAttempted = true;

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
	efitick_t nowNt = getTimeNowNt();

	// Request the stored profile read. The mailbox post can be lost when the
	// storage manager is busy at boot, so re-request every 5 s until the
	// manager actually ran the read (readAttempted).
	if (!readAttempted && (!readRequested || nowNt - lastReadRequestNt > 5 * NT_PER_SECOND)) {
		readRequested = true;
		lastReadRequestNt = nowNt;
		storageReqestReadID(EFI_TOOTH_PROFILE_RECORD_ID);
	}

	float rpm = Sensor::getOrZero(SensorType::Rpm);
	if (rpm > 0) {
		seenEngineRunning = true;
	}

	if (seenEngineRunning && rpm == 0 && profileDirtySinceSave && minLearnedRevs() >= 5) {
		// Engine just stopped: persist the learned profile. The storage manager
		// retries failed writes via its pendingWrites flag; re-post every 15 s
		// in case the mailbox post itself was lost.
		if (nowNt - lastSaveRequestNt > 15 * NT_PER_SECOND) {
			lastSaveRequestNt = nowNt;
			storageRequestWriteID(EFI_TOOTH_PROFILE_RECORD_ID, true);
		}
	} else {
		// engine is running again - arm the save for the next stop
		lastSaveRequestNt = 0;
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
