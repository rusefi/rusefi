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
int findIndex2(const float array[], unsigned size, float value);
float interpolateClamped(float x1, float y1, float x2, float y2, float x);
float interpolateMsg(const char *msg, float x1, float y1, float x2, float y2, float x);

// _technically_ and _theoretically_ we can support flat line for both bins and values but I am not sure if
// such a rare case is something we want to support
template<typename TValue, int TSize>
void ensureArrayIsAscending(const char* msg, const TValue (&values)[TSize]) {
	for (size_t i = 0; i < TSize - 1; i++) {
		float cur = values[i];
		float next = values[i + 1];
		if (next <= cur) {
			firmwareError(CUSTOM_ERR_AXIS_ORDER, "Invalid table axis (must be ascending!): %s %f %f at %d", msg, cur, next, i);
		}
	}
}

template<typename TValue, int TSize>
void ensureArrayIsAscendingOrDefault(const char* msg, const TValue (&values)[TSize]) {
	if (values[1] == 0) {
		return; // looks like default empty array, do not check
	}
	ensureArrayIsAscending(msg, values);
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

#define findIndexMsg(msg, array, size, value) findIndexMsgExt(msg, array, size, value)

/**
 * Sets specified value for specified key in a correction curve
 * see also setLinearCurve()
 */
template<typename VType, typename kType>
void setCurveValue(const kType bins[], VType values[], int size, float key, float value) {
	int index = findIndexMsg("tbVl", bins, size, key);
	if (index == -1)
		index = 0;
	values[index] = value;
}

void initInterpolation();
