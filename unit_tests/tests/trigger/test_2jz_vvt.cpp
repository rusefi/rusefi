/*
 * @file test_2jz_vvt.cpp
 *
 *  Created on: May 10, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

TEST(sensors, test2jz) {
	EngineTestHelper eth(engine_type_e::TOYOTA_2JZ_GTE_VVTi);
	engineConfiguration->isFasterEngineSpinUpEnabled = false;

	eth.setTriggerType(trigger_type_e::TT_ONE);

	ASSERT_EQ( 0,  Sensor::getOrZero(SensorType::Rpm)) << "test2jz RPM";
	for (int i = 0; i < 2; i++) {
		eth.fireRise(12.5);
		eth.fireFall(12.5);
		ASSERT_EQ( 0,  Sensor::getOrZero(SensorType::Rpm)) << "test2jz RPM at " << i;
	}
	eth.fireRise(12.5);
	eth.fireFall(12.5);
	// first time we have RPM
	ASSERT_EQ(2400,  Sensor::getOrZero(SensorType::Rpm)) << "test2jz RPM";


	eth.moveTimeForwardUs(MS2US(3)); // shifting VVT phase a few angles

	hwHandleVvtCamSignal(false, getTimeNowNt(), 0);
	hwHandleVvtCamSignal(true, getTimeNowNt(), 0);

	// Expected angle is 12.5ms + 3ms of a 25ms revolution = 15.5/25 = 223.2 degrees from the sync point
	// Minus 155 degree trigger offset = 68.2
	ASSERT_NEAR(68.2f, engine->triggerCentral.currentVVTEventPosition[0][0], EPS3D);
}
