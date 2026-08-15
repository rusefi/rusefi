/*
 * @file test_can_serial.cpp
 *
 * This is more like ISO-TP test really?
 *
 *  Created on: Nov 26, 2020
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "engine_test_helper.h"
#include "serial_can.h"

#include <array>
#include <list>
#include <string>

using namespace std::string_literals;

// todo: split into TX and RX parts?
class TestCanTransport : public ICanTransport {
public:
	virtual can_msg_t transmit(CanTxMessage &ctfp, can_sysinterval_t timeout) override {
		const CANTxFrame * frame = ctfp.getFrame();
		// invoke copy constructor to clone frame
		CANTxFrame localCopy = *frame;
		localCopy.DLC = 8;
		ctfList.emplace_back(localCopy);
		return CAN_MSG_OK;
	}

  virtual void onTpFirstFrame() override {
    // todo: add coverage?
  }

	virtual can_msg_t receive(CANRxFrame *crfp, can_sysinterval_t timeout) override {
		if (crfList.empty())
			return CAN_MSG_TIMEOUT;
		*crfp = *crfList.begin();
		crfList.pop_front();
		return CAN_MSG_OK;
	}

	template<typename T>
	void checkFrame(const T & frame, const std::string & bytes, int frameIndex) {
		EXPECT_EQ(bytes.size(), frame.DLC);
		for (size_t i = 0; i < bytes.size(); i++) {
  			EXPECT_EQ(bytes[i], frame.data8[i]) << "Frame byte #" << i << " differs! Frame " << frameIndex;
		}
	}

public:
	std::list<CANTxFrame> ctfList;
	std::list<CANRxFrame> crfList;
};

class TestCanStreamerState : public CanStreamerState {
public:
	TestCanStreamerState() : CanStreamerState(&streamer, &streamer, 0, 10, 10) {}

	void test(const std::vector<std::string> & dataList, const std::vector<std::string> & frames, int fifoLeftoverSize, const std::vector<size_t> & receiveChunks) {
		EngineTestHelper eth(engine_type_e::TEST_ENGINE);

		size_t totalSize = 0;
		std::string totalData;
		for (auto data : dataList) {
			size_t np = data.size();

			totalSize += np;
			totalData += data;

			streamAddToTxTimeout(&np, (uint8_t *)data.c_str(), 0);
		}

		// check the FIFO buf size
		EXPECT_EQ(fifoLeftoverSize, txFifoBuf.getCount());

		// send the rest
		streamFlushTx(0);

		// check if correct the TX frames were sent
		EXPECT_EQ(frames.size(), streamer.ctfList.size());

		auto it1 = streamer.ctfList.begin();
		int frameIndex = 0;
		auto it2 = frames.begin();
		for (; it1 != streamer.ctfList.end() && it2 != frames.end(); it1++, it2++) {
			streamer.checkFrame(*it1, *it2, frameIndex++);
		}

		// copy transmitted data back into the receive buffer
		for (auto f : streamer.ctfList) {
			CANRxFrame rf;
			rf.DLC = f.DLC;
			rf.RTR = f.RTR;
			rf.IDE = f.IDE;
			rf.EID = f.EID;
			rf.data64[0] = f.data64[0];
			streamer.crfList.push_back(rf);
		}

		size_t totalReceivedSize = 0;
		std::string totalReceivedData;
		for (size_t chunkSize : receiveChunks) {
			size_t nr = chunkSize;
			uint8_t rxbuf[1256];
			streamReceiveTimeout(&nr, rxbuf, 0);
			EXPECT_EQ(nr, chunkSize);
			totalReceivedSize += nr;
			totalReceivedData += std::string((const char *)rxbuf, nr);
		}
		// we should receive the same amount of bytes that we've sent
		EXPECT_EQ(totalSize, totalReceivedSize);
		// check the data
		for (size_t i = 0; i < totalSize; i++) {
			EXPECT_EQ(totalData[i], totalReceivedData[i]) << "Rcv. byte #" << i << " differs!";
		}
		// check the FIFO buf size
		EXPECT_EQ(0, rxFifoBuf.getCount());

		// clear shared buffer
		txCanBuffer.clear();
		EXPECT_FALSE(txCanBuffer.getCount());
	}

protected:
	TestCanTransport streamer;
};

TEST(testCanSerial, test1Frame) {

	{
		TestCanStreamerState state;
		state.paddingByte = 0;
		state.test({ "1" }, { "\x01"s "1\0\0\0\0\0\0"s }, 1, { 1 }); // 1 byte -> 1 frame, 1 byte in FIFO
	}
	{
		TestCanStreamerState state;
		state.test({ "0123456" }, { "\x07"s "0123456"s }, 7, { 7 }); // 7 bytes -> 1 8-byte frame
	}
	{
		TestCanStreamerState state;
		state.test({ "0123456" }, { "\x07"s "0123456"s }, 7, { 1, 1, 1, 1, 1, 1, 1 }); // 7 bytes -> 1 8-byte frame, split receive test
	}
	{
		TestCanStreamerState state;
		state.test({ "0123456" }, { "\x07"s "0123456"s }, 7, { 3, 4 }); // 7 bytes -> 1 8-byte frame, split receive test
	}
	{
		TestCanStreamerState state;
		state.test({ "0", "1", "2", "3", "4", "5", "6" }, { "\x07"s "0123456"s }, 7, { 7 }); // 7 bytes separately -> 1 8-byte frame
	}
}

TEST(testCanSerial, test2Frames) {
	{
		TestCanStreamerState state;
		state.paddingByte = 0;
		state.test({ "01234567" }, { "\x10"s "\x08"s "012345"s, "\x21"s "67\0\0\0\0\0"s }, 8, { 8 }); // 8 bytes -> 2 8-byte frames, 8 bytes in FIFO
	}
	{
		TestCanStreamerState state;
		state.paddingByte = 0;
		state.test({ "0123456789A" }, { "\x10"s "\x0B"s "012345"s, "\x21"s "6789A\0\0"s }, 11, { 2, 5, 4 }); // 11 bytes -> 2 8-byte frames
	}
	{
		TestCanStreamerState state;
		state.paddingByte = 0;
		state.test({ "0123456ABCDEFG" }, { "\x10"s  "\x0E"s "012345"s, "\x21"s "6ABCDEF"s, "\x22"s "G\0\0\0\0\0\0"s }, 14, { 14 }); // 14 bytes -> 3 8-byte frames, empty FIFO
	}
}

TEST(testCanSerial, testIrregularSplits) {
	{
		TestCanStreamerState state;
		state.paddingByte = 0;
		state.test({ "012", "3456ABCDEFG" }, { "\x10"s  "\x0E"s "012345"s, "\x21"s "6ABCDEF"s, "\x22"s "G\0\0\0\0\0\0"s }, 14, { 7, 7 }); // 14 bytes -> 2 8-byte frames, empty FIFO
	}
	{
		TestCanStreamerState state;
		state.paddingByte = 0;
		state.test({ "0123456ABC", "DEFG" }, { "\x10"s  "\x0E"s "012345"s, "\x21"s "6ABCDEF"s, "\x22"s "G\0\0\0\0\0\0"s }, 14, { 14 }); // 14 bytes -> 2 8-byte frames, empty FIFO
	}
}

TEST(testCanSerial, testLongMessage) {
	{
		TestCanStreamerState state;
		state.test({ "abcdefghijklmnopqrstuvwxyz" }, {
				"\x10"s  "\x1A"s "abcdef"s,
				"\x21"s "ghijklm"s,
				"\x22"s "nopqrst"s,
 		    "\x23"s "uvwxyz\x0A"s }, 26, { 26 }); // 26 bytes -> 4 8-byte frames, 5 bytes left in FIFO
	}
}

TEST(testCanSerial, test64_7Message) {
	std::array<char, 71> buffer;

	std::fill(std::begin(buffer), std::end(buffer), 0);

	buffer[0] = 1;

	buffer[64 + 7 - 1] = 4;
	std::string str(std::begin(buffer),std::end(buffer));

	TestCanStreamerState state;
	state.test({ str }, {
			/* 0 */
			"\x10"s  "\x47"s "\x01\0\0\0\0\0"s,
			"\x21"s "\0\0\0\0\0\0\0"s,
			"\x22"s "\0\0\0\0\0\0\0"s,
		    "\x23"s "\0\0\0\0\0\0\0"s,
		    "\x24"s "\0\0\0\0\0\0\0"s,
		    "\x25"s "\0\0\0\0\0\0\0"s,
		    "\x26"s "\0\0\0\0\0\0\0"s,
		    "\x27"s "\0\0\0\0\0\0\0"s,
		    "\x28"s "\0\0\0\0\0\0\0"s,
		    "\x29"s "\0\0\0\0\0\0\0"s,

			/* 10 */
			"\x2A"s "\0\4\x0A\x0A\x0A\x0A\x0A"s,

	}, 71, { 64 + 7 });
}

/**
 * Coverage for the ISO-TP single-frame length field.
 *
 * A single frame carries its payload length in the low nibble of the PCI byte, so it can encode up
 * to 15 - but a classic-CAN frame only delivers DLC bytes and CANRxFrame::data8 is 8 bytes wide.
 * receiveFrame() used to take that nibble at face value and copy that many bytes from data8 + 1,
 * reading past the end of the payload. DLC itself is only a 4-bit field, so a frame can also claim
 * DLC > 8, which was equally unchecked. The length is now bounded by both.
 *
 * The frame is embedded in a padded wrapper so that the over-read this deliberately provokes lands
 * inside memory the test owns - otherwise it is a stack-buffer-overflow and AddressSanitizer (on by
 * default for these tests) aborts the run.
 */
namespace {
struct PaddedRxFrame {
	CANRxFrame frame;
	// room for the largest over-read the unclamped code can perform: SF_DL 15 starting at data8[1]
	uint8_t overreadGuard[16];
};

// Fails the test rather than corrupting the stack if the guard above is ever too small for the
// over-read these tests provoke (last byte read is data8[15], so data8 + 16 must stay inside).
void assertOverreadStaysInsidePaddedFrame(const PaddedRxFrame& padded) {
	const uint8_t* objectEnd = reinterpret_cast<const uint8_t*>(&padded) + sizeof(padded);
	ASSERT_LE(padded.frame.data8 + 16, objectEnd) << "overreadGuard too small";
}
} // namespace

TEST(testCanSerial, singleFrameLengthIsBoundedByDlc) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	TestCanStreamerState state;

	PaddedRxFrame padded;
	memset(&padded, 0, sizeof(padded));
	padded.frame.DLC = 8;
	// frame type 0 (SINGLE), SF_DL nibble = 15 - more than the 8-byte frame can possibly hold
	padded.frame.data8[0] = 0x0F;

	uint8_t rxbuf[64];
	memset(rxbuf, 0xCC, sizeof(rxbuf));

	assertOverreadStaysInsidePaddedFrame(padded);
	int copied = state.receiveFrame(padded.frame, rxbuf, sizeof(rxbuf), 0);

	// clamped to the 7 payload bytes the frame actually carries
	EXPECT_EQ(7, copied);
}

TEST(testCanSerial, singleFrameLengthIsBoundedByPayloadSize) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	TestCanStreamerState state;

	PaddedRxFrame padded;
	memset(&padded, 0, sizeof(padded));
	// DLC is a 4-bit field, so a malformed frame can claim more than data8[] can hold
	padded.frame.DLC = 15;
	padded.frame.data8[0] = 0x0F;

	uint8_t rxbuf[64];
	memset(rxbuf, 0xCC, sizeof(rxbuf));

	assertOverreadStaysInsidePaddedFrame(padded);
	int copied = state.receiveFrame(padded.frame, rxbuf, sizeof(rxbuf), 0);

	// clamped to sizeof(data8) - 1 even though DLC claims more
	EXPECT_EQ(7, copied);
}

TEST(testCanSerial, test3_64_4Message) {
	std::array<char, 64> buffer64;

	std::fill(std::begin(buffer64), std::end(buffer64), 0);

	buffer64[0] = 1;

	buffer64[64 - 1] = 4;
	std::string str(std::begin(buffer64),std::end(buffer64));

	TestCanStreamerState state;
	state.test({ "123"s, str, "abcd"s }, {
			/* 0 */
			"\x10"s  "\x47"s "123\1\0\0"s,
			"\x21"s "\0\0\0\0\0\0\0"s,
			"\x22"s "\0\0\0\0\0\0\0"s,
		    "\x23"s "\0\0\0\0\0\0\0"s,
		    "\x24"s "\0\0\0\0\0\0\0"s,
		    "\x25"s "\0\0\0\0\0\0\0"s,
		    "\x26"s "\0\0\0\0\0\0\0"s,
		    "\x27"s "\0\0\0\0\0\0\0"s,
		    "\x28"s "\0\0\0\0\0\0\0"s,
		    "\x29"s "\0\0\0\0\4ab"s,

			/* 10 */
			"\x2A"s "cd\x0A\x0A\x0A\x0A\x0A"s,

	}, 71, { 64 + 7 });
}

