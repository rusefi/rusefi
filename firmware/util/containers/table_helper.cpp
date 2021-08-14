/**
 * @file	table_helper.cpp
 * @brief	Helper methods related to 3D & 2D tables manipulation (maps and curves)
 *
 * @date Jul 6, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

void setRpmBin(float array[], int size, float idleRpm, float topRpm) {
	array[0] = idleRpm - 150;
	int rpmStep = (int)(efiRound((topRpm - idleRpm) / (size - 2), 50) - 150);
	for (int i = 1; i < size - 1;i++)
		array[i] = idleRpm + rpmStep * (i - 1);
	array[size - 1] = topRpm;
}

/**
 * initialize RPM table axis using default RPM range
 */
void setRpmTableBin(float array[], int size) {
	setRpmBin(array, size, 800, 7000);
}
