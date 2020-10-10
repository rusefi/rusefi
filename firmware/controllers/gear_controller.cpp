#include "gear_controller.h"
#include "tunerstudio_outputs.h"
#include "engine_configuration.h"

EXTERN_CONFIG;

void GearControllerBase::init() {
    INJECT_ENGINE_REFERENCE(&transmissionController);
    transmissionController.init();
}

void GearControllerBase::update() {
    transmissionController.update(getDesiredGear());
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
