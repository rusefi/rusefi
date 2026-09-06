// file long_term_fuel_trim.h
//
// [tag:disable_engine_module] Optional engine module gated by EFI_LTFT_CONTROL; it owns
// TS page 0x0200 (TS_PAGE_LTFT_TRIMS), so the flag is a TS-page guard flag declared in the
// board prepend.txt only.
// See engine_module.h for the full how-to before copying this pattern.

#pragma once

#include "closed_loop_fuel.h"
#include "long_term_fuel_trim_state_generated.h"

struct LtftState {
  // todo: probably reuse page_2_generated.h?
	float trims[FT_BANK_COUNT][VE_LOAD_COUNT][VE_RPM_COUNT];

	// returns true if the trims were persisted, false on storage failure
	bool save();
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
	// returns true if the trims were persisted; false lets the storage manager keep the request pending and retry
	bool store();
	void reset();
	void applyTrimsToVe();
	bool isVeUpdated();
	void onLiveDataRead();
	// Development only, to be removed
	void fillRandom();

private:
	// only init() ever writes this, and store() null-checks it - that guard is only meaningful
	// if the pointer starts null rather than relying on the instance living at file scope
	LtftState *m_state = nullptr;
	// TODO: move to livedata and kill isVeUpdated() ?
	bool veNeedRefresh = false;
	bool showUpdateToUser = false;
	// a save request that could not be queued yet (storage manager mailbox full); retried on every learn() call
	bool saveRequestNeeded = false;

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
