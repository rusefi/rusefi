#include "pch.h"

// sneaky...
#define protected public
#include "fuel_computer.h"

#include "stored_value_sensor.h"

using ::testing::FloatEq;

class MockFuelComputer : public FuelComputerBase {
public:
	MOCK_METHOD(float, getStoichiometricRatio, (), (const, override));
	MOCK_METHOD(float, getTargetLambda, (float rpm, float load), (const, override));
	MOCK_METHOD(float, getTargetLambdaLoadAxis, (float defaultLoad), (const, override));
};

TEST(FuelComputer, getCycleFuel) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	MockFuelComputer dut;

	EXPECT_CALL(dut, getTargetLambdaLoadAxis(FloatEq(0.8f)))
		.WillOnce(Return(0.8f));
	EXPECT_CALL(dut, getStoichiometricRatio())
		.WillOnce(Return(3.0f));
	EXPECT_CALL(dut, getTargetLambda(1000, FloatEq(0.8f)))
		.WillOnce(Return(5.0f));

	auto result = dut.getCycleFuel(7.0f, 1000, 0.8f);
	EXPECT_FLOAT_EQ(result, 7.0f / (5 * 3));
}

TEST(FuelComputer, FlexFuel) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	FuelComputer dut;

	// easier values for testing
	engineConfiguration->stoichRatioPrimary = 15;
	engineConfiguration->stoichRatioSecondary = 10;

	// No sensor -> returns primary
	Sensor::resetMockValue(SensorType::FuelEthanolPercent);
	EXPECT_FLOAT_EQ(15.0f, dut.getStoichiometricRatio());

	// E0 -> primary afr
	Sensor::setMockValue(SensorType::FuelEthanolPercent, 0);
	EXPECT_FLOAT_EQ(15.0f, dut.getStoichiometricRatio());

	// E50 -> half way between
	Sensor::setMockValue(SensorType::FuelEthanolPercent, 50);
	EXPECT_FLOAT_EQ(12.5f, dut.getStoichiometricRatio());

	// E100 -> secondary afr
	Sensor::setMockValue(SensorType::FuelEthanolPercent, 100);
	EXPECT_FLOAT_EQ(10.0f, dut.getStoichiometricRatio());

	// E(-10) -> clamp to primary
	Sensor::setMockValue(SensorType::FuelEthanolPercent, -10);
	EXPECT_FLOAT_EQ(15.0f, dut.getStoichiometricRatio());

	// E110 -> clamp to secondary
	Sensor::setMockValue(SensorType::FuelEthanolPercent, 110);
	EXPECT_FLOAT_EQ(10.0f, dut.getStoichiometricRatio());
}

namespace {

class InvalidEthanolSensor final : public Sensor {
public:
	InvalidEthanolSensor() : Sensor(SensorType::FuelEthanolPercent) {}
	~InvalidEthanolSensor() { unregister(); }

	SensorResult get() const override {
		SensorResult result = payload;
		result.Valid = false;
		return result;
	}

	void showInfo(const char*) const override {}
	float payload = 0;
};

} // namespace

TEST(FuelComputer, InvalidEthanolPayloadDoesNotControlFuelStoichiometry) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	Sensor::resetMockValue(SensorType::FuelEthanolPercent);
	InvalidEthanolSensor sensor;
	ASSERT_TRUE(sensor.Register());
	ASSERT_TRUE(Sensor::hasSensor(SensorType::FuelEthanolPercent));
	ASSERT_FALSE(Sensor::get(SensorType::FuelEthanolPercent));
	engineConfiguration->stoichRatioPrimary = 15;
	engineConfiguration->stoichRatioSecondary = 10;
	FuelComputer dut;

	// Invalid readings must use primary stoichiometry regardless of payload.
	sensor.payload = 0;
	EXPECT_FLOAT_EQ(15, dut.getStoichiometricRatio());
	sensor.payload = 100;
	EXPECT_FLOAT_EQ(15, dut.getStoichiometricRatio());
}

namespace {

// #7184: exercise target mixture adders through the configured fuel computer,
// including the shared blend interpolation and the targets used by fuel trim.
class TargetAfrBlend : public testing::Test {
protected:
	EngineTestHelper eth{engine_type_e::TEST_ENGINE};
	FuelComputer& fuel = engine->fuelComputer;

	void SetUp() override {
		engineConfiguration->stoichRatioPrimary = 15;
		engineConfiguration->stoichRatioSecondary = 10;
		engineConfiguration->afrOverrideMode = AFR_None;
		// Initialize every cell, including on boards with non-default table sizes.
		setTable(config->lambdaTable, 1);
		for (size_t i = 0; i < efi::size(config->lambdaLoadBins); i++) {
			config->lambdaLoadBins[i] = 20 + 20 * i;
		}
		for (size_t i = 0; i < efi::size(config->lambdaRpmBins); i++) {
			config->lambdaRpmBins[i] = 1000 + 1000 * i;
		}
		for (auto& blend : config->targetAfrBlends) {
			blend = {};
			for (size_t i = 0; i < efi::size(blend.rpmBins); i++) {
				blend.rpmBins[i] = 1000 + 1000 * i;
				blend.loadBins[i] = 20 + 20 * i;
			}
			for (size_t i = 0; i < efi::size(blend.blendBins); i++) {
				blend.blendBins[i] = 20 * i;
			}
			setArrayValues(blend.blendValues, 100);
		}
	}

	auto& configureBlend(size_t index, gppwm_channel_e input, float offset, float weight = 100) {
		auto& blend = config->targetAfrBlends[index];
		blend.blendParameter = input;
		setTable(blend.table, offset);
		setArrayValues(blend.blendValues, weight);
		return blend;
	}

	void expectTelemetry(size_t index, float input, float weight, float output, float load) {
		const auto& channels = engine->outputChannels;
		EXPECT_NEAR(input, channels.targetAfrBlendParameter[index], 0.01f);
		EXPECT_NEAR(weight, channels.targetAfrBlendBias[index], 0.01f);
		// Check representable outputs here: this telemetry has 0.1 resolution,
		// while the actual lambda adder table has 0.01 resolution.
		EXPECT_NEAR(output, channels.targetAfrBlendOutput[index], 0.01f);
		EXPECT_NEAR(load, channels.targetAfrBlendYAxis[index], 0.01f);
	}
};

TEST_F(TargetAfrBlend, DisabledTablesIgnoreValuesAndClearPreviousTelemetry) {
	for (size_t i = 0; i < efi::size(config->targetAfrBlends); i++) {
		configureBlend(i, GPPWM_Zero, -0.2f);
	}
	EXPECT_FLOAT_EQ(1, fuel.getTargetLambda(1500, 30));
	for (size_t i = 0; i < efi::size(config->targetAfrBlends); i++) {
		expectTelemetry(i, 0, 0, 0, 0);
	}

	Sensor::setMockValue(SensorType::Clt, 90);
	config->targetAfrBlends[0].blendParameter = GPPWM_Clt;
	EXPECT_NEAR(0.8f, fuel.getTargetLambda(1500, 30), 1e-6f);
	expectTelemetry(0, 90, 100, -0.2f, 30);

	config->targetAfrBlends[0].blendParameter = GPPWM_Zero;
	EXPECT_FLOAT_EQ(1, fuel.getTargetLambda(1500, 30));
	expectTelemetry(0, 0, 0, 0, 0);
}

TEST_F(TargetAfrBlend, CoolantWeightInterpolatesAndClamps) {
	auto& blend = configureBlend(0, GPPWM_Clt, -0.4f);
	for (size_t i = 0; i < efi::size(blend.blendBins); i++) {
		blend.blendBins[i] = 80 + 40 * i;
		blend.blendValues[i] = i == 0 ? 0 : 100;
	}
	struct Sample { float clt; float weight; float lambda; float offset; };
	for (auto sample : {Sample{-40, 0, 1, 0}, Sample{80, 0, 1, 0},
		Sample{90, 25, 0.9f, -0.1f}, Sample{100, 50, 0.8f, -0.2f},
		Sample{110, 75, 0.7f, -0.3f}, Sample{120, 100, 0.6f, -0.4f},
		Sample{400, 100, 0.6f, -0.4f}}) {
		SCOPED_TRACE(sample.clt);
		Sensor::setMockValue(SensorType::Clt, sample.clt);
		EXPECT_NEAR(sample.lambda, fuel.getTargetLambda(1500, 30), 1e-6f);
		expectTelemetry(0, sample.clt, sample.weight, sample.offset, 30);
	}
}

TEST_F(TargetAfrBlend, InterpolatesBothTableAxesBeforeApplyingWeight) {
	auto& blend = configureBlend(0, GPPWM_Clt, 0, 50);
	// Rows are load, columns are RPM. Unequal slopes expose swapped axes.
	blend.table[0][0] = -0.1f;
	blend.table[0][1] = -0.3f;
	blend.table[1][0] = -0.5f;
	blend.table[1][1] = -0.9f;
	EXPECT_NEAR(0.9f, fuel.getTargetLambda(1500, 20), 1e-6f);
	EXPECT_NEAR(0.85f, fuel.getTargetLambda(1000, 30), 1e-6f);
	// Midpoint of all four cells is -0.45 lambda, weighted by 50%.
	EXPECT_NEAR(0.775f, fuel.getTargetLambda(1500, 30), 1e-6f);
}

TEST_F(TargetAfrBlend, TableAxesClampToBoundaryCells) {
	auto& blend = configureBlend(0, GPPWM_Clt, 0);
	const auto last = efi::size(blend.rpmBins) - 1;
	blend.table[0][0] = 0.1f;
	blend.table[0][last] = 0.2f;
	blend.table[last][0] = 0.3f;
	blend.table[last][last] = 0.4f;
	EXPECT_NEAR(1.1f, fuel.getTargetLambda(0, 0), 1e-6f);
	EXPECT_NEAR(1.2f, fuel.getTargetLambda(20000, 0), 1e-6f);
	EXPECT_NEAR(1.3f, fuel.getTargetLambda(0, 1000), 1e-6f);
	EXPECT_NEAR(1.4f, fuel.getTargetLambda(20000, 1000), 1e-6f);
}

TEST_F(TargetAfrBlend, BothTablesAddSignedOffsetsWithIndependentInputsAndWeights) {
	configureBlend(0, GPPWM_Clt, 0.2f, 50);
	configureBlend(1, GPPWM_EthanolPercent, -0.4f, 75);
	Sensor::setMockValue(SensorType::Clt, 110);
	Sensor::setMockValue(SensorType::FuelEthanolPercent, 60);
	// Add lambda offsets: 1 + 0.1 - 0.3, without scaling the base target.
	EXPECT_NEAR(0.8f, fuel.getTargetLambda(2500, 55), 1e-6f);
	expectTelemetry(0, 110, 50, 0.1f, 55);
	expectTelemetry(1, 60, 75, -0.3f, 55);
}

TEST_F(TargetAfrBlend, PreservesHundredthLambdaCellsAndFractionalWeights) {
	configureBlend(0, GPPWM_Clt, -0.03f, 12.5f);
	// A 0.01-resolution cell and 0.5%-resolution weight must not be rounded
	// to the much coarser blend-output telemetry before computing fuel.
	EXPECT_NEAR(0.99625f, fuel.getTargetLambda(1500, 30), 1e-6f);
	EXPECT_NEAR(0.6f / (15 * 0.99625f), fuel.getCycleFuel(0.6f, 1500, 30), 1e-6f);
}

TEST_F(TargetAfrBlend, MissingOrFailedInputSkipsOnlyItsTableAndRecovers) {
	configureBlend(0, GPPWM_Clt, -0.2f);
	configureBlend(1, GPPWM_EthanolPercent, -0.1f);
	Sensor::resetMockValue(SensorType::FuelEthanolPercent);
	ASSERT_FALSE(Sensor::hasSensor(SensorType::FuelEthanolPercent));
	EXPECT_NEAR(0.8f, fuel.getTargetLambda(1500, 30), 1e-6f);
	expectTelemetry(1, 0, 0, 0, 0);

	Sensor::setMockValue(SensorType::FuelEthanolPercent, 20);
	EXPECT_NEAR(0.7f, fuel.getTargetLambda(1500, 30), 1e-6f);
	expectTelemetry(1, 20, 100, -0.1f, 30);

	Sensor::setInvalidMockValue(SensorType::FuelEthanolPercent);
	EXPECT_NEAR(0.8f, fuel.getTargetLambda(1500, 30), 1e-6f);
	expectTelemetry(1, 0, 0, 0, 0);

	Sensor::setMockValue(SensorType::FuelEthanolPercent, 60);
	EXPECT_NEAR(0.7f, fuel.getTargetLambda(1500, 30), 1e-6f);
	expectTelemetry(1, 60, 100, -0.1f, 30);
}

TEST_F(TargetAfrBlend, LoadOverrideUsesSensorAndFallsBackToZeroWhenInvalid) {
	auto& blend = configureBlend(0, GPPWM_Clt, -0.4f);
	blend.yAxisOverride = GPPWM_Map;
	setArrayValues(blend.table[0], -0.2f);
	Sensor::setMockValue(SensorType::Map, 30);
	EXPECT_NEAR(0.7f, fuel.getTargetLambda(1500, 100), 1e-6f);
	expectTelemetry(0, 70, 100, -0.3f, 30);

	Sensor::setInvalidMockValue(SensorType::Map);
	// Current override fallback is zero, clamped to the first table row.
	EXPECT_NEAR(0.8f, fuel.getTargetLambda(1500, 100), 1e-6f);
	expectTelemetry(0, 70, 100, -0.2f, 0);

	Sensor::setMockValue(SensorType::Map, 40);
	EXPECT_NEAR(0.6f, fuel.getTargetLambda(1500, 100), 1e-6f);
	expectTelemetry(0, 70, 100, -0.4f, 40);
}

TEST_F(TargetAfrBlend, FuelMassAndPublishedTargetsUseLambdaAddersWithFlexStoich) {
	configureBlend(0, GPPWM_Clt, -0.2f, 50);
	auto& blend = configureBlend(1, GPPWM_EthanolPercent, -0.2f);
	for (size_t i = 0; i < efi::size(blend.blendValues); i++) {
		blend.blendValues[i] = std::min(100.0f, 20.0f * i);
	}
	struct Sample { float ethanol; float lambda; float stoich; float afr; };
	for (auto sample : {Sample{0, 0.9f, 15, 13.5f}, Sample{50, 0.8f, 12.5f, 10},
		Sample{100, 0.7f, 10, 7}}) {
		SCOPED_TRACE(sample.ethanol);
		Sensor::setMockValue(SensorType::FuelEthanolPercent, sample.ethanol);
		EXPECT_NEAR(0.6f / sample.afr, fuel.getCycleFuel(0.6f, 1500, 30), 1e-6f);
		EXPECT_NEAR(sample.lambda, fuel.targetLambda, 0.001f);
		EXPECT_NEAR(sample.afr, fuel.targetAFR, 0.01f);
		EXPECT_NEAR(sample.stoich, fuel.stoichiometricRatio, 0.01f);
		EXPECT_FLOAT_EQ(30, fuel.afrTableYAxis);
	}
}

TEST_F(TargetAfrBlend, TargetLoadOverrideFeedsBaseAndDefaultBlendAxes) {
	engineConfiguration->afrOverrideMode = AFR_MAP;
	Sensor::setMockValue(SensorType::Map, 60);
	Sensor::setMockValue(SensorType::Tps1, 40);
	for (size_t row = 0; row < efi::size(config->lambdaTable); row++) {
		// Use exactly representable values: the base table has different
		// packing from the 0.01-resolution lambda adder tables.
		setArrayValues(config->lambdaTable[row], 1.0f - 3.0f * row / PACK_MULT_LAMBDA_CFG);
	}
	configureBlend(0, GPPWM_Clt, 0);
	configureBlend(1, GPPWM_Iat, 0).yAxisOverride = GPPWM_Tps;
	for (auto& blend : config->targetAfrBlends) {
		for (size_t row = 0; row < efi::size(blend.table); row++) {
			setArrayValues(blend.table[row], -0.01f * (row + 1));
		}
	}
	// MAP selects base row 2 and the first adder (-0.03). TPS independently
	// selects the second adder (-0.02), regardless of the caller's load of 20.
	const float expectedLambda = 1.0f - 6.0f / PACK_MULT_LAMBDA_CFG - 0.05f;
	EXPECT_NEAR(0.6f / (15 * expectedLambda), fuel.getCycleFuel(0.6f, 1500, 20), 1e-6f);
	EXPECT_NEAR(expectedLambda, fuel.targetLambda, 0.001f);
	EXPECT_NEAR(15 * expectedLambda, fuel.targetAFR, 0.01f);
	EXPECT_FLOAT_EQ(60, fuel.afrTableYAxis);
	EXPECT_FLOAT_EQ(60, engine->outputChannels.targetAfrBlendYAxis[0]);
	EXPECT_FLOAT_EQ(40, engine->outputChannels.targetAfrBlendYAxis[1]);
}

} // namespace
