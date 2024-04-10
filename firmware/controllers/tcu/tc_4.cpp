#include "pch.h"

#include "tc_4.h"

#if EFI_TCU
Generic4TransmissionController generic4TransmissionController;
static SimplePwm pcPwm("Pressure Control");

void Generic4TransmissionController::init() {
	SimpleTransmissionController::init();

	enginePins.tcuTccOnoffSolenoid.initPin("TCC On/Off Solenoid", engineConfiguration->tcu_tcc_onoff_solenoid, engineConfiguration->tcu_tcc_onoff_solenoid_mode);

	enginePins.tcuPcSolenoid.initPin("Pressure Control Solenoid", engineConfiguration->tcu_pc_solenoid_pin, engineConfiguration->tcu_pc_solenoid_pin_mode);
	startSimplePwm(&pcPwm,
								 "Line Pressure",
								 &engine->executor,
								 &enginePins.tcuPcSolenoid,
								 engineConfiguration->tcu_pc_solenoid_freq,
								 0);
}

void Generic4TransmissionController::update(gear_e gear) {
	if (gear != getCurrentGear()) {
		shiftingFrom = getCurrentGear();
		isShifting = true;
		measureShiftTime(gear);
	}

	setTccState(gear);
	setPcState(gear);

	setCurrentGear(gear);

	SimpleTransmissionController::update(gear);

	float time = isShiftCompleted();
	if (time != 0) {
		lastShiftTime = time;
		isShifting = false;
	}
}

void Generic4TransmissionController::setTccState(gear_e gear) {
	if (isShifting) {
		enginePins.tcuTccOnoffSolenoid.setValue(0);
		return;
	}

	auto tps = Sensor::get(SensorType::DriverThrottleIntent);
	auto vss = Sensor::get(SensorType::VehicleSpeed);
	if (!tps.Valid || !vss.Valid) {
		return;
	}
	if (gear == GEAR_4) {
		int lockSpeed = interpolate2d(tps.Value, config->tcu_tccTpsBins, config->tcu_tccLockSpeed);
		int unlockSpeed = interpolate2d(tps.Value, config->tcu_tccTpsBins, config->tcu_tccUnlockSpeed);
		if (vss.Value > lockSpeed) {
			torqueConverterDuty = 100;
			enginePins.tcuTccOnoffSolenoid.setValue(1);
		} else if (vss.Value < unlockSpeed) {
			torqueConverterDuty = 0;
			enginePins.tcuTccOnoffSolenoid.setValue(1);
		}
	} else {
		torqueConverterDuty = 0;
		enginePins.tcuTccOnoffSolenoid.setValue(0);
	}
}

void Generic4TransmissionController::setPcState(gear_e gear) {
	uint8_t (*pcts)[sizeof(config->tcu_pcAirmassBins)/sizeof(config->tcu_pcAirmassBins[0])];

	switch (gear) {
	case REVERSE:
		pcts = &config->tcu_pcValsR;
		break;
	case NEUTRAL:
		pcts = &config->tcu_pcValsN;
		break;
	case GEAR_1:
		if (isShifting && shiftingFrom == GEAR_2) {
			pcts = &config->tcu_pcVals21;
		} else {
			pcts = &config->tcu_pcVals1;
		}
		break;
	case GEAR_2:
		if (isShifting && shiftingFrom == GEAR_1) {
			pcts = &config->tcu_pcVals12;
		} else if (isShifting && shiftingFrom == GEAR_3) {
			pcts = &config->tcu_pcVals32;
		} else {
			pcts = &config->tcu_pcVals2;
		}
		break;
	case GEAR_3:
		if (isShifting && shiftingFrom == GEAR_2) {
			pcts = &config->tcu_pcVals23;
		} else if (isShifting && shiftingFrom == GEAR_4) {
			pcts = &config->tcu_pcVals43;
		} else {
			pcts = &config->tcu_pcVals3;
		}
		break;
	case GEAR_4:
		if (isShifting && shiftingFrom == GEAR_3) {
			pcts = &config->tcu_pcVals34;
		} else {
			pcts = &config->tcu_pcVals4;
		}
		break;
	default:
		break;
	}

	if (pcts) {
		pressureControlDuty = interpolate2d(engine->engineState.airflowEstimate, config->tcu_pcAirmassBins, *pcts);
		pcPwm.setSimplePwmDutyCycle(0.01f * pressureControlDuty);
	}
}

Generic4TransmissionController* getGeneric4TransmissionController() {
	return &generic4TransmissionController;
}
#endif // EFI_TCU
