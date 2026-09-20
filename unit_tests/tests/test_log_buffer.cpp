#include "pch.h"

extern bool isInHardFaultHandler;
extern bool verboseMode;

using ::testing::ElementsAre;

namespace {
class FaultLogging : public ::testing::Test, public priv::LoggingTestSink {
protected:
	void SetUp() override {
		previousFault = isInHardFaultHandler;
		previousVerbose = verboseMode;
		isInHardFaultHandler = false;
		verboseMode = false;
		previousSink = priv::setLoggingTestSink(this);
	}

	void TearDown() override {
		priv::setLoggingTestSink(previousSink);
		isInHardFaultHandler = previousFault;
		verboseMode = previousVerbose;
	}

	LogLineBuffer* acquire() override {
		acquireCalls++;
		return bufferAvailable ? &line : nullptr;
	}

	void publish(LogLineBuffer* submitted) override {
		publishCalls++;
		EXPECT_EQ(&line, submitted);
		output.writeLine(submitted);
	}

	bool bufferAvailable = true;
	int acquireCalls = 0;
	int publishCalls = 0;
	LogBuffer<300> output;

private:
	LogLineBuffer line{};
	bool previousFault = false;
	bool previousVerbose = false;
	priv::LoggingTestSink* previousSink = nullptr;
};

TEST_F(FaultLogging, NormalLoggingFormatsAndQueuesMessage) {
	efiPrintf("value=%d\nnext", 42);

	EXPECT_EQ(1, acquireCalls);
	EXPECT_EQ(1, publishCalls);
	EXPECT_STREQ(PROTOCOL_MSG LOG_DELIMITER "value=42 next" LOG_DELIMITER, output.get());
}

// TDB: reproduce current bad behavior. The fix should return before acquiring
// a buffer (the firmware backend takes a ChibiOS lock there), with no output.
TEST_F(FaultLogging, CurrentlyQueuesNormalMessageDuringHardFault) {
	isInHardFaultHandler = true;

	efiPrintf("fault context %d", 7);

	EXPECT_EQ(1, acquireCalls);
	EXPECT_EQ(1, publishCalls);
	EXPECT_STREQ(PROTOCOL_MSG LOG_DELIMITER "fault context 7" LOG_DELIMITER, output.get());
	EXPECT_TRUE(isInHardFaultHandler);
}

// An empty queue drops the message, but still enters the unsafe queue path.
// TDB: after suppression is added, acquireCalls must also be zero.
TEST_F(FaultLogging, CurrentlyAttemptsBufferAcquisitionDuringHardFaultWithEmptyQueue) {
	isInHardFaultHandler = true;
	bufferAvailable = false;

	efiPrintf("fault context");

	EXPECT_EQ(1, acquireCalls);
	EXPECT_EQ(0, publishCalls);
	EXPECT_EQ(0u, output.length());
	EXPECT_TRUE(isInHardFaultHandler);
}
} // namespace

TEST(logBuffer, writeSmall) {
	LogBuffer<10> dut;
	memset(dut.m_buffer, 0x55, sizeof(dut.m_buffer));

	LogLineBuffer line;

	strcpy(line.buffer, "test");

	dut.writeLine(&line);

	EXPECT_THAT(dut.m_buffer, ElementsAre(
		't', 'e', 's', 't', '\0',	// this part got copied in
		0x55, 0x55, 0x55, 0x55, 0x55	// rest of the buffer is untouched
	));
}

// https://github.com/rusefi/rusefi/issues/10159 - a line buffer with no null
// terminator anywhere (every byte in use) must not be read past its end
TEST(logBuffer, writeUnterminatedLine) {
	LogBuffer<300> dut;
	memset(dut.m_buffer, 0x55, sizeof(dut.m_buffer));

	LogLineBuffer line;
	memset(line.buffer, 'A', sizeof(line.buffer));

	dut.writeLine(&line);

	// exactly sizeof(line.buffer) chars copied - the read stopped at the source
	// buffer boundary, not at some null byte beyond it
	EXPECT_EQ(sizeof(line.buffer), dut.length());
	for (size_t i = 0; i < sizeof(line.buffer); i++) {
		EXPECT_EQ('A', dut.m_buffer[i]);
	}
	EXPECT_EQ('\0', dut.m_buffer[sizeof(line.buffer)]);
}

// https://github.com/rusefi/rusefi/issues/10159 - a truncated message (think
// massive Lua print) must keep both its null terminator and its trailing
// LOG_DELIMITER framing
TEST(logBuffer, terminateLogLineTruncated) {
	char buffer[16];
	// simulate chvsnprintf of a 100-char message into 16 bytes:
	// 15 chars written, null terminator in the last byte, returns 100
	memset(buffer, 'A', 15);
	buffer[15] = '\0';

	size_t len = priv::terminateLogLine(buffer, sizeof(buffer), 100);

	EXPECT_EQ(15u, len);
	EXPECT_EQ(LOG_DELIMITER[0], buffer[14]);
	EXPECT_EQ('\0', buffer[15]);
	EXPECT_EQ(15u, std::strlen(buffer));
}

TEST(logBuffer, terminateLogLineShortMessageUntouched) {
	char buffer[16] = "abc" LOG_DELIMITER;

	size_t len = priv::terminateLogLine(buffer, sizeof(buffer), 4);

	EXPECT_EQ(4u, len);
	EXPECT_STREQ("abc" LOG_DELIMITER, buffer);
}

// boundary: a message of exactly bufferSize - 1 chars fits with its null
// terminator and must not be modified
TEST(logBuffer, terminateLogLineExactFitUntouched) {
	char buffer[16];
	memset(buffer, 'A', 14);
	buffer[14] = LOG_DELIMITER[0];
	buffer[15] = '\0';

	size_t len = priv::terminateLogLine(buffer, sizeof(buffer), 15);

	EXPECT_EQ(15u, len);
	EXPECT_EQ(LOG_DELIMITER[0], buffer[14]);
	EXPECT_EQ('A', buffer[13]);
	EXPECT_EQ('\0', buffer[15]);
}

TEST(logBuffer, writeOverflow) {
	LogBuffer<10> dut;
	memset(dut.m_buffer, 0x55, sizeof(dut.m_buffer));

	LogLineBuffer line;

	strcpy(line.buffer, "testtesttest");

	dut.writeLine(&line);

	EXPECT_THAT(dut.m_buffer, ElementsAre(
		't', 'e', 's', 't',
		't', 'e', 's', 't',
		't', 0
	));
}
