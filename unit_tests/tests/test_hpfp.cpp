#include "pch.h"

#include "high_pressure_fuel_pump.h"
#include "fuel_computer.h"

using ::testing::_;
using ::testing::StrictMock;

TEST(HPFP, Lobe) {
	EngineTestHelper eth(TEST_ENGINE);

	engineConfiguration->hpfpCam = HPFP_CAM_NONE;
	engineConfiguration->hpfpPeakPos = 123;
	engineConfiguration->hpfpCamLobes = 3;

	engine->triggerCentral.vvtPosition[0][0] = 20; // Bank 0
	engine->triggerCentral.vvtPosition[0][1] = 40;
	engine->triggerCentral.vvtPosition[1][0] = 60; // Bank 1
	engine->triggerCentral.vvtPosition[1][1] = 80;

	HpfpLobe lobe;

	// Run through all five CAM modes
	for (int cam = 0; cam < 5; cam++) {
		static hpfp_cam_e map[5] = { HPFP_CAM_NONE, HPFP_CAM_IN1, HPFP_CAM_EX1,
					     HPFP_CAM_IN2, HPFP_CAM_EX2};
		engineConfiguration->hpfpCam = map[cam];

		// Run through several cycles of the engine to make sure we keep wrapping around
		for (int i = 0; i < 4; i++) {
			EXPECT_EQ(lobe.findNextLobe(), 240 + 123 + cam * 20);
			EXPECT_EQ(lobe.findNextLobe(), 480 + 123 + cam * 20);
			EXPECT_EQ(lobe.findNextLobe(),   0 + 123 + cam * 20);
		}
	}

	// Can we change the number of lobes?
	engineConfiguration->hpfpCam = HPFP_CAM_NONE;
	engineConfiguration->hpfpCamLobes = 4;
	EXPECT_EQ(lobe.findNextLobe(), 180 + 123);
	EXPECT_EQ(lobe.findNextLobe(), 360 + 123);
	EXPECT_EQ(lobe.findNextLobe(), 540 + 123);
	EXPECT_EQ(lobe.findNextLobe(),   0 + 123);

	// Can we change the peak position?
	engineConfiguration->hpfpPeakPos = 95;
	EXPECT_EQ(lobe.findNextLobe(), 180 + 95);
	EXPECT_EQ(lobe.findNextLobe(), 360 + 95);
	EXPECT_EQ(lobe.findNextLobe(), 540 + 95);
	EXPECT_EQ(lobe.findNextLobe(),   0 + 95);
}

TEST(HPFP, InjectionReplacementFuel) {
	EngineTestHelper eth(TEST_ENGINE);

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->hpfpCamLobes = 4;
	engine->injectionMass[0] = 0.05 /* cc/cyl */ * fuelDensity;
	engineConfiguration->hpfpPumpVolume = 0.2; // cc/lobe

	HpfpQuantity math;

	EXPECT_FLOAT_EQ(math.calcFuelPercent(1000), 25);

	// What if we change the # of lobes?
	engineConfiguration->hpfpCamLobes = 3;
	EXPECT_FLOAT_EQ(math.calcFuelPercent(1000), 25 * 1.333333333f);

	// More fuel!
	engine->injectionMass[0] = 0.1 /* cc/cyl */ * fuelDensity;
	EXPECT_FLOAT_EQ(math.calcFuelPercent(1000), 50 * 1.333333333f);

	// More cylinders!
	engineConfiguration->specs.cylindersCount = 6;
	EXPECT_FLOAT_EQ(math.calcFuelPercent(1000), 50 * 2.); // Ooops we maxed out

	// Compensation testing
	engineConfiguration->specs.cylindersCount =
		engineConfiguration->hpfpCamLobes; // Make math easier
	for (int i = 0; i < HPFP_COMPENSATION_SIZE; i++) {
		// one bin every 1000 RPM
		engineConfiguration->hpfpCompensationRpmBins[i] = std::min(i * 1000, 8000);
	}
	for (int i = 0; i < HPFP_COMPENSATION_SIZE; i++) {
		// one bin every 0.05 cc/lobe
		engineConfiguration->hpfpCompensationLoadBins[i] = std::min(i * 0.05, 60.);
	}

	engineConfiguration->hpfpCompensation[2][1] = -10;
	EXPECT_FLOAT_EQ(math.calcFuelPercent(1000), 40); // -10, in cell
	EXPECT_FLOAT_EQ(math.calcFuelPercent(1500), 45); // -5, half way
	EXPECT_FLOAT_EQ(math.calcFuelPercent(2000), 50); // -0, in next cell

	engineConfiguration->hpfpCompensation[2][1] = 20;
	EXPECT_FLOAT_EQ(math.calcFuelPercent(1000), 70); // +20, in cell
	EXPECT_FLOAT_EQ(math.calcFuelPercent(1500), 60); // +10, half way
	EXPECT_FLOAT_EQ(math.calcFuelPercent(2000), 50); // +0, in next cell

	// 25% more fuel (1.25*50=62.5 base), but half way between comp of 20 and 0 (so comp of 10)
	engine->injectionMass[0] = 0.125 /* cc/cyl */ * fuelDensity;
	EXPECT_FLOAT_EQ(math.calcFuelPercent(1000), 72.5); // +10 comp
	EXPECT_FLOAT_EQ(math.calcFuelPercent(1500), 67.5); // +5, half way
	EXPECT_FLOAT_EQ(math.calcFuelPercent(2000), 62.5); // +0 base
}

TEST(HPFP, PI) {
	EngineTestHelper eth(TEST_ENGINE);

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->hpfpCamLobes = 4;
	engine->injectionMass[0] = 0.05 /* cc/cyl */ * fuelDensity;
	engineConfiguration->hpfpPumpVolume = 0.2; // cc/lobe

	HpfpQuantity math;

	for (int i = 0; i < HPFP_TARGET_SIZE; i++) {
		// one bin every 1000 RPM
		engineConfiguration->hpfpTargetRpmBins[i] = std::min(i * 1000, 8000);
	}
	for (int i = 0; i < HPFP_TARGET_SIZE; i++) {
		// one bin every 20kPa
		engineConfiguration->hpfpTargetLoadBins[i] = std::min(i * 20, 200);
	}
	for (int r = 0; r < HPFP_TARGET_SIZE; r++) {
		for (int c = 0; c < HPFP_TARGET_SIZE; c++) {
			engineConfiguration->hpfpTarget[r][c] = 1000 * r + 10 * c;
		}
	}

	Sensor::setMockValue(SensorType::Map, 40);
	Sensor::setMockValue(SensorType::FuelPressureHigh, 1000);
	EXPECT_FLOAT_EQ(math.calcPI(1000, 120), -20); // Test integral clamp
	EXPECT_FLOAT_EQ(math.m_I_sum_percent, -20); // and again
	EXPECT_FLOAT_EQ(math.m_pressureTarget_kPa, 2010);
	EXPECT_FLOAT_EQ(math.calcPI(1000, -40), 40); // Test integral clamp
	EXPECT_FLOAT_EQ(math.m_I_sum_percent, 40); // and again
	EXPECT_FLOAT_EQ(math.m_pressureTarget_kPa, 2010);

	// Test pressure decay
	math.calcPI(4000, 0);
	EXPECT_FLOAT_EQ(math.m_pressureTarget_kPa, 2040);
	math.calcPI(1000, 0);
	EXPECT_FLOAT_EQ(math.m_pressureTarget_kPa, 2040);
	engineConfiguration->hpfpTargetDecay = 1000;
	math.calcPI(1000, 0);
	EXPECT_FLOAT_EQ(math.m_pressureTarget_kPa, 2035); // 5ms of decay

	// Proportional gain
	math.reset(); // Reset for ease of testing
	EXPECT_FLOAT_EQ(math.calcPI(1000, 0), 0); // Validate reset
	EXPECT_FLOAT_EQ(math.m_pressureTarget_kPa, 2010);
	engineConfiguration->hpfpPidP = 0.01;
	EXPECT_FLOAT_EQ(math.calcPI(1000, 0), 10.10);
	engineConfiguration->hpfpPidP = 0.02;
	EXPECT_FLOAT_EQ(math.calcPI(1000, 0), 20.20);

	// Integral gain
	engineConfiguration->hpfpPidI = 0.001;
	EXPECT_FLOAT_EQ(math.calcPI(1000, 0), 20.368334);
	EXPECT_FLOAT_EQ(math.m_I_sum_percent, 0.168333333);
}

TEST(HPFP, Angle) {
	EngineTestHelper eth(TEST_ENGINE);

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->hpfpCamLobes = 4;
	engine->injectionMass[0] = 0.05 /* cc/cyl */ * fuelDensity;
	engineConfiguration->hpfpPumpVolume = 0.2; // cc/lobe

	HpfpQuantity math;

	for (int i = 0; i < HPFP_TARGET_SIZE; i++) {
		// one bin every 1000 RPM
		engineConfiguration->hpfpTargetRpmBins[i] = std::min(i * 1000, 8000);
	}
	for (int i = 0; i < HPFP_TARGET_SIZE; i++) {
		// one bin every 20kPa
		engineConfiguration->hpfpTargetLoadBins[i] = std::min(i * 20, 200);
	}
	for (int r = 0; r < HPFP_TARGET_SIZE; r++) {
		for (int c = 0; c < HPFP_TARGET_SIZE; c++) {
			engineConfiguration->hpfpTarget[r][c] = 1000 * r + 10 * c;
		}
	}
	for (int i = 0; i < HPFP_LOBE_PROFILE_SIZE; i++) {
		engineConfiguration->hpfpLobeProfileQuantityBins[i] = 100. * i / (HPFP_LOBE_PROFILE_SIZE - 1);
		engineConfiguration->hpfpLobeProfileAngle[i] = 150. * i / (HPFP_LOBE_PROFILE_SIZE - 1);
	}

	EXPECT_FLOAT_EQ(math.calcFuelPercent(1000), 25); // Double check baseline
	EXPECT_FLOAT_EQ(math.calcPI(1000, 10), 0); // Validate no PI
	EXPECT_NEAR(math.pumpAngleFuel(1000), 37.5, 0.4); // Given the profile, should be 50% higher

	engine->injectionMass[0] = 0.08 /* cc/cyl */ * fuelDensity;
	EXPECT_FLOAT_EQ(math.calcFuelPercent(1000), 40); // Double check baseline
	EXPECT_FLOAT_EQ(math.calcPI(1000, 10), 0); // Validate no PI
	EXPECT_NEAR(math.pumpAngleFuel(1000), 60, 0.4); // Given the profile, should be 50% higher

	engineConfiguration->hpfpPidP = 0.01;
	Sensor::setMockValue(SensorType::Map, 40);
	Sensor::setMockValue(SensorType::FuelPressureHigh, 1000);
	EXPECT_FLOAT_EQ(math.calcPI(1000, 10), 10.1);
	EXPECT_NEAR(math.pumpAngleFuel(1000), 50.1 * 1.5, 0.4); // Given the profile, should be 50% higher
}

TEST(HPFP, Schedule) {
	EngineTestHelper eth(TEST_ENGINE);

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->hpfpCamLobes = 4;
	engineConfiguration->hpfpPumpVolume = 0.2; // cc/lobe

	for (int i = 0; i < HPFP_TARGET_SIZE; i++) {
		// one bin every 1000 RPM
		engineConfiguration->hpfpTargetRpmBins[i] = std::min(i * 1000, 8000);
	}
	for (int i = 0; i < HPFP_TARGET_SIZE; i++) {
		// one bin every 20kPa
		engineConfiguration->hpfpTargetLoadBins[i] = std::min(i * 20, 200);
	}
	for (int r = 0; r < HPFP_TARGET_SIZE; r++) {
		for (int c = 0; c < HPFP_TARGET_SIZE; c++) {
			engineConfiguration->hpfpTarget[r][c] = 1000 * r + 10 * c;
		}
	}
	for (int i = 0; i < HPFP_LOBE_PROFILE_SIZE; i++) {
		engineConfiguration->hpfpLobeProfileQuantityBins[i] = 100. * i / (HPFP_LOBE_PROFILE_SIZE - 1);
		engineConfiguration->hpfpLobeProfileAngle[i] = 150. * i / (HPFP_LOBE_PROFILE_SIZE - 1);
	}

	auto & hpfp = *engine->module<HpfpController>();

	StrictMock<MockExecutor> mockExec;
	engine->executor.setMockExecutor(&mockExec);

	constexpr angle_t angle0 = 90;
	constexpr angle_t angle1 = 270 - 37.6923065;
	constexpr angle_t angle2 = angle1 + 0;

	constexpr float tick_per_deg = USF2NT(1000000.)*60/360/1000;

	constexpr efitick_t nt0 = tick_per_deg * angle0;
	constexpr efitick_t nt1 = tick_per_deg * angle1;
	constexpr efitick_t nt2 = tick_per_deg * angle2;

	{
		testing::InSequence is;

		// First call to assignRpmValue will cause a dummy call to fast periodic timer.
		// Injection Mass will be 0 so expect a no-op.
		EXPECT_CALL(mockExec, scheduleByTimestampNt(testing::NotNull(), &hpfp.m_event.scheduling, nt0, action_s(HpfpController::pinTurnOff, &hpfp)));

		// Second call will be the start of a real pump event.
		EXPECT_CALL(mockExec, scheduleByTimestampNt(testing::NotNull(), &hpfp.m_event.scheduling, nt1, action_s(HpfpController::pinTurnOn, &hpfp)));

		// Third call will be off event
		EXPECT_CALL(mockExec, scheduleByTimestampNt(testing::NotNull(), &hpfp.m_event.scheduling, nt2, action_s(HpfpController::pinTurnOff, &hpfp)));
	}
	EXPECT_CALL(mockExec, cancel(_)).Times(2);

	// For HPFP to work, events need to be scheduled after the next tooth.  This makes sure the
	// peak pos occurs after the next tooth.
	engineConfiguration->hpfpPeakPos = 90;
	// This will call the fast callback routine
	engine->rpmCalculator.assignRpmValue(1000);
	engine->injectionMass[0] = 0.05 /* cc/cyl */ * fuelDensity;
	engineConfiguration->hpfpValvePin = GPIOA_2; // arbitrary

	hpfp.onFastCallback();
	// First event was scheduled by assignRpmValue with 0 injection mass.  So, it's off.
	eth.assertTriggerEvent("h0", 0, &hpfp.m_event, (void*)&HpfpController::pinTurnOff,
			       1, angle0 - 0);

	// Make the previous event happen, schedule the next.
	engine->module<TriggerScheduler>()->scheduleEventsUntilNextTriggerTooth(
		1000, 1, tick_per_deg * 0);
	// Mock executor doesn't run events, so we run it manually
	HpfpController::pinTurnOff(&hpfp);

	// Now we should have a regular on/off event.
	eth.assertTriggerEvent("h1", 0, &hpfp.m_event, (void*)&HpfpController::pinTurnOn,
			       2, angle1 - 180);

	// Make it happen
	engine->module<TriggerScheduler>()->scheduleEventsUntilNextTriggerTooth(
		1000, 2, tick_per_deg * 180);

	// Since we have a mock scheduler, lets insert the correct timestamp in the scheduling
	// struct.
	hpfp.m_event.scheduling.momentX = nt2;

	HpfpController::pinTurnOn(&hpfp);

	// The off event goes directly to scheduleByAngle and is tested by the last EXPECT_CALL
	// above.
}
