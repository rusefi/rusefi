// file long_term_fuel_trim.h

#pragma once

struct LtftState {
	int ecuRestartCounter = 0;
	int8_t trims[FT_BANK_COUNT][VE_LOAD_COUNT][VE_RPM_COUNT];

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
