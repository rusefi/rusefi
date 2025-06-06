#include "pch.h"

#if EFI_LTFT_CONTROL

#include "storage.h"

#include "long_term_fuel_trim.h"

constexpr float integrator_dt = FAST_CALLBACK_PERIOD_MS * 0.001f;

#if EFI_BACKUP_SRAM
	// TODO: current trims should be stored in backup ram
	static LtftState ltftState;
#else
	static LtftState ltftState;
#endif

void LtftState::save() {
	ecuRestartCounter++;

#if EFI_PROD_CODE
	storageWrite(EFI_LTFT_RECORD_ID, (const uint8_t *)this, sizeof(*this));
#endif //EFI_PROD_CODE
}

void LtftState::load() {
#if EFI_PROD_CODE
	if (storageRead(EFI_LTFT_RECORD_ID, (uint8_t *)this, sizeof(*this)) != StorageStatus::Ok) {
#else
	if (1) {
#endif
		//Reset to some defaules
		memset(trims, 0, sizeof(trims));
	}
}

void LongTermFuelTrim::init(LtftState *state) {
	m_state = state;

	m_state->load();
}

float LongTermFuelTrim::getIntegratorGain()
{
	const auto& cfg = engineConfiguration->ltft;
	//
	return 1 / clampF(30, cfg.timeConstant, 3000);
}

#if (LTFT_BANK_COUNT != STFT_BANK_COUNT)
	#error Expected LTFT_BANK_COUNT to be equal to STFT_BANK_COUNT
#endif

void LongTermFuelTrim::learn(ClosedLoopFuelResult clResult, float rpm, float fuelLoad) {
	// x - load, y - rpm
	auto x = priv::getClosestBin(fuelLoad, config->veLoadBins);
	auto y = priv::getClosestBin(rpm, config->veRpmBins);

	if ((abs(x.Frac) > 0.5) ||
		(abs(y.Frac) > 0.5)) {
		// we are outside table
		miss++;
		return;
	}

	bool adjusted = false;
	const auto& cfg = engineConfiguration->ltft;

	// calculate weight depenting on distance from cell center
	// Is this too heavy?
	float weight = 1.0 - hypotf(x.Frac, y.Frac) / hypotf(0.5, 0.5);
	float k = getIntegratorGain() * integrator_dt * weight;

	for (size_t bank = 0; bank < LTFT_BANK_COUNT; bank++) {
		float lambdaCorrection = clResult.banks[bank] - 1.0;

		// If we're within the deadband, make no adjustment.
		if (std::abs(lambdaCorrection) < 0.01f * cfg.deadband) {
			continue;
		}

		// get current trim
		float trim = m_state->trims[bank][x.Idx][y.Idx];

		// Integrate
		float newTrim = k * lambdaCorrection + trim;

		(void)newTrim;

		adjusted = true;
	}

	if (adjusted) {
		hit++;
	}
}

#if 0
		float trim = interpolate3d(
			m_state->trims[bank],
			config->veLoadBins, fuelLoad,
			config->veRpmBins, rpm
		);
#endif

void LongTermFuelTrim::store() {
	// TODO: lock to avoid modification while writing

	if (m_state) {
		m_state->save();
	}

	// TODO: unlock
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
}

void devPokeLongTermFuelTrim() {
}

#endif // EFI_LTFT_CONTROL
