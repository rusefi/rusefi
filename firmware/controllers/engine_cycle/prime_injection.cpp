/*
 * @file prime_injection.cpp
 */

#include "pch.h"
#include "prime_injection.h"
#include "efi_gpio.h"
#include "sensor.h"


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
