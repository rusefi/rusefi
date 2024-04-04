/**
 * @file        tcu.cpp
 * @brief       Base classes for gear selection and transmission control
 *
 * @date Aug 31, 2020
 * @author David Holdeman, (c) 2020
 */

#include "pch.h"

#include "tcu.h"

#if EFI_TCU
void TransmissionControllerBase::init() {
}

void TransmissionControllerBase::update(gear_e /*gear*/) {
	postState();
}

gear_e TransmissionControllerBase::setCurrentGear(gear_e gear) {
    currentGear = gear;
    return getCurrentGear();
}

gear_e TransmissionControllerBase::getCurrentGear() const {
    return currentGear;
}

void TransmissionControllerBase::postState() {
#if EFI_TUNER_STUDIO
	auto iss = Sensor::get(SensorType::InputShaftSpeed);
	auto rpm = Sensor::get(SensorType::Rpm);
	if (iss.Valid && rpm.Valid) {
		tcRatio = rpm.Value / iss.Value;
	}
  tcuCurrentGear = getCurrentGear();
#endif
}

void TransmissionControllerBase::measureShiftTime(gear_e gear) {
	m_shiftTime = true;
	m_shiftTimer.reset();
	m_shiftTimeGear = gear;
}

float TransmissionControllerBase::isShiftCompleted() {
	auto detected = Sensor::get(SensorType::DetectedGear);
	auto iss = Sensor::get(SensorType::InputShaftSpeed);
	if (detected.Valid && m_shiftTime && m_shiftTimeGear == detected.Value) {
		m_shiftTime = false;
		return m_shiftTimer.getElapsedSeconds();
	} else if (!iss.Valid && m_shiftTime && m_shiftTimer.hasElapsedMs(config->tcu_shiftTime)) {
		m_shiftTime = false;
		return config->tcu_shiftTime * 0.001;
	} else {
		return 0;
	}
}
#endif // EFI_TCU
