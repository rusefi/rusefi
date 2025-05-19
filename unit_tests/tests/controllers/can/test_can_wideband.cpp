#include "pch.h"

#include "AemXSeriesLambda.h"

TEST(CanWideband, AcceptFrameId0) {
	AemXSeriesWideband dut(0, SensorType::Lambda1);
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	CANRxFrame frame;

	frame.IDE = false;
	frame.DLC = 8;

	engineConfiguration->wboType1 = AEM;

	// Check that the AEM format frame is accepted
	frame.SID = 0x180;
	EXPECT_TRUE(dut.acceptFrame(frame));

	// Now switch to RusEFI
	engineConfiguration->wboType1 = RUSEFI;

	// Check that the rusEFI standard data is accepted
	frame.SID = 0x190;
	EXPECT_TRUE(dut.acceptFrame(frame));

	// Check that the rusEFI extended data is accepted
	frame.SID = 0x191;
	EXPECT_TRUE(dut.acceptFrame(frame));
}

TEST(CanWideband, AcceptFrameId1) {
	AemXSeriesWideband dut(1, SensorType::Lambda2);
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	CANRxFrame frame;

	frame.IDE = false;
	frame.DLC = 8;

	engineConfiguration->wboType2 = AEM;

	// Check that the AEM format frame is accepted
	frame.SID = 0x181;
	EXPECT_TRUE(dut.acceptFrame(frame));

	// Now switch to RusEFI
	engineConfiguration->wboType2 = RUSEFI;

	// Check that the rusEFI standard data is accepted
	frame.SID = 0x192;
	EXPECT_TRUE(dut.acceptFrame(frame));

	// Check that the rusEFI extended data is accepted
	frame.SID = 0x193;
	EXPECT_TRUE(dut.acceptFrame(frame));
}

class AemXSeriesWidebandWrapper: AemXSeriesWideband {
public:
	using AemXSeriesWideband::AemXSeriesWideband;
	using AemXSeriesWideband::decodeAemXSeries;
	using AemXSeriesWideband::Register;
};

TEST(CanWideband,DecodeAemXSeriesInvalidLambda){
	AemXSeriesWidebandWrapper wbo(0, SensorType::Lambda1);
	CANRxFrame frame;

	frame.SID = 0x180;
	frame.IDE = false;

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
	CANRxFrame frame;

	frame.SID = 0x180;
	frame.IDE = false;

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
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	AemXSeriesWidebandWrapper wbo(0, SensorType::Lambda1);

	// only this tests needs register
	wbo.Register();

	CANRxFrame frame;

	frame.SID = 0x180;
	frame.IDE = false;

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
	Sensor::resetRegistry();
}

TEST(CanWideband, DecodeValidAemFormat) {
  EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	AemXSeriesWideband dut(0, SensorType::Lambda1);
	dut.Register();

	engineConfiguration->wboType1 = AEM;

	// check not set
	EXPECT_FLOAT_EQ(-1, Sensor::get(SensorType::Lambda1).value_or(-1));

	CANRxFrame frame;

	frame.SID = 0x180;
	frame.IDE = false;

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
	dut.processFrame(frame, getTimeNowNt());
	EXPECT_FLOAT_EQ(0.8f, Sensor::get(SensorType::Lambda1).value_or(-1));


	// Now check invalid data
	frame.data8[6] =
		1 << 1 |	// LSU 4.9 detected
		0 << 7;		// Data INVALID

	dut.processFrame(frame, getTimeNowNt());
	EXPECT_FLOAT_EQ(-1, Sensor::get(SensorType::Lambda1).value_or(-1));


	// Now check sensor fault
	frame.data8[6] =
		1 << 1 |	// LSU 4.9 detected
		1 << 7;		// Data valid
	frame.data8[7] = 1 << 6; // Sensor fault!

	dut.processFrame(frame, getTimeNowNt());
	EXPECT_FLOAT_EQ(-1, Sensor::get(SensorType::Lambda1).value_or(-1));

	Sensor::resetRegistry();
}

#include "wideband_firmware/for_rusefi/wideband_can.h"

TEST(CanWideband, DecodeRusefiStandard)
{
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	AemXSeriesWideband dut(0, SensorType::Lambda1);
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
	dut.processFrame(frame, getTimeNowNt());
	dut.processFrame(diagFrame, getTimeNowNt());
	EXPECT_FLOAT_EQ(0.7f, Sensor::get(SensorType::Lambda1).value_or(-1));

	// Check that temperature updates
	EXPECT_EQ(dut.tempC, 1234);

	// Check that valid bit is respected (should be invalid now)
	frame.data8[1] = 0;
	dut.processFrame(frame, getTimeNowNt());
	dut.processFrame(diagFrame, getTimeNowNt());
	EXPECT_FLOAT_EQ(-1, Sensor::get(SensorType::Lambda1).value_or(-1));

	// ...but no error until egine is runnig
	EXPECT_EQ(HACK_CRANKING_VALUE, dut.faultCode);

	// Now driver should handle valid bit and error states from wbo
	engine->engineState.heaterControlEnabled = true;
	dut.processFrame(frame, getTimeNowNt());
	dut.processFrame(diagFrame, getTimeNowNt());
	EXPECT_EQ(HACK_INVALID_RE, dut.faultCode);
	EXPECT_FLOAT_EQ(-1, Sensor::get(SensorType::Lambda1).value_or(-1));

	// make valid again, but report WBO error in diagnostic frame
	frame.data8[1] = 1;
	diagFrame.data8[5] = (uint8_t)wbo::Fault::SensorNoHeatSupply;
	dut.processFrame(frame, getTimeNowNt());
	dut.processFrame(diagFrame, getTimeNowNt());
	EXPECT_EQ((uint8_t)wbo::Fault::SensorNoHeatSupply, dut.faultCode);
	EXPECT_FLOAT_EQ(0.7f, Sensor::get(SensorType::Lambda1).value_or(-1));
}

TEST(CanWideband, DecodeRusefiStandardWrongVersion)
{
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	AemXSeriesWideband dut(0, SensorType::Lambda1);
	dut.Register();

	CANRxFrame frame;
	frame.SID = 0x190;
	frame.IDE = false;
	frame.DLC = 8;

	// version - WRONG VERSION ON PURPOSE!
	frame.data8[0] = RUSEFI_WIDEBAND_VERSION + 1;

	EXPECT_FATAL_ERROR(dut.processFrame(frame, getTimeNowNt()));
}
