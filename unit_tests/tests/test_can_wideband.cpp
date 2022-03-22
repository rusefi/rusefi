#include "pch.h"

#include "AemXSeriesLambda.h"

using ::testing::StrictMock;
using ::testing::_;

TEST(CanWideband, DecodeValid) {
	Sensor::resetRegistry();

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
}
