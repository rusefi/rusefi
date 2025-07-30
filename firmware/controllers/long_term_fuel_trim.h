// file long_term_fuel_trim.h

#pragma once

#include "closed_loop_fuel.h"
#include "long_term_fuel_trim_state_generated.h"

struct LtftState {
  // todo: probably reuse page_2_generated.h?
	float trims[FT_BANK_COUNT][VE_LOAD_COUNT][VE_RPM_COUNT];

	void save();
	void load();
	void reset();
	// Development only, to be removed
	void fillRandom();
};

class LongTermFuelTrim : public EngineModule, public long_term_fuel_trim_state_s {
public:
	// EngineModule implementation
	void onSlowCallback() override;
	bool needsDelayedShutoff() override;

	void init(LtftState *state);
	void learn(ClosedLoopFuelResult clResult, float rpm, float fuelLoad);
	ClosedLoopFuelResult getTrims(float rpm, float fuelLoad);
	void load();
	void store();
	void reset();
	void onLiveDataRead();
	// Development only, to be removed
	void fillRandom();

private:
	LtftState *m_state;

	float getIntegratorGain() const;
	float getMaxAdjustment() const;
	float getMinAdjustment() const;

	Timer pageRefreshTimer{};
};

void initLtft();
void resetLongTermFuelTrim();
void devPokeLongTermFuelTrim();

void *ltftGetTsPage();
size_t ltftGetTsPageSize();
