/*
 * @file prime_injection.cpp
 */

#include "pch.h"
#include "prime_injection.h"
#include "injection_gpio.h"
#include "sensor.h"
#include "backup_ram.h"

void PrimeController::onPrimeEnd() {
	endSimultaneousInjectionOnlyTogglePins();

	m_isPriming = false;
}

floatms_t PrimeController::getPrimeDuration() const {
	auto clt = Sensor::get(SensorType::Clt);

	// If the coolant sensor is dead, skip the prime. The engine will still start fine, but may take a little longer.
	if (!clt) {
		return 0;
	}

	auto primeMass =
		0.001f *	// convert milligram to gram
		interpolate2d(clt.Value, engineConfiguration->primeBins, engineConfiguration->primeValues);

	return engine->module<InjectorModel>()->getInjectionDuration(primeMass);
}
// Check if the engine is not stopped or cylinder cleanup is activated
static bool isPrimeInjectionPulseSkipped() {
	if (!getEngineRotationState()->isStopped())
		return true;
	return engineConfiguration->isCylinderCleanupEnabled && (Sensor::getOrZero(SensorType::Tps1) > CLEANUP_MODE_TPS);
}

/**
 * Prime injection pulse
 */
void PrimeController::onIgnitionStateChanged(bool ignitionOn) {
	if (!ignitionOn) {
		// don't prime on ignition-off
		return;
	}

	// First, we need a protection against 'fake' ignition switch on and off (i.e. no engine started), to avoid repeated prime pulses.
	// So we check and update the ignition switch counter in non-volatile backup-RAM
#if EFI_PROD_CODE
	uint32_t ignSwitchCounter = backupRamLoad(BACKUP_IGNITION_SWITCH_COUNTER);
#else /* EFI_PROD_CODE */
	uint32_t ignSwitchCounter = 0;
#endif /* EFI_PROD_CODE */

	// if we're just toying with the ignition switch, give it another chance eventually...
	if (ignSwitchCounter > 10)
		ignSwitchCounter = 0;
	// If we're going to skip this pulse, then save the counter as 0.
	// That's because we'll definitely need the prime pulse next time (either due to the cylinder cleanup or the engine spinning)
	if (isPrimeInjectionPulseSkipped())
		ignSwitchCounter = -1;
	// start prime injection if this is a 'fresh start'
	if (ignSwitchCounter == 0) {
		auto primeDelayMs = engineConfiguration->primingDelay * 1000;

		auto startTime = getTimeNowNt() + MS2NT(primeDelayMs);
		getExecutorInterface()->scheduleByTimestampNt("prime", &m_start, startTime, { PrimeController::onPrimeStartAdapter, this});
	} else {
		efiPrintf("Skipped priming pulse since ignSwitchCounter = %d", ignSwitchCounter);
	}
#if EFI_PROD_CODE
	// we'll reset it later when the engine starts
	backupRamSave(BACKUP_IGNITION_SWITCH_COUNTER, ignSwitchCounter + 1);
#endif /* EFI_PROD_CODE */
}

void PrimeController::onPrimeStart() {
	auto durationMs = getPrimeDuration();

	// Don't prime a zero-duration pulse
	if (durationMs <= 0) {
		efiPrintf("Skipped zero-duration priming pulse.");
		return;
	}

	efiPrintf("Firing priming pulse of %.2f ms", durationMs);

	auto endTime = getTimeNowNt() + MS2NT(durationMs);

	// Open all injectors, schedule closing later
	m_isPriming = true;
	startSimultaneousInjection();
	getExecutorInterface()->scheduleByTimestampNt("prime", &m_end, endTime, { onPrimeEndAdapter, this });
}

void updatePrimeInjectionPulseState() {
	static bool counterWasReset = false;
	if (counterWasReset)
		return;

	if (!getEngineRotationState()->isStopped()) {
#if EFI_PROD_CODE
		backupRamSave(BACKUP_IGNITION_SWITCH_COUNTER, 0);
#endif /* EFI_PROD_CODE */
		counterWasReset = true;
	}
}

