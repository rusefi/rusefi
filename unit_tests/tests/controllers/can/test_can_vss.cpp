/*
 * @file test_can_vss.cpp
 *
 * Vehicle speed decoded from CAN
 */

#include "pch.h"

#include "can_vss.h"

// BMW E90 0x1A0: speed is the low 12 bits, the upper nibble of byte 1 carries direction/brake
// flags which are not part of the speed.
TEST(CanVss, bmwE90SpeedIgnoresStatusBits) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->enableCanVss = true;
	engineConfiguration->canVssNbcType = BMW_e90;
	engineConfiguration->canVssScaling = 1;
	initCanVssSupport();

	CANRxFrame frame{};
	frame.SID = 0x1A0;
	frame.DLC = 8;

	// 50.0 km/h, no flags
	frame.data8[0] = 0xF4;
	frame.data8[1] = 0x01;
	processCanRxVss(frame, getTimeNowNt());
	EXPECT_NEAR(50, Sensor::getOrZero(SensorType::VehicleSpeed), 0.01);

	// same speed, with bit 12 (moving forward) set
	frame.data8[1] = 0x11;
	processCanRxVss(frame, getTimeNowNt());
	EXPECT_NEAR(50, Sensor::getOrZero(SensorType::VehicleSpeed), 0.01);

	// all flags set, top of the 12-bit range: 409.5 km/h
	frame.data8[0] = 0xFF;
	frame.data8[1] = 0xFF;
	processCanRxVss(frame, getTimeNowNt());
	EXPECT_NEAR(409.5, Sensor::getOrZero(SensorType::VehicleSpeed), 0.01);
}
