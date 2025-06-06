// file long_term_fuel_trim.h

#pragma once

#include "closed_loop_fuel.h"

struct LtftState {
	int ecuRestartCounter = 0;
	float trims[LTFT_BANK_COUNT][VE_LOAD_COUNT][VE_RPM_COUNT];

	void save();
	void load();
};

// TODO: add livedata!
class LongTermFuelTrim : public EngineModule {
public:
	// EngineModule implementation
	void onSlowCallback() override;
	bool needsDelayedShutoff() override;

	void init(LtftState *state);
	void learn(ClosedLoopFuelResult clResult, float rpm, float fuelLoad);
	void store();

private:
	LtftState *m_state;

	float getIntegratorGain();

	// statistic, move to livedata
	uint32_t miss = 0;
	uint32_t hit = 0;
};

void initLtft();
void resetLongTermFuelTrim();
void devPokeLongTermFuelTrim();
