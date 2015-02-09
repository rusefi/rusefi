/**
 * @file	table_helper.cpp
 * @brief	Helper methods related to 3D & 2D tables manipulation (maps and curves)
 *
 * @date Jul 6, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
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

void Table2D::init(int size, float *aTable, float *bTable) {
	this->size = size;
	this->aTable = aTable;
	this->bTable = bTable;
}

void Table2D::preCalc(float *bin, float *values) {
	this->bin = bin;
	for (int i = 0; i < size - 1; i++) {
		float x1 = bin[i];
		float x2 = bin[i + 1];
		if (x1 == x2) {
			firmwareError("Same x1 and x2 in interpolate: %f/%f", x1, x2);
			return;
		}

		float y1 = values[i];
		float y2 = values[i + 1];

		aTable[i] = INTERPOLATION_A(x1, y1, x2, y2);
		bTable[i] = y1 - aTable[i] * x1;
	}
}

void setTableBin(float array[], int size, float from, float to) {
	setTableBin2(array, size, from, to, 0.01);
}

void setRpmTableBin(float array[], int size) {
	setTableBin2(array, size, 800, 7000, 1);
}
