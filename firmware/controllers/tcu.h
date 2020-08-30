#pragma once

#include "global.h"
#include "io_pins.h"
#include "persistent_configuration.h"
#include "engine_configuration_generated_structures.h"
#include "globalaccess.h"

class TransmissionControllerBase {
public:
    void update(gear_e);
    gear_e getCurrentGear();
private:
    gear_e currentGear = NEUTRAL;
protected:
    gear_e setCurrentGear(gear_e);
    void postState();
};

class GearControllerBase {
public:
    virtual void update();
    gear_e getDesiredGear();
private:
    gear_e desiredGear = NEUTRAL;
protected:
    gear_e setDesiredGear(gear_e);
    void postState();
    TransmissionControllerBase transmissionController;
};
