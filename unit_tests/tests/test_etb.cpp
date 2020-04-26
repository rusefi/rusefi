/*
 * @file test_etb.cpp
 *
 * @date Dec 13, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "engine_test_helper.h"
#include "electronic_throttle.h"
#include "dc_motor.h"
#include "engine_controller.h"
#include "tps.h"

class MockEtb : public IEtbController {
public:
	// todo: somehow I am failinig to figure out GMOCK syntax here?
	// todo: convert to GMOCK
	int resetCount = 0;
	int startCount = 0;
	int initCount = 0;

	void reset() {
		resetCount++;
	}

	void Start() override {
		startCount++;
	}

	void init(DcMotor *motor, int ownIndex, pid_s *pidParameters) {
		initCount++;
	};

	void PeriodicTask() {
	};

	int getPeriodMs() {
		return 1;
	};
};


TEST(etb, singleEtbInitialization) {

	MockEtb mocks[ETB_COUNT];

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	for (int i = 0; i < ETB_COUNT; i++) {
		engine->etbControllers[i] = &mocks[i];
	}

	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_9;

	doInitElectronicThrottle(PASS_ENGINE_PARAMETER_SIGNATURE);

	// assert that 1st ETB is initialized and started
	ASSERT_EQ(1, mocks[0].initCount) << "1st init";
	ASSERT_EQ(1, mocks[0].startCount);

	// assert that 2nd ETB is neither initialized nor started
	ASSERT_EQ(0, mocks[1].initCount) << "2nd init";
	ASSERT_EQ(0, mocks[1].startCount) << "2nd start";


	// todo: set mock pedal position
	// todo: set mock ETB throttle position
	// todo: invoke EtbController#PeriodicTask a few times and assert that duty cycle changes
}

TEST(etb, testTargetTpsIsFloatBug945) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	doInitElectronicThrottle(PASS_ENGINE_PARAMETER_SIGNATURE);

	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_0;

	setMockThrottlePedalSensorVoltage(3 PASS_ENGINE_PARAMETER_SUFFIX);
	engine->etbControllers[0]->PeriodicTask();
	ASSERT_NEAR(50, engine->engineState.targetFromTable, EPS4D);

	setMockThrottlePedalSensorVoltage(3.05 PASS_ENGINE_PARAMETER_SUFFIX);
	ASSERT_NEAR(50.8302, getPedalPosition(PASS_ENGINE_PARAMETER_SIGNATURE), EPS4D);
	engine->etbControllers[0]->PeriodicTask();
	ASSERT_NEAR(50.8302, engine->engineState.targetFromTable, EPS4D);

	setMockThrottlePedalSensorVoltage(3.1 PASS_ENGINE_PARAMETER_SUFFIX);
	engine->etbControllers[0]->PeriodicTask();
	ASSERT_NEAR(51.6605, engine->engineState.targetFromTable, EPS4D);
}
