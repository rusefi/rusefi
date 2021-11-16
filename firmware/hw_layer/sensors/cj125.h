/*
 * @file cj125.h
 *
 * @date: Jul 17, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "cj125_logic.h"


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

#define CJ125_CALIBRATION_DELAY 		1000 	// 1 sec
#define CJ125_TICK_DELAY 				20		// 20 ms
#define CJ125_IDLE_TICK_DELAY 			1000	// 1 sec

#define CJ125_CALIBRATE_NUM_SAMPLES 	10

#define CJ125_UR_PREHEAT_THR			2.0f	// Ur > 2.0 Volts is too cold to control with PID
#define CJ125_UR_OVERHEAT_THR			0.5f	// Ur < 0.5 Volts is overheat
#define CJ125_UR_GOOD_THR				1.4f

#define CJ125_PREHEAT_MIN_DUTY			0.9f

//#define CJ125_PREHEAT_TIMEOUT	90		// 90 secs
#define CJ125_PREHEAT_TIMEOUT	300

#define CJ125_VOLTAGE_TO_16BIT_FACTOR	4096.0f

#define CJ125_PUMP_SHUNT_RESISTOR		61.9f
#define CJ125_STOICH_RATIO				STOICH_RATIO
#define CJ125_PUMP_CURRENT_FACTOR		1000.0f

// Returned if there's no valid measurement
#define CJ125_AFR_NAN					0.0f


#if EFI_TUNER_STUDIO
void cjPostState(TunerStudioOutputChannels *tsOutputChannels);
#endif /* EFI_TUNER_STUDIO */

void initCJ125(DECLARE_ENGINE_PARAMETER_SIGNATURE);

void cjRestart(void);
void cjStartCalibration(void);
float cjGetAfr(DECLARE_ENGINE_PARAMETER_SIGNATURE);
bool cjHasAfrSensor();
void cj125defaultPinout();
