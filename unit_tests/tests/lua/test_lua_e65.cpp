#include "pch.h"
#include "rusefi_lua.h"

#define BMW_CHECKSUM "	function bmwChecksum(canID, data, offset, length) \
		checksum = canID   \
		for i = offset, offset + length - 1,1 \
		do \
	   		checksum = checksum + data[i] \
		end \
		checksum = (checksum >> 8) + (checksum & 0xff) \
		return checksum \
	end "

#define TWO_BYTES "function twoBytes(data, offset, factor)        \
		return (data[offset + 2] * 256 + data[offset + 1]) * factor   \
	end"


#define GET_BIT_RANGE "function getBitRange(data, bitIndex, bitWidth)  \
	byteIndex = bitIndex >> 3  \
	shift = bitIndex - byteIndex * 8  \
	value = data[1 + byteIndex]  \
	if (shift + bitIndex > 8) then  \
		value = value + data[2 + byteIndex] * 256  \
	end  \
	mask = (1 << bitWidth) - 1  \
	return (value >> shift) & mask  \
end"

// https://github.com/HeinrichG-V12/E65_ReverseEngineering/blob/main/docs/0x3B4.md
TEST(LuaE65, Battery) {
	const char* realdata = TWO_BYTES R"(

	function testFunc()
		data = {0xdc, 0x03, 0x00, 0x53, 0xFE, 0xD3, 0x04, 0x00}
		return twoBytes(data, 0, 0.0147)
	end)";

	EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 14.5236);
}

// https://github.com/HeinrichG-V12/E65_ReverseEngineering/blob/main/docs/0x0A8.md
TEST(LuaE65, extractTorqueFromA8) {
	const char* realdata = TWO_BYTES R"(

function testFunc()
	data = { 0x42, 0x89, 0x10, 0x80, 0x10, 0x0F, 0x00, 0x60 }
	return 0.5 * (twoBytes(data, 1, 1) >> 4)
end
)";
			EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 0x108 / 2);
}

// http://loopybunny.co.uk/CarPC/can/0AA.html
TEST(LuaE65, Rpm) {
	const char* realdata = TWO_BYTES R"(

	function testFunc()
		data = {0x5F, 0x59, 0xFF, 0x00, 0x34, 0x0D, 0x80, 0x99}
		return twoBytes(data, 4, 0.25)
	end)";

	EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 845);
}

TEST(LuaE65, gear) {
	const char* realdata = TWO_BYTES R"(

	function testFunc()
		data = {0x58}
		return data[1] & 0xF
	end)";

	EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 8);
}

TEST(LuaE65, gearTorque) {
	const char* realdata = GET_BIT_RANGE R"(

	function testFunc()
		data = {0x9F, 0x01, 0x32, 0x20, 0x23, 0x30, 0xFF, 0x43}
		return getBitRange(data, 12, 12)
	end)";

	EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 800);
}


TEST(LuaE65, sumChecksum) {
	// checksum is first byte
	// id = A8, packet:
	// 12 AD 05 A0 05 0F 00 02

	const char* realdata = BMW_CHECKSUM R"(

	function testFunc()
	    canID = 0xA8
		data = { 0xAD, 0x05, 0xA0, 0x05, 0x0F, 0x00, 0x02 }
		checksum = bmwChecksum(canID, data, 1, 7)
		return checksum; 
	end)";

	EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(realdata).value_or(0), 0x12);
}
