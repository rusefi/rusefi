/**
 * @file        buttonshift.cpp
 * @brief       Polls pins for gear changes
 *
 * @date Aug 31, 2020
 * @author David Holdeman, (c) 2020
 */

#include "pch.h"

#include "buttonshift.h"

#if EFI_TCU
ButtonShiftController buttonShiftController;

ButtonShiftController::ButtonShiftController() :
		debounceUp("gear_up"),
		debounceDown("gear_down")
		{

}

void ButtonShiftController::init() {
	// 500 millisecond is maybe a little long?
	debounceUp.init(500, engineConfiguration->tcuUpshiftButtonPin, engineConfiguration->tcuUpshiftButtonPinMode);
	debounceDown.init(500, engineConfiguration->tcuDownshiftButtonPin, engineConfiguration->tcuDownshiftButtonPinMode);

	GearControllerBase::init();
}

void ButtonShiftController::update() {
    bool upPinState = false;
    bool downPinState = false;
    // Read pins
    upPinState = debounceUp.readPinEvent();
    downPinState = debounceDown.readPinEvent();
    gear_e gear = getDesiredGear();

    // Only Simple Transmission supports six forward gears.
    // Generic 4-speed and GM 4L6X must never request gears above fourth.
    const int configuredGears = engineConfiguration->totalGearsCount;
    const int maxGears = engineConfiguration->transmissionControllerMode == TransmissionControllerMode::SimpleTransmissionController
        ? (configuredGears < 6 ? configuredGears : 6)
        : (configuredGears < 4 ? configuredGears : 4);

    // Select new gear based on current desired gear.
    if (upPinState) {
        switch (gear) {
            case REVERSE:
                setDesiredGear(NEUTRAL);
                break;
            case NEUTRAL:
                if (maxGears >= 1) setDesiredGear(GEAR_1);
                break;
            case GEAR_1:
                if (maxGears >= 2) setDesiredGear(GEAR_2);
                break;
            case GEAR_2:
                if (maxGears >= 3) setDesiredGear(GEAR_3);
                break;
            case GEAR_3:
                if (maxGears >= 4) setDesiredGear(GEAR_4);
                break;
            case GEAR_4:
                if (maxGears >= 5) setDesiredGear(GEAR_5);
                break;
            case GEAR_5:
                if (maxGears >= 6) setDesiredGear(GEAR_6);
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
            case GEAR_5:
                setDesiredGear(GEAR_4);
                break;
            case GEAR_6:
                setDesiredGear(GEAR_5);
                break;
            default:
                break;
        }
    }

	GearControllerBase::update();
}

ButtonShiftController* getButtonShiftController() {
	return &buttonShiftController;
}
#endif // EFI_TCU
