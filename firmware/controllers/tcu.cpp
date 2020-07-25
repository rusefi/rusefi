#include "tcu.h"
#include "tunerstudio_outputs.h"

void GearControllerBase::update() {
    transmissionController.update(desiredGear);
    postState();
}

gear_e GearControllerBase::getDesiredGear() {
    return desiredGear;
}

gear_e GearControllerBase::setDesiredGear(gear_e gear) {
    desiredGear = gear;
    return desiredGear;
}

void GearControllerBase::postState() {
#if EFI_TUNER_STUDIO
    tsOutputChannels.desiredGear = desiredGear;
#endif
}

void TransmissionControllerBase::update(gear_e gear) {
    currentGear = gear;
    postState();
}

gear_e TransmissionControllerBase::getCurrentGear() {
    return currentGear;
}

void TransmissionControllerBase::postState() {
#if EFI_TUNER_STUDIO
    tsOutputChannels.currentGear = currentGear;
#endif
}
