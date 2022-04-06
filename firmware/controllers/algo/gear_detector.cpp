#include "pch.h"

void GearDetector::onSlowCallback() {
	m_gearboxRatio = computeGearboxRatio();

	// TODO: solve for which gear this is
}

float GearDetector::computeGearboxRatio() const {
	auto vssKph = Sensor::getOrZero(SensorType::VehicleSpeed);

	if (vssKph < 5) {
		// Vehicle too slow to determine gearbox ratio, avoid div/0
		return 0;
	}

	auto engineRpm = Sensor::getOrZero(SensorType::Rpm);

	// Convert to wheel RPM
	//                 km                        rev                        1 hr
	//               ------ *               ------------              *  __________
	//                 hr                        km                        60 min
	float wheelRpm = vssKph * engineConfiguration->driveWheelRevPerKm * (1 / 60.0f);

	// Convert to driveshaft RPM
	auto driveshaftRpm = wheelRpm * engineConfiguration->finalGearRatio;

	return engineRpm / driveshaftRpm;
}

float GearDetector::getGearboxRatio() const {
	return m_gearboxRatio;
}
