/*
 * @file CJ125.cpp
 *
 * @date: Jun 24, 2016
 * @author Andrey Belomutskiy, (c) 2012-2016
 *
 */

/**********************************************************************************
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


void initCJ125(void) {
	// still a lot to be done here :)
}



