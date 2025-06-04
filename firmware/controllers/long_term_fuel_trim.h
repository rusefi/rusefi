// file long_term_fuel_trim.h

#pragma once

struct LtftState {
	int ecuRestartCounter = 0;
	int8_t trims[LTFT_BANK_CONUT][LTFT_RPM_CELL_COUNT][LTFT_LOAD_CELL_COUNT];

	void save();
	void load();
};

class LongTermFuelTrim : public EngineModule {
public:
	// EngineModule implementation
	void onSlowCallback() override;
	bool needsDelayedShutoff() override;

	void init(LtftState *state);
	void store();

private:
	LtftState *m_state;
};

void initLtft();
void resetLongTermFuelTrim();
void devPokeLongTermFuelTrim();
