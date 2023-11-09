/*
 * test_lua_vin.cpp
 *
 */

#include "pch.h"
#include "rusefi_lua.h"
#include "lua_lib.h"


TEST(LuaVin, Test) {
	EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);

	strcpy(engineConfiguration->vinNumber, "GM123");

	const char* realdata = R"(

	function testFunc()
		return vin(0)
	end)";

	EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 0x47);
}

TEST(LuaVinCpy, Test) {
	EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);

	strcpy(engineConfiguration->vinNumber, "GM123");

	const char* realdata = ARRAY_EQUALS PRINT_ARRAY R"(

	function testFunc()
	    data = {0, 0, 0, 0, 0}
	    vincpy(data, 2, 1, 3);
		print(arrayToString(data))
	    expected = { 0x31, 0x32, 0x33, 0x00, 0x00 }
		return equals(data, expected)
	end)";

	EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 0);
}
