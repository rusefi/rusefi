/**
 * @file    interpolation.h
 *
 * @date Oct 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include <math.h>
#include "datalogging.h"
#include "obd_error_codes.h"

#include <type_traits>

#ifndef DEBUG_INTERPOLATION
#define DEBUG_INTERPOLATION FALSE
#endif

#define INTERPOLATION_A(x1, y1, x2, y2) ((y1 - y2) / (x1 - x2))

int findIndex(const float array[], int size, float value);
#define findIndexMsg(msg, array, size, value) findIndexMsgExt<float>(msg, array, size, value)
int findIndex2(const float array[], unsigned size, float value);
float interpolateClamped(float x1, float y1, float x2, float y2, float x);
float interpolateMsg(const char *msg, float x1, float y1, float x2, float y2, float x);

template<typename TValue, int TSize>
void ensureArrayIsAscending(const char* msg, const TValue (&values)[TSize]) {
	for (size_t i = 0; i < TSize - 1; i++) {
		auto cur = values[i];
		auto next = values[i + 1];
		if (next <= cur) {
			firmwareError(CUSTOM_ERR_AXIS_ORDER, "Invalid table axis (must be ascending!): %s %f %f at %d", msg, cur, next, i);
		}
	}
}

namespace priv {
struct BinResult
{
	size_t Idx;
	float Frac;
};

/**
 * @brief Finds the location of a value in the bin array.
 * 
 * @param value The value to find in the bins.
 * @return A result containing the index to the left of the value,
 * and how far from (idx) to (idx + 1) the value is located.
 */
template<class TBin, int TSize>
BinResult getBinPtr(float value, const TBin* bins) {
	// Enforce numeric only (int, float, uintx_t, etc)
	static_assert(std::is_arithmetic_v<TBin>, "Table bins must be an arithmetic type");

	// Enforce that there are enough bins to make sense (what does one bin even mean?)
	static_assert(TSize >= 2);

	// Handle NaN
	if (cisnan(value)) {
		return { 0, 0.0f };
	}

	// Handle off-scale low
	if (value <= bins[0]) {
		return { 0, 0.0f };
	}

	// Handle off-scale high
	if (value >= bins[TSize - 1]) {
		return { TSize - 2, 1.0f };
	}

	size_t idx = 0;

	// Find the last index less than the searched value
	// Linear search for now, maybe binary search in future
	// after collecting real perf data
	for (idx = 0; idx < TSize - 1; idx++) {
		if (bins[idx + 1] > value) {
			break;
		}
	}

	float low = bins[idx];
	float high = bins[idx + 1];

	// Compute how far along the bin we are
	// (0.0f = left side, 1.0f = right side)
	float fraction = (value - low) / (high - low);
	
	return { idx, fraction };
}

template<class TBin, int TSize>
BinResult getBin(float value, const TBin (&bins)[TSize]) {
	return getBinPtr<TBin, TSize>(value, &bins[0]);
}

static float linterp(float low, float high, float frac)
{
	return high * frac + low * (1 - frac);
}
} // namespace priv

template <class TBin, class TValue, int TSize>
float interpolate2d(const float value, const TBin (&bin)[TSize], const TValue (&values)[TSize]) {
	// Enforce numeric only (int, float, uintx_t, etc)
	static_assert(std::is_arithmetic_v<TBin>, "Table values must be an arithmetic type");

	auto b = priv::getBin(value, bin);

	// Convert to float as we read it out
	float low = static_cast<float>(values[b.Idx]);
	float high = static_cast<float>(values[b.Idx + 1]);
	float frac = b.Frac;

	return priv::linterp(low, high, frac);
}

/** @brief	Binary search
 * @returns	the highest index within sorted array such that array[i] is greater than or equal to the parameter
 * @note If the parameter is smaller than the first element of the array, -1 is returned.
 *
 * See also ensureArrayIsAscending
 */
template<typename kType>
int findIndexMsgExt(const char *msg, const kType array[], int size, kType value) {
	float fvalue = (float)value;
	if (cisnan(fvalue)) {
		firmwareError(ERROR_NAN_FIND_INDEX, "NaN in findIndex%s", msg);
		return 0;
	}

	if (value < array[0])
		return -1;
	int middle;

	int left = 0;
	int right = size;

	// todo: extract binary search as template method?
	while (true) {
#if 0
		// that's an assertion to make sure we do not loop here
		size--;
		efiAssert(CUSTOM_ERR_ASSERT, size > 0, "Unexpected state in binary search", 0);
#endif

		// todo: compare current implementation with
		// http://eigenjoy.com/2011/01/21/worlds-fastest-binary-search/
		// ?
		middle = (left + right) / 2;

//		print("left=%d middle=%d right=%d: %.2f\r\n", left, middle, right, array[middle]);

		if (middle == left)
			break;

		if (middle != 0 && array[middle - 1] > array[middle]) {
#if EFI_UNIT_TEST
			firmwareError(CUSTOM_ERR_6610, "%s: out of order %.2f %.2f", msg, array[middle - 1], array[middle]);
#else
			warning(CUSTOM_ERR_OUT_OF_ORDER, "%s: out of order %.2f %.2f", msg, array[middle - 1], array[middle]);

#endif /* EFI_UNIT_TEST */
		}

		if (value < array[middle]) {
			right = middle;
		} else if (value > array[middle]) {
			left = middle;
		} else {
			break;
		}
	}

	return middle;
}

void setCurveValue(float bins[], float values[], int size, float key, float value);
void initInterpolation();

class FastInterpolation {
public:
	FastInterpolation();
	FastInterpolation(float x1, float y1, float x2, float y2);
	void init(float x1, float y1, float x2, float y2);
	float getValue(float x) const;
private:
	float a, b;
};
