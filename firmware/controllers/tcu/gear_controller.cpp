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
		// TODO de-init here
		initTransmissionController();
	}

	// check if it was init'd (it wouldn't be if set to NONE)
	if (transmissionController != NULL) {
		// We are responsible for telling the transmission controller
		//  what gear we want.
		transmissionController->update(getDesiredGear());
	}

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
	case GearControllerMode::Generic :
		engine->gearController = getGenericGearController();
		break;
	default :
		engine->gearController = NULL;
		return;
	}
	engine->gearController->init();
}

uint8_t* GearControllerBase::getRangeStateArray(int i) {
	switch (i) {
	case 1 :
		return config->tcu_rangePlus;
		break;
	case 2 :
		return config->tcu_rangeMinus;
		break;
	case 3 :
		return config->tcu_rangeP;
		break;
	case 4 :
		return config->tcu_rangeR;
		break;
	case 5 :
		return config->tcu_rangeN;
		break;
	case 6 :
		return config->tcu_rangeD;
		break;
	case 7 :
		return config->tcu_rangeM;
		break;
	case 8 :
		return config->tcu_rangeM3;
		break;
	case 9 :
		return config->tcu_rangeM2;
		break;
	case 10 :
		return config->tcu_rangeM1;
		break;
	case 11 :
		return config->tcu_rangeLow;
		break;
	default:
		return NULL;
	}
}
#endif // EFI_TCU
