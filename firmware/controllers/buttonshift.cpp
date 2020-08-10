#include "buttonshift.h"
#include "engine.h"

EXTERN_ENGINE;

ButtonShiftController::ButtonShiftController (DECLARE_ENGINE_PARAMETER_SIGNATURE) {
    ButtonDebounce u = ButtonDebounce(10, CONFIG(tcuUpshiftButtonPin), CONFIG(tcuUpshiftButtonPinMode));
    debounceUp = &u;
    ButtonDebounce d = ButtonDebounce(10, CONFIG(tcuDownshiftButtonPin), CONFIG(tcuDownshiftButtonPinMode));
    debounceDown = &d;
}

void ButtonShiftController::update() {
    bool upPinState = debounceUp->readPin();
    bool downPinState = debounceUp->readPin();
    gear_e gear = getDesiredGear();
    if (upPinState) {
        switch (gear) {
            case GEAR_1:
                setDesiredGear(GEAR_2);
                break;
            case GEAR_2:
                setDesiredGear(GEAR_3);
                break;
            case GEAR_3:
                setDesiredGear(GEAR_4);
                break;
            default:
                break;
        }
    } else if (downPinState) {
        switch (gear) {
            case GEAR_2:
                setDesiredGear(GEAR_1);
                break;
            case GEAR_3:
                setDesiredGear(GEAR_2);
                break;
            case GEAR_4:
                setDesiredGear(GEAR_3);
                break;
            default:
                break;
        }
    }
    transmissionController.update(getDesiredGear());
    postState();
}


void initButtonShift(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
    engine->gearController = ButtonShiftController(PASS_ENGINE_PARAMETER_SIGNATURE);
}
