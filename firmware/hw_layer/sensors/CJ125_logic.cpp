/*
 * @file CJ125_logic.cpp
 *
 * @date Feb 1, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "CJ125_logic.h"
#include "engine.h"

EXTERN_ENGINE;

CJ125::CJ125() : wboHeaterControl("wbo") {
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
