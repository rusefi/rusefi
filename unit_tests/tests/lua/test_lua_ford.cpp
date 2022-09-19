#include "pch.h"
#include "rusefi_lua.h"
#include "lua_lib.h"

TEST(LuaFordFocusII, PPS_low) {
	const char* realdata = TWO_BYTES_MSB R"(

	function testFunc()
		data = {0x0e,	0x8f,	0x7d,	0x90,	0x27,	0x10,	0x00,	0x7d}
		return getTwoBytesMSB(data, 6, 1)
	end)";

	EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 0x7d);
}

TEST(LuaFordFocusII, PPS_high) {
	const char* realdata = TWO_BYTES_MSB R"(

	function testFunc()
		data = {0x0e,	0x8f,	0x7d,	0x90,	0x27,	0x10,	0xC8,	0x80}
		return getTwoBytesMSB(data, 6, 1)
	end)";

	EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 0xC880);
}
