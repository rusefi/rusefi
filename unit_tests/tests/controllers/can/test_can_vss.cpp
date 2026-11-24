/*
 * @file test_can_vss.cpp
 *
 * Vehicle speed decoded from CAN
 */

#include "pch.h"

#include "can_vss.h"

// Coverage of a present defect, asserting current behavior (the expectation to move to with the
// fix is noted at the assertion).
// BMW E90 0x1A0: speed is the low 12 bits, the upper nibble of byte 1 carries direction/brake
// flags which are decoded as part of the speed. FOME commit 13e334493d.
TEST(CanVss, bmwE90SpeedIncludesStatusBits) {
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
	// Expected after the fix: 50
	EXPECT_NEAR(50 + 409.6, Sensor::getOrZero(SensorType::VehicleSpeed), 0.01);
}
