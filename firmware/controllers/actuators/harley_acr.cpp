// harley_acr.cpp
//
// Controller for Harley-Davidson Automatic Compression Release (ACR).
//
// Big V-twin engines have very high cylinder compression which makes them hard
// to crank with the starter motor. To ease the starter's job, these engines are
// fitted with an ACR solenoid (one per cylinder on twin-ACR setups) that bleeds
// cylinder pressure to atmosphere while the engine is being cranked.
//
// Control strategy:
//  - As soon as the crank starts moving, energize the ACR solenoid(s) so the
//    very first compression stroke is relieved.
//  - Keep the valve open for `acrRevolutions` crank revolutions (counted via
//    the trigger decoder's synchronization counter).
//  - On the final revolution, close the valve at a specific crank phase
//    (`acrDisablePhase`) so it shuts mid-expansion rather than mid-compression.
//    This avoids leaving a partial charge trapped in the cylinder, which would
//    cause an awkward "half-charge" combustion event on the next stroke.
//  - If the engine is stopped, the valve is de-energized to save power.
//
// Board-specific behavior can override the computed state via the
// `custom_board_getAcrState` hook (see board_overrides.h).
//
// The current ACR state is also published to:
//  - `engine->engineState.acrActive` (for telemetry / outputs),
//  - the composite tooth logger (`LogTriggerAcrState`) so ACR transitions show
//    up alongside trigger, coil and injector events in the CSV log.

#include "pch.h"

#include "board_overrides.h"
#include "tooth_logger.h"

#if EFI_HD_ACR

static bool getAcrState() {
    if (custom_board_getAcrState.has_value()) {
        return custom_board_getAcrState.value()();
    }
    bool engineMovedRecently = getTriggerCentral()->engineMovedRecently();
    engine->engineState.acrEngineMovedRecently = engineMovedRecently;
	auto currentPhase = getTriggerCentral()->getCurrentEnginePhase(getTimeNowNt());
	if (!currentPhase) {
		return engineMovedRecently;
	}

	// Turn off the valve if the engine isn't moving - no sense wasting power on a stopped engine
	if (!engineMovedRecently) {
		return false;
	}

	int revCount = getTriggerCentral()->triggerState.getSynchronizationCounter();
	if (revCount > engineConfiguration->acrRevolutions) {
		// Enough revs have elapsed that we're done with ACR
		return false;
	} else if (revCount == engineConfiguration->acrRevolutions) {
		float wrappedPhase = currentPhase.Value > 360 ? currentPhase.Value - 360 : currentPhase.Value;

		// We're on the rev where ACR should be disabled part way through
		if (wrappedPhase > engineConfiguration->acrDisablePhase) {
			return false;
		} else {
			// Not enough phase elapsed, ACR still active
			return true;
		}
	} else {
		// ACR active - not enough revs completed
		return true;
	}
}

void HarleyAcr::onSlowCallback() {
	// skip if no pin
	if (!isBrainPinValid(engineConfiguration->acrPin)) {
		m_active = false;
		return;
	}

	bool acrState = getAcrState();
	engine->engineState.acrActive = acrState;
	enginePins.harleyAcr.setValue(acrState);
	enginePins.harleyAcr2.setValue(acrState);
#if EFI_TOOTH_LOGGER
	LogTriggerAcrState(getTimeNowNt(), acrState);
#endif // EFI_TOOTH_LOGGER
	m_active = acrState;
}

void HarleyAcr::updateAcr() {
    onSlowCallback();
}

bool HarleyAcr::isActive() const {
	return m_active;
}

#endif // EFI_HD_ACR
