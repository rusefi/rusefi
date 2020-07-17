include "tcu.h"

void GearControllerBase::update() {
    transmissionController(desiredGear);
}

gear_e GearControllerBase::getDesiredGear() {
    return desiredgear;
}

gear_e GearControllerBase::setDesiredGear(gear_e gear) {
    desiredGear = gear;
    return desiredGear;
}

void TransmissionControllerBase::update(gear_e gear) {
    currentGear = gear;
}
