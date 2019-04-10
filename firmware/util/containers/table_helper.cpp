/**
 * @file	table_helper.cpp
 * @brief	Helper methods related to 3D & 2D tables manipulation (maps and curves)
 *
 * @date Jul 6, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "table_helper.h"
#include "efilib.h"
#include "interpolation.h"

/**
 * @param precision for example '0.1' for one digit fractional part
 */
void setLinearCurve(float array[], int size, float from, float to, float precision) {
	for (int i = 0; i < size; i++) {
		float value = interpolateMsg("setLinearCurve", 0, from, size - 1, to, i);
		/**
		 * rounded values look nicer, also we want to avoid precision mismatch with Tuner Studio
		 */
		array[i] = efiRound(value, precision);
	}
}

void setRpmBin(float array[], int size, float idleRpm, float topRpm) {
	array[0] = idleRpm - 150;
	int rpmStep = (int)(efiRound((topRpm - idleRpm) / (size - 2), 50) - 150);
	for (int i = 1; i < size - 1;i++)
		array[i] = idleRpm + rpmStep * (i - 1);
	array[size - 1] = topRpm;
}

void setTableBin(float array[], int size, float from, float to) {
	setLinearCurve(array, size, from, to, 0.01);
}

/**
 * initialize RPM table axis using default RPM range
 */
void setRpmTableBin(float array[], int size) {
	setRpmBin(array, size, 800, 7000);
}
