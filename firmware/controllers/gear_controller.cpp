#include "pch.h"

#include "gear_controller.h"

void GearControllerBase::init() {
	switch (engineConfiguration->transmissionControllerMode) {
	case TransmissionControllerMode::SimpleTransmissionController :
		transmissionController = getSimpleTransmissionController();
		break;
	case TransmissionControllerMode::Gm4l6x :
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
	switch (engineConfiguration->gearControllerMode) {
	case GearControllerMode::ButtonShift :
		engine->gearController = getButtonShiftController();
		break;
	default :
		return;
	}
	engine->gearController->init();
}
