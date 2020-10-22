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
#include "efi_ratio.h"

// popular left edge of CLT-based correction curves
#define CLT_CURVE_RANGE_FROM -40

class ValueProvider3D {
public:
	virtual float getValue(float xRpm, float y) const = 0;
};


/**
 * this helper class brings together 3D table with two 2D axis curves
 */
template<int RPM_BIN_SIZE, int LOAD_BIN_SIZE, typename vType, typename kType, typename TValueMultiplier = efi::ratio<1>>
class Map3D : public ValueProvider3D {
public:
	explicit Map3D(const char*name) {
		create(name);
	}

	void init(vType table[RPM_BIN_SIZE][LOAD_BIN_SIZE], const kType loadBins[LOAD_BIN_SIZE], const kType rpmBins[RPM_BIN_SIZE]) {
		// this method cannot use logger because it's invoked before everything
		// that's because this method needs to be invoked before initial configuration processing
		// and initial configuration load is done prior to logging initialization
		for (int k = 0; k < LOAD_BIN_SIZE; k++) {
			pointers[k] = table[k];
		}

		this->loadBins = loadBins;
		this->rpmBins = rpmBins;
	}

	float getValue(float xRpm, float y) const override {
		efiAssert(CUSTOM_ERR_ASSERT, loadBins, "map not initialized", NAN);
		if (cisnan(y)) {
			warning(CUSTOM_PARAM_RANGE, "%s: y is NaN", name);
			return NAN;
		}

		// todo: we have a bit of a mess: in TunerStudio, RPM is X-axis
		return interpolate3d<vType, kType>(y, loadBins, LOAD_BIN_SIZE, xRpm, rpmBins, RPM_BIN_SIZE, pointers) * TValueMultiplier::asFloat();
	}

	void setAll(vType value) {
		efiAssertVoid(CUSTOM_ERR_6573, loadBins, "map not initialized");
		for (int l = 0; l < LOAD_BIN_SIZE; l++) {
			for (int r = 0; r < RPM_BIN_SIZE; r++) {
				pointers[l][r] = value / TValueMultiplier::asFloat();
			}
		}
	}

	vType *pointers[LOAD_BIN_SIZE];
private:
	void create(const char* name) {
		this->name = name;
		memset(&pointers, 0, sizeof(pointers));
	}

	const kType *loadBins = NULL;
	const kType *rpmBins = NULL;
	const char *name;
};

template<int RPM_BIN_SIZE, int LOAD_BIN_SIZE, typename vType, typename kType, typename TValueMultiplier = efi::ratio<1>>
void copy2DTable(const vType source[LOAD_BIN_SIZE][RPM_BIN_SIZE], vType destination[LOAD_BIN_SIZE][RPM_BIN_SIZE]) {
	for (int k = 0; k < LOAD_BIN_SIZE; k++) {
		for (int rpmIndex = 0; rpmIndex < RPM_BIN_SIZE; rpmIndex++) {
			destination[k][rpmIndex] = source[k][rpmIndex];
		}
	}
}

typedef Map3D<FUEL_RPM_COUNT, FUEL_LOAD_COUNT, uint8_t, float, efi::ratio<1, PACK_MULT_AFR_CFG>> afr_Map3D_t;
typedef Map3D<IGN_RPM_COUNT, IGN_LOAD_COUNT, float, float> ign_Map3D_t;
typedef Map3D<FUEL_RPM_COUNT, FUEL_LOAD_COUNT, float, float> fuel_Map3D_t;
typedef Map3D<BARO_CORR_SIZE, BARO_CORR_SIZE, float, float> baroCorr_Map3D_t;
typedef Map3D<PEDAL_TO_TPS_SIZE, PEDAL_TO_TPS_SIZE, uint8_t, uint8_t> pedal2tps_t;
typedef Map3D<BOOST_RPM_COUNT, BOOST_LOAD_COUNT, uint8_t, uint8_t, efi::ratio<LOAD_1_BYTE_PACKING_MULT>> boostOpenLoop_Map3D_t;
typedef Map3D<BOOST_RPM_COUNT, BOOST_LOAD_COUNT, uint8_t, uint8_t> boostClosedLoop_Map3D_t;
typedef Map3D<IAC_PID_MULT_SIZE, IAC_PID_MULT_SIZE, uint8_t, uint8_t> iacPidMultiplier_t;
typedef Map3D<GPPWM_RPM_COUNT, GPPWM_LOAD_COUNT, uint8_t, uint8_t> gppwm_Map3D_t;

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

template <typename TElement, size_t N, size_t M>
constexpr void setTable(TElement (&dest)[N][M], const TElement value) {
	for (size_t n = 0; n < N; n++) {
		for (size_t m = 0; m < M; m++) {
			dest[n][m] = value;
		}
	}
}

void setRpmTableBin(float array[], int size);
