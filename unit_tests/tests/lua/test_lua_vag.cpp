#include "pch.h"
#include "rusefi_lua.h"
#include "lua_lib.h"

#define VAG_CHECKSUM "function xorChecksum(data)        \
		return data[1] ~ data[2] ~ data[3] ~ data[4] ~ data[5] ~ data[6] ~ data[7]  \
	end"


TEST(LuaVag, Checksum) {
	const char* realdata = VAG_CHECKSUM R"(

	function testFunc()
		data = { 0xE0, 0x20, 0x20, 0x7E, 0xFE, 0xFF, 0xFF, 0x60 }
		return  xorChecksum(data)
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 0x60);
}

// LSB (Least Significant Byte comes first) "Intel"
TEST(LuaVag, packMotor1) {
	const char* realdata = ARRAY_EQUALS SET_TWO_BYTES R"(


	function testFunc()
		rpm = 1207.1
		data = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

		setTwoBytes(data, 2, 4 * rpm)
		expected = { 0x00, 0x00, 0xDF, 0x12, 0x00, 0x00, 0x00, 0x00 }
--		print(data)
		return equals(data, expected)
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 0);
}

