/*
 * test_lua_with_engine.cpp
 *
 *  Created on: Nov 5, 2021
 *      Author: rusefi
 */

#include "pch.h"
#include "script_impl.h"
#include "rusefi_lua.h"

static const char* curveTestScript = R"(

function testFunc()
	index = findCurveIndex("HELLO")
	return curve(index, 40)
end

)";

TEST(LuaHooks, TestCurve) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	strcpy(engineConfiguration->scriptCurveName[3], "hello");
	setLinearCurve(config->scriptCurve4, 500, 600, 1);

	auto index = getCurveIndexByName("helLO");
	ASSERT_EQ(index.value_or(-1), 3);

	EXPECT_EQ(testLuaReturnsNumberOrNil(curveTestScript).value_or(0), 540);
}
