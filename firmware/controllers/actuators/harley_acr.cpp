// Controller for Harley Davidson Automatic Compression Release
//
// To make the starter's job easier, these bike engines have a solenoid
// that dumps cylinder pressure to atmosphere until the engine is spinning
// fast enough to actually have a chance at starting.
// We open the valve the instant the engine starts moving, then close it
// once the specified number of revolutions have occurred, plus some engine phase.
// This allows the valve to close at just the right moment that you don't get a
// weird half-charge if it closed mid way up on a compression stroke.

#include "pch.h"

#if EFI_HD_ACR

static bool getAcrState() {
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
	m_active = acrState;
}

void HarleyAcr::updateAcr() {
    onSlowCallback();
}

bool HarleyAcr::isActive() const {
	return m_active;
}

#endif // EFI_HD_ACR
