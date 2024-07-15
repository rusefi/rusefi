//
// Created by kifir on 7/15/24.
//
#include "pch.h"

TEST(Alternator, TestSetPoint) {
EngineTestHelper eth(engine_type_e::TEST_ENGINE);

const float TEST_ALTERNATOR_VOLTAGE_TARGET = 14.2f;
setTable(config->alternatorVoltageTargetTable, TEST_ALTERNATOR_VOLTAGE_TARGET);
engineConfiguration->cranking.rpm = 500;
engineConfiguration->isAlternatorControlEnabled = true;

AlternatorController dut;

Sensor::setMockValue(SensorType::Rpm, 500);
// disabled if rpm <= cranking.rpm
EXPECT_EQ(-1, dut.getSetpoint().value_or(-1));

Sensor::setMockValue(SensorType::Rpm, 501);
// enabled!
EXPECT_EQ(TEST_ALTERNATOR_VOLTAGE_TARGET, dut.getSetpoint().value_or(-1));

engineConfiguration->isAlternatorControlEnabled = false;
// disabled manually
EXPECT_EQ(-1, dut.getSetpoint().value_or(-1));
}