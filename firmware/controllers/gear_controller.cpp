#include "pch.h"

#include "gear_controller.h"

void GearControllerBase::init() {
	switch (engineConfiguration->transmissionController) {
	case TC_SIMPLE_TRANSMISSION_CONTROLLER :
		transmissionController = getSimpleTransmissionController();
		break;
	case TC_GM_4L6X :
		transmissionController = getGm4l6xTransmissionController();
		break;
	default :
		return;
	}
	transmissionController->init();
}

void GearControllerBase::update() {
    // We are responsible for telling the transmission controller
    //  what gear we want.
    transmissionController->update(getDesiredGear());
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

void GearControllerBase::postState() {
#if EFI_TUNER_STUDIO
    engine->outputChannels.tcuDesiredGear = getDesiredGear();
#endif
}

void initGearController() {
	switch (engineConfiguration->gearController) {
	case GC_BUTTON_SHIFT :
		engine->gearController = getButtonShiftController();
		break;
	default :
		return;
	}
	engine->gearController->init();
}
