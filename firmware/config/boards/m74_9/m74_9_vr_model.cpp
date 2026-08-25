/**
 * @file m74_9_vr_model.cpp
 *
 * VR amplitude model for the L9779 auto-adaptive hysteresis. See the header
 * comment in m74_9_vr_model.h for the model and the calibration contract.
 *
 * This file also carries the strong overrides of the two board seams:
 *  - triggerObserveGapShift(float) - trains the per-level table from the
 *    decoder's per-sync gap measurement (weak default in
 *    trigger_board_hooks.cpp);
 *  - vrModelStorageWrite()/vrModelStorageRead() - the MFS record handlers
 *    (weak defaults in storage_weaks.cpp).
 */

#include "pch.h"
#include "m74_9_vr_model.h"
#include "storage.h"
#include "board_overrides.h"

// PV quantization thresholds (datasheet Table 42, typical values, mV):
// the peak detector's 5 levels are bounded by PV1..PV4.
static constexpr float vrPvThresholdsMv[4] = { 930.0f, 1600.0f, 2300.0f, 3000.0f };

// Hysteresis per level (datasheet Table 43 typical currents x this board's
// 10k series resistor: 5/10/17/32/51 uA -> 50/100/173/322/510 mV).
static constexpr float vrHysteresisMv[5] = { 50.0f, 100.0f, 173.0f, 322.0f, 510.0f };

// Learned per-level missing-teeth gap shift (pitch units). SEEDED with the
// coast rawtrg measurement (0.63 pitch, constant in angle across 373-1129
// rpm). The decoder trains the entry of the CURRENT hysteresis level from
// each in-band per-sync measurement (alpha 0.1) - the model is the correction
// state, the measurement is the teacher. The per-level split refines itself
// once the car runs through the level transitions.
static float vrShiftTablePitch[5] = { 0.63f, 0.63f, 0.63f, 0.63f, 0.63f };
static uint32_t vrTableRevolutions[5] = {};

// Learning rate: ~30 revs to converge (0.6 s at 3000 rpm), slow enough that
// a single corrupted in-band measurement moves the table by a tenth only.
static constexpr float vrLearnAlpha = 0.1f;

// Calibration scalar: Vp = k * rpm (mV). Persisted with the table.
static float vrAmplitudePerRpm = 0.0f;

// eat-risk band: |Vp - PV boundary| / boundary below this fraction = risky.
static constexpr float vrBoundaryRiskFraction = 0.15f;

// ---- persistence (MFS record EFI_VR_MODEL_RECORD_ID) ----

static constexpr uint32_t VrModelMagic = 0x56524D4C;	// 'VRML'
static constexpr uint32_t VrModelVersion = 1;

struct VrModelRecord {
	uint32_t magic;
	uint32_t version;
	uint32_t revolutionsLearned;
	float amplitudePerRpm;
	float shiftTablePitch[5];
	uint32_t crc;
};

static VrModelRecord vrStoredRecord;
static bool vrLoaded = false;
static bool vrDirty = false;
static bool vrReadAttempted = false;
static efitick_t vrLastSaveRequestNt = 0;

float vrAmplitudePerRpmGet() {
	return vrAmplitudePerRpm;
}

int vrLevelForRpm(float rpm) {
	if (vrAmplitudePerRpm <= 0) {
		return 0;
	}

	float vp = vrAmplitudePerRpm * rpm;
	int level = 0;
	while (level < 4 && vp >= vrPvThresholdsMv[level]) {
		level++;
	}
	return level;
}

float vrHysteresisMvForLevel(int level) {
	if (level < 0 || level > 4) {
		return 0;
	}
	return vrHysteresisMv[level];
}

float vrExpectedShiftPitchForRpm(float rpm) {
	return vrShiftTablePitch[vrLevelForRpm(rpm)];
}

float vrBoundaryProximityForRpm(float rpm) {
	if (vrAmplitudePerRpm <= 0) {
		return 0.0f;
	}

	float vp = vrAmplitudePerRpm * rpm;
	float closest = 1.0f;	// 0..1, smaller = closer to a boundary

	for (float threshold : vrPvThresholdsMv) {
		float rel = absF(vp - threshold) / threshold;
		if (rel < closest) {
			closest = rel;
		}
	}

	// 1 = exactly on a boundary, 0 = at/outside the risk band edge
	return closest < vrBoundaryRiskFraction ? (1.0f - closest / vrBoundaryRiskFraction) : 0.0f;
}

// ---- decoder training seam (strong override of the weak trigger hook) ----

void triggerObserveGapShift(float measuredPitch) {
	int level = vrLevelForRpm(Sensor::getOrZero(SensorType::Rpm));
	vrShiftTablePitch[level] += vrLearnAlpha * (measuredPitch - vrShiftTablePitch[level]);
	vrTableRevolutions[level]++;
	vrDirty = true;
}

// ---- diagnostics ----

void m74_9VrModel() {
	float rpm = Sensor::getOrZero(SensorType::Rpm);
	float k = vrAmplitudePerRpm;
	int level = vrLevelForRpm(rpm);

	efiPrintf("vrmodel: k=%.3f mV/rpm rpm=%.0f Vp=%.0f mV level=%d HI=%.0f mV stored=%s revs=%u/%u/%u/%u/%u",
		k, rpm, k * rpm, level + 1, vrHysteresisMv[level],
		vrLoaded ? "yes" : "no",
		(unsigned)vrTableRevolutions[0], (unsigned)vrTableRevolutions[1],
		(unsigned)vrTableRevolutions[2], (unsigned)vrTableRevolutions[3],
		(unsigned)vrTableRevolutions[4]);

	for (int l = 0; l < 5; l++) {
		efiPrintf("vrmodel L%d: shift=%.3f pitch (%.2f deg)%s",
			l + 1, vrShiftTablePitch[l], vrShiftTablePitch[l] * 6.0f,
			l == level ? "  <-- active" : "");
	}

	float proximity = vrBoundaryProximityForRpm(rpm);
	efiPrintf("vrmodel: boundary=%.0f%% risk=%s measured=%.2f deg",
		proximity * 100.0f,
		proximity > 0.5f ? "YES" : "no",
		(double)getTriggerCentral()->gapAnchorCorrectionDeg);

	// k self-calibration helper: the rpm where each PV boundary is crossed for
	// the current k. Compare against the rpm where the measured gap shift
	// STEPS (synctrace gap0 per sync) - each step is a level transition, and
	// k = PV threshold / rpm at the step. No oscilloscope needed.
	if (vrAmplitudePerRpm > 0) {
		efiPrintf("vrmodel level boundaries at rpm: L2>=%.0f L3>=%.0f L4>=%.0f L5>=%.0f",
			vrPvThresholdsMv[0] / vrAmplitudePerRpm,
			vrPvThresholdsMv[1] / vrAmplitudePerRpm,
			vrPvThresholdsMv[2] / vrAmplitudePerRpm,
			vrPvThresholdsMv[3] / vrAmplitudePerRpm);
	}
}

void m74_9VrModelSetK(const char* arg) {
	if (arg == nullptr || arg[0] == 0) {
		efiPrintf("vrk: usage 'vrk <mV per rpm>' (0 = all learning into level 1), current k=%.3f", vrAmplitudePerRpm);
		return;
	}

	float k = atof(arg);
	if (k < 0) {
		efiPrintf("vrk: negative k rejected");
		return;
	}

	vrAmplitudePerRpm = k;
	vrDirty = true;
	m74_9VrModel();

	// Persistence contract: the save is gated on the engine being stopped
	// (a write while running is a flash stall). On the bench the debug ECU
	// resets every ~5 s and reloads the STORED record, so a 'vrk' sent while
	// the engine runs bounces back to the stored value after each reset -
	// set k with the engine stopped, or expect it to persist only on stop.
	if (engine->rpmCalculator.isStopped()) {
		efiPrintf("vrk: engine stopped - model will save on the next slow callback (~2 s)");
	} else {
		efiPrintf("vrk: engine running - k will persist when the engine stops");
	}
}

// ---- persistence handlers ----

bool vrModelStorageWrite() {
	vrStoredRecord.magic = VrModelMagic;
	vrStoredRecord.version = VrModelVersion;
	vrStoredRecord.revolutionsLearned = 0;
	for (int l = 0; l < 5; l++) {
		vrStoredRecord.shiftTablePitch[l] = vrShiftTablePitch[l];
		if (vrTableRevolutions[l] > vrStoredRecord.revolutionsLearned) {
			vrStoredRecord.revolutionsLearned = vrTableRevolutions[l];
		}
	}
	vrStoredRecord.amplitudePerRpm = vrAmplitudePerRpm;
	vrStoredRecord.crc = crc32(&vrStoredRecord, offsetof(VrModelRecord, crc));

	StorageStatus status = storageWrite(EFI_VR_MODEL_RECORD_ID, (uint8_t*)&vrStoredRecord, sizeof(vrStoredRecord));
	if (status == StorageStatus::Ok) {
		vrDirty = false;
		vrLoaded = true;
		efiPrintf("vrmodel: stored (%u revs)", (unsigned)vrStoredRecord.revolutionsLearned);
		return true;
	}

	efiPrintf("vrmodel: store failed (%d)", (int)status);
	return false;
}

bool vrModelStorageRead() {
	vrReadAttempted = true;

	StorageStatus status = storageRead(EFI_VR_MODEL_RECORD_ID, (uint8_t*)&vrStoredRecord, sizeof(vrStoredRecord));
	if (status != StorageStatus::Ok) {
		efiPrintf("vrmodel: no stored record (%d) - learning from seed", (int)status);
		// The load attempt is COMPLETE even though nothing was stored: the RAM
		// table (seed) is now authoritative. vrLoaded gates the periodic saver
		// below - if it stays false on a first boot, the model is NEVER persisted
		// and every power cycle starts from the seed again.
		vrLoaded = true;
		return true;
	}

	uint32_t crc = crc32(&vrStoredRecord, offsetof(VrModelRecord, crc));
	if (vrStoredRecord.magic != VrModelMagic || vrStoredRecord.version != VrModelVersion || crc != vrStoredRecord.crc) {
		efiPrintf("vrmodel: stored record invalid (magic=%lx crc=%lx) - learning from seed",
			(unsigned long)vrStoredRecord.magic, (unsigned long)crc);
		// Same contract as the no-record path above: load done, RAM is the truth.
		vrLoaded = true;
		return true;
	}

	for (int l = 0; l < 5; l++) {
		vrShiftTablePitch[l] = vrStoredRecord.shiftTablePitch[l];
	}
	vrAmplitudePerRpm = vrStoredRecord.amplitudePerRpm;
	vrLoaded = true;
	vrDirty = false;
	efiPrintf("vrmodel: stored record loaded (k=%.3f, %u revs)",
		vrAmplitudePerRpm, (unsigned)vrStoredRecord.revolutionsLearned);
	return true;
}

void m74_9VrModelPeriodic() {
	if (!vrReadAttempted) {
		vrReadAttempted = true;
		storageReqestReadID(EFI_VR_MODEL_RECORD_ID);
		return;
	}

	if (!vrDirty || !vrLoaded) {
		return;
	}

	// save only when stopped, and only through the debounced flash gate
	// (a save while running is a 17-38 ms CPU stall - the exact class the
	// gate exists to prevent)
	if (!engine->rpmCalculator.isStopped()) {
		return;
	}

	if (!get_board_override_result(custom_board_allowFlashNow, true)) {
		return;
	}

	efitick_t nowNt = getTimeNowNt();
	if (nowNt - vrLastSaveRequestNt < 15 * NT_PER_SECOND) {
		return;
	}
	vrLastSaveRequestNt = nowNt;
	storageRequestWriteID(EFI_VR_MODEL_RECORD_ID, true);
}
