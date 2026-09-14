#include "pch.h"
#include "can_msg_tx.h"
#include "isotp.h"
#include "rusefi_lua.h"

namespace {

CANDriver primaryCan;
CANDriver unacknowledgedSecondaryCan;
uint32_t simulatedTimeMs;
uint32_t primaryTransmitTimeMs;
uint32_t primaryTransmitCount;
uint32_t secondaryTransmitCount;
CANTxFrame lastPrimaryFrame;

msg_t simulatedCanTransmit(CANDriver* device, canmbx_t, CANTxFrame* frame, can_sysinterval_t timeout) {
	if (device == &unacknowledgedSecondaryCan) {
		secondaryTransmitCount++;
		simulatedTimeMs += timeout;
		return MSG_TIMEOUT;
	}

	if (device == &primaryCan) {
		primaryTransmitCount++;
		primaryTransmitTimeMs = simulatedTimeMs;
		lastPrimaryFrame = *frame;
	}

	return MSG_OK;
}

void sendSyntheticPrimaryCanFrame() {
	CanTxMessage announcement(CanCategory::SERIAL, 0x770017, 8, /* bus */ 0, /* extended */ true);
}

class DualCanWithDisconnectedSecondaryTest : public ::testing::Test {
protected:
	void SetUp() override {
		canTransmitMock = simulatedCanTransmit;
		CanTxMessage::setDevice(0, &primaryCan);
		CanTxMessage::setDevice(1, &unacknowledgedSecondaryCan);
		simulatedTimeMs = 0;
		primaryTransmitTimeMs = 0;
		primaryTransmitCount = 0;
		secondaryTransmitCount = 0;
		lastPrimaryFrame = {};
	}

	void TearDown() override {
		CanTxMessage::removeDevice(0);
		CanTxMessage::removeDevice(1);
		canTransmitMock = nullptr;
	}
};

} // namespace

TEST(CanTxMessage, SetIntValueLsb) {
    // We need some setup because CanTxMessage destructor tries to send the message.
    // In unit tests, txCanBuffer is used.

    // Create a message
    CanTxMessage msg(CanCategory::NBC, 0x123, 8, /*bus*/0);

    uint32_t value = 0x12345678;
    msg.setIntValueLsb(value, 0);

    EXPECT_EQ(msg[0], 0x78);
    EXPECT_EQ(msg[1], 0x56);
    EXPECT_EQ(msg[2], 0x34);
    EXPECT_EQ(msg[3], 0x12);

    value = 0xAABBCCDD;
    msg.setIntValueLsb(value, 4);

    EXPECT_EQ(msg[4], 0xDD);
    EXPECT_EQ(msg[5], 0xCC);
    EXPECT_EQ(msg[6], 0xBB);
    EXPECT_EQ(msg[7], 0xAA);
}

TEST_F(DualCanWithDisconnectedSecondaryTest, DisconnectedSecondaryDelaysHealthyPrimaryTransmission) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// Healthy baseline: a CAN1 frame sends immediately when no secondary frames precede it.
	sendSyntheticPrimaryCanFrame();
	EXPECT_EQ(0u, primaryTransmitTimeMs);
	EXPECT_EQ(1u, primaryTransmitCount);
	EXPECT_EQ(0x770017u, CAN_ID(lastPrimaryFrame));
	EXPECT_EQ(CAN_IDE_EXT, lastPrimaryFrame.IDE);

	// sender emits 12 frames on configured CAN2.  Each
	// unacknowledged transmit blocks for 100 ms of timeout before
	// the subsequent CAN1 transmission can run.
	simulatedTimeMs = 0;
	engineConfiguration->canBroadcastUseChannel = static_cast<can_broadcast_channel_e>(1);
	sendCanVerbose();
	sendSyntheticPrimaryCanFrame();

	EXPECT_EQ(12u, secondaryTransmitCount);
	EXPECT_EQ(2u, primaryTransmitCount);
	EXPECT_EQ(1200u, primaryTransmitTimeMs);
	EXPECT_GE(primaryTransmitTimeMs, 1000u);
}

TEST_F(DualCanWithDisconnectedSecondaryTest, LuaTransmitsWithPeriodicWriterDisabled) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->canReadEnabled = true;
	engineConfiguration->canWriteEnabled = false;
	engine->allowCanTx = true;

	EXPECT_EQ(1, testLuaReturnsInteger(R"(
		function testFunc()
			txCan(1, 0x123, 0, {0x12, 0x34, 0x56})
			return 1
		end
	)"));

	// Check the HAL call
	ASSERT_EQ(1u, primaryTransmitCount);
	EXPECT_EQ(0u, secondaryTransmitCount);
	EXPECT_EQ(0x123u, CAN_ID(lastPrimaryFrame));
	EXPECT_EQ(CAN_IDE_STD, lastPrimaryFrame.IDE);
	EXPECT_EQ(3u, lastPrimaryFrame.DLC);
	EXPECT_EQ(0x12u, lastPrimaryFrame.data8[0]);
	EXPECT_EQ(0x34u, lastPrimaryFrame.data8[1]);
	EXPECT_EQ(0x56u, lastPrimaryFrame.data8[2]);
}

TEST_F(DualCanWithDisconnectedSecondaryTest, IsoTpTransmitsWithPeriodicWriterDisabled) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->canReadEnabled = true;
	engineConfiguration->canWriteEnabled = false;
	engine->allowCanTx = true;

	IsoTpRxTx isoTp(0, 0x7e0, 0x7e8);
	const uint8_t reply[] = {0x62, 0xf1, 0x90};
	EXPECT_EQ(3, isoTp.writeTimeout(reply, sizeof(reply), TIME_MS2I(10)));

	ASSERT_EQ(1u, primaryTransmitCount);
	EXPECT_EQ(0u, secondaryTransmitCount);
	EXPECT_EQ(0x7e8u, CAN_ID(lastPrimaryFrame));
	EXPECT_EQ(CAN_IDE_STD, lastPrimaryFrame.IDE);
	EXPECT_EQ(8u, lastPrimaryFrame.DLC);
	EXPECT_EQ(3u, lastPrimaryFrame.data8[0]); // Single-frame payload length.
	EXPECT_EQ(0x62u, lastPrimaryFrame.data8[1]);
	EXPECT_EQ(0xf1u, lastPrimaryFrame.data8[2]);
	EXPECT_EQ(0x90u, lastPrimaryFrame.data8[3]);
}
