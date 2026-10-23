#include "pch.h"

#include "binary_mlg_logging.h"
#include "mlg_field.h"
#include "mlg_reader.h"
#include "msl_to_csv.h"
#include "unit_test_logger.h"
#include "value_lookup.h"

#include <filesystem>
#include <fstream>
#include <set>
#include <string>
#include <vector>

// [tag:mlg_open_issue_reproductions] These tests intentionally assert current
// defects/limitations in docs/mlg-logging-fixes.md. Change the expectations when
// fixing each issue; do not treat the observed behavior as the desired contract.
namespace {

class CaptureWriter : public Writer {
public:
	std::string bytes;
	size_t write(const char* buffer, size_t count) override {
		bytes.append(buffer, count);
		return count;
	}
	size_t flush() override { return 0; }
};

uint32_t readUnsignedBE(const char* bytes, size_t size) {
	uint32_t value = 0;
	for (size_t i = 0; i < size; i++) {
		value = (value << 8) | static_cast<unsigned char>(bytes[i]);
	}
	return value;
}

void putUnsignedBE(std::string& bytes, size_t offset, uint32_t value, size_t size) {
	for (size_t i = 0; i < size; i++) {
		bytes[offset + size - 1 - i] = static_cast<char>(value & 0xff);
		value >>= 8;
	}
}

struct EncodedField {
	uint8_t type;
	const char* name;
	std::vector<uint8_t> sample;
};

std::filesystem::path fixturePath() {
	std::filesystem::create_directories(TEST_RESULTS_DIR);
	const auto* info = ::testing::UnitTest::GetInstance()->current_test_info();
	return std::filesystem::path(TEST_RESULTS_DIR) / (std::string(info->test_case_name())
		+ "_" + info->name() + ".mlg");
}

void writeFixture(const std::filesystem::path& path, const std::vector<EncodedField>& fields) {
	// Encode independently of Field, MlgDataType and either reader's size table.
	std::string payload;
	for (const auto& field : fields) {
		for (uint8_t byte : field.sample) {
			payload.push_back(static_cast<char>(byte));
		}
	}
	std::string bytes(24, '\0');
	bytes.replace(0, 5, "MLVLG");
	bytes[7] = 2;
	putUnsignedBE(bytes, 16, 24 + 89 * fields.size(), 4);
	putUnsignedBE(bytes, 20, payload.size(), 2);
	putUnsignedBE(bytes, 22, fields.size(), 2);
	for (const auto& field : fields) {
		std::string descriptor(89, '\0');
		descriptor[0] = field.type;
		descriptor.replace(1, strlen(field.name), field.name);
		putUnsignedBE(descriptor, 46, 0x3f800000, 4); // Scale 1.0, translation 0.
		descriptor[54] = field.type == 7 ? 1 : 0;
		bytes += descriptor;
	}
	uint8_t checksum = 0;
	for (unsigned char byte : payload) {
		checksum += byte;
	}
	for (int i = 0; i < 2; i++) {
		bytes.append(4, '\0');
		bytes[bytes.size() - 3] = i;
		bytes += payload;
		bytes.push_back(static_cast<char>(checksum));
	}
	std::ofstream file(path, std::ios::binary);
	ASSERT_TRUE(file.is_open());
	file.write(bytes.data(), bytes.size());
	file.close();
	ASSERT_TRUE(file.good());
}

void expectCsv(const std::filesystem::path& path, const char* expectedHeader, const char* expectedRow) {
	const std::string csvPath = path.string() + ".csv";
	ASSERT_TRUE(convertMslToCsv(path.string().c_str(), csvPath.c_str()));
	std::ifstream csv(csvPath);
	std::string line;
	ASSERT_TRUE(static_cast<bool>(std::getline(csv, line)));
	EXPECT_EQ(expectedHeader, line);
	for (int i = 0; i < 2; i++) {
		ASSERT_TRUE(static_cast<bool>(std::getline(csv, line)));
		EXPECT_EQ(expectedRow, line);
	}
	EXPECT_FALSE(static_cast<bool>(std::getline(csv, line)));
}

} // namespace

TEST(MlgOpenIssues, SdHeaderOmitsCaptureTimestamp) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	// Issue 4: seed RTC telemetry, not hardware RTC (unavailable in host tests).
	// Even with a nonzero epoch in the emitted sample, each new header says zero.
	for (uint32_t epoch : {0u, 1710000000u, 1710000060u}) {
		SCOPED_TRACE(epoch);
		engine->outputChannels.rtcUnixEpochTime = epoch;
		CaptureWriter header;
		MLG::writeFileHeader(header);
		ASSERT_GE(header.bytes.size(), 24u);
		EXPECT_EQ(0u, readUnsignedBE(header.bytes.data() + 8, 4));
		CaptureWriter record;
		MLG::writeSdBlock(record);
		size_t offset = 4;
		int matches = 0;
		MLG::forEachField([&](const MLG::Entries::Field& field) {
			if (std::string(field.getName()) == "rtcUnixEpochTime") {
				matches++;
				ASSERT_EQ(4u, field.getSize());
				ASSERT_LE(offset + 4, record.bytes.size());
				EXPECT_EQ(epoch, readUnsignedBE(record.bytes.data() + offset, 4));
			}
			offset += field.getSize();
		});
		EXPECT_EQ(1, matches);
	}
}

TEST(MlgOpenIssues, SdHeaderHasNoStartingTuneBlock) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	CaptureWriter header;
	MLG::writeFileHeader(header);
	ASSERT_GE(header.bytes.size(), 24u);
	// Issue 10 is a feature-parity decision: currently there is no information
	// block or space for an embedded tune between descriptors and the first row.
	EXPECT_EQ(0u, readUnsignedBE(header.bytes.data() + 12, 4));
	size_t dataOffset = readUnsignedBE(header.bytes.data() + 16, 4);
	size_t fieldCount = readUnsignedBE(header.bytes.data() + 22, 2);
	EXPECT_GT(fieldCount, 0u);
	EXPECT_EQ(24 + 89 * fieldCount, dataOffset);
	EXPECT_EQ(header.bytes.size(), dataOffset);
}

TEST(MlgOpenIssues, SdIncludesLtftAndMisfireFields) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	CaptureWriter header;
	MLG::writeFileHeader(header);
	ASSERT_GE(header.bytes.size(), 24u);
	size_t fieldCount = readUnsignedBE(header.bytes.data() + 22, 2);
	ASSERT_GE(header.bytes.size(), 24 + 89 * fieldCount);
	std::set<std::string> names;
	for (size_t i = 0; i < fieldCount; i++) {
		std::string name = header.bytes.substr(24 + 89 * i + 1, 34);
		names.insert(name.substr(0, name.find('\0')));
	}
	EXPECT_EQ(1u, names.count("STFT: Bank 1"));
	EXPECT_EQ(1u, names.count("STFT: Bank 2"));
	// LTFT source wiring exposes both bank corrections and the learning counter.
	for (const char* present : {"LTFT: Bank 1", "LTFT: Bank 2", "LTFT: learning: hits"}) {
		EXPECT_EQ(1u, names.count(present)) << present;
	}
	// Mockable module fields are available in host logs as well as firmware.
	for (const char* present : {"Misfire: total count", "Misfire: EMA baseline"}) {
		EXPECT_EQ(1u, names.count(present)) << present;
	}
}

TEST(MlgOpenIssues, LuaIncludesLtftCounterButOmitsIteratedTrims) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engine->module<LongTermFuelTrim>()->ltftCntHit = 42;
	engine->module<LongTermFuelTrim>()->ltftCorrection[0] = 1.05f;
	engine->engineState.stftCorrection[0] = 0.95f;
	engine->module<MisfireController>().unmock().misfireEmaUs = 1250;
	EXPECT_NEAR(1.25f, getOutputValueByName("misfireEmaUs"), 0.00001f);
	EXPECT_FLOAT_EQ(42.0f, getOutputValueByName("ltftCntHit"));
	// Issue 6: iterated LTFT and STFT arrays are still excluded from Lua lookup.
	for (const char* missing : {"ltftCorrection1", "ltftCorrection2",
		"stftCorrection1", "stftCorrection2"}) {
		EXPECT_FLOAT_EQ(static_cast<float>(EFI_ERROR_CODE), getOutputValueByName(missing)) << missing;
	}
}

TEST(MlgOpenIssues, LegacyReaderSwapsIntegerSignedness) {
	const auto path = fixturePath();
	writeFixture(path, {
		{0, "u8", {0xc8}},                          // 200
		{1, "s8", {0xf6}},                          // -10
		{2, "u16", {0xea, 0x60}},                   // 60000
		{3, "s16", {0xfb, 0x2e}},                   // -1234
		{4, "u32", {0x80, 0x00, 0x00, 0x00}},       // 2147483648
		{5, "s32", {0xff, 0xff, 0xff, 0x00}},       // -256
		{7, "float", {0x3f, 0xc0, 0x00, 0x00}},     // 1.5 (control)
	});
	BinarySensorReader reader;
	reader.openMlg(path.string());
	for (int i = 0; i < 2; i++) {
		const auto& values = reader.readBlock();
		// Issue 7: assert the wrong decoded values, not just enum ordinals.
		EXPECT_FLOAT_EQ(-56, values.at("u8"));
		EXPECT_FLOAT_EQ(246, values.at("s8"));
		EXPECT_FLOAT_EQ(-5536, values.at("u16"));
		EXPECT_FLOAT_EQ(64302, values.at("s16"));
		EXPECT_FLOAT_EQ(-2147483648.0f, values.at("u32"));
		EXPECT_FLOAT_EQ(4294967040.0f, values.at("s32"));
		EXPECT_FLOAT_EQ(1.5f, values.at("float"));
	}
	EXPECT_TRUE(reader.eof());
	// The separate converter proves the fixture contains the intended values.
	expectCsv(path, "u8,s8,u16,s16,u32,s32,float", "200,-10,60000,-1234,2147483648,-256,1.5");
}

TEST(MlgOpenIssues, LegacyReaderTreatsInt64AsByteAndLosesAlignment) {
	const auto path = fixturePath();
	writeFixture(path, {
		{6, "s64", {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80}}, // -128
		{0, "sentinel", {42}},
	});
	BinarySensorReader reader;
	reader.openMlg(path.string());
	const auto& values = reader.readBlock();
	// Issue 7: only the first byte of S64 is consumed. The next field reads the
	// second byte, and the next block begins inside the original S64 payload.
	EXPECT_FLOAT_EQ(255, values.at("s64"));
	EXPECT_FLOAT_EQ(-1, values.at("sentinel"));
	EXPECT_THROW(reader.readBlock(), std::runtime_error);
	expectCsv(path, "s64,sentinel", "-128,42");
}
