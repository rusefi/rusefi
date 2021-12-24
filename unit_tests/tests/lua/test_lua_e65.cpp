#include "pch.h"
#include "rusefi_lua.h"

#define TWO_BYTES "function twoBytes(data, offset, factor)        \
		return (data[offset + 2] * 256 + data[offset + 1]) * factor   \
	end"

// https://github.com/HeinrichG-V12/E65_ReverseEngineering/blob/main/docs/0x3B4.md
TEST(LuaE65, Battery) {
	const char* realdata = TWO_BYTES R"(

	function testFunc()
		data = {0xdc, 0x03, 0x00, 0x53, 0xFE, 0xD3, 0x04, 0x00}
		return twoBytes(data, 0, 0.0147)
	end)";

	EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 14.5236);
}

// http://loopybunny.co.uk/CarPC/can/0AA.html
TEST(LuaE65, Rpm) {
	const char* realdata = TWO_BYTES R"(

	function testFunc()
		data = {0x5F, 0x59, 0xFF, 0x00, 0x34, 0x0D, 0x80, 0x99}
		return twoBytes(data, 4, 0.25)
	end)";

	EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 845);
}
