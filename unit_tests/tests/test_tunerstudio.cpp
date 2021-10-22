#include "pch.h"
#include "tunerstudio_io.h"

static uint8_t st5TestBuffer[16000];

class MockTsChannel : public TsChannelBase {
public:
	MockTsChannel() : TsChannelBase("Test") { }

	void write(const uint8_t* buffer, size_t size, bool /*isLastWriteInTransaction*/) override {
		memcpy(&st5TestBuffer[writeIdx], buffer, size);
		writeIdx += size;
	}

	size_t readTimeout(uint8_t* buffer, size_t size, int timeout) override {
		// nothing to do here
		return size;
	}

	void reset() {
		writeIdx = 0;
	}

	size_t writeIdx = 0;
};

#define CODE 2
#define PAYLOAD "123"
#define SIZE strlen(PAYLOAD)

static void assertCrcPacket(MockTsChannel& dut) {
	ASSERT_EQ(dut.writeIdx, SIZE + 7);

	// todo: proper uint16 comparison
	ASSERT_EQ(st5TestBuffer[0], 0);
	ASSERT_EQ(st5TestBuffer[1], SIZE + 1);

	ASSERT_EQ(st5TestBuffer[2], CODE);

	ASSERT_EQ(memcmp(&st5TestBuffer[3], PAYLOAD, SIZE), 0);


	// todo: proper uint32 comparison
	ASSERT_EQ(st5TestBuffer[6], 252);
	ASSERT_EQ(st5TestBuffer[7], 68);
	ASSERT_EQ(st5TestBuffer[8], 173);
	ASSERT_EQ(st5TestBuffer[9], 87);
}

TEST(binary, testWriteCrc) {
	MockTsChannel test;

	// Let it pick which impl (small vs large) to use
	test.reset();
	test.writeCrcPacket(CODE, (const uint8_t*)PAYLOAD, SIZE);
	assertCrcPacket(test);

	// Force the large impl
	test.reset();
	test.writeCrcPacket(CODE, (const uint8_t*)PAYLOAD, SIZE);
	assertCrcPacket(test);

	// Force the small impl
	test.reset();
	test.writeCrcPacket(CODE, (const uint8_t*)PAYLOAD, SIZE);
	assertCrcPacket(test);
}
