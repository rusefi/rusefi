#include "mlg_field.h"
#include "buffered_writer.h"
#include <stdexcept>
#include <string>
#include "mlg_reader.h"
#include "msl_to_csv.h"
#include "unit_test_logger.h"

#include <gmock/gmock.h>
#include <filesystem>
#include <fstream>

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
		// Translation defaults to 0
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
	EXPECT_EQ(4u, lf.writeData(buffer));

	// Check that big endian data was written, and bytes after weren't touched
	EXPECT_THAT(buffer, ElementsAre(0x00, 0xbc, 0x61, 0x4e, 0xAA, 0xAA));
}

TEST(BinaryLogField, EnumValues) {
	enum class UnsignedState : uint8_t { Active = 0xa5 };
	enum class SignedState : int16_t { Error = -2 };
	UnsignedState unsignedState = UnsignedState::Active;
	SignedState signedState = SignedState::Error;
	Field unsignedField(unsignedState, "Unsigned state", "", 0);
	Field signedField(signedState, "Signed state", "", 0);

	EXPECT_EQ(static_cast<uint8_t>(Types::Field::Scalar::U08), unsignedField.getTypeId());
	EXPECT_EQ(static_cast<uint8_t>(Types::Field::Scalar::S16), signedField.getTypeId());
	EXPECT_EQ(1u, unsignedField.getSize());
	EXPECT_EQ(2u, signedField.getSize());
	EXPECT_DOUBLE_EQ(0xa5, unsignedField.readValueAsDouble());
	EXPECT_DOUBLE_EQ(-2, signedField.readValueAsDouble());

	char unsignedData[1];
	char signedData[2];
	EXPECT_EQ(1u, unsignedField.writeData(unsignedData));
	EXPECT_EQ(2u, signedField.writeData(signedData));
	EXPECT_EQ(0xa5, static_cast<uint8_t>(unsignedData[0]));
	EXPECT_EQ(0xff, static_cast<uint8_t>(signedData[0]));
	EXPECT_EQ(0xfe, static_cast<uint8_t>(signedData[1]));
}

TEST(BinaryLogField, FloatDisplayScale) {
	float grams = 0.125f;
	Field field(grams, "Fuel mass", "mg", 3, "none", 1000);
	// [tag:live_data_output_scale] The log reader converts grams to mg using the header, not payload writes.
	EXPECT_FLOAT_EQ(1000, field.getMultiplier());
	EXPECT_DOUBLE_EQ(125, field.readValueAsDouble());
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
	EXPECT_EQ(sizeof(float), field.writeData(data));
	EXPECT_THAT(data, ElementsAre(0x3e, 0, 0, 0));
	EXPECT_FLOAT_EQ(0.125f, grams);
}

static float readBigEndianFloat(const char* bytes) {
	uint32_t bits = 0;
	for (size_t i = 0; i < sizeof(bits); i++) {
		bits = (bits << 8) | static_cast<unsigned char>(bytes[i]);
	}
	float value;
	memcpy(&value, &bits, sizeof(value));
	return value;
}

TEST(BinaryLogField, FloatTranslation) {
	float trim = 1;
	Field field(trim, "Trim", "%", 1, "Fuel", 100, -1);
	char header[Types::Field::DescriptorSize];
	StrictMock<MockWriter> writer;
	EXPECT_CALL(writer, write(_, sizeof(header))).WillOnce([&](const char* buffer, size_t count) {
		memcpy(header, buffer, count);
		return count;
	});
	field.writeHeader(writer);
	EXPECT_FLOAT_EQ(100, readBigEndianFloat(header + 46));
	// [tag:live_data_output_scale] Decode emitted bytes to verify shift-before-scale and raw payloads.
	EXPECT_FLOAT_EQ(-1, readBigEndianFloat(header + 50));
	for (float raw : {0.95f, 1.0f, 1.05f}) {
		trim = raw;
		char data[sizeof(float)];
		ASSERT_EQ(sizeof(data), field.writeData(data));
		EXPECT_FLOAT_EQ(raw, readBigEndianFloat(data));
		double expected = (static_cast<double>(raw) - 1) * 100;
		EXPECT_NEAR(expected, (readBigEndianFloat(data) + readBigEndianFloat(header + 50))
			* readBigEndianFloat(header + 46), 0.00001);
		EXPECT_NEAR(expected, field.readValueAsDouble(), 0.00001);
	}
}

TEST(BinaryLogField, PackedTranslation) {
	scaled_channel<int16_t, 10> packed;
	Field field(packed, "Packed", "unit", 1, "none", 1, 100);
	char header[Types::Field::DescriptorSize];
	StrictMock<MockWriter> writer;
	EXPECT_CALL(writer, write(_, sizeof(header))).WillOnce([&](const char* buffer, size_t count) {
		memcpy(header, buffer, count);
		return count;
	});
	field.writeHeader(writer);
	EXPECT_FLOAT_EQ(0.1f, readBigEndianFloat(header + 46));
	EXPECT_FLOAT_EQ(100, readBigEndianFloat(header + 50));
	for (int raw : {-20, 0, 20}) {
		packed = raw / 10.0f;
		char data[sizeof(int16_t)];
		ASSERT_EQ(sizeof(data), field.writeData(data));
		int decodedRaw = (static_cast<unsigned char>(data[0]) << 8) | static_cast<unsigned char>(data[1]);
		if (decodedRaw >= 0x8000) {
			decodedRaw -= 0x10000;
		}
		EXPECT_EQ(raw, decodedRaw);
		double expected = (raw + 100) / 10.0;
		EXPECT_NEAR(expected, (decodedRaw + readBigEndianFloat(header + 50))
			* readBigEndianFloat(header + 46), 0.00001);
		EXPECT_NEAR(expected, field.readValueAsDouble(), 0.00001);
	}
}

TEST(BinaryLogField, ReadersApplyTranslation) {
	// Independent on-disk fixture: float trim with shift -1, packed integer with shift +100.
	const auto path = std::filesystem::path(TEST_RESULTS_DIR) / "output_offset_readers.mlg";
	const auto csvPath = path.string() + ".csv";
	std::filesystem::create_directories(path.parent_path());
	std::ofstream file(path, std::ios::binary);
	ASSERT_TRUE(file.is_open());
	const unsigned char fileHeader[24] = {'M', 'L', 'V', 'L', 'G', 0, 0, 2,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 202, 0, 6, 0, 2};
	file.write(reinterpret_cast<const char*>(fileHeader), sizeof(fileHeader));
	float trim = 1.05f;
	scaled_channel<int16_t, 10> packed = 2.0f;
	Field fields[] = {{trim, "Trim", "%", 1, "Fuel", 100}, {packed, "Packed", "unit", 1}};
	const unsigned char shifts[2][4] = {{0xbf, 0x80, 0, 0}, {0x42, 0xc8, 0, 0}};
	char payload[6];
	size_t position = 0;
	for (size_t i = 0; i < 2; i++) {
		StrictMock<MockWriter> writer;
		EXPECT_CALL(writer, write(_, 89)).WillOnce([&](const char* buffer, size_t count) {
			char header[89];
			memcpy(header, buffer, count);
			memcpy(header + 50, shifts[i], 4);
			file.write(header, count);
			return count;
		});
		fields[i].writeHeader(writer);
		position += fields[i].writeData(payload + position);
	}
	ASSERT_EQ(sizeof(payload), position);
	const char recordHeader[4] = {};
	file.write(recordHeader, sizeof(recordHeader));
	file.write(payload, sizeof(payload));
	uint8_t checksum = 0;
	for (char byte : payload) {
		checksum += static_cast<unsigned char>(byte);
	}
	file.put(static_cast<char>(checksum));
	file.close();
	{
		BinarySensorReader reader;
		reader.openMlg(path.string());
		auto& values = reader.readBlock();
		EXPECT_NEAR(5, values.at("Trim"), 0.00001);
		EXPECT_NEAR(12, values.at("Packed"), 0.00001);
	}
	ASSERT_TRUE(convertMslToCsv(path.string().c_str(), csvPath.c_str()));
	{
		std::ifstream csv(csvPath);
		std::string line;
		ASSERT_TRUE(static_cast<bool>(std::getline(csv, line)));
		EXPECT_EQ("Trim (%),Packed (unit)", line);
		ASSERT_TRUE(static_cast<bool>(std::getline(csv, line)));
		EXPECT_EQ("5.0,12.0", line);
	}
	std::filesystem::remove(path);
	std::filesystem::remove(csvPath);
}
