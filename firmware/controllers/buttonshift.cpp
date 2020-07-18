#include "buttonshift.h"
#include "debounce.h"

ButtonShiftController::ButtonShiftController () {
    buttonDebounce = new ButtonDebounce(10);
}

ButtonShiftController::update() {
    bool upPinState = buttonDebounce.readPin(CONFIG(tcuUpshiftButtonPin))
    bool downPinState = buttonDebounce.readPin(CONFIG(tcuDownshiftButtonPin))
    gear_e gear = getDesiredGear()
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
        }
    }
    else if (downPinState) {
            case GEAR_2:
                setDesiredGear(GEAR_1);
                break;
            case GEAR_3:
                setDesiredGear(GEAR_2);
                break;
            case GEAR_4:
                setDesiredGear(GEAR_3);
                break;        
    }
}
