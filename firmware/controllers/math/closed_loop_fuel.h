// closed_loop_fuel.h

#pragma once

#include "closed_loop_fuel_cell.h"
#include "deadband.h"
#include "short_term_fuel_trim_state_generated.h"

struct stft_s;

struct ClosedLoopFuelResult {
	ClosedLoopFuelResult() {
		// Default is no correction, aka 1.0 multiplier
		for (size_t i = 0; i < FT_BANK_COUNT; i++) {
			banks[i] = 1.0f;
		}
	}

	float banks[FT_BANK_COUNT];
};

struct FuelingBank {
	ClosedLoopFuelCellImpl cells[STFT_CELL_COUNT];
};

class ShortTermFuelTrim : public EngineModule, public short_term_fuel_trim_state_s {
public:
	void init(stft_s *stftCfg);
	// EngineModule implementation
	void onSlowCallback() override;
	bool needsDelayedShutoff() override;

	ClosedLoopFuelResult getCorrection();

private:
	FuelingBank banks[FT_BANK_COUNT];

	Deadband<25> idleDeadband;
	Deadband<2> overrunDeadband;
	Deadband<2> loadDeadband;

	SensorType getSensorForBankIndex(size_t index);
	size_t computeStftBin(float rpm, float load, stft_s& cfg);
	bool shouldCorrect();
	bool shouldUpdateCorrection(SensorType sensor);
};

void initStft(void);

/* TODO: move out of here */
bool checkIfTuningVeNow();
