/*
 * @file CJ125_logic.cpp
 *
 * @date Feb 1, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "cj125_logic.h"
#include "engine.h"

EXTERN_ENGINE;

CJ125::CJ125() : wboHeaterControl("wbo"),
		heaterPid(&heaterPidConfig){
}

void CJ125::SetHeater(float value DECLARE_ENGINE_PARAMETER_SUFFIX) {
	// limit duty cycle for sensor safety
	// todo: would be much nicer to have continuous function (vBatt)
	float maxDuty = (engine->sensors.vBatt > CJ125_HEATER_LIMITING_VOLTAGE) ? CJ125_HEATER_LIMITING_RATE : 1.0f;
	heaterDuty = (value < CJ125_HEATER_MIN_DUTY) ? 0.0f : minF(maxF(value, 0.0f), maxDuty);
#ifdef CJ125_DEBUG
	scheduleMsg(logger, "cjSetHeater: %.2f", heaterDuty);
#endif
	// a little trick to disable PWM if needed.
	// todo: this should be moved to wboHeaterControl.setPwmDutyCycle()
	// todo: is this really needed?!
	wboHeaterControl.setFrequency(heaterDuty == 0.0f ? NAN : CJ125_HEATER_PWM_FREQ);
	wboHeaterControl.setSimplePwmDutyCycle(heaterDuty);
}

void CJ125::SetIdleHeater(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	// small preheat for faster start & moisture anti-shock therapy for the sensor
	SetHeater(CJ125_HEATER_IDLE_RATE PASS_ENGINE_PARAMETER_SUFFIX);
}

bool CJ125::isWorkingState(void) {
	return state != CJ125_ERROR && state != CJ125_INIT && state != CJ125_IDLE;
}

void CJ125::StartHeaterControl(pwm_gen_callback *stateChangeCallback DECLARE_ENGINE_PARAMETER_SUFFIX) {
	// todo: use custom pin state method, turn pin off while not running
	startSimplePwmExt(&wboHeaterControl, "wboHeaterPin",
			&engine->executor,
			CONFIGB(wboHeaterPin),
			&wboHeaterPin, CJ125_HEATER_PWM_FREQ, 0.0f, stateChangeCallback);
	SetIdleHeater(PASS_ENGINE_PARAMETER_SIGNATURE);
}

void CJ125::cjIdentify(void) {
	// read Ident register
	int ident = spi->ReadRegister(IDENT_REG_RD) & CJ125_IDENT_MASK;

	// set initial registers
	spi->WriteRegister(INIT_REG1_WR, CJ125_INIT1_NORMAL_17);
	spi->WriteRegister(INIT_REG2_WR, CJ125_INIT2_DIAG);
	// check if regs are ok
	int init1 = spi->ReadRegister(INIT_REG1_RD);
	int init2 = spi->ReadRegister(INIT_REG2_RD);

	diag = spi->ReadRegister(DIAG_REG_RD);
	scheduleMsg(logger, "cj125: Check ident=0x%x diag=0x%x init1=0x%x init2=0x%x", ident, diag, init1, init2);
	if (ident != CJ125_IDENT) {
		scheduleMsg(logger, "cj125: Error! Wrong ident! Cannot communicate with CJ125!");
	}
	if (init1 != CJ125_INIT1_NORMAL_17 || init2 != CJ125_INIT2_DIAG) {
		scheduleMsg(logger, "cj125: Error! Cannot set init registers! Cannot communicate with CJ125!");
	}
#if 0
	if (diag != CJ125_DIAG_NORM) {
		scheduleMsg(logger, "cj125: Diag error!");
	}
#endif
}

void CJ125::cjSetMode(cj125_mode_e m) {
	if (mode == m)
		return;
	switch (m) {
	case CJ125_MODE_NORMAL_8:
		spi->WriteRegister(INIT_REG1_WR, CJ125_INIT1_NORMAL_8);
		amplCoeff = 1.0f / 8.0f;
		break;
	case CJ125_MODE_NORMAL_17:
		spi->WriteRegister(INIT_REG1_WR, CJ125_INIT1_NORMAL_17);
		amplCoeff = 1.0f / 17.0f;
		break;
	case CJ125_MODE_CALIBRATION:
		spi->WriteRegister(INIT_REG1_WR, CJ125_INIT1_CALBRT);
		amplCoeff = 0.0f;
		break;
	default:
		;
	}
	mode = m;
}

bool CJ125::isValidState() {
	// check if controller is functioning
	if (!isWorkingState())
		return false;
	// check if amplification is turned on
	if (amplCoeff == 0.0f)
		return false;
	// check if UA calibration value is valid
	if (vUaCal < CJ125_UACAL_MIN || vUaCal > CJ125_UACAL_MAX)
		return false;
	return true;
}

void CJ125::cjInitPid(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if(engineConfiguration->cj125isLsu49) {
		heaterPidConfig.pFactor = CJ125_PID_LSU49_P;
		heaterPidConfig.iFactor = CJ125_PID_LSU49_I;
	} else {
		heaterPidConfig.pFactor = CJ125_PID_LSU42_P;
		heaterPidConfig.iFactor = CJ125_PID_LSU42_I;
	}
	heaterPidConfig.dFactor = 0.0f;
	heaterPidConfig.minValue = 0;
	heaterPidConfig.maxValue = 1;
	heaterPidConfig.offset = 0;
	// todo: period?
	heaterPidConfig.periodMs = 1.0f;
	heaterPid.reset();
}
