/*
 * @file CJ125_logic.cpp
 *
 * @date Feb 1, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "cj125_logic.h"

#define LOW_VOLTAGE "Low Voltage"

CJ125::CJ125() : wboHeaterControl("wbo"),
		heaterPid(&heaterPidConfig) {
}

void CJ125::SetHeater(float value DECLARE_ENGINE_PARAMETER_SUFFIX) {
	// limit duty cycle for sensor safety
	// todo: would be much nicer to have continuous function (vBatt)
	float maxDuty = (Sensor::get(SensorType::BatteryVoltage).value_or(VBAT_FALLBACK_VALUE) > CJ125_HEATER_LIMITING_VOLTAGE) ? CJ125_HEATER_LIMITING_RATE : 1.0f;
	heaterDuty = (value < CJ125_HEATER_MIN_DUTY) ? 0.0f : minF(maxF(value, 0.0f), maxDuty);
#ifdef CJ125_DEBUG
	efiPrintf("cjSetHeater: %.2f", heaterDuty);
#endif
	// a little trick to disable PWM if needed.
	// todo: this should be moved to wboHeaterControl.setPwmDutyCycle()
	// todo: is this really needed?!
	wboHeaterControl.setFrequency(heaterDuty == 0.0f ? NAN : CJ125_HEATER_PWM_FREQ);
	wboHeaterControl.setSimplePwmDutyCycle(heaterDuty);
}

void CJ125::SetIdleHeater(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	// small preheat for faster start & moisture anti-shock therapy for the sensor
	SetHeater(CJ125_HEATER_IDLE_RATE);
}

bool CJ125::isWorkingState(void) const {
	return state != CJ125_ERROR && state != CJ125_INIT && state != CJ125_IDLE;
}

void CJ125::StartHeaterControl(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	// todo: use custom pin state method, turn pin off while not running
	startSimplePwmExt(&wboHeaterControl, "wboHeaterPin",
			&engine->executor,
			CONFIG(wboHeaterPin),
			&wboHeaterPin, CJ125_HEATER_PWM_FREQ, 0.0f);
	SetIdleHeater();
}

static void printDiagCode(const char *msg, int code, const char *code1message) {
	switch(code & 0x3) {
	case 0:
		efiPrintf("%s Short to GND", msg);
		return;
	case 1:
		efiPrintf("%s %s", msg, code1message);
		return;
	case 2:
		efiPrintf("%s Short to Vbatt", msg);
		return;
	case 3:
		efiPrintf("%s LOOKS GOOD", msg);
		return;
	}
}

void CJ125::printDiag() {
	if (diag == CJ125_DIAG_NORM) {
		efiPrintf("cj125: diag Looks great!");
	} else {
		efiPrintf("cj125: diag NOT GOOD");
		printDiagCode("VM", diag, LOW_VOLTAGE);
		printDiagCode("UN", diag >> 2, LOW_VOLTAGE);
		printDiagCode("IA", diag >> 4, LOW_VOLTAGE);
		printDiagCode("HR", diag >> 6, "open load");
/* todo: do we want to throw CRITICAL on diag start-up error? probably not?
		firmwareError(CUSTOM_ERR_CJ125_DIAG, "CJ125 is not well");
*/
	}
}

/**
 * @return true in case of positive SPI identification
 *         false in case of unexpected SPI response
 */
bool CJ125::cjIdentify(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	efiAssert(OBD_PCM_Processor_Fault, spi!= NULL, "No SPI pointer", false);
	// read Ident register
	int ident = spi->ReadRegister(IDENT_REG_RD) & CJ125_IDENT_MASK;

	// set initial registers
	spi->WriteRegister(INIT_REG1_WR, CJ125_INIT1_NORMAL_17);
	spi->WriteRegister(INIT_REG2_WR, CJ125_INIT2_DIAG);
	// check if regs are ok
	int init1 = spi->ReadRegister(INIT_REG1_RD);
	int init2 = spi->ReadRegister(INIT_REG2_RD);

	diag = spi->ReadRegister(DIAG_REG_RD);
	efiPrintf("cj125: Check ident=0x%x diag=0x%x init1=0x%x init2=0x%x", ident, diag, init1, init2);
	if (ident != CJ125_IDENT) {
		efiPrintf("cj125: Error! Wrong ident! Cannot communicate with CJ125!");
		setError(CJ125_ERROR_WRONG_IDENT);
		return false;
	}
	if (init1 != CJ125_INIT1_NORMAL_17 || init2 != CJ125_INIT2_DIAG) {
		efiPrintf("cj125: Error! Cannot set init registers! Cannot communicate with CJ125!");
		setError(CJ125_ERROR_WRONG_IDENT);
		return false;
	}
	printDiag();
	return true;
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

bool CJ125::isValidState() const {
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
	if (engineConfiguration->cj125isLsu49) {
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
	/**
	 * See hard-coded CJ125_TICK_DELAY - we run PID at 50Hz
	 */
	heaterPid.reset();
}
