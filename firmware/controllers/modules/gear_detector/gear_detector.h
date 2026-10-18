#pragma once
#include "sensor.h"

class GearDetector : public EngineModule, public Sensor {
public:
	GearDetector();
	~GearDetector();

	void onSlowCallback() override;
	void onConfigurationChange(engine_configuration_s const * /*previousConfig*/) override;

	float getGearboxRatio() const;

	// Returns 0 for neutral, 1 for 1st, 5 for 5th, etc.
	size_t determineGearFromRatio(float ratio) const;

	float getRpmInGear(size_t gear) const;

	// Percent deviation of the configured Slip RPM Source from the RPM expected for the
	// Detected Gear. 0 = no slip, positive = source spinning faster than expected. Reads 0
	// whenever isSlipValid() is false -- callers that need to distinguish "confirmed no slip"
	// from "can't tell right now" must check isSlipValid() too, not just this value.
	float getSlipPercent() const;

	// False whenever slip can't currently be measured: Slip Detection disabled, Detected Gear is
	// neutral, the driveshaft speed reference isn't a real Output Shaft Speed sensor, vehicle
	// speed is below transmissionSlipMinVss, or the configured RPM source sensor is invalid.
	bool isSlipValid() const;

	SensorResult get() const override;
	void showInfo(const char* sensorName) const override;

private:
	float computeGearboxRatio() const;
	void computeSlip();
	float getDriveshaftRpm() const;
	void initGearDetector();
    bool isInitialized = false;

	float m_gearboxRatio = 0;
	float m_slipPercent = 0;
	bool m_slipValid = false;
	size_t m_currentGear = 0;

	float m_gearThresholds[TCU_GEAR_COUNT - 1];
};
