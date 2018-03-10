/*
 * @file unit_test_framework.cpp
 *
 *  Created on: Mar 4, 2018
 *      Author: Andrey Belomutskiy, (c) 2012-2018
 */

#include <stdlib.h>
#include "main.h"

void assertEqualsM5(const char *prefix, const char *message, float expected, float actual, float EPS) {
	char msg[100];
	strcpy(msg, prefix);
	strcat(msg, message);
	if (cisnan(actual) && !cisnan(expected)) {
		printf("Assert failed: %s %.4f while expected %.4f\r\n", msg, actual, expected);
		exit(-1);
	}

	float delta = absF(actual - expected);
	if (delta > EPS) {
		printf("delta: %.7f\r\n", delta);
		printf("Unexpected: %s %.4f while expected %.4f\r\n", msg, actual, expected);
		exit(-1);
	}
	printf("Validated %s: %f\r\n", msg, expected);
}

void assertEqualsM2(const char *msg, float expected, float actual, float eps) {
	assertEqualsM5("", msg, expected, actual, eps);
}

void assertEqualsM4(const char *prefix, const char *msg, float expected, float actual) {
	assertEqualsM5(prefix, msg, expected, actual, 0.00001);
}

void assertEqualsLM(const char *msg, long expected, long actual) {
	if (expected != actual) {
		printf("Assert failed: %s %d while expected %d\r\n", msg, actual, expected);
		exit(-1);
	}
}

void assertEqualsM(const char *msg, float expected, float actual) {
	assertEqualsM2(msg, expected, actual, 0.0001);
}

void assertEquals(float expected, float actual) {
	assertEqualsM("", expected, actual);
}

void assertTrueM(const char *msg, float actual) {
	assertEqualsM(msg, TRUE, actual);
}

void assertTrue(float actual) {
	assertTrueM("", actual);
}

void assertFalseM(const char *msg, float actual) {
	assertEqualsM(msg, FALSE, actual);
}

void assertFalse(float actual) {
	assertFalseM("", actual);
}

void chDbgAssert(int c, char *msg, void *arg) {
	if (!c) {
		printf("assert failed: %s\r\n", msg);
		exit(-1);
	}
}



