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

static bool ActionArgumentHasLowBitSet(const action_s& a) {
	return (reinterpret_cast<uintptr_t>(a.getArgument()) & 1) != 0;
}

TEST(injectionScheduling, InjectionIsScheduled) {
	StrictMock<MockExecutor> mockExec;

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engine->executor.setMockExecutor(&mockExec);

	efitick_t nowNt = 1000000;

	InjectionEvent event;
	InjectorOutputPin pin;
	pin.injectorIndex = 0;
	event.outputs[0] = &pin;

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
		EXPECT_CALL(mockExec, scheduleByTimestampNt(testing::NotNull(), _, startTime, Not(Truly(ActionArgumentHasLowBitSet))));
		// falling edge 20ms later
		EXPECT_CALL(mockExec, scheduleByTimestampNt(testing::NotNull(), _, startTime + MS2NT(20), Property(&action_s::getArgument, Eq(&event))));
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
	engine->executor.setMockExecutor(&mockExec);
	engine->module<InjectorModelPrimary>().set(&im);
	engine->module<InjectorModelSecondary>().set(&im);

	efitick_t nowNt = 1000000;

	InjectionEvent event;
	InjectorOutputPin pin;
	pin.injectorIndex = 0;
	event.outputs[0] = &pin;

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
		EXPECT_CALL(mockExec, scheduleByTimestampNt(testing::NotNull(), _, startTime, Truly(ActionArgumentHasLowBitSet)));
		// falling edge (primary) 20ms later
		EXPECT_CALL(mockExec, scheduleByTimestampNt(testing::NotNull(), _, startTime + MS2NT(20), Property(&action_s::getArgument, Eq(&event))));
		// falling edge (secondary) 10ms later
		EXPECT_CALL(mockExec, scheduleByTimestampNt(testing::NotNull(), _, startTime + MS2NT(10), Property(&action_s::getArgument, Eq(&event))));
	}

	// Event scheduled at 125 degrees
	event.injectionStartAngle = 125;

	// We are at 120 degrees now, next tooth 130
	event.onTriggerTooth(nowNt, 120, 130);
}

TEST(injectionScheduling, InjectionIsScheduledBeforeWraparound) {
	StrictMock<MockExecutor> mockExec;

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engine->executor.setMockExecutor(&mockExec);

	efitick_t nowNt = 1000000;

	InjectionEvent event;
	InjectorOutputPin pin;
	pin.injectorIndex = 0;
	event.outputs[0] = &pin;

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
		EXPECT_CALL(mockExec, scheduleByTimestampNt(testing::NotNull(), _, startTime, Not(Truly(ActionArgumentHasLowBitSet))));
		// falling edge 20ms later
		EXPECT_CALL(mockExec, scheduleByTimestampNt(testing::NotNull(), _, startTime + MS2NT(20), Property(&action_s::getArgument, Eq(&event))));
	}

	// Event scheduled at 715 degrees
	event.injectionStartAngle = 715;

	// We are at 710 degrees now, next tooth 010
	event.onTriggerTooth(nowNt, 710, 010);
}

TEST(injectionScheduling, InjectionIsScheduledAfterWraparound) {
	StrictMock<MockExecutor> mockExec;

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engine->executor.setMockExecutor(&mockExec);

	efitick_t nowNt = 1000000;

	InjectionEvent event;
	InjectorOutputPin pin;
	pin.injectorIndex = 0;
	event.outputs[0] = &pin;

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
		EXPECT_CALL(mockExec, scheduleByTimestampNt(testing::NotNull(), _, startTime, Not(Truly(ActionArgumentHasLowBitSet))));
		// falling edge 20ms later
		EXPECT_CALL(mockExec, scheduleByTimestampNt(testing::NotNull(), _, startTime + MS2NT(20), Property(&action_s::getArgument, Eq(&event))));
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
	engine->executor.setMockExecutor(&mockExec);

	efitick_t nowNt = 1000000;

	InjectionEvent event;
	InjectorOutputPin pin;
	pin.injectorIndex = 0;
	event.outputs[0] = &pin;

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