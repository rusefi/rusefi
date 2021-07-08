#include "engine_test_helper.h"
#include "injector_model.h"
#include "mocks.h"
#include "mock/mock_sensor.h"

#include "gtest/gtest.h"

using ::testing::_;
using ::testing::StrictMock;

class MockInjectorModel : public InjectorModelBase {
public:
	MOCK_METHOD(floatms_t, getDeadtime, (), (const, override));
	MOCK_METHOD(float, getInjectorMassFlowRate, (), (const, override));
	MOCK_METHOD(float, getInjectorFlowRatio, (), (const, override));
	MOCK_METHOD(expected<float>, getAbsoluteRailPressure, (), (const, override));
	MOCK_METHOD(float, correctShortPulse, (float baseDuration), (const, override));
};

TEST(InjectorModel, Prepare) {
	StrictMock<MockInjectorModel> dut;

	EXPECT_CALL(dut, getDeadtime());
	EXPECT_CALL(dut, getInjectorMassFlowRate());

	dut.prepare();
}

TEST(InjectorModel, getInjectionDuration) {
	StrictMock<MockInjectorModel> dut;

	EXPECT_CALL(dut, getDeadtime())
		.WillOnce(Return(2.0f));
	EXPECT_CALL(dut, getInjectorMassFlowRate())
		.WillOnce(Return(4.8f)); // 400cc/min
	EXPECT_CALL(dut, correctShortPulse(_))
		.Times(2)
		.WillRepeatedly([](float b) { return b; });

	dut.prepare();

	EXPECT_NEAR(dut.getInjectionDuration(0.01f), 10 / 4.8f + 2.0f, EPS4D);
	EXPECT_NEAR(dut.getInjectionDuration(0.02f), 20 / 4.8f + 2.0f, EPS4D);
}

TEST(InjectorModel, getInjectionDurationNonlinear) {
	StrictMock<MockInjectorModel> dut;

	EXPECT_CALL(dut, getDeadtime())
		.WillOnce(Return(2.0f));
	EXPECT_CALL(dut, getInjectorMassFlowRate())
		.WillOnce(Return(4.8f)); // 400cc/min

	// Dummy nonlinearity correction: just doubles the pulse
	EXPECT_CALL(dut, correctShortPulse(_))
		.Times(2)
		.WillRepeatedly([](float b) { return 2 * b; });

	dut.prepare();

	EXPECT_NEAR(dut.getInjectionDuration(0.01f), 2 * 10 / 4.8f + 2.0f, EPS4D);
	EXPECT_NEAR(dut.getInjectionDuration(0.02f), 2 * 20 / 4.8f + 2.0f, EPS4D);
}

TEST(InjectorModel, nonlinearPolynomial) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	InjectorModel dut;
	INJECT_ENGINE_REFERENCE(&dut);

	CONFIG(applyNonlinearBelowPulse) = 10;

	for (int i = 0; i < 8; i++) {
		CONFIG(injectorCorrectionPolynomial)[i] = i + 1;
	}

	// expect return of the original value, plus polynomial f(x)
	EXPECT_NEAR(dut.correctInjectionPolynomial(-3), -3 + -13532, EPS4D);
	EXPECT_NEAR(dut.correctInjectionPolynomial(-2), -2 +   -711, EPS4D);
	EXPECT_NEAR(dut.correctInjectionPolynomial(-1), -1 +     -4, EPS4D);
	EXPECT_NEAR(dut.correctInjectionPolynomial(0),   0 +      1, EPS4D);
	EXPECT_NEAR(dut.correctInjectionPolynomial(1),   1 +     36, EPS4D);
	EXPECT_NEAR(dut.correctInjectionPolynomial(2),   2 +   1793, EPS4D);
	EXPECT_NEAR(dut.correctInjectionPolynomial(3),   3 +  24604, EPS4D);
}

TEST(InjectorModel, Deadtime) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	// Some test data in the injector correction table
	for (size_t i = 0; i < efi::size(engineConfiguration->injector.battLagCorr); i++) {
		CONFIG(injector.battLagCorr)[i] = 2 * i;
		CONFIG(injector.battLagCorrBins)[i] = i;
	}

	InjectorModel dut;
	INJECT_ENGINE_REFERENCE(&dut);

	Sensor::setMockValue(SensorType::BatteryVoltage, 3);
	EXPECT_EQ(dut.getDeadtime(), 6);

	Sensor::setMockValue(SensorType::BatteryVoltage, 7);
	EXPECT_EQ(dut.getDeadtime(), 14);
}

struct TesterGetFlowRate : public InjectorModel {
	MOCK_METHOD(float, getInjectorFlowRatio, (), (const, override));
};

struct TesterGetRailPressure : public InjectorModel {
	MOCK_METHOD(expected<float>, getAbsoluteRailPressure, (), (const, override));
};

class FlowRateFixture : public ::testing::TestWithParam<float> {
};

INSTANTIATE_TEST_SUITE_P(
	InjectorModel,
	FlowRateFixture,
	::testing::Values(0.1f, 0.5f, 1.0f, 2.0f, 10.0f)
);

TEST_P(FlowRateFixture, FlowRateRatio) {
	float flowRatio = GetParam();

	StrictMock<TesterGetFlowRate> dut;
	EXPECT_CALL(dut, getInjectorFlowRatio()).WillOnce(Return(flowRatio));

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	INJECT_ENGINE_REFERENCE(&dut);
	engineConfiguration->injector.flow = 500;

	// 500 cc/min = 6g/s
	float expectedFlow = flowRatio * 6.0f;

	// Check that flow is adjusted correctly
	EXPECT_FLOAT_EQ(expectedFlow, dut.getInjectorMassFlowRate());
}

TEST_P(FlowRateFixture, PressureRatio) {
	float pressureRatio = GetParam();
	// Flow ratio should be the sqrt of pressure ratio
	float expectedFlowRatio = sqrtf(pressureRatio);
	float fakeMap = 35.0f;

	StrictMock<TesterGetRailPressure> dut;
	EXPECT_CALL(dut, getAbsoluteRailPressure()).WillOnce(Return(400 * pressureRatio + fakeMap));

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	INJECT_ENGINE_REFERENCE(&dut);

	// Use injector compensation
	engineConfiguration->injectorCompensationMode = ICM_SensedRailPressure;

	// Reference pressure is 400kPa 
	engineConfiguration->fuelReferencePressure = 400.0f;

	// MAP sensor always reads 35 kpa
	Sensor::setMockValue(SensorType::Map, fakeMap);

	// Should return the expected ratio
	EXPECT_FLOAT_EQ(expectedFlowRatio, dut.getInjectorFlowRatio());
}

TEST(InjectorModel, NegativePressureDelta) {
	StrictMock<TesterGetRailPressure> dut;

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	INJECT_ENGINE_REFERENCE(&dut);

	// Use injector compensation
	engineConfiguration->injectorCompensationMode = ICM_SensedRailPressure;

	// Reference pressure is 400kPa 
	engineConfiguration->fuelReferencePressure = 400.0f;

	EXPECT_CALL(dut, getAbsoluteRailPressure()).WillOnce(Return(50));
	// MAP sensor reads more pressure than fuel rail
	Sensor::setMockValue(SensorType::Map, 100);

	// Flow ratio defaults to 1.0 in this case
	EXPECT_FLOAT_EQ(1.0f, dut.getInjectorFlowRatio());
}

TEST(InjectorModel, VariableInjectorFlowModeNone) {
	StrictMock<TesterGetRailPressure> dut;

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	INJECT_ENGINE_REFERENCE(&dut);

	engineConfiguration->injectorCompensationMode = ICM_None;

	// This shoudn't call getAbsoluteRailPressure, it should just return 1.0
	EXPECT_FLOAT_EQ(1, dut.getInjectorFlowRatio());
}

TEST(InjectorModel, RailPressureFixed) {
	InjectorModel dut;

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	INJECT_ENGINE_REFERENCE(&dut);

	// Reference pressure is 350kpa
	engineConfiguration->fuelReferencePressure = 350;
	engineConfiguration->injectorCompensationMode = ICM_FixedRailPressure;

	// Should be reference pressure + 1 atm
	EXPECT_FLOAT_EQ(101.325f + 350.0f, dut.getAbsoluteRailPressure().value_or(0));
}

TEST(InjectorModel, RailPressureSensed) {
	InjectorModel dut;

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	INJECT_ENGINE_REFERENCE(&dut);

	// Reference pressure is 350kpa
	engineConfiguration->injectorCompensationMode = ICM_SensedRailPressure;

	// Should just return rail sensor value
	Sensor::setMockValue(SensorType::FuelPressureInjector, 100);
	EXPECT_FLOAT_EQ(100, dut.getAbsoluteRailPressure().value_or(-1));
	Sensor::setMockValue(SensorType::FuelPressureInjector, 200);
	EXPECT_FLOAT_EQ(200, dut.getAbsoluteRailPressure().value_or(-1));
	Sensor::setMockValue(SensorType::FuelPressureInjector, 300);
	EXPECT_FLOAT_EQ(300, dut.getAbsoluteRailPressure().value_or(-1));
}

TEST(InjectorModel, FailedPressureSensor) {
	InjectorModel dut;

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	INJECT_ENGINE_REFERENCE(&dut);

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
	InjectorModel dut;

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	INJECT_ENGINE_REFERENCE(&dut);

	// Reference pressure is 350kpa
	engineConfiguration->injectorCompensationMode = ICM_SensedRailPressure;

	// Sensor is missing!
	Sensor::resetMockValue(SensorType::FuelPressureInjector);

	// Missing sensor should trigger a fatal as it's a misconfiguration
	EXPECT_FATAL_ERROR(dut.getInjectorFlowRatio());
}
