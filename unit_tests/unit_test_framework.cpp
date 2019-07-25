/*
 * @file unit_test_framework.cpp
 *
 *  Created on: Mar 4, 2018
 *      Author: Andrey Belomutskiy, (c) 2012-2018
 */

#include <stdlib.h>
#include "global.h"
#include "unit_test_framework.h"

void assertEqualsM2(const char *msg, float expected, float actual, float eps) {
	ASSERT_NEAR(expected, actual, eps) << msg;
}

void assertEqualsM4(const char *prefix, const char *msg, float expected, float actual) {
	ASSERT_NEAR(expected, actual, 0.00001) << prefix << msg;
}

void assertEqualsLM(const char *msg, long expected, long actual) {
	ASSERT_EQ(expected, actual) << msg;
}

void assertEqualsM(const char *msg, float expected, float actual) {
	assertEqualsM2(msg, expected, actual, 0.0001);
}

void chDbgAssert(int c, char *msg, void *arg) {
	if (!c) {
		printf("assert failed: %s\r\n", msg);
		exit(-1);
	}
}



