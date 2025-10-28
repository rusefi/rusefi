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
								 &engine->scheduler,
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

	// set torque converter and pressure control state
	setTccState(gear);
	setPcState(gear);

	setCurrentGear(gear);

	SimpleTransmissionController::update(gear);

	float time = isShiftCompleted();
	// 0 means shift is not completed
	if (time != 0) {
		lastShiftTime = time;
		isShifting = false;
	}
}

void Generic4TransmissionController::setTccState(gear_e gear) {
	// disable if shifting
	if (isShifting) {
		enginePins.tcuTccOnoffSolenoid.setValue(0);
		return;
	}

	auto tps = Sensor::get(SensorType::DriverThrottleIntent);
	auto vss = Sensor::get(SensorType::VehicleSpeed);
	if (!tps.Valid || !vss.Valid) {
		return;
	}
	// only enable TC in gear 4
	if (gear == GEAR_4) {
		int lockSpeed = interpolate2d(tps.Value, config->tcu_tccTpsBins, config->tcu_tccLockSpeed);
		int unlockSpeed = interpolate2d(tps.Value, config->tcu_tccTpsBins, config->tcu_tccUnlockSpeed);
		if (vss.Value > lockSpeed) {
			// torqueConverterDuty is only used for a gauge
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
	uint8_t (*pcts)[TCU_TABLE_WIDTH];

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

// here we have default 4R70W calibration
void configureTcu4R70W() {
	// TCU Software Config
	engineConfiguration->tcuEnabled = true;
	engineConfiguration->gearControllerMode = GearControllerMode::Generic;
	engineConfiguration->transmissionControllerMode = TransmissionControllerMode::Generic4;

	// TCU Inputs
	// Buttonshift
	engineConfiguration->tcuUpshiftButtonPin = Gpio::E14;
	engineConfiguration->tcuUpshiftButtonPinMode = PI_PULLUP;
	engineConfiguration->tcuDownshiftButtonPin = Gpio::E13;
	engineConfiguration->tcuDownshiftButtonPinMode = PI_PULLUP;
	// Analog range sensor - used in early 4R70
	engineConfiguration->tcu_rangeSensorBiasResistor = 1753;
	engineConfiguration->tcu_rangeAnalogInput[0] = EFI_ADC_4;
	// Two digital for +/- add-on
	engineConfiguration->tcu_rangeInput[1] = Gpio::C6;
	engineConfiguration->tcu_rangeInputMode[1] = PI_PULLUP;
	engineConfiguration->tcu_rangeInput[2] = Gpio::E15;
	engineConfiguration->tcu_rangeInputMode[2] = PI_PULLUP;
  // Range voltages/digital states
	config->tcu_rangeR[0] = 6072.0;
	config->tcu_rangeR[1] = 0.0;
	config->tcu_rangeR[2] = 0.0;
	config->tcu_rangeP[0] = 18500.0;
	config->tcu_rangeP[1] = 0.0;
	config->tcu_rangeP[2] = 0.0;
	config->tcu_rangeN[0] = 3028.0;
	config->tcu_rangeN[1] = 0.0;
	config->tcu_rangeN[2] = 0.0;
	config->tcu_rangeD[0] = 1645.0;
	config->tcu_rangeD[1] = 0.0;
	config->tcu_rangeD[2] = 0.0;
	config->tcu_rangeM2[0] = 864.0;
	config->tcu_rangeM2[1] = 0.0;
	config->tcu_rangeM2[2] = 0.0;
	config->tcu_rangeM1[0] = 358.0;
	config->tcu_rangeM1[1] = 0.0;
	config->tcu_rangeM1[2] = 0.0;
	config->tcu_rangePlus[0] = 0.0;
	config->tcu_rangePlus[1] = 1.0;
	config->tcu_rangePlus[2] = 0.0;
	config->tcu_rangeMinus[0] = 0.0;
	config->tcu_rangeMinus[1] = 0.0;
	config->tcu_rangeMinus[2] = 1.0;
	// Disable these states
	config->tcu_rangeM[1] = 3.0;
	config->tcu_rangeM3[1] = 3.0;
	config->tcu_rangeLow[1] = 3.0;

	// TCU Outputs
	engineConfiguration->tcu_tcc_onoff_solenoid = Gpio::B8;
	engineConfiguration->tcu_tcc_onoff_solenoid_mode = OM_DEFAULT;
	engineConfiguration->tcu_pc_solenoid_pin = Gpio::E1;
	engineConfiguration->tcu_pc_solenoid_pin_mode = OM_DEFAULT;
	engineConfiguration->tcu_pc_solenoid_freq = 240;
	engineConfiguration->tcu_solenoid[0] = Gpio::B6;
	engineConfiguration->tcu_solenoid_mode[0] = OM_DEFAULT;
	engineConfiguration->tcu_solenoid[1] = Gpio::B5;
	engineConfiguration->tcu_solenoid_mode[1] = OM_DEFAULT;
	// Reverse
	config->tcuSolenoidTable[0][0] = 1;
	config->tcuSolenoidTable[1][0] = 0;
	// Neutral
	config->tcuSolenoidTable[0][1] = 1;
	config->tcuSolenoidTable[1][1] = 0;
	// 1
	config->tcuSolenoidTable[0][2] = 1;
	config->tcuSolenoidTable[1][2] = 0;
	// 2
	config->tcuSolenoidTable[0][3] = 0;
	config->tcuSolenoidTable[1][3] = 0;
	// 3
	config->tcuSolenoidTable[0][4] = 0;
	config->tcuSolenoidTable[1][4] = 1;
	// 4
	config->tcuSolenoidTable[0][5] = 1;
	config->tcuSolenoidTable[1][5] = 1;

	// Pressure Control
	config->tcu_pcAirmassBins[0] = 50.0;
	config->tcu_pcAirmassBins[1] = 110.0;
	config->tcu_pcAirmassBins[2] = 220.0;
	config->tcu_pcAirmassBins[3] = 350.0;
	config->tcu_pcAirmassBins[4] = 500.0;
	config->tcu_pcAirmassBins[5] = 750.0;
	config->tcu_pcAirmassBins[6] = 900.0;
	config->tcu_pcAirmassBins[7] = 1000.0;
	config->tcu_pcValsR[0] = 40.0;
	config->tcu_pcValsR[1] = 35.0;
	config->tcu_pcValsR[2] = 30.0;
	config->tcu_pcValsR[3] = 30.0;
	config->tcu_pcValsR[4] = 30.0;
	config->tcu_pcValsR[5] = 25.0;
	config->tcu_pcValsR[6] = 10.0;
	config->tcu_pcValsR[7] = 10.0;
	config->tcu_pcValsN[0] = 40.0;
	config->tcu_pcValsN[1] = 35.0;
	config->tcu_pcValsN[2] = 30.0;
	config->tcu_pcValsN[3] = 30.0;
	config->tcu_pcValsN[4] = 30.0;
	config->tcu_pcValsN[5] = 25.0;
	config->tcu_pcValsN[6] = 10.0;
	config->tcu_pcValsN[7] = 10.0;
	config->tcu_pcVals1[0] = 40.0;
	config->tcu_pcVals1[1] = 35.0;
	config->tcu_pcVals1[2] = 30.0;
	config->tcu_pcVals1[3] = 30.0;
	config->tcu_pcVals1[4] = 30.0;
	config->tcu_pcVals1[5] = 25.0;
	config->tcu_pcVals1[6] = 10.0;
	config->tcu_pcVals1[7] = 10.0;
	config->tcu_pcVals2[0] = 40.0;
	config->tcu_pcVals2[1] = 35.0;
	config->tcu_pcVals2[2] = 30.0;
	config->tcu_pcVals2[3] = 30.0;
	config->tcu_pcVals2[4] = 30.0;
	config->tcu_pcVals2[5] = 25.0;
	config->tcu_pcVals2[6] = 10.0;
	config->tcu_pcVals2[7] = 10.0;
	config->tcu_pcVals3[0] = 40.0;
	config->tcu_pcVals3[1] = 35.0;
	config->tcu_pcVals3[2] = 30.0;
	config->tcu_pcVals3[3] = 30.0;
	config->tcu_pcVals3[4] = 30.0;
	config->tcu_pcVals3[5] = 25.0;
	config->tcu_pcVals3[6] = 10.0;
	config->tcu_pcVals3[7] = 10.0;
	config->tcu_pcVals4[0] = 40.0;
	config->tcu_pcVals4[1] = 35.0;
	config->tcu_pcVals4[2] = 30.0;
	config->tcu_pcVals4[3] = 30.0;
	config->tcu_pcVals4[4] = 30.0;
	config->tcu_pcVals4[5] = 25.0;
	config->tcu_pcVals4[6] = 10.0;
	config->tcu_pcVals4[7] = 10.0;
	config->tcu_pcVals12[0] = 80.0;
	config->tcu_pcVals12[1] = 60.0;
	config->tcu_pcVals12[2] = 50.0;
	config->tcu_pcVals12[3] = 44.0;
	config->tcu_pcVals12[4] = 40.0;
	config->tcu_pcVals12[5] = 20.0;
	config->tcu_pcVals12[6] = 10.0;
	config->tcu_pcVals12[7] = 10.0;
	config->tcu_pcVals23[0] = 45.0;
	config->tcu_pcVals23[1] = 40.0;
	config->tcu_pcVals23[2] = 35.0;
	config->tcu_pcVals23[3] = 30.0;
	config->tcu_pcVals23[4] = 35.0;
	config->tcu_pcVals23[5] = 15.0;
	config->tcu_pcVals23[6] = 10.0;
	config->tcu_pcVals23[7] = 10.0;
	config->tcu_pcVals34[0] = 35.0;
	config->tcu_pcVals34[1] = 32.0;
	config->tcu_pcVals34[2] = 30.0;
	config->tcu_pcVals34[3] = 38.0;
	config->tcu_pcVals34[4] = 25.0;
	config->tcu_pcVals34[5] = 15.0;
	config->tcu_pcVals34[6] = 10.0;
	config->tcu_pcVals34[7] = 10.0;
	config->tcu_pcVals21[0] = 60.0;
	config->tcu_pcVals21[1] = 55.0;
	config->tcu_pcVals21[2] = 50.0;
	config->tcu_pcVals21[3] = 45.0;
	config->tcu_pcVals21[4] = 40.0;
	config->tcu_pcVals21[5] = 35.0;
	config->tcu_pcVals21[6] = 30.0;
	config->tcu_pcVals21[7] = 30.0;
	config->tcu_pcVals32[0] = 60.0;
	config->tcu_pcVals32[1] = 55.0;
	config->tcu_pcVals32[2] = 50.0;
	config->tcu_pcVals32[3] = 45.0;
	config->tcu_pcVals32[4] = 40.0;
	config->tcu_pcVals32[5] = 35.0;
	config->tcu_pcVals32[6] = 30.0;
	config->tcu_pcVals32[7] = 30.0;
	config->tcu_pcVals43[0] = 60.0;
	config->tcu_pcVals43[1] = 55.0;
	config->tcu_pcVals43[2] = 50.0;
	config->tcu_pcVals43[3] = 45.0;
	config->tcu_pcVals43[4] = 40.0;
	config->tcu_pcVals43[5] = 35.0;
	config->tcu_pcVals43[6] = 30.0;
	config->tcu_pcVals43[7] = 30.0;

	// TCC Control
	config->tcu_tccTpsBins[0] = 11.0;
	config->tcu_tccTpsBins[1] = 22.0;
	config->tcu_tccTpsBins[2] = 33.0;
	config->tcu_tccTpsBins[3] = 44.0;
	config->tcu_tccTpsBins[4] = 55.0;
	config->tcu_tccTpsBins[5] = 66.0;
	config->tcu_tccTpsBins[6] = 77.0;
	config->tcu_tccTpsBins[7] = 88.0;
	config->tcu_tccLockSpeed[0] = 40.0;
	config->tcu_tccLockSpeed[1] = 45.0;
	config->tcu_tccLockSpeed[2] = 52.0;
	config->tcu_tccLockSpeed[3] = 60.0;
	config->tcu_tccLockSpeed[4] = 70.0;
	config->tcu_tccLockSpeed[5] = 83.0;
	config->tcu_tccLockSpeed[6] = 97.0;
	config->tcu_tccLockSpeed[7] = 115.0;
	config->tcu_tccUnlockSpeed[0] = 30.0;
	config->tcu_tccUnlockSpeed[1] = 35.0;
	config->tcu_tccUnlockSpeed[2] = 41.0;
	config->tcu_tccUnlockSpeed[3] = 49.0;
	config->tcu_tccUnlockSpeed[4] = 53.0;
	config->tcu_tccUnlockSpeed[5] = 67.0;
	config->tcu_tccUnlockSpeed[6] = 78.0;
	config->tcu_tccUnlockSpeed[7] = 93.0;

	// Shift Config
	config->tcu_shiftTime = 600.0;
	config->tcu_shiftTpsBins[0] = 11.0;
	config->tcu_shiftTpsBins[1] = 22.0;
	config->tcu_shiftTpsBins[2] = 33.0;
	config->tcu_shiftTpsBins[3] = 44.0;
	config->tcu_shiftTpsBins[4] = 55.0;
	config->tcu_shiftTpsBins[5] = 67.0;
	config->tcu_shiftTpsBins[6] = 76.0;
	config->tcu_shiftTpsBins[7] = 88.0;
	config->tcu_shiftSpeed12[0] = 10.0;
	config->tcu_shiftSpeed12[1] = 12.0;
	config->tcu_shiftSpeed12[2] = 16.0;
	config->tcu_shiftSpeed12[3] = 23.0;
	config->tcu_shiftSpeed12[4] = 28.0;
	config->tcu_shiftSpeed12[5] = 32.0;
	config->tcu_shiftSpeed12[6] = 38.0;
	config->tcu_shiftSpeed12[7] = 42.0;
	config->tcu_shiftSpeed23[0] = 20.0;
	config->tcu_shiftSpeed23[1] = 25.0;
	config->tcu_shiftSpeed23[2] = 31.0;
	config->tcu_shiftSpeed23[3] = 37.0;
	config->tcu_shiftSpeed23[4] = 45.0;
	config->tcu_shiftSpeed23[5] = 55.0;
	config->tcu_shiftSpeed23[6] = 63.0;
	config->tcu_shiftSpeed23[7] = 72.0;
	config->tcu_shiftSpeed34[0] = 35.0;
	config->tcu_shiftSpeed34[1] = 40.0;
	config->tcu_shiftSpeed34[2] = 47.0;
	config->tcu_shiftSpeed34[3] = 55.0;
	config->tcu_shiftSpeed34[4] = 65.0;
	config->tcu_shiftSpeed34[5] = 78.0;
	config->tcu_shiftSpeed34[6] = 92.0;
	config->tcu_shiftSpeed34[7] = 110.0;
	config->tcu_shiftSpeed21[0] = 5.0;
	config->tcu_shiftSpeed21[1] = 7.0;
	config->tcu_shiftSpeed21[2] = 11.0;
	config->tcu_shiftSpeed21[3] = 17.0;
	config->tcu_shiftSpeed21[4] = 20.0;
	config->tcu_shiftSpeed21[5] = 23.0;
	config->tcu_shiftSpeed21[6] = 26.0;
	config->tcu_shiftSpeed21[7] = 32.0;
	config->tcu_shiftSpeed32[0] = 10.0;
	config->tcu_shiftSpeed32[1] = 13.0;
	config->tcu_shiftSpeed32[2] = 21.0;
	config->tcu_shiftSpeed32[3] = 30.0;
	config->tcu_shiftSpeed32[4] = 35.0;
	config->tcu_shiftSpeed32[5] = 45.0;
	config->tcu_shiftSpeed32[6] = 50.0;
	config->tcu_shiftSpeed32[7] = 55.0;
	config->tcu_shiftSpeed43[0] = 25.0;
	config->tcu_shiftSpeed43[1] = 30.0;
	config->tcu_shiftSpeed43[2] = 36.0;
	config->tcu_shiftSpeed43[3] = 44.0;
	config->tcu_shiftSpeed43[4] = 48.0;
	config->tcu_shiftSpeed43[5] = 62.0;
	config->tcu_shiftSpeed43[6] = 73.0;
	config->tcu_shiftSpeed43[7] = 88.0;
}

#endif // EFI_TCU
