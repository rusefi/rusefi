#include "pch.h"

#include "gear_controller.h"

void GearControllerBase::init() {
<<<<<<< HEAD
	initTransmissionController();
}

void GearControllerBase::initTransmissionController() {
	switch (engineConfiguration->transmissionControllerMode) {
=======
	switch (engineConfiguration->transmissionController) {
>>>>>>> de8849fd38 (try enum class)
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
	} else if (transmissionController->mode != engineConfiguration->transmissionControllerMode) {
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
