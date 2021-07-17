/**
 * @file	test_fuel_map.cpp
 *
 * @date Nov 6, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "fuel_math.h"
#include "trigger_structure.h"
#include "allsensors.h"
#include "trigger_decoder.h"
#include "engine_test_helper.h"
#include "advance_map.h"
#include "mocks.h"

using ::testing::FloatNear;

TEST(misc, testFuelMap) {
	printf("Setting up FORD_ASPIRE_1996\r\n");
	WITH_ENGINE_TEST_HELPER(FORD_ASPIRE_1996);

	for (int i = 0; i < VBAT_INJECTOR_CURVE_SIZE; i++) {
		CONFIG(injector.battLagCorrBins[i]) = i;
		CONFIG(injector.battLagCorr[i]) = 0.5 + 2 * i;
	}

	eth.engine.updateSlowSensors(PASS_ENGINE_PARAMETER_SIGNATURE);

	Sensor::setMockValue(SensorType::Clt, 36.605f);
	Sensor::setMockValue(SensorType::Iat, 30.0f);

	// because all the correction tables are zero
	printf("*************************************************** getRunningFuel 1\r\n");
	eth.engine.periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	ASSERT_NEAR(5.3679, getRunningFuel(5 PASS_ENGINE_PARAMETER_SUFFIX), EPS4D) << "base fuel";

	printf("*************************************************** setting IAT table\r\n");
	for (int i = 0; i < IAT_CURVE_SIZE; i++) {
		eth.engine.config->iatFuelCorrBins[i] = i * 10;
		eth.engine.config->iatFuelCorr[i] = 2 * i;
	}
	eth.engine.config->iatFuelCorr[0] = 2;

	printf("*************************************************** setting CLT table\r\n");
	for (int i = 0; i < CLT_CURVE_SIZE; i++) {
		eth.engine.config->cltFuelCorrBins[i] = i * 10;
		eth.engine.config->cltFuelCorr[i] = i;
	}

	Sensor::setMockValue(SensorType::Clt, 70.0f);
	Sensor::setMockValue(SensorType::Iat, 30.0f);

	setFlatInjectorLag(0 PASS_CONFIG_PARAMETER_SUFFIX);

	float iatCorrection = getIatFuelCorrection(PASS_ENGINE_PARAMETER_SIGNATURE);
	ASSERT_EQ( 6,  iatCorrection) << "IAT";
	float cltCorrection = getCltFuelCorrection(PASS_ENGINE_PARAMETER_SIGNATURE);
	ASSERT_EQ( 7,  cltCorrection) << "CLT";


	engineConfiguration->mafAdcChannel = EFI_ADC_10;
	engine->engineState.mockAdcState.setMockVoltage(EFI_ADC_10, 5 PASS_ENGINE_PARAMETER_SUFFIX);

	// 1005 * 2 for IAT correction
	printf("*************************************************** getRunningFuel 2\r\n");
	eth.engine.periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);

	// Check that runningFuel corrects appropriately
	EXPECT_EQ( 42,  getRunningFuel(1 PASS_ENGINE_PARAMETER_SUFFIX)) << "v1";
	EXPECT_EQ( 84,  getRunningFuel(2 PASS_ENGINE_PARAMETER_SUFFIX)) << "v1";

	engine->engineState.mockAdcState.setMockVoltage(EFI_ADC_10, 0 PASS_ENGINE_PARAMETER_SUFFIX);

	engineConfiguration->cranking.baseFuel = 4000;

	// Should use 20 degree correction in case of failed sensor
	Sensor::resetMockValue(SensorType::Clt);
	EXPECT_NEAR(12.4, getCrankingFuel3(2, 0 PASS_ENGINE_PARAMETER_SUFFIX), EPS4D);

	Sensor::setMockValue(SensorType::Clt, 0);
	EXPECT_NEAR(7.7333, getCrankingFuel3(2, 4 PASS_ENGINE_PARAMETER_SUFFIX), EPS4D);
	Sensor::setMockValue(SensorType::Clt, 8);
	EXPECT_NEAR(7, getCrankingFuel3(2, 15 PASS_ENGINE_PARAMETER_SUFFIX), EPS4D);
	Sensor::setMockValue(SensorType::Clt, 70);
	EXPECT_NEAR(8, getCrankingFuel3(2, 0 PASS_ENGINE_PARAMETER_SUFFIX), EPS4D);
	Sensor::setMockValue(SensorType::Clt, 70);
	EXPECT_NEAR(4, getCrankingFuel3(2, 50 PASS_ENGINE_PARAMETER_SUFFIX), EPS4D);
}


static void confgiureFordAspireTriggerWaveform(TriggerWaveform * s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);

	s->addEvent720(53.747, T_SECONDARY, TV_RISE);
	s->addEvent720(121.90, T_SECONDARY, TV_FALL);
	s->addEvent720(232.76, T_SECONDARY, TV_RISE);
	s->addEvent720(300.54, T_SECONDARY, TV_FALL);
	s->addEvent720(360, T_PRIMARY, TV_RISE);

	s->addEvent720(409.8412, T_SECONDARY, TV_RISE);
	s->addEvent720(478.6505, T_SECONDARY, TV_FALL);
	s->addEvent720(588.045, T_SECONDARY, TV_RISE);
	s->addEvent720(657.03, T_SECONDARY, TV_FALL);
	s->addEvent720(720, T_PRIMARY, TV_FALL);

	ASSERT_FLOAT_EQ(53.747 / 720, s->wave.getSwitchTime(0));
	ASSERT_EQ( 1,  s->wave.getChannelState(1, 0)) << "@0";
	ASSERT_EQ( 1,  s->wave.getChannelState(1, 0)) << "@0";

	ASSERT_EQ( 0,  s->wave.getChannelState(0, 1)) << "@1";
	ASSERT_EQ( 0,  s->wave.getChannelState(1, 1)) << "@1";

	ASSERT_EQ( 0,  s->wave.getChannelState(0, 2)) << "@2";
	ASSERT_EQ( 1,  s->wave.getChannelState(1, 2)) << "@2";

	ASSERT_EQ( 0,  s->wave.getChannelState(0, 3)) << "@3";
	ASSERT_EQ( 0,  s->wave.getChannelState(1, 3)) << "@3";

	ASSERT_EQ( 1,  s->wave.getChannelState(0, 4)) << "@4";
	ASSERT_EQ( 1,  s->wave.getChannelState(1, 5)) << "@5";
	ASSERT_EQ( 0,  s->wave.getChannelState(1, 8)) << "@8";
	ASSERT_FLOAT_EQ(121.90 / 720, s->wave.getSwitchTime(1));
	ASSERT_FLOAT_EQ(657.03 / 720, s->wave.getSwitchTime(8));

	ASSERT_EQ( 0,  s->wave.findAngleMatch(53.747 / 720.0, s->getSize())) << "expecting 0";
	assertEqualsM("expecting not found", -1, s->wave.findAngleMatch(53 / 720.0, s->getSize()));
	ASSERT_EQ(7, s->wave.findAngleMatch(588.045 / 720.0, s->getSize()));

	ASSERT_EQ( 0,  s->wave.findInsertionAngle(23.747 / 720.0, s->getSize())) << "expecting 0";
	ASSERT_EQ( 1,  s->wave.findInsertionAngle(63.747 / 720.0, s->getSize())) << "expecting 1";
}

TEST(misc, testAngleResolver) {
	printf("*************************************************** testAngleResolver\r\n");

	WITH_ENGINE_TEST_HELPER(FORD_ASPIRE_1996);

	engineConfiguration->globalTriggerAngleOffset = 175;

	TriggerWaveform * ts = &engine->triggerCentral.triggerShape;
	TriggerFormDetails *triggerFormDetails = &engine->triggerCentral.triggerFormDetails;
	engine->initializeTriggerWaveform(PASS_ENGINE_PARAMETER_SIGNATURE);

	assertEqualsM("index 2", 52.76, triggerFormDetails->eventAngles[3]); // this angle is relation to synch point
	assertEqualsM("time 2", 0.3233, ts->wave.getSwitchTime(2));
	assertEqualsM("index 5", 412.76, triggerFormDetails->eventAngles[6]);
	assertEqualsM("time 5", 0.5733, ts->wave.getSwitchTime(5));

	ASSERT_EQ(4, ts->getTriggerWaveformSynchPointIndex());

	ASSERT_EQ( 10,  ts->getSize()) << "shape size";

	event_trigger_position_s injectionStart;

	printf("*************************************************** testAngleResolver 0\r\n");
	findTriggerPosition(&ENGINE(triggerCentral.triggerShape), &ENGINE(triggerCentral.triggerFormDetails),&injectionStart, -122, engineConfiguration->globalTriggerAngleOffset);
	ASSERT_EQ( 2,  injectionStart.triggerEventIndex) << "eventIndex@0";
	ASSERT_NEAR(0.24, injectionStart.angleOffsetFromTriggerEvent, EPS5D);

	printf("*************************************************** testAngleResolver 0.1\r\n");
	findTriggerPosition(&ENGINE(triggerCentral.triggerShape), &ENGINE(triggerCentral.triggerFormDetails),&injectionStart, -80, engineConfiguration->globalTriggerAngleOffset);
	ASSERT_EQ( 2,  injectionStart.triggerEventIndex) << "eventIndex@0";
	ASSERT_FLOAT_EQ(42.24, injectionStart.angleOffsetFromTriggerEvent);

	printf("*************************************************** testAngleResolver 0.2\r\n");
	findTriggerPosition(&ENGINE(triggerCentral.triggerShape), &ENGINE(triggerCentral.triggerFormDetails),&injectionStart, -54, engineConfiguration->globalTriggerAngleOffset);
	ASSERT_EQ( 2,  injectionStart.triggerEventIndex) << "eventIndex@0";
	ASSERT_FLOAT_EQ(68.2400, injectionStart.angleOffsetFromTriggerEvent);

	printf("*************************************************** testAngleResolver 0.3\r\n");
	findTriggerPosition(&ENGINE(triggerCentral.triggerShape), &ENGINE(triggerCentral.triggerFormDetails),&injectionStart, -53, engineConfiguration->globalTriggerAngleOffset);
	ASSERT_EQ(2, injectionStart.triggerEventIndex);
	ASSERT_FLOAT_EQ(69.24, injectionStart.angleOffsetFromTriggerEvent);

	printf("*************************************************** testAngleResolver 1\r\n");
	findTriggerPosition(&ENGINE(triggerCentral.triggerShape), &ENGINE(triggerCentral.triggerFormDetails),&injectionStart, 0, engineConfiguration->globalTriggerAngleOffset);
	ASSERT_EQ(2, injectionStart.triggerEventIndex);
	ASSERT_FLOAT_EQ(122.24, injectionStart.angleOffsetFromTriggerEvent);

	printf("*************************************************** testAngleResolver 2\r\n");
	findTriggerPosition(&ENGINE(triggerCentral.triggerShape), &ENGINE(triggerCentral.triggerFormDetails),&injectionStart, 56, engineConfiguration->globalTriggerAngleOffset);
	ASSERT_EQ(2, injectionStart.triggerEventIndex);
	ASSERT_FLOAT_EQ(178.24, injectionStart.angleOffsetFromTriggerEvent);

	TriggerWaveform t;
	confgiureFordAspireTriggerWaveform(&t);
}

TEST(misc, testPinHelper) {
	printf("*************************************************** testPinHelper\r\n");
	ASSERT_EQ(0, getElectricalValue(0, OM_DEFAULT));
	ASSERT_EQ(1, getElectricalValue(1, OM_DEFAULT));

	ASSERT_EQ(0, getElectricalValue(1, OM_INVERTED));
	ASSERT_EQ(1, getElectricalValue(0, OM_INVERTED));
}
