//
// Created by kifir on 11/12/24.
//

#include "pch.h"

#include "rusefi_lua.h"
#include "lua_lib.h"

namespace {
	class LuaBitRangeMsbTest : public testing::Test {
	protected:
		void checkSetAndGet(int bitStart, int length, int testValue);
	};

	void LuaBitRangeMsbTest::checkSetAndGet(const int bitStart, const int length, const int testValue) {
		const char* const luaCodeFormatString = SET_BIT_RANGE_MSB GET_BIT_RANGE_MSB R"(

	function testFunc()
		data = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
		setBitRangeMsb(data, %d, %d, %d)
		return getBitRangeMsb(data, %d, %d)
	end)";
		char luaCode[4096];
		std::snprintf(luaCode, sizeof(luaCode), luaCodeFormatString, bitStart, length, testValue, bitStart, length);
		EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(luaCode).value_or(0), testValue)
			<< "bitStart=" << bitStart << ", length=" << length << ", testValue=" << testValue << std::endl << luaCode;
	}

	TEST_F(LuaBitRangeMsbTest, checkVariousBitRanges) {
		checkSetAndGet(24, 4, 0xB);
		checkSetAndGet(24, 7, 0x59);
		checkSetAndGet(24, 8, 0xB3);
		checkSetAndGet(24, 10, 0x2C9);
		checkSetAndGet(24, 9, 0x173);
		checkSetAndGet(22, 8, 0xB3);
		checkSetAndGet(22, 9, 0x167);
		checkSetAndGet(23, 8, 0xB3);
	}

	TEST_F(LuaBitRangeMsbTest, checkWithTotalBitIndex22AndBitWidth8) {
		const char* realdata = SET_BIT_RANGE_MSB GET_BIT_RANGE_MSB R"(

	function testFunc()
		data = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
		setBitRangeMsb(data, 22, 8, 0xB3)
		return getBitRangeMsb(data, 22, 8)
	end)";

		EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 0xB3);
	}
}