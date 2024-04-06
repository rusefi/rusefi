#include "pch.h"

#include "math.h"
#include "gc_generic.h"

#if EFI_TCU
GenericGearController genericGearController;

GenericGearController::GenericGearController() {
}

void GenericGearController::init() {
	for (size_t i = 0; i < efi::size(engineConfiguration->tcu_rangeInput); i++) {
		if (isBrainPinValid(engineConfiguration->tcu_rangeInput[i])) {
			efiSetPadMode("Range Input", engineConfiguration->tcu_rangeInput[i], getInputMode(engineConfiguration->tcu_rangeInputMode[i]));
		}
	}

	GearControllerBase::init();
}

SensorType GenericGearController::getAnalogSensorType(int zeroBasedSensorIndex) {
	return static_cast<SensorType>(zeroBasedSensorIndex + static_cast<int>(SensorType::RangeInput1));
}

bool GenericGearController::isNearest(float value, int pinIndex, float* rangeStates) {
	float distance = fabs(rangeStates[pinIndex] - value);
	for (int i = 1; i <= TCU_RANGE_COUNT; i++) {
		float pinDistance = fabs(getRangeStateArray(i)[pinIndex] - value);
		if (pinDistance < distance) {
			return false;
		}
	}
	return true;
}

void GenericGearController::update() {
	SelectedGear gear = SelectedGear::Invalid;
	// Loop through possible range states
	// 1 based because 0 is SelectedGear::Invalid
	for (int i = 1; i <= TCU_RANGE_COUNT; i++) {
		float *rangeStates = getRangeStateArray(i);
		// Loop through inputs
		for (size_t p = 0; p < efi::size(engineConfiguration->tcu_rangeInput); p++) {
			float cellState = rangeStates[p];
			if (isAdcChannelValid(engineConfiguration->tcu_rangeAnalogInput[p])) {
				float pinState = Sensor::getOrZero(getAnalogSensorType(p));
				if (isNearest(pinState, p, rangeStates)) {
					// Set the gear to the one we're checking, and continue to the next pin
					gear = static_cast<SelectedGear>(i);
				} else {
					// This possibility doesn't match, set to invalid
					gear = SelectedGear::Invalid;
				}
			} else if (isBrainPinValid(engineConfiguration->tcu_rangeInput[p])) {
				// If we've locked out this range with 3 in a cell
				if (cellState == 3) {
					gear = SelectedGear::Invalid;
					break;
				}
				bool pinState = efiReadPin(engineConfiguration->tcu_rangeInput[p]);
				// If the pin doesn't matter, or if it matches the cellState
				if (cellState == 2 || (pinState && cellState == 1) || (!pinState && cellState == 0)) {
					// Set the gear to the one we're checking, and continue to the next pin
					gear = static_cast<SelectedGear>(i);
				} else {
					// This possibility doesn't match, set to invalid
					gear = SelectedGear::Invalid;
				}
			}
		}
		// If we didn't find it, try the next range
		if (gear == SelectedGear::Invalid) {
			continue;
		// We found a match
		} else {
			break;
		}
	}
	if (gear != SelectedGear::Invalid) {
		switch (gear) {
		case SelectedGear::Manual3 :
			setDesiredGear(GEAR_3);
			break;
		case SelectedGear::Manual2 :
			setDesiredGear(GEAR_2);
			break;
		case SelectedGear::Manual1 :
			setDesiredGear(GEAR_1);
			break;
		case SelectedGear::Reverse :
			setDesiredGear(REVERSE);
			break;
		case SelectedGear::Park :
		case SelectedGear::Neutral :
			setDesiredGear(NEUTRAL);
			break;
		case SelectedGear::ManualPlus :
			switch (getDesiredGear()) {
			case GEAR_1 :
				setDesiredGear(GEAR_2);
				break;
			case GEAR_2 :
				setDesiredGear(GEAR_3);
				break;
			case GEAR_3 :
				setDesiredGear(GEAR_4);
				break;
			default:
				break;
			}
			break;
		case SelectedGear::ManualMinus :
			switch (getDesiredGear()) {
			case GEAR_2 :
				setDesiredGear(GEAR_1);
				break;
			case GEAR_3 :
				setDesiredGear(GEAR_2);
				break;
			case GEAR_4 :
				setDesiredGear(GEAR_3);
				break;
			default:
				break;
			}
			break;
		case SelectedGear::Drive :
			// TODO
			//  for Drive, need shift tables etc.
			break;
		default:
			break;
		}
	}

	GearControllerBase::update();
}

GenericGearController* getGenericGearController() {
	return &genericGearController;
}
#endif // EFI_TCU
