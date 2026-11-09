#include "pch.h"

#include "math.h"
#include "gc_generic.h"

#if EFI_TCU
GenericGearController genericGearController;

GenericGearController::GenericGearController() {
}

void GenericGearController::init() {
#if EFI_PROD_CODE
	for (size_t i = 0; i < efi::size(engineConfiguration->tcu_rangeInput); i++) {
		if (isBrainPinValid(engineConfiguration->tcu_rangeInput[i])) {
			efiSetPadMode("Range Input", engineConfiguration->tcu_rangeInput[i], getInputMode(engineConfiguration->tcu_rangeInputMode[i]));
		}
	}
#endif /* EFI_PROD_CODE */

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
			// We allow the user to configure either a digital input or an analog input for each pin,
			//  so we need to check which is valid.
			if (isAdcChannelValid(engineConfiguration->tcu_rangeAnalogInput[p])) {
				float pinState = Sensor::getOrZero(getAnalogSensorType(p));
				if (getRangeStateArray(i)[p] == 0 || isNearest(pinState, p, rangeStates)) {
					// Set the gear to the one we're checking, and continue to the next pin
					gear = static_cast<SelectedGear>(i);
				} else {
					// This possibility doesn't match, set to invalid
					gear = SelectedGear::Invalid;
					break;
				}
			} else if (isBrainPinValid(engineConfiguration->tcu_rangeInput[p])) {
				bool pinState = efiReadPin(engineConfiguration->tcu_rangeInput[p]) ^ (engineConfiguration->tcu_rangeInputMode[p] == PI_PULLUP);
				// If the pin doesn't matter, or if it matches the cellState
				if (cellState == 2 || (pinState && cellState == 1) || (!pinState && cellState == 0)) {
					// Set the gear to the one we're checking, and continue to the next pin
					gear = static_cast<SelectedGear>(i);
				} else {
					// This possibility doesn't match, set to invalid
					gear = SelectedGear::Invalid;
					break;
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
			// Only allow manual shift once per 500 ms,
			// and if the selected range was Manual prior to this update
			if (!shiftTimer.hasElapsedMs(500) || lastRange != SelectedGear::Manual) {
				break;
			}
			shiftTimer.reset();
			switch (getDesiredGear()) {
			case GEAR_1 :
				if (engineConfiguration->totalGearsCount >= 2) setDesiredGear(GEAR_2);
				break;
			case GEAR_2 :
				if (engineConfiguration->totalGearsCount >= 3) setDesiredGear(GEAR_3);
				break;
			case GEAR_3 :
				if (engineConfiguration->totalGearsCount >= 4) setDesiredGear(GEAR_4);
				break;
			case GEAR_4 :
				if (engineConfiguration->transmissionControllerMode == TransmissionControllerMode::SimpleTransmissionController && engineConfiguration->totalGearsCount >= 5) setDesiredGear(GEAR_5);
				break;
			case GEAR_5 :
				if (engineConfiguration->transmissionControllerMode == TransmissionControllerMode::SimpleTransmissionController && engineConfiguration->totalGearsCount >= 6) setDesiredGear(GEAR_6);
				break;
			default:
				break;
			}
			break;
		case SelectedGear::ManualMinus :
			// Only allow manual shift once per 500 ms,
			// and if the selected range was Manual prior to this update
			if (!shiftTimer.hasElapsedMs(500) || lastRange != SelectedGear::Manual) {
				break;
			}
			shiftTimer.reset();
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
			case GEAR_5 :
				setDesiredGear(GEAR_4);
				break;
			case GEAR_6 :
				setDesiredGear(GEAR_5);
				break;
			default:
				break;
			}
			break;
		case SelectedGear::Manual:
			// Moving D -> M latches the current desired forward gear. In M,
			// automatic shift maps must not change the selected gear.
			break;
		case SelectedGear::Drive :
			// M -> D resumes automatic shifts from the current selected gear.
			// Record Drive BEFORE the early return: otherwise a stale Manual
			// state can incorrectly authorize a subsequent M+ / M- command.
			lastRange = SelectedGear::Drive;
			AutomaticGearController::update();
			return;
		default:
			break;
		}

		lastRange = gear;
	}

	GearControllerBase::update();
}

GenericGearController* getGenericGearController() {
	return &genericGearController;
}
#endif // EFI_TCU
