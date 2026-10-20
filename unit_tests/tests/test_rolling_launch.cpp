#include "pch.h"
#include "rolling_launch.h"
#include "fuel_math.h"
#include "script_impl.h"
#include "rusefi_lua.h"

// Rolling Launch Control: a button-held, captured-RPM hold (via the shared hard spark limiter)
// to spool the turbo while the car is moving. Snapshots RPM at the press edge, holds at
// capturedRpm + window (clamped to maxRpm), pulls flat timing + adds fuel while held, and on
// release ends the hold immediately while ramping the pulled timing back to zero.

static RollingLaunchControl& getRl() {
	return engine->module<RollingLaunchControl>().unmock();
}

static void setupRl() {
	auto* d = engineConfiguration;
	d->rollingLaunchEnabled = true;
	d->rollingLaunchMinVss = 10;          // km/h
	d->rollingLaunchMinArmRpm = 2500;
	d->rollingLaunchRpmWindow = 500;
	d->rollingLaunchMaxRpm = 7000;
	d->rollingLaunchTimingRetard = 10;    // deg
	d->rollingLaunchFuelAdderPercent = 5; // %
	d->rollingLaunchRampOutTime = 1.0f;   // s
}

// Drive RPM, VSS and the Lua activation, then run the slow callback the module hooks into.
static void step(float rpm, float vss, bool active) {
	Sensor::setMockValue(SensorType::Rpm, rpm);
	Sensor::setMockValue(SensorType::VehicleSpeed, vss);
	getRl().luaRollingLaunchState = active;
	getRl().onSlowCallback();
}

TEST(RollingLaunch, doesNotArmBelowSpeedOrRpm) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupRl();

	// Button held but stationary -> not armed (this is the "rolling" gate).
	step(3500, 0, true);
	EXPECT_FALSE(getRl().isRollingLaunchArmed);
	EXPECT_NEAR(getRl().getSparkSkipRatio(), 0.0f, 1e-3);

	// Button held, moving, but lugging below the min arm RPM -> not armed.
	step(1500, 30, true);
	EXPECT_FALSE(getRl().isRollingLaunchArmed);

	// Button held, moving, above min arm RPM -> armed.
	step(3500, 30, true);
	EXPECT_TRUE(getRl().isRollingLaunchArmed);
}

TEST(RollingLaunch, capturesAndClampsTarget) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupRl();

	// Press at 3500 with a 500 window -> target 4000.
	step(3500, 30, true);
	EXPECT_NEAR(getRl().capturedRpm, 3500.0f, 1e-3);
	EXPECT_NEAR(getRl().rollingLaunchTargetRpm, 4000.0f, 1e-3);

	// Release, then press again near redline -> target clamps to the max-RPM ceiling.
	step(3500, 30, false);
	step(6800, 30, true);
	EXPECT_NEAR(getRl().capturedRpm, 6800.0f, 1e-3);
	EXPECT_NEAR(getRl().rollingLaunchTargetRpm, 7000.0f, 1e-3); // min(6800+500, 7000)
}

TEST(RollingLaunch, sparkSkipRampsAcrossWindow) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupRl();

	// Capture at 3500 -> [3500, 4000].
	step(3500, 30, true);
	EXPECT_NEAR(getRl().getSparkSkipRatio(), 0.0f, 1e-3);

	// Midway through the window -> ~0.5.
	step(3750, 30, true);
	EXPECT_NEAR(getRl().getSparkSkipRatio(), 0.5f, 1e-3);

	// At/above the target -> full cut.
	step(4000, 30, true);
	EXPECT_NEAR(getRl().getSparkSkipRatio(), 1.0f, 1e-3);
	step(4500, 30, true);
	EXPECT_NEAR(getRl().getSparkSkipRatio(), 1.0f, 1e-3);
}

TEST(RollingLaunch, fuelCoefficientOnlyWhileHeld) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupRl();

	// Not armed -> no enrichment.
	step(3500, 0, true);
	EXPECT_NEAR(getRl().getFuelCoefficient(), 1.0f, 1e-3);

	// Armed -> +5%.
	step(3500, 30, true);
	EXPECT_NEAR(getRl().getFuelCoefficient(), 1.05f, 1e-3);

	// Released -> back to 1.0.
	step(3500, 30, false);
	EXPECT_NEAR(getRl().getFuelCoefficient(), 1.0f, 1e-3);
}

TEST(RollingLaunch, releaseDropsHoldAndRampsOutTiming) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupRl();

	// Capture at 3500 -> target 4000; flat retard applies immediately while armed.
	step(3500, 30, true);
	EXPECT_NEAR(getRl().getTimingRetard(), 10.0f, 1e-3);

	// Rev to the hold target -> full spark cut, retard still flat at 10.
	step(4000, 30, true);
	EXPECT_NEAR(getRl().getTimingRetard(), 10.0f, 1e-3);
	EXPECT_NEAR(getRl().getSparkSkipRatio(), 1.0f, 1e-3);

	// Release: hold ends immediately (no spark cut), ramp starts at the full pulled value.
	step(4000, 30, false);
	EXPECT_FALSE(getRl().isRollingLaunchArmed);
	EXPECT_NEAR(getRl().getSparkSkipRatio(), 0.0f, 1e-3);
	EXPECT_TRUE(getRl().isRollingLaunchRampActive);
	EXPECT_NEAR(getRl().getTimingRetard(), 10.0f, 0.1f);

	// Halfway through the 1s ramp-out -> ~5 deg.
	eth.moveTimeForwardSec(0.5f);
	getRl().onSlowCallback();
	EXPECT_NEAR(getRl().getTimingRetard(), 5.0f, 0.2f);

	// Past the ramp-out time -> deactivates, no retard.
	eth.moveTimeForwardSec(0.6f);
	getRl().onSlowCallback();
	EXPECT_FALSE(getRl().isRollingLaunchRampActive);
	EXPECT_NEAR(getRl().getTimingRetard(), 0.0f, 1e-3);
}

TEST(RollingLaunch, disabledDoesNothing) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupRl();
	engineConfiguration->rollingLaunchEnabled = false;

	step(4000, 30, true);
	EXPECT_FALSE(getRl().isRollingLaunchArmed);
	EXPECT_NEAR(getRl().getSparkSkipRatio(), 0.0f, 1e-3);
	EXPECT_NEAR(getRl().getFuelCoefficient(), 1.0f, 1e-3);
	EXPECT_NEAR(getRl().getTimingRetard(), 0.0f, 1e-3);
}

TEST(RollingLaunch, CapturedRpmAboveCeiling) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupRl();
	step(7500, 30, true);
	ASSERT_FLOAT_EQ(getRl().rollingLaunchTargetRpm, 7000);
	ASSERT_FLOAT_EQ(getRl().getSparkSkipRatio(), 1);
	step(7200, 30, true);
	// Stay fully cut above the capped target, even below the captured RPM.
	EXPECT_FLOAT_EQ(getRl().getSparkSkipRatio(), 1);
	step(6999, 30, true);
	EXPECT_FLOAT_EQ(getRl().getSparkSkipRatio(), 0);
}

TEST(RollingLaunch, MissingSensorsWithZeroGates) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupRl();
	engineConfiguration->rollingLaunchMinArmRpm = 0;
	engineConfiguration->rollingLaunchMinVss = 0;
	Sensor::setInvalidMockValue(SensorType::Rpm);
	Sensor::setInvalidMockValue(SensorType::VehicleSpeed);
	getRl().luaRollingLaunchState = true;
	getRl().onSlowCallback();
	// Missing sensors must not satisfy zero-valued gates.
	EXPECT_FALSE(getRl().isRollingLaunchArmed);
}

TEST(RollingLaunch, UnassignedPinClearsSwitchState) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupRl();
	getRl().rollingLaunchSwitchState = true;
	step(3500, 30, false);
	// Removing a pin must not retain a previously pressed switch state.
	EXPECT_FALSE(getRl().isRollingLaunchArmed);
	EXPECT_FALSE(getRl().rollingLaunchSwitchState);
}

TEST(RollingLaunch, DefaultsRemainDisabled) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	EXPECT_FALSE(engineConfiguration->rollingLaunchEnabled);
	EXPECT_EQ(engineConfiguration->rollingLaunchActivatePin, Gpio::Unassigned);
	EXPECT_EQ(engineConfiguration->rollingLaunchRpmWindow, 500);
	EXPECT_EQ(engineConfiguration->rollingLaunchMinArmRpm, 2500);
	EXPECT_EQ(engineConfiguration->rollingLaunchMaxRpm, 7000);
	EXPECT_EQ(engineConfiguration->rollingLaunchMinVss, 10);
	EXPECT_EQ(engineConfiguration->rollingLaunchTimingRetard, 10);
	EXPECT_EQ(engineConfiguration->rollingLaunchFuelAdderPercent, 5);
	EXPECT_FLOAT_EQ(engineConfiguration->rollingLaunchRampOutTime, 1);
}

TEST(RollingLaunch, ZeroWindowAndZeroReleaseTime) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupRl();
	engineConfiguration->rollingLaunchRpmWindow = 0;
	engineConfiguration->rollingLaunchRampOutTime = 0;
	step(3500, 30, true);
	EXPECT_FLOAT_EQ(getRl().getSparkSkipRatio(), 1);
	step(3400, 30, true);
	EXPECT_FLOAT_EQ(getRl().getSparkSkipRatio(), 0);
	step(3500, 30, false);
	EXPECT_FALSE(getRl().isRollingLaunchRampActive);
	EXPECT_FLOAT_EQ(getRl().getTimingRetard(), 0);
}

TEST(RollingLaunch, GateLossReleasesAndRecoveryRecaptures) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupRl();
	// Inclusive gate boundaries arm even if the trigger was already held.
	step(2499, 10, true);
	EXPECT_FALSE(getRl().isRollingLaunchArmed);
	step(2500, 10, true);
	ASSERT_TRUE(getRl().isRollingLaunchArmed);
	step(3000, 9, true);
	EXPECT_FALSE(getRl().isRollingLaunchArmed);
	EXPECT_TRUE(getRl().isRollingLaunchRampActive);
	EXPECT_FLOAT_EQ(getRl().getFuelCoefficient(), 1);
	EXPECT_FLOAT_EQ(getRl().getSparkSkipRatio(), 0);
	eth.moveTimeForwardSec(0.5f);
	getRl().onSlowCallback();
	EXPECT_NEAR(getRl().getTimingRetard(), 5, 0.01);
	step(4500, 30, true);
	EXPECT_TRUE(getRl().isRollingLaunchArmed);
	EXPECT_FALSE(getRl().isRollingLaunchRampActive);
	EXPECT_FLOAT_EQ(getRl().capturedRpm, 4500);
	EXPECT_FLOAT_EQ(getRl().rollingLaunchTargetRpm, 5000);
	EXPECT_FLOAT_EQ(getRl().getTimingRetard(), 10);
	EXPECT_FLOAT_EQ(getRl().rollingLaunchRampElapsed, 0);
	step(2499, 30, true);
	EXPECT_FALSE(getRl().isRollingLaunchArmed);
	EXPECT_TRUE(getRl().isRollingLaunchRampActive);
}

TEST(RollingLaunch, InvalidAndNonfiniteSensorsReleaseHold) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupRl();
	for (auto type : {SensorType::Rpm, SensorType::VehicleSpeed}) {
		step(3500, 30, true);
		ASSERT_TRUE(getRl().isRollingLaunchArmed);
		Sensor::setInvalidMockValue(type);
		getRl().onSlowCallback();
		EXPECT_FALSE(getRl().isRollingLaunchArmed);
		EXPECT_FLOAT_EQ(getRl().getSparkSkipRatio(), 0);
		EXPECT_FLOAT_EQ(getRl().getFuelCoefficient(), 1);
		EXPECT_TRUE(getRl().isRollingLaunchRampActive);
		for (float invalid : {NAN, INFINITY, -1.0f}) {
			step(3500, 30, true);
			Sensor::setMockValue(type, invalid);
			getRl().onSlowCallback();
			EXPECT_FALSE(getRl().isRollingLaunchArmed);
		}
	}
	engineConfiguration->rollingLaunchMinArmRpm = 0;
	step(0, 30, true);
	EXPECT_FALSE(getRl().isRollingLaunchArmed);
}

TEST(RollingLaunch, DisableStopAndBurnClearHoldAndRamp) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupRl();
	step(3500, 30, true);
	step(4000, 30, true);
	engineConfiguration->rollingLaunchEnabled = false;
	getRl().onSlowCallback();
	EXPECT_FALSE(getRl().isRollingLaunchArmed);
	EXPECT_FALSE(getRl().isRollingLaunchRampActive);
	EXPECT_FLOAT_EQ(getRl().getSparkSkipRatio(), 0);
	EXPECT_FLOAT_EQ(getRl().getFuelCoefficient(), 1);
	EXPECT_FLOAT_EQ(getRl().getTimingRetard(), 0);
	EXPECT_FLOAT_EQ(getRl().rollingLaunchTargetRpm, 0);

	engineConfiguration->rollingLaunchEnabled = true;
	step(4500, 30, true);
	getRl().onEngineStop();
	EXPECT_FALSE(getRl().isRollingLaunchArmed);
	EXPECT_FLOAT_EQ(getRl().getTimingRetard(), 0);
	step(5000, 30, true);
	EXPECT_FLOAT_EQ(getRl().capturedRpm, 5000);
	step(5000, 30, false);
	ASSERT_TRUE(getRl().isRollingLaunchRampActive);
	getRl().onConfigurationChange(engineConfiguration);
	EXPECT_FALSE(getRl().isRollingLaunchRampActive);
	EXPECT_FLOAT_EQ(getRl().getTimingRetard(), 0);
	EXPECT_FLOAT_EQ(getRl().rollingLaunchRampElapsed, 0);
}

TEST(RollingLaunch, HardwareAndLuaTriggersAreOrCombined) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupRl();
	const auto pin = Gpio::F3;
	const bool previousState = efiReadPin(pin);
	engineConfiguration->rollingLaunchActivatePin = pin;
	engineConfiguration->rollingLaunchActivatePinMode = PI_DEFAULT;
	setMockState(pin, true);
	step(3500, 30, false);
	EXPECT_TRUE(getRl().rollingLaunchSwitchState);
	EXPECT_TRUE(getRl().isRollingLaunchArmed);
	setMockState(pin, false);
	step(3500, 30, true);
	EXPECT_FALSE(getRl().rollingLaunchSwitchState);
	EXPECT_TRUE(getRl().isRollingLaunchArmed);
	step(3500, 30, false);
	EXPECT_FALSE(getRl().isRollingLaunchArmed);
	engineConfiguration->rollingLaunchActivatePinMode = PI_INVERTED_PULLUP;
	step(3500, 30, false);
	EXPECT_TRUE(getRl().isRollingLaunchArmed);
	setMockState(pin, true);
	step(3500, 30, false);
	EXPECT_FALSE(getRl().isRollingLaunchArmed);
	setMockState(pin, previousState);
}

TEST(RollingLaunch, LuaHookControlsTrigger) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupRl();
	EXPECT_EQ(testLuaReturnsNumber(R"(
		function testFunc()
			setRollingLaunchTrigger(1)
			return 1
		end
	)"), 1);
	Sensor::setMockValue(SensorType::Rpm, 3500);
	Sensor::setMockValue(SensorType::VehicleSpeed, 30);
	getRl().onSlowCallback();
	ASSERT_TRUE(getRl().isRollingLaunchArmed);
	EXPECT_EQ(testLuaReturnsNumber(R"(
		function testFunc()
			setRollingLaunchTrigger(0)
			return 0
		end
	)"), 0);
	getRl().onSlowCallback();
	EXPECT_FALSE(getRl().isRollingLaunchArmed);
}

TEST(RollingLaunch, SharedLimiterPreservesOtherRequestsOnRelease) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupRl();
	engineConfiguration->useHardSkipInTraction = true;
	engine->engineState.luaHardSparkSkip = 0.2f;
	engine->engineState.luaSoftSparkSkip = 0.15f;
	engine->engineState.tractionControlSparkSkip = 10.0f; // percent
	step(3500, 30, true);
	step(3750, 30, true);
	engine->engineState.updateSparkSkip();
	EXPECT_NEAR(engine->hardSparkLimiter.getTargetSkipRatio(), 0.8f, 0.001f);
	EXPECT_NEAR(engine->softSparkLimiter.getTargetSkipRatio(), 0.15f, 0.001f);
	step(4000, 30, true);
	engine->engineState.updateSparkSkip();
	EXPECT_NEAR(engine->hardSparkLimiter.getTargetSkipRatio(), 1.3f, 0.001f);
	for (int i = 0; i < 20; i++) {
		EXPECT_TRUE(engine->hardSparkLimiter.shouldSkip());
	}
	step(4000, 30, false);
	engine->engineState.updateSparkSkip();
	EXPECT_NEAR(engine->hardSparkLimiter.getTargetSkipRatio(), 0.3f, 0.001f);
}

TEST(RollingLaunch, RunningFuelAndIgnitionCorrections) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupRl();
	engine->rpmCalculator.setRpmValue(3500);
	engine->fuelComputer.running.intakeTemperatureCoefficient = 1;
	engine->fuelComputer.running.coolantTemperatureCoefficient = 1;
	engine->fuelComputer.running.postCrankingFuelCorrection = 1;
	engine->engineState.baroCorrection = 1;
	setTable(config->ignitionTable, 25);
	const float baselineFuel = getRunningFuel(10);
	const float baselineTiming = engine->ignitionState.getWrappedAdvance(3500, 50);
	ASSERT_TRUE(std::isfinite(baselineFuel));
	ASSERT_GT(baselineFuel, 0);
	ASSERT_TRUE(std::isfinite(baselineTiming));
	step(3500, 30, true);
	EXPECT_NEAR(getRunningFuel(10), baselineFuel * 1.05f, 0.001f);
	EXPECT_NEAR(engine->ignitionState.getWrappedAdvance(3500, 50), baselineTiming - 10, 0.001f);
	step(3500, 30, false);
	eth.moveTimeForwardSec(0.5f);
	getRl().onSlowCallback();
	EXPECT_NEAR(getRunningFuel(10), baselineFuel, 0.001f);
	EXPECT_NEAR(engine->ignitionState.getWrappedAdvance(3500, 50), baselineTiming - 5, 0.001f);
	engineConfiguration->timingMode = TM_FIXED;
	engineConfiguration->fixedTiming = 13;
	EXPECT_FLOAT_EQ(engine->ignitionState.getWrappedAdvance(3500, 50), 13);
}

TEST(RollingLaunch, LuaResetReleasesHoldAndRampsOutTiming) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupRl();
	EXPECT_EQ(testLuaReturnsNumber(R"(
		function testFunc()
			setRollingLaunchTrigger(1)
			return 1
		end
	)"), 1);
	Sensor::setMockValue(SensorType::Rpm, 3500);
	Sensor::setMockValue(SensorType::VehicleSpeed, 30);
	getRl().onSlowCallback();
	Sensor::setMockValue(SensorType::Rpm, 4000);
	getRl().onSlowCallback();
	ASSERT_TRUE(getRl().isRollingLaunchArmed);
	ASSERT_FLOAT_EQ(getRl().getSparkSkipRatio(), 1);

	// A script reload clears its trigger, then the next slow callback releases
	// fuel/spark hold and starts the ordinary timing ramp-out.
	engine->resetLua();
	EXPECT_FALSE(getRl().luaRollingLaunchState);
	getRl().onSlowCallback();
	engine->engineState.updateSparkSkip();
	EXPECT_FALSE(getRl().isRollingLaunchActive);
	EXPECT_FALSE(getRl().isRollingLaunchArmed);
	EXPECT_TRUE(getRl().isRollingLaunchRampActive);
	EXPECT_FLOAT_EQ(getRl().getFuelCoefficient(), 1);
	EXPECT_FLOAT_EQ(getRl().getTimingRetard(), 10);
	EXPECT_FLOAT_EQ(engine->hardSparkLimiter.getTargetSkipRatio(), 0);

	eth.moveTimeForwardSec(0.5f);
	engine->resetLua(); // Another reset must not restart or cancel the release ramp.
	getRl().onSlowCallback();
	EXPECT_FLOAT_EQ(getRl().getTimingRetard(), 5);
	eth.moveTimeForwardSec(0.5f);
	getRl().onSlowCallback();
	EXPECT_FALSE(getRl().isRollingLaunchRampActive);
	EXPECT_FLOAT_EQ(getRl().getTimingRetard(), 0);
}

TEST(RollingLaunch, LuaResetPreservesHardwareRequest) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupRl();
	const auto pin = Gpio::F3;
	const bool previousState = efiReadPin(pin);
	engineConfiguration->rollingLaunchActivatePin = pin;
	engineConfiguration->rollingLaunchActivatePinMode = PI_DEFAULT;
	setMockState(pin, true);
	step(3500, 30, true);
	step(3750, 30, true);
	engine->resetLua();
	getRl().onSlowCallback();
	EXPECT_FALSE(getRl().luaRollingLaunchState);
	EXPECT_TRUE(getRl().isRollingLaunchArmed);
	EXPECT_FLOAT_EQ(getRl().capturedRpm, 3500);
	EXPECT_FLOAT_EQ(getRl().getSparkSkipRatio(), 0.5f);

	setMockState(pin, false);
	getRl().onSlowCallback();
	EXPECT_FALSE(getRl().isRollingLaunchArmed);
	EXPECT_TRUE(getRl().isRollingLaunchRampActive);
	setMockState(pin, previousState);
}
