#include "pch.h"
#include "fuel_math.h"
#include "alphan_airmass.h"
#include "maf_airmass.h"

using ::testing::StrictMock;
using ::testing::FloatNear;
using ::testing::InSequence;
using ::testing::_;

TEST(FuelMath, getStandardAirCharge) {
	EngineTestHelper eth(TEST_ENGINE);

	// Miata 1839cc 4cyl
	engineConfiguration->specs.displacement = 1.839f;
	engineConfiguration->specs.cylindersCount = 4;

	EXPECT_FLOAT_EQ(0.5535934f, getStandardAirCharge());

	// LS 5.3 liter v8
	engineConfiguration->specs.displacement = 5.327f;
	engineConfiguration->specs.cylindersCount = 8;

	EXPECT_FLOAT_EQ(0.80179232f, getStandardAirCharge());

	// Chainsaw - single cylinder 32cc
	engineConfiguration->specs.displacement = 0.032f;
	engineConfiguration->specs.cylindersCount = 1;
	EXPECT_FLOAT_EQ(0.038531788f, getStandardAirCharge());

	// Leopard 1 47.666 liter v12
	engineConfiguration->specs.displacement = 47.666f;
	engineConfiguration->specs.cylindersCount = 12;

	EXPECT_FLOAT_EQ(4.782959f, getStandardAirCharge());
}

TEST(AirmassModes, AlphaNNormal) {
	EngineTestHelper eth(TEST_ENGINE);
	// 4 cylinder 4 liter = easy math
	engineConfiguration->specs.displacement = 4.0f;
	engineConfiguration->specs.cylindersCount = 4;

	StrictMock<MockVp3d> veTable;

	EXPECT_CALL(veTable, getValue(1200, FloatNear(0.71f, EPS4D)))
		.WillOnce(Return(35.0f));

	AlphaNAirmass dut(veTable);

	Sensor::setMockValue(SensorType::Tps1, 0.71f);

	// Mass of 1 liter of air * VE
	float expectedAirmass = 1.2047f * 0.35f;

	auto result = dut.getAirmass(1200);
	EXPECT_NEAR(result.CylinderAirmass, expectedAirmass, EPS4D);
	EXPECT_NEAR(result.EngineLoadPercent, 0.71f, EPS4D);
}

TEST(AirmassModes, AlphaNFailedTps) {
	EngineTestHelper eth(TEST_ENGINE);

	// Shouldn't get called
	StrictMock<MockVp3d> veTable;

	AlphaNAirmass dut(veTable);

	// explicitly reset the sensor
	Sensor::resetMockValue(SensorType::Tps1);
	// Ensure that it's actually failed
	ASSERT_FALSE(Sensor::get(SensorType::Tps1).Valid);

	auto result = dut.getAirmass(1200);
	EXPECT_EQ(result.CylinderAirmass, 0);
}

TEST(AirmassModes, MafNormal) {
	EngineTestHelper eth(FORD_ASPIRE_1996);
	engineConfiguration->fuelAlgorithm = LM_REAL_MAF;
	engineConfiguration->injector.flow = 200;

	MockVp3d veTable;
	// Ensure that the correct cell is read from the VE table
	EXPECT_CALL(veTable, getValue(6000, FloatNear(70.9814f, EPS4D)))
		.WillOnce(Return(75.0f));

	MafAirmass dut(veTable);

	auto airmass = dut.getAirmassImpl(200, 6000);

	// Check results
	EXPECT_NEAR(0.277777f * 0.75f, airmass.CylinderAirmass, EPS4D);
	EXPECT_NEAR(70.9814f, airmass.EngineLoadPercent, EPS4D);
}

TEST(AirmassModes, VeOverride) {
	StrictMock<MockVp3d> veTable;

	{
		InSequence is;

		// Default
		EXPECT_CALL(veTable, getValue(_, 10.0f)).WillOnce(Return(0));
		// TPS
		EXPECT_CALL(veTable, getValue(_, 30.0f)).WillOnce(Return(0));
	}

	struct DummyAirmassModel : public AirmassVeModelBase {
		DummyAirmassModel(const ValueProvider3D& veTable) : AirmassVeModelBase(veTable) {}

		AirmassResult getAirmass(int rpm) override {
			// Default load value 10, will be overriden
			getVe(rpm, 10.0f);

			return {};
		}
	};

	EngineTestHelper eth(TEST_ENGINE);
	DummyAirmassModel dut(veTable);

	// Use default mode - will call with 10
	dut.getAirmass(0);
	EXPECT_FLOAT_EQ(engine->engineState.currentVeLoad, 10.0f);

	// Override to TPS
	engineConfiguration->veOverrideMode = VE_TPS;
	Sensor::setMockValue(SensorType::Tps1, 30.0f);
	dut.getAirmass(0);
	EXPECT_FLOAT_EQ(engine->engineState.currentVeLoad, 30.0f);
}

void setInjectionMode(int value);

TEST(FuelMath, testDifferentInjectionModes) {
	EngineTestHelper eth(TEST_ENGINE);
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);

	EXPECT_CALL(eth.mockAirmass, getAirmass(_))
		.WillRepeatedly(Return(AirmassResult{1.3440001f, 50.0f}));

	setInjectionMode((int)IM_BATCH);
	engine->periodicFastCallback();
	EXPECT_FLOAT_EQ( 20,  engine->injectionDuration) << "injection while batch";

	setInjectionMode((int)IM_SIMULTANEOUS);
	engine->periodicFastCallback();
	EXPECT_FLOAT_EQ( 10,  engine->injectionDuration) << "injection while simultaneous";

	setInjectionMode((int)IM_SEQUENTIAL);
	engine->periodicFastCallback();
	EXPECT_FLOAT_EQ( 40,  engine->injectionDuration) << "injection while IM_SEQUENTIAL";

	setInjectionMode((int)IM_SINGLE_POINT);
	engine->periodicFastCallback();
	EXPECT_FLOAT_EQ( 40,  engine->injectionDuration) << "injection while IM_SINGLE_POINT";
	EXPECT_EQ( 0, eth.recentWarnings()->getCount()) << "warningCounter#testDifferentInjectionModes";
}

TEST(FuelMath, deadtime) {
	EngineTestHelper eth(TEST_ENGINE);

	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);

	EXPECT_CALL(eth.mockAirmass, getAirmass(_))
		.WillRepeatedly(Return(AirmassResult{1.3440001f, 50.0f}));

	// First test with no deadtime
	engine->periodicFastCallback();
	EXPECT_FLOAT_EQ( 20,  engine->injectionDuration);

	// Now add some deadtime
	setArrayValues(engineConfiguration->injector.battLagCorr, 2.0f);

	// Should have deadtime now!
	engine->periodicFastCallback();
	EXPECT_FLOAT_EQ( 20 + 2,  engine->injectionDuration);
}
