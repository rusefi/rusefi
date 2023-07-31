#include "pch.h"
#include "rusefi_lua.h"
#include "lua_lib.h"

TEST(LuaKia, idleRpm1) {
#define real316Packet_1 "\ndata = { 0x05, 0x1B, 0x7A, 0x0F, 0x1B, 0x2C, 0x00, 0x7F}\n "
	const char* script = 	TWO_BYTES_LSB	real316Packet_1	R"(
    	function testFunc()
    		rpm = getTwoBytesLSB(data, 2, 0.25)
    		return rpm
    	end
    	)";

        EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(script).value_or(0), 990.5);
}

TEST(LuaKia, packRpm) {
	EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(
			TWO_BYTES_MSB ARRAY_EQUALS SET_TWO_BYTES_LSB PRINT_ARRAY R"(
rpm = 990
                                             		function testFunc()
		canEngine = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
		setTwoBytesLsb(canEngine, 2, 4 * rpm)

		print(arrayToString(canEngine))
		expected = { 0x00, 0x00, 0x78, 0x0F, 0x00, 0x00, 0x00, 0x00 }
		return equals(canEngine, expected)
                                             		end
                                             		)"
			).value_or(0), 0);
}

TEST(LuaKia, calcSumOfNibbles) {
#define real81Packet "\npacket = { 0x40, 0x85, 0x5F, 0x00, 0x00, 0x00, 0x00, 0xED}\n "
	const char* script = HYUNDAI_SUM_NIBBLES	real81Packet	R"(
function testFunc()
  return hyundaiSumNibbles(packet, 0xD)
end
    	)";
	EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(script).value_or(0), 0xE);
}
