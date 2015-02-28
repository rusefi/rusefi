/**
 * @file	table_helper.h
 *
 * @date Jul 6, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
 */
#ifndef TABLE_HELPER_H_
#define TABLE_HELPER_H_

#include <math.h>
#include "error_handling.h"
#include "interpolation.h"

// 'random' value to be sure we are not treating any non-zero trash as TRUE
#define MAGIC_TRUE_VALUE 153351512

template<int RPM_BIN_SIZE, int LOAD_BIN_SIZE>
class Map3D {
public:
	Map3D();
	void init(float table[RPM_BIN_SIZE][LOAD_BIN_SIZE], float loadBins[LOAD_BIN_SIZE], float rpmBins[RPM_BIN_SIZE]);
	float getValue(float x, float rpm);
	void setAll(float value);
private:
	float *pointers[LOAD_BIN_SIZE];
	float *loadBins;
	float *rpmBins;
	int initialized;
};

template<int SIZE>
class Table2D {
public:
	Table2D();
	void preCalc(float *bin, float *values);
	float aTable[SIZE];
	float bTable[SIZE];
	float *bin;
};

template<int SIZE>
Table2D<SIZE>::Table2D() {
	bin = NULL;
}


template<int SIZE>
void Table2D<SIZE>::preCalc(float *bin, float *values) {
	this->bin = bin;
	for (int i = 0; i < SIZE - 1; i++) {
		float x1 = bin[i];
		float x2 = bin[i + 1];
		if (x1 == x2) {
			warning(OBD_PCM_Processor_Fault, "preCalc: Same x1 and x2 in interpolate: %f/%f", x1, x2);
			return;
		}

		float y1 = values[i];
		float y2 = values[i + 1];

		aTable[i] = INTERPOLATION_A(x1, y1, x2, y2);
		bTable[i] = y1 - aTable[i] * x1;
	}
}


template<int RPM_BIN_SIZE, int LOAD_BIN_SIZE>
void Map3D<RPM_BIN_SIZE, LOAD_BIN_SIZE>::init(float table[RPM_BIN_SIZE][LOAD_BIN_SIZE],
		float loadBins[LOAD_BIN_SIZE],
		float rpmBins[RPM_BIN_SIZE]) {
  for (int k = 0; k < LOAD_BIN_SIZE; k++) {
		pointers[k] = table[k];
  }
	initialized = MAGIC_TRUE_VALUE;
	this->loadBins = loadBins;
	this->rpmBins = rpmBins;
}

template<int RPM_BIN_SIZE, int LOAD_BIN_SIZE>
float Map3D<RPM_BIN_SIZE, LOAD_BIN_SIZE>::getValue(float x, float rpm) {
	efiAssert(initialized == MAGIC_TRUE_VALUE, "map not initialized", NAN);
	return interpolate3d(x, loadBins, LOAD_BIN_SIZE, rpm, rpmBins, RPM_BIN_SIZE, pointers);
}

template<int RPM_BIN_SIZE, int LOAD_BIN_SIZE>
Map3D<RPM_BIN_SIZE, LOAD_BIN_SIZE>::Map3D() {
	initialized = 0;
	memset(&pointers, 0, sizeof(pointers));
}

template<int RPM_BIN_SIZE, int LOAD_BIN_SIZE>
void Map3D<RPM_BIN_SIZE, LOAD_BIN_SIZE>::setAll(float value) {
	efiAssertVoid(initialized == MAGIC_TRUE_VALUE, "map not initialized");
	for (int l = 0; l < LOAD_BIN_SIZE; l++) {
		for (int r = 0; r < RPM_BIN_SIZE; r++) {
			pointers[l][r] = value;
		}
	}
}

typedef Map3D<16, 16> Map3D1616;

void setTableBin(float array[], int size, float l, float r);
void setTableBin2(float array[], int size, float l, float r, float precision);
void setRpmTableBin(float array[], int size);

#endif /* TABLE_HELPER_H_ */
