#include "pch.h"
#include "logicdata_csv_reader.h"
#include <vector>

/**
 * Toyota BEAN (Body Electronics Area Network) protocol decoder.
 *
 * BEAN is a single-wire bus running at 10 kbit/s (100us per bit).
 * The protocol uses bit-stuffing (after 5 consecutive same-value bits,
 * a complementary stuff bit is inserted) and marks end-of-message with
 * specific byte values.
 *
 * This decoder processes individual bits from a logic analyzer capture,
 * handles de-stuffing, assembles bytes, and detects message boundaries.
 */
class BeanDecoder {
public:
	/**
	 * Feed a CSV edge into the decoder. Computes how many bit periods
	 * elapsed since the last edge, then calls processBit() for each one.
	 *
	 * @param timestamp  absolute time in seconds of this edge
	 * @param value      logic level (true = high) at this edge
	 */
	void processEdge(double timestamp, bool value) {
		if (!m_hasFirstEdge) {
			m_lastTimestamp = timestamp;
			m_lastValue = value;
			m_hasFirstEdge = true;
			return;
		}

		double duration = timestamp - m_lastTimestamp;
		// 100us per bit, with small offset (30us) to compensate for rounding
		// in logic analyzer sampling
		int bitCount = (int)((duration + 0.00003) / 0.0001);
		// Cap unreasonably long gaps (idle bus) to a single bit
		if (bitCount > 50) bitCount = 1;

		for (int i = 0; i < bitCount; i++) {
			processBit(m_lastValue);
		}

		m_lastTimestamp = timestamp;
		m_lastValue = value;
	}

	/**
	 * Total stream of all decoded bytes, including EOM markers and any noise/preamble.
	 * Used by tests to verify the full decoded output.
	 */
	std::vector<uint8_t> m_allBytes;

private:
	/**
	 * Process a single decoded bit through the BEAN protocol state machine.
	 *
	 * Handles bit-stuffing removal: after 5 consecutive bits of the same
	 * polarity, the next bit is a stuff bit inserted by the transmitter
	 * and must be discarded (not shifted into the byte register).
	 *
	 * Once 8 data bits are accumulated, the resulting byte is stored and
	 * checked for end-of-message markers.
	 */
	void processBit(bool bit) {
		if (m_stuffingCount == 5) {
			// After 5 consecutive same-value bits, this bit is a stuff bit
			// inserted by the transmitter for clock recovery — discard it
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
			/**
			 * 0x7E is the canonical BEAN End-of-Message (EOM) marker.
			 * 0x7C and 0x7D are also accepted because logic analyzer
			 * bit-sampling jitter can cause the last 1-2 bits of the
			 * EOM byte to be misread (off-by-one in the LSBs).
			 * All three share the upper 6 bits 0b011111xx.
			 */
			if (m_currentByte == 0x7E || m_currentByte == 0x7C || m_currentByte == 0x7D) {
				// End of message boundary detected
				m_message.clear();
			}
			m_bitCount = 0;
			m_currentByte = 0;
		}
	}

	// Bit assembly state
	uint8_t m_currentByte = 0;
	int m_bitCount = 0;

	// Bit-stuffing tracking
	int m_stuffingCount = 0;
	bool m_lastBit = false;

	// Current message accumulator (reset on EOM)
	std::vector<uint8_t> m_message;

	// Edge timing state
	double m_lastTimestamp = 0;
	bool m_lastValue = false;
	bool m_hasFirstEdge = false;
};

TEST(bean, mr2_cluster) {
	CsvReader reader(1, /* vvtCount */ 0);
	reader.open("tests/bean/resources/mr2_cluster_bean_bench1.csv");

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	BeanDecoder decoder;

	while (reader.haveMore()) {
		double values[1];
		double timestamp = reader.readTimestampAndValues(values);
		bool value = values[0] > 0.5;
		decoder.processEdge(timestamp, value);
	}

	printf("Total bytes decoded: %zu\n", decoder.m_allBytes.size());

	// Helper to find a sequence of bytes in the total decoded stream (m_allBytes).
	// Returns the index of the first occurrence, or -1 if not found.
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

	// The datalog contains these repeating BEAN messages:
	// 01 63 13 D7 20 18 7E 00
	// 01 63 13 E4 10 D2 7E 00

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
