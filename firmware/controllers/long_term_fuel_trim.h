// file long_term_fuel_trim.h

#pragma once

#include "closed_loop_fuel.h"

struct LtftState {
  // todo: probably reuse page_2_generated.h?
	float trims[FT_BANK_COUNT][VE_LOAD_COUNT][VE_RPM_COUNT];

	void save();
	void load();
	void reset();
};

// TODO: add livedata!
class LongTermFuelTrim : public EngineModule {
public:
	// EngineModule implementation
	void onSlowCallback() override;
	bool needsDelayedShutoff() override;

	void init(LtftState *state);
	void learn(ClosedLoopFuelResult clResult, float rpm, float fuelLoad);
	ClosedLoopFuelResult getTrims(float rpm, float fuelLoad);
	void store();
	void reset();

private:
	LtftState *m_state;

	float getIntegratorGain() const;
	float getMaxAdjustment() const;
	float getMinAdjustment() const;

	// statistic, move to livedata
	uint32_t cntHit = 0;
	uint32_t cntMiss = 0;
	uint32_t cntDead = 0;
};

void initLtft();
void resetLongTermFuelTrim();
void devPokeLongTermFuelTrim();

void *ltftGetTsPage();
size_t ltftGetTsPageSize();
