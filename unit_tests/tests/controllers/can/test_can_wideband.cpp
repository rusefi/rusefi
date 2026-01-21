#include "pch.h"

#include "AemXSeriesLambda.h"

TEST(CanWideband, AcceptFrameId0) {
	AemXSeriesWideband dut(0, SensorType::Lambda1);
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	CANRxFrame frame;

	// AEM uses extended CAN ID!
	frame.IDE = true;
	frame.DLC = 8;

	engineConfiguration->canWbo[0].type = AEM;

	// Check that the AEM format frame is accepted
	frame.EID = 0x180;
	EXPECT_TRUE(dut.acceptFrame(0, frame));

	// Check that the AEM frame with standard CAN ID is not accepted
	frame.IDE = false;
	frame.SID = 0x180;
	EXPECT_FALSE(dut.acceptFrame(0, frame));

	// Now switch to RusEFI
	engineConfiguration->canWbo[0].type = RUSEFI;

	// Check that the rusEFI standard data is accepted
	frame.IDE = false;
	frame.SID = 0x190;
	EXPECT_TRUE(dut.acceptFrame(0, frame));

	// Check that the rusEFI extended data is accepted
	frame.SID = 0x191;
	EXPECT_TRUE(dut.acceptFrame(0, frame));

	// Check that the rusEFI frames with extended CAN ID are not accepted
	frame.IDE = true;
	frame.EID = 0x190;
	EXPECT_FALSE(dut.acceptFrame(0, frame));
	frame.EID = 0x191;
	EXPECT_FALSE(dut.acceptFrame(0, frame));
}

TEST(CanWideband, AcceptFrameId1) {
	AemXSeriesWideband dut(1, SensorType::Lambda2);
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	CANRxFrame frame;

	// AEM uses extended CAN ID!
	frame.IDE = true;
	frame.DLC = 8;

	engineConfiguration->canWbo[1].type = AEM;
	engineConfiguration->canWbo[1].aemId = WBO_AEM_ID2;

	// Check that the AEM format frame is accepted
	frame.EID = 0x181;
	EXPECT_TRUE(dut.acceptFrame(0, frame));

	// Now switch to RusEFI
	engineConfiguration->canWbo[1].type = RUSEFI;
	engineConfiguration->canWbo[1].reId = WBO_RE_ID2;

	// Check that the rusEFI standard data is accepted
	frame.IDE = false;
	frame.SID = 0x192;
	EXPECT_TRUE(dut.acceptFrame(0, frame));

	// Check that the rusEFI extended data is accepted
	frame.SID = 0x193;
	EXPECT_TRUE(dut.acceptFrame(0, frame));
}

class AemXSeriesWidebandWrapper: AemXSeriesWideband {
public:
	using AemXSeriesWideband::AemXSeriesWideband;
	using AemXSeriesWideband::decodeAemXSeries;
	using AemXSeriesWideband::Register;
};

TEST(CanWideband,DecodeAemXSeriesInvalidLambda){
	AemXSeriesWidebandWrapper wbo(0, SensorType::Lambda1);
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	CANRxFrame frame;

	engineConfiguration->canWbo[0].type = AEM;

	// AEM uses extended CAN ID!
	frame.IDE = true;
	frame.EID = 0x180;

	frame.DLC = 8;

	frame.data8[0] = 0x1F;
	frame.data8[1] = 0x40;
	frame.data8[2] = 0;
	frame.data8[3] = 0;
	frame.data8[4] = 0x79;
	frame.data8[5] = 0x6E;
	frame.data8[6] = 0b01000000;
	frame.data8[7] = 0;
	
	EXPECT_FALSE(wbo.decodeAemXSeries(frame, getTimeNowNt()));
}

TEST(CanWideband,DecodeAemXSeriesSensorFault){
	AemXSeriesWidebandWrapper wbo(0, SensorType::Lambda1);
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	CANRxFrame frame;

	engineConfiguration->canWbo[0].type = AEM;

	// AEM uses extended CAN ID!
	frame.IDE = true;
	frame.EID = 0x180;

	frame.DLC = 8;

	frame.data8[0] = 0x1F;
	frame.data8[1] = 0x40;
	frame.data8[2] = 0;
	frame.data8[3] = 0;
	frame.data8[4] = 0x79;
	frame.data8[5] = 0x6E;
	frame.data8[6] = 0;
	frame.data8[7] = 0b00000010;
	
	EXPECT_FALSE(wbo.decodeAemXSeries(frame, getTimeNowNt()));
}


TEST(CanWideband,DecodeAemXSeriesValidLambda){
	AemXSeriesWidebandWrapper wbo(0, SensorType::Lambda1);
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	// we dont call initLambda on the tests init code. so we need to register this sensor on the test
	smoothedLambda1Sensor.Register();

	engineConfiguration->canWbo[0].type = AEM;

	// only this tests needs register
	wbo.Register();

	CANRxFrame frame;

	// AEM uses extended CAN ID!
	frame.IDE = true;
	frame.EID = 0x180;

	frame.DLC = 8;

	frame.data8[0] = 0x2F;	// 8000, lambda 0.8
	frame.data8[1] = 0x00;
	frame.data8[2] = 0;
	frame.data8[3] = 0;
	frame.data8[4] = 0;
	frame.data8[5] = 0;
	frame.data8[6] =
		1 << 1 |	// LSU 4.9 detected
		1 << 7;		// Data valid
	frame.data8[7] = 0;

	wbo.decodeAemXSeries(frame, getTimeNowNt());

	EXPECT_FLOAT_EQ(1.2032f, Sensor::get(SensorType::Lambda1).value_or(-1));
	EXPECT_FLOAT_EQ(1.2032f, Sensor::get(SensorType::SmoothedLambda1).value_or(-1));
	Sensor::resetRegistry();
}

TEST(CanWideband, DecodeValidAemFormat) {
	AemXSeriesWideband dut(0, SensorType::Lambda1);
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	dut.Register();

	engineConfiguration->canWbo[0].type = AEM;

	// check not set
	EXPECT_FLOAT_EQ(-1, Sensor::get(SensorType::Lambda1).value_or(-1));

	CANRxFrame frame;

	// AEM uses extended CAN ID!
	frame.IDE = true;
	frame.EID = 0x180;

	frame.DLC = 8;

	frame.data8[0] = 0x1F;
	frame.data8[1] = 0x40;
	frame.data8[2] = 0;
	frame.data8[3] = 0;
	frame.data8[4] = 0;
	frame.data8[5] = 0;
	frame.data8[6] =
		1 << 1 |	// LSU 4.9 detected
		1 << 7;		// Data valid
	frame.data8[7] = 0;

	// check that lambda updates
	dut.processFrame(0, frame, getTimeNowNt());
	EXPECT_FLOAT_EQ(0.8f, Sensor::get(SensorType::Lambda1).value_or(-1));


	// Now check invalid data
	frame.data8[6] =
		1 << 1 |	// LSU 4.9 detected
		0 << 7;		// Data INVALID

	dut.processFrame(0, frame, getTimeNowNt());
	EXPECT_FLOAT_EQ(-1, Sensor::get(SensorType::Lambda1).value_or(-1));


	// Now check sensor fault
	frame.data8[6] =
		1 << 1 |	// LSU 4.9 detected
		1 << 7;		// Data valid
	frame.data8[7] = 1 << 6; // Sensor fault!

	dut.processFrame(0, frame, getTimeNowNt());
	EXPECT_FLOAT_EQ(-1, Sensor::get(SensorType::Lambda1).value_or(-1));

	Sensor::resetRegistry();
}

#include "wideband_firmware/for_rusefi/wideband_can.h"

TEST(CanWideband, DecodeRusefiStandard)
{
	AemXSeriesWideband dut(0, SensorType::Lambda1);
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	// we dont call initLambda on the tests init code. so we need to register this sensor on the test
	smoothedLambda1Sensor.Register();

	engineConfiguration->canWbo[0].type = RUSEFI;

	dut.Register();

	CANRxFrame frame;
	frame.SID = 0x190;
	frame.IDE = false;
	frame.DLC = 8;

	// version
	frame.data8[0] = RUSEFI_WIDEBAND_VERSION;

	// valid
	frame.data8[1] = 1;

	// data = 0.7 lambda
	*reinterpret_cast<uint16_t*>(&frame.data8[2]) = 7000;

	// data = 1234 deg C
	*reinterpret_cast<uint16_t*>(&frame.data8[4]) = 1234;

	CANRxFrame diagFrame;
	diagFrame.SID = 0x191;
	diagFrame.IDE = false;
	diagFrame.DLC = 8;

	// ESR
	*reinterpret_cast<uint16_t*>(&diagFrame.data8[0]) = 720;

	// nernst DC
	*reinterpret_cast<uint16_t*>(&diagFrame.data8[2]) = 450;

	// PumpDuty
	diagFrame.data8[4] = 127;

	// Status
	diagFrame.data8[5] = 0;

	// HeaterDuty
	diagFrame.data8[6] = 127;

	// check not set
	EXPECT_FLOAT_EQ(-1, Sensor::get(SensorType::Lambda1).value_or(-1));

	// check that lambda updates
	dut.processFrame(0, frame, getTimeNowNt());
	dut.processFrame(0, diagFrame, getTimeNowNt());
	EXPECT_FLOAT_EQ(0.7f, Sensor::get(SensorType::Lambda1).value_or(-1));
	EXPECT_FLOAT_EQ(0.7f, Sensor::get(SensorType::SmoothedLambda1).value_or(-1));

	// Check that temperature updates
	EXPECT_EQ(dut.tempC, 1234);

	// Check that valid bit is respected (should be invalid now)
	frame.data8[1] = 0;
	dut.processFrame(0, frame, getTimeNowNt());
	dut.processFrame(0, diagFrame, getTimeNowNt());
	EXPECT_FLOAT_EQ(-1, Sensor::get(SensorType::Lambda1).value_or(-1));

	// ...but no error until egine is runnig
	EXPECT_EQ((uint8_t)wbo::Status::NotAllowed, dut.faultCode);

	// Now driver should handle valid bit and error states from wbo
	engine->engineState.heaterControlEnabled = true;
	dut.processFrame(0, frame, getTimeNowNt());
	dut.processFrame(0, diagFrame, getTimeNowNt());
	EXPECT_FLOAT_EQ(-1, Sensor::get(SensorType::Lambda1).value_or(-1));

	// make valid again, but report WBO error in diagnostic frame
	frame.data8[1] = 1;
	diagFrame.data8[5] = (uint8_t)wbo::Status::SensorUnderheat;
	dut.processFrame(0, frame, getTimeNowNt());
	dut.processFrame(0, diagFrame, getTimeNowNt());
	EXPECT_EQ((uint8_t)wbo::Status::SensorUnderheat, dut.faultCode);
	EXPECT_FLOAT_EQ(0.7f, Sensor::get(SensorType::Lambda1).value_or(-1));
}

TEST(CanWideband, DecodeRusefiStandardWrongVersion)
{
	AemXSeriesWideband dut(0, SensorType::Lambda1);
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->canWbo[0].type = RUSEFI;

	dut.Register();

	CANRxFrame frame;
	frame.SID = 0x190;
	frame.IDE = false;
	frame.DLC = 8;

	// version - WRONG VERSION ON PURPOSE!
	frame.data8[0] = RUSEFI_WIDEBAND_VERSION_MIN - 1;

	EXPECT_FATAL_ERROR(dut.processFrame(0, frame, getTimeNowNt()));
}
