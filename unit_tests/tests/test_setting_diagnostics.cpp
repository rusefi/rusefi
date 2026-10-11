#include "pch.h"

#include "can_dash.h"
#include "can_vss.h"
#include "fuel_computer.h"
#include "fuel_math.h"
#include "spark_logic.h"
#include "stepper.h"
#include <stdexcept>
#include <string>

using ::testing::StrEq;
using ::testing::ThrowsMessage;

// [tag:user_facing_ini_labels] Exercise the error producers, including invalid
// values and feature-specific branches. Their messages should name controls
// from tunerstudio.template.ini; qualify generic labels with their UI context.
class SettingDiagnostics : public testing::Test {
protected:
	EngineTestHelper eth{engine_type_e::TEST_ENGINE};

	void prepareIgnition() {
		engine->ignitionState.dwellDurationAngle = 10;
		setArrayValues(engine->engineState.timingAdvance, 10.0f);
	}
};

TEST_F(SettingDiagnostics, PrimaryStoichiometricRatio) {
	engineConfiguration->stoichRatioPrimary = 0;
	EXPECT_THAT([&] { engine->fuelComputer.getStoichiometricRatio(); },
		ThrowsMessage<std::logic_error>(StrEq("Please set Stoichiometric ratio")));
}

TEST_F(SettingDiagnostics, E100StoichiometricRatio) {
	engineConfiguration->stoichRatioPrimary = 14.7f;
	engineConfiguration->stoichRatioSecondary = 0;
	Sensor::setMockValue(SensorType::FuelEthanolPercent, 50);
	EXPECT_THAT([&] { engine->fuelComputer.getStoichiometricRatio(); },
		ThrowsMessage<std::logic_error>(StrEq("Please set E100 stoichiometric ratio")));
}

TEST_F(SettingDiagnostics, ChargeTemperatureMode) {
	engineConfiguration->tChargeMode = static_cast<decltype(engineConfiguration->tChargeMode)>(182);
	EXPECT_THAT([&] { engine->fuelComputer.getTCharge(1000, 20); },
		ThrowsMessage<std::logic_error>(StrEq("Invalid Mode in Estimated cylinder air temperature: 182")));
}

#if EFI_ANTILAG_SYSTEM
TEST_F(SettingDiagnostics, AntiLagActivationMode) {
	engineConfiguration->antiLagActivationMode = static_cast<antiLagActivationMode_e>(182);
	EXPECT_THAT([&] { engine->antilagController.isInsideALSSwitchCondition(); },
		ThrowsMessage<std::logic_error>(StrEq("Invalid Activation Mode in AntiLag/Rotational Idle Settings: 182")));
}
#endif

TEST_F(SettingDiagnostics, AdcReferenceVoltage) {
	engineConfiguration->adcVcc = 0.5f;
	EXPECT_THAT([&] { validateConfigOnStartUpOrBurn(); },
		ThrowsMessage<std::logic_error>(StrEq("Invalid ADC vRef voltage: 0.500000")));
}

TEST_F(SettingDiagnostics, FuelStrategy) {
	engineConfiguration->fuelAlgorithm = static_cast<engine_load_mode_e>(182);
	EXPECT_THAT([&] { getAirmassModel(engineConfiguration->fuelAlgorithm); },
		ThrowsMessage<std::logic_error>(StrEq("Invalid Fuel strategy: 182")));
}

TEST_F(SettingDiagnostics, MissingFuelStrategyModel) {
	// The host-only model selection lets us exercise the null-model fallback.
	engineConfiguration->fuelAlgorithm = engine_load_mode_e::UNSUPPORTED_ENUM_VALUE;
	engine->mockAirmassModel = nullptr;
	EXPECT_THAT([&] { getInjectionMass(1200); },
		ThrowsMessage<std::logic_error>(StrEq("Invalid Fuel strategy")));
}

TEST_F(SettingDiagnostics, InjectionModePulseCount) {
	EXPECT_THAT([&] { getNumberOfInjections(static_cast<injection_mode_e>(182)); },
		ThrowsMessage<std::logic_error>(StrEq("Invalid Injection Mode: 182")));
}

TEST_F(SettingDiagnostics, InjectionModeDuration) {
	engine->rpmCalculator.setRpmValue(1200);
	engineConfiguration->injectionMode = static_cast<injection_mode_e>(182);
	EXPECT_THAT([&] { getInjectionModeDurationMultiplier(); },
		ThrowsMessage<std::logic_error>(StrEq("Invalid Injection Mode: 182")));
}

TEST_F(SettingDiagnostics, IgnitionModeSparkCount) {
	EXPECT_THAT([&] { getNumberOfSparks(static_cast<ignition_mode_e>(182)); },
		ThrowsMessage<std::logic_error>(StrEq("Invalid Ignition Mode: 182")));
}

TEST_F(SettingDiagnostics, IgnitionModeSchedule) {
	prepareIgnition();
	engineConfiguration->ignitionMode = static_cast<ignition_mode_e>(182);
	EXPECT_THAT([&] { initializeIgnitionActions(); },
		ThrowsMessage<std::logic_error>(StrEq("Invalid Ignition Mode: 182")));
}

TEST_F(SettingDiagnostics, ZeroCylinders) {
	prepareIgnition();
	engineConfiguration->cylindersCount = 0;
	EXPECT_THAT([&] { initializeIgnitionActions(); },
		ThrowsMessage<std::logic_error>(StrEq("Number of cylinders must be greater than zero")));
}

TEST_F(SettingDiagnostics, TooManyCylinders) {
	engineConfiguration->cylindersCount = MAX_CYLINDER_COUNT + 1;
	EXPECT_THAT([&] { validateConfigOnStartUpOrBurn(); },
		ThrowsMessage<std::logic_error>(StrEq("Invalid Number of cylinders: " + std::to_string(MAX_CYLINDER_COUNT + 1))));
}

TEST_F(SettingDiagnostics, StepperSteppingMode) {
	engineConfiguration->stepperNumMicroSteps = static_cast<stepper_num_micro_steps_e>(16);
	DualHBridgeStepper dut;
	EXPECT_THAT([&] { dut.initialize(nullptr, nullptr, 1); },
		ThrowsMessage<std::logic_error>(StrEq("Invalid Stepping Mode")));
}

TEST_F(SettingDiagnostics, CanDashType) {
	engineConfiguration->canNbcType = static_cast<can_nbc_e>(182);
	EXPECT_THAT([&] { updateDash(CanCycle(0)); },
		ThrowsMessage<std::logic_error>(StrEq("Unsupported CAN dash type: 182")));
}

TEST_F(SettingDiagnostics, CanVssType) {
	engineConfiguration->enableCanVss = true;
	engineConfiguration->canVssNbcType = static_cast<can_vss_nbc_e>(182);
	EXPECT_THAT([&] { initCanVssSupport(); },
		ThrowsMessage<std::logic_error>(StrEq("Invalid CAN VSS type: 182")));
}

TEST_F(SettingDiagnostics, CanVssTypeWithoutRpmSupport) {
	engineConfiguration->enableCanVss = true;
	engineConfiguration->canInputBCM = true;
	engineConfiguration->canVssNbcType = BMW_e46;
	EXPECT_THAT([&] { initCanVssSupport(); },
		ThrowsMessage<std::logic_error>(StrEq("CAN VSS type does not support RPM input: 0")));
}

TEST_F(SettingDiagnostics, CanVssTypeWithoutSecondSpeedSupport) {
	engineConfiguration->enableCanVss = true;
	engineConfiguration->canInputBCM = false;
	engineConfiguration->canVssNbcType = NISSAN_350;
	initCanVssSupport();

	// Reconfigure after init to reach the second-speed decoder's defensive branch.
	engineConfiguration->canVssNbcType = BMW_e46;
	CANRxFrame frame = {};
	frame.SID = 645;
	EXPECT_THAT([&] { processCanRxVss(frame, 0); },
		ThrowsMessage<std::logic_error>(StrEq("CAN VSS type does not support second speed input: 0")));
}
