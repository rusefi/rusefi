//
// Created by FDSoftware on 10/04/25.
//

#include "pch.h"
#include "ignition_state.h"

using ::testing::_;

TEST(ignition_state, getRunningAdvanceSimple) {
  EngineTestHelper eth(engine_type_e::TEST_ENGINE);
  const float rpm = 4500;
  const float load = 50;

  setWholeTimingTable(10);
  initIgnitionAdvanceControl();

  // invalid load, should return NAN
  auto correction = getRunningAdvance(rpm, NAN);
  EXPECT_TRUE(std::isnan(correction));

  // valid load, all secondary conditions off == 10°
  Sensor::setMockValue(SensorType::Clt, 35);
  Sensor::setMockValue(SensorType::VehicleSpeed, 50);
  Sensor::setMockValue(SensorType::WheelSlipRatio, 0);

  // coverage on: test_ignition_angle_advance.cpp
  engineConfiguration->launchControlEnabled = false;
  // to cover: (on test_antilag.cpp)
  engineConfiguration->antiLagEnabled = false;
  // coverage on: test_shift_torque_reduction_spark_skip_ratio.cpp
  engineConfiguration->torqueReductionEnabled = false;
  // coverage on: test_nitrous_ignition_retard.cpp
  engineConfiguration->nitrousControlEnabled = false;
  // to cover (this same file)
  engineConfiguration->useSeparateAdvanceForIdle = false;

  correction = getRunningAdvance(rpm, load);
  EXPECT_NEAR(10, getRunningAdvance(rpm, load), EPS2D);
}

struct MockIdle : public MockIdleController {
  bool isIdling = true;

  bool isIdlingOrTaper() const override {
    return isIdling;
  }
};

TEST(ignition_state, getRunningAdvanceIdleTable) {
  EngineTestHelper eth(engine_type_e::TEST_ENGINE);
  const float rpm = 900;
  const float load = 30;
  setWholeTimingTable(10);
  setArrayValues(config->idleAdvance, 5);

  MockIdle idler;
  engine->engineModules.get<IdleController>().set(&idler);

  Sensor::setMockValue(SensorType::DriverThrottleIntent, 0);

  engineConfiguration->useSeparateAdvanceForIdle = true;
  engineConfiguration->idlePidDeactivationTpsThreshold = 2;

  // no TPS === running advance
  EXPECT_NEAR(10, getRunningAdvance(rpm, load), EPS2D);

  idler.isIdling = true;
  EXPECT_TRUE(engine->module<IdleController>()->isIdlingOrTaper());

  // 0% of idle threshold -> idle table
  Sensor::setMockValue(SensorType::DriverThrottleIntent, 0.001);
  EXPECT_NEAR(5, getRunningAdvance(rpm, load), EPS2D);

  // 50% of idle threshold -> idle table
  Sensor::setMockValue(SensorType::DriverThrottleIntent, 1);
  EXPECT_NEAR(7.5, getRunningAdvance(rpm, load), EPS2D);

  // idle threshold -> normal table
  Sensor::setMockValue(SensorType::DriverThrottleIntent, 2);
  EXPECT_NEAR(10, getRunningAdvance(rpm, load), EPS2D);
}