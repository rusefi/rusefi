/**
 * @file    interpolation.cpp
 * @brief	Linear interpolation algorithms
 *
 * See test_interpolation_3d.cpp
 *
 *
 * @date Oct 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 * @author Dmitry Sidin, (c) 2015
 */

#include "interpolation.h"

#include "global.h"

#include <math.h>
#include "efilib.h"
#include "efilib2.h"
#include "obd_error_codes.h"
#include "error_handling.h"

bool needInterpolationLoggingValue = true;

int needInterpolationLogging(void) {
	return needInterpolationLoggingValue;
}

#define BINARY_PERF true

Logging * logger;

#if BINARY_PERF && ! EFI_UNIT_TEST

#define COUNT 10000

float array16[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

static void testBinary(void) {
	const int size16 = 16;

	uint32_t totalOld = 0;
	uint32_t totalNew = 0;

	for (int v = 0; v <= 16; v++) {
		uint32_t timeOld;
		{
			uint32_t start = GET_TIMESTAMP();
			int temp = 0;
			for (int i = 0; i < COUNT; i++) {
				temp += findIndex(array16, size16, v);
			}
			timeOld = GET_TIMESTAMP() - start;
		}
		uint32_t timeNew;
		{
			uint32_t start = GET_TIMESTAMP();
			int temp = 0;
			for (int i = 0; i < COUNT; i++) {
				temp += findIndex2(array16, size16, v);
			}
			timeNew = GET_TIMESTAMP() - start;
		}
		scheduleMsg(logger, "for v=%d old=%d ticks", v, timeOld);
		scheduleMsg(logger, "for v=%d new=%d ticks", v, timeNew);

		totalOld += timeOld;
		totalNew += timeNew;
	}
	scheduleMsg(logger, "totalOld=%d ticks", totalOld);
	scheduleMsg(logger, "totalNew=%d ticks", totalNew);

}

#endif

FastInterpolation::FastInterpolation() {
	init(0, 0, 1, 1);
}

FastInterpolation::FastInterpolation(float x1, float y1, float x2, float y2) {
	init(x1, y1, x2, y2);
}

void FastInterpolation::init(float x1, float y1, float x2, float y2) {
	if (x1 == x2) {
		firmwareError(CUSTOM_ERR_INTERPOLATE, "init: Same x1 and x2 in interpolate: %.2f/%.2f", x1, x2);
		return;
	}
	a = INTERPOLATION_A(x1, y1, x2, y2);
	b = y1 - a * x1;
}

float FastInterpolation::getValue(float x) {
	return a * x + b;
}

/** @brief	Linear interpolation by two points
 *
 * @param	x1 key of the first point
 * @param	y1 value of the first point
 * @param	x2 key of the second point
 * @param	y2 value of the second point
 * @param	X key to be interpolated
 *
 * @note	For example, "interpolateMsg("", engineConfiguration.tpsMin, 0, engineConfiguration.tpsMax, 100, adc);"
 */
float interpolateMsg(const char *msg, float x1, float y1, float x2, float y2, float x) {
	// todo: double comparison using EPS
	if (x1 == x2) {
		/**
		 * we could end up here for example while resetting bins while changing engine type
		 */
		warning(CUSTOM_INTEPOLATE_ERROR, "interpolate%s: Same x1 and x2 in interpolate: %.2f/%.2f", msg, x1, x2);
		return NAN;
	}

	// a*x1 + b = y1
	// a*x2 + b = y2
//	efiAssertVoid(CUSTOM_ERR_ASSERT_VOID, x1 != x2, "no way we can interpolate");
	float a = INTERPOLATION_A(x1, y1, x2, y2);
	float b = y1 - a * x1;
	float result = a * x + b;
#if	DEBUG_FUEL
	printf("x1=%.2f y1=%.2f x2=%.2f y2=%.2f\r\n", x1, y1, x2, y2);
	printf("a=%.2f b=%.2f result=%.2f\r\n", a, b, result);
#endif
	return result;
}

float interpolateClamped(float x1, float y1, float x2, float y2, float x) {
	if (x <= x1)
		return y1;
	if (x >= x2)
		return y2;

	float a = INTERPOLATION_A(x1, y1, x2, y2);
	float b = y1 - a * x1;
	return a * x + b;
}

/**
 * Another implementation, which one is faster?
 */
int findIndex2(const float array[], unsigned size, float value) {
	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(value), "NaN in findIndex2", 0);
	efiAssert(CUSTOM_ERR_ASSERT, size > 1, "size in findIndex", 0);
//	if (size <= 1)
//		return size && *array <= value ? 0 : -1;

	signed i = 0;
	//unsigned b = 1 << int(log(float(size) - 1) / 0.69314718055994530942);
	unsigned b = size >> 1; // in our case size is always a power of 2
	efiAssert(CUSTOM_ERR_ASSERT, b + b == size, "Size not power of 2", -1);
	for (; b; b >>= 1) {
		unsigned j = i | b;
		/**
		 * it should be
		 * "if (j < size && array[j] <= value)"
		 * but in our case size is always power of 2 thus size is always more then j
		 */
		// efiAssert(CUSTOM_ERR_ASSERT, j < size, "size", 0);
		if (array[j] <= value)
			i = j;
	}
	return i || *array <= value ? i : -1;
}

/**
 * in order to use binary search we need to know that axis elements are sorted
 */
void ensureArrayIsAscending(const char *msg, const float array[], int size) {
	for (int i = 0; i < size - 1; i ++) {
		if (array[i] >= array[i+ 1]) {
			// todo: this should become a warning under https://github.com/rusefi/rusefi/issues/440
			firmwareError(CUSTOM_ERR_AXIS_ORDER, "invalid axis %s at %.2f", msg, array[i]);
		}
	}
}

/** @brief	Binary search
 * @returns	the highest index within sorted array such that array[i] is greater than or equal to the parameter
 * @note If the parameter is smaller than the first element of the array, -1 is returned.
 *
 * See also ensureArrayIsAscending
 */
int findIndexMsg(const char *msg, const float array[], int size, float value) {
	if (cisnan(value)) {
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
#if EFI_UNIT_TEST || defined(__DOXYGEN__)
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

int findIndex(const float array[], int size, float value) {
	return findIndexMsg("", array, size, value);
}

/**
 * @brief	One-dimensional table lookup with linear interpolation
 * 
 * Deprecated in favor of new Table2d<TBin, TValue> class.
 */
float interpolate2d_DEPRECATED(const char *msg, float value, float bin[], float values[], int size) {
	if (isnan(value)) {
		firmwareError(CUSTOM_INTERPOLATE_NAN, "NaN in interpolate2d %s", msg);
		return NAN;
	}
	int index = findIndexMsg("value", bin, size, value);

	if (index == -1)
		return values[0];
	if (index == size - 1)
		return values[size - 1];

	return interpolateMsg("2d", bin[index], values[index], bin[index + 1], values[index + 1], value);
}

template <typename vType>
float interpolate3d(float x, float xBin[], int xBinSize, float y, float yBin[], int yBinSize, vType* map[]) {
	if (cisnan(x)) {
		warning(CUSTOM_INTEPOLATE_ERROR_3, "%.2f: x is NaN in interpolate3d", x);
		return NAN;
	}
	if (cisnan(y)) {
		warning(CUSTOM_INTEPOLATE_ERROR_2, "%.2f: y is NaN in interpolate3d", y);
		return NAN;
	}

	int xIndex = findIndexMsg("x", xBin, xBinSize, x);
#if	DEBUG_INTERPOLATION
	if (needInterpolationLogging())
		printf("X index=%d\r\n", xIndex);
#endif /* DEBUG_INTERPOLATION */
	int yIndex = findIndexMsg("y", yBin, yBinSize, y);
	if (xIndex < 0 && yIndex < 0) {
#if	DEBUG_INTERPOLATION
		if (needInterpolationLogging())
			printf("X and Y are smaller than smallest cell in table: %d\r\n", xIndex);
#endif /* DEBUG_INTERPOLATION */
		return map[0][0];
	}

	if (xIndex < 0) {
#if	DEBUG_INTERPOLATION
		if (needInterpolationLogging())
			printf("X is smaller than smallest cell in table: %dr\n", xIndex);
#endif /* DEBUG_INTERPOLATION */
		if (yIndex == yBinSize - 1)
			return map[0][yIndex];
		float keyMin = yBin[yIndex];
		float keyMax = yBin[yIndex + 1];
		float rpmMinValue = map[0][yIndex];
		float rpmMaxValue = map[0][yIndex + 1];

		return interpolateMsg("3d", keyMin, rpmMinValue, keyMax, rpmMaxValue, y);
	}

	if (yIndex < 0) {
#if	DEBUG_INTERPOLATION
		if (needInterpolationLogging())
			printf("Y is smaller than smallest cell in table: %d\r\n", yIndex);
#endif /* DEBUG_INTERPOLATION */
		if (xIndex == xBinSize - 1)
			return map[xIndex][0];
		float key1 = xBin[xIndex];
		float key2 = xBin[xIndex + 1];
		float value1 = map[xIndex][0];
		float value2 = map[xIndex + 1][0];

		return interpolateMsg("out3d", key1, value1, key2, value2, x);
	}

	if (xIndex == xBinSize - 1 && yIndex == yBinSize - 1) {
#if	DEBUG_INTERPOLATION
		if (needInterpolationLogging())
			printf("X and Y are larger than largest cell in table: %d %d\r\n", xIndex, yIndex);
#endif /* DEBUG_INTERPOLATION */
		return map[xBinSize - 1][yBinSize - 1];
	}

	if (xIndex == xBinSize - 1) {
#if	DEBUG_INTERPOLATION
		if (needInterpolationLogging())
			printf("TODO BETTER LOGGING x overflow %d\r\n", yIndex);
#endif /* DEBUG_INTERPOLATION */
		// here yIndex is less than yBinSize - 1, we've checked that condition already

		float key1 = yBin[yIndex];
		float key2 = yBin[yIndex + 1];
		float value1 = map[xIndex][yIndex];
		float value2 = map[xIndex][yIndex + 1];

		return interpolateMsg("out3d", key1, value1, key2, value2, y);
	}

	if (yIndex == yBinSize - 1) {
#if	DEBUG_INTERPOLATION
		if (needInterpolationLogging())
			printf("Y is larger than largest cell in table: %d\r\n", yIndex);
#endif /* DEBUG_INTERPOLATION */
		// here xIndex is less than xBinSize - 1, we've checked that condition already

		float key1 = xBin[xIndex];
		float key2 = xBin[xIndex + 1];
		float value1 = map[xIndex][yIndex];
		float value2 = map[xIndex + 1][yIndex];

		return interpolateMsg("out3d", key1, value1, key2, value2, x);
	}

	/*
	 * first we find the interpolated value for this RPM
	 */
	int rpmMaxIndex = xIndex + 1;

	float xMin = xBin[xIndex];
	float xMax = xBin[xIndex + 1];
	float rpmMinKeyMinValue = map[xIndex][yIndex];
	float rpmMaxKeyMinValue = map[xIndex + 1][yIndex];

	float keyMinValue = interpolateMsg("", xMin, rpmMinKeyMinValue, xMax, rpmMaxKeyMinValue, x);

#if	DEBUG_INTERPOLATION
	if (needInterpolationLogging()) {
		printf("X=%.2f:\r\nrange %.2f - %.2f\r\n", x, xMin, xMax);
		printf("X interpolation range %.2f   %.2f result %.2f\r\n", rpmMinKeyMinValue, rpmMaxKeyMinValue, keyMinValue);
	}
#endif /* DEBUG_INTERPOLATION */

	int keyMaxIndex = yIndex + 1;
	float keyMin = yBin[yIndex];
	float keyMax = yBin[keyMaxIndex];
	float rpmMinKeyMaxValue = map[xIndex][keyMaxIndex];
	float rpmMaxKeyMaxValue = map[rpmMaxIndex][keyMaxIndex];

	float keyMaxValue = interpolateMsg("3d", xMin, rpmMinKeyMaxValue, xMax, rpmMaxKeyMaxValue, x);

#if	DEBUG_INTERPOLATION
	if (needInterpolationLogging()) {
		printf("key=%.2f:\r\nrange %.2f - %.2f\r\n", y, keyMin, keyMax);
		printf("key interpolation range %.2f   %.2f result %.2f\r\n", rpmMinKeyMaxValue, rpmMaxKeyMaxValue, keyMaxValue);
	}
#endif /* DEBUG_INTERPOLATION */

	float result = interpolateMsg("3d", keyMin, keyMinValue, keyMax, keyMaxValue, y);
	return result;
}

// Explicitly instantiate the ones we use
template float interpolate3d(float x, float xBin[], int xBinSize, float y, float yBin[], int yBinSize, float* map[]);
template float interpolate3d(float x, float xBin[], int xBinSize, float y, float yBin[], int yBinSize, uint8_t* map[]);
template float interpolate3d(float x, float xBin[], int xBinSize, float y, float yBin[], int yBinSize, int16_t* map[]);

/**
 * Sets specified value for specified key in a correction curve
 * see also setLinearCurve()
 */
void setCurveValue(float bins[], float values[], int size, float key, float value) {
	int index = findIndexMsg("tbVl", bins, size, key);
	if (index == -1)
		index = 0;
	values[index] = value;
}

void initInterpolation(Logging *sharedLogger) {
	logger = sharedLogger;
#if BINARY_PERF && ! EFI_UNIT_TEST
	addConsoleAction("binarytest", testBinary);
#endif

}
