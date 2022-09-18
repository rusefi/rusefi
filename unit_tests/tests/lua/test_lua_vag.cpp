#include "pch.h"
#include "rusefi_lua.h"
#include "lua_lib.h"

// XOR of the array, skipping target index
#define VAG_CHECKSUM " \
function xorChecksum(data, targetIndex) \
	local index = 1 \
	local result = 0 \
	while data[index] ~= nil do \
		if index ~= targetIndex then \
			result = result ~ data[index] \
		end \
		index = index + 1 \
	end \
	data[targetIndex] = result \
	return result \
end \
"


TEST(LuaVag, Checksum) {
	const char* realdata = VAG_CHECKSUM R"(

	function testFunc()
		data = { 0xE0, 0x20, 0x20, 0x7E, 0xFE, 0xFF, 0xFF, 0x60 }
		return xorChecksum(data, 8)
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 0x60);
}

// LSB (Least Significant Byte comes first) "Intel"
TEST(LuaVag, packMotor1) {
	const char* realdata = PRINT_ARRAY ARRAY_EQUALS SET_TWO_BYTES R"(

	function testFunc()
		engineTorque = 15.21
		rpm = 1207.1
		innerTorqWithoutExt = 21.6
		tps = 31.6
		torqueLoss = 9.75
		requestedTorque = 21.84

		canMotor1 = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

		canMotor1[2] = engineTorque / 0.39
		setTwoBytes(canMotor1, 2, rpm / 0.25)
		canMotor1[5] = innerTorqWithoutExt / 0.4
 		canMotor1[6] = tps / 0.4
		canMotor1[7] = torqueLoss / 0.39
		canMotor1[8] = requestedTorque / 0.39

		print(arrayToString(canMotor1))

		expected = { 0x00, 0x27, 0xDC, 0x12, 0x36, 0x4F, 0x19, 0x38 }
		return equals(canMotor1, expected)
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 0);
}

#define realMotor1Packet "\ndata = { 0x00, 0x27, 0x8A, 0x1A, 0x36, 0x4F, 0x19, 0x38}\n "

TEST(LuaVag, unpackMotor1_engine_torq) {
	const char* script = 	GET_BIT_RANGE_LSB	realMotor1Packet	R"(
	function testFunc()
		engineTorque = getBitRange(data, 8, 8) * 0.39
		return engineTorque
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(script).value_or(0), 15.21);
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

#define realMotor3Packet "\ndata = { 0x00, 0x62, 0xFA, 0xDA, 0x22, 0x00, 0x00, 0xFA}\n "

TEST(LuaVag, packMotor3) {
	const char* script = SET_BIT_RANGE_LSB PRINT_ARRAY ARRAY_EQUALS SET_TWO_BYTES R"(

	function testFunc()
		tps = 100
		iat = 25.5
		desired_wheel_torque = 284.7

		canMotor3 = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

 		canMotor3[2] = (iat + 48) / 0.75
		canMotor3[3] = tps / 0.4
		canMotor3[5] = 0x20
		setBitRange(canMotor3, 24, 12, math.floor(desired_wheel_torque / 0.39))
 		canMotor3[8] = tps / 0.4

		print(arrayToString(canMotor3))

		expected = { 0x00, 0x62, 0xFA, 0xDA, 0x22, 0x00, 0x00, 0xFA }
		return equals(canMotor3, expected)
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(script).value_or(0), 0);
}


TEST(LuaVag, unpackMotor3_tps) {
	const char* script = 	GET_BIT_RANGE_LSB	realMotor3Packet	R"(
	function testFunc()
		tps = getBitRange(data, 56, 8) * 0.40
		return tps
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(script).value_or(0), 100);
}

TEST(LuaVag, unpackMotor3_pps) {
	const char* script = 	GET_BIT_RANGE_LSB	realMotor3Packet	R"(
	function testFunc()
		pps = getBitRange(data, 16, 8) * 0.40
		return pps
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(script).value_or(0), 100);
}

TEST(LuaVag, setBitRange) {
	{
		const char* script = PRINT_ARRAY ARRAY_EQUALS SET_BIT_RANGE_LSB R"(

		function testFunc()
			data = { 0x34, 0x56, 0x00 }
			setBitRange(data, 4, 8, 0xAB)

			print(arrayToString(data))

			expected = { 0xB4, 0x5A, 0x00 }
			return equals(data, expected)
		end
		)";

	    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(script).value_or(0), 0);
	}

	{
		const char* script = PRINT_ARRAY ARRAY_EQUALS SET_BIT_RANGE_LSB R"(

		function testFunc()
			data = { 0x00, 0x00, 0x00 }
			setBitRange(data, 5, 9, 0x1FF)

			print(arrayToString(data))

			expected = { 0xE0, 0x3F, 0x00 }
			return equals(data, expected)
		end
		)";

	    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(script).value_or(0), 0);
	}


	{
		const char* script = PRINT_ARRAY ARRAY_EQUALS SET_BIT_RANGE_LSB R"(

		function testFunc()
			data = { 0xFF, 0xFF, 0x00 }
			setBitRange(data, 5, 9, 0x0)

			print(arrayToString(data))

			expected = { 0x1F, 0xC0, 0x00 }
			return equals(data, expected)
		end
		)";

	    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(script).value_or(0), 0);
	}

}

TEST(LuaVag, unpackMotor3_iat) {
	const char* script = 	GET_BIT_RANGE_LSB	realMotor3Packet	R"(
	function testFunc()
		iat = getBitRange(data, 8, 8) * 0.75 - 48
		return iat
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(script).value_or(0), 25.5);
}

TEST(LuaVag, unpackMotor3_desired_wheel_torque) {
	const char* script = 	GET_BIT_RANGE_LSB	realMotor3Packet	R"(
	function testFunc()
		desired_wheel_torque = getBitRange(data, 24, 12) * 0.39
		return desired_wheel_torque
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(script).value_or(0), 284.7);
}

#define realMotor6Packet "\ndata = { 0x3D, 0x54, 0x69, 0x7E, 0xFE, 0xFF, 0xFF, 0x80}\n "

TEST(LuaVag, unpackMotor6_actual_torq) {
	const char* script = 	GET_BIT_RANGE_LSB	realMotor6Packet	R"(
	function testFunc()
		actualTorque = getBitRange(data, 16, 8) * 0.39
		return actualTorque
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(script).value_or(0), 40.95);
}

TEST(LuaVag, unpackMotor6_target_torq) {
	const char* script = 	GET_BIT_RANGE_LSB	realMotor6Packet	R"(
	function testFunc()
		targetTorque = getBitRange(data, 8, 8) * 0.39
		return targetTorque
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(script).value_or(0), 32.76);
}

TEST(LuaVag, unpackMotor6_feedback) {
	const char* script = 	GET_BIT_RANGE_LSB	realMotor6Packet	R"(
	function testFunc()
		feedbackGearbox = getBitRange(data, 40, 8) * 0.39
		return feedbackGearbox
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(script).value_or(0), 99.45);
}

TEST(LuaVag, packMotor6) {
	const char* script = PRINT_ARRAY ARRAY_EQUALS VAG_CHECKSUM R"(

	function testFunc()
		actualTorque = 40.95
		targetTorque = 32.76
		feedbackGearbox = 99.45

		motor6Data = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

 		motor6Data[2] = math.floor(targetTorque / 0.39)
		motor6Data[3] = math.floor(actualTorque / 0.39)
 		motor6Data[6] = math.floor(feedbackGearbox / 0.39)

		xorChecksum(motor6Data, 1)
		print(arrayToString(motor6Data))

		expected = { 0xC2, 0x54, 0x69, 0x00, 0x00, 0xFF, 0x00, 0x00 }
		return equals(motor6Data, expected)
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(script).value_or(0), 0);
}

TEST(LuaVag, ChecksumMotor6) {
	const char* realdata = VAG_CHECKSUM realMotor6Packet R"(

	function testFunc()
		return xorChecksum(data, 1)
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 0x3D);
}

#define realMotor5Packet "\ndata = { 0x1C, 0x08, 0xF3, 0x55, 0x19, 0x00, 0x06, 0xAD}\n "

TEST(LuaVag, ChecksumMotor5) {
	const char* realdata = VAG_CHECKSUM realMotor5Packet R"(

	function testFunc()
		return xorChecksum(data, 8)
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 0xAD);
}

TEST(LuaVag, unpackMotor5_fuel) {
	const char* script = 	GET_BIT_RANGE_LSB	realMotor5Packet	R"(
	function testFunc()
		fuelConsumption = getBitRange(data, 16, 15)
		return fuelConsumption
	end
	)";

    EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(script).value_or(0), 22003);
}
