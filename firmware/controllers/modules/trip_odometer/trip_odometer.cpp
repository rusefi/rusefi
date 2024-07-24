#include "pch.h"

void TripOdometer::reset() {
	m_consumedGrams = 0;
	m_consumedRemainder = 0;

	m_distanceMeters = 0;
	m_distanceRemainder = 0;

	m_slowCallbackCounter = 0;
	m_engineRunningSeconds = 0;
	m_ignitionOnSeconds = 0;
}

void TripOdometer::consumeFuel(float grams, efitick_t nowNt) {
	m_consumedRemainder += grams;

	while (m_consumedRemainder > 1) {
		m_consumedRemainder--;
		m_consumedGrams++;
	}
	
	m_consumedGrams += grams;

	float elapsedSecond = m_timer.getElapsedSecondsAndReset(nowNt);

	// If it's been a long time since last injection, ignore this pulse
	if (elapsedSecond > 0.2f) {
		m_rate = 0;
	} else {
		m_rate = grams / elapsedSecond;
	}
}

uint32_t TripOdometer::getConsumedGrams() const {
	return m_consumedGrams;
}

float TripOdometer::getConsumptionGramPerSecond() const {
	return m_rate;
}

void TripOdometer::onSlowCallback() {
	float meterPerSecond = Sensor::getOrZero(SensorType::VehicleSpeed) / 3.6f;
	float metersThisTick = meterPerSecond * (SLOW_CALLBACK_PERIOD_MS / 1000.0f);

	m_distanceRemainder += metersThisTick;
	while (m_distanceRemainder > 1.0f) {
		m_distanceMeters++;
		m_distanceRemainder--;
	}

	constexpr float slowCallbackPerSecond = 1000 / SLOW_CALLBACK_PERIOD_MS;
	m_slowCallbackCounter++;
	if (m_slowCallbackCounter == slowCallbackPerSecond) {
		m_slowCallbackCounter = 0;

		m_ignitionOnSeconds++;

#if EFI_SHAFT_POSITION_INPUT
		if (engine->rpmCalculator.isRunning()) {
			m_engineRunningSeconds++;
		}
#endif // EFI_SHAFT_POSITION_INPUT
	}
}

uint32_t TripOdometer::getDistanceMeters() const {
	return m_distanceMeters;
}

uint32_t TripOdometer::getIgnitionOnTime() const {
	return m_ignitionOnSeconds;
}

uint32_t TripOdometer::getEngineRunTime() const {
	return m_engineRunningSeconds;
}
