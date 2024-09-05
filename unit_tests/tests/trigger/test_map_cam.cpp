

#include "pch.h"
#include "trigger_central.h"
#include "spark_logic.h"

TEST(trigger, map_cam_by_magic_point) {

	EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
	engineConfiguration->isFasterEngineSpinUpEnabled = false;

	engineConfiguration->camInputs[0] = Gpio::A0;
	engineConfiguration->vvtMode[0] = VVT_MAP_V_TWIN;
	eth.engine.periodicFastCallback(); // trigger limp mode
	ASSERT_FALSE(getLimpManager()->allowIgnition());
	ASSERT_FALSE(getLimpManager()->allowInjection());
	ASSERT_EQ(ClearReason::EnginePhase, getLimpManager()->allowIgnition().reason);
	ASSERT_EQ(ClearReason::EnginePhase, getLimpManager()->allowInjection().reason);

	engine->outputChannels.instantMAPValue = 100;

	engineConfiguration->mapCamDetectionAnglePosition = 90;

	eth.smartFireTriggerEvents2(/*count*/10, /*delayMs*/200);
	ASSERT_EQ(150,  Sensor::getOrZero(SensorType::Rpm)) << "RPM";

	ASSERT_EQ(1, engine->triggerCentral.mapVvt_map_peak);
	ASSERT_EQ(0, engine->triggerCentral.triggerState.camResyncCounter);

	// Nothing should have been scheduled yet
	ASSERT_EQ(1, engine->scheduler.size());
	scheduling_s* next = engine->scheduler.getForUnitTest(0);
	eth.assertEvent5("spark down#0", 0, (void*)fireSparkAndPrepareNextSchedule, 188333);

	engine->outputChannels.instantMAPValue = 120;
	eth.smartFireTriggerEvents2(/*count*/4, /*delayMs*/200);

	ASSERT_EQ(2, engine->triggerCentral.mapVvt_map_peak);
	ASSERT_EQ(1, engine->triggerCentral.triggerState.camResyncCounter);
	ASSERT_EQ(10, engine->triggerCentral.mapVvt_MAP_AT_CYCLE_COUNT);

	ASSERT_EQ(ClearReason::None, getLimpManager()->allowIgnition().reason);
	ASSERT_EQ(ClearReason::None, getLimpManager()->allowInjection().reason);

	// We have "VVT" sync, things should be scheduled!
	ASSERT_EQ(2, engine->scheduler.size());
	eth.assertEvent5("spark down#0", 0, (void*)turnSparkPinHighStartCharging, 185333);
	eth.assertEvent5("spark down#1", 1, (void*)fireSparkAndPrepareNextSchedule, 188333);
}
