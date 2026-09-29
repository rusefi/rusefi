#include "pch.h"
#include "slcan_frame.h"
#include "can_sniffer.h"
#include "can_msg_tx.h"

#include <array>
#include <stdexcept>
#include <string>
#include <vector>

TEST(Slcan, ChannelPrefixesAndLegacyFormat) {
	const uint8_t data[] = {0xAA, 0xBB};
	const char* expected[] = {"t1232AABB\r", "&t1232AABB\r", "$t1232AABB\r"};
	char buffer[slcan::FrameBufferSize];
	for (size_t bus = 0; bus < 3; bus++) {
		ASSERT_EQ(strlen(expected[bus]), slcan::formatFrame(buffer, bus, true,
			0x123, false, false, 2, data, false, 0));
		EXPECT_STREQ(expected[bus], buffer);
		slcan::formatFrame(buffer, bus, false, 0x123, false, false, 2, data, false, 0);
		EXPECT_STREQ("t1232AABB\r", buffer);
	}
}

TEST(Slcan, ExtendedMaximumLengthAndTimestamp) {
	const uint8_t data[] = {0, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0xFF};
	char buffer[slcan::FrameBufferSize];
	EXPECT_EQ(sizeof(buffer) - 1, slcan::formatFrame(buffer, 2, true,
		0x1FFFFFFF, true, false, 8, data, true, 0xABCD));
	EXPECT_STREQ("$T1FFFFFFF800112233445566FFABCD\r", buffer);
}

TEST(Slcan, RemoteFramesAndInvalidInput) {
	char buffer[slcan::FrameBufferSize];
	slcan::formatFrame(buffer, 1, true, 0x123, false, true, 8, nullptr, true, 0x1234);
	EXPECT_STREQ("&r12381234\r", buffer);
	slcan::formatFrame(buffer, 2, true, 0x123, true, true, 0, nullptr, false, 0);
	EXPECT_STREQ("$R000001230\r", buffer);
	EXPECT_EQ(0u, slcan::formatFrame(buffer, 3, true, 0, false, true, 0, nullptr, false, 0));
	EXPECT_STREQ("", buffer);
	EXPECT_EQ(0u, slcan::formatFrame(buffer, 0, true, 0, false, true, 9, nullptr, false, 0));
}

TEST(Slcan, NewTunesIncludeBusByDefault) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	EXPECT_TRUE(engineConfiguration->canSnifferIncludeBus);
}

namespace {

class SlcanTransmit : public testing::Test {
protected:
	EngineTestHelper eth{engine_type_e::TEST_ENGINE};
	CanSniffer sniffer;
	std::array<CANDriver, EFI_CAN_BUS_COUNT> drivers;
	std::array<std::vector<CANTxFrame>, EFI_CAN_BUS_COUNT> frames;
	inline static SlcanTransmit* activeTest = nullptr;

	void SetUp() override {
		activeTest = this;
		engine->allowCanTx = true;
		engineConfiguration->canSnifferTxBus = CAN_BUS_NONE;
		canTransmitMock = captureFrame;
		for (size_t bus = 0; bus < drivers.size(); bus++) {
			CanTxMessage::setDevice(bus, &drivers[bus]);
			CanTxMessage::stopBus(bus);
		}
	}

	void TearDown() override {
		for (size_t bus = 0; bus < drivers.size(); bus++) {
			CanTxMessage::removeDevice(bus);
		}
		canTransmitMock = nullptr;
		txCanBuffer.clear();
		activeTest = nullptr;
	}

	static msg_t captureFrame(CANDriver* driver, canmbx_t, CANTxFrame* frame, can_sysinterval_t) {
		for (size_t bus = 0; bus < activeTest->drivers.size(); bus++) {
			if (driver == &activeTest->drivers[bus]) {
				activeTest->frames[bus].push_back(*frame);
				return MSG_OK;
			}
		}
		ADD_FAILURE() << "Unexpected CAN device";
		return MSG_TIMEOUT;
	}

	std::string command(const std::string& text) {
		std::string response = sniffer.executeCommandForUnitTest(text.c_str());
		for (size_t bus = 0; bus < drivers.size(); bus++) {
			CanTxMessage::service(bus);
		}
		return response;
	}

	void open(const char* mode = "O\r") {
		ASSERT_EQ("\r", command("S6\r"));
		ASSERT_EQ("\r", command(mode));
	}

	void expectNoFrames() {
		for (const auto& busFrames : frames) {
			EXPECT_TRUE(busFrames.empty());
		}
	}
};

TEST_F(SlcanTransmit, UnprefixedFramesUseConfiguredBus) {
	open();
	for (size_t bus = 0; bus < drivers.size(); bus++) {
		SCOPED_TRACE(bus);
		engineConfiguration->canSnifferTxBus = static_cast<can_bus_channel_e>(CAN_BUS_CAN1 + bus);
		EXPECT_EQ("z\r", command("t1232aAbB\r"));
		ASSERT_EQ(1u, frames[bus].size());
		const auto& frame = frames[bus].back();
		EXPECT_EQ(0x123u, CAN_ID(frame));
		EXPECT_FALSE(CAN_ISX(frame));
		EXPECT_FALSE(CAN_ISRTR(frame));
		EXPECT_EQ(2, frame.DLC);
		EXPECT_EQ(0xAA, frame.data8[0]);
		EXPECT_EQ(0xBB, frame.data8[1]);
	}
	for (const auto& busFrames : frames) {
		EXPECT_EQ(1u, busFrames.size());
	}
}

TEST_F(SlcanTransmit, PrefixOverridesConfiguredBusAndAllowsDisabledDefault) {
	open();
	const char* prefixes[] = {"*", "&", "$"};
	for (auto configured : {CAN_BUS_NONE, CAN_BUS_CAN1, CAN_BUS_CAN2}) {
		engineConfiguration->canSnifferTxBus = configured;
		for (size_t bus = 0; bus < drivers.size(); bus++) {
			SCOPED_TRACE(bus);
			EXPECT_EQ("z\r", command(std::string(prefixes[bus]) + "t7ff0\r"));
			ASSERT_FALSE(frames[bus].empty());
			EXPECT_EQ(0x7ffu, CAN_ID(frames[bus].back()));
			EXPECT_EQ(0, frames[bus].back().DLC);
			EXPECT_EQ(configured, engineConfiguration->canSnifferTxBus);
		}
	}
	for (const auto& busFrames : frames) {
		EXPECT_EQ(3u, busFrames.size());
	}
}

TEST_F(SlcanTransmit, ExtendedFramesPreserveIdentifierAndAllPayloadBytes) {
	open();
	const char* prefixes[] = {"*", "&", "$"};
	const uint8_t expected[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0xAB, 0xFF};
	for (size_t bus = 0; bus < drivers.size(); bus++) {
		SCOPED_TRACE(bus);
		EXPECT_EQ("Z\r", command(std::string(prefixes[bus]) + "T1fFfFfFf8001122334455aBfF\r"));
		ASSERT_EQ(1u, frames[bus].size());
		const auto& frame = frames[bus].back();
		EXPECT_EQ(0x1fffffffu, CAN_ID(frame));
		EXPECT_TRUE(CAN_ISX(frame));
		EXPECT_FALSE(CAN_ISRTR(frame));
		ASSERT_EQ(8, frame.DLC);
		for (size_t i = 0; i < sizeof(expected); i++) {
			EXPECT_EQ(expected[i], frame.data8[i]);
		}
	}
}

TEST_F(SlcanTransmit, PrefixOnlySelectsBusForOneCommand) {
	open();
	engineConfiguration->canSnifferTxBus = CAN_BUS_CAN2;
	EXPECT_EQ("Z\r", command("*T012345670\r"));
	EXPECT_EQ("Z\r", command("T076543210\r"));
	ASSERT_EQ(1u, frames[0].size());
	ASSERT_EQ(1u, frames[1].size());
	EXPECT_EQ(0x01234567u, CAN_ID(frames[0].back()));
	EXPECT_EQ(0x07654321u, CAN_ID(frames[1].back()));
	EXPECT_TRUE(CAN_ISX(frames[1].back()));
}

TEST_F(SlcanTransmit, UnprefixedFramesRequireConfiguredBus) {
	open();
	for (auto configured : {CAN_BUS_NONE, static_cast<can_bus_channel_e>(EFI_CAN_BUS_COUNT + 2)}) {
		engineConfiguration->canSnifferTxBus = configured;
		EXPECT_EQ("\a", command("t1230\r"));
		EXPECT_EQ("\a", command("T000001230\r"));
	}
	expectNoFrames();
}

TEST_F(SlcanTransmit, PrefixDoesNotBypassClosedOrListenOnlyTerminal) {
	engineConfiguration->canSnifferTxBus = CAN_BUS_CAN1;
	auto expectRejected = [this] {
		for (const char* prefix : {"", "*", "&", "$"}) {
			EXPECT_EQ("\a", command(std::string(prefix) + "t1230\r"));
			EXPECT_EQ("\a", command(std::string(prefix) + "T000001230\r"));
		}
	};
	expectRejected();
	open("L\r");
	expectRejected();
	EXPECT_EQ("\r", command("C\r"));
	open();
	EXPECT_EQ("\r", command("C\r"));
	expectRejected();
	expectNoFrames();
}

TEST_F(SlcanTransmit, InvalidIdentifiersLengthsAndRemoteFramesAreRejected) {
	open();
	engineConfiguration->canSnifferTxBus = CAN_BUS_CAN1;
	for (const char* prefix : {"", "*", "&", "$"}) {
		for (const char* frame : {"t8000\r", "T200000000\r", "t1239\r", "T000001239\r",
			"r1230\r", "R000001230\r"}) {
			const std::string text = std::string(prefix) + frame;
			SCOPED_TRACE(text);
			EXPECT_EQ("\a", command(text));
		}
	}
	expectNoFrames();
}

#if EFI_CAN_BUS_COUNT == 2
TEST_F(SlcanTransmit, CurrentlyRaisesCriticalErrorForUnavailableThirdBus) {
	open();
	// Reproduce the current off-by-one check (channel > EFI_CAN_BUS_COUNT + 1).
	// A follow-up fix should return BELL instead of reaching CanTxMessage's
	// criticalError, which throws in unit tests.
	EXPECT_THROW(command("$t1230\r"), std::logic_error);
	EXPECT_THROW(command("$T000001230\r"), std::logic_error);
	engineConfiguration->canSnifferTxBus = CAN_BUS_CAN3;
	EXPECT_THROW(command("t1230\r"), std::logic_error);
	expectNoFrames();
}
#endif

} // namespace
