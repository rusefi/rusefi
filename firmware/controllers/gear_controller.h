#pragma once

#include "global.h"
#include "io_pins.h"
#include "persistent_configuration.h"
#include "engine_configuration_generated_structures.h"
#include "globalaccess.h"
#include "simple_tcu.h"

class GearControllerBase {
public:
    virtual void update();
    gear_e getDesiredGear() const;
    virtual void init();
private:
    gear_e desiredGear = NEUTRAL;
protected:
    gear_e setDesiredGear(gear_e);

private:
    void postState();
    SimpleTransmissionController transmissionController;
};
