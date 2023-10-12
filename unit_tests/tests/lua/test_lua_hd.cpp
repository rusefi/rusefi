#include "pch.h"
#include "rusefi_lua.h"
#include "lua_lib.h"

TEST(LuaHd2023, packet) {
	const char* realHDdata = R"(

	function testFunc()
		return crc8_j1850({0x00, 0x00, 0x00, 0x00, 0xa0, 0x00, 0x2d }, 7)
	end

	)";
	EXPECT_EQ(testLuaReturnsNumberOrNil(realHDdata).value_or(0), 0xfc);
}