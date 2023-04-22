#include "pch.h"
#include "rusefi_lua.h"
#include "lua_lib.h"

TEST(LuaHonda, idleRpm1) {
#define real380Packet_1 "\ndata = { 0x00, 0x00, 0x08, 0x4A, 0x00, 0x00, 0x00, 0x1D}\n "
	const char* script = 	TWO_BYTES_MSB	real380Packet_1	R"(
    	function testFunc()
    		rpm = getTwoBytesMSB(data, 2, 1)
    		return rpm
    	end
    	)";

        EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(script).value_or(0), 2122);
}

TEST(LuaHonda, idleRpm2) {
#define real380Packet_2 "\ndata = { 0x0C, 0x01, 0x07, 0xE1, 0x80, 0x00, 0x00, 0x18}\n "
	const char* script = 	TWO_BYTES_MSB	real380Packet_2	R"(
    	function testFunc()
    		rpm = getTwoBytesMSB(data, 2, 1)
    		return rpm
    	end
    	)";

        EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(script).value_or(0), 2017);
}

TEST(LuaHonda, wheelSpeed) {
#define real309Packet "\ndata = { 0x06, 0x7C, 0x0A, 0x09, 0x05, 0xCC, 0x00, 0x9A}\n "
	const char* script = 	TWO_BYTES_LSB	real309Packet	R"(
    	function testFunc()
    		speedKph = getTwoBytesLSB(data, 1, 0.01)
    		return speedKph
    	end
    	)";

        EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(script).value_or(0), 26.84);
}
