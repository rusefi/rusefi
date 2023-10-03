#pragma once

class TripOdometer : public EngineModule {
public:
	void onSlowCallback() override;
	void reset();

	void consumeFuel(float grams, efitick_t nowNt);

	uint32_t getConsumedGrams() const;
	float getConsumptionGramPerSecond() const;
	uint32_t getDistanceMeters() const;
	uint32_t getIgnitionOnTime() const;
	uint32_t getEngineRunTime() const;

private:
	// Fuel consumed
	uint32_t m_consumedGrams = 0;
	float m_consumedRemainder = 0;
	float m_rate = 0;

	// Distance travelled
	uint32_t m_distanceMeters = 0;
	float m_distanceRemainder = 0;

	// Engine run time and ign on time
	uint32_t m_slowCallbackCounter = 0;
	uint32_t m_ignitionOnSeconds = 0;
	uint32_t m_engineRunningSeconds = 0;

	Timer m_timer;
};
