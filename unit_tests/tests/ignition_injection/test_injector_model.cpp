#include "pch.h"
#include "injector_model.h"

using ::testing::_;
using ::testing::StrictMock;

class MockInjectorModel : public InjectorModelBase {
public:
	MOCK_METHOD(floatms_t, getDeadtime, (), (const, override));
	MOCK_METHOD(float, getBaseFlowRate, (), (const, override));
	MOCK_METHOD(float, getInjectorFlowRatio, (), (override));
	MOCK_METHOD(expected<float>, getFuelDifferentialPressure, (), (const, override));
	MOCK_METHOD(float, getSmallPulseFlowRate, (), (const, override));
	MOCK_METHOD(float, getSmallPulseBreakPoint, (), (const, override));
	MOCK_METHOD(InjectorNonlinearMode, getNonlinearMode, (), (const, override));
};

TEST(InjectorModel, Prepare) {
	StrictMock<MockInjectorModel> dut;

	EXPECT_CALL(dut, getDeadtime());
	EXPECT_CALL(dut, getBaseFlowRate());
	EXPECT_CALL(dut, getNonlinearMode());
	EXPECT_CALL(dut, getInjectorFlowRatio());

	dut.prepare();
}

TEST(InjectorModel, getInjectionDuration) {
	StrictMock<MockInjectorModel> dut;

	EXPECT_CALL(dut, getDeadtime())
		.WillOnce(Return(2.0f));
	EXPECT_CALL(dut, getInjectorFlowRatio())
		.WillOnce(Return(1.0f));
	EXPECT_CALL(dut, getBaseFlowRate())
		.WillOnce(Return(4.8f)); // 400cc/min
	EXPECT_CALL(dut, getNonlinearMode())
		.WillRepeatedly(Return(INJ_None));

	dut.prepare();

	EXPECT_NEAR(dut.getInjectionDuration(0.01f), 10 / 4.8f + 2.0f, EPS4D);
	EXPECT_NEAR(dut.getInjectionDuration(0.02f), 20 / 4.8f + 2.0f, EPS4D);
}

TEST(InjectorModel, getInjectionDurationWithFlowRatio) {
	StrictMock<MockInjectorModel> dut;

	EXPECT_CALL(dut, getDeadtime())
		.WillOnce(Return(2.0f));
	EXPECT_CALL(dut, getInjectorFlowRatio())
		.WillOnce(Return(1.1f));
	EXPECT_CALL(dut, getBaseFlowRate())
		.WillOnce(Return(4.8f)); // 400cc/min
	EXPECT_CALL(dut, getNonlinearMode())
		.WillRepeatedly(Return(INJ_None));

	dut.prepare();

	EXPECT_NEAR(dut.getInjectionDuration(0.01f), 10 / (4.8f * 1.1f) + 2.0f, EPS4D);
	EXPECT_NEAR(dut.getInjectionDuration(0.02f), 20 / (4.8f * 1.1f) + 2.0f, EPS4D);
}

TEST(InjectorModel, nonLinearFordMode) {
	StrictMock<MockInjectorModel> dut;

	EXPECT_CALL(dut, getDeadtime())
		.WillOnce(Return(0));
	EXPECT_CALL(dut, getInjectorFlowRatio())
		.WillOnce(Return(1.0f));

	// 2005 F150 injectors
	EXPECT_CALL(dut, getBaseFlowRate())
		.WillRepeatedly(Return(2.979f));
	EXPECT_CALL(dut, getSmallPulseFlowRate())
		.WillRepeatedly(Return(3.562f));
	EXPECT_CALL(dut, getSmallPulseBreakPoint())
		.WillRepeatedly(Return(0.00627f));
	EXPECT_CALL(dut, getNonlinearMode())
		.WillRepeatedly(Return(INJ_FordModel));

	dut.prepare();

	EXPECT_NEAR(dut.getBaseDurationImpl(0.000f), 0.344f, 1e-3);
	EXPECT_NEAR(dut.getBaseDurationImpl(0.001f), 0.625f, 1e-3);
	EXPECT_NEAR(dut.getBaseDurationImpl(0.002f), 0.906f, 1e-3);
	EXPECT_NEAR(dut.getBaseDurationImpl(0.003f), 1.187f, 1e-3);
	EXPECT_NEAR(dut.getBaseDurationImpl(0.004f), 1.467f, 1e-3);
	EXPECT_NEAR(dut.getBaseDurationImpl(0.005f), 1.748f, 1e-3);
	EXPECT_NEAR(dut.getBaseDurationImpl(0.006f), 2.029f, 1e-3);
	EXPECT_NEAR(dut.getBaseDurationImpl(0.007f), 2.350f, 1e-3);
	EXPECT_NEAR(dut.getBaseDurationImpl(0.008f), 2.685f, 1e-3);
	EXPECT_NEAR(dut.getBaseDurationImpl(0.009f), 3.021f, 1e-3);
	EXPECT_NEAR(dut.getBaseDurationImpl(0.010f), 3.357f, 1e-3);
	EXPECT_NEAR(dut.getBaseDurationImpl(0.011f), 3.693f, 1e-3);
	EXPECT_NEAR(dut.getBaseDurationImpl(0.012f), 4.028f, 1e-3);
	EXPECT_NEAR(dut.getBaseDurationImpl(0.013f), 4.364f, 1e-3);
	EXPECT_NEAR(dut.getBaseDurationImpl(0.014f), 4.700f, 1e-3);
	EXPECT_NEAR(dut.getBaseDurationImpl(0.015f), 5.035f, 1e-3);
	EXPECT_NEAR(dut.getBaseDurationImpl(0.016f), 5.371f, 1e-3);
	EXPECT_NEAR(dut.getBaseDurationImpl(0.017f), 5.707f, 1e-3);
	EXPECT_NEAR(dut.getBaseDurationImpl(0.018f), 6.042f, 1e-3);
	EXPECT_NEAR(dut.getBaseDurationImpl(0.019f), 6.378f, 1e-3);
	EXPECT_NEAR(dut.getBaseDurationImpl(0.020f), 6.714f, 1e-3);
}

TEST(InjectorModel, nonlinearPolynomial) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	InjectorModelPrimary dut;

	engineConfiguration->applyNonlinearBelowPulse = 10;

	for (int i = 0; i < 8; i++) {
		engineConfiguration->injectorCorrectionPolynomial[i] = i + 1;
	}

	// expect return of the original value, plus polynomial f(x)
	EXPECT_NEAR(dut.correctInjectionPolynomial(-3), -3 + -13532, EPS4D);
	EXPECT_NEAR(dut.correctInjectionPolynomial(-2), -2 +   -711, EPS4D);
	EXPECT_NEAR(dut.correctInjectionPolynomial(-1), -1 +     -4, EPS4D);
	EXPECT_NEAR(dut.correctInjectionPolynomial(0),   0 +      1, EPS4D);
	EXPECT_NEAR(dut.correctInjectionPolynomial(1),   1 +     36, EPS4D);
	EXPECT_NEAR(dut.correctInjectionPolynomial(2),   2 +   1793, EPS4D);
	EXPECT_NEAR(dut.correctInjectionPolynomial(3),   3 +  24604, EPS4D);

	// Check that the disable threshold works
	EXPECT_NE(dut.correctInjectionPolynomial(9.9f), 9.9f);
	EXPECT_EQ(dut.correctInjectionPolynomial(10.1f), 10.1f);
}

TEST(InjectorModel, Deadtime) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// Some test data in the injector correction table
	for (size_t i = 0; i < efi::size(engineConfiguration->injector.battLagCorr); i++) {
		engineConfiguration->injector.battLagCorr[i] = 2 * i;
		engineConfiguration->injector.battLagCorrBins[i] = i;
	}

	InjectorModelPrimary dut;

	Sensor::setMockValue(SensorType::BatteryVoltage, 3);
	EXPECT_EQ(dut.getDeadtime(), 6);

	Sensor::setMockValue(SensorType::BatteryVoltage, 7);
	EXPECT_EQ(dut.getDeadtime(), 14);
}

struct TesterGetFlowRate : public InjectorModelPrimary {
	MOCK_METHOD(float, getInjectorFlowRatio, (), (override));
};

struct TesterGetRailPressure : public InjectorModelPrimary {
	MOCK_METHOD(expected<float>, getFuelDifferentialPressure, (), (const, override));
};

class FlowRateFixture : public ::testing::TestWithParam<float> {
};

INSTANTIATE_TEST_SUITE_P(
	InjectorModel,
	FlowRateFixture,
	::testing::Values(0.1f, 0.5f, 1.0f, 2.0f, 10.0f)
);

TEST_P(FlowRateFixture, PressureRatio) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	float pressureRatio = GetParam();
	// Flow ratio should be the sqrt of pressure ratio
	float expectedFlowRatio = sqrtf(pressureRatio);

	StrictMock<TesterGetRailPressure> dut;
	EXPECT_CALL(dut, getFuelDifferentialPressure()).WillOnce(Return(400 * pressureRatio));

	// Use injector compensation
	engineConfiguration->injectorCompensationMode = ICM_SensedRailPressure;

	// Reference pressure is 400kPa
	engineConfiguration->fuelReferencePressure = 400.0f;

	// Should return the expected ratio
	EXPECT_FLOAT_EQ(expectedFlowRatio, dut.getInjectorFlowRatio());
}

TEST(InjectorModel, NegativePressureDelta) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	StrictMock<TesterGetRailPressure> dut;

	// Use injector compensation
	engineConfiguration->injectorCompensationMode = ICM_SensedRailPressure;

	// Reference pressure is 400kPa
	engineConfiguration->fuelReferencePressure = 400.0f;

	EXPECT_CALL(dut, getFuelDifferentialPressure()).WillOnce(Return(-50));

	// Flow ratio defaults to 1.0 in this case
	EXPECT_FLOAT_EQ(1.0f, dut.getInjectorFlowRatio());
}

TEST(InjectorModel, VariableInjectorFlowModeNone) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	StrictMock<TesterGetRailPressure> dut;

	engineConfiguration->injectorCompensationMode = ICM_None;

	// This shoudn't call getFuelDifferentialPressure, it should just return 1.0
	EXPECT_FLOAT_EQ(1, dut.getInjectorFlowRatio());
}

TEST(InjectorModel, RailPressureFixed) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	InjectorModelPrimary dut;

	// Reference pressure is 350kpa
	engineConfiguration->fuelReferencePressure = 350;
	engineConfiguration->injectorCompensationMode = ICM_FixedRailPressure;

	// MAP is 75kPa
	Sensor::setMockValue(SensorType::Map, 75);

	// Baro is 100kpa
	Sensor::setMockValue(SensorType::BarometricPressure, 100);

	// Should be reference pressure + 1 atm - 75kpa -> 375kPa
	EXPECT_FLOAT_EQ(375.0f, dut.getFuelDifferentialPressure().value_or(0));
}

TEST(InjectorModel, RailPressureSensedAbsolute) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	InjectorModelPrimary dut;

	// Reference pressure is 350kpa
	engineConfiguration->injectorCompensationMode = ICM_SensedRailPressure;
	engineConfiguration->fuelPressureSensorMode = FPM_Absolute;

	Sensor::setMockValue(SensorType::Map, 50);

	// Should just return rail sensor value - MAP
	Sensor::setMockValue(SensorType::FuelPressureInjector, 100);
	EXPECT_FLOAT_EQ(100 - 50, dut.getFuelDifferentialPressure().value_or(-1));
	Sensor::setMockValue(SensorType::FuelPressureInjector, 200);
	EXPECT_FLOAT_EQ(200 - 50, dut.getFuelDifferentialPressure().value_or(-1));
	Sensor::setMockValue(SensorType::FuelPressureInjector, 300);
	EXPECT_FLOAT_EQ(300 - 50, dut.getFuelDifferentialPressure().value_or(-1));
}

TEST(InjectorModel, RailPressureSensedGauge) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	InjectorModelPrimary dut;

	// Reference pressure is 350kpa
	engineConfiguration->injectorCompensationMode = ICM_SensedRailPressure;
	engineConfiguration->fuelPressureSensorMode = FPM_Gauge;

	Sensor::setMockValue(SensorType::BarometricPressure, 110);
	Sensor::setMockValue(SensorType::Map, 50);

	// Should just return rail sensor value + baro - MAP
	Sensor::setMockValue(SensorType::FuelPressureInjector, 100);
	EXPECT_FLOAT_EQ(100 + 110 - 50, dut.getFuelDifferentialPressure().value_or(-1));
	Sensor::setMockValue(SensorType::FuelPressureInjector, 200);
	EXPECT_FLOAT_EQ(200 + 110 - 50, dut.getFuelDifferentialPressure().value_or(-1));
	Sensor::setMockValue(SensorType::FuelPressureInjector, 300);
	EXPECT_FLOAT_EQ(300 + 110 - 50, dut.getFuelDifferentialPressure().value_or(-1));
}

TEST(InjectorModel, RailPressureSensedDifferential) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	InjectorModelPrimary dut;

	// Reference pressure is 350kpa
	engineConfiguration->injectorCompensationMode = ICM_SensedRailPressure;
	engineConfiguration->fuelPressureSensorMode = FPM_Differential;

	// Should just return rail sensor value
	Sensor::setMockValue(SensorType::FuelPressureInjector, 100);
	EXPECT_FLOAT_EQ(100, dut.getFuelDifferentialPressure().value_or(-1));
	Sensor::setMockValue(SensorType::FuelPressureInjector, 200);
	EXPECT_FLOAT_EQ(200, dut.getFuelDifferentialPressure().value_or(-1));
	Sensor::setMockValue(SensorType::FuelPressureInjector, 300);
	EXPECT_FLOAT_EQ(300, dut.getFuelDifferentialPressure().value_or(-1));
}

TEST(InjectorModel, FailedPressureSensor) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	InjectorModelPrimary dut;

	// Reference pressure is 350kpa
	engineConfiguration->injectorCompensationMode = ICM_SensedRailPressure;

	// Sensor is broken!
	// We have to register a broken sensor because the fuel pressure comp system
	// has different logic for missing sensor
	MockSensor ms(SensorType::FuelPressureInjector);
	ms.invalidate();
	ms.Register();

	EXPECT_EQ(1.0f, dut.getInjectorFlowRatio());
}

TEST(InjectorModel, MissingPressureSensor) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	InjectorModelPrimary dut;

	// Reference pressure is 350kpa
	engineConfiguration->injectorCompensationMode = ICM_SensedRailPressure;

	// Sensor is missing!
	Sensor::resetMockValue(SensorType::FuelPressureInjector);

	// no warning before
	ASSERT_EQ(0, eth.recentWarnings()->getCount());

	// Missing sensor should return 1.0 flow ratio and log a warning
	EXPECT_EQ(1.0f, dut.getInjectorFlowRatio());

	EXPECT_EQ(1, eth.recentWarnings()->getCount());
}
