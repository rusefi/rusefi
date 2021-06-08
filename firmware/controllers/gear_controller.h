#pragma once

#include "global.h"
#include "io_pins.h"
#include "persistent_configuration.h"
#include "engine_configuration_generated_structures.h"
#include "globalaccess.h"
#include "simple_tcu.h"
#include "debounce.h"

class GearControllerBase {
public:
    DECLARE_ENGINE_PTR;

    virtual void update();
    gear_e getDesiredGear() const;
    virtual void init(DECLARE_ENGINE_PARAMETER_SIGNATURE);
private:
    gear_e desiredGear = NEUTRAL;
protected:
    adc_channel_e getAnalogInputPin(int);
    brain_pin_e *getDigitalInputPin(int);
    pin_input_mode_e getDigitalInputPinMode(int);
    int getInputRoleD();
    int getInputRoleA();
    ButtonDebounce tcuDigitalInputs[TCU_INPUT_COUNT];
    adc_channel_e tcuAnalogInputs[TCU_INPUT_COUNT];
    gear_e setDesiredGear(gear_e);
    int getInputRole();

private:
    void postState();
    SimpleTransmissionController transmissionController;
};
