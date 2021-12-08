

#include "pch.h"
#include "trigger_central.h"

static int getZigZag(int index) {
	index = index % 1000;

	if (index < 400) {
		// going up from 0 to 400
		return index;
	}
	if (index < 500) {
		// going down from 400 to 300
		return 800 - index;
	}
	if (index < 600) {
		// going up from 300 to 400
		return index - 200;
	}
	// going down from 400 to 0
	return 1000 - index;
}

TEST(trigger, map_cam) {
	EngineTestHelper eth(TEST_ENGINE);

	engineConfiguration->mapCamAveragingLength = 8;
	MapState state;

	int i = 0;
	for (;i<403;i++) {
		state.add(getZigZag(i));

		if (state.mapBuffer.getCount() > engineConfiguration->mapCamAveragingLength) {
			ASSERT_FALSE(state.isPeak(false)) << "high At " << i;
			ASSERT_FALSE(state.isPeak(true)) << "low At " << i;
		}
	}

	state.add(getZigZag(i));
	ASSERT_FALSE(state.isPeak(false)) << "high At " << i;
	ASSERT_FALSE(state.isPeak(true)) << "low At " << i;
	i++;


	state.add(getZigZag(i));
	ASSERT_TRUE(state.isPeak(false)) << "high At " << i;
	ASSERT_FALSE(state.isPeak(true)) << "low At " << i;

	for (;i<504;i++) {
		state.add(getZigZag(i));
		ASSERT_FALSE(state.isPeak(false)) << "high At " << i;
		ASSERT_FALSE(state.isPeak(true)) << "low At " << i;
	}

	state.add(getZigZag(i));
	ASSERT_FALSE(state.isPeak(false)) << "high At " << i;
	ASSERT_TRUE(state.isPeak(true)) << "low At " << i;
	i++;

	for (;i<604;i++) {
		state.add(getZigZag(i));
		ASSERT_FALSE(state.isPeak(false)) << "high At " << i;
		ASSERT_FALSE(state.isPeak(true)) << "low At " << i;
	}

	state.add(getZigZag(i));
	ASSERT_TRUE(state.isPeak(false)) << "high At " << i;
	ASSERT_TRUE(state.isPeak(false)) << "low At " << i;
}

TEST(trigger, map_cam_by_magic_point) {

	EngineTestHelper eth(TEST_ENGINE);

	engineConfiguration->camInputs[0] = GPIOA_0;
	engineConfiguration->vvtMode[0] = VVT_MAP_V_TWIN_ANOTHER;

	Sensor::setMockValue(SensorType::Map, 100);

	engineConfiguration->mapCamDetectionAnglePosition = 90;

	eth.smartFireTriggerEvents2(/*count*/10, /*delayMs*/200);
	ASSERT_EQ( 150,  GET_RPM()) << "RPM";
	ASSERT_EQ(1, engine->triggerCentral.mapCamCounter);


	Sensor::setMockValue(SensorType::Map, 120);
	eth.smartFireTriggerEvents2(/*count*/2, /*delayMs*/200);
	ASSERT_EQ(2, engine->triggerCentral.mapCamCounter);


}
