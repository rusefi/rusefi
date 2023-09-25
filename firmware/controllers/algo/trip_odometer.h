#pragma once

class TripOdometer : public EngineModule {
public:
	void onSlowCallback() override;
	void reset();

	void consumeFuel(float grams, efitick_t nowNt);

	uint32_t getConsumedGrams() const;
	float getConsumptionGramPerSecond() const;
	uint32_t getDistanceMeters() const;

private:
	uint32_t m_consumedGrams = 0;
	float m_consumedRemainder = 0;
	float m_rate = 0;

	uint32_t m_distanceMeters = 0;
	float m_distanceRemainder = 0;

	Timer m_timer;
};
