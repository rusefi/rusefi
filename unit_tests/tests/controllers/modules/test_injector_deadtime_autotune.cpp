/*
 * @file test_injector_deadtime_autotune.cpp
 *
 */
#include "pch.h"
#include "fuel_math.h"
#include "rusefi_lua.h"
#include "injector_model.h"
#include "main_trigger_callback.h"
#include "lua_hooks.h"

// The firmware intentionally omits Lua's base library (assert, error, pcall).
static float runDeadtimeLua(const char* script) {
    const std::string source = std::string(R"(
        function assert(value)
            if not value then assertionFailed() end
        end
    )") + script;
    return testLuaReturnsNumber(source.c_str());
}

static void setupDeadtimeExperiment() {
	engineConfiguration->cylindersCount = 4;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
	engineConfiguration->crankingInjectionMode = IM_BATCH;
	engineConfiguration->cranking.rpm = 500;
	engineConfiguration->enableStagedInjection = false;
	engineConfiguration->injectorNonlinearMode = INJ_None;
	engineConfiguration->useInjectorFlowLinearizationTable = false;
	engine->triggerCentral.triggerState.setNeedsDisambiguation(false);
	engine->triggerCentral.triggerState.resetHasFullSync();
	engine->rpmCalculator.setRpmValue(2000);
	engine->engineState.injectionDuration = 3;
	engine->engineState.injectionOffset = 0;
}

static void applyDeadtimeControls() {
	auto& module = engine->module<InjectorDeadtimeAutotune>();
	module->beginFastCallback();
	module->endFastCallback();
}

TEST(InjectorDeadtimeAutotune, batchPreservesCycleMassAndAccountsForBothPulses) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth, IM_SEQUENTIAL);
	setupDeadtimeExperiment();
	EXPECT_CALL(*eth.mockAirmass, getAirmass(testing::_, testing::_))
		.WillRepeatedly(testing::Return(AirmassResult{0.1008f, 50.0f}));
	engineConfiguration->fuelClosedLoopCorrectionEnabled = false;
	engineConfiguration->ltft.enabled = false;
	engineConfiguration->ltft.correctionEnabled = false;
	engine->periodicFastCallback();
	const float sequentialMass = engine->engineState.injectionMass[0];
	ASSERT_GT(sequentialMass, 0);
	ASSERT_TRUE(engine->module<InjectorDeadtimeAutotune>()->setInjectionMode(IM_BATCH));
	engine->periodicFastCallback();
	EXPECT_EQ(IM_BATCH, getCurrentInjectionMode());
	EXPECT_NEAR(sequentialMass / 2, engine->engineState.injectionMass[0], 1e-6f);
	EXPECT_NEAR(100 * 2 * engine->engineState.injectionDuration / getEngineCycleDuration(2000),
		getInjectorDutyCycle(2000), EPS4D);

	engine->module<TripOdometer>()->reset();
	auto& event = engine->injectionEvents.elements[0];
	event.injectionStartAngle = 5;
	event.onTriggerTooth(getTimeNowNt(), 0, 10);
	EXPECT_NEAR(sequentialMass, engine->module<TripOdometer>()->getConsumedGramsRemainder(), 1e-6f);
	eth.clearQueue();
}

TEST(InjectorDeadtimeAutotune, luaValidationAndEligibility) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	EXPECT_EQ(1, runDeadtimeLua(R"(
		function testFunc()
			assert(not setInjectionModeOverride("batch"))
			assert(not setInjectorDeadtimeAdd(0.1))
			assert(not setLtftLearningDisabled(true))
			assert(setInjectionModeOverride(nil))
			assert(setInjectorDeadtimeAdd(0))
			assert(setLtftLearningDisabled(false))
			return 1
		end
	)"));
	for (const char* invalid : {
        "setInjectionModeOverride('simultaneous')", "setInjectorDeadtimeAdd(0/0)",
        "setInjectorDeadtimeAdd(math.huge)", "setInjectorDeadtimeAdd(2.01)",
        "setInjectorDeadtimeAdd(-2.01)", "setLtftLearningDisabled(1)",
        "getFuelTrim(0)", "getFuelTrim(3)", "getFuelTrim(4294967297)"}) {
        const std::string script = std::string("function testFunc() ") + invalid + " return 1 end";
        EXPECT_THROW(runDeadtimeLua(script.c_str()), std::logic_error) << invalid;
        engine->resetLua();
    }
	setupDeadtimeExperiment();
	EXPECT_EQ(1, runDeadtimeLua(R"(
		function testFunc()
			assert(setInjectionModeOverride("batch"))
			assert(setInjectorDeadtimeAdd(0.25))
			assert(setLtftLearningDisabled(true))
			local mode, pending = getInjectionMode()
			assert(mode == "sequential" and pending)
			return 1
		end
	)"));
	applyDeadtimeControls();
	EXPECT_EQ(1, runDeadtimeLua(R"(
		function testFunc()
			local mode, pending = getInjectionMode()
			assert(mode == "batch" and not pending)
			return 1
		end
	)"));
	EXPECT_EQ(IM_SEQUENTIAL, engineConfiguration->injectionMode);
}

TEST(InjectorDeadtimeAutotune, independentLeasesAndReset) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupDeadtimeExperiment();
	auto& module = engine->module<InjectorDeadtimeAutotune>();
	ASSERT_TRUE(module->setInjectionMode(IM_BATCH));
	ASSERT_TRUE(module->setDeadtimeAdd(0.25));
	ASSERT_TRUE(module->setLtftLearningDisabled(true));
	applyDeadtimeControls();
	advanceTimeUs(900000);
	ASSERT_TRUE(module->setInjectionMode(IM_BATCH));
	advanceTimeUs(100000);
	applyDeadtimeControls();
	EXPECT_EQ(IM_BATCH, getCurrentInjectionMode());
	EXPECT_FLOAT_EQ(0, module->getDeadtimeAdd());
	EXPECT_FALSE(module->isLtftLearningDisabled());
	advanceTimeUs(900000);
	applyDeadtimeControls();
	EXPECT_EQ(IM_SEQUENTIAL, getCurrentInjectionMode());
	EXPECT_FALSE(module->dtAutotuneActive);
	ASSERT_TRUE(module->setInjectionMode(IM_BATCH));
	ASSERT_TRUE(module->setDeadtimeAdd(0.25));
	ASSERT_TRUE(module->setLtftLearningDisabled(true));
	applyDeadtimeControls();
	engine->resetLua();
	applyDeadtimeControls();
	EXPECT_EQ(IM_SEQUENTIAL, getCurrentInjectionMode());
	EXPECT_FLOAT_EQ(0, module->getDeadtimeAdd());
	EXPECT_FALSE(module->isLtftLearningDisabled());
}

TEST(InjectorDeadtimeAutotune, errorsLatchUntilReload) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupDeadtimeExperiment();
	EXPECT_THROW(runDeadtimeLua(R"(
		function testFunc()
			assert(setInjectionModeOverride("batch"))
			assert(setInjectorDeadtimeAdd(0.2))
			assert(setLtftLearningDisabled(true))
			missingExperimentCallback()
		end
	)"), std::logic_error);
	auto& module = engine->module<InjectorDeadtimeAutotune>();
	applyDeadtimeControls();
	EXPECT_EQ(IM_SEQUENTIAL, getCurrentInjectionMode());
	EXPECT_FLOAT_EQ(0, module->getDeadtimeAdd());
	EXPECT_FALSE(module->isLtftLearningDisabled());
	EXPECT_FALSE(module->setInjectionMode(IM_BATCH));
	EXPECT_FALSE(module->setDeadtimeAdd(0.2));
	engine->resetLua();
	EXPECT_TRUE(module->setInjectionMode(IM_BATCH));
}

TEST(InjectorDeadtimeAutotune, errorBeforeFirstRequestLatchesVm) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupDeadtimeExperiment();
	LuaHandle ls(luaL_newstate());
	ASSERT_NE(nullptr, static_cast<lua_State*>(ls));
	configureRusefiLuaHooks(ls);
	ASSERT_EQ(LUA_OK, luaL_loadstring(ls, "missingCallback()"));
	EXPECT_NE(LUA_OK, luaProtectedCall(ls, 0, 0));
	lua_settop(ls, 0);
	ASSERT_EQ(LUA_OK, luaL_loadstring(ls, "return setInjectionModeOverride('batch')"));
	ASSERT_EQ(LUA_OK, luaProtectedCall(ls, 0, 1));
	EXPECT_FALSE(lua_toboolean(ls, -1));
	lua_settop(ls, 0);
	ASSERT_EQ(LUA_OK, luaL_loadstring(ls, "return setInjectionModeOverride(nil)"));
	ASSERT_EQ(LUA_OK, luaProtectedCall(ls, 0, 1));
	EXPECT_TRUE(lua_toboolean(ls, -1));
}

TEST(InjectorDeadtimeAutotune, primaryDeadtimeOnly) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupDeadtimeExperiment();
	setTable(engineConfiguration->injector.battLagCorrTable, 1.0f);
	setTable(engineConfiguration->injectorSecondary.battLagCorrTable, 1.5f);
	auto& primary = engine->module<InjectorModelPrimary>();
	auto& secondary = engine->module<InjectorModelSecondary>();
	auto& module = engine->module<InjectorDeadtimeAutotune>();
	ASSERT_TRUE(module->setDeadtimeAdd(0.25));
	EXPECT_FLOAT_EQ(1.25f, primary->getDeadtime());
	EXPECT_FLOAT_EQ(1.5f, secondary->getDeadtime());
	ASSERT_TRUE(module->setDeadtimeAdd(-2));
	EXPECT_FLOAT_EQ(0, primary->getDeadtime());
	ASSERT_TRUE(module->setDeadtimeAdd(0));
	EXPECT_FLOAT_EQ(1, primary->getDeadtime());
	EXPECT_FLOAT_EQ(1, engineConfiguration->injector.battLagCorrTable[0][0]);
}

TEST(InjectorDeadtimeAutotune, queuedPulseClosesBeforeModeChanges) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupDeadtimeExperiment();
	engine->injectionEvents.addFuelEvents();
	auto& event = engine->injectionEvents.elements[0];
	ASSERT_EQ(nullptr, event.outputs[1]);
	engine->engineState.injectionMass[0] = 0.01f;
	engine->module<InjectorModelPrimary>()->prepare();
	event.injectionStartAngle = 5;
	event.onTriggerTooth(getTimeNowNt(), 0, 10);
	ASSERT_EQ(2, engine->scheduler.size());
	auto& module = engine->module<InjectorDeadtimeAutotune>();
	ASSERT_TRUE(module->setInjectionMode(IM_BATCH));
	module->beginFastCallback();
	EXPECT_TRUE(module->isSchedulingBlocked());
	EXPECT_EQ(IM_SEQUENTIAL, getCurrentInjectionMode());
	EXPECT_TRUE(event.update());
	EXPECT_EQ(nullptr, event.outputs[1]);
	// Another tooth must not queue a new pulse during the drain.
	event.onTriggerTooth(getTimeNowNt(), 0, 10);
	EXPECT_EQ(2, engine->scheduler.size());
	eth.clearQueue();
	applyDeadtimeControls();
	EXPECT_EQ(IM_BATCH, getCurrentInjectionMode());
	EXPECT_NE(nullptr, event.outputs[1]);
	EXPECT_FLOAT_EQ(0.5f, getInjectionModeDurationMultiplier());
	// Exercise the reverse transition with two output pins already queued.
	engine->engineState.injectionMass[0] = 0.005f;
	event.injectionStartAngle = 5;
	event.onTriggerTooth(getTimeNowNt(), 0, 10);
	ASSERT_EQ(2, engine->scheduler.size());
	ASSERT_TRUE(module->setInjectionMode(-1));
	module->beginFastCallback();
	EXPECT_EQ(IM_BATCH, getCurrentInjectionMode());
	EXPECT_TRUE(event.update());
	EXPECT_NE(nullptr, event.outputs[1]);
	eth.clearQueue();
	applyDeadtimeControls();
	EXPECT_EQ(IM_SEQUENTIAL, getCurrentInjectionMode());
	EXPECT_EQ(nullptr, event.outputs[1]);
	EXPECT_FLOAT_EQ(1, getInjectionModeDurationMultiplier());
	for (auto& injector : enginePins.injectors) {
		EXPECT_EQ(0, injector.getOverlappingCounter());
	}
}

TEST(InjectorDeadtimeAutotune, ltftInhibitRetainsCorrection) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupDeadtimeExperiment();
	LtftState state;
	state.load();
	LongTermFuelTrim ltft;
	ltft.init(&state);
	engineConfiguration->ltft.enabled = true;
	engineConfiguration->ltft.correctionEnabled = true;
	engineConfiguration->ltft.maxAdd = 15;
	engineConfiguration->ltft.deadband = 0;
	engineConfiguration->ltft.timeConstant[ftRegionIdle] = 30;
	engine->module<ShortTermFuelTrim>()->stftCorrectionState = stftEnabled;
	ClosedLoopFuelResult input;
	input.region = ftRegionIdle;
	input.banks[0] = 1.1f;
	const auto rpm = config->veRpmBins[0];
	const auto load = config->veLoadBins[0];
	for (int i = 0; i < 100; i++) {
		ltft.learn(input, rpm, load);
	}
	const auto before = ltft.getTrims(rpm, load).banks[0];
	ASSERT_GT(before, 1);
	auto& module = engine->module<InjectorDeadtimeAutotune>();
	ASSERT_TRUE(module->setLtftLearningDisabled(true));
	for (int i = 0; i < 100; i++) {
		ltft.learn(input, rpm, load);
	}
	EXPECT_FALSE(ltft.ltftLearning);
	EXPECT_FLOAT_EQ(before, ltft.getTrims(rpm, load).banks[0]);
	ASSERT_TRUE(module->setLtftLearningDisabled(false));
	ltft.learn(input, rpm, load);
	EXPECT_GT(ltft.getTrims(rpm, load).banks[0], before);
	EXPECT_TRUE(engineConfiguration->ltft.enabled);
	EXPECT_TRUE(engineConfiguration->ltft.correctionEnabled);
}

TEST(InjectorDeadtimeAutotune, eligibilityLossAndStopRelease) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupDeadtimeExperiment();
	auto& module = engine->module<InjectorDeadtimeAutotune>();
	ASSERT_TRUE(module->setInjectionMode(IM_BATCH));
	applyDeadtimeControls();
	engine->triggerCentral.triggerState.setNeedsDisambiguation(true);
	engine->triggerCentral.triggerState.resetHasFullSync();
	applyDeadtimeControls();
	EXPECT_EQ(IM_SEQUENTIAL, getCurrentInjectionMode());
	EXPECT_FALSE(module->setInjectionMode(IM_SEQUENTIAL));
	setupDeadtimeExperiment();
	engineConfiguration->enableStagedInjection = true;
	EXPECT_FALSE(module->setInjectionMode(IM_BATCH));
	setupDeadtimeExperiment();
	engineConfiguration->cylindersCount = 1;
	EXPECT_FALSE(module->setInjectionMode(IM_BATCH));
	setupDeadtimeExperiment();
	ASSERT_TRUE(module->setInjectionMode(IM_BATCH));
	ASSERT_TRUE(module->setDeadtimeAdd(0.25));
	applyDeadtimeControls();
	engine->rpmCalculator.setStopSpinning();
	applyDeadtimeControls();
	EXPECT_EQ(IM_SEQUENTIAL, getCurrentInjectionMode());
	EXPECT_FLOAT_EQ(0, module->getDeadtimeAdd());
	EXPECT_FALSE(module->dtAutotuneActive);
}

TEST(InjectorDeadtimeAutotune, crankingAbortDrainsOldMappings) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupDeadtimeExperiment();
	auto& module = engine->module<InjectorDeadtimeAutotune>();
	ASSERT_TRUE(module->setInjectionMode(IM_BATCH));
	applyDeadtimeControls();
	auto& event = engine->injectionEvents.elements[0];
	ASSERT_NE(nullptr, event.outputs[1]);
	engine->engineState.injectionMass[0] = 0.01f;
	engine->module<InjectorModelPrimary>()->prepare();
	event.injectionStartAngle = 5;
	event.onTriggerTooth(getTimeNowNt(), 0, 10);
	ASSERT_EQ(2, engine->scheduler.size());
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engine->rpmCalculator.setStopSpinning();
	engine->rpmCalculator.setRpmValue(300);
	EXPECT_EQ(IM_SIMULTANEOUS, getCurrentInjectionMode());
	EXPECT_TRUE(module->isTransitioning());
	// RpmCalculator must not replace a queued batch pulse's second output.
	EXPECT_NE(nullptr, event.outputs[1]);
	module->beginFastCallback();
	eth.clearQueue();
	applyDeadtimeControls();
	EXPECT_EQ(nullptr, event.outputs[1]);
	EXPECT_FALSE(module->hasModeOverride());
	for (auto& injector : enginePins.injectors) {
		EXPECT_EQ(0, injector.getOverlappingCounter());
	}
}

TEST(InjectorDeadtimeAutotune, trimSnapshotAndSaturation) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupDeadtimeExperiment();
	auto& stft = engine->module<ShortTermFuelTrim>();
	stft->stftCorrectionState = stftEnabled;
	stft->stftLearningState[0] = stftEnabled;
	stft->stftLearningState[1] = stftDisabledDFCO;
	stft->stftCorrectionBinIdx = ftRegionCruise;
	engine->engineState.stftCorrection[0] = 1.05f;
	engine->engineState.stftCorrection[1] = 0.98f;
	Sensor::setMockValue(SensorType::Lambda1, 1.0f);
	engineConfiguration->stft.cellCfgs[ftRegionCruise].maxAdd = 10;
	engineConfiguration->stft.cellCfgs[ftRegionCruise].maxRemove = 10;
	stft->init(&engineConfiguration->stft);
	EXPECT_EQ(1, runDeadtimeLua(R"(
		function testFunc()
			local t = getFuelTrim(1)
			assert(math.abs(t.correction - 1.05) < 0.0001)
			assert(t.cell == 4 and t.enabled and not t.saturated)
			assert(t.correctionState == 0 and t.learningState == 0)
			local other = getFuelTrim(2)
			assert(math.abs(other.correction - 0.98) < 0.0001)
			assert(not other.enabled and other.learningState == 7)
			return 1
		end
	)"));
	// An integrator pinned at a zero correction limit must not look settled.
	engineConfiguration->stft.cellCfgs[ftRegionCruise].maxAdd = 0;
	stft->stftLearningState[0] = stftDisabledTpsAccel;
	EXPECT_EQ(1, runDeadtimeLua(R"(
		function testFunc()
			local t = getFuelTrim(1)
			assert(t.saturated and not t.enabled and t.learningState == 8)
			return 1
		end
	)"));
}

// Duty accounting follows the effective mode, including batch cranking.
TEST(InjectorDeadtimeAutotune, dutyUsesEffectiveMode) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->cylindersCount = 4;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
	engineConfiguration->crankingInjectionMode = IM_BATCH;
	engineConfiguration->cranking.rpm = 500;
	engine->rpmCalculator.setRpmValue(300);
	ASSERT_EQ(IM_BATCH, getCurrentInjectionMode());
	engine->engineState.injectionDuration = 10;
	engine->engineState.injectionDurationStage2 = 5;
	EXPECT_FLOAT_EQ(5.0f, getInjectorDutyCycle(300));
	EXPECT_FLOAT_EQ(2.5f, getInjectorDutyCycleStage2(300));
}

TEST(InjectorDeadtimeAutotune, moduleRegisteredWithDefaultState) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto module = engine->module<InjectorDeadtimeAutotune>();
	module->onSlowCallback();
	EXPECT_FALSE(module->dtAutotuneActive);
}
