/*
 * unit_test_framework.h
 *
 *  Created on: Mar 4, 2018
 *      Author: Andrey Belomutskiy, (c) 2012-2018
 */

#ifndef UNIT_TEST_FRAMEWORK_H_
#define UNIT_TEST_FRAMEWORK_H_

#include "engine.h"

void assertEqualsM2(const char *msg, float expected, float actual, float EPS);
void assertEqualsM(const char *msg, float expected, float actual);
void assertEqualsLM(const char *msg, long expected, long actual);
void assertEqualsM4(const char *prefix, const char *msg, float expected, float actual);
void assertEqualsM5(const char *prefix, const char *message, float expected, float actual, float EPS);
void assertEquals(float expected, float actual);
void assertTrue(float actual);
void assertTrueM(const char *msg, float actual);
void assertFalse(float actual);
void assertFalseM(const char *msg, float actual);

scheduling_s * assertEvent5(const char *msg, int index, void *callback, efitime_t start, efitime_t momentX);
void assertEvent(const char *msg, int index, void *callback, efitime_t start, efitime_t momentX, long param);
void assertInjectorUpEvent(const char *msg, int eventIndex, efitime_t momentX, long injectorIndex DECLARE_ENGINE_PARAMETER_SUFFIX);
void assertInjectorDownEvent(const char *msg, int eventIndex, efitime_t momentX, long injectorIndex DECLARE_ENGINE_PARAMETER_SUFFIX);

#endif /* UNIT_TEST_FRAMEWORK_H_ */
