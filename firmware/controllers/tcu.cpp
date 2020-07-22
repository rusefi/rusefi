#include "tcu.h"
#include "tunerstudio.h"

GearControllerBase::GearControllerBase() {
    desiredGear = PARK;
}

void GearControllerBase::update() {
    transmissionController.update(desiredGear);
    postState(&tsOutputChannels)
}

gear_e GearControllerBase::getDesiredGear() {
    return desiredgear;
}

gear_e GearControllerBase::setDesiredGear(gear_e gear) {
    desiredGear = gear;
    return desiredGear;
}

void GearControllerBase::postState(TunerStudioOutputChannels *tsOutputChannels) {
    tsOutputChannels->desiredGear = desiredGear;
}

TransmissionControllerBase::TransmissionControllerBase() {
    currentGear = PARK;
}

void TransmissionControllerBase::update(gear_e gear) {
    currentGear = gear;
    postState(&tsOutputChannels);
}

gear_e TransmissionController::getCurrentGear() {
    return currentGear;
}

void TransmissionControllerBase::postState(TunerStudioOutputChannels *tsOutputChannels) {
    tsOutputChannels->currentGear = currentGear;
}
