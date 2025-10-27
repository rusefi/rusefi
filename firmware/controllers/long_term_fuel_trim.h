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
	void applyToVe();
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
	void applyTrimsToVe();
	bool isVeUpdated();
	void onLiveDataRead();
	// Development only, to be removed
	void fillRandom();

private:
	LtftState *m_state;
	// TODO: move to livedata and kill isVeUpdated() ?
	bool veNeedRefresh = false;
	bool showUpdateToUser = false;

	float getIntegratorGain(const ltft_s& cfg, ft_region_e region) const;
	float getMaxAdjustment(const ltft_s& cfg) const;
	float getMinAdjustment(const ltft_s& cfg) const;

	Timer pageRefreshTimer{};
};

void initLtft();
void resetLongTermFuelTrim();
void applyLongTermFuelTrimToVe();
bool ltftNeedVeRefresh();
void devPokeLongTermFuelTrim();

void *ltftGetTsPage();
LtftState *ltftGetState();
size_t ltftGetTsPageSize();
