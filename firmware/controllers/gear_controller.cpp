#include "pch.h"

#include "gear_controller.h"

void GearControllerBase::init(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
    transmissionController.inject(PASS_ENGINE_PARAMETER_SIGNATURE);
    transmissionController.init();
}

void GearControllerBase::update() {
    // We are responsible for telling the transmission controller
    //  what gear we want.
    transmissionController.update(getDesiredGear());
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
    tsOutputChannels.tcuDesiredGear = getDesiredGear();
#endif
}
