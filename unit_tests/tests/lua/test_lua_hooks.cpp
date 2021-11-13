#include "pch.h"

#include "rusefi_lua.h"

static const char* getSensorTestByIndex = R"(

function testFunc()
	return getSensorByIndex(10)
end

)";

TEST(LuaHooks, TestGetSensorByIndex) {
	// Test failed sensor, returns nil
	Sensor::resetMockValue(static_cast<SensorType>(10));
	EXPECT_EQ(testLuaReturnsNumberOrNil(getSensorTestByIndex), unexpected);

	// Now test with a value, returns value
	Sensor::setMockValue(10, 33);
	EXPECT_EQ(testLuaReturnsNumberOrNil(getSensorTestByIndex).value_or(0), 33);
}

static const char* getSensorTestByName = R"(

function testFunc()
	return getSensor("CLT")
end

)";

TEST(LuaHooks, TestGetSensorByName) {
	// Test failed sensor, returns nil
	Sensor::resetMockValue(SensorType::Clt);
	EXPECT_EQ(testLuaReturnsNumberOrNil(getSensorTestByName), unexpected);

	// Now test with a value, returns value
	Sensor::setMockValue((int)SensorType::Clt, 33);
	EXPECT_EQ(testLuaReturnsNumberOrNil(getSensorTestByName).value_or(0), 33);
}

static const char* tableTest = R"(
function testFunc()
	return table3d(1, 1000, 40)
end
)";

TEST(LuaHooks, Table3d) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	setTable(config->fsioTable2, (uint8_t)33);
	EXPECT_EQ(testLuaReturnsNumber(tableTest), 33);

	setTable(config->fsioTable2, (uint8_t)14);
	EXPECT_EQ(testLuaReturnsNumber(tableTest), 14);
}

TEST(LuaHooks, CanTxChannel) {
	// channel 1 valid
	EXPECT_NO_THROW(testLuaExecString("txCan(1, 0, 0, {0})"));

	// channel 0 invalid
	EXPECT_ANY_THROW(testLuaExecString("txCan(0, 0, 0, {0})"));
}

TEST(LuaHooks, CanTxId) {
	// std ID ok
	EXPECT_NO_THROW(testLuaExecString("txCan(1, 2047, 0, {0})"));
	// std ID too high
	EXPECT_ANY_THROW(testLuaExecString("txCan(1, 2048, 0, {0})"));

	// ext ID ok
	EXPECT_NO_THROW(testLuaExecString("txCan(1, 536870911, 1, {0})"));
	// ext ID too high
	EXPECT_ANY_THROW(testLuaExecString("txCan(1, 536870912, 1, {0})"));
}

TEST(LuaHooks, CanTxDataLength) {
	// valid: DLC = 0
	EXPECT_NO_THROW(testLuaExecString("txCan(1, 0, 0, {})"));
	// valid: DLC = 1
	EXPECT_NO_THROW(testLuaExecString("txCan(1, 0, 0, {0})"));
	// valid: DLC = 8
	EXPECT_NO_THROW(testLuaExecString("txCan(1, 0, 0, {1, 2, 3, 4, 5, 6, 7, 8})"));

	// invalid: DLC = 9
	EXPECT_ANY_THROW(testLuaExecString("txCan(1, 0, 0, {1, 2, 3, 4, 5, 6, 7, 8, 9})"));

	// invalid: is a table, but contains a not-number
	EXPECT_ANY_THROW(testLuaExecString("txCan(1, 0, 0, {1, 2, 3, 'hello', 5, 6})"));

	// invalid: not a table
	EXPECT_ANY_THROW(testLuaExecString("txCan(1, 0, 0, 26)"));
}

static const char* interpolationTest = R"(
function testFunc()
	return interpolate(1, 10, 5, 50, 3)
end
)";

TEST(LuaHooks, LuaInterpolate) {
	EXPECT_EQ(testLuaReturnsNumber(interpolationTest), 30);
}

static const char* timerTest = R"(
function testFunc()
	local a = Timer.new()
	a:reset()
	return a:getElapsedSeconds()
end
)";

TEST(LuaHooks, TestLuaTimer) {
	EXPECT_EQ(testLuaReturnsNumber(timerTest), 0);
}

static const char* sensorTest = R"(
function testFunc()
	local sens = Sensor.new("CLT")
	
	-- Check valid sensor
	sens:set(33)
	if getSensor("CLT") ~= 33 then
		return 1
	end

	-- Check invalidation
	sens:invalidate()
	if getSensor("CLT") then
		return 2
	end

	return 0
end
)";

TEST(LuaHooks, LuaSensor) {
	EXPECT_EQ(testLuaReturnsNumber(sensorTest), 0);

	// Ensure that the sensor got unregistered on teardown of the Lua interpreter
	EXPECT_FALSE(Sensor::hasSensor(SensorType::Clt));
}

static const char* pidTest = R"(
function testFunc()
	local pid = Pid.new(0.5, 0, 0, -10, 10)

	-- delta is -4, output -2
	if pid:get(3, 7) ~= -2 then
		return 1
	end

	-- delta is 6, output 3
	if pid:get(4, -2) ~= 3 then
		return 2
	end

	-- test clamping
	if pid:get(0, 100) ~= -10 then
		return 3
	end

	if pid:get(0, -100) ~= 10 then
		return 4
	end

	return 0
end
)";

TEST(LuaHooks, LuaPid) {
	EXPECT_EQ(testLuaReturnsNumber(pidTest), 0);
}
