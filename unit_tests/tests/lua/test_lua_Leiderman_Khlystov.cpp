#include "pch.h"
#include "rusefi_lua.h"
#include "lua_lib.h"


// Leiderman-Khlystov Coefficients for Estimating Engine Full Load Characteristics and Performance
TEST(LuaVag, LeidermaKhlystov) {
	const char* magic = LUA_POW R"(


currentRpm = 2000

maxPowerHp = 148
maxPowerRpm = 6000 
maxTorqueNm = 147
maxTorqueRpm = 3500


maxPowerKw = maxPowerHp * 0.7355


torqAtMaxPower = maxPowerKw * 9549 / maxPowerRpm
print('torqAtMaxPower ' ..torqAtMaxPower)

rpmCoef = maxPowerRpm / maxTorqueRpm
print('rpmCoef ' ..rpmCoef)

torqCoef = maxTorqueNm / torqAtMaxPower
print('torqCoef ' ..torqCoef)

torquePotential =(torqCoef -1) * 100
zz =(100 *(rpmCoef -1) *(rpmCoef -1))
print('torquePotential ' ..torquePotential)
print('zz ' ..zz)

ax = 1 -((torquePotential * rpmCoef *(2 - rpmCoef)) / zz)
bx = 2 *((torquePotential * rpmCoef) / zz)
cx =(torquePotential * rpmCoef * rpmCoef) / zz

print('ax ' ..ax .. ', bx ' ..bx .. ', cx ' ..cx)

rpmRatio = currentRpm / maxPowerRpm
abcMult = ax * rpmRatio + bx * pow(rpmRatio, 2) - cx * pow(rpmRatio, 3)
print ('abcMult ' ..abcMult)

print('rpmRatio ' .. rpmRatio)

LeidermanPower = maxPowerKw * abcMult
print('LeidermanPower ' .. LeidermanPower)

LeidermanTorque = (9550 * LeidermanPower) / currentRpm
print('LeidermanTorque ' .. LeidermanTorque)

	function testFunc()
		return LeidermanTorque
	end

	)";

	EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(magic).value_or(0), 156.463);
}


TEST(LuaVag, LeidermaKhlystov2) {
	const char* magic = LUA_POW R"(


currentRpm = 2000

maxPowerHp = 148
maxPowerRpm = 6000 
maxTorqueNm = 147
maxTorqueRpm = 3500


maxPowerKw = maxPowerHp * 0.7355


torqAtMaxPower = maxPowerKw * 9549 / maxPowerRpm
print('torqAtMaxPower ' ..torqAtMaxPower)

rpmCoef = maxPowerRpm / maxTorqueRpm
print('rpmCoef ' ..rpmCoef)

torqCoef = maxTorqueNm / torqAtMaxPower
print('torqCoef ' ..torqCoef)

torquePotential =(torqCoef -1) * 100

print('torquePotential ' ..torquePotential)

ax=2-(25/torquePotential);
bx=(50/torquePotential) - 1;
cx=25/torquePotential;

print('ax ' ..ax .. ', bx ' ..bx .. ', cx ' ..cx)

rpmRatio = currentRpm / maxPowerRpm
abcMult = ax * rpmRatio + bx * pow(rpmRatio, 2) - cx * pow(rpmRatio, 3)
print ('abcMult ' ..abcMult)

print('rpmRatio ' .. rpmRatio)

LeidermanPower = maxPowerKw * abcMult
print('LeidermanPower ' .. LeidermanPower)

LeidermanTorque = (9550 * LeidermanPower) / currentRpm
print('LeidermanTorque ' .. LeidermanTorque)

	function testFunc()
		return LeidermanTorque
	end

	)";

	EXPECT_NEAR_M3(testLuaReturnsNumberOrNil(magic).value_or(0), 415.858);
}
