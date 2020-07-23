#include "tcu.h"

void GearControllerBase::update() {
    transmissionController.update(desiredGear);
    postState(&tsOutputChannels);
}

gear_e GearControllerBase::getDesiredGear() {
    return desiredGear;
}

gear_e GearControllerBase::setDesiredGear(gear_e gear) {
    desiredGear = gear;
    return desiredGear;
}

void GearControllerBase::postState(TunerStudioOutputChannels *tsOutputChannels) {
    tsOutputChannels->desiredGear = desiredGear;
}

void TransmissionControllerBase::update(gear_e gear) {
    currentGear = gear;
    postState(&tsOutputChannels);
}

gear_e TransmissionControllerBase::getCurrentGear() {
    return currentGear;
}

void TransmissionControllerBase::postState(TunerStudioOutputChannels *tsOutputChannels) {
    tsOutputChannels->currentGear = currentGear;
}
