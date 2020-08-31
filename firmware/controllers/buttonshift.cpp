#include "buttonshift.h"
#include "engine.h"

EXTERN_ENGINE;

ButtonShiftController buttonShiftController;

void ButtonShiftController::init (DECLARE_ENGINE_PARAMETER_SIGNATURE) {
    ButtonDebounce d = ButtonDebounce(500, CONFIG(tcuDownshiftButtonPin), CONFIG(tcuDownshiftButtonPinMode));
    debounceDown = &d;
    ButtonDebounce u = ButtonDebounce(500, CONFIG(tcuUpshiftButtonPin), CONFIG(tcuUpshiftButtonPinMode));
    debounceUp = &u;
}

void ButtonShiftController::update() {
    bool upPinState = false;
    bool downPinState = false;
    if (CONFIG(tcuUpshiftButtonPin) && CONFIG(tcuEnabled)) {
        upPinState = debounceUp->readPinEvent();
    }
    if (CONFIG(tcuDownshiftButtonPin) && CONFIG(tcuEnabled)) {
        downPinState = debounceDown->readPinEvent();
    }
    gear_e gear = getDesiredGear();
    if (upPinState) {
        switch (gear) {
            case REVERSE:
                setDesiredGear(NEUTRAL);
                break;
            case NEUTRAL:
                setDesiredGear(GEAR_1);
                break;
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
            case NEUTRAL:
                setDesiredGear(REVERSE);
                break;
            case GEAR_1:
                setDesiredGear(NEUTRAL);
                break;
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
    buttonShiftController.init(PASS_ENGINE_PARAMETER_SIGNATURE);
    engine->gearController = &buttonShiftController;
}
