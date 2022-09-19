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

TEST(LuaE38, packECMEngineStatus) {
	EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(
			TWO_BYTES_MSB ARRAY_EQUALS PRINT_ARRAY R"(
tps = 5.881
                                             		function testFunc()
		canEngine = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		canEngine[5] = tps / 0.3921

			print(arrayToString(canEngine))
		expected = { 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00 }
		return equals(canEngine, expected)
                                             		end
                                             		)"
			).value_or(0), 0);
}

#define ECMEngineCoolantTemp "\ndata = { 0x00, 0xCA, 0x69, 0x3F, 0x6A, 0x00, 0x00, 0x00}\n "

TEST(LuaE38, clt) {

	EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(TWO_BYTES_MSB ECMEngineCoolantTemp R"(
                                             		function testFunc()
                                             			clt = data[3] -40
                                             			return clt
                                             		end
                                             		)"
			).value_or(0), 65);
}

TEST(LuaE38, packECMEngineCoolantTemp) {
	EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(
			TWO_BYTES_MSB ARRAY_EQUALS PRINT_ARRAY R"(
clt = 65
                                             		function testFunc()
		canEngine = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		canEngine[3] = clt + 40

			print(arrayToString(canEngine))
		expected = { 0x00, 0x00, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00 }
		return equals(canEngine, expected)
                                             		end
                                             		)"
			).value_or(0), 0);
}
