/**
 * @file    efi_interpolation.h
 * See also libfirmware interpolation.h
 *
 * @date Oct 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include <cmath>
#include "datalogging.h"
#include "obd_error_codes.h"
#include "error_handling.h"

#include <type_traits>

#ifndef DEBUG_INTERPOLATION
#define DEBUG_INTERPOLATION FALSE
#endif

#define INTERPOLATION_A(x1, y1, x2, y2) ((y1 - y2) / (x1 - x2))

float interpolateClampedWithValidation(float x1, float y1, float x2, float y2, float x);
float interpolateClamped(float x1, float y1, float x2, float y2, float x);
float interpolateMsg(const char *msg, float x1, float y1, float x2, float y2, float x);

/**
 * [tag:table_axis_order] Table and curve axes ("bins") must be strictly ascending.
 *
 * Why: lookups (findIndexMsg(), priv::getBin(), interpolate2d/3d) search the bins assuming
 * that order and divide by the distance between neighbours. Equal neighbours (a "flat line",
 * for example an axis of zeros from a tune made for another firmware version) or a descending
 * pair give a zero or negative interval, so the looked-up fuel/timing value is not usable.
 * _technically_ and _theoretically_ we can support flat line for both bins and values but
 * I am not sure if such a rare case is something we want to support.
 *
 * This is the check alone: it reports nothing and repairs nothing. This header is in util/
 * and must not depend on controllers/, and state about the tune belongs to the caller: see
 * ensureArrayIsAscending() next to validateConfig() in engine_controller.cpp for how a broken
 * axis is reported and what it does to the engine.
 *
 * @return index of the first element which is not below its successor, -1 for a good axis
 * see also: setLinearCurve
 */
template<typename TValue, int TSize>
int findBrokenAxisIndex(const TValue (&values)[TSize]) {
	for (int i = 0; i < TSize - 1; i++) {
		// compared as float, the type the lookups interpolate in
		float cur = values[i];
		float next = values[i + 1];
		if (next <= cur) {
			return i;
		}
	}
	return -1;
}

/** @brief	Binary search
 * @returns	the highest index within sorted array such that array[i] is greater than or equal to the parameter
 * @note If the parameter is smaller than the first element of the array, -1 is returned.
 *
 * See also ensureArrayIsAscending
 */
template<typename kType>
int findIndexMsg(const char *msg, const kType array[], int size, kType value) {
	float fvalue = (float)value;
	if (std::isnan(fvalue)) {
		firmwareError(ObdCode::ERROR_NAN_FIND_INDEX, "NaN in findIndex%s", msg);
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
		efiAssert(ObdCode::CUSTOM_ERR_ASSERT, size > 0, "Unexpected state in binary search", 0);
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
			firmwareError(ObdCode::CUSTOM_ERR_6610, "%s: out of order %.2f %.2f", msg, array[middle - 1], array[middle]);
#else
			warning(ObdCode::CUSTOM_ERR_OUT_OF_ORDER, "%s: out of order %.2f %.2f", msg, array[middle - 1], array[middle]);

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
