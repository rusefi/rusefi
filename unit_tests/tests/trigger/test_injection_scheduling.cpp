#include "pch.h"
#include "main_trigger_callback.h"
#include "injector_model.h"

using ::testing::_;
using ::testing::StrictMock;
using ::testing::InSequence;

using ::testing::Eq;
using ::testing::Not;
using ::testing::Property;
using ::testing::Truly;

TEST(injectionScheduling, InjectionIsScheduled) {
	StrictMock<MockExecutor> mockExec;

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engine->scheduler.setMockExecutor(&mockExec);

	efitick_t nowNt = 1000000;

	InjectionEvent event;

	// Injection duration of 20ms
	MockInjectorModel2 im;
	EXPECT_CALL(im, getInjectionDuration(_)).WillOnce(Return(20.0f));
	engine->module<InjectorModelPrimary>().set(&im);

	engine->rpmCalculator.oneDegreeUs = 100;

	{
		InSequence is;

		// Should schedule one normal injection:
		// rising edge 5 degrees from now
		float nt5deg = USF2NT(engine->rpmCalculator.oneDegreeUs * 5);
		efitick_t startTime = nowNt + nt5deg;
		EXPECT_CALL(mockExec, schedule(testing::NotNull(), _, startTime, Property(&action_s::getArgumentRaw, Eq(InjectorContext(1, 0xF, false).raw))));
		// falling edge 20ms later
		EXPECT_CALL(mockExec, schedule(testing::NotNull(), _, startTime + MS2NT(20), Property(&action_s::getArgumentRaw, Eq(InjectorContext(1, 0xF, false).raw))));
	}

	// Event scheduled at 125 degrees
	event.injectionStartAngle = 125;

	// We are at 120 degrees now, next tooth 130
	event.onTriggerTooth(nowNt, 120, 130);
}

TEST(injectionScheduling, InjectionIsScheduledDualStage) {
	StrictMock<MockExecutor> mockExec;
	StrictMock<MockInjectorModel2> im;

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engine->scheduler.setMockExecutor(&mockExec);
	engine->module<InjectorModelPrimary>().set(&im);
	engine->module<InjectorModelSecondary>().set(&im);

	efitick_t nowNt = 1000000;

	InjectionEvent event;

	engine->rpmCalculator.oneDegreeUs = 100;

	// Some nonzero fuel quantity on both stages
	engine->engineState.injectionMass[0] = 50;
	engine->engineState.injectionStage2Fraction = 0.2;

	{
		InSequence is;

		// Primary injection duration of 20ms, secondary 10ms
		EXPECT_CALL(im, getInjectionDuration(40)).WillOnce(Return(20.0f));
		EXPECT_CALL(im, getInjectionDuration(10)).WillOnce(Return(10.0f));
	}

	{
		InSequence is;

		// Should schedule one normal injection:
		// rising edge 5 degrees from now
		float nt5deg = USF2NT(engine->rpmCalculator.oneDegreeUs * 5);
		efitick_t startTime = nowNt + nt5deg;
		EXPECT_CALL(mockExec, schedule(testing::NotNull(), _, startTime, Property(&action_s::getArgumentRaw, Eq(InjectorContext(1, 0xF, true).raw))));
		// falling edge (primary) 20ms later
		EXPECT_CALL(mockExec, schedule(testing::NotNull(), _, startTime + MS2NT(20), Property(&action_s::getArgumentRaw, Eq(InjectorContext(1, 0xF, true).raw))));
		// falling edge (secondary) 10ms later
		EXPECT_CALL(mockExec, schedule(testing::NotNull(), _, startTime + MS2NT(10), Property(&action_s::getArgumentRaw, Eq(InjectorContext(1, 0xF, true).raw))));
	}

	// Event scheduled at 125 degrees
	event.injectionStartAngle = 125;

	// We are at 120 degrees now, next tooth 130
	event.onTriggerTooth(nowNt, 120, 130);
}

// Coverage for #10414: stage 2 must close the outputs chosen before a mode change.
TEST(injectionScheduling, Stage2CloseAfterOutputChange) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	MockInjectorModel2 im;
	engine->module<InjectorModelPrimary>().set(&im);
	engine->module<InjectorModelSecondary>().set(&im);
	EXPECT_CALL(im, getInjectionDuration(40)).WillOnce(Return(20.0f));
	EXPECT_CALL(im, getInjectionDuration(10)).WillOnce(Return(10.0f));

	engine->rpmCalculator.oneDegreeUs = 100;
	engine->engineState.injectionMass[0] = 50;
	engine->engineState.injectionStage2Fraction = 0.2f;
	engine->engineState.injectionOffset = 0;
	engine->engineState.injectionDuration = 20;

	InjectionEvent event;
	event.setIndex(0);
	event.injectionStartAngle = 125;
	event.onTriggerTooth(1000000, 120, 130);
	ASSERT_EQ(3, engine->scheduler.size());

	auto open = engine->scheduler.getForUnitTest(0)->action;
	auto closeStage2 = engine->scheduler.getForUnitTest(1)->action;
	open.execute();
	EXPECT_EQ(1, enginePins.injectorsStage2[0].getOverlappingCounter());

	// A mode change rebuilds the event while its previous pulse is active.
	engineConfiguration->injectionMode = IM_BATCH;
	ASSERT_TRUE(event.update());
	ASSERT_NE(1, event.calculateInjectorOutputMask());
	closeStage2.execute();
	EXPECT_EQ(0, enginePins.injectorsStage2[0].getOverlappingCounter());
}

TEST(injectionScheduling, InjectionIsScheduledBeforeWraparound) {
	StrictMock<MockExecutor> mockExec;

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engine->scheduler.setMockExecutor(&mockExec);

	efitick_t nowNt = 1000000;

	InjectionEvent event;

	// Injection duration of 20ms
	MockInjectorModel2 im;
	EXPECT_CALL(im, getInjectionDuration(_)).WillOnce(Return(20.0f));
	engine->module<InjectorModelPrimary>().set(&im);

	engine->rpmCalculator.oneDegreeUs = 100;

	{
		InSequence is;

		// Should schedule one normal injection:
		// rising edge 5 degrees from now
		float nt5deg = USF2NT(engine->rpmCalculator.oneDegreeUs * 5);
		efitick_t startTime = nowNt + nt5deg;
		EXPECT_CALL(mockExec, schedule(testing::NotNull(), _, startTime, Property(&action_s::getArgumentRaw, Eq(InjectorContext(1, 0xF, false).raw))));
		// falling edge 20ms later
		EXPECT_CALL(mockExec, schedule(testing::NotNull(), _, startTime + MS2NT(20), Property(&action_s::getArgumentRaw, Eq(InjectorContext(1, 0xF, false).raw))));
	}

	// Event scheduled at 715 degrees
	event.injectionStartAngle = 715;

	// We are at 710 degrees now, next tooth 010
	event.onTriggerTooth(nowNt, 710, 010);
}

TEST(injectionScheduling, InjectionIsScheduledAfterWraparound) {
	StrictMock<MockExecutor> mockExec;

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engine->scheduler.setMockExecutor(&mockExec);

	efitick_t nowNt = 1000000;

	InjectionEvent event;

	// Injection duration of 20ms
	MockInjectorModel2 im;
	EXPECT_CALL(im, getInjectionDuration(_)).WillOnce(Return(20.0f));
	engine->module<InjectorModelPrimary>().set(&im);

	engine->rpmCalculator.oneDegreeUs = 100;

	{
		InSequence is;

		// Should schedule one normal injection:
		// rising edge 15 degrees from now
		float nt5deg = USF2NT(engine->rpmCalculator.oneDegreeUs * 15);
		efitick_t startTime = nowNt + nt5deg;
		EXPECT_CALL(mockExec, schedule(testing::NotNull(), _, startTime, Property(&action_s::getArgumentRaw, Eq(InjectorContext(1, 0xF, false).raw))));
		// falling edge 20ms later
		EXPECT_CALL(mockExec, schedule(testing::NotNull(), _, startTime + MS2NT(20), Property(&action_s::getArgumentRaw, Eq(InjectorContext(1, 0xF, false).raw))));
	}

	// Event scheduled at 5 degrees
	event.injectionStartAngle = 5;

	// We are at 710 degrees now, next tooth 010
	event.onTriggerTooth(nowNt, 710, 010);
}

TEST(injectionScheduling, InjectionNotScheduled) {
	// StrictMock since we expect no scheduler calls!
	StrictMock<MockExecutor> mockExec;

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engine->scheduler.setMockExecutor(&mockExec);

	efitick_t nowNt = 1000000;

	InjectionEvent event;

	// Expect no calls to injector model
	StrictMock<MockInjectorModel2> im;
	engine->module<InjectorModelPrimary>().set(&im);

	engine->rpmCalculator.oneDegreeUs = 100;

	{
		InSequence is;

		// Expect no scheduler calls!
	}


	// Event scheduled at 125 degrees
	event.injectionStartAngle = 125;

	// We are at 130 degrees now, next tooth 140
	event.onTriggerTooth(nowNt, 130, 140);
}
