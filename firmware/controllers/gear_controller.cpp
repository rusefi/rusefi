#include "gear_controller.h"
#include "tunerstudio_outputs.h"
#include "adc_inputs.h"
#include "engine_configuration.h"

EXTERN_CONFIG

#define vRange(x,y,z) (x > (y-z) && x < (y+z))

void GearControllerBase::init(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
    for (int i = 0; i < TCU_INPUT_COUNT; i++) {
        if (CONFIG(tcuInputMode)) {
            tcuDigitalInputs[i].init(10, *getDigitalInputPin(i), *getDigitalInputPinMode(i));
        } else {
            tcuAnalogInputs[i] = getAnalogInputPin(i);
        }
    }
    INJECT_ENGINE_REFERENCE(&transmissionController);
    transmissionController.init();
}

void GearControllerBase::update() {
    // We are responsible for telling the transmission controller
    //  what gear we want.
    transmissionController.update(getDesiredGear());
    // Post state to TS
    postState();
}

gear_e GearControllerBase::getDesiredGear() const {
    return desiredGear;
}

gear_e GearControllerBase::setDesiredGear(gear_e gear) {
    desiredGear = gear;
    return getDesiredGear();
}

int GearControllerBase::getInputRole() {
    if (CONFIG(tcuInputMode)) {
        return getInputRoleD();
    } else {
        return getInputRoleA();
    }
}

int GearControllerBase::getInputRoleD() {
    bool states[TCU_INPUT_COUNT];
    for (int i = 0; i < TCU_INPUT_COUNT; i++) {
        states[i] = tcuDigitalInputs[i].readPinState();
    }
    for (int i = 0; i < TCU_STATE_COUNT; i++) {
        for (int ii = 0; ii < TCU_INPUT_COUNT; ii++) {
            if ((config->tcuInputTable[ii][i] != states[i]) && (config->tcuInputTable[ii][i] >= 0)) {
                break;
            }
            if (ii == (TCU_INPUT_COUNT - 1)) {
                return i;
            }
        }
    }
    return -1;
}

int GearControllerBase::getInputRoleA() {
    int levels[TCU_INPUT_COUNT];
    for (int i = 0; i < TCU_INPUT_COUNT; i++) {
        levels[i] = adcToVolts(getAdcValue("tcuinput", tcuAnalogInputs[i]));
    }
    for (int i = 0; i < TCU_STATE_COUNT; i++) {
        for (int ii = 0; ii < TCU_INPUT_COUNT; ii++) {
            if (!(vrange(levels(i),config->tcuInputTable[ii][i],config->tcuAnalogInputTolerance) && (config->tcuInputTable[ii][i] >= 0)) {
                break;
            }
            if (ii == (TCU_INPUT_COUNT - 1)) {
                return i;
            }
        }
    }
    return -1;
}

brain_pin_e* GearControllerBase::getDigitalInputPin(int input) {
    switch (input) {
        case 1:
            return &CONFIG(tcuInputPin1D);
        case 2:
            return &CONFIG(tcuInputPin2D);
        case 3:
            return &CONFIG(tcuInputPin3D);
        case 4:
            return &CONFIG(tcuInputPin4D);
        case 5:
            return &CONFIG(tcuInputPin5D);
        case 6:
            return &CONFIG(tcuInputPin6D);
    }
    return nullptr;
}

pin_input_mode_e* GearControllerBase::getDigitalInputPinMode(int input) {
    switch (input) {
        case 1:
            return &CONFIG(tcuInputPin1Mode);
        case 2:
            return &CONFIG(tcuInputPin2Mode);
        case 3:
            return &CONFIG(tcuInputPin3Mode);
        case 4:
            return &CONFIG(tcuInputPin4Mode);
        case 5:
            return &CONFIG(tcuInputPin5Mode);
        case 6:
            return &CONFIG(tcuInputPin6Mode);
    }
    return nullptr;
}

adc_channel_e GearControllerBase::getAnalogInputPin(int input) {
    switch (input) {
        case 1:
            return CONFIG(tcuInputPin1A);
        case 2:
            return CONFIG(tcuInputPin2A);
        case 3:
            return CONFIG(tcuInputPin3A);
        case 4:
            return CONFIG(tcuInputPin4A);
        case 5:
            return CONFIG(tcuInputPin5A);
        case 6:
            return CONFIG(tcuInputPin6A);
    }
    return EFI_ADC_NONE;
}

void GearControllerBase::postState() {
#if EFI_TUNER_STUDIO
    tsOutputChannels.tcuDesiredGear = getDesiredGear();
#endif
}

