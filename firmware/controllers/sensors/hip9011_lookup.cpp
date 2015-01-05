/**
 * @file	hip9011_lookup.cpp
 *
 * @date Jan 4, 2015
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "hip9011_lookup.h"

/**
 * These are HIP9011 magic values - integrator time constants in uS
 */
const int integratorValues[INT_LOOKUP_SIZE] = { 40, 45, 50, 55, 60, 65, 70, 75, 80, 90, 100, 110, 120, 130, 140, 150,
		160, 180, 200, 220, 240, 260, 280, 300, 320, 360, 400, 440, 480, 520, 560, 600 };

/**
 * 'TC is typically TINT/(2*Pi*VOUT)'
 * Knock Sensor Training TPIC8101, page 24
 */
float getRpmByAngleWindowAndTimeUs(int timeUs, float angleWindowWidth) {
	float windowWidthMult = angleWindowWidth / 360.0f;
	// '60000000' because revolutions per MINUTE in uS conversion

	return 60000000.0f / (timeUs * 2 * PIF * DESIRED_OUTPUT_VALUE * windowWidthMult);
}
