#include "rusefi_lua.h"
#include <gtest/gtest.h>
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
