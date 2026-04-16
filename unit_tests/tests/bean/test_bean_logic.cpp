#include "pch.h"
#include "logicdata_csv_reader.h"
#include <vector>

// Toyota BEAN protocol decoder skeleton
class BeanDecoder {
public:
	void processBit(bool bit) {
		if (m_stuffingCount == 5) {
			// This is a stuffing bit, verify it's the opposite of previous bits
			// But for now, just skip it
			m_stuffingCount = 1;
			m_lastBit = bit;
			return;
		}

		if (bit == m_lastBit) {
			m_stuffingCount++;
		} else {
			m_stuffingCount = 1;
			m_lastBit = bit;
		}

		m_currentByte = (m_currentByte << 1) | (bit ? 1 : 0);
		m_bitCount++;

		if (m_bitCount == 8) {
			m_allBytes.push_back(m_currentByte);
			m_message.push_back(m_currentByte);
			if (m_currentByte == 0x7E) {
				// End of message
				printMessage();
				m_message.clear();
			}
			m_bitCount = 0;
			m_currentByte = 0;
		}
	}

	void printMessage() {
		printf("BEAN MSG: ");
		for (uint8_t b : m_message) {
			printf("%02X ", b);
		}
		printf("\n");
	}

private:
	uint8_t m_currentByte = 0;
	int m_bitCount = 0;
	int m_stuffingCount = 0;
	bool m_lastBit = false;
	std::vector<uint8_t> m_message;
public:
	std::vector<uint8_t> m_allBytes;
};

TEST(bean, mr2_cluster) {
	CsvReader reader(1, /* vvtCount */ 0);
	reader.open("tests/bean/resources/mr2_cluster_bean_bench1.csv");

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	BeanDecoder decoder;
	double lastTimestamp = 0;
	bool lastValue = false;

	if (reader.haveMore()) {
		double values[1];
		lastTimestamp = reader.readTimestampAndValues(values);
		lastValue = values[0] > 0.5;
	}

	while (reader.haveMore()) {
		double values[1];
		double timestamp = reader.readTimestampAndValues(values);
		bool value = values[0] > 0.5;

		double duration = timestamp - lastTimestamp;
		int bitCount = (int)((duration + 0.00005) / 0.0001); // 100us per bit
		if (bitCount > 50) bitCount = 1;

		// printf("Duration: %f, bitCount: %d, value: %d\n", duration, bitCount, lastValue);

		for (int i = 0; i < bitCount; i++) {
			decoder.processBit(lastValue);
		}

		lastTimestamp = timestamp;
		lastValue = value;
	}

	printf("Total bytes decoded: %zu\n", decoder.m_allBytes.size());
	ASSERT_GE(decoder.m_allBytes.size(), 12U);
	EXPECT_EQ(decoder.m_allBytes[0], 0x58);
	EXPECT_EQ(decoder.m_allBytes[1], 0xC4);
	EXPECT_EQ(decoder.m_allBytes[2], 0xF5);
	EXPECT_EQ(decoder.m_allBytes[3], 0xC8);
	EXPECT_EQ(decoder.m_allBytes[4], 0x06);
	EXPECT_EQ(decoder.m_allBytes[5], 0x1F);
	EXPECT_EQ(decoder.m_allBytes[6], 0x02);
	EXPECT_EQ(decoder.m_allBytes[7], 0xC6);
	EXPECT_EQ(decoder.m_allBytes[8], 0x27);
	EXPECT_EQ(decoder.m_allBytes[9], 0xAE);
	EXPECT_EQ(decoder.m_allBytes[10], 0x40);
	EXPECT_EQ(decoder.m_allBytes[11], 0x30);
}
