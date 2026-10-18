/**
 * @file gear_detector.cpp
 * @brief Current gear detection.
 *
 * Infers the engaged transmission gear by comparing the measured RPM-to-vehicle-speed
 * ratio against the configured per-gear ratios, picking the closest match.
 */

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

	bool hasClutchDown = isBrainPinValid(engineConfiguration->clutchDownPin);
	bool hasClutchUp   = isBrainPinValid(engineConfiguration->clutchUpPin);

	if (hasClutchDown || hasClutchUp) {
		bool clutchDown = engine->engineState.clutchDownState;
		bool clutchUp   = engine->engineState.clutchUpState != 0;

		// Clutch disengaged means drivetrain is disconnected — ratio is meaningless
		if ((hasClutchDown && clutchDown) || (hasClutchUp && !clutchUp)) {
			m_gearboxRatio = 0;
			m_currentGear  = 0;
			m_slipPercent  = 0;
			m_slipValid    = false;
			return;
		}
	}

	float ratio = computeGearboxRatio();
	m_gearboxRatio = ratio;

	m_currentGear = determineGearFromRatio(ratio);

	computeSlip();
}

size_t GearDetector::determineGearFromRatio(float ratio) const {
	// NaN makes every comparison below false, otherwise falling through to first gear.
	if (!std::isfinite(ratio)) {
		return 0;
	}

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
	if (engineConfiguration->gearDetectionUseOutputShaftSpeed) {
		// OSS is measured at the transmission output - it already *is* driveshaft RPM, no
		// conversion needed. Returns invalid/0 if OSS isn't actually configured/registered.
		return Sensor::getOrZero(SensorType::OutputShaftSpeed);
	}

	// Preserve the existing main-VSS path and its automatic input-shaft RPM selection.
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
	if (engineConfiguration->gearDetectionUseOutputShaftSpeed) {
		// Explicit choice, only meaningful when paired with Output Shaft Speed.
		engineRpm = engineConfiguration->gearDetectionRpmSourceIsInputShaftSpeed
				? Sensor::getOrZero(SensorType::InputShaftSpeed)
				: Sensor::getOrZero(SensorType::Rpm);
	} else if (Sensor::hasSensor(SensorType::InputShaftSpeed)) {
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

float GearDetector::getSlipPercent() const {
	return m_slipPercent;
}

bool GearDetector::isSlipValid() const {
	return m_slipValid;
}

void GearDetector::computeSlip() {
	m_slipPercent = 0;
	m_slipValid = false;

	if (!engineConfiguration->transmissionSlipDetectionEnabled || m_currentGear == 0 || !std::isfinite(m_gearboxRatio)) {
		return;
	}

	// This checkout retains the legacy VSS providers, which do not identify their physical
	// source. Only the explicit OSS path guarantees an output-shaft reference for slip.
	if (!engineConfiguration->gearDetectionUseOutputShaftSpeed) {
		return;
	}

	// Below this speed the driveshaft-RPM ratio math is unstable (near-0 denominator), same
	// class of issue as getDriveshaftRpm()'s hardcoded <3kph floor on the wheel-speed path, but
	// user-configurable here since the OSS-direct path has no such floor of its own.
	auto vss = Sensor::get(SensorType::VehicleSpeed);
	if (!vss.Valid || !std::isfinite(vss.Value) || vss.Value < engineConfiguration->transmissionSlipMinVss) {
		return;
	}

	auto rpmSourceType = engineConfiguration->transmissionSlipRpmSourceIsInputShaftSpeed
			? SensorType::InputShaftSpeed
			: SensorType::Rpm;

	auto actualRpm = Sensor::get(rpmSourceType);
	if (!actualRpm.Valid || !std::isfinite(actualRpm.Value) || actualRpm.Value < 0) {
		return;
	}

	// Ideal (no-slip) RPM for the Detected Gear given the current driveshaft speed.
	float expectedRpm = getRpmInGear(m_currentGear);
	if (!std::isfinite(expectedRpm) || expectedRpm <= 0) {
		return;
	}

	m_slipPercent = 100.0f * (actualRpm.Value / expectedRpm - 1.0f);
	m_slipValid = true;
}

SensorResult GearDetector::get() const {
	return m_currentGear;
}

void GearDetector::showInfo(const char* sensorName) const {
	efiPrintf("Sensor \"%s\" is gear detector.", sensorName);
	efiPrintf("    Gearbox ratio: %.3f", m_gearboxRatio);
	efiPrintf("    Detected gear: %d", m_currentGear);
	efiPrintf("    Slip: %.1f%% (%s)", m_slipPercent, m_slipValid ? "valid" : "invalid");
}
