#include "pch.h"
#include "rusefi_lua.h"
#include "lua_lib.h"

TEST(LuaNissan, wheelSpeed) {
#define realWheelPacket "\ndata = { 0x19, 0x6c, 0x19, 0x68, 0x0e, 0xb0, 0x37, 0x81}\n "
	const char* script = 	TWO_BYTES_MSB	realWheelPacket	R"(
    	function testFunc()
    		engineTorque = getTwoBytesMSB(data, 0, 0.005)
    		return engineTorque
    	end
    	)";

        EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(script).value_or(0), 32.54);
}
