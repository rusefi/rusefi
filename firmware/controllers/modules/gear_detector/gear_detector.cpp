#include "pch.h"
#include "gear_detector.h"

static constexpr float geometricMean(float x, float y) {
	return sqrtf(x * y);
}

GearDetector::GearDetector()
	: Sensor(SensorType::DetectedGear)
{
}

GearDetector::~GearDetector() {
	unregister();
}

void GearDetector::initGearDetector() {
	// Compute gear thresholds between gears

	uint8_t gearCount = engineConfiguration->totalGearsCount;

	if (gearCount == 0) {
		// No gears, nothing to do here.
		return;
	}

	if (gearCount > TCU_GEAR_COUNT) {
		criticalError("too many gears");
		return;
	}

	// validate gears
	for (size_t i = 0; i < gearCount; i++) {
		if (engineConfiguration->gearRatio[i] <= 0) {
			criticalError("Expecting positive gear ratio for #%d", i + 1);
			return;
		}
	}

	for (int i = 0; i < gearCount - 1; i++) {
		// Threshold i is the threshold between gears i and i+1
		float gearI        = engineConfiguration->gearRatio[i];
		float gearIplusOne = engineConfiguration->gearRatio[i + 1];

		if (gearI <= gearIplusOne) {
			criticalError("Invalid gear ordering near gear #%d", i + 1);
		}

		m_gearThresholds[i] = geometricMean(gearI, gearIplusOne);
	}

	Register();
}

void GearDetector::onConfigurationChange(engine_configuration_s const * /*previousConfig*/) {
    initGearDetector();
}

void GearDetector::onSlowCallback() {
    if (!isInitialized) {
        initGearDetector();
        isInitialized = true;
    }

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

	if (vssKph < 3) {
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

SensorResult GearDetector::get() const {
	return m_currentGear;
}

void GearDetector::showInfo(const char* sensorName) const {
	efiPrintf("Sensor \"%s\" is gear detector.", sensorName);
	efiPrintf("    Gearbox ratio: %.3f", m_gearboxRatio);
	efiPrintf("    Detected gear: %d", m_currentGear);
}
