#include "pch.h"
#include "main_trigger_callback.h"
#include "injector_model.h"

using ::testing::_;
using ::testing::StrictMock;
using ::testing::InSequence;

TEST(injectionScheduling, InjectionIsScheduled) {
	StrictMock<MockExecutor> mockExec;

	EngineTestHelper eth(TEST_ENGINE);
	engine->executor.setMockExecutor(&mockExec);

	efitick_t nowNt = 1000000;

	InjectionEvent event;
	InjectorOutputPin pin;
	pin.injectorIndex = 0;
	event.outputs[0] = &pin;

	// Injection duration of 20ms
	MockInjectorModel2 im;
	EXPECT_CALL(im, getInjectionDuration(_)).WillOnce(Return(20.0f));
	engine->module<InjectorModel>().set(&im);

	engine->rpmCalculator.oneDegreeUs = 100;

	{
		InSequence is;

		// Should schedule one normal injection:
		// rising edge 5 degrees from now
		float nt5deg = USF2NT(engine->rpmCalculator.oneDegreeUs * 5);
		efitick_t startTime = nowNt + nt5deg;
		EXPECT_CALL(mockExec, scheduleByTimestampNt(testing::NotNull(), &event.signalTimerUp, startTime, _));
		// falling edge 20ms later
		EXPECT_CALL(mockExec, scheduleByTimestampNt(testing::NotNull(), &event.endOfInjectionEvent, startTime + MS2NT(20), _));
	}

	
	// Event scheduled at 125 degrees
	event.injectionStartAngle = 125;

	// We are at 120 degrees now, next tooth 130
	event.onTriggerTooth(1000, nowNt, 120, 130);
}

TEST(injectionScheduling, InjectionIsScheduledBeforeWraparound) {
	StrictMock<MockExecutor> mockExec;

	EngineTestHelper eth(TEST_ENGINE);
	engine->executor.setMockExecutor(&mockExec);

	efitick_t nowNt = 1000000;

	InjectionEvent event;
	InjectorOutputPin pin;
	pin.injectorIndex = 0;
	event.outputs[0] = &pin;

	// Injection duration of 20ms
	MockInjectorModel2 im;
	EXPECT_CALL(im, getInjectionDuration(_)).WillOnce(Return(20.0f));
	engine->module<InjectorModel>().set(&im);

	engine->rpmCalculator.oneDegreeUs = 100;

	{
		InSequence is;

		// Should schedule one normal injection:
		// rising edge 5 degrees from now
		float nt5deg = USF2NT(engine->rpmCalculator.oneDegreeUs * 5);
		efitick_t startTime = nowNt + nt5deg;
		EXPECT_CALL(mockExec, scheduleByTimestampNt(testing::NotNull(), &event.signalTimerUp, startTime, _));
		// falling edge 20ms later
		EXPECT_CALL(mockExec, scheduleByTimestampNt(testing::NotNull(), &event.endOfInjectionEvent, startTime + MS2NT(20), _));
	}

	// Event scheduled at 715 degrees
	event.injectionStartAngle = 715;

	// We are at 710 degrees now, next tooth 010
	event.onTriggerTooth(1000, nowNt, 710, 010);
}

TEST(injectionScheduling, InjectionIsScheduledAfterWraparound) {
	StrictMock<MockExecutor> mockExec;

	EngineTestHelper eth(TEST_ENGINE);
	engine->executor.setMockExecutor(&mockExec);

	efitick_t nowNt = 1000000;

	InjectionEvent event;
	InjectorOutputPin pin;
	pin.injectorIndex = 0;
	event.outputs[0] = &pin;

	// Injection duration of 20ms
	MockInjectorModel2 im;
	EXPECT_CALL(im, getInjectionDuration(_)).WillOnce(Return(20.0f));
	engine->module<InjectorModel>().set(&im);

	engine->rpmCalculator.oneDegreeUs = 100;

	{
		InSequence is;

		// Should schedule one normal injection:
		// rising edge 15 degrees from now
		float nt5deg = USF2NT(engine->rpmCalculator.oneDegreeUs * 15);
		efitick_t startTime = nowNt + nt5deg;
		EXPECT_CALL(mockExec, scheduleByTimestampNt(testing::NotNull(), &event.signalTimerUp, startTime, _));
		// falling edge 20ms later
		EXPECT_CALL(mockExec, scheduleByTimestampNt(testing::NotNull(), &event.endOfInjectionEvent, startTime + MS2NT(20), _));
	}

	// Event scheduled at 5 degrees
	event.injectionStartAngle = 5;

	// We are at 710 degrees now, next tooth 010
	event.onTriggerTooth(1000, nowNt, 710, 010);
}

TEST(injectionScheduling, InjectionNotScheduled) {
	// StrictMock since we expect no scheduler calls!
	StrictMock<MockExecutor> mockExec;

	EngineTestHelper eth(TEST_ENGINE);
	engine->executor.setMockExecutor(&mockExec);

	efitick_t nowNt = 1000000;

	InjectionEvent event;
	InjectorOutputPin pin;
	pin.injectorIndex = 0;
	event.outputs[0] = &pin;

	// Expect no calls to injector model
	StrictMock<MockInjectorModel2> im;
	engine->module<InjectorModel>().set(&im);

	engine->rpmCalculator.oneDegreeUs = 100;

	{
		InSequence is;

		// Expect no scheduler calls!
	}

	
	// Event scheduled at 125 degrees
	event.injectionStartAngle = 125;

	// We are at 130 degrees now, next tooth 140
	event.onTriggerTooth(1000, nowNt, 130, 140);
}