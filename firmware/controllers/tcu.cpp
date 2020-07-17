include "tcu.h"

GearControllerBase::GearControllerBase() {
    desiredGear = PARK;
}

void GearControllerBase::update() {
    transmissionController.update(desiredGear);
}

gear_e GearControllerBase::getDesiredGear() {
    return desiredgear;
}

gear_e GearControllerBase::setDesiredGear(gear_e gear) {
    desiredGear = gear;
    return desiredGear;
}

TransmissionControllerBase::TransmissionControllerBase() {
    currentGear = PARK;
}

void TransmissionControllerBase::update(gear_e gear) {
    currentGear = gear;
}
