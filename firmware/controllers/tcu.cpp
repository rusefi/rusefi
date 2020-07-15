include "tcu.h"

TransmissionControllerBase::TransmissionControllerBase () {
	desiredGear = NEUTRAL;
	currentGear = NEUTRAL:
}

void TransmissionControllerBase::update() {
	auto shiftRequest = GetShiftRequest();
	desiredGear = shiftRequest;
}

gear_e TransmissionControllerBase::GetShiftRequest() {
	// TODO check pins and if a button is pressed, return updated desiredGear
	//      and update currentGear
}
