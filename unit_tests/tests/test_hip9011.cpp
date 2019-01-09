/*
 * @file test_hip9011.cpp
 *
 *  Created on: Mar 22, 2018
 */

#include "unit_test_framework.h"
#include "hip9011_lookup.h"
#include "HIP9011_logic.h"
using ::testing::_;

TEST(hip9011, lookup) {
	assertEqualsM2("", 3183.1013, getRpmByAngleWindowAndTimeUs(600, 360), 0.1);
	assertEqualsM2("40us", 47746.5195, getRpmByAngleWindowAndTimeUs(40, 360), 0.1);

	assertEqualsM2("600us 50 degree", 442.0974, getRpmByAngleWindowAndTimeUs(600, 50), 0.1);
	assertEqualsM2("240us 50 degree", 1105.2435, getRpmByAngleWindowAndTimeUs(240, 50), 0.1);
	assertEqualsM2("240us 50 degree", 6631.4619, getRpmByAngleWindowAndTimeUs(40, 50), 0.1);

	EXPECT_EQ(0, getHip9011GainIndex(/* knockBandCustom*/NAN, /*cylinderBore*/NAN, /*hip9011Gain*/3));
	EXPECT_EQ(0, getHip9011GainIndex(/* knockBandCustom*/NAN, /*cylinderBore*/NAN, /*hip9011Gain*/2));
	EXPECT_EQ(47, getHip9011GainIndex(/* knockBandCustom*/NAN, /*cylinderBore*/NAN, /*hip9011Gain*/0.234));
	EXPECT_EQ(63, getHip9011GainIndex(/* knockBandCustom*/NAN, /*cylinderBore*/NAN, /*hip9011Gain*/0.000001));

}

TEST(hip9011, rpmLookup) {
	HIP9011 instace(NULL);

	instace.prepareHip9011RpmLookup(50);

	EXPECT_EQ(31, instace.getIntegrationIndexByRpm(1));
	EXPECT_EQ(21, instace.getIntegrationIndexByRpm(1100));
	EXPECT_EQ(1, instace.getIntegrationIndexByRpm(6600));
	EXPECT_EQ(0, instace.getIntegrationIndexByRpm(16600));
}

TEST(hip9011, band) {

	EXPECT_FLOAT_EQ(3, getHIP9011Band(/* knockBandCustom*/3, /*cylinderBore*/76, /*hip9011Gain*/NAN));
	EXPECT_FLOAT_EQ(7.5389242, getHIP9011Band(/* knockBandCustom*/0, /*cylinderBore*/76, /*hip9011Gain*/NAN));

	EXPECT_EQ(42, getBandIndex(/* knockBandCustom*/0, /*cylinderBore*/76, /*hip9011Gain*/NAN));

}

class MockHip9011Hardware : public Hip9011HardwareInterface
{
public:
	MockHip9011Hardware() {  }

    MOCK_METHOD1(sendSyncCommand, void(unsigned char));
    MOCK_METHOD1(sendCommand, void(unsigned char));
};

TEST(hip9011, configurationCommands) {

	MockHip9011Hardware mock;

	HIP9011 instance(&mock);

	instance.prepareHip9011RpmLookup(50);

// want to invoke method with same parameters a few times
#define PARAMETERS 600, _8MHZ_PRESCALER, /* knockBandCustom*/0, /*cylinderBore*/76, /*hip9011Gain*/1

	 // Not making assumptions on the message send ...
	EXPECT_CALL(mock, sendSyncCommand(_)).Times(0);
	EXPECT_CALL(mock, sendCommand(SET_GAIN_CMD + 0xE)).Times(1);
	instance.handleValue(PARAMETERS);

	EXPECT_CALL(mock, sendSyncCommand(_)).Times(0);
	EXPECT_CALL(mock, sendCommand(SET_INTEGRATOR_CMD + 0x1C)).Times(1);
	instance.handleValue(PARAMETERS);

	EXPECT_CALL(mock, sendSyncCommand(_)).Times(0);
	EXPECT_CALL(mock, sendCommand(SET_BAND_PASS_CMD + 0x2A)).Times(1);
	instance.handleValue(PARAMETERS);

	EXPECT_CALL(mock, sendSyncCommand(_)).Times(0);
	EXPECT_CALL(mock, sendCommand(SET_PRESCALER_CMD + 6)).Times(1);
	instance.handleValue(PARAMETERS);

	// initialization is over, no commands should be sent
	EXPECT_CALL(mock, sendSyncCommand(_)).Times(0);
	EXPECT_CALL(mock, sendCommand(_)).Times(0);
	instance.handleValue(PARAMETERS);
}
