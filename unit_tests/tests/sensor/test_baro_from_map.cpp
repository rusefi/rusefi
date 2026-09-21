/**
 * @file test_baro_from_map.cpp
 *
 * Tests for 'useFixedBaroCorrFromMap' - see https://github.com/rusefi/rusefi/issues/9744
 */

#include "pch.h"

// mimics the real start-up order: sensors are registered, then initMapDecoder() runs before
// the ADC has delivered a single MAP sample
static void initWithBaroFromMap(bool enabled) {
	engineConfiguration->useFixedBaroCorrFromMap = enabled;
	Sensor::resetMockValue(SensorType::BarometricPressure);
	Sensor::setInvalidMockValue(SensorType::MapSlow);

	initMapDecoder();
}

TEST(BaroFromMap, latchesFirstValidMapSample) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	initWithBaroFromMap(true);

	// nothing to grab yet: baro must not be latched at the STD_ATMOSPHERE placeholder
	EXPECT_FALSE(Sensor::hasSensor(SensorType::BarometricPressure));
	updateFixedBaroFromMap();
	EXPECT_FALSE(Sensor::hasSensor(SensorType::BarometricPressure));

	// MAP finally reports the actual atmospheric pressure
	Sensor::setMockValue(SensorType::MapSlow, 95);
	updateFixedBaroFromMap();

	ASSERT_TRUE(Sensor::hasSensor(SensorType::BarometricPressure));
	EXPECT_FLOAT_EQ(95, Sensor::getOrZero(SensorType::BarometricPressure));

	// the value is latched once and does not follow MAP afterwards
	Sensor::setMockValue(SensorType::MapSlow, 30);
	updateFixedBaroFromMap();
	EXPECT_FLOAT_EQ(95, Sensor::getOrZero(SensorType::BarometricPressure));
}

TEST(BaroFromMap, disabledDoesNotTouchBaro) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	initWithBaroFromMap(false);

	Sensor::setMockValue(SensorType::MapSlow, 95);
	updateFixedBaroFromMap();

	EXPECT_FALSE(Sensor::hasSensor(SensorType::BarometricPressure));
}

TEST(BaroFromMap, implausibleReadingIsRejected) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	initWithBaroFromMap(true);

	// way below the lowest plausible atmospheric pressure
	Sensor::setMockValue(SensorType::MapSlow, 40);
	updateFixedBaroFromMap();
	EXPECT_FALSE(Sensor::hasSensor(SensorType::BarometricPressure));

	// and we do not keep retrying afterwards either
	Sensor::setMockValue(SensorType::MapSlow, 95);
	updateFixedBaroFromMap();
	EXPECT_FALSE(Sensor::hasSensor(SensorType::BarometricPressure));
}

TEST(BaroFromMap, doesNotGrabWhileEngineTurns) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	initWithBaroFromMap(true);

	// MAP no longer reads atmosphere once the engine is turning
	Sensor::setMockValue(SensorType::Rpm, 300);
	Sensor::setMockValue(SensorType::MapSlow, 95);
	updateFixedBaroFromMap();

	EXPECT_FALSE(Sensor::hasSensor(SensorType::BarometricPressure));
}

TEST(BaroFromMap, givesUpAfterTimeout) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	initWithBaroFromMap(true);

	updateFixedBaroFromMap();
	eth.moveTimeForwardSec(10);
	updateFixedBaroFromMap();

	// MAP shows up way too late to be trusted as a start-up reading
	Sensor::setMockValue(SensorType::MapSlow, 95);
	updateFixedBaroFromMap();
	EXPECT_FALSE(Sensor::hasSensor(SensorType::BarometricPressure));
}
