#include "pch.h"

#include "gc_generic.h"

#if EFI_TCU
GenericGearController genericGearController;

GenericGearController::GenericGearController() {
}

void GenericGearController::init() {
	for (int i = 0; i < efi::size(engineConfiguration->tcu_rangeInput); i++) {
		if (isBrainPinValid(engineConfiguration->tcu_rangeInput[i])) {
			efiSetPadMode("Range Input", engineConfiguration->tcu_rangeInput[i], getInputMode(engineConfiguration->tcu_rangeInputMode[i]));
		}
	}

	GearControllerBase::init();
}

void GenericGearController::update() {
	SelectedGear gear = SelectedGear::Invalid;
	// 1-9 because 0 is SelectedGear::Invalid
	for (int i = 1; i <= 9; i++) {
		uint8_t *rangeStates = getRangeStateArray(i);
		for (int p = 0; p < efi::size(engineConfiguration->tcu_rangeInput); p++) {
			int cellState = rangeStates[p];
			if (!isBrainPinValid(engineConfiguration->tcu_rangeInput[p]) || cellState == 3) {
				gear = SelectedGear::Invalid;
				break;
			}
			bool pinState = efiReadPin(engineConfiguration->tcu_rangeInput[p]);
			if (cellState == 2 || (pinState && cellState == 1) || (!pinState && cellState == 0)) {
				gear = static_cast<SelectedGear>(i);
				continue;
			}
		}
		if (gear == SelectedGear::Invalid) {
			continue;
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
