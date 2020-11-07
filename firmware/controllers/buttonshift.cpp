/**
 * @file        buttonshift.cpp
 * @brief       Polls pins for gear changes
 *
 * @date Aug 31, 2020
 * @author David Holdeman, (c) 2020
 */
#include "buttonshift.h"
#include "engine.h"

EXTERN_ENGINE;

ButtonShiftController buttonShiftController;


ButtonShiftController::ButtonShiftController() :
		debounceUp("up"),
		debounceDown("down")
		{

}

void ButtonShiftController::init (DECLARE_ENGINE_PARAMETER_SIGNATURE) {
    // 500 millisecond is maybe a little long?
    debounceUp.init(500, CONFIG(tcuUpshiftButtonPin), CONFIG(tcuUpshiftButtonPinMode));
    debounceDown.init(500, CONFIG(tcuDownshiftButtonPin), CONFIG(tcuDownshiftButtonPinMode));
    INJECT_ENGINE_REFERENCE(&transmissionController);
    transmissionController.init();
}

void ButtonShiftController::update() {
    bool upPinState = false;
    bool downPinState = false;
    // Read pins
    upPinState = debounceUp.readPinEvent();
    downPinState = debounceDown.readPinEvent();
    gear_e gear = getDesiredGear();
    // Select new gear based on current desired gear.
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
    // We are responsible for telling the transmission controller
    //  what gear we want.
    transmissionController.update(getDesiredGear());
    // Post state to TS
    postState();
}


void initButtonShift(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
    buttonShiftController.init(PASS_ENGINE_PARAMETER_SIGNATURE);
    engine->gearController = &buttonShiftController;
    INJECT_ENGINE_REFERENCE(&buttonShiftController);
}
