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
	const char* realdata = PRINT_ARRAY ARRAY_EQUALS SET_TWO_BYTES R"(


function arrayToString(arr)
	local str = ""
	local index = 1
	while arr[index] ~= nil do
		str = str.." "..toHexString(arr[index])
		index = index + 1
	end
	return str
end

	function testFunc()
		engineTorque = 15.21
		rpm = 1207.1
		innerTorqWithoutExt = 21.6
		tps = 31.6
		requestedTorque = 21.84

		data = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

		data[2] = engineTorque / 0.39
		setTwoBytes(data, 2, rpm / 0.25)
		data[5] = innerTorqWithoutExt / 0.4

print(arrayToString(data))

		expected = { 0x00, 0x27, 0xDC, 0x12, 0x36, 0x00, 0x00, 0x00 }
--		print(data)
		return equals(data, expected)
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 0);
}

#define realMotor1Packet "\ndata = { 0x00, 0x27, 0x8A, 0x1A, 0x36, 0x4F, 0x19, 0x38}\n "

TEST(LuaVag, unpackMotor1_engine_torq) {
	const char* realdata = 	GET_BIT_RANGE_LSB	realMotor1Packet	R"(
	function testFunc()
		engineTorque = getBitRange(data, 8, 8) * 0.39
		return engineTorque
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 15.21);
}

TEST(LuaVag, unpackMotor1_rpm) {
	const char* realdata = 	GET_BIT_RANGE_LSB	realMotor1Packet	R"(
	function testFunc()
		rpm = getBitRange(data, 16, 16) * 0.25
		return rpm
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 1698.5);
}

TEST(LuaVag, unpackMotor1_inner_torq) {
	const char* realdata = 	GET_BIT_RANGE_LSB	realMotor1Packet	R"(
	function testFunc()
		innerTorqWithoutExt = getBitRange(data, 32, 8) * 0.4
		return innerTorqWithoutExt
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 21.6);
}

TEST(LuaVag, unpackMotor1_tps) {
	const char* realdata = 	GET_BIT_RANGE_LSB	realMotor1Packet	R"(
	function testFunc()
		tps = getBitRange(data, 40, 8) * 0.4
		return tps
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 31.6);
}

TEST(LuaVag, unpackMotor1_torq_loss) {
	const char* realdata = 	GET_BIT_RANGE_LSB	realMotor1Packet	R"(
	function testFunc()
		torqueLoss = getBitRange(data, 48, 8) * 0.39
		return torqueLoss
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 9.75);
}


TEST(LuaVag, unpackMotor1_torq_req) {
	const char* realdata = 	GET_BIT_RANGE_LSB	realMotor1Packet	R"(
	function testFunc()
		requestedTorque = getBitRange(data, 56, 8) * 0.39
		return requestedTorque
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 21.84);
}

