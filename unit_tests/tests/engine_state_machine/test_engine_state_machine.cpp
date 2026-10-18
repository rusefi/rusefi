#include "pch.h"

#include "engine_state_machine.h"

// Default cranking RPM threshold used by TEST_ENGINE
static constexpr float TEST_CRANKING_RPM = 400.0f;
static constexpr float TEST_RUNNING_RPM  = 800.0f;

static void setupSmConfig() {
	engineConfiguration->useEngineStateMachine = true;
	engineConfiguration->smWotTpsThreshold = 90;
	engineConfiguration->smTransientHoldTimeMs = 0; // no hold-off by default in tests
	// Lower AE threshold so tests can trigger Transient with small TPS steps on Tps1
	engineConfiguration->tpsAccelEnrichmentThreshold = 5.0f;
}

// Standard accumulator config used across direction-detection tests.
// At threshold (500 RPM/s or 5 km/h/s), gain=20 %/s → 1%/callback @50ms.
// Strong signals (4× threshold) build ±80 % in 20 callbacks; flat signal decays to 0.
static void setupAccumulatorConfig() {
	engineConfiguration->smAccelRateThreshold           = 500;
	engineConfiguration->smDecelRateThreshold           = 500;
	engineConfiguration->smAccumulatorGain              = 20.0f;
	engineConfiguration->smAccumulatorDecayRate         = 10.0f;
	engineConfiguration->smAccumulatorUpshiftThreshold  = 40;
	engineConfiguration->smAccumulatorDownshiftThreshold = 40;
}

static EngineStateMachine& getSm() {
	return engine->module<EngineStateMachine>().unmock();
}

static EngineStateMachineState runAndGetState() {
	engine->periodicSlowCallback();
	return getSm().getCurrentState();
}

// ---- Disabled by default ----

TEST(EngineStateMachine, disabledByDefault) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// Feature is off by default
	ASSERT_FALSE(engineConfiguration->useEngineStateMachine);

	engine->rpmCalculator.setRpmValue(TEST_RUNNING_RPM);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 50.0f);

	engine->periodicSlowCallback();

	auto& sm = getSm();
	EXPECT_FALSE(sm.isEnabled());
	// With SM disabled, engineSmCurrentState must not have been computed
	EXPECT_FALSE(sm.engineSmEnabled);
}

// ---- State transitions ----

TEST(EngineStateMachine, offState) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();

	engine->rpmCalculator.setRpmValue(0);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0.0f);

	EXPECT_EQ(EngineStateMachineState::Off, runAndGetState());
}

TEST(EngineStateMachine, crankingState) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();

	// Below cranking RPM threshold → Cranking
	engine->rpmCalculator.setRpmValue(TEST_CRANKING_RPM - 50.0f);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0.0f);

	EXPECT_EQ(EngineStateMachineState::Cranking, runAndGetState());
}

TEST(EngineStateMachine, afterStartState) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();

	MockIdleController mockIdle;
	engine->module<IdleController>().set(&mockIdle);

	engine->rpmCalculator.setRpmValue(TEST_RUNNING_RPM);
	// Seed stable state; Tps1 not mocked so AE stays quiet
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 50.0f);
	ON_CALL(mockIdle, getCurrentPhase()).WillByDefault(Return(IIdleController::Phase::Running));
	runAndGetState();

	// Engine in crank-to-idle taper → SM reports Afterstart
	ON_CALL(mockIdle, getCurrentPhase()).WillByDefault(Return(IIdleController::Phase::CrankToIdleTaper));
	EXPECT_EQ(EngineStateMachineState::Afterstart, runAndGetState());

	// Taper complete → SM reports Cruising (throttle mid-range, above idle RPM)
	ON_CALL(mockIdle, getCurrentPhase()).WillByDefault(Return(IIdleController::Phase::Running));
	EXPECT_EQ(EngineStateMachineState::Cruising, runAndGetState());
}

TEST(EngineStateMachine, wotState) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();

	engine->rpmCalculator.setRpmValue(TEST_RUNNING_RPM);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0.0f);

	// WOT above threshold
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 95.0f);
	EXPECT_EQ(EngineStateMachineState::WOT, runAndGetState());
}

TEST(EngineStateMachine, wotPriorityOverTransient) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();

	engine->rpmCalculator.setRpmValue(TEST_RUNNING_RPM);

	// Seed AE buffer at low TPS so next jump triggers both WOT and Transient
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0.0f);
	Sensor::setMockValue(SensorType::Tps1, 0.0f);
	runAndGetState();

	// Big jump — WOT (DTI > 90%) and AE fires (Tps1 delta 95% > 5% threshold)
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 95.0f);
	Sensor::setMockValue(SensorType::Tps1, 95.0f);
	// WOT has higher priority than Transient
	EXPECT_EQ(EngineStateMachineState::WOT, runAndGetState());
}

TEST(EngineStateMachine, transientState) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();

	engine->rpmCalculator.setRpmValue(TEST_RUNNING_RPM);

	// Seed AE buffer at 30% Tps1 so the buffer has a stable reference
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 30.0f);
	Sensor::setMockValue(SensorType::Tps1, 30.0f);
	runAndGetState();

	// Step Tps1 to 40% — delta = 10% > 5% threshold → AE isAboveAccelThreshold = true
	Sensor::setMockValue(SensorType::Tps1, 40.0f);
	EXPECT_EQ(EngineStateMachineState::Transient, runAndGetState());
}

TEST(EngineStateMachine, transientHoldoff) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	engineConfiguration->smTransientHoldTimeMs = 100; // hold Transient for 100 ms (2 ticks) after AE drops

	// Use a 2-entry AE buffer so delta from the step scrolls out on the very next stable callback
	engine->module<TpsAccelEnrichment>()->setLength(2);

	engine->rpmCalculator.setRpmValue(TEST_RUNNING_RPM);

	// Seed: 2 callbacks to fill the 2-entry buffer at stable 30%
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 30.0f);
	Sensor::setMockValue(SensorType::Tps1, 30.0f);
	runAndGetState();
	advanceTimeUs(SLOW_CALLBACK_PERIOD_MS * 1000);
	runAndGetState();
	advanceTimeUs(SLOW_CALLBACK_PERIOD_MS * 1000);

	// Trigger: step Tps1 to 40% → delta=10% > 5% threshold → AE fires → Transient
	Sensor::setMockValue(SensorType::Tps1, 40.0f);
	EXPECT_EQ(EngineStateMachineState::Transient, runAndGetState());

	// AE drops on next callback (buffer is [40,40], delta=0); hold-off starts
	advanceTimeUs(SLOW_CALLBACK_PERIOD_MS * 1000);
	EXPECT_EQ(EngineStateMachineState::Transient, runAndGetState()); // 50 ms into the 100 ms hold
	advanceTimeUs(SLOW_CALLBACK_PERIOD_MS * 1000);
	EXPECT_EQ(EngineStateMachineState::Transient, runAndGetState()); // 100 ms into the 100 ms hold
	// Hold-off expired — state falls through to Cruising
	advanceTimeUs(SLOW_CALLBACK_PERIOD_MS * 1000);
	EXPECT_NE(EngineStateMachineState::Transient, runAndGetState());
}

TEST(EngineStateMachine, idleState) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();

	MockIdleController mockIdle;
	engine->module<IdleController>().set(&mockIdle);
	ON_CALL(mockIdle, getCurrentPhase()).WillByDefault(Return(IIdleController::Phase::Idling));

	engine->rpmCalculator.setRpmValue(TEST_RUNNING_RPM);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0.0f);

	EXPECT_EQ(EngineStateMachineState::Idle, runAndGetState());
}

TEST(EngineStateMachine, coastingState) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	// Keep RPM below the DFCO overrun threshold so the state stays Coasting, not Overrun
	engineConfiguration->coastingFuelCutRpmHigh = 4000;

	MockIdleController mockIdle;
	engine->module<IdleController>().set(&mockIdle);
	ON_CALL(mockIdle, getCurrentPhase()).WillByDefault(Return(IIdleController::Phase::Coasting));

	engine->rpmCalculator.setRpmValue(2000.0f);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0.0f);

	EXPECT_EQ(EngineStateMachineState::Coasting, runAndGetState());
}

TEST(EngineStateMachine, overrunWhenDfcoConditionsMet) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	// Configure overrun (DFCO) detection thresholds
	engineConfiguration->coastingFuelCutTps = 5;        // TPS < 5% → overrun allowed
	engineConfiguration->coastingFuelCutRpmHigh = 2000; // RPM > 2000 → activate
	engineConfiguration->coastingFuelCutRpmLow  = 1500; // RPM < 1500 → deactivate
	engineConfiguration->coastingFuelCutVssHigh = 0;    // VSS always qualifies
	engineConfiguration->coastingFuelCutVssLow  = 0;
	engineConfiguration->coastingFuelCutMap = 0;        // MAP check disabled (no MAP sensor)

	MockIdleController mockIdle;
	engine->module<IdleController>().set(&mockIdle);
	ON_CALL(mockIdle, getCurrentPhase()).WillByDefault(Return(IIdleController::Phase::Coasting));

	// Throttle closed, RPM high → isOverrun() true → Overrun state
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0.0f);
	engine->rpmCalculator.setRpmValue(3000.0f);
	EXPECT_EQ(EngineStateMachineState::Overrun, runAndGetState());

	// Throttle open → isOverrun() false → Coasting
	// Tps1 not mocked so AE stays quiet; seed a callback to stabilise state
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 50.0f);
	runAndGetState();
	EXPECT_EQ(EngineStateMachineState::Coasting, runAndGetState());

	// Throttle closed, RPM too low → not overrun → Coasting
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0.0f);
	engine->rpmCalculator.setRpmValue(1000.0f);
	runAndGetState();
	EXPECT_EQ(EngineStateMachineState::Coasting, runAndGetState());
}

TEST(EngineStateMachine, cruisingDefault) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();

	MockIdleController mockIdle;
	engine->module<IdleController>().set(&mockIdle);
	ON_CALL(mockIdle, getCurrentPhase()).WillByDefault(Return(IIdleController::Phase::Running));

	engine->rpmCalculator.setRpmValue(TEST_RUNNING_RPM);

	// Mid throttle, part-throttle, IdleController in Running → Cruising
	// Tps1 not mocked → AE sees 0, no transient fires
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 50.0f);
	runAndGetState();

	Sensor::setMockValue(SensorType::DriverThrottleIntent, 50.0f);
	EXPECT_EQ(EngineStateMachineState::Cruising, runAndGetState());
}

// ---- Helper: put the engine into a stable running state ----
static void enterRunning(float rpmVal = 2000.0f, float tpsVal = 50.0f) {
	engine->rpmCalculator.setRpmValue(rpmVal);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, tpsVal);
	engine->periodicSlowCallback();
}

// ---- Flat-shift torque reduction ----

TEST(EngineStateMachine, flatShiftSetsUpshiftAndTorqueReduction) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	enterRunning(3000.0f, 95.0f);

	// Simulate flat-shift torque reduction engaging
	engineConfiguration->torqueReductionEnabled = true;
	engine->shiftTorqueReductionController.isFlatShiftConditionSatisfied = true;

	engine->periodicSlowCallback();
	auto& sm = getSm();

	EXPECT_TRUE(sm.engineSmIsUpshifting);
	EXPECT_TRUE(sm.engineSmIsTorqueReduction);
	EXPECT_FALSE(sm.engineSmIsDownshifting);
	// Primary display state should be overridden to Upshifting (10)
	EXPECT_EQ(10u, sm.engineSmCurrentState);
}

TEST(EngineStateMachine, flatShiftClearsWhenInactive) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	enterRunning(3000.0f, 95.0f);

	engineConfiguration->torqueReductionEnabled = true;
	engine->shiftTorqueReductionController.isFlatShiftConditionSatisfied = true;
	engine->periodicSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsUpshifting);

	engine->shiftTorqueReductionController.isFlatShiftConditionSatisfied = false;
	engine->periodicSlowCallback();
	EXPECT_FALSE(getSm().engineSmIsUpshifting);
	EXPECT_FALSE(getSm().engineSmIsTorqueReduction);
}

// ---- Launch Control overlay ----

TEST(EngineStateMachine, launchControlOverlay) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	enterRunning(2000.0f, 50.0f);

	engine->launchController.isLaunchCondition = true;

	engine->periodicSlowCallback();
	auto& sm = getSm();
	EXPECT_TRUE(sm.engineSmIsLaunchControl);
	// Launch overrides primary display (9)
	EXPECT_EQ(9u, sm.engineSmCurrentState);
}

TEST(EngineStateMachine, launchControlDoesNotSetShiftBits) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	enterRunning(2000.0f, 50.0f);

	engine->launchController.isLaunchCondition = true;
	engine->periodicSlowCallback();
	auto& sm = getSm();

	EXPECT_FALSE(sm.engineSmIsUpshifting);
	EXPECT_FALSE(sm.engineSmIsDownshifting);
}

// ---- Overlay priority: Upshifting > Downshifting > LaunchControl ----

TEST(EngineStateMachine, overlayPriorityUpshiftBeatsLaunch) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	enterRunning(3000.0f, 95.0f);

	engineConfiguration->torqueReductionEnabled = true;
	engine->shiftTorqueReductionController.isFlatShiftConditionSatisfied = true;
	engine->launchController.isLaunchCondition = true;

	engine->periodicSlowCallback();
	// Upshifting (10) takes priority over LaunchControl (9)
	EXPECT_EQ(10u, getSm().engineSmCurrentState);
}

// ---- Clutch-based shift detection ----

static void setupShiftDetectionConfig() {
	setupSmConfig();
	// Both directions mapped to Clutch Down switch, RPM Rate accumulator mode
	engineConfiguration->smUpshiftClutchSwitch   = sm_clutch_switch_e::ClutchDown;
	engineConfiguration->smDownshiftClutchSwitch = sm_clutch_switch_e::ClutchDown;
	engineConfiguration->smShiftDetectionMode    = sm_shift_detection_mode_e::RpmRate;
	setupAccumulatorConfig();
}

// Run N callbacks at constant RPM/TPS. With constant RPM the accumulator stays at 0
// (rate=0 → dead-band decay from 0). Used by single-direction tests where no
// accumulator build-up is required before pressing the clutch.
static void seedHistory(int callbacks = 10, float rpm = 2000.0f, float tps = 50.0f) {
	engine->rpmCalculator.setRpmValue(rpm);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, tps);
	for (int i = 0; i < callbacks; i++) {
		engine->periodicSlowCallback();
		advanceTimeUs(SLOW_CALLBACK_PERIOD_MS * 1000);
	}
}

// Run N callbacks with linearly ramping RPM. Used to pre-build the shift-direction accumulator
// before pressing the clutch. Rising RPM → positive accumulator (upshift); falling → negative.
static void seedRampedRpm(float startRpm, float rpmPerCallback, int callbacks, float tps) {
	Sensor::setMockValue(SensorType::DriverThrottleIntent, tps);
	for (int i = 0; i < callbacks; i++) {
		engine->rpmCalculator.setRpmValue(startRpm + rpmPerCallback * i);
		engine->periodicSlowCallback();
		advanceTimeUs(SLOW_CALLBACK_PERIOD_MS * 1000);
	}
}

TEST(EngineStateMachine, upshiftDetectedAccumulator) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupShiftDetectionConfig();
	enterRunning();

	// Rising RPM builds a positive accumulator → upshift confirmed.
	// 100 RPM/callback × 20 Hz = 2000 RPM/s, ratio = 4× → 4 %/callback → 80 % after 20 callbacks.
	seedRampedRpm(2000.0f, 100.0f, 20, 50.0f);

	engine->engineState.lua.clutchDownState = true;
	engine->periodicSlowCallback();

	auto& sm = getSm();
	EXPECT_TRUE(sm.engineSmIsUpshifting);
	EXPECT_FALSE(sm.engineSmIsDownshifting);
	EXPECT_EQ(10u, sm.engineSmCurrentState);
}

TEST(EngineStateMachine, downshiftDetectedAccumulator) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupShiftDetectionConfig();
	// Anchor the RPM-rate calc at the ramp's actual starting RPM (4000) -- otherwise the first
	// quantized recompute would measure against enterRunning()'s default 2000 RPM anchor, a
	// same-tick discontinuity that doesn't occur with real (continuous) RPM.
	enterRunning(4000.0f, 0.0f);

	// Falling RPM builds a negative accumulator → downshift confirmed.
	// TPS=0% ensures tps < coastingFuelCutTps (default 2%) so SM reaches Overrun state.
	seedRampedRpm(4000.0f, -100.0f, 20, 0.0f);

	engine->engineState.lua.clutchDownState = true;
	engine->periodicSlowCallback();

	auto& sm = getSm();
	EXPECT_FALSE(sm.engineSmIsUpshifting);
	EXPECT_TRUE(sm.engineSmIsDownshifting);
	EXPECT_EQ(11u, sm.engineSmCurrentState);
}

TEST(EngineStateMachine, shiftClearedOnClutchRelease) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupShiftDetectionConfig();
	enterRunning();
	seedRampedRpm(2000.0f, 100.0f, 20, 50.0f); // build upshift accumulator

	engine->engineState.lua.clutchDownState = true;
	engine->periodicSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsUpshifting);

	// Release clutch — window should close
	engine->engineState.lua.clutchDownState = false;
	engine->periodicSlowCallback();
	EXPECT_FALSE(getSm().engineSmIsUpshifting);
	EXPECT_FALSE(getSm().engineSmIsDownshifting);
}

TEST(EngineStateMachine, shiftClearedOnTimeout) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupShiftDetectionConfig();
	enterRunning();
	seedRampedRpm(2000.0f, 100.0f, 20, 50.0f); // build upshift accumulator

	// Press clutch and hold for longer than SM_SHIFT_TIMEOUT_MS (3000ms)
	engine->engineState.lua.clutchDownState = true;
	engine->periodicSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsUpshifting);

	// Advance past the 3-second timeout
	advanceTimeUs(3100 * 1000);
	engine->periodicSlowCallback();

	EXPECT_FALSE(getSm().engineSmIsUpshifting);
	EXPECT_FALSE(getSm().engineSmIsDownshifting);
}

// ---- RPM Rate accumulator shift detection ----

static void setupRpmRateModeConfig() {
	setupSmConfig();
	engineConfiguration->smUpshiftClutchSwitch   = sm_clutch_switch_e::ClutchDown;
	engineConfiguration->smDownshiftClutchSwitch = sm_clutch_switch_e::ClutchDown;
	engineConfiguration->smShiftDetectionMode    = sm_shift_detection_mode_e::RpmRate;
	setupAccumulatorConfig(); // sets rate thresholds and accumulator params
}

// Rising RPM builds a positive accumulator → upshift confirmed.
// 2000 RPM/s at 500 RPM/s threshold → 4× → 4 %/callback → 80 % after 20 callbacks (threshold 40 %).
TEST(EngineStateMachine, upshiftDetectedRpmMode) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupRpmRateModeConfig();
	enterRunning();

	seedRampedRpm(2000.0f, 100.0f, 20, 50.0f);

	engine->engineState.lua.clutchDownState = true;
	engine->periodicSlowCallback();

	auto& sm = getSm();
	EXPECT_TRUE(sm.engineSmIsUpshifting);
	EXPECT_FALSE(sm.engineSmIsDownshifting);
}

// Falling RPM builds a negative accumulator → downshift confirmed.
TEST(EngineStateMachine, downshiftDetectedRpmMode) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupRpmRateModeConfig();
	// Anchor the RPM-rate calc at the ramp's actual starting RPM (4000) -- see
	// downshiftDetectedAccumulator for why.
	enterRunning(4000.0f, 0.0f);

	// TPS=0% ensures tps < coastingFuelCutTps (default 2%) so SM reaches Overrun state.
	seedRampedRpm(4000.0f, -100.0f, 20, 0.0f);

	engine->engineState.lua.clutchDownState = true;
	engine->periodicSlowCallback();

	auto& sm = getSm();
	EXPECT_FALSE(sm.engineSmIsUpshifting);
	EXPECT_TRUE(sm.engineSmIsDownshifting);
}

// Flat RPM → rate = 0 → dead-band decay → accumulator stays at 0 → neither direction confirmed.
TEST(EngineStateMachine, rpmModeFlatRpmNotConfirmed) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupRpmRateModeConfig();
	enterRunning();

	seedHistory(20, 3000.0f, 50.0f);

	engine->engineState.lua.clutchDownState = true;
	engine->periodicSlowCallback();

	auto& sm = getSm();
	EXPECT_FALSE(sm.engineSmIsUpshifting);
	EXPECT_FALSE(sm.engineSmIsDownshifting);
}

TEST(EngineStateMachine, noShiftBitsWithoutClutchSwitch) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	// No clutch switch configured (default = None)
	engineConfiguration->smUpshiftClutchSwitch   = sm_clutch_switch_e::None;
	engineConfiguration->smDownshiftClutchSwitch = sm_clutch_switch_e::None;
	enterRunning();

	engine->engineState.lua.clutchDownState = true;
	engine->periodicSlowCallback();

	EXPECT_FALSE(getSm().engineSmIsUpshifting);
	EXPECT_FALSE(getSm().engineSmIsDownshifting);
}

// ---- Time-based accumulator snap (smAccumulatorSnapTimeMs) ----
//
// A same-gear direction reversal (e.g. a long Overrun run immediately followed by acceleration)
// would otherwise need an equally long run the other way just to null out a large stale
// opposite-sign accumulator, risking a stale shift-direction call (e.g. a spurious downshift
// blip on clutch-in shortly after tipping back into the throttle). smAccumulatorSnapTimeMs
// forces the accumulator to 0 once we've dwelled continuously in the opposing state long enough,
// instead of waiting for it to decay/climb across zero on its own.

// Reversing RPM direction takes ~3 callbacks to show up in the SM's classified state, due to the
// deliberate one-callback lag documented on determineState() (it consumes the *previous* tick's
// rate) stacked with recordRpmSampleAndComputeRate()'s own recompute-then-hold behavior. The ramp
// lengths below give that entry lag plus a full smAccumulatorSnapTimeMs dwell (5 callbacks here)
// room to elapse before asserting.
TEST(EngineStateMachine, accumulatorSnapsToZeroAfterSustainedAcceleration) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupRpmRateModeConfig();
	engineConfiguration->smAccumulatorSnapTimeMs = 250; // 5 callbacks @ 50 ms

	// Build a strongly negative accumulator via a sustained decel (falling RPM, TPS closed) --
	// same setup as downshiftDetectedRpmMode, ~ -70 % after 20 callbacks.
	enterRunning(4000.0f, 0.0f);
	seedRampedRpm(4000.0f, -100.0f, 20, 0.0f);
	ASSERT_LT(getSm().engineSmShiftAccumulator, -50.0f);

	// Reverse into a sustained Accelerating ramp (2000 RPM/s, well above the 500 RPM/s
	// threshold, +4 %/callback). Without the snap this would take many more callbacks to climb
	// back across zero; verify it happens far sooner once the configured dwell time elapses.
	seedRampedRpm(2100.0f, 100.0f, 9, 50.0f);

	EXPECT_EQ(EngineStateMachineState::Accelerating, getSm().getCurrentState());
	EXPECT_GT(getSm().engineSmShiftAccumulator, 0.0f);
}

TEST(EngineStateMachine, accumulatorDoesNotSnapWhenDisabled) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupRpmRateModeConfig();
	engineConfiguration->smAccumulatorSnapTimeMs = 0; // disabled (legacy decay/cross-only behavior)

	enterRunning(4000.0f, 0.0f);
	seedRampedRpm(4000.0f, -100.0f, 20, 0.0f);
	ASSERT_LT(getSm().engineSmShiftAccumulator, -50.0f);

	// Same reversal as the test above, but with the snap disabled the accumulator only climbs by
	// normal integration and stays deep in negative territory instead of jumping to positive.
	seedRampedRpm(2100.0f, 100.0f, 9, 50.0f);

	EXPECT_EQ(EngineStateMachineState::Accelerating, getSm().getCurrentState());
	EXPECT_LT(getSm().engineSmShiftAccumulator, -40.0f);
}

TEST(EngineStateMachine, accumulatorSnapsToZeroAfterSustainedOverrun) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupRpmRateModeConfig();
	engineConfiguration->smAccumulatorSnapTimeMs = 250; // 5 callbacks @ 50 ms

	// Force Overrun to be reachable regardless of the real IdleController's own settle timing --
	// same technique as overrunWhenDfcoConditionsMet.
	engineConfiguration->coastingFuelCutTps     = 5;
	engineConfiguration->coastingFuelCutRpmHigh = 2000;
	engineConfiguration->coastingFuelCutRpmLow  = 1500;
	engineConfiguration->coastingFuelCutVssHigh = 0;
	engineConfiguration->coastingFuelCutVssLow  = 0;
	engineConfiguration->coastingFuelCutMap     = 0;
	MockIdleController mockIdle;
	engine->module<IdleController>().set(&mockIdle);
	ON_CALL(mockIdle, getCurrentPhase()).WillByDefault(Return(IIdleController::Phase::Coasting));

	// Build a strongly positive accumulator via a sustained Accelerating ramp -- same setup as
	// upshiftDetectedRpmMode, ~ +70 % after 20 callbacks.
	enterRunning(2000.0f, 50.0f);
	seedRampedRpm(2000.0f, 100.0f, 20, 50.0f);
	ASSERT_GT(getSm().engineSmShiftAccumulator, 50.0f);

	// Reverse into a sustained Overrun (falling RPM, TPS closed). Without the snap this would
	// take many more callbacks to fall back across zero; verify it happens far sooner once the
	// configured dwell time elapses. RPM stays well above coastingFuelCutRpmHigh throughout.
	seedRampedRpm(3900.0f, -100.0f, 9, 0.0f);

	EXPECT_EQ(EngineStateMachineState::Overrun, getSm().getCurrentState());
	EXPECT_LT(getSm().engineSmShiftAccumulator, 0.0f);
}

// ---- RPM/t configurable rate window ----

// For a linear RPM ramp, the rate is identical regardless of window length, because the rate
// calc always divides by the *actual* elapsed time between the two sampled points -- this is
// what lets smAccelRateThreshold/smDecelRateThreshold stay in RPM/s without re-tuning when the
// user changes the window.
TEST(EngineStateMachine, rpmRateWindowInvariantOnLinearRamp) {
	float shortWindowRate;
	float longWindowRate;
	{
		EngineTestHelper eth(engine_type_e::TEST_ENGINE);
		setupSmConfig();
		engineConfiguration->smUpshiftClutchSwitch = sm_clutch_switch_e::ClutchDown;
		engineConfiguration->smRpmRateWindowMs = 50; // one tick
		enterRunning();

		// 100 RPM/tick @ 50 ms/tick == 2000 RPM/s.
		seedRampedRpm(2000.0f, 100.0f, 10, 50.0f);
		shortWindowRate = getSm().getLastRpmRate();
	}
	{
		EngineTestHelper eth(engine_type_e::TEST_ENGINE);
		setupSmConfig();
		engineConfiguration->smUpshiftClutchSwitch = sm_clutch_switch_e::ClutchDown;
		engineConfiguration->smRpmRateWindowMs = 500; // ten ticks
		enterRunning();

		seedRampedRpm(2000.0f, 100.0f, 12, 50.0f); // extra ticks so a full window elapses
		longWindowRate = getSm().getLastRpmRate();
	}

	EXPECT_NEAR(2000.0f, shortWindowRate, 5.0f);
	EXPECT_NEAR(2000.0f, longWindowRate, 5.0f);
}

// The rate value now only recomputes once smRpmRateWindowMs has actually elapsed since the last
// recompute -- holding the previous value on every tick in between -- rather than recomputing on
// every 50 ms tick regardless of the configured window. This is what makes the number visibly
// update at the rate the user configures (in multiples of the 50 ms tick), instead of jittering
// every tick no matter what window is set.
TEST(EngineStateMachine, rpmRateHoldsUntilWindowElapsesThenUpdates) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	engineConfiguration->smUpshiftClutchSwitch = sm_clutch_switch_e::ClutchDown;
	engineConfiguration->smRpmRateWindowMs = 200; // four 50 ms ticks

	// First-ever sample: no reference yet, rate reads 0 and anchors here.
	engine->rpmCalculator.setRpmValue(2000.0f);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 50.0f);
	engine->periodicSlowCallback();
	EXPECT_EQ(0.0f, getSm().getLastRpmRate());
	advanceTimeUs(SLOW_CALLBACK_PERIOD_MS * 1000);

	// RPM jumps immediately, but the window (200 ms) hasn't elapsed since the anchor yet on any
	// of the next three ticks (50/100/150 ms in) -- the rate must hold at the old value.
	engine->rpmCalculator.setRpmValue(2500.0f);
	for (int i = 0; i < 3; i++) {
		engine->periodicSlowCallback();
		EXPECT_EQ(0.0f, getSm().getLastRpmRate());
		advanceTimeUs(SLOW_CALLBACK_PERIOD_MS * 1000);
	}

	// The fourth tick lands exactly on the 200 ms window boundary -- the rate now recomputes
	// over the full window, picking up the jump.
	engine->periodicSlowCallback();
	EXPECT_NEAR(2500.0f, getSm().getLastRpmRate(), 1.0f); // (2500-2000)/0.2s
}

// A shorter window recomputes (and thus can respond to an RPM change) more often than a longer
// one -- directly exercising that smRpmRateWindowMs controls the recompute cadence.
TEST(EngineStateMachine, shorterWindowRecomputesMoreOften) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	engineConfiguration->smUpshiftClutchSwitch = sm_clutch_switch_e::ClutchDown;
	engineConfiguration->smRpmRateWindowMs = 100; // two 50 ms ticks

	engine->rpmCalculator.setRpmValue(2000.0f);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 50.0f);
	engine->periodicSlowCallback(); // anchor
	advanceTimeUs(SLOW_CALLBACK_PERIOD_MS * 1000);

	engine->rpmCalculator.setRpmValue(2100.0f);
	engine->periodicSlowCallback(); // 50 ms since anchor -- window not elapsed yet, holds
	EXPECT_EQ(0.0f, getSm().getLastRpmRate());
	advanceTimeUs(SLOW_CALLBACK_PERIOD_MS * 1000);

	engine->periodicSlowCallback(); // 100 ms since anchor -- window elapsed, recomputes
	EXPECT_NEAR(1000.0f, getSm().getLastRpmRate(), 1.0f); // (2100-2000)/0.1s
}

// smRpmRateWindowMs == 0 (e.g. an old tune saved before this field existed) must floor to one
// slow-callback tick and behave identically to an explicit 50 ms window -- calibration
// compatibility for existing tunes.
TEST(EngineStateMachine, rpmRateWindowZeroMatchesOneTickWindow) {
	float zeroWindowRate;
	float explicitOneTickRate;
	{
		EngineTestHelper eth(engine_type_e::TEST_ENGINE);
		setupSmConfig();
		engineConfiguration->smUpshiftClutchSwitch = sm_clutch_switch_e::ClutchDown;
		engineConfiguration->smRpmRateWindowMs = 0;
		enterRunning(2000.0f, 50.0f);
		seedHistory(6, 2000.0f, 50.0f);
		engine->rpmCalculator.setRpmValue(2500.0f);
		engine->periodicSlowCallback();
		zeroWindowRate = getSm().getLastRpmRate();
	}
	{
		EngineTestHelper eth(engine_type_e::TEST_ENGINE);
		setupSmConfig();
		engineConfiguration->smUpshiftClutchSwitch = sm_clutch_switch_e::ClutchDown;
		engineConfiguration->smRpmRateWindowMs = 50;
		enterRunning(2000.0f, 50.0f);
		seedHistory(6, 2000.0f, 50.0f);
		engine->rpmCalculator.setRpmValue(2500.0f);
		engine->periodicSlowCallback();
		explicitOneTickRate = getSm().getLastRpmRate();
	}
	EXPECT_NEAR(explicitOneTickRate, zeroWindowRate, 0.01f);
}

// No reference sample yet on the very first slow callback -- rate must read 0, not garbage.
TEST(EngineStateMachine, rpmRateZeroBeforeEnoughHistory) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	engineConfiguration->smUpshiftClutchSwitch = sm_clutch_switch_e::ClutchDown;
	engineConfiguration->smRpmRateWindowMs = 500;

	engine->rpmCalculator.setRpmValue(2000.0f);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 50.0f);
	engine->periodicSlowCallback(); // first-ever sample, no reference yet

	EXPECT_EQ(0.0f, getSm().getLastRpmRate());
}

// The RPM-rate value now has a consumer beyond the accumulator: the Accelerating state itself,
// gated directly on smAccelRateThreshold against the windowed rate.
TEST(EngineStateMachine, acceleratingStateEnteredOnConfiguredRate) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	engineConfiguration->smUpshiftClutchSwitch = sm_clutch_switch_e::ClutchDown;
	engineConfiguration->smAccelRateThreshold = 500; // RPM/s
	engineConfiguration->smRpmRateWindowMs = 100;
	enterRunning(2000.0f, 50.0f);

	// 100 RPM/tick @ 50 ms/tick == 2000 RPM/s, comfortably above the 500 RPM/s threshold.
	seedRampedRpm(2000.0f, 100.0f, 5, 50.0f);
	engine->periodicSlowCallback();

	auto& sm = getSm();
	EXPECT_TRUE(sm.engineSmIsAccelerating);
	EXPECT_EQ(14u, sm.engineSmCurrentState);
}

// ---- Accelerating hold (smAccelHoldMs) ----
//
// determineState() consumes m_lastRpmRate one slow-callback tick after it's computed (see the
// comment on m_lastRpmRate / recordRpmSampleAndComputeRate), so with smRpmRateWindowMs floored to
// one tick, a single RPM step becomes visible to determineState() only on the *following* tick.
// The helpers below spell out each tick's real time and rate explicitly to keep that lag honest.

// Once the rate threshold is met, Accelerating must keep reporting for smAccelHoldMs after the
// rate drops back below threshold, then fall through once the hold expires.
TEST(EngineStateMachine, acceleratingHoldsAfterRateDrops) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	// m_lastRpmRate is only ever computed inside updateShiftAccumulator(), which
	// updateShiftDetection() skips entirely unless a clutch switch is configured -- so a switch
	// must be wired even though these tests don't exercise shift detection itself.
	engineConfiguration->smUpshiftClutchSwitch = sm_clutch_switch_e::ClutchDown;
	engineConfiguration->smAccelRateThreshold = 500; // RPM/s
	engineConfiguration->smRpmRateWindowMs = 50;     // recompute every tick (one slow-callback tick)
	engineConfiguration->smAccelHoldMs = 120;

	engine->rpmCalculator.setRpmValue(2000.0f);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 50.0f);
	engine->periodicSlowCallback(); // t=0: establishes the rate anchor, no rate yet
	advanceTimeUs(SLOW_CALLBACK_PERIOD_MS * 1000);

	// 500 RPM over one 50 ms tick == 10,000 RPM/s, comfortably above threshold. Not yet visible
	// to determineState() this same tick (one-tick lag).
	engine->rpmCalculator.setRpmValue(2500.0f);
	EXPECT_NE(EngineStateMachineState::Accelerating, runAndGetState()); // t=50 ms
	advanceTimeUs(SLOW_CALLBACK_PERIOD_MS * 1000);

	// RPM goes flat from here. The 10,000 RPM/s computed last tick is now visible -- Accelerating
	// triggers and the hold timer resets, even though the rate itself is about to read 0.
	EXPECT_EQ(EngineStateMachineState::Accelerating, runAndGetState()); // t=100 ms: trigger
	advanceTimeUs(SLOW_CALLBACK_PERIOD_MS * 1000);

	// Rate has now recomputed to 0 (flat RPM) -- below threshold -- but the hold keeps it latched.
	EXPECT_EQ(EngineStateMachineState::Accelerating, runAndGetState()); // t=150 ms: 50 ms into hold
	advanceTimeUs(SLOW_CALLBACK_PERIOD_MS * 1000);
	EXPECT_EQ(EngineStateMachineState::Accelerating, runAndGetState()); // t=200 ms: 100 ms into hold

	// Past smAccelHoldMs (120 ms since the t=100 ms trigger) -- hold expires, falls through.
	advanceTimeUs(SLOW_CALLBACK_PERIOD_MS * 1000);
	EXPECT_EQ(EngineStateMachineState::Cruising, runAndGetState()); // t=250 ms: 150 ms, expired
}

// A fresh threshold crossing during the hold window must reset it back to the full duration,
// not just let the original window run out underneath it.
TEST(EngineStateMachine, acceleratingHoldResetsOnReTrigger) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	// m_lastRpmRate is only ever computed inside updateShiftAccumulator(), which
	// updateShiftDetection() skips entirely unless a clutch switch is configured -- so a switch
	// must be wired even though these tests don't exercise shift detection itself.
	engineConfiguration->smUpshiftClutchSwitch = sm_clutch_switch_e::ClutchDown;
	engineConfiguration->smAccelRateThreshold = 500; // RPM/s
	engineConfiguration->smRpmRateWindowMs = 50;
	engineConfiguration->smAccelHoldMs = 120;

	engine->rpmCalculator.setRpmValue(2000.0f);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 50.0f);
	engine->periodicSlowCallback(); // t=0
	advanceTimeUs(SLOW_CALLBACK_PERIOD_MS * 1000);

	engine->rpmCalculator.setRpmValue(2500.0f);
	engine->periodicSlowCallback(); // t=50 ms: rate computed, not yet visible
	advanceTimeUs(SLOW_CALLBACK_PERIOD_MS * 1000);

	EXPECT_EQ(EngineStateMachineState::Accelerating, runAndGetState()); // t=100 ms: trigger #1
	advanceTimeUs(SLOW_CALLBACK_PERIOD_MS * 1000);

	// Still within trigger #1's hold. Jump RPM again -- this re-trigger's rate isn't visible yet
	// (one-tick lag), so this tick is still riding out the *original* hold.
	engine->rpmCalculator.setRpmValue(3000.0f);
	EXPECT_EQ(EngineStateMachineState::Accelerating, runAndGetState()); // t=150 ms
	advanceTimeUs(SLOW_CALLBACK_PERIOD_MS * 1000);

	// The re-trigger's rate (10,000 RPM/s) is now visible -- Accelerating re-triggers and the
	// hold timer resets to t=200 ms.
	EXPECT_EQ(EngineStateMachineState::Accelerating, runAndGetState()); // t=200 ms: re-trigger
	advanceTimeUs(SLOW_CALLBACK_PERIOD_MS * 1000);

	// t=250/300 ms: 50/100 ms since the re-trigger -- still held. Without the reset, the
	// *original* trigger's 120 ms hold (from t=100 ms) would already have expired by t=220 ms.
	EXPECT_EQ(EngineStateMachineState::Accelerating, runAndGetState()); // t=250 ms
	advanceTimeUs(SLOW_CALLBACK_PERIOD_MS * 1000);
	EXPECT_EQ(EngineStateMachineState::Accelerating, runAndGetState()); // t=300 ms
	advanceTimeUs(SLOW_CALLBACK_PERIOD_MS * 1000);

	// Past the reset hold (150 ms since the t=200 ms re-trigger) -- now falls through.
	EXPECT_EQ(EngineStateMachineState::Cruising, runAndGetState()); // t=350 ms
}

// A lift into Coasting/Overrun must break the Accelerating hold immediately -- a closed throttle
// physically contradicts a stale "still accelerating" claim, and Overrun/DFCO must never be
// masked by a latch that was still holding from an earlier tip-in.
TEST(EngineStateMachine, overrunBreaksAcceleratingHoldImmediately) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	// m_lastRpmRate is only ever computed inside updateShiftAccumulator(), which
	// updateShiftDetection() skips entirely unless a clutch switch is configured -- so a switch
	// must be wired even though these tests don't exercise shift detection itself.
	engineConfiguration->smUpshiftClutchSwitch = sm_clutch_switch_e::ClutchDown;
	engineConfiguration->smAccelRateThreshold = 500; // RPM/s
	engineConfiguration->smRpmRateWindowMs = 50;
	engineConfiguration->smAccelHoldMs = 200; // long hold relative to the steps below

	MockIdleController mockIdle;
	engine->module<IdleController>().set(&mockIdle);
	ON_CALL(mockIdle, getCurrentPhase()).WillByDefault(Return(IIdleController::Phase::Running));

	engine->rpmCalculator.setRpmValue(2000.0f);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 50.0f);
	engine->periodicSlowCallback(); // t=0
	advanceTimeUs(SLOW_CALLBACK_PERIOD_MS * 1000);

	engine->rpmCalculator.setRpmValue(2500.0f);
	engine->periodicSlowCallback(); // t=50 ms: rate computed, not yet visible
	advanceTimeUs(SLOW_CALLBACK_PERIOD_MS * 1000);

	EXPECT_EQ(EngineStateMachineState::Accelerating, runAndGetState()); // t=100 ms: trigger
	advanceTimeUs(SLOW_CALLBACK_PERIOD_MS * 1000);

	// Lift off: throttle closes and the idle controller reports Coasting. Only 50 ms have
	// elapsed since the trigger, deep inside the 200 ms hold -- but Coasting must override it
	// immediately rather than waiting out the hold.
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0.0f);
	ON_CALL(mockIdle, getCurrentPhase()).WillByDefault(Return(IIdleController::Phase::Coasting));
	engineConfiguration->coastingFuelCutRpmHigh = 9000; // keep this plain Coasting, not Overrun
	EXPECT_EQ(EngineStateMachineState::Coasting, runAndGetState()); // t=150 ms
}

// ---- Clutch-Up-only shift detection (single switch, no Clutch-Down wired) ----

static void setupClutchUpOnlyConfig() {
	setupSmConfig();
	engineConfiguration->smUpshiftClutchSwitch         = sm_clutch_switch_e::ClutchUp;
	engineConfiguration->smDownshiftClutchSwitch       = sm_clutch_switch_e::None;
	engineConfiguration->smShiftDetectionMode          = sm_shift_detection_mode_e::RpmRate;
	engineConfiguration->smSecondClutchSwitchAvailable = false;
	setupAccumulatorConfig();
}

// The pedal leaving rest (falling edge) is the moment the drivetrain is still locked, which is
// when the window must open. This is the edge that was previously missed entirely.
TEST(EngineStateMachine, clutchUpOnlyOpensOnFallingEdge) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupClutchUpOnlyConfig();
	enterRunning();

	// Pedal at rest while history is seeded.
	engine->engineState.lua.clutchUpState = true;
	seedHistory(10, 3000.0f, 80.0f);

	// Pedal leaves rest — window should open and confirm immediately (no second direction to
	// disambiguate, zero delay).
	engine->engineState.lua.clutchUpState = false;
	engine->periodicSlowCallback();

	EXPECT_TRUE(getSm().engineSmIsUpshifting);
	EXPECT_FALSE(getSm().engineSmIsDownshifting);
}

// Regression guard for the original bug: the pedal returning to rest (rising edge) must NOT by
// itself open a new shift window.
TEST(EngineStateMachine, clutchUpOnlyDoesNotOpenOnRisingEdge) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupClutchUpOnlyConfig();
	enterRunning();

	// Pedal already pressed (not at rest) while history is seeded.
	engine->engineState.lua.clutchUpState = false;
	seedHistory(10, 3000.0f, 80.0f);

	// Pedal returns to rest — must not open the window.
	engine->engineState.lua.clutchUpState = true;
	engine->periodicSlowCallback();

	EXPECT_FALSE(getSm().engineSmIsUpshifting);
	EXPECT_FALSE(getSm().engineSmIsDownshifting);
}

TEST(EngineStateMachine, clutchUpOnlyClosesOnRisingEdge) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupClutchUpOnlyConfig();
	enterRunning();

	engine->engineState.lua.clutchUpState = true;
	seedHistory(10, 3000.0f, 80.0f);

	engine->engineState.lua.clutchUpState = false; // falling edge: window opens
	engine->periodicSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsUpshifting);

	engine->engineState.lua.clutchUpState = true; // rising edge: clutch re-engaged, window closes
	engine->periodicSlowCallback();
	EXPECT_FALSE(getSm().engineSmIsUpshifting);
	EXPECT_FALSE(getSm().engineSmIsDownshifting);
}

// ---- smSecondClutchSwitchAvailable: using the complementary switch to refine timing ----

// Shared-switch configuration (both directions keyed to Clutch Down, accumulator-disambiguated)
// but a Clutch-Up switch is also physically wired. The window should open on the earlier Clutch-Up
// falling edge instead of waiting for the (later) Clutch-Down rising edge.
TEST(EngineStateMachine, secondSwitchOpensEarlyOnComplementaryFallingEdge) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupShiftDetectionConfig(); // up & down both ClutchDown
	engineConfiguration->smSecondClutchSwitchAvailable = true;
	enterRunning();

	engine->engineState.lua.clutchUpState   = true;
	engine->engineState.lua.clutchDownState = false;
	// Build upshift accumulator so direction can be confirmed when the window opens.
	seedRampedRpm(2000.0f, 100.0f, 20, 50.0f);

	// Pedal leaves rest (Clutch-Up falling) well before reaching full press (Clutch-Down still
	// false) — the window must open here, not wait for the Down switch.
	engine->engineState.lua.clutchUpState = false;
	engine->periodicSlowCallback();

	EXPECT_TRUE(getSm().engineSmIsUpshifting);
	EXPECT_FALSE(getSm().engineSmIsDownshifting);
}

static void setupSecondSwitchUpPrimaryConfig() {
	setupSmConfig();
	engineConfiguration->smUpshiftClutchSwitch   = sm_clutch_switch_e::ClutchUp;
	engineConfiguration->smDownshiftClutchSwitch = sm_clutch_switch_e::None;
	engineConfiguration->smShiftDetectionMode    = sm_shift_detection_mode_e::RpmRate;
	setupAccumulatorConfig();
}

// With a Clutch-Down switch also wired, its falling edge (release starting) should close the
// window earlier than waiting for Clutch-Up to fully return to rest.
TEST(EngineStateMachine, secondSwitchClosesEarlyOnComplementaryFallingEdge) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSecondSwitchUpPrimaryConfig();
	engineConfiguration->smSecondClutchSwitchAvailable = true;
	enterRunning();

	engine->engineState.lua.clutchUpState   = true;
	engine->engineState.lua.clutchDownState = false;
	seedHistory(10, 3000.0f, 80.0f);

	engine->engineState.lua.clutchUpState = false; // press begins: window opens
	engine->periodicSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsUpshifting);

	engine->engineState.lua.clutchDownState = true; // full press reached
	engine->periodicSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsUpshifting) << "still mid-shift, no close edge yet";

	// Release starts (Clutch-Down falling) while Clutch-Up has NOT yet returned to rest.
	engine->engineState.lua.clutchDownState = false;
	engine->periodicSlowCallback();
	EXPECT_FALSE(getSm().engineSmIsUpshifting);
	EXPECT_FALSE(getSm().engineSmIsDownshifting);
}

// Without the second-switch flag, the same Clutch-Down falling edge must NOT close the window —
// only Clutch-Up returning to rest (or the timeout) may close it.
TEST(EngineStateMachine, withoutSecondSwitchFlagComplementaryEdgeDoesNotCloseEarly) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSecondSwitchUpPrimaryConfig();
	engineConfiguration->smSecondClutchSwitchAvailable = false;
	enterRunning();

	engine->engineState.lua.clutchUpState   = true;
	engine->engineState.lua.clutchDownState = false;
	seedHistory(10, 3000.0f, 80.0f);

	engine->engineState.lua.clutchUpState = false; // press begins: window opens
	engine->periodicSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsUpshifting);

	engine->engineState.lua.clutchDownState = true; // full press reached
	engine->periodicSlowCallback();

	// Release starts (Clutch-Down falling) — must NOT close without the second-switch flag.
	engine->engineState.lua.clutchDownState = false;
	engine->periodicSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsUpshifting) << "must stay open until Clutch-Up itself returns to rest";

	// Clutch-Up finally returns to rest — now it closes.
	engine->engineState.lua.clutchUpState = true;
	engine->periodicSlowCallback();
	EXPECT_FALSE(getSm().engineSmIsUpshifting);
	EXPECT_FALSE(getSm().engineSmIsDownshifting);
}

TEST(EngineStateMachine, flatShiftOverridesClutchDetection) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupShiftDetectionConfig();
	enterRunning();
	seedHistory(10, 3000.0f, 2.0f); // accumulator stays 0 (flat RPM) — no direction confirmed

	// Flat shift active — must win regardless of accumulator state
	engineConfiguration->torqueReductionEnabled = true;
	engine->shiftTorqueReductionController.isFlatShiftConditionSatisfied = true;
	engine->engineState.lua.clutchDownState = true;
	engine->periodicSlowCallback();

	EXPECT_TRUE(getSm().engineSmIsUpshifting);
	EXPECT_FALSE(getSm().engineSmIsDownshifting);
	EXPECT_TRUE(getSm().engineSmIsTorqueReduction);
}

// ---- VSS Rate mode: minimum-speed gate (smShiftMinVss) ----

static void setupVssRateModeConfig() {
	setupSmConfig();
	engineConfiguration->smUpshiftClutchSwitch    = sm_clutch_switch_e::ClutchDown;
	engineConfiguration->smDownshiftClutchSwitch  = sm_clutch_switch_e::ClutchDown;
	engineConfiguration->smShiftDetectionMode     = sm_shift_detection_mode_e::VssRate;
	engineConfiguration->smAccelRateThreshold     = 5;  // km/h per second
	engineConfiguration->smDecelRateThreshold     = 5;
	engineConfiguration->smAccumulatorGain              = 20.0f;
	engineConfiguration->smAccumulatorDecayRate         = 10.0f;
	engineConfiguration->smAccumulatorUpshiftThreshold  = 40;
	engineConfiguration->smAccumulatorDownshiftThreshold = 40;
}

// Seed the history buffer with linearly ramping VSS, mirroring seedRampedRpm.
static void seedRampedVss(float startVss, float vssPerCallback, int callbacks, float tps) {
	Sensor::setMockValue(SensorType::DriverThrottleIntent, tps);
	for (int i = 0; i < callbacks; i++) {
		Sensor::setMockValue(SensorType::VehicleSpeed, startVss + vssPerCallback * i);
		engine->periodicSlowCallback();
		advanceTimeUs(SLOW_CALLBACK_PERIOD_MS * 1000);
	}
}

// Below smShiftMinVss the accumulator decays instead of accumulating, so the confirmation
// threshold is never reached even when the VSS rate would otherwise qualify.
TEST(EngineStateMachine, vssRateModeBlockedBelowMinVss) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupVssRateModeConfig();
	engineConfiguration->smShiftMinVss = 15; // require at least 15 km/h
	enterRunning();

	// Speed stays below 15 km/h throughout (0.5 to 10.0 km/h) — accumulator decays the whole time.
	seedRampedVss(0.5f, 0.5f, 20, 80.0f);

	engine->engineState.lua.clutchDownState = true;
	engine->periodicSlowCallback();

	auto& sm = getSm();
	EXPECT_FALSE(sm.engineSmIsUpshifting);
	EXPECT_FALSE(sm.engineSmIsDownshifting);
}

// With smShiftMinVss = 0 the accumulator builds on every callback.
// 10 km/h/s at 5 km/h/s threshold → 2× → ~2 %/callback → ~50 % after 25 callbacks (threshold 40 %).
TEST(EngineStateMachine, vssRateModeAllowedAboveMinVss) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupVssRateModeConfig();
	engineConfiguration->smShiftMinVss = 0;
	enterRunning();

	// Start at 0.5 km/h so the first callback already has a non-zero rate vs. the initial 0.
	// Use 25 callbacks to land at ~50% — well above the 40% threshold despite float rounding.
	seedRampedVss(0.5f, 0.5f, 25, 80.0f);

	engine->engineState.lua.clutchDownState = true;
	engine->periodicSlowCallback();

	auto& sm = getSm();
	EXPECT_TRUE(sm.engineSmIsUpshifting);
	EXPECT_FALSE(sm.engineSmIsDownshifting);
}

// ---- Shift latch (smShiftLatchTimeMs): masks rapid on/off flicker ----

TEST(EngineStateMachine, latchDefaultIsZeroAndClearsInstantly) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupClutchUpOnlyConfig(); // smShiftLatchTimeMs left at its 0 default
	enterRunning();

	engine->engineState.lua.clutchUpState = true;
	seedHistory(10, 3000.0f, 80.0f);

	engine->engineState.lua.clutchUpState = false; // window opens and confirms immediately
	engine->periodicSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsUpshifting);

	engine->engineState.lua.clutchUpState = true; // window closes
	engine->periodicSlowCallback();
	EXPECT_FALSE(getSm().engineSmIsUpshifting)
		<< "0ms latch must behave exactly like the old instantaneous signal";
}

// Clutch-Up returning to rest must force the overlay off immediately, overriding any still-active
// latch. The blipper/RPM-hold modules trust this flag with no clutch check of their own, so the
// latch may never outlive the pedal actually being back up.
TEST(EngineStateMachine, clutchUpForceOffOverridesActiveLatch) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupClutchUpOnlyConfig();
	engineConfiguration->smShiftLatchTimeMs = 200;
	enterRunning();

	engine->engineState.lua.clutchUpState = true;
	seedHistory(10, 3000.0f, 80.0f);

	engine->engineState.lua.clutchUpState = false; // window opens, confirms, latch armed for 200ms
	engine->periodicSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsUpshifting);

	engine->engineState.lua.clutchUpState = true; // pedal back at rest: force-off wins despite the latch
	engine->periodicSlowCallback();
	EXPECT_FALSE(getSm().engineSmIsUpshifting) << "Clutch-Up true must force the overlay off even with an active latch";
	EXPECT_FALSE(getSm().engineSmIsDownshifting);

	// Stays cleared rather than reappearing on a later tick.
	advanceTimeUs(50 * 1000);
	engine->periodicSlowCallback();
	EXPECT_FALSE(getSm().engineSmIsUpshifting);
}

// A later Clutch-Down full-press must not resurrect an overlay already killed by Clutch-Up — the
// force-off clears the latch outright, so relatch-on-clutch-down has nothing left to extend.
TEST(EngineStateMachine, clutchUpForceOffSurvivesSubsequentRelatchAttempt) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupClutchUpOnlyConfig();
	engineConfiguration->smSecondClutchSwitchAvailable = true;
	engineConfiguration->smShiftLatchTimeMs = 100;
	engineConfiguration->smRelatchOnClutchDown = true;
	enterRunning();

	engine->engineState.lua.clutchUpState   = true;
	engine->engineState.lua.clutchDownState = false;
	seedHistory(10, 3000.0f, 80.0f);

	engine->engineState.lua.clutchUpState = false; // window opens, confirms, latch until +100ms
	engine->periodicSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsUpshifting);

	engine->engineState.lua.clutchUpState = true; // pedal back at rest: force-off wins, latch destroyed
	engine->periodicSlowCallback();
	EXPECT_FALSE(getSm().engineSmIsUpshifting);

	advanceTimeUs(10 * 1000);
	engine->engineState.lua.clutchDownState = true; // fresh full-press
	engine->periodicSlowCallback();
	EXPECT_FALSE(getSm().engineSmIsUpshifting)
		<< "relatch-on-clutch-down must not be able to resurrect an overlay already killed by Clutch-Up";
}

// ---- smRelatchOnClutchDown: a fresh Clutch-Down press refreshes an active latch ----

static void setupRelatchConfig() {
	setupClutchUpOnlyConfig();
	engineConfiguration->smSecondClutchSwitchAvailable = true;
	engineConfiguration->smShiftLatchTimeMs = 100;
}

TEST(EngineStateMachine, repeatedConfirmationDoesNotExtendLatchWithoutRelatch) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupRelatchConfig();
	engineConfiguration->smRelatchOnClutchDown = false;
	enterRunning();

	engine->engineState.lua.clutchUpState   = true;
	engine->engineState.lua.clutchDownState = false;
	seedHistory(10, 3000.0f, 80.0f);

	engine->engineState.lua.clutchUpState = false; // window opens, confirms, latch until +100ms
	engine->periodicSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsUpshifting);

	engine->engineState.lua.clutchDownState = true; // full press, well short of Clutch-Up returning to rest
	engine->periodicSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsUpshifting);

	// Release starts (Clutch-Down falling): earlyClose shuts the window early. Clutch-Up is still
	// false (pedal not yet at rest), so the latch — not the Clutch-Up force-off — covers us here.
	engine->engineState.lua.clutchDownState = false;
	engine->periodicSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsUpshifting);

	// 90ms later, a fresh Clutch-Down full-press reconfirms (opens a new window, same direction)
	// but must NOT push the original 100ms deadline back out.
	advanceTimeUs(90 * 1000);
	engine->engineState.lua.clutchDownState = true;
	engine->periodicSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsUpshifting);

	// 15ms later (105ms after the original confirmation) the original deadline has passed.
	advanceTimeUs(15 * 1000);
	engine->engineState.lua.clutchDownState = false;
	engine->periodicSlowCallback();
	EXPECT_FALSE(getSm().engineSmIsUpshifting)
		<< "without smRelatchOnClutchDown, the latch must not be extended by a later confirmation";
}

TEST(EngineStateMachine, relatchOnClutchDownExtendsLatch) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupRelatchConfig();
	engineConfiguration->smRelatchOnClutchDown = true;
	enterRunning();

	engine->engineState.lua.clutchUpState   = true;
	engine->engineState.lua.clutchDownState = false;
	seedHistory(10, 3000.0f, 80.0f);

	engine->engineState.lua.clutchUpState = false; // window opens, confirms, latch until +100ms
	engine->periodicSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsUpshifting);

	engine->engineState.lua.clutchDownState = true; // full press, well short of Clutch-Up returning to rest
	engine->periodicSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsUpshifting);

	// Release starts (Clutch-Down falling): earlyClose shuts the window early. Clutch-Up is still
	// false (pedal not yet at rest), so the latch — not the Clutch-Up force-off — covers us here.
	engine->engineState.lua.clutchDownState = false;
	engine->periodicSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsUpshifting);

	// 90ms later, a fresh Clutch-Down full-press relatches: deadline becomes (now + 100ms).
	advanceTimeUs(90 * 1000);
	engine->engineState.lua.clutchDownState = true;
	engine->periodicSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsUpshifting);

	// 15ms later (105ms after the original confirmation) — past the ORIGINAL deadline, but the
	// relatch pushed it out to 190ms, so the overlay must still be asserted. Clutch-Up stays false
	// throughout, so the force-off override never engages here.
	advanceTimeUs(15 * 1000);
	engine->engineState.lua.clutchDownState = false;
	engine->periodicSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsUpshifting) << "smRelatchOnClutchDown should have pushed the deadline out";
}

// ---- Shifting states block the overrun (DFCO) fuel cut ----

TEST(EngineStateMachine, liveDataFieldsPopulated) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();

	engine->rpmCalculator.setRpmValue(0);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0.0f);
	engine->periodicSlowCallback();

	auto& sm = getSm();
	EXPECT_TRUE(sm.engineSmEnabled);
	EXPECT_EQ(0, sm.engineSmCurrentState);  // Off = 0
	EXPECT_TRUE(sm.engineSmIsOff);
	EXPECT_FALSE(sm.engineSmIsCranking);
	EXPECT_FALSE(sm.engineSmIsWot);
}

// ---- Temperature overlay ----

TEST(EngineStateMachine, tempOverlay_cold) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	engineConfiguration->smColdTempThreshold = 60;
	engineConfiguration->smHotTempThreshold  = 100;
	enterRunning();

	Sensor::setMockValue(SensorType::Clt, 30.0f);
	engine->periodicSlowCallback();

	auto& sm = getSm();
	EXPECT_TRUE(sm.engineSmIsCold);
	EXPECT_FALSE(sm.engineSmIsOperating);
	EXPECT_FALSE(sm.engineSmIsHot);
}

TEST(EngineStateMachine, tempOverlay_operating) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	engineConfiguration->smColdTempThreshold = 60;
	engineConfiguration->smHotTempThreshold  = 100;
	enterRunning();

	Sensor::setMockValue(SensorType::Clt, 80.0f);
	engine->periodicSlowCallback();

	auto& sm = getSm();
	EXPECT_FALSE(sm.engineSmIsCold);
	EXPECT_TRUE(sm.engineSmIsOperating);
	EXPECT_FALSE(sm.engineSmIsHot);
}

TEST(EngineStateMachine, tempOverlay_hot) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	engineConfiguration->smColdTempThreshold = 60;
	engineConfiguration->smHotTempThreshold  = 100;
	enterRunning();

	Sensor::setMockValue(SensorType::Clt, 110.0f);
	engine->periodicSlowCallback();

	auto& sm = getSm();
	EXPECT_FALSE(sm.engineSmIsCold);
	EXPECT_FALSE(sm.engineSmIsOperating);
	EXPECT_TRUE(sm.engineSmIsHot);
}

TEST(EngineStateMachine, tempOverlay_noSensor_treatsAsOperating) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	enterRunning();

	// No CLT sensor set — should not false-trigger Cold or Hot
	Sensor::resetMockValue(SensorType::Clt);
	engine->periodicSlowCallback();

	auto& sm = getSm();
	EXPECT_FALSE(sm.engineSmIsCold);
	EXPECT_TRUE(sm.engineSmIsOperating);
	EXPECT_FALSE(sm.engineSmIsHot);
}

TEST(EngineStateMachine, tempOverlay_exactlyAtColdThreshold_isNotCold) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	engineConfiguration->smColdTempThreshold = 60;
	engineConfiguration->smHotTempThreshold  = 100;
	enterRunning();

	// Exactly at the threshold is Operating, not Cold (strict less-than)
	Sensor::setMockValue(SensorType::Clt, 60.0f);
	engine->periodicSlowCallback();

	auto& sm = getSm();
	EXPECT_FALSE(sm.engineSmIsCold);
	EXPECT_TRUE(sm.engineSmIsOperating);
}

// These expectations exercise the fixes to the defects reproduced in 96175a70798.
TEST(EngineStateMachineRegression, DisableClearsWot) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	enterRunning(3000, 95);
	ASSERT_TRUE(getSm().engineSmIsWot);
	engineConfiguration->useEngineStateMachine = false;
	getSm().onSlowCallback();
	EXPECT_FALSE(getSm().engineSmEnabled);
	EXPECT_FALSE(getSm().engineSmIsWot);
	EXPECT_EQ(getSm().getCurrentState(), EngineStateMachineState::Off);
}

TEST(EngineStateMachineRegression, EngineStopClearsWot) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	enterRunning(3000, 95);
	ASSERT_TRUE(getSm().engineSmIsWot);
	engine->rpmCalculator.setRpmValue(0);
	getSm().onEngineStop();
	EXPECT_FALSE(getSm().engineSmIsWot);
	EXPECT_EQ(getSm().getCurrentState(), EngineStateMachineState::Off);
}

TEST(EngineStateMachineRegression, BurnClearsWot) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	enterRunning(3000, 95);
	ASSERT_TRUE(getSm().engineSmIsWot);
	getSm().onConfigurationChange(nullptr);
	EXPECT_FALSE(getSm().engineSmIsWot);
	EXPECT_EQ(getSm().getCurrentState(), EngineStateMachineState::Off);
}

TEST(EngineStateMachineRegression, RateDetectionWithoutClutchSwitch) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	setupAccumulatorConfig();
	MockIdleController idle;
	engine->module<IdleController>().set(&idle);
	ON_CALL(idle, getCurrentPhase()).WillByDefault(Return(IIdleController::Phase::Running));
	enterRunning();
	seedRampedRpm(2000, 100, 20, 50);
	EXPECT_NEAR(getSm().engineSmRpmRate, 2000, 0.1f);
	EXPECT_TRUE(getSm().engineSmIsAccelerating);
	EXPECT_FLOAT_EQ(getSm().engineSmShiftAccumulator, 0);

	engineConfiguration->torqueReductionEnabled = true;
	engine->shiftTorqueReductionController.isFlatShiftConditionSatisfied = true;
	seedRampedRpm(4000, 200, 10, 50);
	EXPECT_TRUE(getSm().engineSmIsUpshifting);
	EXPECT_NEAR(getSm().engineSmRpmRate, 4000, 0.1f);
	EXPECT_FLOAT_EQ(getSm().engineSmShiftAccumulator, 0);
}

TEST(EngineStateMachineRegression, LaunchDoesNotAccumulateShiftEvidence) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupShiftDetectionConfig();
	MockIdleController idle;
	engine->module<IdleController>().set(&idle);
	ON_CALL(idle, getCurrentPhase()).WillByDefault(Return(IIdleController::Phase::Running));
	enterRunning();
	engine->launchController.isLaunchCondition = true;
	seedRampedRpm(2000, 100, 20, 50);
	ASSERT_TRUE(getSm().engineSmIsLaunchControl);
	EXPECT_FLOAT_EQ(getSm().engineSmShiftAccumulator, 0);
}

TEST(EngineStateMachineRegression, FirstVssSampleIsNotAcceleration) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupVssRateModeConfig();
	advanceTimeUs(10 * 1000000);
	engine->rpmCalculator.setRpmValue(3000);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 50);
	Sensor::setMockValue(SensorType::VehicleSpeed, 40);
	getSm().onSlowCallback();
	EXPECT_FLOAT_EQ(getSm().engineSmShiftAccumulator, 0);
}

TEST(EngineStateMachineRegression, InvalidVssDoesNotCreateDownshiftEvidence) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupVssRateModeConfig();
	enterRunning();
	Sensor::setMockValue(SensorType::VehicleSpeed, 60);
	seedHistory();
	Sensor::setInvalidMockValue(SensorType::VehicleSpeed);
	advanceTimeUs(50000);
	getSm().onSlowCallback();
	EXPECT_FLOAT_EQ(getSm().engineSmShiftAccumulator, 0);
	EXPECT_FALSE(getSm().engineSmIsDownshifting);

	// A recovered sensor also needs a fresh anchor, not a comparison with zero.
	Sensor::setMockValue(SensorType::VehicleSpeed, 80);
	advanceTimeUs(50000);
	getSm().onSlowCallback();
	EXPECT_FLOAT_EQ(getSm().engineSmShiftAccumulator, 0);
	Sensor::setMockValue(SensorType::VehicleSpeed, 81);
	advanceTimeUs(50000);
	getSm().onSlowCallback();
	EXPECT_GT(getSm().engineSmShiftAccumulator, 0);
}

// Reset must establish clutch history without synthesizing a press edge.
TEST(EngineStateMachineRegression, HeldClutchDoesNotCreateFreshShiftAfterBurn) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	engineConfiguration->smUpshiftClutchSwitch = sm_clutch_switch_e::ClutchDown;
	enterRunning();
	engine->engineState.lua.clutchDownState = true;
	engine->periodicSlowCallback();
	ASSERT_TRUE(getSm().engineSmIsUpshifting);
	advanceTimeUs(3100 * 1000);
	engine->periodicSlowCallback();
	ASSERT_FALSE(getSm().engineSmIsUpshifting); // Original shift expired with the pedal still down.

	getSm().onConfigurationChange(nullptr);
	ASSERT_FALSE(getSm().engineSmIsUpshifting);
	// No new physical edge: the pedal has stayed down throughout the settings burn.
	engine->periodicSlowCallback();
	EXPECT_FALSE(getSm().engineSmIsUpshifting); // No new edge, so the expired shift stays inactive.
}

TEST(EngineStateMachineRegression, InvalidVssDoesNotQualifyDecelerating) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	setupAccumulatorConfig();
	engineConfiguration->smUpshiftClutchSwitch = sm_clutch_switch_e::ClutchDown;
	MockIdleController idle;
	engine->module<IdleController>().set(&idle);
	ON_CALL(idle, getCurrentPhase()).WillByDefault(Return(IIdleController::Phase::Running));
	Sensor::setMockValue(SensorType::DetectedGear, 0);
	Sensor::setInvalidMockValue(SensorType::VehicleSpeed);
	ASSERT_TRUE(Sensor::hasSensor(SensorType::VehicleSpeed));
	ASSERT_FALSE(Sensor::get(SensorType::VehicleSpeed).Valid);
	enterRunning(4000);
	seedRampedRpm(4000, -100, 10, 50);
	EXPECT_LT(getSm().engineSmRpmRate, -500);
	EXPECT_FALSE(getSm().engineSmIsDecelerating);

	// Valid speed restores classification without changing the neutral-gear input.
	Sensor::setMockValue(SensorType::VehicleSpeed, 60);
	getSm().onSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsDecelerating);
}

TEST(EngineStateMachineRegression, OverrunHonorsUnassignedClutch) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	engineConfiguration->clutchDownPin = Gpio::A1;
	engineConfiguration->coastingFuelCutTps = 5;
	engineConfiguration->coastingFuelCutRpmHigh = 2000;
	MockIdleController idle;
	engine->module<IdleController>().set(&idle);
	ON_CALL(idle, getCurrentPhase()).WillByDefault(Return(IIdleController::Phase::Coasting));
	engine->rpmCalculator.setRpmValue(3000);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0);
	engine->engineState.clutchDownState = true;
	ASSERT_EQ(engineConfiguration->smUpshiftClutchSwitch, sm_clutch_switch_e::None);
	ASSERT_EQ(engineConfiguration->smDownshiftClutchSwitch, sm_clutch_switch_e::None);
	getSm().onSlowCallback();
	EXPECT_FALSE(getSm().isTransmissionEngaged());
	EXPECT_TRUE(getSm().engineSmIsCoasting);
	engineConfiguration->stateBasedStft = true;
	auto& stft = engine->module<ShortTermFuelTrim>().unmock();
	EXPECT_EQ(stft.computeStftBin(3000, 50, engineConfiguration->stft), ftRegionIdle);

	// Assigning the switch to shift recognition must not change the primary state.
	engineConfiguration->smUpshiftClutchSwitch = sm_clutch_switch_e::ClutchDown;
	getSm().onSlowCallback();
	EXPECT_FALSE(getSm().isTransmissionEngaged());
	EXPECT_TRUE(getSm().engineSmIsCoasting);
	EXPECT_EQ(stft.computeStftBin(3000, 50, engineConfiguration->stft), ftRegionIdle);
}

static void checkShiftHistoryReset(void (*reset)()) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	setupAccumulatorConfig();
	engineConfiguration->smUpshiftClutchSwitch = sm_clutch_switch_e::ClutchDown;
	engineConfiguration->smShiftLatchTimeMs = 2000;
	engineConfiguration->smAccelHoldMs = 5000;
	MockIdleController idle;
	engine->module<IdleController>().set(&idle);
	ON_CALL(idle, getCurrentPhase()).WillByDefault(Return(IIdleController::Phase::Running));
	enterRunning();
	seedRampedRpm(2000, 100, 20, 50);
	ASSERT_GT(getSm().engineSmShiftAccumulator, 40);
	ASSERT_GT(getSm().engineSmRpmRate, 500);
	engine->engineState.lua.clutchDownState = true;
	engine->periodicSlowCallback();
	ASSERT_TRUE(getSm().engineSmIsUpshifting);

	reset();
	EXPECT_FALSE(getSm().engineSmIsUpshifting);
	EXPECT_FALSE(getSm().engineSmIsDownshifting);
	EXPECT_FLOAT_EQ(getSm().engineSmShiftAccumulator, 0);
	EXPECT_FLOAT_EQ(getSm().engineSmRpmRate, 0);
	EXPECT_EQ(getSm().getCurrentState(), EngineStateMachineState::Off);
	EXPECT_TRUE(getSm().engineSmIsOff);

	engineConfiguration->useEngineStateMachine = true;
	engine->rpmCalculator.setRpmValue(3000);
	engine->periodicSlowCallback();
	advanceTimeUs(100000);
	engine->periodicSlowCallback();
	// A new RPM anchor and expired hold timers allow steady cruising immediately.
	EXPECT_EQ(getSm().getCurrentState(), EngineStateMachineState::Cruising);
	EXPECT_FLOAT_EQ(getSm().engineSmShiftAccumulator, 0);
	EXPECT_FLOAT_EQ(getSm().engineSmRpmRate, 0);
	EXPECT_FALSE(getSm().engineSmIsUpshifting); // Pedal remained pressed through reset.

	engine->engineState.lua.clutchDownState = false;
	engine->periodicSlowCallback();
	advanceTimeUs(50000);
	engine->engineState.lua.clutchDownState = true;
	engine->periodicSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsUpshifting); // A real new press still works.
}

TEST(EngineStateMachineRegression, DisableClearsShiftHistory) {
	checkShiftHistoryReset([] {
		engineConfiguration->useEngineStateMachine = false;
		getSm().onSlowCallback();
	});
}

TEST(EngineStateMachineRegression, EngineStopClearsShiftHistory) {
	checkShiftHistoryReset([] {
		engine->rpmCalculator.setRpmValue(0);
		getSm().onEngineStop();
	});
}

TEST(EngineStateMachineRegression, BurnClearsShiftHistory) {
	checkShiftHistoryReset([] { getSm().onConfigurationChange(nullptr); });
}

TEST(EngineStateMachineRegression, VssLossCancelsShiftUntilNewClutchPress) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupVssRateModeConfig();
	engineConfiguration->smDownshiftClutchSwitch = sm_clutch_switch_e::None;
	engineConfiguration->smShiftLatchTimeMs = 2000;
	Sensor::setMockValue(SensorType::VehicleSpeed, 40);
	enterRunning();
	engine->engineState.lua.clutchDownState = true;
	engine->periodicSlowCallback();
	ASSERT_TRUE(getSm().engineSmIsUpshifting);

	Sensor::setInvalidMockValue(SensorType::VehicleSpeed);
	advanceTimeUs(50000);
	engine->periodicSlowCallback();
	EXPECT_FALSE(getSm().engineSmIsUpshifting);
	EXPECT_FALSE(getSm().engineSmIsDownshifting);
	EXPECT_FLOAT_EQ(getSm().engineSmShiftAccumulator, 0);

	Sensor::setMockValue(SensorType::VehicleSpeed, 80);
	advanceTimeUs(50000);
	engine->periodicSlowCallback();
	EXPECT_FALSE(getSm().engineSmIsUpshifting); // Lost window/latch must not revive.
	EXPECT_FLOAT_EQ(getSm().engineSmShiftAccumulator, 0);
	engine->engineState.lua.clutchDownState = false;
	engine->periodicSlowCallback();
	engine->engineState.lua.clutchDownState = true;
	engine->periodicSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsUpshifting);
}

TEST(EngineStateMachineRegression, OverrunHonorsEitherConfiguredClutchInput) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	engineConfiguration->clutchUpPin = Gpio::A2;
	engineConfiguration->coastingFuelCutTps = 5;
	engineConfiguration->coastingFuelCutRpmHigh = 2000;
	MockIdleController idle;
	engine->module<IdleController>().set(&idle);
	ON_CALL(idle, getCurrentPhase()).WillByDefault(Return(IIdleController::Phase::Coasting));
	engine->rpmCalculator.setRpmValue(3000);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0);
	engine->engineState.clutchUpState = 0;
	getSm().onSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsCoasting);
	engine->engineState.clutchUpState = 1;
	getSm().onSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsOverrun);

	// With both switches installed, either indication of disengagement blocks Overrun.
	engineConfiguration->clutchDownPin = Gpio::A1;
	engine->engineState.clutchDownState = true;
	getSm().onSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsCoasting);
	engine->engineState.clutchDownState = false;
	engine->engineState.clutchUpState = 0;
	getSm().onSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsCoasting);
	engine->engineState.clutchUpState = 1;
	getSm().onSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsOverrun);
}

TEST(EngineStateMachineRegression, StoppedAndCrankingClearOverlays) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSmConfig();
	enterRunning();
	engineConfiguration->torqueReductionEnabled = true;
	engine->launchController.isLaunchCondition = true;
	engine->shiftTorqueReductionController.isFlatShiftConditionSatisfied = true;
	getSm().onSlowCallback();
	ASSERT_TRUE(getSm().engineSmIsLaunchControl);
	ASSERT_TRUE(getSm().engineSmIsUpshifting);

	engine->rpmCalculator.setRpmValue(0);
	getSm().onSlowCallback();
	EXPECT_EQ(getSm().getCurrentState(), EngineStateMachineState::Off);
	EXPECT_EQ(getSm().engineSmCurrentState, static_cast<uint8_t>(EngineStateMachineState::Off));
	EXPECT_FALSE(getSm().engineSmIsLaunchControl);
	EXPECT_FALSE(getSm().engineSmIsUpshifting);
	EXPECT_FALSE(getSm().engineSmIsTorqueReduction);
	engine->rpmCalculator.setRpmValue(TEST_CRANKING_RPM - 50);
	engine->launchController.isLaunchCondition = true;
	engine->shiftTorqueReductionController.isFlatShiftConditionSatisfied = true;
	getSm().onSlowCallback();
	EXPECT_TRUE(getSm().engineSmIsCranking);
	EXPECT_EQ(getSm().engineSmCurrentState, static_cast<uint8_t>(EngineStateMachineState::Cranking));
	EXPECT_FALSE(getSm().engineSmIsLaunchControl);
	EXPECT_FALSE(getSm().engineSmIsUpshifting);
	EXPECT_FALSE(getSm().engineSmIsTorqueReduction);
}
