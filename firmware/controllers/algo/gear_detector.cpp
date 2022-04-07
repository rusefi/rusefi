#include "pch.h"

static constexpr float geometricMean(float x, float y) {
	return sqrtf(x * y);
}

void GearDetector::onConfigurationChange(engine_configuration_s const * /*previousConfig*/) {
	// Compute gear thresholds between gears

	for (size_t i = 0; i < efi::size(m_gearThresholds); i++) {
		// Threshold i is the threshold between gears i and i+1

		m_gearThresholds[i] = geometricMean(
			engineConfiguration->gearRatio[i],
			engineConfiguration->gearRatio[i + 1]
		);

		// TODO: validate gears are in correct order
	}
}

void GearDetector::onSlowCallback() {
	float ratio = computeGearboxRatio();
	m_gearboxRatio = ratio;

	m_currentGear = determineGearFromRatio(ratio);

	// TODO: solve for which gear this is
}

size_t GearDetector::determineGearFromRatio(float ratio) const {
	// 1.5x first gear is neutral or clutch slip or something
	if (ratio > engineConfiguration->gearRatio[0] * 1.5f) {
		return 0;
	}

	auto gearCount = engineConfiguration->totalGearsCount;

	// 0.66x top gear is coasting with engine off or something
	if (ratio < engineConfiguration->gearRatio[gearCount - 1] * 0.66f) {
		return 0;
	}

	size_t currentGear = gearCount;

	while (currentGear > 1) {
		if (ratio < m_gearThresholds[currentGear - 2]) {
			break;
		}

		currentGear--;
	}

	return currentGear;
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
