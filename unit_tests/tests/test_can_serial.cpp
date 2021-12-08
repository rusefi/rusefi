/*
 * @file test_can_serial.cpp
 *
 * This is more like ISO-TP test really?
 *
 *  Created on: Nov 26, 2020
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include <list>
#include <string>

#include "pch.h"
#include "engine_test_helper.h"
#include "serial_can.h"


using namespace std::string_literals;

class TestCanStreamer : public ICanStreamer {
public:
	virtual can_msg_t transmit(canmbx_t mailbox, const CanTxMessage *ctfp, can_sysinterval_t timeout) override {
		const CANTxFrame * frame = ctfp->getFrame();
		// invoke copy constructor to clone frame
		CANTxFrame localCopy = *frame;
		localCopy.DLC = 8;
		ctfList.emplace_back(localCopy);
		return CAN_MSG_OK;
	}

	virtual can_msg_t receive(canmbx_t mailbox, CANRxFrame *crfp, can_sysinterval_t timeout) override {
		if (crfList.empty())
			return CAN_MSG_TIMEOUT;
		*crfp = *crfList.begin();
		crfList.pop_front();
		return CAN_MSG_OK;
	}

	template<typename T>
	void checkFrame(const T & frame, const std::string & bytes) {
		EXPECT_EQ(bytes.size(), frame.DLC);
		for (size_t i = 0; i < bytes.size(); i++) {
  			EXPECT_EQ(bytes[i], frame.data8[i]) << "Frame byte #" << i << " differs!";
		}
	}

public:
	std::list<CANTxFrame> ctfList;
	std::list<CANRxFrame> crfList;
};

class TestCanStreamerState : public CanStreamerState {
public:
	TestCanStreamerState() : CanStreamerState(&streamer) {}

	void test(const std::vector<std::string> & dataList, const std::vector<std::string> & frames, int fifoLeftoverSize, const std::vector<size_t> & receiveChunks) {
		EngineTestHelper eth(TEST_ENGINE);

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
		auto it2 = frames.begin();
		for (; it1 != streamer.ctfList.end() && it2 != frames.end(); it1++, it2++) {
			streamer.checkFrame(*it1, *it2);
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
			uint8_t rxbuf[256];
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
	}

protected:
	TestCanStreamer streamer;
};


TEST(testCanSerial, test1Frame) {

	{
		TestCanStreamerState state;
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
		state.test({ "01234567" }, { "\x10"s "\x08"s "012345"s, "\x21"s "67\0\0\0\0\0"s }, 8, { 8 }); // 8 bytes -> 2 8-byte frames, 8 bytes in FIFO
	}
	/*
	{
		TestCanStreamerState state;
		state.test({ "0123456ABCDEFG" }, { "\x07"s "0123456"s, "\x07"s "ABCDEFG"s }, 0, { 14 }); // 14 bytes -> 2 8-byte frames, empty FIFO
	}
	{
		TestCanStreamerState state;
		state.test({ "0123456ABCDEFG" }, { "\x07"s "0123456"s, "\x07"s "ABCDEFG"s }, 0, { 6, 1, 1, 6 }); // 14 bytes -> 2 8-byte frames, empty FIFO, split receive test
	}
	*/
}

/*
TEST(testCanSerial, testIrregularSplits) {
	{
		TestCanStreamerState state;
		state.test({ "012", "3456ABCDEFG" }, { "\x07"s "0123456"s, "\x07"s "ABCDEFG"s }, 0, { 7, 7 }); // 14 bytes -> 2 8-byte frames, empty FIFO
	}
	{
		TestCanStreamerState state;
		state.test({ "0123456ABC", "DEFG" }, { "\x07"s "0123456"s, "\x07"s "ABCDEFG"s }, 0, { 14 }); // 14 bytes -> 2 8-byte frames, empty FIFO
	}
}

TEST(testCanSerial, testLongMessage) {
	{
		TestCanStreamerState state;
		state.test({ "abcdefghijklmnopqrstuvwxyz" }, {
			"\x07"s "abcdefg"s,
			"\x07"s "hijklmn"s,
			"\x07"s "opqrstu"s,
			"\x05"s "vwxyz\0\0"s }, 5, { 26 }); // 26 bytes -> 4 8-byte frames, 5 bytes left in FIFO
	}
}
*/



