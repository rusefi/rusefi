#pragma once

#include "global.h"
#include "io_pins.h"
#include "persistent_configuration.h"
#include "engine_configuration_generated_structures.h"
#include "globalaccess.h"
#include "simple_tcu.h"

class GearControllerBase {
public:
    DECLARE_ENGINE_PTR;

    virtual void update();
    gear_e getDesiredGear() const;
    virtual void init(DECLARE_ENGINE_PARAMETER_SIGNATURE);
private:
    gear_e desiredGear = NEUTRAL;
    adc_channel_e getAnalogInputPin;
    brain_pin_e getDigitalInputPin;
    int getInputRoleD();
    int getInputRoleA();
    bool tcuDigitalInputs[TCU_INPUT_COUNT];
    float tcuAnalogInputs[TCU_INPUT_COUNT];
protected:
    gear_e setDesiredGear(gear_e);
    int getInputRole();

private:
    void postState();
    SimpleTransmissionController transmissionController;
};
