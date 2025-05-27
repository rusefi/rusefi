/*
 * @file engine_cylinder.cpp
 *
 * @date: may 25, 2025
 * @author Matthew Kennedy, FDSoftware
 */
#include "pch.h"
#include "engine_cylinder.hpp"

void OneCylinder::updateCylinderNumber(uint8_t index, uint8_t cylinderNumber) {
    m_cylinderIndex = index;
    m_cylinderNumber = cylinderNumber;
    m_valid = true;
}

void OneCylinder::invalidCylinder() {
    m_valid = false;
}

void EngineCylinders::updateCylinders() {
    // Update valid cylinders with their position in the firing order
    uint16_t cylinderUpdateMask = 0;
    for (size_t i = 0; i < engineConfiguration->cylindersCount; i++) {
        auto cylinderNumber = getCylinderNumberAtIndex(i);

        engine->cylinders[cylinderNumber].updateCylinderNumber(i, cylinderNumber);

        auto mask = 1 << cylinderNumber;
        // Assert that this cylinder was not configured yet
        efiAssertVoid(ObdCode::OBD_PCM_Processor_Fault, (cylinderUpdateMask & mask) == 0, "cylinder update err");
        cylinderUpdateMask |= mask;
    }

    // Assert that all cylinders were configured
    uint16_t expectedMask = (1 << (engineConfiguration->cylindersCount)) - 1;
    efiAssertVoid(ObdCode::OBD_PCM_Processor_Fault, cylinderUpdateMask == expectedMask, "cylinder update err");

    // Invalidate the remaining cylinders
    for (size_t i = engineConfiguration->cylindersCount; i < efi::size(engine->cylinders); i++) {
        engine->cylinders[i].invalidCylinder();
    }
}
