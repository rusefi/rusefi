/*
 * @file test_change_engine_type.cpp
 *
 * @date Jul 14, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "engine_test_helper.h"
#include "settings.h"

TEST(misc, changeEngineType) {
	WITH_ENGINE_TEST_HELPER (FORD_ASPIRE_1996);

	setEngineType((int)CITROEN_TU3JP PASS_ENGINE_PARAMETER_SUFFIX);
}

