#pragma once

struct stft_s;

struct ClosedLoopFuelResult {
	ClosedLoopFuelResult() {
		// Default is no correction, aka 1.0 multiplier
		for (size_t i = 0; i < STFT_BANK_COUNT; i++) {
			banks[i] = 1.0f;
		}
	}

	float banks[STFT_BANK_COUNT];
};

ClosedLoopFuelResult fuelClosedLoopCorrection();
size_t computeStftBin(int rpm, float load, stft_s& cfg);
bool shouldUpdateCorrection(SensorType sensor);
