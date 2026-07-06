

#include "pch.h"
#include "trigger_central.h"
#include "spark_logic.h"

TEST(trigger, map_cam_by_magic_point) {

	EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
	engineConfiguration->isFasterEngineSpinUpEnabled = false;

  engineConfiguration->isPhaseSyncRequiredForIgnition = true;

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

	ASSERT_EQ(1, engine->triggerCentral.mapVvt_sync_counter);
	ASSERT_EQ(0, engine->triggerCentral.triggerState.phaseResyncCounter);

	// Nothing should have been scheduled yet
	ASSERT_EQ(1, engine->scheduler.size());
	scheduling_s* next = engine->scheduler.getForUnitTest(0);
	auto const fireSparkAndPrepareNextScheduleAction{ action_s::make<fireSparkAndPrepareNextSchedule>((IgnitionEvent*){})};

	eth.assertEvent5("spark down#0", 0, fireSparkAndPrepareNextScheduleAction, 193333);

	engine->outputChannels.instantMAPValue = 120;
	eth.smartFireTriggerEvents2(/*count*/4, /*delayMs*/200);

	ASSERT_EQ(2, engine->triggerCentral.mapVvt_sync_counter);
	ASSERT_EQ(1, engine->triggerCentral.triggerState.phaseResyncCounter);
	ASSERT_EQ(10, engine->triggerCentral.mapVvt_MAP_AT_CYCLE_COUNT);

	ASSERT_EQ(ClearReason::None, getLimpManager()->allowIgnition().reason);
	ASSERT_EQ(ClearReason::None, getLimpManager()->allowInjection().reason);

	// NOTHING TO DO HERE - this loop was dangerous and unnecessary.
	// printf("Scheduler size: %d\n", engine->scheduler.size());
	// for (size_t i = 0; i < (size_t)engine->scheduler.size(); i++) {
	// 	printf("%d %s\n", (int)i, engine->scheduler.getForUnitTest(i)->action.getCallbackName());
	// }

	// We have "VVT" sync, things should be scheduled!
	ASSERT_EQ(4, engine->scheduler.size());
	auto const turnSparkPinHighStartChargingAction{ action_s::make<turnSparkPinHighStartCharging>((IgnitionEvent*){})};
	// these moments are computed at the true 150 RPM: the cycle-RPM sample disturbed by the
	// phase re-sync (which used to read a doubled 300 RPM) is discarded now
	eth.assertEvent5("spark down#0", 2, turnSparkPinHighStartChargingAction, 187333);
	eth.assertEvent5("spark down#1", 3, fireSparkAndPrepareNextScheduleAction, 193333);
}

// Dashboard RPM used to momentarily read exactly DOUBLE the true value the moment full
// 720-degree engine phase was acquired mid-revolution, see #9779.
//
// Mechanism: on a crank-speed trigger the engine-cycle event index is
//   CurrentIndex + (synchronizationCounter % crankDivider) * triggerShape.getSize()
// and cycle RPM is sampled each time that index wraps to 0, assuming the elapsed period spans a
// full engine cycle (rpm = 60 * mult / periodSeconds with mult = 2 for a four stroke).
// PrimaryTriggerDecoder::syncEnginePhase() bumps synchronizationCounter to fix the crank parity,
// so the next index-0 arrives only 360 crank degrees after the previous one while the RPM math
// still assumes 720 - one cycle-RPM sample used to read exactly 2x. Beyond the dash glitch, the
// bogus sample fed setRpmValue(), so it could prematurely flip CRANKING to RUNNING and mis-time
// scheduling for that cycle. The fix discards the one cycle-RPM sample whose period was
// disturbed by the re-sync.
TEST(trigger, rpmDuringEnginePhaseResync) {
	EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
	engineConfiguration->isFasterEngineSpinUpEnabled = false;

	engineConfiguration->camInputs[0] = Gpio::A0;
	engineConfiguration->vvtMode[0] = VVT_MAP_V_TWIN;

	engine->outputChannels.instantMAPValue = 100;
	engineConfiguration->mapCamDetectionAnglePosition = 90;

	eth.smartFireTriggerEvents2(/*count*/10, /*delayMs*/200);
	ASSERT_EQ(150, Sensor::getOrZero(SensorType::Rpm));
	ASSERT_EQ(0, engine->triggerCentral.triggerState.phaseResyncCounter);

	// MAP pattern now disagrees with the currently assumed phase, so the next magic-point
	// detection re-syncs the engine phase mid-cycle
	engine->outputChannels.instantMAPValue = 120;

	// keep cranking at exactly the same speed across the phase re-sync and watch RPM
	// crank speed never changes, so RPM must stay at 150 on every single event
	// (it used to read exactly 2 x 150 = 300 on the sample disturbed by the re-sync)
	float maxRpm = 0;
	for (int i = 0; i < 8; i++) {
		eth.smartFireTriggerEvents2(/*count*/1, /*delayMs*/200);
		maxRpm = std::max(maxRpm, Sensor::getOrZero(SensorType::Rpm));
		EXPECT_NEAR(150, Sensor::getOrZero(SensorType::Rpm), 1) << "at event " << i;
	}

	// the re-sync did happen during those events
	ASSERT_EQ(1, engine->triggerCentral.triggerState.phaseResyncCounter);

	ASSERT_EQ(150, maxRpm);

	ASSERT_EQ(150, Sensor::getOrZero(SensorType::Rpm));
}
