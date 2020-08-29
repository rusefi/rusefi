#include "buttonshift.h"
#include "engine.h"

EXTERN_ENGINE;

ButtonShiftController buttonShiftController();

ButtonShiftController::ButtonShiftController () {
    ButtonDebounce u = ButtonDebounce(10, CONFIG(tcuUpshiftButtonPin), CONFIG(tcuUpshiftButtonPinMode));
    debounceUp = &u;
    ButtonDebounce d = ButtonDebounce(10, CONFIG(tcuDownshiftButtonPin), CONFIG(tcuDownshiftButtonPinMode));
    debounceDown = &d;
}

void ButtonShiftController::update() {
    bool upPinState = debounceUp->readPinEvent();
    bool downPinState = debounceDown->readPinEvent();
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
setDesiredGear(GEAR_1);
    transmissionController.update(getDesiredGear());
    postState();
}


void initButtonShift() {
    engine->gearController = &buttonShiftController;
}
