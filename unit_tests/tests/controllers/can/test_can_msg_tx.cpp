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
bool drainWaitHook;
bool resetWaitHook;
bool reenterWaitHook;
bool disableWaitHook;
msg_t reenterResult;
bool servicingFromReschedule;

void serviceWorkerFromReschedule() {
	if (!servicingFromReschedule) {
		servicingFromReschedule = true;
		CanTxMessage::serviceOne(0);
		servicingFromReschedule = false;
	}
}

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
	{
		CanTxMessage announcement(CanCategory::SERIAL, 0x770017, 8, /* bus */ 0, /* extended */ true);
	}
	// Run the CAN1 worker here because host tests do not start real worker threads.
	CanTxMessage::serviceOne(0);
}

// Simulate what happens while a sender waits: run the worker, reset the bus,
// disable transmission, or start another send, depending on the test.
void serviceWaitHook(size_t bus) {
	if (disableWaitHook) {
		engine->allowCanTx = false;
	}
	if (resetWaitHook) {
		CanTxMessage::stopBus(bus);
		return;
	}
	if (drainWaitHook) {
		CanTxMessage::serviceOne(bus);
	}
	if (reenterWaitHook) {
		reenterWaitHook = false;
		CanTxMessage nested(CanCategory::SERIAL, 0x456, 8, bus);
		reenterResult = nested.submitAndWait(TIME_IMMEDIATE);
	}
}

class DualCanWithDisconnectedSecondaryTest : public ::testing::Test {
protected:
	void SetUp() override {
		canTransmitMock = simulatedCanTransmit;
		CanTxMessage::setDevice(0, &primaryCan);
		CanTxMessage::setDevice(1, &unacknowledgedSecondaryCan);
		CanTxMessage::stopBus(0);
		CanTxMessage::stopBus(1);
		simulatedTimeMs = 0;
		primaryTransmitTimeMs = 0;
		primaryTransmitCount = 0;
		secondaryTransmitCount = 0;
		lastPrimaryFrame = {};
		drainWaitHook = false;
		resetWaitHook = false;
		reenterWaitHook = false;
		disableWaitHook = false;
		reenterResult = MSG_OK;
		servicingFromReschedule = false;
		CanTxMessage::setWaitHookForUnitTest(serviceWaitHook);
		CanTxMessage::setRescheduleHookForUnitTest(nullptr);
	}

	void TearDown() override {
		CanTxMessage::removeDevice(0);
		CanTxMessage::removeDevice(1);
		canTransmitMock = nullptr;
		CanTxMessage::setWaitHookForUnitTest(nullptr);
		CanTxMessage::setRescheduleHookForUnitTest(nullptr);
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

	// Queue 12 frames on disconnected CAN2. CAN1 must send immediately,
	// without waiting for CAN2's 100 ms timeout on each frame.
	simulatedTimeMs = 0;
	engineConfiguration->canBroadcastUseChannel = static_cast<can_broadcast_channel_e>(1);
	sendCanVerbose();
	sendSyntheticPrimaryCanFrame();

	EXPECT_EQ(2u, primaryTransmitCount);
	EXPECT_EQ(0u, primaryTransmitTimeMs);
	EXPECT_EQ(0u, secondaryTransmitCount);
	for (int i = 0; i < 12; i++) {
		ASSERT_TRUE(CanTxMessage::serviceOne(1));
	}
	EXPECT_EQ(12u, secondaryTransmitCount);
	EXPECT_EQ(1200u, simulatedTimeMs);
}

TEST_F(DualCanWithDisconnectedSecondaryTest, QueueCopiesFrameAndExplicitSubmissionDoesNotDoubleSend) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	{
		CanTxMessage message(CanCategory::SERIAL, 0x123, 8, 0);
		message[0] = 0x5a;
		ASSERT_TRUE(message.submit());
		message[0] = 0;
	}

	ASSERT_TRUE(CanTxMessage::serviceOne(0));
	EXPECT_EQ(1u, primaryTransmitCount);
	EXPECT_EQ(0x5au, lastPrimaryFrame.data8[0]);
	EXPECT_FALSE(CanTxMessage::serviceOne(0));
}

TEST_F(DualCanWithDisconnectedSecondaryTest, FullQueueDropsNewestAndResetDiscardsTraffic) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	for (int i = 0; i < CAN_TX_QUEUE_CAPACITY; i++) {
		CanTxMessage message(CanCategory::SERIAL, i, 8, 1);
		ASSERT_TRUE(message.submit());
	}
	{
		CanTxMessage newest(CanCategory::SERIAL, 0x7ff, 8, 1);
		EXPECT_FALSE(newest.submit());
	}
	EXPECT_EQ(1, CanTxMessage::getQueueDropCount(1));

	CanTxMessage::stopBus(1);
	EXPECT_FALSE(CanTxMessage::serviceOne(1));
	EXPECT_EQ(0u, secondaryTransmitCount);
}

TEST_F(DualCanWithDisconnectedSecondaryTest, SubmitAndWaitUsesQueuedWorkerAndCanCompleteRepeatedly) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	drainWaitHook = true;

	for (int i = 0; i < 4; i++) {
		CanTxMessage message(CanCategory::SERIAL, 0x500 + i, 8, 0);
		EXPECT_EQ(MSG_OK, message.submitAndWait(TIME_MS2I(10)));
	}
	EXPECT_EQ(4u, primaryTransmitCount);
}

// let the higher-priority worker finish the send as soon as it is
// woken, before submitAndWait starts waiting for the result.
TEST_F(DualCanWithDisconnectedSecondaryTest, SubmitAndWaitRunsWorkerFromSchedulerHandoff) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	CanTxMessage::setWaitHookForUnitTest(nullptr);
	CanTxMessage::setRescheduleHookForUnitTest(serviceWorkerFromReschedule);
	CanTxMessage message(CanCategory::SERIAL, 0x510, 8, 0);
	EXPECT_EQ(MSG_OK, message.submitAndWait(TIME_MS2I(10)));
	EXPECT_EQ(1u, primaryTransmitCount);
}

TEST_F(DualCanWithDisconnectedSecondaryTest, DoneSlotIsNotReusedBeforeItsOwnerConsumesIt) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	drainWaitHook = true;
	// Finish the first send, then start a second send before the first caller
	// reads its result. The second send times out, but must not overwrite the
	// first send's successful result.
	reenterWaitHook = true;
	CanTxMessage message(CanCategory::SERIAL, 0x501, 8, 0);
	EXPECT_EQ(MSG_OK, message.submitAndWait(TIME_MS2I(10)));
	EXPECT_EQ(MSG_TIMEOUT, reenterResult);
}

TEST_F(DualCanWithDisconnectedSecondaryTest, TimedOutQueuedFrameIsCancelledBeforeTransmit) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	CanTxMessage::setWaitHookForUnitTest(nullptr);
	CanTxMessage message(CanCategory::SERIAL, 0x502, 8, 0);
	EXPECT_EQ(MSG_TIMEOUT, message.submitAndWait(TIME_MS2I(10)));
	EXPECT_TRUE(CanTxMessage::serviceOne(0));
	EXPECT_EQ(0u, primaryTransmitCount);

	// The worker discarded the timed-out frame and freed its slot for a new send.
	drainWaitHook = true;
	CanTxMessage::setWaitHookForUnitTest(serviceWaitHook);
	CanTxMessage retry(CanCategory::SERIAL, 0x503, 8, 0);
	EXPECT_EQ(MSG_OK, retry.submitAndWait(TIME_MS2I(10)));
}

TEST_F(DualCanWithDisconnectedSecondaryTest, ResetReleasesSynchronousWaiterAndFullQueueCanProgress) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	resetWaitHook = true;
	CanTxMessage resetMessage(CanCategory::SERIAL, 0x504, 8, 0);
	EXPECT_EQ(MSG_RESET, resetMessage.submitAndWait(TIME_MS2I(10)));

	CanTxMessage::setDevice(0, &primaryCan);
	for (int i = 0; i < CAN_TX_QUEUE_CAPACITY; i++) {
		CanTxMessage message(CanCategory::SERIAL, i, 8, 0);
		ASSERT_TRUE(message.submit());
	}
	resetWaitHook = false;
	drainWaitHook = true;
	CanTxMessage waiting(CanCategory::SERIAL, 0x505, 8, 0);
	EXPECT_EQ(MSG_OK, waiting.submitAndWait(TIME_MS2I(10)));
}

TEST_F(DualCanWithDisconnectedSecondaryTest, ImmediateBadBusAndDisabledTxDoNotQueueOrDoubleSubmit) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	CanTxMessage immediate(CanCategory::SERIAL, 0x506, 8, 0);
	EXPECT_EQ(MSG_TIMEOUT, immediate.submitAndWait(TIME_IMMEDIATE));
	EXPECT_TRUE(CanTxMessage::serviceOne(0));
	EXPECT_EQ(0u, primaryTransmitCount);

	CanTxMessage badBus(CanCategory::SERIAL, 0x507, 8, 0);
	badBus.busIndex = EFI_CAN_BUS_COUNT;
	EXPECT_EQ(MSG_RESET, badBus.submitAndWait(TIME_MS2I(1)));

	engine->allowCanTx = false;
	CanTxMessage disabled(CanCategory::SERIAL, 0x508, 8, 0);
	EXPECT_EQ(MSG_RESET, disabled.submitAndWait(TIME_MS2I(1)));
	EXPECT_EQ(0u, primaryTransmitCount);
}

// disabling CAN transmission must also stop frames already queued,
// including periodic messages and messages whose callers are waiting for a result.
TEST_F(DualCanWithDisconnectedSecondaryTest, DisableTxAfterAdmissionResetsQueuedTraffic) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	{
		CanTxMessage periodic(CanCategory::VERBOSE, 0x200, 8, 0);
		ASSERT_TRUE(periodic.submit());
	}
	drainWaitHook = true;
	disableWaitHook = true;
	CanTxMessage synchronous(CanCategory::SERIAL, 0x710, 8, 0);
	EXPECT_EQ(MSG_RESET, synchronous.submitAndWait(TIME_MS2I(10)));
	EXPECT_EQ(0u, primaryTransmitCount);
	EXPECT_FALSE(CanTxMessage::serviceOne(0));
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
