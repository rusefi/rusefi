/**
 * @file    interpolation.cpp
 * @brief	Linear interpolation algorithms
 *
 * See test_interpolation_3d.cpp
 *
 *
 * @date Oct 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 * @author Dmitry Sidin, (c) 2015
 */

#include "pch.h"

#include "efi_interpolation.h"

/** @brief	Linear interpolation by two points
 *
 * @param	x1 key of the first point
 * @param	y1 value of the first point
 * @param	x2 key of the second point
 * @param	y2 value of the second point
 * @param	X key to be interpolated
 *
 * @note	For example, "interpolateMsg("", engineConfiguration.tpsMin, 0, engineConfiguration.tpsMax, 100, adc);"
 * @see interpolateClamped
 */
float interpolateMsg(const char *msg, float x1, float y1, float x2, float y2, float x) {
	if (std::isnan(x1) || std::isnan(x2) || std::isnan(y1) || std::isnan(y2)) {
		warning(ObdCode::CUSTOM_ERR_INTERPOLATE_1, "interpolate%s: why param", msg);
		return NAN;
	}
	if (std::isnan(x)) {
		warning(ObdCode::CUSTOM_ERR_INTERPOLATE_2, "interpolate%s: why X", msg);
		return NAN;
	}
	// todo: double comparison using EPS
	if (x1 == x2) {
		/**
		 * we could end up here for example while resetting bins while changing engine type
		 */
		warning(ObdCode::CUSTOM_ERR_INTERPOLATE_3, "interpolate%s: Same x1 and x2 in interpolate: %.2f/%.2f", msg, x1, x2);
		return NAN;
	}

	// a*x1 + b = y1
	// a*x2 + b = y2
//	efiAssertVoid(ObdCode::CUSTOM_ERR_ASSERT_VOID, x1 != x2, "no way we can interpolate");
	float a = INTERPOLATION_A(x1, y1, x2, y2);
	if (std::isnan(a)) {
		warning(ObdCode::CUSTOM_ERR_INTERPOLATE_4, "interpolate%s: why a", msg);
		return NAN;
	}
	float b = y1 - a * x1;
	float result = a * x + b;

	return result;
}

/**
 * @see interpolateMsg
 */
float interpolateClamped(float x1, float y1, float x2, float y2, float x) {
	if (x <= x1)
		return y1;
	if (x >= x2)
		return y2;

	float a = INTERPOLATION_A(x1, y1, x2, y2);
	float b = y1 - a * x1;
	return a * x + b;
}
