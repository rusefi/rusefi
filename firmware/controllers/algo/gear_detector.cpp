#include "pch.h"

static constexpr float geometricMean(float x, float y) {
	return sqrtf(x * y);
}

void GearDetector::onConfigurationChange(engine_configuration_s const * /*previousConfig*/) {
	// Compute gear thresholds between gears

	uint8_t gearCount = engineConfiguration->totalGearsCount;

	if (gearCount == 0) {
		// No gears, nothing to do here.
		return;
	}

	if (gearCount > GEARS_COUNT) {
		firmwareError(ObdCode::OBD_PCM_Processor_Fault, "too many gears");
		return;
	}

	// validate gears
	for (size_t i = 0; i < gearCount; i++) {
		if (engineConfiguration->gearRatio[i] <= 0) {
			firmwareError(ObdCode::OBD_PCM_Processor_Fault, "Invalid gear ratio for #%d", i + 1);
			return;
		}
	}

	for (int i = 0; i < gearCount - 1; i++) {
		// Threshold i is the threshold between gears i and i+1
		float gearI        = engineConfiguration->gearRatio[i];
		float gearIplusOne = engineConfiguration->gearRatio[i + 1];

		if (gearI <= gearIplusOne) {
			firmwareError(ObdCode::OBD_PCM_Processor_Fault, "Invalid gear ordering near gear #%d", i + 1);
		}

		m_gearThresholds[i] = geometricMean(gearI, gearIplusOne);
	}
}

void GearDetector::onSlowCallback() {
	float ratio = computeGearboxRatio();
	m_gearboxRatio = ratio;

	m_currentGear = determineGearFromRatio(ratio);
}

size_t GearDetector::determineGearFromRatio(float ratio) const {
	auto gearCount = engineConfiguration->totalGearsCount;
	if (gearCount == 0) {
		// No gears, we only have neutral.
		return 0;
	}

	// 1.5x first gear is neutral or clutch slip or something
	if (ratio > engineConfiguration->gearRatio[0] * 1.5f) {
		return 0;
	}

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

float GearDetector::getDriveshaftRpm() const {
	auto vssKph = Sensor::getOrZero(SensorType::VehicleSpeed);

	if (vssKph < 5) {
		// Vehicle too slow to determine gearbox ratio, avoid div/0
		return 0;
	}

	// Convert to wheel RPM
	//                 km                        rev                        1 hr
	//               ------ *               ------------              *  __________
	//                 hr                        km                        60 min
	float wheelRpm = vssKph * engineConfiguration->driveWheelRevPerKm * (1 / 60.0f);

	// Convert to driveshaft RPM
	return wheelRpm * engineConfiguration->finalGearRatio;
}

float GearDetector::computeGearboxRatio() const {
	float driveshaftRpm = getDriveshaftRpm();

	if (driveshaftRpm == 0) {
		return 0;
	}

	float engineRpm;
	if (Sensor::hasSensor(SensorType::InputShaftSpeed)) {
		engineRpm = Sensor::getOrZero(SensorType::InputShaftSpeed);
	} else {
		engineRpm = Sensor::getOrZero(SensorType::Rpm);
	}

	return engineRpm / driveshaftRpm;
}

float GearDetector::getRpmInGear(size_t gear) const {
	if (gear <= 0 || gear > engineConfiguration->totalGearsCount) {
		return 0;
	}

	// Ideal engine RPM is driveshaft speed times gear
	return getDriveshaftRpm() * engineConfiguration->gearRatio[gear - 1];
}

float GearDetector::getGearboxRatio() const {
	return m_gearboxRatio;
}

size_t GearDetector::getCurrentGear() const {
	return m_currentGear;
}
