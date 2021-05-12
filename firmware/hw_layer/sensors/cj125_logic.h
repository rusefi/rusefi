/*
 * @file CJ125_logic.h
 *
 * @date Feb 1, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine_configuration.h"
#include "pwm_generator_logic.h"
#include "pid.h"

typedef enum {
	CJ125_LSU_42 = 0,
	CJ125_LSU_49 = 1,

} cj125_sensor_type_e;

typedef enum {
	CJ125_INIT = 0,
	CJ125_IDLE = 1,
	CJ125_CALIBRATION = 2,
	CJ125_PREHEAT = 3,
	CJ125_HEAT_UP = 4,
	CJ125_READY = 5,
	CJ125_OVERHEAT = 6,
	CJ125_ERROR = 7,

} cj125_state_e;

typedef enum {
	CJ125_NO_ERROR = 0,
	CJ125_ERROR_HEATER_MALFUNCTION = 1,
	CJ125_ERROR_OVERHEAT = 2,
	CJ125_ERROR_WRONG_IDENT = 3,
	CJ125_ERROR_WRONG_INIT = 4,
	CJ125_ERROR_DISABLED = 5,
} cj125_error_e;

typedef enum {
	CJ125_MODE_NONE,
	CJ125_MODE_NORMAL_8,
	CJ125_MODE_NORMAL_17,
	CJ125_MODE_CALIBRATION,
} cj125_mode_e;

class Cj125SpiStream {
public:
	virtual uint8_t ReadRegister(uint8_t regAddr) = 0;
	virtual void WriteRegister(uint8_t regAddr, uint8_t regValue) = 0;
};

class CJ125 {
public:
	CJ125();
	Cj125SpiStream *spi = nullptr;

	SimplePwm wboHeaterControl;

	// Chip diagnostics register contents
	volatile int diag = 0;

	efitick_t startHeatingNt;
	efitick_t prevNt;
	float heaterDuty = 0.0f;

	pid_s heaterPidConfig;
	Pid heaterPid;

	volatile cj125_mode_e mode = CJ125_MODE_NONE;

	// Amplification coefficient, needed by cjGetAfr()
	volatile float amplCoeff = 0.0f;
	// Calculated Lambda-value
	volatile float lambda = 1.0f;

	// Current values
	// lambda
	volatile float vUa = 0.0f;
	// heater
	volatile float vUr = 0.0f;

	// Calibration values
	// lambda
	volatile float vUaCal = 0.0f;
	// header
	volatile float vUrCal = 0.0f;

	OutputPin wboHeaterPin;
	OutputPin cj125Cs;

	// Used by CJ125 driver state machine
	volatile cj125_state_e state = CJ125_INIT;
	// Last Error code
	volatile cj125_error_e errorCode = CJ125_NO_ERROR;

	void setError(cj125_error_e errCode DECLARE_ENGINE_PARAMETER_SUFFIX);
	bool isWorkingState(void) const;
	void SetHeater(float value DECLARE_ENGINE_PARAMETER_SUFFIX);
	void SetIdleHeater(DECLARE_ENGINE_PARAMETER_SIGNATURE);
	void StartHeaterControl(DECLARE_ENGINE_PARAMETER_SIGNATURE);
	bool cjIdentify(DECLARE_ENGINE_PARAMETER_SIGNATURE);
	void printDiag();
	void calibrate(DECLARE_ENGINE_PARAMETER_SIGNATURE);
	void cjSetMode(cj125_mode_e m);
	bool isValidState() const;
	void cjInitPid(DECLARE_ENGINE_PARAMETER_SIGNATURE);
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

// CJ125 SPI Registers
#define	IDENT_REG_RD					0x48 // Read Identity Register
#define	INIT_REG1_WR					0x56 // Write To Initialization Register 1
#define	INIT_REG2_WR					0x5A // Write To Initialization Register 2
#define	INIT_REG1_RD					0x6C // Read Initialization Register 1
#define	DIAG_REG_RD						0x78 // Read Diagnostics Register
#define	INIT_REG2_RD					0x7E // Read Initialization Register 2

#define	CJ125_IDENT						0x60 // 96
#define	CJ125_IDENT_MASK 				0xF8

#define	CJ125_INIT1_NORMAL_8			0x88 // 0b10001000 (Normal mode, Amplification 8)
#define	CJ125_INIT1_NORMAL_17			0x89 // 0b10001001 (Normal mode, Amplification 17)
#define	CJ125_INIT1_CALBRT				0x9D // 0b10011101 (Calibration mode, LA=1, RA=1)

#define	CJ125_INIT2_NORMAL				0x00 // 0b00000000, (Normal mode)
#define	CJ125_INIT2_DIAG 				0x10 // 0b00010000, (Extended diagnostics mode, SET_DIA_Q=1)
#define	CJ125_INIT2_RESET				0x40 // 0b01000000, SRESET=1

#define	CJ125_DIAG_NORM					0xFF // no errors

#define CJ125_UACAL_MIN					1.0f	// Calibration UA values range
#define CJ125_UACAL_MAX					2.0f

// Some experimental magic values for heater PID regulator
#define CJ125_PID_LSU42_P				(80.0f / 16.0f * 5.0f / 1024.0f)
#define CJ125_PID_LSU42_I				(25.0f / 16.0f * 5.0f / 1024.0f)

#define CJ125_PID_LSU49_P               (8.0f)
#define CJ125_PID_LSU49_I               (0.003f)

