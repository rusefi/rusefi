/**
 * @file	table_helper.h
 *
 * @date Jul 6, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include <math.h>
#include "error_handling.h"
#include "interpolation.h"
#include "efilib.h"

// popular left edge of CLT-based correction curves
#define CLT_CURVE_RANGE_FROM -40

class ValueProvider3D {
public:
	virtual float getValue(float xRpm, float y) const = 0;
};


/**
 * this helper class brings together 3D table with two 2D axis curves
 */
template<int RPM_BIN_SIZE, int LOAD_BIN_SIZE, typename vType, typename kType, int TValueDivisor>
class Map3D : public ValueProvider3D {
public:
	explicit Map3D(const char*name);
	void init(vType table[RPM_BIN_SIZE][LOAD_BIN_SIZE], const kType loadBins[LOAD_BIN_SIZE], const kType rpmBins[RPM_BIN_SIZE]);
	float getValue(float xRpm, float y) const override;
	void setAll(vType value);
	vType *pointers[LOAD_BIN_SIZE];
private:
	void create(const char*name);
	const kType *loadBins = NULL;
	const kType *rpmBins = NULL;
	bool initialized =  false;
	const char *name;
};

/*
 * this dead code is a questionable performance optimization idea: instead of division every time
 * we want interpolation for a curve we can pre-calculate A and B and save the division at the cost of more RAM usage
 * Realistically we probably value RAM over CPU at this time and the costs are not justified.
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
			warning(CUSTOM_INTEPOLATE_ERROR_4, "preCalc: Same x1 and x2 in interpolate: %.2f/%.2f", x1, x2);
			return;
		}

		float y1 = values[i];
		float y2 = values[i + 1];

		aTable[i] = INTERPOLATION_A(x1, y1, x2, y2);
		bTable[i] = y1 - aTable[i] * x1;
	}
}
*/

template<int RPM_BIN_SIZE, int LOAD_BIN_SIZE, typename vType, typename kType, int TValueDivisor>
void Map3D<RPM_BIN_SIZE, LOAD_BIN_SIZE, vType, kType, TValueDivisor>::init(vType table[RPM_BIN_SIZE][LOAD_BIN_SIZE],
		const kType loadBins[LOAD_BIN_SIZE],
		const kType rpmBins[RPM_BIN_SIZE]) {
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

template<int RPM_BIN_SIZE, int LOAD_BIN_SIZE, typename vType, typename kType, int TValueDivisor>
float Map3D<RPM_BIN_SIZE, LOAD_BIN_SIZE, vType, kType, TValueDivisor>::getValue(float xRpm, float y) const {
	efiAssert(CUSTOM_ERR_ASSERT, initialized, "map not initialized", NAN);
	if (cisnan(y)) {
		warning(CUSTOM_PARAM_RANGE, "%s: y is NaN", name);
		return NAN;
	}
	// todo: we have a bit of a mess: in TunerStudio, RPM is X-axis
	return interpolate3d<vType, kType>(y, loadBins, LOAD_BIN_SIZE, xRpm, rpmBins, RPM_BIN_SIZE, pointers) / (float)TValueDivisor;
}

template<int RPM_BIN_SIZE, int LOAD_BIN_SIZE, typename vType, typename kType, int TValueDivisor>
Map3D<RPM_BIN_SIZE, LOAD_BIN_SIZE, vType, kType, TValueDivisor>::Map3D(const char *name) {
	create(name);
}

template<int RPM_BIN_SIZE, int LOAD_BIN_SIZE, typename vType, typename kType, int TValueDivisor>
void Map3D<RPM_BIN_SIZE, LOAD_BIN_SIZE, vType, kType, TValueDivisor>::create(const char *name) {
	this->name = name;
	memset(&pointers, 0, sizeof(pointers));
}

template<int RPM_BIN_SIZE, int LOAD_BIN_SIZE, typename vType, typename kType, int TValueDivisor>
void Map3D<RPM_BIN_SIZE, LOAD_BIN_SIZE, vType, kType, TValueDivisor>::setAll(vType value) {
	efiAssertVoid(CUSTOM_ERR_6573, initialized, "map not initialized");
	for (int l = 0; l < LOAD_BIN_SIZE; l++) {
		for (int r = 0; r < RPM_BIN_SIZE; r++) {
			pointers[l][r] = value * TValueDivisor;
		}
	}
}

template<int RPM_BIN_SIZE, int LOAD_BIN_SIZE, typename vType, typename kType, int TValueDivisor>
void copy2DTable(const vType source[LOAD_BIN_SIZE][RPM_BIN_SIZE], vType destination[LOAD_BIN_SIZE][RPM_BIN_SIZE]) {
	for (int k = 0; k < LOAD_BIN_SIZE; k++) {
		for (int rpmIndex = 0; rpmIndex < RPM_BIN_SIZE; rpmIndex++) {
			destination[k][rpmIndex] = source[k][rpmIndex];
		}
	}
}

typedef Map3D<FUEL_RPM_COUNT, FUEL_LOAD_COUNT, uint8_t, float, PACK_MULT_AFR_CFG> afr_Map3D_t;
typedef Map3D<IGN_RPM_COUNT, IGN_LOAD_COUNT, float, float, 1> ign_Map3D_t;
typedef Map3D<FUEL_RPM_COUNT, FUEL_LOAD_COUNT, float, float, 1> fuel_Map3D_t;
typedef Map3D<BARO_CORR_SIZE, BARO_CORR_SIZE, float, float, 1> baroCorr_Map3D_t;
typedef Map3D<PEDAL_TO_TPS_SIZE, PEDAL_TO_TPS_SIZE, uint8_t, uint8_t, 1> pedal2tps_t;
typedef Map3D<BOOST_RPM_COUNT, BOOST_LOAD_COUNT, uint8_t, uint8_t, 1> boostOpenLoop_Map3D_t;
typedef Map3D<BOOST_RPM_COUNT, BOOST_LOAD_COUNT, uint8_t, uint8_t, 1> boostClosedLoop_Map3D_t;
typedef Map3D<IAC_PID_MULT_SIZE, IAC_PID_MULT_SIZE, uint8_t, uint8_t, 1> iacPidMultiplier_t;
typedef Map3D<GPPWM_RPM_COUNT, GPPWM_LOAD_COUNT, uint8_t, uint8_t, 1> gppwm_Map3D_t;

void setRpmBin(float array[], int size, float idleRpm, float topRpm);

/**
 * @param precision for example '0.1' for one digit fractional part. Default to 0.01, two digits.
 */
template<typename TValue, int TSize>
void setLinearCurve(TValue (&array)[TSize], float from, float to, float precision = 0.01f) {
	for (int i = 0; i < TSize; i++) {
		float value = interpolateMsg("setLinearCurve", 0, from, TSize - 1, to, i);

		/**
		 * rounded values look nicer, also we want to avoid precision mismatch with Tuner Studio
		 */
		array[i] = efiRound(value, precision);
	}
}

template<typename TValue, int TSize>
void setArrayValues(TValue (&array)[TSize], TValue value) {
	for (int i = 0; i < TSize; i++) {
		array[i] = value;
	}
}

void setRpmTableBin(float array[], int size);
