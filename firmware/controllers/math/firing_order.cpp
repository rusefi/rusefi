#include "pch.h"

static const uint8_t order_1[] = {1};

static const uint8_t order_1_2[] = {1, 2};

static const uint8_t order_1_2_3[] = {1, 2, 3};
static const uint8_t order_1_3_2[] = {1, 3, 2};
// 4 cylinder

static const uint8_t order_1_THEN_3_THEN_4_THEN2[] = { 1, 3, 4, 2 };
static const uint8_t order_1_THEN_2_THEN_4_THEN3[] = { 1, 2, 4, 3 };
static const uint8_t order_1_THEN_3_THEN_2_THEN4[] = { 1, 3, 2, 4 };
static const uint8_t order_1_THEN_4_THEN_3_THEN2[] = { 1, 4, 3, 2 };

// 5 cylinder
static const uint8_t order_1_2_4_5_3[] = {1, 2, 4, 5, 3};

// 6 cylinder
static const uint8_t order_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4[] = { 1, 5, 3, 6, 2, 4 };
static const uint8_t order_1_THEN_4_THEN_2_THEN_5_THEN_3_THEN_6[] = { 1, 4, 2, 5, 3, 6 };
static const uint8_t order_1_THEN_2_THEN_3_THEN_4_THEN_5_THEN_6[] = { 1, 2, 3, 4, 5, 6 };
static const uint8_t order_1_6_3_2_5_4[] = {1, 6, 3, 2, 5, 4};
static const uint8_t order_1_4_3_6_2_5[] = {1, 4, 3, 6, 2, 5};
static const uint8_t order_1_6_2_4_3_5[] = {1, 6, 2, 4, 3, 5};
static const uint8_t order_1_6_5_4_3_2[] = {1, 6, 5, 4, 3, 2};
static const uint8_t order_1_4_5_2_3_6[] = {1, 4, 5, 2, 3, 6};

// 8 cylinder
static const uint8_t order_1_8_4_3_6_5_7_2[] = { 1, 8, 4, 3, 6, 5, 7, 2 };
static const uint8_t order_1_8_7_2_6_5_4_3[] = { 1, 8, 7, 2, 6, 5, 4, 3 };
static const uint8_t order_1_5_4_2_6_3_7_8[] = { 1, 5, 4, 2, 6, 3, 7, 8 };
static const uint8_t order_1_2_7_8_4_5_6_3[] = { 1, 2, 7, 8, 4, 5, 6, 3 };
static const uint8_t order_1_3_7_2_6_5_4_8[] = { 1, 3, 7, 2, 6, 5, 4, 8 };
static const uint8_t order_1_2_3_4_5_6_7_8[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
static const uint8_t order_1_5_4_8_6_3_7_2[] = { 1, 5, 4, 8, 6, 3, 7, 2 };
static const uint8_t order_1_8_7_3_6_5_4_2[] = { 1, 8, 7, 3, 6, 5, 4, 2 };
static const uint8_t order_1_5_4_8_3_7_2_6[] = { 1, 5, 4, 8, 3, 7, 2, 6 };
static const uint8_t order_1_8_6_2_7_3_4_5[] = { 1, 8, 6, 2, 7, 3, 4, 5 };

// 9 cylinder
static const uint8_t order_1_2_3_4_5_6_7_8_9[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

// 10 cylinder
static const uint8_t order_1_10_9_4_3_6_5_8_7_2[] = {1, 10, 9, 4, 3, 6, 5, 8, 7, 2};
static const uint8_t order_1_6_5_10_2_7_3_8_4_9[] = {1, 6, 5, 10, 2, 7, 3, 8, 4, 9};

// 12 cyliner
static const uint8_t order_1_7_5_11_3_9_6_12_2_8_4_10[] = {1, 7, 5, 11, 3, 9, 6, 12, 2, 8, 4, 10};
static const uint8_t order_1_7_4_10_2_8_6_12_3_9_5_11[] = {1, 7, 4, 10, 2, 8, 6, 12, 3, 9, 5, 11};
static const uint8_t order_1_12_5_8_3_10_6_7_2_11_4_9[] = {1, 12, 5, 8, 3, 10, 6, 7, 2, 11, 4, 9};
static const uint8_t order_1_2_3_4_5_6_7_8_9_10_11_12[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

// no comments
static const uint8_t order_1_14_9_4_7_12_15_6_13_8_3_16_11_2_5_10[] = {1, 14, 9, 4, 7, 12, 15, 6, 13, 8, 3, 16, 11, 2, 5, 10};

static size_t getFiringOrderLength() {

	switch (engineConfiguration->firingOrder) {
	case FO_1:
		return 1;
// 2 cylinder
	case FO_1_2:
		return 2;
// 3 cylinder
	case FO_1_2_3:
	case FO_1_3_2:
		return 3;
// 4 cylinder
	case FO_1_3_4_2:
	case FO_1_2_4_3:
	case FO_1_3_2_4:
	case FO_1_4_3_2:
		return 4;
// 5 cylinder
	case FO_1_2_4_5_3:
		return 5;

// 6 cylinder
	case FO_1_5_3_6_2_4:
	case FO_1_4_2_5_3_6:
	case FO_1_2_3_4_5_6:
	case FO_1_6_3_2_5_4:
	case FO_1_4_3_6_2_5:
	case FO_1_6_2_4_3_5:
	case FO_1_6_5_4_3_2:
	case FO_1_4_5_2_3_6:
		return 6;

// 8 cylinder
	case FO_1_8_4_3_6_5_7_2:
	case FO_1_8_7_2_6_5_4_3:
	case FO_1_5_4_2_6_3_7_8:
	case FO_1_2_7_8_4_5_6_3:
	case FO_1_3_7_2_6_5_4_8:
	case FO_1_2_3_4_5_6_7_8:
	case FO_1_5_4_8_6_3_7_2:
	case FO_1_8_7_3_6_5_4_2:
	case FO_1_5_4_8_3_7_2_6:
	case FO_1_8_6_2_7_3_4_5:
		return 8;

// 9 cylinder radial
	case FO_1_2_3_4_5_6_7_8_9:
		return 9;

// 10 cylinder
	case FO_1_10_9_4_3_6_5_8_7_2:
	case FO_1_6_5_10_2_7_3_8_4_9:
		return 10;

// 12 cylinder
	case FO_1_7_5_11_3_9_6_12_2_8_4_10:
	case FO_1_7_4_10_2_8_6_12_3_9_5_11:
	case FO_1_12_5_8_3_10_6_7_2_11_4_9:
	case FO_1_2_3_4_5_6_7_8_9_10_11_12:
		return 12;

	case FO_1_14_9_4_7_12_15_6_13_8_3_16_11_2_5_10:
		return 16;

	default:
		firmwareError(ObdCode::CUSTOM_OBD_UNKNOWN_FIRING_ORDER, "Invalid firing order: %d", engineConfiguration->firingOrder);
	}
	return 1;
}

static const uint8_t* getFiringOrderTable() {
	switch (engineConfiguration->firingOrder) {
	case FO_1:
		return order_1;
// 2 cylinder
	case FO_1_2:
		return order_1_2;
// 3 cylinder
	case FO_1_2_3:
		return order_1_2_3;
	case FO_1_3_2:
		return order_1_3_2;
// 4 cylinder
	case FO_1_3_4_2:
		return order_1_THEN_3_THEN_4_THEN2;
	case FO_1_2_4_3:
		return order_1_THEN_2_THEN_4_THEN3;
	case FO_1_3_2_4:
		return order_1_THEN_3_THEN_2_THEN4;
	case FO_1_4_3_2:
		return order_1_THEN_4_THEN_3_THEN2;
// 5 cylinder
	case FO_1_2_4_5_3:
		return order_1_2_4_5_3;

// 6 cylinder
	case FO_1_5_3_6_2_4:
		return order_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4;
	case FO_1_4_2_5_3_6:
		return order_1_THEN_4_THEN_2_THEN_5_THEN_3_THEN_6;
	case FO_1_2_3_4_5_6:
		return order_1_THEN_2_THEN_3_THEN_4_THEN_5_THEN_6;
	case FO_1_6_3_2_5_4:
		return order_1_6_3_2_5_4;
	case FO_1_4_3_6_2_5:
		return order_1_4_3_6_2_5;
	case FO_1_6_2_4_3_5:
		return order_1_6_2_4_3_5;
	case FO_1_6_5_4_3_2:
		return order_1_6_5_4_3_2;
	case FO_1_4_5_2_3_6:
		return order_1_4_5_2_3_6;

// 8 cylinder
	case FO_1_8_4_3_6_5_7_2:
		return order_1_8_4_3_6_5_7_2;
	case FO_1_8_7_2_6_5_4_3:
		return order_1_8_7_2_6_5_4_3;
	case FO_1_5_4_2_6_3_7_8:
		return order_1_5_4_2_6_3_7_8;
	case FO_1_2_7_8_4_5_6_3:
		return order_1_2_7_8_4_5_6_3;
	case FO_1_3_7_2_6_5_4_8:
		return order_1_3_7_2_6_5_4_8;
	case FO_1_2_3_4_5_6_7_8:
		return order_1_2_3_4_5_6_7_8;
	case FO_1_5_4_8_6_3_7_2:
		return order_1_5_4_8_6_3_7_2;
	case FO_1_8_7_3_6_5_4_2:
		return order_1_8_7_3_6_5_4_2;
	case FO_1_5_4_8_3_7_2_6:
		return order_1_5_4_8_3_7_2_6;
	case FO_1_8_6_2_7_3_4_5:
		return order_1_5_4_8_3_7_2_6;

// 9 cylinder
	case FO_1_2_3_4_5_6_7_8_9:
		return order_1_2_3_4_5_6_7_8_9;


// 10 cylinder
	case FO_1_10_9_4_3_6_5_8_7_2:
		return order_1_10_9_4_3_6_5_8_7_2;
	case FO_1_6_5_10_2_7_3_8_4_9:
		return order_1_6_5_10_2_7_3_8_4_9;

// 12 cylinder
	case FO_1_7_5_11_3_9_6_12_2_8_4_10:
		return order_1_7_5_11_3_9_6_12_2_8_4_10;
	case FO_1_7_4_10_2_8_6_12_3_9_5_11:
		return order_1_7_4_10_2_8_6_12_3_9_5_11;
	case FO_1_12_5_8_3_10_6_7_2_11_4_9:
		return order_1_12_5_8_3_10_6_7_2_11_4_9;
	case FO_1_2_3_4_5_6_7_8_9_10_11_12:
		return order_1_2_3_4_5_6_7_8_9_10_11_12;

// do not ask
	case FO_1_14_9_4_7_12_15_6_13_8_3_16_11_2_5_10:
		return order_1_14_9_4_7_12_15_6_13_8_3_16_11_2_5_10;

	default:
		firmwareError(ObdCode::CUSTOM_OBD_UNKNOWN_FIRING_ORDER, "Invalid firing order: %d", engineConfiguration->firingOrder);
	}

	return NULL;
}

/**
 * @param index from zero to cylindersCount - 1
 * @return cylinderId from one to cylindersCount
 */
size_t getCylinderNumberAtIndex(size_t index) {
	const size_t firingOrderLength = getFiringOrderLength();

	if (firingOrderLength < 1 || firingOrderLength > MAX_CYLINDER_COUNT) {
		firmwareError(ObdCode::CUSTOM_FIRING_LENGTH, "fol %d", firingOrderLength);
		return 0;
	}
	if (engineConfiguration->cylindersCount != firingOrderLength) {
		// May 2020 this somehow still happens with functional tests, maybe race condition?
		firmwareError(ObdCode::CUSTOM_OBD_WRONG_FIRING_ORDER, "Wrong cyl count for firing order, expected %d cylinders, got %d", firingOrderLength, engineConfiguration->cylindersCount);
		return 0;
	}

	if (index >= firingOrderLength) {
		// May 2020 this somehow still happens with functional tests, maybe race condition?
		warning(ObdCode::CUSTOM_ERR_6686, "firing order index %d", index);
		return 0;
	}

	if (auto firingOrderTable = getFiringOrderTable()) {
		return firingOrderTable[index] - 1;
	} else {
		// error already reported
		return 0;
	}
}

/**
 * @param prevCylinderId from one to cylindersCount
 * @return cylinderId from one to cylindersCount
 */
size_t getNextFiringCylinderId(size_t prevCylinderId) {
	const size_t firingOrderLength = getFiringOrderLength();
	auto firingOrderTable = getFiringOrderTable();

	if (firingOrderTable) {
		for (size_t i = 0; i < firingOrderLength; i++) {
			if (firingOrderTable[i] == prevCylinderId) {
				return firingOrderTable[(i + 1) % firingOrderLength];
			}
		}
	}

	return 1;
}
