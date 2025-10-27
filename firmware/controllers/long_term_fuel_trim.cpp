#include "pch.h"

#if EFI_LTFT_CONTROL

#include "storage.h"

#include "long_term_fuel_trim.h"

#include "board_overrides.h"

// +/-25% maximum
#define MAX_ADJ (0.25f)

#define SAVE_AFTER_HITS	1000

constexpr float integrator_dt = FAST_CALLBACK_PERIOD_MS * 0.001f;

// TODO: store in backup ram and validate on start
static LtftState ltftState;

// LTFT to VE table custom apply algo
std::optional<setup_custom_board_overrides_type> custom_board_LtftTrimToVeApply;

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
		reset();
	}
}

void LtftState::reset() {
	for (size_t bank = 0; bank < FT_BANK_COUNT; bank++) {
		setTable(trims[bank], 0.0f);
	}
}

void LtftState::fillRandom() {
	for (size_t bank = 0; bank < FT_BANK_COUNT; bank++) {
		for (size_t loadIndex = 0; loadIndex < VE_LOAD_COUNT; loadIndex++) {
			for (size_t rpmIndex = 0; rpmIndex < VE_RPM_COUNT; rpmIndex++) {
				trims[bank][loadIndex][rpmIndex] = PERCENT_DIV * (loadIndex + rpmIndex * 0.1);
			}
		}
	}
}

void LtftState::applyToVe() {
	// if we have custom implementation
	if (call_board_override(custom_board_LtftTrimToVeApply)) {
		return;
	}

	for (size_t loadIndex = 0; loadIndex < VE_LOAD_COUNT; loadIndex++) {
		for (size_t rpmIndex = 0; rpmIndex < VE_RPM_COUNT; rpmIndex++) {
			float k = 0;

			/* We have single VE table, but FT_BANK_COUNT banks of trims */
			for (size_t bank = 0; bank < FT_BANK_COUNT; bank++) {
				k += 1.0f + trims[bank][loadIndex][rpmIndex];
			}
			k = k / FT_BANK_COUNT;

			config->veTable[loadIndex][rpmIndex] = config->veTable[loadIndex][rpmIndex] * k;
		}
	}
}

void LongTermFuelTrim::init(LtftState *state) {
	m_state = state;

#if EFI_PROD_CODE
	ltftLoadPending = storageReqestReadID(EFI_LTFT_RECORD_ID);
#else
	ltftLoadPending = false;
	reset();
#endif
}

float LongTermFuelTrim::getIntegratorGain(const ltft_s& cfg, ft_region_e region) const
{
	return 1 / clampF(1, cfg.timeConstant[region], 3000);
}

float LongTermFuelTrim::getMaxAdjustment(const ltft_s& cfg) const {
	// Don't allow maximum less than 0, or more than maximum add adjustment
	return clampF(0, PERCENT_DIV * cfg.maxAdd, MAX_ADJ);
}

float LongTermFuelTrim::getMinAdjustment(const ltft_s& cfg) const {
	// Don't allow minimum more than 0, or less than maximum remove adjustment
	return clampF(-MAX_ADJ, -PERCENT_DIV * cfg.maxRemove, 0);
}

void LongTermFuelTrim::learn(ClosedLoopFuelResult clResult, float rpm, float fuelLoad) {
	const auto& cfg = engineConfiguration->ltft;

	// LTFT uses STFT output, so if STFT is not correcting for some reason - LTFT also should not learn
	if ((!cfg.enabled) || (ltftSavePending) || (ltftLoadPending) ||
		(engine->module<ShortTermFuelTrim>()->stftCorrectionState != stftEnabled)) {
		ltftLearning = false;
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
		ltftCntMiss++;
		ltftLearning = false;
		return;
	}

	bool adjusted = false;

	// calculate weight depenting on distance from cell center
	// Is this too heavy?
	float weight = 1.0 - hypotf(x.Frac, y.Frac) / hypotf(0.5, 0.5);
	float k = getIntegratorGain(cfg, clResult.region) * integrator_dt * weight;

	for (size_t bank = 0; bank < FT_BANK_COUNT; bank++) {
		float lambdaCorrection = clResult.banks[bank] - 1.0;

		// If we're within the deadband, make no adjustment.
		if (std::abs(lambdaCorrection) < PERCENT_DIV * cfg.deadband) {
			continue;
		}

		// get current trim
		float trim = m_state->trims[bank][x.Idx][y.Idx];

		// Integrate
		float newTrim = trim + k * (lambdaCorrection - trim);

		// TODO:
		// rise OBD code if we hit trim limit

		// Clamp to bounds and save
		newTrim = clampF(getMinAdjustment(cfg), newTrim, getMaxAdjustment(cfg));

		// accumulate
		ltftAccummulatedCorrection[bank] += newTrim - trim;

		// store
		m_state->trims[bank][x.Idx][y.Idx] = newTrim;

		adjusted = true;
	}

	ltftLearning = adjusted;
	if (adjusted) {
		ltftCntHit++;
		showUpdateToUser = true;
		if ((ltftCntHit % SAVE_AFTER_HITS) == 0) {
			// request save
#if EFI_PROD_CODE
			settingsLtftRequestWriteToFlash();
#endif
		}
	} else {
		ltftCntDeadband++;
	}
}

ClosedLoopFuelResult LongTermFuelTrim::getTrims(float rpm, float fuelLoad) {
	const auto& cfg = engineConfiguration->ltft;

	if ((!cfg.correctionEnabled) || (ltftLoadPending)) {
		for (size_t bank = 0; bank < FT_BANK_COUNT; bank++) {
			ltftCorrection[bank] = 1.0f;
		}
		ltftCorrecting = false;
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

	// Is there any reason we should not apply LTFT?

	for (size_t bank = 0; bank < FT_BANK_COUNT; bank++) {
		ltftCorrection[bank] = 1.0f + interpolate3d(
			m_state->trims[bank],
			config->veLoadBins, fuelLoad,
			config->veRpmBins, rpm
		);
	}

	ClosedLoopFuelResult result;
	for (size_t bank = 0; bank < FT_BANK_COUNT; bank++) {
		result.banks[bank] = ltftCorrection[bank];
	}

	ltftCorrecting = true;
	return result;
}

// Called from storage manager thread when requested ID is ready
void LongTermFuelTrim::load() {
	m_state->load();

	ltftLoadPending = false;
}

void LongTermFuelTrim::store() {
	// TODO: lock to avoid modification while writing
	ltftSavePending = true;

	if (m_state) {
		m_state->save();
	}

	// TODO: unlock
	ltftSavePending = false;
}

void LongTermFuelTrim::reset() {
	m_state->reset();

	ltftCntHit = 0;
	ltftCntMiss = 0;
	ltftCntDeadband = 0;

	for (size_t bank = 0; bank < FT_BANK_COUNT; bank++) {
		ltftAccummulatedCorrection[bank] = 0.0;
	}
}

void LongTermFuelTrim::applyTrimsToVe() {
	m_state->applyToVe();
	m_state->reset();

	veNeedRefresh = true;
}

bool LongTermFuelTrim::isVeUpdated() {
	if (veNeedRefresh) {
		veNeedRefresh = false;
		return true;
	}
	return false;
}

void LongTermFuelTrim::onLiveDataRead() {
	// rise refresh flag every second for one TS reading of livedata if we have something new...
	if (ltftPageRefreshFlag) {
		ltftPageRefreshFlag = false;
		showUpdateToUser = false;
		pageRefreshTimer.reset();
	} else {
		// was update to table and timeout
		ltftPageRefreshFlag = showUpdateToUser && pageRefreshTimer.hasElapsedSec(1);
	}
}

void LongTermFuelTrim::fillRandom() {
	m_state->fillRandom();
}

void LongTermFuelTrim::onSlowCallback() {
	// we can wait some time for LTFT to be loaded from storage...
	if ((ltftLoadPending) &&
#if EFI_SHAFT_POSITION_INPUT
		(engine->rpmCalculator.getSecondsSinceEngineStart(getTimeNowNt()) > 5.0) &&
#endif
		(1)) {
		efiPrintf("LTFT: failed to load calibrations");
		m_state->reset();
		ltftLoadPending = false;
		ltftLoadError = true;
	}
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

void applyLongTermFuelTrimToVe() {
	engine->module<LongTermFuelTrim>()->applyTrimsToVe();
}

bool ltftNeedVeRefresh() {
	return engine->module<LongTermFuelTrim>()->isVeUpdated();
}

void devPokeLongTermFuelTrim() {
	engine->module<LongTermFuelTrim>()->fillRandom();
}

void *ltftGetTsPage() {
	return (void *)ltftState.trims;
}

LtftState *ltftGetState() {
	return &ltftState;
}

size_t ltftGetTsPageSize() {
	return sizeof(ltftState.trims);
}

#endif // EFI_LTFT_CONTROL
