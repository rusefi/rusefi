/**
 * @file	table_helper.cpp
 * @brief	Helper methods related to 3D & 2D tables manipulation (maps and curves)
 *
 * @date Jul 6, 2014
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#include "table_helper.h"
#include "efilib.h"
#include "interpolation.h"

void setTableBin2(float array[], int size, float l, float r, float precision) {
	for (int i = 0; i < size; i++) {
		float value = interpolate(0, l, size - 1, r, i);
		/**
		 * rounded values look nicer, also we want to avoid precision mismatch with Tuner Studio
		 */
		array[i] = efiRound(value, precision);
	}
}

void setTableBin(float array[], int size, float from, float to) {
	setTableBin2(array, size, from, to, 0.01);
}

void setRpmTableBin(float array[], int size) {
	setTableBin2(array, size, 800, 7000, 100);
}
