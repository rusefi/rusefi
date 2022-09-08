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

