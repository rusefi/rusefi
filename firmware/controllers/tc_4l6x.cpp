#include "pch.h"

#include "tc_4l6x.h"

Gm4l6xTransmissionController gm4l6xTransmissionController;

void Gm4l6xTransmissionController::init() {
    for (size_t i = 0; i < efi::size(engineConfiguration->tcu_solenoid); i++) {
        enginePins.tcuSolenoids[i].initPin("Transmission Solenoid", engineConfiguration->tcu_solenoid[i], &engineConfiguration->tcu_solenoid_mode[i]);
    }
		enginePins.tcuTccOnoffSolenoid.initPin("TCC On/Off Solenoid", engineConfiguration->tcu_tcc_onoff_solenoid, &engineConfiguration->tcu_tcc_onoff_solenoid_mode);
		enginePins.tcuTccPwmSolenoid.initPin("TCC PWM Solenoid", engineConfiguration->tcu_tcc_pwm_solenoid, &engineConfiguration->tcu_tcc_pwm_solenoid_mode);
}

void Gm4l6xTransmissionController::update(gear_e gear) {
	setCurrentGear(gear);
	setTccState();
	postState();

#if EFI_TUNER_STUDIO
	if (engineConfiguration->debugMode == DBG_TCU) {
		engine->outputChannels.debugIntField1 = config->tcuSolenoidTable[static_cast<int>(gear) + 1][0];
		engine->outputChannels.debugIntField2 = config->tcuSolenoidTable[static_cast<int>(gear) + 1][1];
		engine->outputChannels.debugIntField3 = config->tcuSolenoidTable[static_cast<int>(gear) + 1][2];
		engine->outputChannels.debugIntField4 = config->tcuSolenoidTable[static_cast<int>(gear) + 1][3];
		engine->outputChannels.debugIntField5 = config->tcuSolenoidTable[static_cast<int>(gear) + 1][4];
	}
#endif
}

gear_e Gm4l6xTransmissionController::setCurrentGear(gear_e gear) {
	if (gear == getCurrentGear()) {
		return getCurrentGear();
	}
	currentGear = gear;
	enginePins.tcuTccOnoffSolenoid.setValue(0);
	for (size_t i = 0; i < efi::size(engineConfiguration->tcu_solenoid); i++) {
#if ! EFI_UNIT_TEST
		enginePins.tcuSolenoids[i].setValue(config->tcuSolenoidTable[static_cast<int>(gear) + 1][i]);
#endif
	}
	return getCurrentGear();
}

void Gm4l6xTransmissionController::setTccState() {
	auto tps = Sensor::get(SensorType::DriverThrottleIntent);
	auto vss = Sensor::get(SensorType::VehicleSpeed);
	if (!tps.Valid || !vss.Valid) {
		return;
	}
	if (getCurrentGear() == GEAR_4) {
		int lockSpeed = interpolate2d(tps.Value, engineConfiguration->tcu_tccTpsBins, engineConfiguration->tcu_tccLockSpeed);
		int unlockSpeed = interpolate2d(tps.Value, engineConfiguration->tcu_tccTpsBins, engineConfiguration->tcu_tccUnlockSpeed);
		if (vss.Value > lockSpeed) {
			enginePins.tcuTccOnoffSolenoid.setValue(1);
		} else if (vss.Value < lockSpeed) {
			enginePins.tcuTccOnoffSolenoid.setValue(1);
		}
	} else {
		enginePins.tcuTccOnoffSolenoid.setValue(0);
	}
}

Gm4l6xTransmissionController* getGm4l6xTransmissionController() {
	return &gm4l6xTransmissionController;
}
