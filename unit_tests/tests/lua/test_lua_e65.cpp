#include "pch.h"
#include "rusefi_lua.h"

TEST(LuaE65, Battery) {
	const char* realdata = R"(
	function twoBytes(data, offset, factor)   
		return (data[offset + 1] * 256 + data[offset]) * factor   
	end

	function testFunc()
		data = {0xdc, 0x03, 0x00, 0x53, 0xFE, 0xD3, 0x04, 0x00}
		return twoBytes(data, 1, 0.0147)
	end

	)";

	EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 14.5236);


}
