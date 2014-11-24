/**
 * @file	table_helper.h
 *
 * @date Jul 6, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
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
	void init(float table[RPM_BIN_SIZE][LOAD_BIN_SIZE]);
	float getValue(float x, float xBin[], float y, float yBin[]);
	void setAll(float value);
private:
	float *pointers[LOAD_BIN_SIZE];
	int initialized;
};

class Table2D {
public:
	void init(int size, float *aTable, float *bTable);
	void preCalc(float *bin, float *values);
	int size;
	float *aTable;
	float *bTable;
};

template<int RPM_BIN_SIZE, int LOAD_BIN_SIZE>
void Map3D<RPM_BIN_SIZE, LOAD_BIN_SIZE>::init(float table[RPM_BIN_SIZE][LOAD_BIN_SIZE]) {
  for (int k = 0; k < LOAD_BIN_SIZE; k++) {
		pointers[k] = table[k];
  }
	initialized = MAGIC_TRUE_VALUE;
}

template<int RPM_BIN_SIZE, int LOAD_BIN_SIZE>
float Map3D<RPM_BIN_SIZE, LOAD_BIN_SIZE>::getValue(float x, float xBin[], float y, float yBin[]) {
	efiAssert(initialized == MAGIC_TRUE_VALUE, "map not initialized", NAN);
	return interpolate3d(x, xBin, LOAD_BIN_SIZE, y, yBin, RPM_BIN_SIZE, pointers);
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
