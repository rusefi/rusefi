/*
 * @file CJ125_logic.h
 *
 * @date Feb 1, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#ifndef HW_LAYER_SENSORS_CJ125_LOGIC_H_
#define HW_LAYER_SENSORS_CJ125_LOGIC_H_

#include "engine_configuration.h"
#include "pwm_generator_logic.h"

typedef enum {
	CJ125_LSU_42 = 0,
	CJ125_LSU_49 = 1,

} cj125_sensor_type_e;

typedef enum {
	CJ125_INIT,
	CJ125_IDLE,
	CJ125_CALIBRATION,
	CJ125_PREHEAT,
	CJ125_HEAT_UP,
	CJ125_READY,
	CJ125_OVERHEAT,
	CJ125_ERROR,

} cj125_state_e;

typedef enum {
	CJ125_ERROR_NONE = 0,
	CJ125_ERROR_HEATER_MALFUNCTION = 1,
	CJ125_ERROR_OVERHEAT = 2,
	CJ125_ERROR_WRONG_IDENT = 3,
	CJ125_ERROR_WRONG_INIT = 4,
} cj125_error_e;

typedef enum {
	CJ125_MODE_NONE,
	CJ125_MODE_NORMAL_8,
	CJ125_MODE_NORMAL_17,
	CJ125_MODE_CALIBRATION,
} cj125_mode_e;

class CJ125 {
public:
	CJ125();

	SimplePwm wboHeaterControl;

	efitick_t startHeatingNt;
	efitick_t prevNt;
	float heaterDuty = 0.0f;

	OutputPin wboHeaterPin;
	OutputPin cj125Cs;

	// Used by CJ125 driver state machine
	volatile cj125_state_e state = CJ125_INIT;
	// Last Error code
	volatile cj125_error_e errorCode = CJ125_ERROR_NONE;

	void setError(cj125_error_e errCode DECLARE_ENGINE_PARAMETER_SUFFIX);
	bool isWorkingState(void);
	void SetHeater(float value DECLARE_ENGINE_PARAMETER_SUFFIX);
	void SetIdleHeater(DECLARE_ENGINE_PARAMETER_SIGNATURE);
	void StartHeaterControl(pwm_gen_callback *stateChangeCallback DECLARE_ENGINE_PARAMETER_SUFFIX);
};

// Heater params for Idle(cold), Preheating and Control stages
// See http://www.waltech.com/wideband-files/boschsensordatasheet.htm
#define CJ125_HEATER_IDLE_RATE			0.15f	// for a very cold sensor (presumably), we allow 15% duty max.
#define CJ125_HEATER_PREHEAT_PERIOD 	300		// 300 ms
#define CJ125_HEATER_CONTROL_PERIOD		180		// 180 ms
#define CJ125_HEATER_OVERHEAT_PERIOD	500		// 500 ms
#define CJ125_HEATER_PREHEAT_RATE		(0.4f/14.0f)	// Assuming that dutycycle=1.0 equals to 14V, and max.allowed heater rate is 0.4V/sec

#define CJ125_HEATER_MIN_DUTY			0.1f
#define CJ125_HEATER_PWM_FREQ 			100		// 100 Hz

#define CJ125_HEATER_LIMITING_VOLTAGE	12.0f	// Do not allow more than 90% heating for high battery voltage (>12V).
#define CJ125_HEATER_LIMITING_RATE		0.92f	// This prevents sensor overheating.

#endif /* HW_LAYER_SENSORS_CJ125_LOGIC_H_ */
