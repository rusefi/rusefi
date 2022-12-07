#pragma once

class GearDetector : public EngineModule {
public:
	void onSlowCallback() override;
	void onConfigurationChange(engine_configuration_s const * /*previousConfig*/) override;

	float getGearboxRatio() const;

	// Returns 0 for neutral, 1 for 1st, 5 for 5th, etc.
	size_t getCurrentGear() const;

	size_t determineGearFromRatio(float ratio) const;

	float getRpmInGear(size_t gear) const;

private:
	float computeGearboxRatio() const;
	float getDriveshaftRpm() const;

	float m_gearboxRatio = 0;
	size_t m_currentGear = 0;

	float m_gearThresholds[GEARS_COUNT - 1];
};
