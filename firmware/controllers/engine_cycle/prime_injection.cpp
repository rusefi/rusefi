/*
 * @file prime_injection.cpp
 */

#include "pch.h"
#include "prime_injection.h"
#include "injection_gpio.h"
#include "sensor.h"
#include "backup_ram.h"
#if EFI_PROD_CODE
#include "microsecond_timer.h"
#endif

floatms_t PrimeController::getPrimeDuration() const {
	auto clt = Sensor::get(SensorType::Clt);

	// If the coolant sensor is dead, skip the prime. The engine will still start fine, but may take a little longer.
	if (!clt) {
		return 0;
	}

	auto primeMass =
		0.001f *	// convert milligram to gram
		interpolate2d(clt.Value, engineConfiguration->primeBins, engineConfiguration->primeValues);

	efiPrintf("Priming pulse mass: %.4f g", primeMass);

	return engine->module<InjectorModelPrimary>()->getInjectionDuration(primeMass);
}

// Check if the engine is not stopped or cylinder cleanup is activated
static bool isPrimeInjectionPulseSkipped() {
	// Skip if the engine is already spinning
	if (!getEngineRotationState()->isStopped()) {
		return true;
	}

	// Skip if cylinder cleanup is active
	return engineConfiguration->isCylinderCleanupEnabled && (Sensor::getOrZero(SensorType::Tps1) > CLEANUP_MODE_TPS);
}

void PrimeController::onIgnitionStateChanged(bool ignitionOn) {
	if (!ignitionOn) {
		// don't prime on ignition-off
		return;
	}

	// First, we need a protection against 'fake' ignition switch on and off (i.e. no engine started), to avoid repeated prime pulses.
	// So we check and update the ignition switch counter in non-volatile backup-RAM
	uint32_t ignSwitchCounter = getKeyCycleCounter();

	// if we're just toying with the ignition switch, give it another chance eventually...
	if (ignSwitchCounter > 10) {
		ignSwitchCounter = 0;
	}

	// If we're going to skip this pulse, then save the counter as 0.
	// That's because we'll definitely need the prime pulse next time (either due to the cylinder cleanup or the engine spinning)
	if (isPrimeInjectionPulseSkipped()) {
		ignSwitchCounter = -1;
	}

	// start prime injection if this is a 'fresh start'
	if (ignSwitchCounter == 0) {
		// Give sensors long enough to wake up before priming
		constexpr float minimumPrimeDelayMs = 100;
		int32_t primeDelayNt = assertFloatFitsInto32BitsAndCast("primingDelay", MSF2NT(engineConfiguration->primingDelay * 1000 + minimumPrimeDelayMs));

		auto startTime = getTimeNowNt() + primeDelayNt;
		getScheduler()->schedule("primingDelay", nullptr, startTime, { PrimeController::onPrimeStartAdapter, this });
	} else {
		efiPrintf("Skipped priming pulse since ignSwitchCounter = %lu", ignSwitchCounter);
	}

	// we'll reset it later when the engine starts
	setKeyCycleCounter(ignSwitchCounter + 1);
}

void PrimeController::setKeyCycleCounter(uint32_t count) {
#if EFI_BACKUP_SRAM
	backupRamSave(backup_ram_e::IgnCounter, count);
#endif // EFI_BACKUP_SRAM
}

uint32_t PrimeController::getKeyCycleCounter() const {
#if EFI_BACKUP_SRAM
	return backupRamLoad(backup_ram_e::IgnCounter);
#else // not EFI_BACKUP_SRAM
	return 0;
#endif // EFI_BACKUP_SRAM
}

void PrimeController::onPrimeStart() {
	auto durationMs = getPrimeDuration();

	// Don't prime a zero-duration pulse
	if (durationMs <= 0) {
		efiPrintf("Skipped zero-duration priming pulse.");
		return;
	}
#if EFI_PROD_CODE
	if (durationMs >= TOO_FAR_INTO_FUTURE_MS) {
	  criticalError("Priming duration too long %dms", durationMs);
	}
#endif

	efiPrintf("Firing priming pulse of %.2f ms", durationMs);
	engine->outputChannels.injectionPrimingCounter++;

	auto endTime = sumTickAndFloat(getTimeNowNt(), MSF2NT(durationMs));

	// Open all injectors, schedule closing later
	m_isPriming = true;
	startSimultaneousInjection();
	getScheduler()->schedule("onPrimeStart", nullptr, endTime, { onPrimeEndAdapter, this });
}

void PrimeController::onPrimeEnd() {
	endSimultaneousInjectionOnlyTogglePins();

	m_isPriming = false;
}

void PrimeController::onSlowCallback() {
	if (!getEngineRotationState()->isStopped()) {
#if EFI_BACKUP_SRAM
		backupRamSave(backup_ram_e::IgnCounter, 0);
#endif /* EFI_BACKUP_SRAM */
	}
}
