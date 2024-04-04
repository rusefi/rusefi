#include "pch.h"

#include "tc_4l6x.h"

#if EFI_TCU
Gm4l6xTransmissionController gm4l6xTransmissionController;
static SimplePwm tccPwm("TCC Control");
static SimplePwm shift32Pwm("3-2 Shift Control");

void Gm4l6xTransmissionController::init() {
	Generic4TransmissionController::init();
	
	enginePins.tcuTccPwmSolenoid.initPin("TCC PWM Solenoid", engineConfiguration->tcu_tcc_pwm_solenoid, engineConfiguration->tcu_tcc_pwm_solenoid_mode);
	startSimplePwm(&tccPwm,
								 "TCC",
								 &engine->executor,
								 &enginePins.tcuTccPwmSolenoid,
								 engineConfiguration->tcu_tcc_pwm_solenoid_freq,
								 0);

	enginePins.tcu32Solenoid.initPin("3-2 Shift Solenoid", engineConfiguration->tcu_32_solenoid_pin, engineConfiguration->tcu_32_solenoid_pin_mode);
	startSimplePwm(&shift32Pwm,
								 "3-2 Solenoid",
								 &engine->executor,
								 &enginePins.tcu32Solenoid,
								 engineConfiguration->tcu_32_solenoid_freq,
								 0);
}

void Gm4l6xTransmissionController::update(gear_e gear) {
	Generic4TransmissionController::update(gear);

	set32State(gear);
}

void Gm4l6xTransmissionController::set32State(gear_e gear) {
	if (isShifting && shiftingFrom == GEAR_3 && gear == GEAR_2) {
		auto vss = Sensor::get(SensorType::VehicleSpeed);
		if (!vss.Valid) {
			return;
		}
		//huh?uint8_t (*pcts)[sizeof(config->tcu_32SpeedBins)/sizeof(config->tcu_32SpeedBins[0])];
		int pct = interpolate2d(vss.Value, config->tcu_32SpeedBins, config->tcu_32Vals);
		shift32Pwm.setSimplePwmDutyCycle(pct*0.01);
	} else {
		shift32Pwm.setSimplePwmDutyCycle(0);
	}
}

Gm4l6xTransmissionController* getGm4l6xTransmissionController() {
	return &gm4l6xTransmissionController;
}

// here we have default 4R70W calibration
void configureTcu4R70W() {
}
#endif // EFI_TCU
