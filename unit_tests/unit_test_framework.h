/*
 * unit_test_framework.h
 *
 *  Created on: Mar 4, 2018
 *      Author: Andrey Belomutskiy, (c) 2012-2018
 */

#ifndef UNIT_TEST_FRAMEWORK_H_
#define UNIT_TEST_FRAMEWORK_H_

#include "engine.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#define EPS1D 0.1
#define EPS2D 0.01
#define EPS3D 0.001
#define EPS4D 0.0001
#define EPS5D 0.00001

void assertEqualsM2(const char *msg, float expected, float actual, float EPS);
void assertEqualsM(const char *msg, float expected, float actual);
void assertEqualsLM(const char *msg, long expected, long actual);
void assertEqualsM4(const char *prefix, const char *msg, float expected, float actual);
void assertEqualsM5(const char *prefix, const char *message, float expected, float actual, float EPS);
void assertTrueM(const char *msg, float actual);
void assertFalseM(const char *msg, float actual);

#endif /* UNIT_TEST_FRAMEWORK_H_ */
