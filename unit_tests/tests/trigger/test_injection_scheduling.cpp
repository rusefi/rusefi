#include "engine_test_helper.h"
#include "main_trigger_callback.h"

#include <gmock/gmock.h>
#include "mocks.h"

using ::testing::_;
using ::testing::StrictMock;
using ::testing::InSequence;

TEST(injectionScheduling, NormalDutyCycle) {
	StrictMock<MockExecutor> mockExec;

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	engine->executor.setMockExecutor(&mockExec);

	efitick_t nowNt = 1000000;

	InjectionEvent event;
	INJECT_ENGINE_REFERENCE(&event);
	InjectorOutputPin pin;
	pin.injectorIndex = 0;
	event.outputs[0] = &pin;

	// Injection duration of 20ms
	engine->injectionDuration = 20.0f;

	{
		InSequence is;

		// Should schedule one normal injection:
		// rising edge now
		EXPECT_CALL(mockExec, scheduleByTimestampNt(nowNt + 0, _));
		// falling edge 10ms later
		EXPECT_CALL(mockExec, scheduleByTimestampNt(nowNt + MS2NT(20), _));
	}

	engine->rpmCalculator.oneDegreeUs = 100;

	event.onTriggerTooth(0, 1000, nowNt);
}
