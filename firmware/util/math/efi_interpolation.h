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
 * This is the validation half of that rule; it does not repair anything. It is called for each
 * axis in use from validateConfig() in engine_controller.cpp, on start-up and on every burn.
 * @param msg names the axis in the user-facing message, see [tag:user_facing_ini_labels]
 *
 * Severity: a broken axis is reported with firmwareError(), the critical kind described in
 * error_handling.h, although its cause is a tune the user could fix in TunerStudio, which is
 * what configError() is for. Critical is used because it is the one report which also stops
 * the engine: LimpManager::fatalError() cuts fuel, spark and ETB, and nothing consults
 * hasConfigError() before firing outputs. The price is that the error stays latched until
 * reboot, so fixing the axis and burning is not enough.
 * Reporting this as a config error needs all of the following, not just a different call:
 *  - a level-triggered producer in refreshConfigErrorState(), set and cleared by each validation
 *  - an engine cut in LimpManager::updateState() for as long as the axis is broken
 *  - a decision for the tables which are read outside of fuel and spark (ETB pedal map,
 *    boost, VVT, idle), which a fuel/spark cut does not stop
 *
 * Returns false after reporting the first broken pair. In unit tests and the simulator
 * firmwareError() throws (see EXPECT_FATAL_ERROR).
 * This header is in util/ and must not depend on controllers/: the report goes through
 * error_handling.h, state about the tune belongs to the caller.
 *
 * see also: setLinearCurve
 */
template<typename TValue, int TSize>
bool ensureArrayIsAscending(const char* msg, const TValue (&values)[TSize]) {
	for (int i = 0; i < TSize - 1; i++) {
		float cur = values[i];
		float next = values[i + 1];
		if (!(cur < next) || !std::isfinite(cur) || !std::isfinite(next)) {
			firmwareError(ObdCode::CUSTOM_ERR_AXIS_ORDER, "Broken axis (must be ascending!): %s %.3f should be below next value %.3f at index %d", msg, cur, next, i);
			return false;
		}
	}
	return true;
}

/**
 * [tag:table_axis_order] For axes of optional features: an axis which was never filled in
 * (all zeros, recognized by its second element) is accepted, since the feature is not in use.
 * This is also what lets a tune made before the table existed pass validation. Use the strict
 * ensureArrayIsAscending() for an axis the engine reads regardless of optional settings.
 */
template<typename TValue, int TSize>
bool ensureArrayIsAscendingOrDefault(const char* msg, const TValue (&values)[TSize]) {
	if (values[1] == 0) {
		return true; // looks like default empty array, do not check
	}
	return ensureArrayIsAscending(msg, values);
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
