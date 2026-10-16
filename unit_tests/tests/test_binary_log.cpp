#include "mlg_field.h"
#include "buffered_writer.h"

#include <gmock/gmock.h>

using namespace MLG;
using namespace MLG::Entries;

using ::testing::_;
using ::testing::ElementsAre;
using ::testing::StrictMock;

class MockWriter : public Writer {
public:
	MOCK_METHOD(size_t, write, (const char* buffer, size_t count), (override));
	MOCK_METHOD(size_t, flush, (), (override));
};

TEST(BinaryLogField, FieldHeader) {
	scaled_channel<int8_t, 10> channel;
	Field field(channel, "name", "units", 2, "category");

	char buffer[89];
	StrictMock<MockWriter> bufWriter;
	EXPECT_CALL(bufWriter, write(_, 89))
		.WillOnce([&] (const char* buf, size_t count) {
			memcpy(buffer, buf, count);
			return 0;
		});

	// Should write 89 bytes
	field.writeHeader(bufWriter);

	// Expect correctly written header
	EXPECT_THAT(buffer, ElementsAre(
		1,	// type: int8_t
		// name - 34 bytes, 0 padded
		'n', 'a', 'm', 'e', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		// units - 10 bytes, 0 padded
		'u', 'n', 'i', 't', 's', 0, 0, 0, 0, 0,
		// display style: float
		0,
		// Scale = 0.1 (float)
		0x3d, 0xcc, 0xcc, 0xcd,
		// Transform - we always use 0
		0, 0, 0, 0,
		// Digits - 2, as configured
		2,
		'c', 'a', 't', 'e', 'g', 'o', 'r', 'y', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	));
}

TEST(BinaryLogField, Value) {
	scaled_channel<uint32_t, 1> testValue = 12345678;
	Field lf(testValue, "test", "unit", 0);

	char buffer[6];
	memset(buffer, 0xAA, sizeof(buffer));

	// Should write 4 bytes
	EXPECT_EQ(4u, lf.writeData(buffer, nullptr));

	// Check that big endian data was written, and bytes after weren't touched
	EXPECT_THAT(buffer, ElementsAre(0x00, 0xbc, 0x61, 0x4e, 0xAA, 0xAA));
}

TEST(BinaryLogField, FloatDisplayScale) {
	float grams = 0.125f;
	Field field(grams, "Fuel mass", "mg", 3, "none", 1000);
	// [tag:live_data_output_scale] The log reader converts grams to mg using the header, not payload writes.
	EXPECT_FLOAT_EQ(1000, field.getMultiplier());
	EXPECT_DOUBLE_EQ(125, field.readValueAsDouble(nullptr));
	StrictMock<MockWriter> writer;
	EXPECT_CALL(writer, write(_, 89)).WillOnce([](const char* buffer, size_t count) {
		EXPECT_EQ(0x44, static_cast<unsigned char>(buffer[46]));
		EXPECT_EQ(0x7a, static_cast<unsigned char>(buffer[47]));
		EXPECT_EQ(0, buffer[48]);
		EXPECT_EQ(0, buffer[49]);
		return count;
	});
	field.writeHeader(writer);
	char data[sizeof(float)];
	EXPECT_EQ(sizeof(float), field.writeData(data, nullptr));
	EXPECT_THAT(data, ElementsAre(0x3e, 0, 0, 0));
	EXPECT_FLOAT_EQ(0.125f, grams);
}
