/**
 * @file	table_helper.h
 *
 * @date Jul 6, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include <math.h>
#include "interpolation.h"
#include "efilib.h"
#include "efi_ratio.h"
#include "scaled_channel.h"

// popular left edge of CLT-based correction curves
#define CLT_CURVE_RANGE_FROM -40

class ValueProvider3D {
public:
	virtual float getValue(float xColumn, float yRow) const = 0;
};


/**
 * this helper class brings together 3D table with two 2D axis curves
 */
template<int TColNum, int TRowNum, typename TValue, typename TColumn, typename TRow, typename TValueMultiplier = efi::ratio<1>>
class Map3D : public ValueProvider3D {
public:
	template <typename TValueInit, typename TRowInit, typename TColumnInit>
	void init(TValueInit table[TRowNum][TColNum], const TRowInit rowBins[TRowNum], const TColumnInit columnBins[TColNum]) {
		// This splits out here so that we don't need one overload of init per possible combination of table/rows/columns types/dimensions
		// Overload resolution figures out the correct versions of the functions below to call, some of which have assertions about what's allowed
		initValues(table);
		initRows(rowBins);
		initCols(columnBins);
	}

	float getValue(float xColumn, float yRow) const override {
		if (!m_values) {
			// not initialized, return 0
			return 0;
		}
		
		auto row = priv::getBinPtr<TRow, TRowNum>(yRow * m_rowMult, m_rowBins);
		auto col = priv::getBinPtr<TColumn, TColNum>(xColumn * m_colMult, m_columnBins);

		// Orient the table such that (0, 0) is the bottom left corner,
		// then the following variable names will make sense
		float lowerLeft = getValueAtPosition(row.Idx, col.Idx);
		float upperLeft = getValueAtPosition(row.Idx + 1, col.Idx);
		float lowerRight = getValueAtPosition(row.Idx, col.Idx + 1);
		float upperRight = getValueAtPosition(row.Idx + 1, col.Idx + 1);

		// Interpolate each side by itself
		float left = priv::linterp(lowerLeft, upperLeft, row.Frac);
		float right = priv::linterp(lowerRight, upperRight, row.Frac);

		// Then interpolate between those
		float tableValue = priv::linterp(left, right, col.Frac);

		// Correct by the ratio of table units to "world" units
		return tableValue * TValueMultiplier::asFloat();
	}

	void setAll(TValue value) {
		efiAssertVoid(CUSTOM_ERR_6573, m_values, "map not initialized");

		for (size_t i = 0; i < TRowNum * TColNum; i++) {
			m_values[i] = value / TValueMultiplier::asFloat();
		}
	}

private:
	template <int TMult>
	void initValues(scaled_channel<TValue, TMult> table[TRowNum][TColNum]) {
		static_assert(TValueMultiplier::den == TMult);
		static_assert(TValueMultiplier::num == 1);

		m_values = reinterpret_cast<TValue*>(&table[0][0]);
	}

	void initValues(TValue table[TRowNum][TColNum]) {
		m_values = &table[0][0];
	}

	template <int TRowMult>
	void initRows(const scaled_channel<TRow, TRowMult> rowBins[TRowNum]) {
		m_rowBins = reinterpret_cast<const TRow*>(&rowBins[0]);
		m_rowMult = TRowMult;
	}

	void initRows(const TRow rowBins[TRowNum]) {
		m_rowBins = &rowBins[0];
		m_rowMult = 1;
	}

	template <int TColMult>
	void initCols(const scaled_channel<TColumn, TColMult> columnBins[TColNum]) {
		m_columnBins = reinterpret_cast<const TColumn*>(&columnBins[0]);
		m_colMult = TColMult;
	}

	void initCols(const TColumn columnBins[TColNum]) {
		m_columnBins = &columnBins[0];
		m_colMult = 1;
	}

	static size_t getIndexForCoordinates(size_t row, size_t column) {
		// Index 0 is bottom left corner
		// Index TColNum - 1 is bottom right corner
		// indicies count right, then up
		return row * TColNum + column;
	}

	TValue getValueAtPosition(size_t row, size_t column) const {
		auto idx = getIndexForCoordinates(row, column);
		return m_values[idx];
	}

	// TODO: should be const
	/*const*/ TValue* m_values = nullptr;

	const TRow *m_rowBins = nullptr;
	const TColumn *m_columnBins = nullptr;

	float m_rowMult = 1;
	float m_colMult = 1;
};

typedef Map3D<FUEL_RPM_COUNT, FUEL_LOAD_COUNT, uint8_t, float, float, efi::ratio<1, PACK_MULT_LAMBDA_CFG>> lambda_Map3D_t;
typedef Map3D<IGN_RPM_COUNT, IGN_LOAD_COUNT, float, float, float> ign_Map3D_t;
typedef Map3D<FUEL_RPM_COUNT, FUEL_LOAD_COUNT, float, float, float> fuel_Map3D_t;
typedef Map3D<BARO_CORR_SIZE, BARO_CORR_SIZE, float, float, float> baroCorr_Map3D_t;
typedef Map3D<PEDAL_TO_TPS_SIZE, PEDAL_TO_TPS_SIZE, uint8_t, uint8_t, uint8_t> pedal2tps_t;
typedef Map3D<BOOST_RPM_COUNT, BOOST_LOAD_COUNT, uint8_t, uint8_t, uint8_t, efi::ratio<LOAD_1_BYTE_PACKING_MULT>> boostOpenLoop_Map3D_t;
typedef Map3D<BOOST_RPM_COUNT, BOOST_LOAD_COUNT, uint8_t, uint8_t, uint8_t> boostClosedLoop_Map3D_t;
typedef Map3D<IAC_PID_MULT_SIZE, IAC_PID_MULT_SIZE, uint8_t, uint8_t, uint8_t> iacPidMultiplier_t;
typedef Map3D<GPPWM_RPM_COUNT, GPPWM_LOAD_COUNT, uint8_t, uint8_t, uint8_t> gppwm_Map3D_t;
typedef Map3D<FUEL_RPM_COUNT, FUEL_LOAD_COUNT, uint16_t, uint16_t, uint16_t, efi::ratio<1, PACK_MULT_MAP_ESTIMATE>> mapEstimate_Map3D_t;

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

template <typename TElement, size_t N, size_t M, int mult = 1>
constexpr void setTable(scaled_channel<TElement, mult> (&dest)[N][M], float value) {
	for (size_t n = 0; n < N; n++) {
		for (size_t m = 0; m < M; m++) {
			dest[n][m] = value;
		}
	}
}

template <typename TDest, typename TSource, size_t N, size_t M>
constexpr void copyTable(TDest (&dest)[N][M], const TSource (&source)[N][M], float multiply = 1.0f) {
	for (size_t n = 0; n < N; n++) {
		for (size_t m = 0; m < M; m++) {
			dest[n][m] = source[n][m] * multiply;
		}
	}
}

void setRpmTableBin(float array[], int size);
