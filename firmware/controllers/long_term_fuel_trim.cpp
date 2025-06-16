#include "pch.h"

#if EFI_LTFT_CONTROL

#include "storage.h"

#include "long_term_fuel_trim.h"

// +/-25% maximum
#define MAX_ADJ (0.25f)

constexpr float integrator_dt = FAST_CALLBACK_PERIOD_MS * 0.001f;

#if EFI_PROD_CODE
// current trims are stored in backup ram (if exists)
static BKUP_RAM_NOINIT LtftState ltftState;
#else
static LtftState ltftState;
#endif

void LtftState::save() {
#if EFI_PROD_CODE
	storageWrite(EFI_LTFT_RECORD_ID, (const uint8_t *)trims, sizeof(trims));
#endif //EFI_PROD_CODE
}

void LtftState::load() {
#if EFI_PROD_CODE
	if (storageRead(EFI_LTFT_RECORD_ID, (uint8_t *)trims, sizeof(trims)) != StorageStatus::Ok) {
#else
	if (1) {
#endif
		//Reset to some defaules
		memset(trims, 0, sizeof(trims));
	}
}

void LtftState::reset() {
	for (size_t bank = 0; bank < FT_BANK_COUNT; bank++) {
		setTable(trims[bank], 0.0f);
	}
}

void LongTermFuelTrim::init(LtftState *state) {
	m_state = state;

	m_state->load();
}

float LongTermFuelTrim::getIntegratorGain() const
{
	const auto& cfg = engineConfiguration->ltft;

	return 1 / clampF(30, cfg.timeConstant, 3000);
}

float LongTermFuelTrim::getMaxAdjustment() const {
	const auto& cfg = engineConfiguration->ltft;

	float raw = 0.01 * cfg.maxAdd;
	// Don't allow maximum less than 0, or more than maximum add adjustment
	return clampF(0, raw, MAX_ADJ);
}

float LongTermFuelTrim::getMinAdjustment() const {
	const auto& cfg = engineConfiguration->ltft;

	float raw = -0.01f * cfg.maxRemove;
	// Don't allow minimum more than 0, or less than maximum remove adjustment
	return clampF(-MAX_ADJ, raw, 0);
}

void LongTermFuelTrim::learn(ClosedLoopFuelResult clResult, float rpm, float fuelLoad) {
	const auto& cfg = engineConfiguration->ltft;

	if (!cfg.enabled) {
		return;
	}

	// TODO: should we swap x and y here to keep aligned to wierd TS table definition?
	// x - load, y - rpm
	auto x = priv::getClosestBin(fuelLoad, config->veLoadBins);
	auto y = priv::getClosestBin(rpm, config->veRpmBins);

	// Skip learning if current load point falls far outside the table
	if ((abs(x.Frac) > 0.5) ||
		(abs(y.Frac) > 0.5)) {
		// we are outside table
		cntMiss++;
		return;
	}

	bool adjusted = false;

	// calculate weight depenting on distance from cell center
	// Is this too heavy?
	float weight = 1.0 - hypotf(x.Frac, y.Frac) / hypotf(0.5, 0.5);
	float k = getIntegratorGain() * integrator_dt * weight;

	for (size_t bank = 0; bank < FT_BANK_COUNT; bank++) {
		float lambdaCorrection = clResult.banks[bank] - 1.0;

		// If we're within the deadband, make no adjustment.
		if (std::abs(lambdaCorrection) < 0.01f * cfg.deadband) {
			continue;
		}

		// get current trim
		float trim = m_state->trims[bank][x.Idx][y.Idx];

		// Integrate
		float newTrim = trim + k * (lambdaCorrection - trim);

		// TODO:
		// rise OBD code if we hit trim limit

		// Clamp to bounds and save
		newTrim = clampF(getMinAdjustment(), newTrim, getMaxAdjustment());

		// store
		m_state->trims[bank][x.Idx][y.Idx] = newTrim;

		adjusted = true;
	}

	if (adjusted) {
		cntHit++;
	} else {
		cntDead++;
	}
}

ClosedLoopFuelResult LongTermFuelTrim::getTrims(float rpm, float fuelLoad) {
	const auto& cfg = engineConfiguration->ltft;

	if (!cfg.correctionEnabled) {
		return { };
	}

	// Keep calculating/applying correction even load point is far outside table
#if 0
	// x - load, y - rpm
	auto x = priv::getClosestBin(fuelLoad, config->veLoadBins);
	auto y = priv::getClosestBin(rpm, config->veRpmBins);

	// do not interpolate outside table...
	if ((abs(x.Frac) > 0.5) ||
		(abs(y.Frac) > 0.5)) {
		// we are outside table
		miss++;
		return { };
	}
#endif

	ClosedLoopFuelResult result;

	for (size_t bank = 0; bank < FT_BANK_COUNT; bank++) {
		result.banks[bank] = 1.0f + interpolate3d(
			m_state->trims[bank],
			config->veLoadBins, fuelLoad,
			config->veRpmBins, rpm
		);
	}

	return result;
}

void LongTermFuelTrim::store() {
	// TODO: lock to avoid modification while writing

	if (m_state) {
		m_state->save();
	}

	// TODO: unlock
}

void LongTermFuelTrim::reset() {
	m_state->reset();

	cntHit = 0;
	cntMiss = 0;
	cntDead = 0;
}

void LongTermFuelTrim::onSlowCallback() {
	// Do some magic math here?

	/* ... */
}

bool LongTermFuelTrim::needsDelayedShutoff() {
	// TODO: We should delay power off until we store LTFT
	return false;
}

void initLtft(void)
{
	engine->module<LongTermFuelTrim>()->init(&ltftState);
}

void resetLongTermFuelTrim() {
	engine->module<LongTermFuelTrim>()->reset();
}

void devPokeLongTermFuelTrim() {
}

void *ltftGetTsPage() {
	return (void *)ltftState.trims;
}

size_t ltftGetTsPageSize() {
	return sizeof(ltftState.trims);
}

#endif // EFI_LTFT_CONTROL
