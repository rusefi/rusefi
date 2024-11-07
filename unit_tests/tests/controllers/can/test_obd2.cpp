#include "pch.h"

#include "can_msg_tx.h"
#include "can.h"
#include "can_listener.h"
#include "obd2.h"


TEST(CanObd2, obdSendPacket)
{
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	obdSendPacket(0, PID_RPM, 1, 0xff, 0);
	EXPECT_TRUE(txCanBuffer.getCount());

	CANTxFrame frame = txCanBuffer.get();

	EXPECT_EQ(frame.data8[0], 3);		// correct data size
	EXPECT_EQ(frame.data8[1], 0X40);	// correct header
	EXPECT_EQ(frame.data8[2], PID_RPM); // correct PID

	// clear shared buffer
	txCanBuffer.clear();
	EXPECT_FALSE(txCanBuffer.getCount());
}
