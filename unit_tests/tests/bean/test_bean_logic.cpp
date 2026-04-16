#include "pch.h"
#include <cstdio>
#include "logicdata_csv_reader.h"
#include "bean_decoder.h"
#include "bean_encoder.h"

/**
 * Assert that the first 127 bytes in the cyclic buffer match the expected
 * BEAN protocol decode from mr2_cluster_bean_bench1.csv.
 */
static void assertFirst127Bytes(cyclic_buffer<uint8_t, BEAN_ALL_BYTES_BUFFER_SIZE>& bytes) {
	ASSERT_GE(bytes.getCount(), 127);
	static const uint8_t expected[127] = {
		0x58, 0xC4, 0xF5, 0xC8, 0x06, 0x1F, 0x02, 0xC6, 0x27, 0xAE,
		0x40, 0x30, 0xF8, 0x16, 0x31, 0x3D, 0x72, 0x01, 0x87, 0xC0,
		0x00, 0x00, 0x0C, 0x62, 0x7C, 0x82, 0x1A, 0x4F, 0x81, 0x63,
		0x13, 0xE4, 0x10, 0xD2, 0x7C, 0x0B, 0x18, 0x9F, 0x20, 0x86,
		0x93, 0xEB, 0x18, 0x9E, 0xB9, 0x00, 0xC3, 0xE0, 0x58, 0xC4,
		0xF5, 0xC8, 0x06, 0x1F, 0x02, 0xC6, 0x27, 0xAE, 0x40, 0x30,
		0xF8, 0x00, 0x00, 0x01, 0x8C, 0x4F, 0x90, 0x43, 0x49, 0xF0,
		0x2C, 0x62, 0x7C, 0x82, 0x1A, 0x4F, 0x81, 0x63, 0x13, 0xE4,
		0x10, 0xD2, 0x7D, 0x63, 0x13, 0xD7, 0x20, 0x18, 0x7C, 0x0B,
		0x18, 0x9E, 0xB9, 0x00, 0xC3, 0xE0, 0x58, 0xC4, 0xF5, 0xC8,
		0x06, 0x1F, 0x00, 0x00, 0x00, 0x31, 0x89, 0xF2, 0x08, 0x69,
		0x3E, 0x05, 0x8C, 0x4F, 0x90, 0x43, 0x49, 0xF0, 0x2C, 0x62,
		0x7C, 0x82, 0x1A, 0x4F, 0xAC, 0x62, 0x7A,
	};
	for (int i = 0; i < 127; i++) {
		EXPECT_EQ(bytes.get(i), expected[i]) << "Mismatch at index " << i;
	}
}

TEST(bean, mr2_cluster) {
	CsvReader reader(1, /* vvtCount */ 0);
	reader.open("tests/bean/resources/mr2_cluster_bean_bench1.csv");

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	BeanDecoder decoder;

	// Track when we first accumulate 127 bytes to assert the initial decode
	bool assertedFirst127 = false;

	while (reader.haveMore()) {
		double values[1];
		double timestamp = reader.readTimestampAndValues(values);
		bool value = values[0] > 0.5;
		decoder.processEdge(timestamp, value);

		if (!assertedFirst127 && decoder.m_totalBytesDecoded >= 127) {
			assertFirst127Bytes(decoder.m_allBytes);
			assertedFirst127 = true;
		}
	}

	printf("Total bytes decoded: %zu\n", decoder.m_totalBytesDecoded);

	ASSERT_EQ(decoder.m_totalBytesDecoded, 786);
}

/**
 * Encoder roundtrip test: encode a known byte sequence into a CSV file,
 * then decode it back through BeanDecoder and verify the bytes match.
 */
TEST(bean, encoder_roundtrip) {
	static const uint8_t testData[] = {
		0x58, 0xC4, 0xF5, 0xC8, 0x06, 0x1F, 0x02, 0xC6, 0x27, 0xAE
	};
	static const int testLen = sizeof(testData) / sizeof(testData[0]);

	// Write encoded edges to a temporary CSV file
	const char* csvPath = "tests/bean/resources/bean_encoder_test.csv";
	FILE* csvFile = fopen(csvPath, "w");
	ASSERT_NE(csvFile, nullptr) << "Failed to create " << csvPath;
	fprintf(csvFile, "Time [s],Channel 0\n");
	// Initial idle state far enough before data that the decoder's
	// >50 bit cap treats the gap as a single bit (like real captures)
	fprintf(csvFile, "0.000000000,0\n");

	struct CsvWriter {
		FILE* file;
	};
	CsvWriter writer{csvFile};

	BeanEncoder encoder;
	encoder.reset(0.010);  // start encoding at 10ms (large gap triggers cap)
	encoder.setEdgeCallback([](double ts, bool level, void* ud) {
		CsvWriter* w = static_cast<CsvWriter*>(ud);
		fprintf(w->file, "%.9f,%d\n", ts, level ? 1 : 0);
	}, &writer);

	encoder.encodeBytes(testData, testLen);

	// Emit a trailing edge so the decoder can flush the last bit group
	double endTs = encoder.getNextTimestamp() + BeanEncoder::BIT_PERIOD;
	fprintf(csvFile, "%.9f,%d\n", endTs, encoder.getCurrentLevel() ? 0 : 1);

	fclose(csvFile);

	printf("Encoder produced %d edges\n", encoder.getEdgeCount());

	// Now decode the generated CSV back
	CsvReader reader(1, /* vvtCount */ 0);
	reader.open(csvPath);

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	BeanDecoder decoder;
	while (reader.haveMore()) {
		double values[1];
		double timestamp = reader.readTimestampAndValues(values);
		bool value = values[0] > 0.5;
		decoder.processEdge(timestamp, value);
	}

	printf("Roundtrip decoded %zu bytes\n", decoder.m_totalBytesDecoded);
	ASSERT_EQ(decoder.m_totalBytesDecoded, (size_t)testLen);

	for (int i = 0; i < testLen; i++) {
		EXPECT_EQ(decoder.m_allBytes.get(i), testData[i])
			<< "Mismatch at byte " << i;
	}
}
