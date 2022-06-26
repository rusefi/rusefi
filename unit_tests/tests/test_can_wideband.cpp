#include "pch.h"

#include "AemXSeriesLambda.h"

TEST(CanWideband, AcceptFrameId0) {
	AemXSeriesWideband dut(0, SensorType::Lambda1);

	CANRxFrame frame;

	frame.IDE = false;
	frame.DLC = 8;

	// Check that the AEM format frame is accepted
	frame.SID = 0x180;
	EXPECT_TRUE(dut.acceptFrame(frame));

	// Check that the rusEFI standard data is accepted
	frame.SID = 0x190;
	EXPECT_TRUE(dut.acceptFrame(frame));

	// Check that the rusEFI extended data is accepted
	frame.SID = 0x191;
	EXPECT_TRUE(dut.acceptFrame(frame));
}

TEST(CanWideband, AcceptFrameId1) {
	AemXSeriesWideband dut(1, SensorType::Lambda2);

	CANRxFrame frame;

	frame.IDE = false;
	frame.DLC = 8;

	// Check that the AEM format frame is accepted
	frame.SID = 0x181;
	EXPECT_TRUE(dut.acceptFrame(frame));

	// Check that the rusEFI standard data is accepted
	frame.SID = 0x192;
	EXPECT_TRUE(dut.acceptFrame(frame));

	// Check that the rusEFI extended data is accepted
	frame.SID = 0x193;
	EXPECT_TRUE(dut.acceptFrame(frame));
}

TEST(CanWideband, DecodeValidAemFormat) {
	AemXSeriesWideband dut(0, SensorType::Lambda1);
	dut.Register();

	// check not set
	EXPECT_FLOAT_EQ(-1, Sensor::get(SensorType::Lambda1).value_or(-1));

	CANRxFrame frame;

	frame.SID = 0x180;
	frame.IDE = false;

	frame.DLC = 8;

	frame.data8[0] = 0x1F;	// 8000, lambda 0.8
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
	EngineTestHelper eth(TEST_ENGINE);

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

	// check not set
	EXPECT_FLOAT_EQ(-1, Sensor::get(SensorType::Lambda1).value_or(-1));

	// check that lambda updates
	dut.processFrame(frame, getTimeNowNt());
	EXPECT_FLOAT_EQ(0.7f, Sensor::get(SensorType::Lambda1).value_or(-1));

	// Check that temperature updates
	EXPECT_EQ(dut.tempC, 1234);

	// Check that valid bit is respected (should be invalid now)
	frame.data8[1] = 0;
	dut.processFrame(frame, getTimeNowNt());
	EXPECT_FLOAT_EQ(-1, Sensor::get(SensorType::Lambda1).value_or(-1));
}

TEST(CanWideband, DecodeRusefiStandardWrongVersion)
{
	EngineTestHelper eth(TEST_ENGINE);

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
