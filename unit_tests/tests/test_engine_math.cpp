/*
 * @file	test_engine_math.c
 *
 * @date Nov 14, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "speed_density.h"
#include "maf.h"
#include "advance_map.h"

TEST(misc, testIgnitionPlanning) {
	printf("*************************************************** testIgnitionPlanning\r\n");
	EngineTestHelper eth(engine_type_e::FORD_ESCORT_GT);

	engine->periodicFastCallback();
	assertEqualsM("testIgnitionPlanning_AFR", 13.5, engine->fuelComputer.targetAFR);

	ASSERT_EQ(IM_BATCH, engineConfiguration->injectionMode);
}

TEST(misc, testEngineMath) {
	printf("*************************************************** testEngineMath\r\n");

	// todo: let's see if we can make 'engine' unneeded in this test?
	EngineTestHelper eth(engine_type_e::FORD_ESCORT_GT);

    setCamOperationMode();
	engineConfiguration->fuelAlgorithm = LM_SPEED_DENSITY;

	ASSERT_NEAR( 50,  getOneDegreeTimeMs(600) * 180, EPS4D) << "600 RPM";
	ASSERT_EQ( 5,  getOneDegreeTimeMs(6000) * 180) << "6000 RPM";

	auto fuelComputer = &engine->fuelComputer;

	Sensor::setMockValue(SensorType::Clt, 300);
	Sensor::setMockValue(SensorType::Iat, 350);
	ASSERT_FLOAT_EQ(312.5, fuelComputer->getTCharge(1000, 0));
	ASSERT_FLOAT_EQ(313.5833, fuelComputer->getTCharge(1000, 50));
	ASSERT_FLOAT_EQ(314.6667, fuelComputer->getTCharge(1000, 100));


	ASSERT_FLOAT_EQ(312.5, fuelComputer->getTCharge(4000, 0));
	ASSERT_FLOAT_EQ(320.0833, fuelComputer->getTCharge(4000, 50));
	ASSERT_FLOAT_EQ(327.6667, fuelComputer->getTCharge(4000, 100));

	// test Air Interpolation mode
	engineConfiguration->tChargeMode = TCHARGE_MODE_AIR_INTERP;
	engineConfiguration->tChargeAirCoefMin = 0.098f;
	engineConfiguration->tChargeAirCoefMax = 0.902f;
	engineConfiguration->tChargeAirFlowMax = 153.6f;
	// calc. some airMass given the engine displacement=1.839 and 4 cylinders (FORD_ESCORT_GT)
	fuelComputer->sdAirMassInOneCylinder = SpeedDensityBase::getAirmassImpl(/*VE*/1.0f, /*MAP*/100.0f, /*tChargeK*/273.15f + 20.0f);
	ASSERT_NEAR(0.5464f, fuelComputer->sdAirMassInOneCylinder, EPS4D);

	Sensor::setMockValue(SensorType::Clt, 90);
	Sensor::setMockValue(SensorType::Iat, 20);
	Sensor::setMockValue(SensorType::Map, 100);
	Sensor::setMockValue(SensorType::Tps1, 0);
	Sensor::setMockValue(SensorType::Rpm, 1000);

	// calc. airFlow using airMass, and find tCharge
	engine->periodicFastCallback();
	ASSERT_NEAR(59.12f, engine->engineState.sd.tCharge, EPS4D);
	ASSERT_NEAR(56.9758f/*kg/h*/, engine->engineState.airflowEstimate, EPS4D);
}

typedef enum {
    CS_OPEN = 0,
    CS_CLOSED = 1,
    CS_SWIRL_TUMBLE = 2,

} chamber_style_e;

/**
 * @param octane gas octane number
 * @param bore in mm
 */
static float getTopAdvanceForBore(chamber_style_e style, int octane, double compression, double bore) {
    int octaneCorrection;
    if ( octane <= 90) {
        octaneCorrection = -2;
    } else if (octane < 94) {
        octaneCorrection = -1;
    } else {
        octaneCorrection = 0;
    }

    int compressionCorrection;
    if (compression <= 9) {
        compressionCorrection = 2;
    } else if (compression <= 10) {
        compressionCorrection = 1;
    } else if (compression <= 11) {
        compressionCorrection = 0;
    } else {
        // compression ratio above 11
        compressionCorrection = -2;
    }
    int base;
    if (style == CS_OPEN) {
    	base = 33;
    } else if (style == CS_CLOSED) {
    	base = 28;
    } else {
    	// CS_SWIRL_TUMBLE
    	base = 22;
    }

    float boreCorrection = (bore - 4 * 25.4) / 25.4 * 6;
    float result = base + octaneCorrection + compressionCorrection + boreCorrection;
    return ((int)(result * 10)) / 10.0;
}


TEST(misc, testIgnitionMapGenerator) {
	printf("*************************************************** testIgnitionMapGenerator\r\n");

	ASSERT_EQ(35, getTopAdvanceForBore(CS_OPEN, 98, 8, 101.6));
	ASSERT_EQ(33, getTopAdvanceForBore(CS_OPEN, 98, 11, 101.6));

	float rpmBin[16];
	setRpmBin(rpmBin, 16, 800, 7000);
	ASSERT_EQ(650, rpmBin[0]);
	ASSERT_EQ( 800,  rpmBin[1]) << "@1";
	ASSERT_EQ( 1100,  rpmBin[2]) << "@2";
	ASSERT_EQ( 1400,  rpmBin[3]) << "rpm@3";
	ASSERT_EQ( 4700,  rpmBin[14]) << "rpm@14";
	ASSERT_EQ(7000, rpmBin[15]);


	ASSERT_FLOAT_EQ(22.0, getTopAdvanceForBore(CS_SWIRL_TUMBLE, 89, 9, 101.6));
    ASSERT_FLOAT_EQ(32.2, getTopAdvanceForBore(CS_SWIRL_TUMBLE, 89, 9, 145));

    assertEqualsM2("100@6000", 36.0, getInitialAdvance(6000, 100, 36), 0.1);
    assertEqualsM2("100@600", 9.9, getInitialAdvance(600, 100, 36), 0.2);

    assertEqualsM2("2400", 34.2, getInitialAdvance(2400, 40, 36), 0.1);
    assertEqualsM2("4400", 41.9, getInitialAdvance(4400, 40, 36), 0.1);
    assertEqualsM2("20@800", 14.2, getInitialAdvance(800, 20, 36), 0.2);
}
