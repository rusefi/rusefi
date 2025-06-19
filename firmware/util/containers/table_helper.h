/**
 * @file	table_helper.h
 *
 * @date Jul 6, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include <cmath>
#include "efi_interpolation.h"
#include "efilib.h"
#include "efi_ratio.h"
#include "efi_scaled_channel.h"
#include <rusefi/interpolation.h>

#if EFI_UNIT_TEST
#include <stdexcept>
#endif

// popular left edge of CLT-based correction curves
#define CLT_CURVE_RANGE_FROM -40

class ValueProvider3D {
public:
	virtual float getValue(float xColumn, float yRow) const = 0;
};


/**
 * this helper class brings together 3D table with two 2D axis curves
 * TODO: explicitly spell out why do we have this template and when exactly is it useful (useful with scaled content?)
 * todo: improve interpolate3d to handle scaling?
 * *** WARNING *** https://en.wikipedia.org/wiki/KISS_principle
 * *** WARNING *** this helper requires initialization, make sure that helper is useful any time you consider using it
 * *** WARNING *** we had too many bugs where we were not initializing, often just using the underlying interpolate3d is the way to go
 */
template<int TColNum, int TRowNum, typename TValue, typename TXColumn, typename TRow>
class Map3D : public ValueProvider3D {
public:
  Map3D(const char *name) {
    m_name = name;
  }
	template <typename TValueInit, typename TXColumnInit, typename TRowInit>
	void initTable(TValueInit (&table)[TRowNum][TColNum],
				  const TXColumnInit (&columnBins)[TColNum], const TRowInit (&rowBins)[TRowNum]) {
		// This splits out here so that we don't need one overload of init per possible combination of table/rows/columns types/dimensions
		// Overload resolution figures out the correct versions of the functions below to call, some of which have assertions about what's allowed
		initValues(table);
		initRows(rowBins);
		initCols(columnBins);
	}

  // RPM is usually X/Column
	float getValue(float xColumn, float yRow) const final {
		if (!m_values) {
			criticalError("Access to uninitialized table: %s", m_name);
			return 0;
		}

		return interpolate3d(*m_values,
								*m_rowBins, yRow * m_rowMult,
								*m_columnBins, xColumn * m_colMult) *
			m_valueMult;
	}

	void setAll(TValue value) {
		efiAssertVoid(ObdCode::CUSTOM_ERR_6573, m_values, "map not initialized");

		for (size_t r = 0; r < TRowNum; r++) {
			for (size_t c = 0; c < TColNum; c++) {
				(*m_values)[r][c] = value / m_valueMult;
			}
		}
	}

private:
	template <int TMult, int TDiv>
	void initValues(scaled_channel<TValue, TMult, TDiv> (&table)[TRowNum][TColNum]) {
		m_values = reinterpret_cast<TValue (*)[TRowNum][TColNum]>(&table);
		m_valueMult = 1 / efi::ratio<TMult, TDiv>::asFloat();
	}

	void initValues(TValue (&table)[TRowNum][TColNum]) {
		m_values = &table;
		m_valueMult = 1;
	}

	template <int TRowMult, int TRowDiv>
	void initRows(const scaled_channel<TRow, TRowMult, TRowDiv> (&rowBins)[TRowNum]) {
		m_rowBins = reinterpret_cast<const TRow (*)[TRowNum]>(&rowBins);
		m_rowMult = efi::ratio<TRowMult, TRowDiv>::asFloat();
	}

	void initRows(const TRow (&rowBins)[TRowNum]) {
		m_rowBins = &rowBins;
		m_rowMult = 1;
	}

	template <int TColMult, int TColDiv>
	void initCols(const scaled_channel<TXColumn, TColMult, TColDiv> (&columnBins)[TColNum]) {
		m_columnBins = reinterpret_cast<const TXColumn (*)[TColNum]>(&columnBins);
		m_colMult = efi::ratio<TColMult, TColDiv>::asFloat();
	}

	void initCols(const TXColumn (&columnBins)[TColNum]) {
		m_columnBins = &columnBins;
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
	/*const*/ TValue (*m_values)[TRowNum][TColNum] = nullptr;

	const TRow (*m_rowBins)[TRowNum] = nullptr;
	const TXColumn (*m_columnBins)[TColNum] = nullptr;

	float m_rowMult = 1;
	float m_colMult = 1;
	float m_valueMult = 1;
	const char *m_name;
};

typedef Map3D<VE_RPM_COUNT, VE_LOAD_COUNT, uint16_t, uint16_t, uint16_t> ve_Map3D_t;
typedef Map3D<PEDAL_TO_TPS_SIZE, PEDAL_TO_TPS_SIZE, uint8_t, uint8_t, uint8_t> pedal2tps_t;
typedef Map3D<MAP_EST_RPM_COUNT, MAP_EST_LOAD_COUNT, uint16_t, uint16_t, uint16_t> mapEstimate_Map3D_t;

/**
 * @param precision for example '0.1' for one digit fractional part. Default to 0.01, two digits.
 * see also: ensureArrayIsAscending
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
void setArrayValues(TValue (&array)[TSize], float value) {
	for (int i = 0; i < TSize; i++) {
		array[i] = value;
	}
}

template <typename TElement, typename VElement, size_t N, size_t M>
constexpr void setTable(TElement (&dest)[N][M], const VElement value) {
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

// specialization that can use memcpy when src and dest types match
template <typename TDest, size_t N, size_t M>
constexpr void copyTable(scaled_channel<TDest, 1, 1> (&dest)[N][M], const TDest (&source)[N][M]) {
	memcpy(dest, source, N * M * sizeof(TDest));
}

template <typename TDest, size_t N, size_t M>
constexpr void copyTable(TDest (&dest)[N][M], const TDest (&source)[N][M]) {
	memcpy(dest, source, N * M * sizeof(TDest));
}

template<typename kType>
void setRpmBin(kType array[], int size, float idleRpm, float topRpm) {
	array[0] = idleRpm - 150;
	int rpmStep = (int)(efiRound((topRpm - idleRpm) / (size - 2), 50) - 150);
	for (int i = 1; i < size - 1;i++)
		array[i] = idleRpm + rpmStep * (i - 1);
	array[size - 1] = topRpm;
}

/**
 * initialize RPM table axis using default RPM range
 */
template<typename TValue, int TSize>
void setRpmTableBin(TValue (&array)[TSize]) {
	setRpmBin(array, TSize, 800, DEFAULT_RPM_AXIS_HIGH_VALUE);
}
