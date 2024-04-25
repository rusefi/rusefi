#include "pch.h"

#include "can_listener.h"

using ::testing::StrictMock;
using ::testing::_;

class MockCanListener : public CanListener {
public:
	MockCanListener(uint32_t id) : CanListener(id) { }

	MOCK_METHOD(void, decodeFrame, (const CANRxFrame& frame, efitick_t nowNt), (override));
	MOCK_METHOD(bool, acceptFrame, (const CANRxFrame& frame), (const, override));
};

TEST(CanListener, FrameAccepted) {
	StrictMock<MockCanListener> dut(0x123);

	CANRxFrame frame;

	// Accept should be called, returns true
	EXPECT_CALL(dut, acceptFrame(_)).WillOnce(Return(true));

	// Because accept returns true, decode is called
	EXPECT_CALL(dut, decodeFrame(_, efitick_t{1234}));

	dut.processFrame(frame, 1234);
}

TEST(CanListener, FrameNotAccepted) {
	StrictMock<MockCanListener> dut(0x123);

	CANRxFrame frame;

	// Accept should be called, returns false, so decode not called
	EXPECT_CALL(dut, acceptFrame(_)).WillOnce(Return(false));

	dut.processFrame(frame, 1234);
}

struct CanListenerNoDecode : public CanListener {
	CanListenerNoDecode(uint32_t id) : CanListener(id) { }

	void decodeFrame(const CANRxFrame& frame, efitick_t nowNt) override { }
};

TEST(CanListener, FrameAcceptedChecksId) {
	CanListenerNoDecode dut(0x123);

	CANRxFrame frame;

	frame.SID = 0x123;
	frame.IDE = false;

	EXPECT_TRUE(dut.acceptFrame(frame));
}

TEST(CanListener, FrameNotAcceptedChecksId) {
	CanListenerNoDecode dut(0x123);

	CANRxFrame frame;

	frame.SID = 0x456;
	frame.IDE = false;

	EXPECT_FALSE(dut.acceptFrame(frame));
}
