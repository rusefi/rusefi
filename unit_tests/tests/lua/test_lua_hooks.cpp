#include "rusefi_lua.h"
#include <gtest/gtest.h>
#include "engine_test_helper.h"
#include "sensor.h"

static const char* getSensorTest = R"(

function testFunc()
	return getSensor(10)
end

)";

TEST(LuaHooks, TestGetSensor) {
	// Test failed sensor, returns nil
	Sensor::resetMockValue(static_cast<SensorType>(10));
	EXPECT_EQ(testLuaReturnsNumberOrNil(getSensorTest), unexpected);

	// Now test with a value, returns value
	Sensor::setMockValue(10, 33);
	EXPECT_EQ(testLuaReturnsNumberOrNil(getSensorTest).value_or(0), 33);
}

static const char* tableTest = R"(
function testFunc()
	return table3d(1, 1000, 40)
end
)";

TEST(LuaHooks, Table3d) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	EXPECT_EQ(testLuaReturnsNumber(tableTest), 33);
}
