#include "pch.h"
#include "uds.h"
#include "can_msg_tx.h"

#if EFI_UDS
static CANRxFrame identityRequest() {
	CANRxFrame frame{};
	CAN_SID(frame) = 0x7E0;
	frame.DLC = 8;
	frame.data8[0] = 3;
	frame.data8[1] = 0x22;
	frame.data8[2] = 0xF1;
	frame.data8[3] = 0xA4;
	return frame;
}

TEST(CanUds, IdentityWithoutSessionOrAuthentication) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	txCanBuffer.clear();
	udsOnCanPacketRx(identityRequest(), 0);
	ASSERT_EQ(txCanBuffer.getCount(), 1);
	auto response = txCanBuffer.get();
	EXPECT_EQ(CAN_SID(response), 0x7E8);
	EXPECT_FALSE(CAN_ISX(response));
	EXPECT_EQ(response.DLC, 8);
	const uint8_t expected[] = {7, 0x62, 0xF1, 0xA4, 'r', 'E', 'F', 'I'};
	EXPECT_EQ(memcmp(response.data8, expected, sizeof(expected)), 0);
}

TEST(CanUds, IgnoreOtherTrafficIncludingBoardActivation) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	txCanBuffer.clear();
	for (int variation = 0; variation < 9; variation++) {
		auto frame = identityRequest();
		switch (variation) {
		case 0: CAN_ISX(frame) = true; break;
		case 1: CAN_ISRTR(frame) = true; break;
		case 2: CAN_SID(frame) = 0x7DF; break;
		case 3: frame.DLC = 3; break;
		case 4: frame.DLC = 9; break;
		case 5: frame.data8[0] = 0x10; break;
		case 6: frame.data8[1] = 0x10; break;
		case 7: frame.data8[2] = 0xF0; break;
		case 8: frame.data8[3] = 0xA0; break;
		}
		udsOnCanPacketRx(frame, 0);
		EXPECT_EQ(txCanBuffer.getCount(), 0) << variation;
	}
}

TEST(CanUds, InvalidLengthAndUnpaddedRequest) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	txCanBuffer.clear();
	auto frame = identityRequest();
	frame.data8[0] = 5;
	udsOnCanPacketRx(frame, 0);
	ASSERT_EQ(txCanBuffer.getCount(), 1);
	auto response = txCanBuffer.get();
	EXPECT_EQ(response.data8[1], 0x7F);
	EXPECT_EQ(response.data8[2], 0x22);
	EXPECT_EQ(response.data8[3], 0x13);
	frame.data8[0] = 3;
	frame.DLC = 4;
	udsOnCanPacketRx(frame, 0);
	ASSERT_EQ(txCanBuffer.getCount(), 1);
	EXPECT_EQ(txCanBuffer.get().data8[1], 0x62);
}
#endif
