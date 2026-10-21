#include "pch.h"

#include <rusefi/crc.h>

#include <string>
#include <vector>

// loggingcentral.cpp line buffers: LogBuffer<128>, 64 of them
static constexpr size_t lineSize = 128;
static constexpr size_t lineCount = 64;
// loggingSendOutputData() consolidates at most this many buffers per packet
static constexpr size_t maxBuffersPerPacket = 16;

#define MSG_PREFIX PROTOCOL_MSG LOG_DELIMITER

namespace {

class CaptureTsChannel : public TsChannelBase {
public:
	CaptureTsChannel() : TsChannelBase("capture") { }

	void write(const uint8_t* buffer, size_t size, bool isEndOfPacket) override {
		EXPECT_FALSE(packetComplete) << "write after end of packet";
		bytes.insert(bytes.end(), buffer, buffer + size);
		packetComplete = isEndOfPacket;
	}

	size_t readTimeout(uint8_t* /*buffer*/, size_t /*size*/, int /*timeout*/) override {
		return 0;
	}

	// Validates the framing of the single captured packet and returns its payload
	std::string payload() {
		EXPECT_TRUE(packetComplete);
		// 2 bytes size + 1 byte response code + payload + 4 bytes CRC
		if (bytes.size() < 7) {
			ADD_FAILURE() << "packet too short: " << bytes.size();
			return {};
		}

		size_t sizeField = (bytes[0] << 8) | bytes[1];
		EXPECT_EQ(bytes.size() - 6, sizeField);
		EXPECT_EQ(TS_RESPONSE_OK, bytes[2]);

		// CRC covers response code and payload
		uint32_t expectedCrc = crc32(&bytes[2], bytes.size() - 6);
		size_t crcPos = bytes.size() - 4;
		uint32_t actualCrc = (bytes[crcPos] << 24) | (bytes[crcPos + 1] << 16) | (bytes[crcPos + 2] << 8) | bytes[crcPos + 3];
		EXPECT_EQ(expectedCrc, actualCrc);

		return std::string(bytes.begin() + 3, bytes.end() - 4);
	}

	// Runs one loggingSendOutputData() round and returns the packet payload
	std::string send(size_t* returnedSize = nullptr) {
		bytes.clear();
		packetComplete = false;
		size_t size = loggingSendOutputData(this);
		if (returnedSize) {
			*returnedSize = size;
		}
		std::string result = payload();
		EXPECT_EQ(result.size(), size);
		return result;
	}

	std::vector<uint8_t> bytes;
	bool packetComplete = false;
};

// Externally owned buffer, as the engine sniffer uses
template <size_t bufferSize = 64>
struct TestOwnedBuffer : LogBuffer<bufferSize> {
	void free() override {
		freeCount++;
	}

	void fill(const char* text) {
		this->len = strlen(text);
		memcpy(this->buffer, text, this->len);
	}

	int freeCount = 0;
};

class LoggingCentral : public ::testing::Test {
protected:
	void SetUp() override {
		resetLoggingForUnitTest();
	}

	void TearDown() override {
		resetLoggingForUnitTest();
	}

	void checkPacketSizeThreshold(const char* boundaryText) {
		// External buffers reach even a custom board's large byte threshold
		// before the independent 16-buffer limit stops consolidation.
		std::string text(scratchBuffer_SIZE - 1, 'y');
		TestOwnedBuffer<scratchBuffer_SIZE> first;
		first.fill(text.c_str());
		TestOwnedBuffer<> boundary;
		boundary.fill(boundaryText);
		TestOwnedBuffer<> deferred;
		deferred.fill("later");

		loggingPostBuffer(&first);
		loggingPostBuffer(&boundary);
		loggingPostBuffer(&deferred);

		EXPECT_EQ(text + boundaryText, channel.send());
		EXPECT_EQ(1, first.freeCount);
		EXPECT_EQ(1, boundary.freeCount);
		EXPECT_EQ(0, deferred.freeCount);

		EXPECT_EQ("later", channel.send());
		EXPECT_EQ(1, deferred.freeCount);
		EXPECT_EQ("", channel.send());
		EXPECT_EQ(1, first.freeCount);
		EXPECT_EQ(1, boundary.freeCount);
		EXPECT_EQ(1, deferred.freeCount);
	}

	CaptureTsChannel channel;
};

} // namespace

TEST_F(LoggingCentral, emptyQueueSendsEmptyPacket) {
	size_t size = 1234;
	EXPECT_EQ("", channel.send(&size));
	EXPECT_EQ(0u, size);
	// size field counts the response code only
	EXPECT_EQ(7u, channel.bytes.size());
}

TEST_F(LoggingCentral, efiPrintfAddsFraming) {
	efiPrintf("hello %d %s", 42, "world");

	EXPECT_EQ(MSG_PREFIX "hello 42 world" LOG_DELIMITER, channel.send());

	// line was consumed
	EXPECT_EQ("", channel.send());
}

TEST_F(LoggingCentral, efiPrintfInternalRawFormat) {
	priv::efiPrintfInternal("raw%c%u", '-', 7u);

	EXPECT_EQ("raw-7", channel.send());
}

TEST_F(LoggingCentral, linesAreConsolidatedInOrder) {
	efiPrintf("first");
	efiPrintf("second");
	efiPrintf("third");

	EXPECT_EQ(
		MSG_PREFIX "first" LOG_DELIMITER
		MSG_PREFIX "second" LOG_DELIMITER
		MSG_PREFIX "third" LOG_DELIMITER,
		channel.send());
}

TEST_F(LoggingCentral, nonPrintableCharsReplacedWithSpace) {
	priv::efiPrintfInternal("a\tb\r\nc\x01" "d");

	EXPECT_EQ("a b  c d", channel.send());
}

// see https://github.com/rusefi/rusefi/issues/10159 and test_log_buffer.cpp
TEST_F(LoggingCentral, longLineTruncatedKeepsDelimiter) {
	std::string longText(500, 'x');
	efiPrintf("%s", longText.c_str());

	std::string sent = channel.send();
	ASSERT_EQ(lineSize - 1, sent.size());
	EXPECT_EQ(0u, sent.find(MSG_PREFIX "xxx"));
	EXPECT_EQ(LOG_DELIMITER[0], sent.back());
	EXPECT_EQ('x', sent[sent.size() - 2]);
}

TEST_F(LoggingCentral, atMostSixteenBuffersPerPacket) {
	for (size_t i = 0; i < maxBuffersPerPacket + 4; i++) {
		priv::efiPrintfInternal("%c", 'a' + (int)i);
	}

	EXPECT_EQ("abcdefghijklmnop", channel.send());
	EXPECT_EQ("qrst", channel.send());
	EXPECT_EQ("", channel.send());
}

// once the packet reaches scratchBuffer_SIZE no more buffers are added, but the
// buffer crossing the threshold is still sent whole
TEST_F(LoggingCentral, packetSizeThreshold) {
	checkPacketSizeThreshold("zz");
}

TEST_F(LoggingCentral, packetSizeThresholdExact) {
	checkPacketSizeThreshold("z");
}

TEST_F(LoggingCentral, linesDroppedWhenPoolExhausted) {
	for (size_t i = 0; i < lineCount + 10; i++) {
		priv::efiPrintfInternal("%c", 'A' + (int)(i % 26));
	}

	size_t total = 0;
	std::string all;
	for (int i = 0; i < 10; i++) {
		size_t size;
		all += channel.send(&size);
		total += size;
	}

	// only the first lineCount lines made it, later ones were dropped
	EXPECT_EQ(lineCount, total);
	EXPECT_EQ('A', all[0]);
	EXPECT_EQ('A' + (int)((lineCount - 1) % 26), all.back());
}

TEST_F(LoggingCentral, sentBuffersAreReleased) {
	// several times the pool size: only works if every sent line buffer returns to the free queue
	for (int round = 0; round < 5; round++) {
		for (size_t i = 0; i < lineCount; i++) {
			priv::efiPrintfInternal("z");
		}
		size_t total = 0;
		for (size_t i = 0; i < lineCount / maxBuffersPerPacket; i++) {
			size_t size;
			channel.send(&size);
			total += size;
		}
		EXPECT_EQ(lineCount, total) << "round " << round;
		EXPECT_EQ("", channel.send());
	}
}

TEST_F(LoggingCentral, externalBufferSentAndFreed) {
	TestOwnedBuffer<> owned;
	owned.fill("sniffer data");

	efiPrintf("before");
	loggingPostBuffer(&owned);
	efiPrintf("after");

	EXPECT_EQ(0, owned.freeCount);
	EXPECT_EQ(MSG_PREFIX "before" LOG_DELIMITER "sniffer data" MSG_PREFIX "after" LOG_DELIMITER, channel.send());
	EXPECT_EQ(1, owned.freeCount);

	EXPECT_EQ("", channel.send());
	EXPECT_EQ(1, owned.freeCount);
}

// only 'len' bytes are sent, not the whole storage
TEST_F(LoggingCentral, externalBufferSendsUsedBytesOnly) {
	TestOwnedBuffer<> owned;
	memset(owned.buffer, 'Q', sizeof(owned.buffer));
	owned.len = 3;

	loggingPostBuffer(&owned);

	EXPECT_EQ("QQQ", channel.send());
	EXPECT_EQ(1, owned.freeCount);
}
