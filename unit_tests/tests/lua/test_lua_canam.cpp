#include "pch.h"
#include "rusefi_lua.h"
#include "lua_lib.h"

TEST(canam, ecu102_1) {
	const char* realdata = VAG_CHECKSUM R"(

	function testFunc()
		data = { 0x16, 0xca, 0x0d, 0x70, 0x4d, 0xad, 0x04, 0x45 }
		return xorChecksum(data, 8)
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 0x45);
}

TEST(canam, bytes) {
	const char* realdata = TWO_BYTES_MSB R"(

	function testFunc()
		data = {0x00, 0x00, 0x00, 0x00, 0x01, 0xc7, 0x00, 0xc6}
		return getTwoBytesMSB(data, 4, 1)
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 455);
}

TEST(canam, bytes2) {
	const char* realdata = TWO_BYTES_MSB R"(

	function testFunc()
		data = {0x00, 0x00, 0x04, 0xa2, 0x02, 0x6c, 0x00, 0x00}
		return getTwoBytesMSB(data, 2, 1)
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 1186);
}

TEST(canam, ecu102_4) {
	const char* realdata = VAG_CHECKSUM R"(

	function testFunc()
		data = {0x00, 0x00, 0x0e, 0x6a, 0x53, 0xac, 0x06, 0}
		return xorChecksum(data, 8)
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 0x9d);
}
