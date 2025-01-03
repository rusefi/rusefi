#include "pch.h"

#include "mfs_storage.h"

#include "long_term_fuel_trim.h"

#if EFI_BACKUP_SRAM
	// TODO: current trims should be stored in backup ram
	static LtftState ltftState;
#else
	static LtftState ltftState;
#endif

void LtftState::save() {
	ecuRestartCounter++;

	mfsStorageWrite(EFI_LTFT_RECORD_ID, (const uint8_t *)this, sizeof(*this));
}

void LtftState::load() {
	if (mfsStorageRead(EFI_LTFT_RECORD_ID, (uint8_t *)this, sizeof(*this)) != FlashState::Ok) {
		//Reset to some defaules
		memset(trims, 0, sizeof(trims));
	}
}

void LongTermFuelTrim::init(LtftState *state) {
	m_state = state;

	m_state->load();
}

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
