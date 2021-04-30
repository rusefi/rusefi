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

void ButtonShiftController::init(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	GearControllerBase::init(PASS_ENGINE_PARAMETER_SIGNATURE);
}

void ButtonShiftController::update() {
    int role = getInputRole();
    gear_e gear = getDesiredGear();
    // Select new gear based on current desired gear.
    if (role == ROLE_UPSHIFT) {
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
    } else if (role == ROLE_DOWNSHIFT) {
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

	GearControllerBase::update();
}


void initButtonShift(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
    buttonShiftController.init(PASS_ENGINE_PARAMETER_SIGNATURE);
    engine->gearController = &buttonShiftController;
    INJECT_ENGINE_REFERENCE(&buttonShiftController);
}
