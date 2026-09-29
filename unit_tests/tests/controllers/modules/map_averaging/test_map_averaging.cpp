/*
 * @file test_map_averaging.cpp
 *
 * @date: may 12, 2025
 * @author FDSoftware
 */

#include "pch.h"
#include "map_averaging.h"
#include "harley.h"
#include "linear_func.h"

#include <array>

namespace {
	auto const startAveragingAction{ action_s::make<startAveraging>((mapSampler*){}) };
}

TEST(MapAveragingAdc, BeforeConverterInitialization) {
	EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
	MapAverager sensor(SensorType::MapFast, MS2NT(200));

	// A healthy N52 key-on voltage arrives before initMap attaches the converter.
	sensor.onAdcSample(0.74f, true);
	EXPECT_EQ(0, eth.getWarningCounter());
	EXPECT_FALSE(engine->engineState.warnings.isWarningNow(ObdCode::CUSTOM_INSTANT_MAP_DECODING));
	EXPECT_FALSE(engine->outputChannels.isMapValid);
	EXPECT_FLOAT_EQ(0, engine->outputChannels.instantMAPValue);

	LinearFunc converter;
	converter.configure(4.5f, 0, 0.7f, 100, 5, 410);
	sensor.setFunction(converter);
	sensor.onAdcSample(0.74f, true);
	EXPECT_EQ(0, eth.getWarningCounter());
	EXPECT_TRUE(engine->outputChannels.isMapValid);
	EXPECT_NEAR(98.947f, engine->outputChannels.instantMAPValue, 0.04f);
}

TEST(MapAveragingAdc, BeforeChannelInitialization) {
	EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
	MapAverager sensor(SensorType::MapFast, MS2NT(200));
	LinearFunc converter;
	converter.configure(4.5f, 0, 0.7f, 100, 5, 410);
	sensor.setFunction(converter);

	// Neither a plausible voltage from an unselected channel nor an invalid
	// voltage should be treated as a MAP reading until its token is ready.
	sensor.onAdcSample(0.74f, false);
	EXPECT_FALSE(engine->outputChannels.isMapValid);
	EXPECT_FLOAT_EQ(0, engine->outputChannels.instantMAPValue);
	sensor.onAdcSample(4.5f, false);
	EXPECT_EQ(0, eth.getWarningCounter());
	EXPECT_FALSE(engine->engineState.warnings.isWarningNow(ObdCode::CUSTOM_INSTANT_MAP_DECODING));

	sensor.onAdcSample(0.74f, true);
	EXPECT_EQ(0, eth.getWarningCounter());
	EXPECT_TRUE(engine->outputChannels.isMapValid);
	EXPECT_NEAR(98.947f, engine->outputChannels.instantMAPValue, 0.04f);
}

TEST(MapAveragingAdc, InvalidVoltageAfterInitialization) {
	EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
	MapAverager sensor(SensorType::MapFast, MS2NT(200));
	LinearFunc converter;
	converter.configure(4.5f, 0, 0.7f, 100, 5, 410);
	sensor.setFunction(converter);

	// Readiness must not suppress real faults with a stopped engine or with
	// crank-synchronous averaging disabled.
	engineConfiguration->isMapAveragingEnabled = false;
	sensor.onAdcSample(4.5f, true);
	EXPECT_EQ(1, eth.getWarningCounter());
	EXPECT_TRUE(engine->engineState.warnings.isWarningNow(ObdCode::CUSTOM_INSTANT_MAP_DECODING));
	EXPECT_FALSE(engine->outputChannels.isMapValid);
	EXPECT_FLOAT_EQ(0, engine->outputChannels.instantMAPValue);
}

TEST(EngineModules, MapAveragingModule_onEnginePhase) {
    EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
    engineConfiguration->isMapAveragingEnabled = true;
    engineConfiguration->measureMapOnlyInOneCylinder = true;
	engine->engineState.mapAveragingDuration = 100;
	engine->rpmCalculator.setRpmValue(200);

	engine->module<MapAveragingModule>()->onEnginePhase(200, getTimeNowNt(), 50.f, 180.f);

	// check for startMapAveraging schedule at 50° on the future (since onEnginePhase was called 50° late [start angle is 100°])
    bool averageDone = eth.assertEventExistsAtEnginePhase("startMapAveraging callback", startAveragingAction, static_cast<angle_t>(50));
    EXPECT_TRUE(averageDone);

    // move forward, we expect that the startAveraging is called and we are currently running the averaging code
	eth.moveTimeForwardMs(50);
	eth.executeActions();
	EXPECT_TRUE(engine->outputChannels.isMapAveraging);

    // move forward, here we expect the averaging is done and endAveraging was called
   	eth.moveTimeForwardMs(50);
	eth.executeActions();
    EXPECT_FALSE(engine->outputChannels.isMapAveraging);
}

TEST(EngineModules, MapAveragingModule_onFastCallback) {
    EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
    engineConfiguration->isMapAveragingEnabled = true;

    MapAveragingModule mapModule;

    // trigger events at crank speed
    for (size_t i = 0; i < 9; i++) {
        eth.fireTriggerEventsWithDuration(200);
        eth.executeActions();
    }
    ASSERT_EQ(150,  Sensor::getOrZero(SensorType::Rpm));

    mapModule.onFastCallback();

    // we expect here than the map start angles correspond to the phase of the cylinder + 100 of samplingAngle
   	EXPECT_EQ(engine->engineState.mapAveragingStart[0], 100);
    EXPECT_EQ(engine->engineState.mapAveragingStart[1], 640);
    EXPECT_EQ(engine->engineState.mapAveragingStart[2], 280);
    EXPECT_EQ(engine->engineState.mapAveragingStart[3], 460);

    EXPECT_EQ(engine->engineState.mapAveragingDuration, 50);
}

TEST(EngineModules, MapAveragingModule_onFastCallbackCustomSampleWindow) {
    EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
    engineConfiguration->isMapAveragingEnabled = true;
	setArrayValues(engineConfiguration->map.samplingAngle, 75);

    MapAveragingModule mapModule;

    // trigger events at crank speed
    for (size_t i = 0; i < 9; i++) {
        eth.fireTriggerEventsWithDuration(200);
        eth.executeActions();
    }
    ASSERT_EQ(150,  Sensor::getOrZero(SensorType::Rpm));

    mapModule.onFastCallback();

    // we expect here than the map start angles correspond to the phase of the cylinder + 75 of samplingAngle
    EXPECT_EQ(engine->engineState.mapAveragingStart[0], 75);  // 0 + 75
    EXPECT_EQ(engine->engineState.mapAveragingStart[1], 615); // 540 + 75
    EXPECT_EQ(engine->engineState.mapAveragingStart[2], 255); // 180 + 75
    EXPECT_EQ(engine->engineState.mapAveragingStart[3], 435); // 360 + 75

    EXPECT_EQ(engine->engineState.mapAveragingDuration, 50);
}

TEST(EngineModules, MapAveragingModule_onFastCallbackOddFire) {
	EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
	setHarley();
	// Apply the new cylinder count, firing order, and trigger before calculating
	// offsets. Otherwise cylinder 2 retains the four-cylinder setup's 540 degrees.
	// Update geometry without notifying unrelated global hardware controllers.
	engine->updateTriggerConfiguration();
	setArrayValues(engineConfiguration->map.samplingAngle, 75);
	engine->rpmCalculator.setRpmValue(150);

	engine->module<MapAveragingModule>()->onFastCallback();

	ASSERT_EQ(2, engineConfiguration->cylindersCount);
	EXPECT_FLOAT_EQ(52.5f, engine->engineState.mapAveragingStart[0]);  // 0 + 75 - 22.5
	EXPECT_FLOAT_EQ(457.5f, engine->engineState.mapAveragingStart[1]); // 360 + 75 + 22.5
	EXPECT_FLOAT_EQ(50, engine->engineState.mapAveragingDuration);
}

TEST(EngineModules, MapAveragingModule_onEnginePhase60_2_one_cylinder) {
    EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
    engineConfiguration->isMapAveragingEnabled = true;
    engineConfiguration->measureMapOnlyInOneCylinder = true;
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);
	MapAveragingModule mapModule;
	engine->rpmCalculator.setRpmValue(200);

	engine->module<MapAveragingModule>()->onEnginePhase(200, getTimeNowNt(), 0.f, 180.f);

	// we expect offset of enginePhase (0 since we call onEnginePhase directy) + 100° of samplingAngle (default setting)
	bool averageDone = eth.assertEventExistsAtEnginePhase("startMapAveraging callback", startAveragingAction, static_cast<angle_t>(100));
    EXPECT_TRUE(averageDone);
}

TEST(EngineModules, MapAveragingModule_onEnginePhase60_2_one_cylinderCustomSampleWindow) {
    EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
    engineConfiguration->isMapAveragingEnabled = true;
    engineConfiguration->measureMapOnlyInOneCylinder = true;
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);
	MapAveragingModule mapModule;
	engine->rpmCalculator.setRpmValue(200);
	setArrayValues(engineConfiguration->map.samplingAngle, 75);

	engine->module<MapAveragingModule>()->onFastCallback();
	engine->module<MapAveragingModule>()->onEnginePhase(200, getTimeNowNt(), 0.f, 180.f);

	// we expect offset of enginePhase (0 since we call onEnginePhase directy) + 75° of samplingAngle
	bool averageDone = eth.assertEventExistsAtEnginePhase("startMapAveraging callback", startAveragingAction, static_cast<angle_t>(75));
    EXPECT_TRUE(averageDone);
}

namespace {

class MapSamplingAngle : public testing::Test {
protected:
	EngineTestHelper eth{engine_type_e::TEST_CRANK_ENGINE};
	MapAveragingModule& module = *engine->module<MapAveragingModule>();

	void SetUp() override {
		engineConfiguration->isMapAveragingEnabled = true;
		engineConfiguration->measureMapOnlyInOneCylinder = true;
		engineConfiguration->isIgnitionEnabled = false;
		engineConfiguration->isInjectionEnabled = false;
		engine->rpmCalculator.setRpmValue(1000);
		module.init();
		setWindow(100, 50);
		engine->scheduler.clear();
	}

	void TearDown() override {
		engine->scheduler.clear();
		getMapAvg(0).stop();
	}

	void setWindow(float start, float duration) {
		setArrayValues(engineConfiguration->map.samplingAngle, start);
		setArrayValues(engineConfiguration->map.samplingWindow, duration);
		module.onFastCallback();
	}

	void expectStart(int queueIndex, size_t cylinder, efitimeus_t edgeUs, float delayUs) {
		auto event = engine->scheduler.getForUnitTest(queueIndex);
		ASSERT_NE(nullptr, event);
		EXPECT_EQ(startAveragingAction.getCallback(), event->action.getCallback());
		EXPECT_EQ(&module.samplers[cylinder], event->action.getArgument<mapSampler*>());
		EXPECT_EQ(cylinder, module.samplers[cylinder].cylinderNumber);
		EXPECT_NEAR(edgeUs + delayUs, event->getMomentUs(), 1);
	}
};

TEST_F(MapSamplingAngle, InterpolatesIndependentAngleAndWindowRpmCurves) {
	auto& map = engineConfiguration->map;
	for (size_t i = 0; i < efi::size(map.samplingAngle); i++) {
		map.samplingAngleBins[i] = 1000 + 1000 * i;
		map.samplingAngle[i] = 40 + 20 * i;
	}
	for (size_t i = 0; i < efi::size(map.samplingWindow); i++) {
		map.samplingWindowBins[i] = 1000 + 2000 * i;
		map.samplingWindow[i] = 20 + 10 * i;
	}
	struct Sample { float rpm; float angle; float duration; };
	for (const auto& sample : {Sample{0, 40, 20}, Sample{1000, 40, 20},
		Sample{1500, 50, 22.5f}, Sample{2000, 60, 25}, Sample{16000, 180, 90}}) {
		SCOPED_TRACE(sample.rpm);
		engine->rpmCalculator.setRpmValue(sample.rpm);
		module.onFastCallback();
		EXPECT_FLOAT_EQ(sample.angle, engine->engineState.mapAveragingStart[0]);
		EXPECT_FLOAT_EQ(sample.duration, engine->engineState.mapAveragingDuration);
	}
}

TEST_F(MapSamplingAngle, WrapsNegativeAndOverflowAnglesForEveryCylinder) {
	struct Sample { float angle; std::array<float, 4> starts; };
	for (const auto& sample : {Sample{-100, {620, 440, 80, 260}},
		Sample{300, {300, 120, 480, 660}}, Sample{720, {0, 540, 180, 360}}}) {
		SCOPED_TRACE(sample.angle);
		setWindow(sample.angle, 50);
		for (size_t cylinder = 0; cylinder < sample.starts.size(); cylinder++) {
			EXPECT_FLOAT_EQ(sample.starts[cylinder], engine->engineState.mapAveragingStart[cylinder]);
		}
	}
}

TEST_F(MapSamplingAngle, ClampsWindowToTenDegreesAndCylinderPeriodMinusTen) {
	struct Sample { float window; float expected; };
	for (const auto& sample : {Sample{-50, 10}, Sample{0, 10}, Sample{5, 10},
		Sample{10, 10}, Sample{50, 50}, Sample{170, 170}, Sample{180, 170}, Sample{720, 170}}) {
		SCOPED_TRACE(sample.window);
		setWindow(100, sample.window);
		EXPECT_FLOAT_EQ(sample.expected, engine->engineState.mapAveragingDuration);
	}

	setHarley();
	// applyTriggerWaveform also broadcasts a configuration change to static ETB
	// controllers, whose PID pointers may outlive a previous EngineTestHelper.
	// Only trigger/cylinder geometry is relevant to these MAP tests.
	engine->updateTriggerConfiguration();
	setWindow(100, 720);
	EXPECT_FLOAT_EQ(350, engine->engineState.mapAveragingDuration);
}

TEST_F(MapSamplingAngle, UsesTwoStrokeCycleForWrappingAndWindowLimit) {
	engineConfiguration->twoStroke = true;
	engine->updateTriggerConfiguration();
	ASSERT_FLOAT_EQ(360, engine->engineState.engineCycle);
	setWindow(-20, 200);
	const float expected[] = {340, 250, 70, 160};
	for (size_t cylinder = 0; cylinder < efi::size(expected); cylinder++) {
		EXPECT_FLOAT_EQ(expected[cylinder], engine->engineState.mapAveragingStart[cylinder]);
	}
	EXPECT_FLOAT_EQ(80, engine->engineState.mapAveragingDuration);
}

TEST_F(MapSamplingAngle, IncludesCurrentPhaseAndExcludesNextPhase) {
	struct Interval { float current; float next; bool scheduled; };
	for (const auto& interval : {Interval{100, 180, true}, Interval{50, 100, false},
		Interval{101, 180, false}, Interval{0, 50, false}}) {
		SCOPED_TRACE(interval.current);
		engine->scheduler.clear();
		module.onEnginePhase(1000, getTimeNowNt(), interval.current, interval.next);
		ASSERT_EQ(interval.scheduled ? 1 : 0, engine->scheduler.size());
		if (interval.scheduled) {
			expectStart(0, 0, getTimeNowUs(), 0);
		}
	}
}

TEST_F(MapSamplingAngle, SchedulesAcrossCycleBoundaryWithCorrectDelay) {
	struct Sample { float start; bool scheduled; float delayUs; };
	for (const auto& sample : {Sample{10, true, 5000}, Sample{700, true, 0},
		Sample{20, false, 0}, Sample{690, false, 0}}) {
		SCOPED_TRACE(sample.start);
		engine->scheduler.clear();
		setWindow(sample.start, 50);
		module.onEnginePhase(1000, getTimeNowNt(), 700, 20);
		ASSERT_EQ(sample.scheduled ? 1 : 0, engine->scheduler.size());
		if (sample.scheduled) {
			// 700 -> 10 is 30 degrees, or 5 ms at 1000 RPM.
			expectStart(0, 0, getTimeNowUs(), sample.delayUs);
		}
	}
}

TEST_F(MapSamplingAngle, DisabledAveragingDoesNotScheduleAnyCylinder) {
	engineConfiguration->isMapAveragingEnabled = false;
	engineConfiguration->measureMapOnlyInOneCylinder = false;
	module.onEnginePhase(1000, getTimeNowNt(), 0, 700);
	EXPECT_EQ(0, engine->scheduler.size());
	EXPECT_FALSE(engine->outputChannels.isMapAveraging);
}

TEST_F(MapSamplingAngle, SchedulesAllCylindersInFiringOrder) {
	engineConfiguration->measureMapOnlyInOneCylinder = false;
	module.onEnginePhase(1000, getTimeNowNt(), 0, 700);
	ASSERT_EQ(4, engine->scheduler.size());
	// 1-3-4-2 firing order, at 100/280/460/640 degrees and 1000 RPM.
	expectStart(0, 0, getTimeNowUs(), 16666.667f);
	expectStart(1, 2, getTimeNowUs(), 46666.667f);
	expectStart(2, 3, getTimeNowUs(), 76666.667f);
	expectStart(3, 1, getTimeNowUs(), 106666.667f);
}

TEST_F(MapSamplingAngle, SingleCylinderModeSchedulesOnlyCylinderOne) {
	module.onEnginePhase(1000, getTimeNowNt(), 0, 700);
	ASSERT_EQ(1, engine->scheduler.size());
	expectStart(0, 0, getTimeNowUs(), 16666.667f);
}

TEST_F(MapSamplingAngle, UsesTriggerTimestampRatherThanCallbackTime) {
	eth.moveTimeForwardUs(10000);
	const auto edgeUs = getTimeNowUs() - 2000;
	module.onEnginePhase(1000, US2NT(edgeUs), 50, 180);
	ASSERT_EQ(1, engine->scheduler.size());
	expectStart(0, 0, edgeUs, 8333.333f);
}

TEST_F(MapSamplingAngle, ZeroDurationDoesNotStartAveraging) {
	engine->engineState.mapAveragingDuration = 0;
	startAveraging(&module.samplers[0]);
	EXPECT_EQ(0, engine->scheduler.size());
	EXPECT_FALSE(engine->outputChannels.isMapAveraging);
}

TEST_F(MapSamplingAngle, TriggerEdgesStartAndStopWindowAtConfiguredAngles) {
	setWindow(75, 30);
	engineConfiguration->globalTriggerAngleOffset = 0;
	engine->updateTriggerConfiguration();
	// The test engine's half-moon crank wheel has an edge every 180 degrees.
	// Feed real edges through the decoder to establish 150 RPM and engine phase.
	eth.smartFireTriggerEvents2(10, 200);
	ASSERT_FLOAT_EQ(150, Sensor::getOrZero(SensorType::Rpm));
	for (int edge = 0; edge < 4 && engine->triggerCentral.currentEngineDecodedPhase != 0; edge++) {
		if (edge % 2 == 0) {
			eth.smartFireRise(200);
		} else {
			eth.smartFireFall(200);
		}
	}
	ASSERT_FLOAT_EQ(0, engine->triggerCentral.currentEngineDecodedPhase);
	const auto edgeUs = getTimeNowUs();
	// Drain the TDC marker queued at this edge, before the future MAP window.
	eth.executeActions();
	EXPECT_FALSE(engine->outputChannels.isMapAveraging);
	ASSERT_EQ(1, engine->scheduler.size());
	expectStart(0, 0, edgeUs, 83333.333f); // 75 degrees at 150 RPM
	const auto startNt = engine->scheduler.getHead()->getMomentNt();
	eth.setTimeAndInvokeEventsNt(startNt - US2NT(1));
	EXPECT_FALSE(engine->outputChannels.isMapAveraging);
	eth.setTimeAndInvokeEventsNt(startNt);
	EXPECT_TRUE(engine->outputChannels.isMapAveraging);
	ASSERT_EQ(1, engine->scheduler.size());
	const auto endNt = engine->scheduler.getHead()->getMomentNt();
	EXPECT_NEAR(33333.333f, NT2USF(endNt - startNt), 1); // 30-degree window
	eth.setTimeAndInvokeEventsNt(endNt - US2NT(1));
	EXPECT_TRUE(engine->outputChannels.isMapAveraging);
	eth.setTimeAndInvokeEventsNt(endNt);
	EXPECT_FALSE(engine->outputChannels.isMapAveraging);
	EXPECT_EQ(0, engine->scheduler.size());
}

} // namespace
