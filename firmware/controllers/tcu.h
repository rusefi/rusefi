/**
 * @file        tcu.h
 * @brief       Base classes for gear selection and transmission control
 *
 * @date Aug 31, 2020
 * @author David Holdeman, (c) 2020
 */
#pragma once

#include "global.h"
#include "io_pins.h"
#include "persistent_configuration.h"
#include "engine_configuration_generated_structures.h"
#include "globalaccess.h"

class TransmissionControllerBase {
public:
    void update(gear_e);
    void init();
    gear_e getCurrentGear() const;
private:
    gear_e currentGear = NEUTRAL;
protected:
    gear_e setCurrentGear(gear_e);
    void postState();
};
