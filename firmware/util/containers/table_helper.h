/**
 * @file	table_helper.h
 *
 * @date Jul 6, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */
#ifndef TABLE_HELPER_H_
#define TABLE_HELPER_H_

#include "error_handling.h"
#include "interpolation.h"
#include "efilib.h"

#include <math.h>
#include <ratio>

// popular left edge of CLT-based correction curves
#define CLT_CURVE_RANGE_FROM -40

class ValueProvider3D {
public:
	virtual float getValue(float xRpm, float y) = 0;
};

/**
 * this helper class brings together 3D table with two 2D axis curves
 */
template <int TRpmBins, int TLoadBins, typename vType, typename kType, typename TStorageRatio = std::ratio<1>>
class Map3D : public ValueProvider3D {
public:
	explicit Map3D(const char *name) {
		this->name = name;
		memset(&pointers, 0, sizeof(pointers));
	}

	void init(vType table[TRpmBins][TLoadBins], const kType (&loadBins)[TLoadBins], const kType (&rpmBins)[TRpmBins]) {
		// this method cannot use logger because it's invoked before everything
		// that's because this method needs to be invoked before initial configuration processing
		// and initial configuration load is done prior to logging initialization

		for (int k = 0; k < TLoadBins; k++) {
			pointers[k] = table[k];
		}
		initialized = true;
		this->loadBins = loadBins;
		this->rpmBins = rpmBins;
	}

	float getValue(float xRpm, float y) {
		efiAssert(CUSTOM_ERR_ASSERT, initialized, "map not initialized", NAN);
		if (cisnan(y)) {
			warning(CUSTOM_PARAM_RANGE, "%s: y is NaN", name);
			return NAN;
		}
		// todo: we have a bit of a mess: in TunerStudio, RPM is X-axis
		return interpolate3d<TRpmBins, TLoadBins>(y, loadBins, xRpm, rpmBins, pointers) / GetStorageRatio();
	}

	void setAll(vType value) {
		efiAssertVoid(CUSTOM_ERR_6573, initialized, "map not initialized");

		for (int l = 0; l < TLoadBins; l++) {
			for (int r = 0; r < TRpmBins; r++) {
				pointers[l][r] = value * GetStorageRatio();
			}
		}
	}

	static constexpr float GetStorageRatio() {
		return static_cast<float>(TStorageRatio::num) / TStorageRatio::den;
	}

	vType *pointers[TLoadBins];

private:
	const kType *loadBins = nullptr;
	const kType *rpmBins = nullptr;

	bool initialized = false;
	const char *name;
};

template <int RPM_BIN_SIZE, int LOAD_BIN_SIZE, typename vType, typename kType>
void copy2DTable(const vType (&source)[LOAD_BIN_SIZE][RPM_BIN_SIZE],
				 vType (&destination)[LOAD_BIN_SIZE][RPM_BIN_SIZE]) {
	for (int k = 0; k < LOAD_BIN_SIZE; k++) {
		for (int rpmIndex = 0; rpmIndex < RPM_BIN_SIZE; rpmIndex++) {
			destination[k][rpmIndex] = source[k][rpmIndex];
		}
	}
}

// AFR value is packed into uint8_t with a multiplier of 10
typedef Map3D<FUEL_RPM_COUNT, FUEL_LOAD_COUNT, uint8_t, float, std::ratio<10>> afr_Map3D_t;
typedef Map3D<IGN_RPM_COUNT, IGN_LOAD_COUNT, float, float> ign_Map3D_t;
// TPS-based Advance value is packed into int16_t with a multiplier of 100
typedef Map3D<IGN_RPM_COUNT, IGN_TPS_COUNT, int16_t, float, std::ratio<100>> ign_tps_Map3D_t;
typedef Map3D<FUEL_RPM_COUNT, FUEL_LOAD_COUNT, float, float> fuel_Map3D_t;
typedef Map3D<BARO_CORR_SIZE, BARO_CORR_SIZE, float, float> baroCorr_Map3D_t;
typedef Map3D<PEDAL_TO_TPS_SIZE, PEDAL_TO_TPS_SIZE, uint8_t, uint8_t> pedal2tps_t;

void setRpmBin(float array[], int size, float idleRpm, float topRpm);

void setTableBin(float array[], int size, float from, float to);

#define setArrayValues(array, size, value) setTableBin(array, size, value, value)

/**
 * @param precision for example '0.1' for one digit fractional part
 */
template <typename vType>
void setLinearCurveAny(vType array[], int size, float from, float to, float precision) {
	for (int i = 0; i < size; i++) {
		float value = interpolateMsg("setLinearCurve", 0, from, size - 1, to, i);
		/**
		 * rounded values look nicer, also we want to avoid precision mismatch with Tuner Studio
		 */
		array[i] = efiRound(value, precision);
	}
}

#define setLinearCurve setLinearCurveAny<float>
void setRpmTableBin(float array[], int size);

#endif /* TABLE_HELPER_H_ */
