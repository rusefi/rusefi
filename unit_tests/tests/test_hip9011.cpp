/*
 * @file test_hip9011.cpp
 *
 *  Created on: Mar 22, 2018
 */

#include "pch.h"
#include "hip9011_logic.h"

using ::testing::_;

TEST(hip9011, lookup) {
	HIP9011 instance(NULL);

	assertEqualsM2("", 3183.1013, instance.getRpmByAngleWindowAndTimeUs(600, 360), 0.1);
	assertEqualsM2("40us", 47746.5195, instance.getRpmByAngleWindowAndTimeUs(40, 360), 0.1);

	assertEqualsM2("600us 50 degree", 442.0974, instance.getRpmByAngleWindowAndTimeUs(600, 50), 0.1);
	assertEqualsM2("240us 50 degree", 1105.2435, instance.getRpmByAngleWindowAndTimeUs(240, 50), 0.1);
	assertEqualsM2("240us 50 degree", 6631.4619, instance.getRpmByAngleWindowAndTimeUs(40, 50), 0.1);

	EXPECT_EQ(0, instance.getGainIndex(/* knockBandCustom*/NAN, /*cylinderBore*/NAN, /*hip9011Gain*/3, 0, NAN, NAN));
	EXPECT_EQ(0, instance.getGainIndex(/* knockBandCustom*/NAN, /*cylinderBore*/NAN, /*hip9011Gain*/2, 0, NAN, NAN));
	EXPECT_EQ(47, instance.getGainIndex(/* knockBandCustom*/NAN, /*cylinderBore*/NAN, /*hip9011Gain*/0.234, 0, NAN, NAN));
	EXPECT_EQ(63, instance.getGainIndex(/* knockBandCustom*/NAN, /*cylinderBore*/NAN, /*hip9011Gain*/0.000001, 0, NAN, NAN));
	EXPECT_EQ(63, instance.getGainIndex(/* knockBandCustom*/NAN, /*cylinderBore*/NAN, /*hip9011Gain*/-1.0, 0, NAN, NAN));

}

TEST(hip9011, rpmLookup) {
	HIP9011 instance(NULL);

	instance.angleWindowWidth = 50.0;
	instance.prepareRpmLookup();

	EXPECT_EQ(31, instance.getIntegrationIndexByRpm(1));
	EXPECT_EQ(21, instance.getIntegrationIndexByRpm(1100));
	EXPECT_EQ(1, instance.getIntegrationIndexByRpm(6600));
	EXPECT_EQ(0, instance.getIntegrationIndexByRpm(16600));
}

TEST(hip9011, band) {
	HIP9011 instance(NULL);

	EXPECT_FLOAT_EQ(3, instance.getBand(/* knockBandCustom*/3, /*cylinderBore*/76, /*hip9011Gain*/NAN, 0, NAN, NAN));
	EXPECT_FLOAT_EQ(7.5389242, instance.getBand(/* knockBandCustom*/0, /*cylinderBore*/76, /*hip9011Gain*/NAN, 0, NAN, NAN));

	EXPECT_EQ(42, instance.getBandIndex(/* knockBandCustom*/0, /*cylinderBore*/76, /*hip9011Gain*/NAN, 0, NAN, NAN));

}

class MockHip9011Hardware : public Hip9011HardwareInterface
{
public:
	MockHip9011Hardware() {  }

    MOCK_METHOD2(sendSyncCommand, int(unsigned char, unsigned char *));
};

TEST(hip9011, configurationCommands) {

	MockHip9011Hardware mock;

	HIP9011 instance(&mock);

// want to invoke method with same parameters a few times
#define PARAMETERS 600, /* knockBandCustom*/0, /*cylinderBore*/76, /*hip9011Gain*/1, HIP_8MHZ_PRESCALER, 0.0, 50.0

	 // Not making assumptions on the message send ...
	EXPECT_CALL(mock, sendSyncCommand(SET_GAIN_CMD(0xE), 0)).Times(1);
	EXPECT_CALL(mock, sendSyncCommand(SET_INTEGRATOR_CMD(0x1C), 0)).Times(1);
	EXPECT_CALL(mock, sendSyncCommand(SET_BAND_PASS_CMD(0x2A), 0)).Times(1);
	EXPECT_CALL(mock, sendSyncCommand(SET_PRESCALER_CMD(6), 0)).Times(1);
	instance.handleSettings(PARAMETERS);

	// initialization is over, no commands should be sent
	EXPECT_CALL(mock, sendSyncCommand(_, _)).Times(0);
	instance.handleSettings(PARAMETERS);
}
