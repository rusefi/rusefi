/**
 * @file	table_helper.h
 *
 * @date Jul 6, 2014
 * @author Andrey Belomutskiy, (c) 2012-2016
 */
#ifndef TABLE_HELPER_H_
#define TABLE_HELPER_H_

#include <math.h>
#include "error_handling.h"
#include "interpolation.h"
#include "efilib.h"

/**
 * this helper class brings together 3D table with two 2D axis curves
 */
template<int RPM_BIN_SIZE, int LOAD_BIN_SIZE, typename vType>
class Map3D {
public:
	Map3D(const char*name);
	Map3D(const char*name, float multiplier);
	void create(const char*name, float multiplier);
	void init(vType table[RPM_BIN_SIZE][LOAD_BIN_SIZE], float loadBins[LOAD_BIN_SIZE], float rpmBins[RPM_BIN_SIZE]);
	float getValue(float xRpm, float y);
	void setAll(vType value);
private:
	vType *pointers[LOAD_BIN_SIZE];
	float *loadBins;
	float *rpmBins;
	bool initialized;
	const char *name;
	float multiplier;
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


template<int RPM_BIN_SIZE, int LOAD_BIN_SIZE, typename vType>
void Map3D<RPM_BIN_SIZE, LOAD_BIN_SIZE, vType>::init(vType table[RPM_BIN_SIZE][LOAD_BIN_SIZE],
		float loadBins[LOAD_BIN_SIZE],
		float rpmBins[RPM_BIN_SIZE]) {
	// this method cannot use logger because it's invoked before everything
	// that's because this method needs to be invoked before initial configuration processing
	// and initial configuration load is done prior to logging initialization

  for (int k = 0; k < LOAD_BIN_SIZE; k++) {
		pointers[k] = table[k];
  }
	initialized = true;
	this->loadBins = loadBins;
	this->rpmBins = rpmBins;
}

template<int RPM_BIN_SIZE, int LOAD_BIN_SIZE, typename vType>
float Map3D<RPM_BIN_SIZE, LOAD_BIN_SIZE, vType>::getValue(float xRpm, float y) {
	efiAssert(initialized, "map not initialized", NAN);
	if (cisnan(y)) {
		warning(OBD_PCM_Processor_Fault, "%s: y is NaN", name);
		return NAN;
	}
	// todo: we have a bit of a mess: in TunerStudio, RPM is X-axis
	return multiplier * interpolate3d<vType>(y, loadBins, LOAD_BIN_SIZE, xRpm, rpmBins, RPM_BIN_SIZE, pointers);
}

template<int RPM_BIN_SIZE, int LOAD_BIN_SIZE, typename vType>
Map3D<RPM_BIN_SIZE, LOAD_BIN_SIZE, vType>::Map3D(const char *name) {
	create(name, 1);
}

template<int RPM_BIN_SIZE, int LOAD_BIN_SIZE, typename vType>
Map3D<RPM_BIN_SIZE, LOAD_BIN_SIZE, vType>::Map3D(const char *name, float multiplier) {
	create(name, multiplier);
}

template<int RPM_BIN_SIZE, int LOAD_BIN_SIZE, typename vType>
void Map3D<RPM_BIN_SIZE, LOAD_BIN_SIZE, vType>::create(const char *name, float multiplier) {
	this->name = name;
	this->multiplier = multiplier;
	initialized = false;
	memset(&pointers, 0, sizeof(pointers));
	loadBins = NULL;
	rpmBins = NULL;
}

template<int RPM_BIN_SIZE, int LOAD_BIN_SIZE, typename vType>
void Map3D<RPM_BIN_SIZE, LOAD_BIN_SIZE, vType>::setAll(vType value) {
	efiAssertVoid(initialized, "map not initialized");
	for (int l = 0; l < LOAD_BIN_SIZE; l++) {
		for (int r = 0; r < RPM_BIN_SIZE; r++) {
			pointers[l][r] = value / multiplier;
		}
	}
}

typedef Map3D<FUEL_RPM_COUNT, FUEL_LOAD_COUNT, float> afr_Map3D_t;
typedef Map3D<IGN_RPM_COUNT, IGN_LOAD_COUNT, float> ign_Map3D_t;
typedef Map3D<FUEL_RPM_COUNT, FUEL_LOAD_COUNT, float> fuel_Map3D_t;
typedef Map3D<BARO_CORR_SIZE, BARO_CORR_SIZE, float> baroCorr_Map3D_t;

void setRpmBin(float array[], int size, float idleRpm, float topRpm);

void setTableBin(float array[], int size, float l, float r);
void setTableBin2(float array[], int size, float l, float r, float precision);
void setRpmTableBin(float array[], int size);

#endif /* TABLE_HELPER_H_ */
