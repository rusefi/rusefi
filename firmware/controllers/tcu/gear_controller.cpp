#include "pch.h"

#include "gear_controller.h"

#if EFI_TCU
void GearControllerBase::init() {
	initTransmissionController();
}

void GearControllerBase::initTransmissionController() {
	switch (engineConfiguration->transmissionControllerMode) {
	case TransmissionControllerMode::SimpleTransmissionController :
		transmissionController = getSimpleTransmissionController();
		break;
	case TransmissionControllerMode::Gm4l6x :
		transmissionController = getGm4l6xTransmissionController();
		break;
	default :
		transmissionController = NULL;
		return;
	}
	transmissionController->init();
}

void GearControllerBase::update() {
	if (transmissionController == NULL) {
		initTransmissionController();
	} else if (transmissionController->getMode() != engineConfiguration->transmissionControllerMode) {
		initTransmissionController();
	}
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
		engine->gearController = NULL;
		return;
	}
	engine->gearController->init();
}
#endif // EFI_TCU
