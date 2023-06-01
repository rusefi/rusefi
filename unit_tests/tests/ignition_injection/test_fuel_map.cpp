/**
 * @file	test_fuel_map.cpp
 *
 * @date Nov 6, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "fuel_math.h"
#include "trigger_structure.h"
#include "trigger_decoder.h"
#include "advance_map.h"

using ::testing::FloatNear;

TEST(misc, testFuelMap) {
	printf("Setting up FORD_ASPIRE_1996\r\n");
	EngineTestHelper eth(engine_type_e::FORD_ASPIRE_1996);

	for (int i = 0; i < VBAT_INJECTOR_CURVE_SIZE; i++) {
		engineConfiguration->injector.battLagCorrBins[i] = i;
		engineConfiguration->injector.battLagCorr[i] = 0.5 + 2 * i;
	}

	eth.engine.updateSlowSensors();

	Sensor::setMockValue(SensorType::Clt, 36.605f);
	Sensor::setMockValue(SensorType::Iat, 30.0f);

	// because all the correction tables are zero
	printf("*************************************************** getRunningFuel 1\r\n");
	eth.engine.periodicFastCallback();
	ASSERT_NEAR(5.3679, getRunningFuel(5), EPS4D) << "base fuel";

	printf("*************************************************** setting IAT table\r\n");
	for (int i = 0; i < IAT_CURVE_SIZE; i++) {
		config->iatFuelCorrBins[i] = i * 10;
		config->iatFuelCorr[i] = 2 * i;
	}
	config->iatFuelCorr[0] = 2;

	printf("*************************************************** setting CLT table\r\n");
	for (int i = 0; i < CLT_CURVE_SIZE; i++) {
		config->cltFuelCorrBins[i] = i * 10;
		config->cltFuelCorr[i] = i;
	}

	Sensor::setMockValue(SensorType::Clt, 70.0f);
	Sensor::setMockValue(SensorType::Iat, 30.0f);

	setFlatInjectorLag(0);

	float iatCorrection = getIatFuelCorrection();
	ASSERT_EQ( 6,  iatCorrection) << "IAT";
	float cltCorrection = getCltFuelCorrection();
	ASSERT_EQ( 7,  cltCorrection) << "CLT";


	engineConfiguration->mafAdcChannel = EFI_ADC_10;

	// 1005 * 2 for IAT correction
	printf("*************************************************** getRunningFuel 2\r\n");
	eth.engine.periodicFastCallback();

	// Check that runningFuel corrects appropriately
	EXPECT_EQ( 42,  getRunningFuel(1)) << "v1";
	EXPECT_EQ( 84,  getRunningFuel(2)) << "v1";

	engineConfiguration->cranking.baseFuel = 4000;

	// Should use 20 degree correction in case of failed sensor
	Sensor::resetMockValue(SensorType::Clt);
	EXPECT_NEAR(12.4, getCrankingFuel3(2, 0), EPS4D);

	Sensor::setMockValue(SensorType::Clt, 0);
	EXPECT_NEAR(7.7333, getCrankingFuel3(2, 4), EPS4D);
	Sensor::setMockValue(SensorType::Clt, 8);
	EXPECT_NEAR_M4(7, getCrankingFuel3(2, 15));
	Sensor::setMockValue(SensorType::Clt, 70);
	EXPECT_NEAR_M4(8, getCrankingFuel3(2, 0));
	Sensor::setMockValue(SensorType::Clt, 70);
	EXPECT_NEAR_M4(4, getCrankingFuel3(2, 50));
}


static void configureFordAspireTriggerWaveform(TriggerWaveform * s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Rise);

	s->addEvent720(53.747, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(121.90, false, TriggerWheel::T_SECONDARY);
	s->addEvent720(232.76, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(300.54, false, TriggerWheel::T_SECONDARY);
	s->addEvent720(360, true, TriggerWheel::T_PRIMARY);

	s->addEvent720(409.8412, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(478.6505, false, TriggerWheel::T_SECONDARY);
	s->addEvent720(588.045, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(657.03, false, TriggerWheel::T_SECONDARY);
	s->addEvent720(720, false, TriggerWheel::T_PRIMARY);

	ASSERT_FLOAT_EQ(53.747 / 720, s->wave.getSwitchTime(0));
	ASSERT_EQ(true,  s->wave.getChannelState(1, 0)) << "@0";
	ASSERT_EQ(true,  s->wave.getChannelState(1, 0)) << "@0";

	ASSERT_EQ(false, s->wave.getChannelState(0, 1)) << "@1";
	ASSERT_EQ(false, s->wave.getChannelState(1, 1)) << "@1";

	ASSERT_EQ(false, s->wave.getChannelState(0, 2)) << "@2";
	ASSERT_EQ(true,  s->wave.getChannelState(1, 2)) << "@2";

	ASSERT_EQ(false, s->wave.getChannelState(0, 3)) << "@3";
	ASSERT_EQ(false, s->wave.getChannelState(1, 3)) << "@3";

	ASSERT_EQ(true,  s->wave.getChannelState(0, 4)) << "@4";
	ASSERT_EQ(true,  s->wave.getChannelState(1, 5)) << "@5";
	ASSERT_EQ(false, s->wave.getChannelState(1, 8)) << "@8";
	ASSERT_FLOAT_EQ(121.90 / 720, s->wave.getSwitchTime(1));
	ASSERT_FLOAT_EQ(657.03 / 720, s->wave.getSwitchTime(8));

	ASSERT_EQ(0, s->wave.findAngleMatch(53.747 / 720.0).value_or(-1)) << "expecting 0";
	ASSERT_FALSE(s->wave.findAngleMatch(53 / 720.0).Valid) << "expecting not found";
	ASSERT_EQ(7, s->wave.findAngleMatch(588.045 / 720.0).value_or(-1));

	ASSERT_EQ( 0,  s->wave.findInsertionAngle(23.747 / 720.0)) << "expecting 0";
	ASSERT_EQ( 1,  s->wave.findInsertionAngle(63.747 / 720.0)) << "expecting 1";
}

TEST(misc, testAngleResolver) {
	printf("*************************************************** testAngleResolver\r\n");

	EngineTestHelper eth(engine_type_e::FORD_ASPIRE_1996);

	engineConfiguration->globalTriggerAngleOffset = 175;

	TriggerWaveform * ts = &engine->triggerCentral.triggerShape;
	TriggerFormDetails *triggerFormDetails = &engine->triggerCentral.triggerFormDetails;
	engine->updateTriggerWaveform();

	assertEqualsM("index 2", 52.76, triggerFormDetails->eventAngles[3]); // this angle is relation to synch point
	assertEqualsM("time 2", 0.3233, ts->wave.getSwitchTime(2));
	assertEqualsM("index 5", 412.76, triggerFormDetails->eventAngles[6]);
	assertEqualsM("time 5", 0.5733, ts->wave.getSwitchTime(5));

	ASSERT_EQ(4, ts->getTriggerWaveformSynchPointIndex());

	ASSERT_EQ( 10,  ts->getSize()) << "shape size";

	TriggerWaveform t;
	configureFordAspireTriggerWaveform(&t);
}

TEST(misc, testPinHelper) {
	printf("*************************************************** testPinHelper\r\n");
	ASSERT_EQ(0, getElectricalValue(0, OM_DEFAULT));
	ASSERT_EQ(1, getElectricalValue(1, OM_DEFAULT));

	ASSERT_EQ(0, getElectricalValue(1, OM_INVERTED));
	ASSERT_EQ(1, getElectricalValue(0, OM_INVERTED));
}
