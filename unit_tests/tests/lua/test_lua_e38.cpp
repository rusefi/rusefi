#include "pch.h"
#include "rusefi_lua.h"
#include "lua_lib.h"

/**
 * MSB bit conversion insanity
 * data[1] means start offset 7
 * data[2] means start offset 15
 * data[3] means start offset 23
 * data[4] means start offset 31
 * data[5] means start offset 39
 * data[6] means start offset 47
 * data[7] means start offset 55
 * data[8] means start offset 63
 */

#define ECMEngineStatus "\ndata = { 0x80, 0x20, 0x29, 0x16, 0x0F, 0x40, 0x08, 0x00}\n "

TEST(LuaE38, rpm) {

	const char* script = TWO_BYTES_MSB ECMEngineStatus R"(
		function testFunc()
			rpm = getTwoBytesMSB(data, 2, 0.25)
			return rpm
		end
		)";
	EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(script).value_or(0), 2629.5);
}


TEST(LuaE38, tps) {

	const char* script = TWO_BYTES_MSB ECMEngineStatus R"(
		function testFunc()
			tps = data[5] * 0.3921
			return tps
		end
		)";
	EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(script).value_or(0), 5.881);
}
