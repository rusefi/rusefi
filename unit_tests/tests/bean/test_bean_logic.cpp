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
			if (m_currentByte == 0x7E || m_currentByte == 0x7C || m_currentByte == 0x7D) {
				// End of message
				printMessage();
				m_message.clear();
			}
			m_bitCount = 0;
			m_currentByte = 0;
		}
	}

	void printMessage() {
		// printf("BEAN MSG: ");
		// for (uint8_t b : m_message) {
		// 	printf("%02X ", b);
		// }
		// printf("\n");
	}

private:
	uint8_t m_currentByte = 0;
	int m_bitCount = 0;
	int m_stuffingCount = 0;
	bool m_lastBit = false;
	std::vector<uint8_t> m_message;
public:
	/**
	 * Total stream of all decoded bytes, including EOM markers and any noise/preamble.
	 */
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
		int bitCount = (int)((duration + 0.00003) / 0.0001); // 100us per bit, with small offset for rounding
		if (bitCount > 50) bitCount = 1;

		// printf("Duration: %f, bitCount: %d, value: %d\n", duration, bitCount, lastValue);

		for (int i = 0; i < bitCount; i++) {
			decoder.processBit(lastValue);
		}

		lastTimestamp = timestamp;
		lastValue = value;
	}

	printf("Total bytes decoded: %zu\n", decoder.m_allBytes.size());

	// Helper to find a sequence of bytes in the total decoded stream (m_allBytes)
	// Returns the index of the first byte of the sequence, or -1 if not found.
	auto findSequence = [&](const std::vector<uint8_t>& seq) {
		for (size_t i = 0; i <= decoder.m_allBytes.size() - seq.size(); i++) {
			bool match = true;
			for (size_t j = 0; j < seq.size(); j++) {
				if (decoder.m_allBytes[i + j] != seq[j]) {
					match = false;
					break;
				}
			}
			if (match) return (int)i;
		}
		return -1;
	};

	// The user mentions these repeating over and over:
	// 01 63 13 D7 20 18 7E 00
	// 01 63 13 E4 10 D2 7E 00

	// Let's check for these. Note: sometimes the first bit of 01 might be merged with previous 7E if there's no gap.
	// Or my decoder might have them slightly shifted.

	int pos1 = findSequence({0x63, 0x13, 0xE4, 0x10, 0xD2});
	int pos2 = findSequence({0x63, 0x13, 0xD7, 0x20, 0x18});

	EXPECT_EQ(pos1, 29);
	EXPECT_EQ(pos2, 83);

	ASSERT_EQ(decoder.m_allBytes.size(), 786);

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
	for (int i = 0; i < 100 && i < (int)decoder.m_allBytes.size(); i++) {
		printf("Data at index %d: %02X\n", i, decoder.m_allBytes[i]);
	}
}
