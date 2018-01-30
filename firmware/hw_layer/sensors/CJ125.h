/*
 * @file CJ125.h
 *
 * @date: Jul 17, 2016
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#ifndef HW_LAYER_SENSORS_CJ125_H_
#define HW_LAYER_SENSORS_CJ125_H_

// CJ125 SPI Registers
#define	IDENT_REG_RD					0x48 // Read Identity Register
#define	INIT_REG1_WR					0x56 // Write To Initialization Register 1
#define	INIT_REG2_WR					0x5A // Write To Initialization Register 2
#define	INIT_REG1_RD					0x6C // Read Initialization Register 1
#define	DIAG_REG_RD						0x78 // Read Diagnostics Register
#define	INIT_REG2_RD					0x7E // Read Initialization Register 2

/**********************************************************************************
 * Courtesy "Turbo SOB"
 * http://www.turbo-mopar.com/forums/showthread.php?77041-Wideband-O2-Controller-Project-with-EGT-A2D-DAQ-Diagnostic-Etc&p=1110559#post1110559

    INIT_REG1 - CJ125 Initialization Register 1
        00x000x0
        ||||||||---- VL - Pump Current Sense Amplifier - 0 = x8, 1 = x17
                            If VL is set to "0" the amplification of Ip is 8.
                            If VL is "1" the amplification is 17. The higher
                            amplification is needed for a better resolution
                            at Lambda > 1, amplification 8 for Lambda < 1.
                            Note: It seems to make no difference, always set to 1.
        |||||||----- Unused
        ||||||------ LA - Pump Current Sense Amplifier - 0 = measurement, 1 = calibration
                            If LA is "0" the measurement mode for Ip is active.
                            The current pump current is displayed on Ua. If LA is
                            set to "1" the calibration mode is active. The shown
                            voltage on Ua must been subtracted from the later measurement.
        |||||------- Undocumented Bit - Note: CJ125 only seems to work when set to 1.
        ||||-------- RA - Measurement Current for Ri - 0 = measurement, 1 = calibration
                            If RA is "0" the measurement mode for Ri is active and
                            the current resistance of the probe is "displayed" on Ur.
                            If RA is "1" the calibration mode is active. You get the
                            optimal voltage for the inner resistance of the probe on Ur.
        |||--------- Unused
        ||---------- PA - Pump Current Control - Set to 0 to be active
        |----------- ENABLE/HOLD - Must be set to 1 to enable
***********************************************************************************/

/**********************************************************************************
    INIT_REG2 - CJ125 Initialization Register 1
        xxx00000
        ||||||||---- PR0 - Reference Pump Current (10 uA)
        |||||||----- PR1 - Reference Pump Current (20 uA)
        ||||||------ PR2 - Reference Pump Current (40 uA)
        |||||------- PR3 - Reference Pump Current (80 uA)
        ||||-------- ENSCUN - Enables diagnostics for pin UN of the CJ125
        |||--------- Unused
        ||---------- Unused
        |----------- Unused
***********************************************************************************/

/**********************************************************************************
    DIAG_REG - CJ125 Diagnostic Register Definition (Read Only)
        00000000
        ||||||||---- Sensor VM Diagnostic Bit 0
        |||||||----- Sensor VM Diagnostic Bit 1
        ||||||------ Sensor UN Diagnostic Bit 0
        |||||------- Sensor UN Diagnostic Bit 1
        ||||-------- Sensor IA/IP Diagnostic Bit 0
        |||--------- Sensor IA/IP Diagnostic Bit 1
                            00 = Short circuit to ground
                            01 = Low Battery
                            10 = Short circuit to Vbatt
                            11 = No Failure
        ||---------- Ext. Heater DIAHD Diagnostic Bit
        |----------- Ext. Heater DIAHG Diagnostic Bit
                            00 = Short circuit to ground
                            01 = Open Load
                            10 = Short circuit to Vbatt
                            11 = No Failure
***********************************************************************************/

#define	CJ125_INIT1_NORMAL_8			0x88 // 0b10001000 (Normal mode, Amplification 8)
#define	CJ125_INIT1_NORMAL_17			0x89 // 0b10001001 (Normal mode, Amplification 17)
#define	CJ125_INIT1_CALBRT				0x9D // 0b10011101 (Calibration mode, LA=1, RA=1)

#define	CJ125_INIT2_NORMAL				0x00 // 0b00000000, (Normal mode)
#define	CJ125_INIT2_DIAG 				0x10 // 0b00010000, (Extended diagnostics mode, SET_DIA_Q=1)
#define	CJ125_INIT2_RESET				0x40 // 0b01000000, SRESET=1

#define	CJ125_DIAG_NORM					0xFF // no errors

#define	CJ125_IDENT						0x60
#define	CJ125_IDENT_MASK 				0xF8

#define CJ125_CALIBRATION_DELAY 		1000 	// 1 sec
#define CJ125_TICK_DELAY 				20		// 20 ms
#define CJ125_IDLE_TICK_DELAY 			1000	// 1 sec

// Heater params for Idle(cold), Preheating and Control stages
// See http://www.waltech.com/wideband-files/boschsensordatasheet.htm
#define CJ125_HEATER_IDLE_RATE			0.15f	// for a very cold sensor (presumably), we allow 15% duty max.
#define CJ125_HEATER_PREHEAT_PERIOD 	300		// 300 ms
#define CJ125_HEATER_CONTROL_PERIOD		180		// 180 ms
#define CJ125_HEATER_OVERHEAT_PERIOD	500		// 500 ms
#define CJ125_HEATER_PWM_FREQ 			100		// 100 Hz
#define CJ125_HEATER_PREHEAT_RATE		(0.4f/14.0f)	// Assuming that dutycycle=1.0 equals to 14V, and max.allowed heater rate is 0.4V/sec

#define CJ125_HEATER_LIMITING_VOLTAGE	12.0f	// Do not allow more than 90% heating for high battery voltage (>12V).
#define CJ125_HEATER_LIMITING_RATE		0.92f	// This prevents sensor overheating.

#define CJ125_CALIBRATE_NUM_SAMPLES 	10

#define CJ125_UR_PREHEAT_THR			2.0f	// Ur > 2.0 Volts is too cold to control with PID
#define CJ125_UR_OVERHEAT_THR			0.5f	// Ur < 0.5 Volts is overheat
#define CJ125_UR_GOOD_THR				1.4f

#define CJ125_UACAL_MIN					1.0f	// Calibration UA values range
#define CJ125_UACAL_MAX					2.0f

#define CJ125_HEATER_MIN_DUTY			0.1f
#define CJ125_PREHEAT_MIN_DUTY			0.9f

//#define CJ125_PREHEAT_TIMEOUT	90		// 90 secs
#define CJ125_PREHEAT_TIMEOUT	300

#define CJ125_VOLTAGE_TO_16BIT_FACTOR	4096.0f

#define CJ125_PUMP_SHUNT_RESISTOR		61.9f
#define CJ125_STOICH_RATIO				14.7f
#define CJ125_PUMP_CURRENT_FACTOR		1000.0f

// Some experimental magic values for heater PID regulator
#define CJ125_PID_LSU42_P				(80.0f / 16.0f * 5.0f / 1024.0f)
#define CJ125_PID_LSU42_I				(25.0f / 16.0f * 5.0f / 1024.0f)

// Returned if there's no valid measurement
#define CJ125_AFR_NAN					0.0f

typedef enum {
	CJ125_IDLE,
	CJ125_INIT,
	CJ125_CALIBRATION,
	CJ125_PREHEAT,
	CJ125_HEAT_UP,
	CJ125_READY,
	CJ125_OVERHEAT,
	CJ125_ERROR,

} cj125_state_e;

typedef enum {
	CJ125_ERROR_NONE,
	CJ125_ERROR_HEATER_MALFUNCTION,
	CJ125_ERROR_OVERHEAT,

} cj125_error_e;

typedef enum {
	CJ125_MODE_NONE,
	CJ125_MODE_NORMAL_8,
	CJ125_MODE_NORMAL_17,
	CJ125_MODE_CALIBRATION,
} cj125_mode_e;

typedef enum {
	CJ125_LSU_42 = 0,
	CJ125_LSU_49 = 1,

} cj125_sensor_type_e;


void initCJ125(Logging *shared);
void cjPostState(TunerStudioOutputChannels *tsOutputChannels);
float cjGetAfr(DECLARE_ENGINE_PARAMETER_SIGNATURE);
bool cjHasAfrSensor(DECLARE_ENGINE_PARAMETER_SIGNATURE);

#endif /* HW_LAYER_SENSORS_CJ125_H_ */
