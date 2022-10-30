/**
 * @file        tcu.cpp
 * @brief       Base classes for gear selection and transmission control
 *
 * @date Aug 31, 2020
 * @author David Holdeman, (c) 2020
 */

#include "pch.h"

#include "tcu.h"
#include "engine.h"

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
    engine->outputChannels.tcuCurrentGear = getCurrentGear();
#endif
}

void TransmissionControllerBase::measureShiftTime(gear_e gear) {
	m_shiftTime = true;
	m_shiftTimer.reset();
	m_shiftTimeGear = gear;
}

float TransmissionControllerBase::isShiftCompleted() {
	if (m_shiftTime &&  m_shiftTimeGear == engine->module<GearDetector>()->getCurrentGear()) {
		m_shiftTime = false;
		return m_shiftTimer.getElapsedSeconds();
	} else {
		return 0;
	}
}
#endif // EFI_TCU
