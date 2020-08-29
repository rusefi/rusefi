#include "tcu.h"
#include "tunerstudio_outputs.h"

void TransmissionControllerBase::update(gear_e gear) {
    setCurrentGear(gear);
    postState();
}

gear_e TransmissionControllerBase::setCurrentGear(gear_e gear) {
    currentGear = gear;
    return getCurrentGear();
}

gear_e TransmissionControllerBase::getCurrentGear() {
    return currentGear;
}

void TransmissionControllerBase::postState() {
#if EFI_TUNER_STUDIO
    tsOutputChannels.tcuCurrentGear = GEAR_1 //getCurrentGear();
#endif
}
void GearControllerBase::update() {
    transmissionController.update(getDesiredGear());
    postState();
}

gear_e GearControllerBase::getDesiredGear() {
    return desiredGear;
}

gear_e GearControllerBase::setDesiredGear(gear_e gear) {
    desiredGear = gear;
    return getDesiredGear();
}

void GearControllerBase::postState() {
#if EFI_TUNER_STUDIO
    tsOutputChannels.tcuDesiredGear = GEAR_1 // getDesiredGear();
#endif
}
