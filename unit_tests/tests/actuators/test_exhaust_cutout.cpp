#include "pch.h"
#include "exhaust_cutout.h"

static void tick() {
	engine->module<ExhaustCutoutController>()->onSlowCallback();
}

// Switch activation with no pin → getInputHigh() always returns false → behaviorLow applies.
static void setupAutoMode() {
	engineConfiguration->exhaustCutoutEnabled = true;
	engineConfiguration->exhaustCutoutActivationMode = EXHAUST_CUTOUT_SWITCH;
	engineConfiguration->exhaustCutoutBehaviorLow = EXHAUST_CUTOUT_AUTO;
	engineConfiguration->exhaustCutoutBehaviorHigh = EXHAUST_CUTOUT_ALWAYS_OPEN;
}

TEST(ExhaustCutout, Disabled) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// exhaustCutoutEnabled defaults to false
	Sensor::setMockValue(SensorType::Rpm, 8000);
	Sensor::setMockValue(SensorType::MapSlow, 300);
	tick();

	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->isTriggerRpm);
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->isTriggerTps);
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->isTriggerMap);
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->targetOpen);
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->isCutoutOpen);
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->isCutoutMoving);
}

TEST(ExhaustCutout, ActivationModeOff) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// enabled flag set but activation mode = OFF → should behave like disabled
	engineConfiguration->exhaustCutoutEnabled = true;
	engineConfiguration->exhaustCutoutActivationMode = EXHAUST_CUTOUT_OFF;
	Sensor::setMockValue(SensorType::Rpm, 8000);
	tick();

	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->targetOpen);
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->isCutoutOpen);
}

TEST(ExhaustCutout, RpmTrigger) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupAutoMode();
	engineConfiguration->exhaustCutoutOpenRpm = 3000;

	// Below threshold: stays closed
	Sensor::setMockValue(SensorType::Rpm, 2000);
	tick();
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->isTriggerRpm);
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->targetOpen);
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->isCutoutOpen);

	// Above threshold: requests open, enters OPENING (isCutoutOpen=true, isCutoutMoving=true)
	Sensor::setMockValue(SensorType::Rpm, 4000);
	tick();
	EXPECT_TRUE(engine->module<ExhaustCutoutController>()->isTriggerRpm);
	EXPECT_TRUE(engine->module<ExhaustCutoutController>()->targetOpen);
	EXPECT_TRUE(engine->module<ExhaustCutoutController>()->isCutoutOpen);
	EXPECT_TRUE(engine->module<ExhaustCutoutController>()->isCutoutMoving);

	// Back below threshold (with no closing delay): requests close
	Sensor::setMockValue(SensorType::Rpm, 2000);
	tick();
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->isTriggerRpm);
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->targetOpen);
}

TEST(ExhaustCutout, TpsAntiBlip) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupAutoMode();
	engineConfiguration->exhaustCutoutOpenTps = 80;
	engineConfiguration->exhaustCutoutTpsDelayS = 1.0f;

	setTimeNowUs(0);

	// Tick with TPS below threshold to arm the hold timer at t=0
	Sensor::setMockValue(SensorType::Tps1, 20);
	tick();
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->isTriggerTps);

	// TPS above threshold but delay not elapsed → no trigger yet
	Sensor::setMockValue(SensorType::Tps1, 90);
	tick();
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->isTriggerTps);

	// After 0.5 s: still not triggered
	advanceTimeUs(0.5e6);
	tick();
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->isTriggerTps);
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->targetOpen);

	// After 1.1 s total: delay elapsed → trigger fires
	advanceTimeUs(0.6e6);
	tick();
	EXPECT_TRUE(engine->module<ExhaustCutoutController>()->isTriggerTps);
	EXPECT_TRUE(engine->module<ExhaustCutoutController>()->targetOpen);

	// TPS drops: trigger clears, hold timer resets
	Sensor::setMockValue(SensorType::Tps1, 20);
	tick();
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->isTriggerTps);

	// TPS back up: delay must re-elapse before triggering again
	Sensor::setMockValue(SensorType::Tps1, 90);
	tick();
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->isTriggerTps);
}

TEST(ExhaustCutout, MapTrigger) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupAutoMode();
	engineConfiguration->exhaustCutoutOpenMapKpa = 150.0f;

	// Below threshold: no trigger
	Sensor::setMockValue(SensorType::MapSlow, 100);
	tick();
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->isTriggerMap);
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->targetOpen);

	// Above threshold: trigger fires
	Sensor::setMockValue(SensorType::MapSlow, 200);
	tick();
	EXPECT_TRUE(engine->module<ExhaustCutoutController>()->isTriggerMap);
	EXPECT_TRUE(engine->module<ExhaustCutoutController>()->targetOpen);

	// Back below: closes (no closing delay configured)
	Sensor::setMockValue(SensorType::MapSlow, 100);
	tick();
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->isTriggerMap);
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->targetOpen);
}

TEST(ExhaustCutout, ClosingDelay) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupAutoMode();
	engineConfiguration->exhaustCutoutOpenRpm = 3000;
	engineConfiguration->exhaustCutoutClosingDelayS = 2.0f;

	setTimeNowUs(0);

	// Open it
	Sensor::setMockValue(SensorType::Rpm, 4000);
	tick();
	EXPECT_TRUE(engine->module<ExhaustCutoutController>()->targetOpen);

	// Trigger clears: cutout stays open during delay
	Sensor::setMockValue(SensorType::Rpm, 1000);
	tick();
	EXPECT_TRUE(engine->module<ExhaustCutoutController>()->targetOpen);

	// 1 second later: still open
	advanceTimeUs(1.0e6);
	tick();
	EXPECT_TRUE(engine->module<ExhaustCutoutController>()->targetOpen);

	// After delay elapses: finally closes
	advanceTimeUs(1.5e6);
	tick();
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->targetOpen);
}

TEST(ExhaustCutout, MoveDuration) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupAutoMode();
	engineConfiguration->exhaustCutoutOpenRpm = 3000;
	engineConfiguration->exhaustCutoutMoveDurationS = 1.0f;

	setTimeNowUs(0);

	// Trigger: enters OPENING (moving but not yet fully open)
	Sensor::setMockValue(SensorType::Rpm, 4000);
	tick();
	EXPECT_TRUE(engine->module<ExhaustCutoutController>()->isCutoutOpen);
	EXPECT_TRUE(engine->module<ExhaustCutoutController>()->isCutoutMoving);

	// Before duration elapses: still OPENING
	advanceTimeUs(0.5e6);
	tick();
	EXPECT_TRUE(engine->module<ExhaustCutoutController>()->isCutoutMoving);

	// After move duration elapses: transitions to OPEN (not moving)
	advanceTimeUs(0.6e6);
	tick();
	EXPECT_TRUE(engine->module<ExhaustCutoutController>()->isCutoutOpen);
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->isCutoutMoving);
}

TEST(ExhaustCutout, BehaviorAlwaysOpen) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->exhaustCutoutEnabled = true;
	engineConfiguration->exhaustCutoutActivationMode = EXHAUST_CUTOUT_SWITCH;
	engineConfiguration->exhaustCutoutBehaviorLow = EXHAUST_CUTOUT_ALWAYS_OPEN;

	// No sensors above any threshold, but behavior forces open
	tick();
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->isTriggerRpm);
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->isTriggerTps);
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->isTriggerMap);
	EXPECT_TRUE(engine->module<ExhaustCutoutController>()->targetOpen);
}

TEST(ExhaustCutout, BehaviorAlwaysClosed) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->exhaustCutoutEnabled = true;
	engineConfiguration->exhaustCutoutActivationMode = EXHAUST_CUTOUT_SWITCH;
	engineConfiguration->exhaustCutoutBehaviorLow = EXHAUST_CUTOUT_ALWAYS_CLOSED;
	engineConfiguration->exhaustCutoutOpenRpm = 1000;

	// RPM well above threshold but behavior forces closed
	Sensor::setMockValue(SensorType::Rpm, 8000);
	tick();
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->targetOpen);
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->isCutoutOpen);
}

TEST(ExhaustCutout, LuaGaugeInput) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->exhaustCutoutEnabled = true;
	engineConfiguration->exhaustCutoutActivationMode = EXHAUST_CUTOUT_LUA_GAUGE;
	engineConfiguration->exhaustCutoutLuaGauge = LUA_GAUGE_1;
	engineConfiguration->exhaustCutoutLuaGaugeMeaning = LUA_GAUGE_LOWER_BOUND;
	engineConfiguration->exhaustCutoutLuaGaugeThreshold = 0.5f;
	// When gauge is HIGH (>= threshold): always open; when LOW: always closed
	engineConfiguration->exhaustCutoutBehaviorHigh = EXHAUST_CUTOUT_ALWAYS_OPEN;
	engineConfiguration->exhaustCutoutBehaviorLow = EXHAUST_CUTOUT_ALWAYS_CLOSED;

	// Gauge below threshold: isInputHigh=false → ALWAYS_CLOSED
	Sensor::setMockValue(SensorType::LuaGauge1, 0.2f);
	tick();
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->isInputHigh);
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->targetOpen);

	// Gauge above threshold: isInputHigh=true → ALWAYS_OPEN
	Sensor::setMockValue(SensorType::LuaGauge1, 1.0f);
	tick();
	EXPECT_TRUE(engine->module<ExhaustCutoutController>()->isInputHigh);
	EXPECT_TRUE(engine->module<ExhaustCutoutController>()->targetOpen);

	// Gauge invalid: treated as low → ALWAYS_CLOSED
	Sensor::setInvalidMockValue(SensorType::LuaGauge1);
	tick();
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->isInputHigh);
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->targetOpen);
}

TEST(ExhaustCutout, MultiTriggerOr) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupAutoMode();
	engineConfiguration->exhaustCutoutOpenRpm = 3000;
	engineConfiguration->exhaustCutoutOpenMapKpa = 150.0f;

	// Only MAP trigger fires — cutout should open
	Sensor::setMockValue(SensorType::Rpm, 1000);
	Sensor::setMockValue(SensorType::MapSlow, 200);
	tick();
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->isTriggerRpm);
	EXPECT_TRUE(engine->module<ExhaustCutoutController>()->isTriggerMap);
	EXPECT_TRUE(engine->module<ExhaustCutoutController>()->targetOpen);

	// Both triggers fire — cutout still open
	Sensor::setMockValue(SensorType::Rpm, 4000);
	tick();
	EXPECT_TRUE(engine->module<ExhaustCutoutController>()->isTriggerRpm);
	EXPECT_TRUE(engine->module<ExhaustCutoutController>()->isTriggerMap);
	EXPECT_TRUE(engine->module<ExhaustCutoutController>()->targetOpen);

	// Both clear — closes (no closing delay)
	Sensor::setMockValue(SensorType::Rpm, 1000);
	Sensor::setMockValue(SensorType::MapSlow, 100);
	tick();
	EXPECT_FALSE(engine->module<ExhaustCutoutController>()->targetOpen);
}
