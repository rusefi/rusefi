/*
 * test_log_file_content.cpp
 *
 * Asserts the per-test artifact log files (NDJSON, CSV, and binary MLG)
 * created by setUnitTestCreateLogs(true) have the expected basic content.
 */

#include "pch.h"

#include "unit_test_logger.h"
#include "msl_to_csv.h"
#include "binary_mlg_logging.h"
#include "mlg_field.h"
#include "value_lookup.h"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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

float readFloatBE(const char* bytes) {
	uint32_t bits = 0;
	for (size_t i = 0; i < sizeof(bits); i++) {
		bits = (bits << 8) | static_cast<unsigned char>(bytes[i]);
	}
	float result;
	memcpy(&result, &bits, sizeof(result));
	return result;
}

std::string makeArtifactPath(const std::string& suffix) {
	auto testInfo = ::testing::UnitTest::GetInstance()->current_test_info();
	std::string base = std::string(TEST_RESULTS_DIR) + "/unittest_"
		+ testInfo->test_case_name() + "_" + testInfo->name();
	return base + suffix;
}

std::string readAllBytes(const std::string& path) {
	std::ifstream f(path, std::ios::binary);
	std::ostringstream ss;
	ss << f.rdbuf();
	return ss.str();
}

std::string readFirstLine(const std::string& path) {
	std::ifstream f(path);
	std::string line;
	std::getline(f, line);
	return line;
}

} // namespace

extern bool useNdJSONFiles;

TEST(unitTestLog, binaryHeaderMatchesEmittedRecord) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	CaptureWriter header;
	MLG::writeFileHeader(header);
	CaptureWriter record;
	MLG::writeSdBlock(record);
	ASSERT_GE(header.bytes.size(), 24u);
	ASSERT_GE(record.bytes.size(), 5u);
	auto readUnsignedBE = [&](size_t offset, size_t size) {
		size_t value = 0;
		for (size_t i = 0; i < size; i++) {
			value = (value << 8) | static_cast<unsigned char>(header.bytes[offset + i]);
		}
		return value;
	};
	// Decode the wire header independently, including bytes with their high bit set.
	EXPECT_EQ(header.bytes.size(), readUnsignedBE(16, 4));
	// A data block has a four-byte prefix and a one-byte checksum outside the payload.
	EXPECT_EQ(record.bytes.size() - 5, readUnsignedBE(20, 2));
	EXPECT_EQ((header.bytes.size() - 24) / 89, readUnsignedBE(22, 2));
}

TEST(unitTestLog, generatedEnumFieldsContainCurrentValues) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& stft = engine->module<ShortTermFuelTrim>().unmock();
	stft.stftCorrectionBinIdx = ftRegionPower;
	stft.stftCorrectionState = stftDisabledRPM;
	stft.stftLearningState[0] = stftDisabledLaunch;

	CaptureWriter record;
	MLG::writeSdBlock(record);
	size_t offset = 4;
	int matches = 0;
	MLG::forEachField([&](const MLG::Entries::Field& field) {
		const std::string name(field.getName());
		int expected = -1;
		if (name == "STFT: used bin") expected = ftRegionPower;
		if (name == "STFT: stftCorrectionState") expected = stftDisabledRPM;
		if (name == "STFT: stftLearningState 1") expected = stftDisabledLaunch;
		if (expected >= 0) {
			matches++;
			ASSERT_EQ(1u, field.getSize()) << name;
			EXPECT_EQ(static_cast<uint8_t>(MLG::Types::Field::Scalar::U08), field.getTypeId()) << name;
			ASSERT_LT(offset, record.bytes.size());
			EXPECT_EQ(expected, static_cast<uint8_t>(record.bytes[offset])) << name;
			EXPECT_DOUBLE_EQ(expected, field.readValueAsDouble()) << name;
		}
		offset += field.getSize();
	});
	EXPECT_EQ(3, matches);
}

TEST(unitTestLog, sourcesFollowEngineAndMockReplacement) {
	// The generated table exists before any EngineTestHelper. Reuse it across
	// destroyed fixtures and changes to each of the four mockable sources.
	for (int fixture = 1; fixture <= 2; fixture++) {
		EngineTestHelper eth(engine_type_e::TEST_ENGINE);
		ThrottleModel throttleA, throttleB;
		HpfpController hpfpA, hpfpB;
		AcController acA, acB;
		MisfireController misfireA, misfireB;
		auto seed = [](auto& throttle, auto& hpfp, auto& ac, auto& misfire, int value) {
			throttle.throttleModelCrossoverAngle = value * 12.5f;
			throttle.throttleUseWotModel = value % 2;
			hpfp.fuel_requested_percent = value * 10;
			hpfp.isHpfpActive = value % 2;
			ac.acPressureTooHigh = value % 2;
			misfire.misfireEmaUs = value * 1250;
		};
		seed(engine->module<ThrottleModel>().unmock(), engine->module<HpfpController>().unmock(),
			 engine->module<AcController>().unmock(), engine->module<MisfireController>().unmock(), fixture);
		seed(throttleA, hpfpA, acA, misfireA, fixture + 2);
		seed(throttleB, hpfpB, acB, misfireB, fixture + 3);
		engine->outputChannels.rtcUnixEpochTime = fixture * 123456;
		for (int phase = 0; phase < 4; phase++) {
			SCOPED_TRACE(::testing::Message() << "fixture " << fixture << " phase " << phase);
			engine->module<ThrottleModel>().set(phase == 1 ? &throttleA : phase == 2 ? &throttleB : nullptr);
			engine->module<HpfpController>().set(phase == 1 ? &hpfpA : phase == 2 ? &hpfpB : nullptr);
			engine->module<AcController>().set(phase == 1 ? &acA : phase == 2 ? &acB : nullptr);
			engine->module<MisfireController>().set(phase == 1 ? &misfireA : phase == 2 ? &misfireB : nullptr);
			const int value = fixture + (phase == 1 ? 2 : phase == 2 ? 3 : 0);
			CaptureWriter record;
			MLG::writeSdBlock(record);
			size_t offset = 4;
			int matches = 0;
			MLG::forEachField([&](const MLG::Entries::Field& field) {
				const std::string name(field.getName());
				auto expect = [&](double engineering, double raw, bool floating) {
					matches++;
					EXPECT_NEAR(engineering, field.readValueAsDouble(), 0.00001) << name;
					ASSERT_LE(offset + field.getSize(), record.bytes.size());
					if (floating) {
						EXPECT_FLOAT_EQ(raw, readFloatBE(record.bytes.data() + offset)) << name;
					} else {
						uint32_t stored = 0;
						for (size_t i = 0; i < field.getSize(); i++) {
							stored = (stored << 8) | static_cast<unsigned char>(record.bytes[offset + i]);
						}
						EXPECT_EQ(raw, stored) << name;
					}
				};
				if (name == "Air: Throttle crossover pos") {
					expect(value * 12.5, value * 1250, false);
				} else if (name == "Air: Throttle model WOT" || name == "AC: pressure too high" || name == "GDI: HPFP Active") {
					expect(value % 2, value % 2, false);
				} else if (name == "GDI: HPFP requested duty") {
					expect(value * 10, value * 10, true);
				} else if (name == "Misfire: EMA baseline") {
					expect(value * 1.25, value * 1250, true);
				} else if (name == "rtcUnixEpochTime") {
					expect(fixture * 123456, fixture * 123456, false);
				}
				offset += field.getSize();
			});
			EXPECT_EQ(7, matches);
			EXPECT_NEAR(value * 12.5, getOutputValueByName("throttleModelCrossoverAngle"), 0.00001);
			EXPECT_FLOAT_EQ(value * 10, getOutputValueByName("fuel_requested_percent"));
			EXPECT_FLOAT_EQ(value % 2, getOutputValueByName("acPressureTooHigh"));
			EXPECT_NEAR(value * 1.25, getOutputValueByName("misfireEmaUs"), 0.00001);
		}
	}
}

TEST(unitTestLog, generatedOutputConversions) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& misfire = engine->module<MisfireController>().unmock();
	misfire.misfireEmaUs = 1250;
	EXPECT_NEAR(1.25, getOutputValueByName("misfireEmaUs"), 0.00001);
	EXPECT_FLOAT_EQ(1250, misfire.misfireEmaUs);
	CaptureWriter header;
	MLG::writeFileHeader(header);
	for (float raw : {0.95f, 1.0f, 1.05f}) {
		engine->engineState.stftCorrection[0] = raw;
		engine->engineState.stftCorrection[1] = raw;
		engine->engineState.dwellActualRatio = raw;
		CaptureWriter record;
		MLG::writeSdBlock(record);
		size_t descriptorOffset = MLG::Types::Header::Size;
		size_t payloadOffset = 4; // Standard data-block prefix.
		int matches = 0;
		MLG::forEachField([&](const MLG::Entries::Field& field) {
			std::string name(field.getName());
			if (name == "STFT: Bank 1" || name == "STFT: Bank 2" || name == "Ignition: Dwell deviation") {
				matches++;
				ASSERT_LE(descriptorOffset + 89, header.bytes.size());
				ASSERT_LE(payloadOffset + sizeof(float), record.bytes.size());
				float scale = readFloatBE(header.bytes.data() + descriptorOffset + 46);
				float translation = readFloatBE(header.bytes.data() + descriptorOffset + 50);
				// [tag:live_data_output_scale] Actual generated fields must match their INI conversions.
				EXPECT_FLOAT_EQ(100, scale) << name;
				EXPECT_FLOAT_EQ(-1, translation) << name;
				float stored = readFloatBE(record.bytes.data() + payloadOffset);
				EXPECT_FLOAT_EQ(raw, stored) << name;
				EXPECT_NEAR((raw - 1) * 100, (stored + translation) * scale, 0.00001) << name;
			}
			descriptorOffset += MLG::Types::Field::DescriptorSize;
			payloadOffset += field.getSize();
		});
		EXPECT_EQ(3, matches);
		EXPECT_NEAR((raw - 1) * 100, getOutputValueByName("dwellActualRatio"), 0.00001);
	}
}

TEST(unitTestLog, ndjsonCsvAndBinaryContent) {
  bool wasUsing = useNdJSONFiles;
  useNdJSONFiles = true;
	ScopedUnitTestCreateLogs scoped(true);

	std::string mslPath = makeArtifactPath(".msl");
	std::string csvPath = makeArtifactPath(".csv");
	std::string ndjsonPath = makeArtifactPath(".ndjson");

	// Remove any stale artifacts from a previous run so we test only this iteration.
	std::error_code ec;
	std::filesystem::remove(mslPath, ec);
	std::filesystem::remove(csvPath, ec);
	std::filesystem::remove(ndjsonPath, ec);

	{
		EngineTestHelper eth(engine_type_e::TEST_ENGINE);

		// Produce a handful of log lines so each file has a header and at least one data row.
		for (int i = 0; i < 3; i++) {
			eth.moveTimeForwardUs(1000);
			writeUnitTestLogLine();
		}
		// EngineTestHelper destructor calls closeUnitTestLog() so the files get flushed.
	}

	// --- Binary MLG (.msl) ---
	{
		std::string blob = readAllBytes(mslPath);
		ASSERT_GE(blob.size(), 256u) << "MLG file too small: " << mslPath;
		// File format magic: "MLVLG\0" — see firmware/console/binary_mlg_log/binary_mlg_logging.cpp
		const char expectedMagic[6] = {'M', 'L', 'V', 'L', 'G', '\0'};
		EXPECT_EQ(0, std::memcmp(blob.data(), expectedMagic, sizeof(expectedMagic)))
			<< "Unexpected MLG header magic in " << mslPath;

		// The MLG header lays out fixed-width field metadata after the file-format
		// magic. Sanity-check that the first field is "Time" (NUL-terminated and
		// padded), followed by its unit "sec" — both must appear well within the
		// header, before the second field "SD: Present".
		size_t timePos = blob.find(std::string("Time", 4) + std::string(1, '\0'));
		ASSERT_NE(std::string::npos, timePos) << "MLG header missing 'Time' field name";
		ASSERT_LT(timePos, 64u) << "'Time' field name should be near the start of MLG header";

		size_t secPos = blob.find(std::string("sec", 3) + std::string(1, '\0'), timePos);
		ASSERT_NE(std::string::npos, secPos) << "MLG header missing 'sec' unit for Time field";
		ASSERT_LT(secPos - timePos, 64u) << "'sec' unit should be near 'Time' field name";

		size_t presentPos = blob.find("SD: Present");
		ASSERT_NE(std::string::npos, presentPos) << "MLG should contain 'SD: Present' field name";
		ASSERT_GT(presentPos, secPos) << "'SD: Present' should appear after Time/sec metadata";
	}

	// --- Convert MLG to CSV via msl_to_csv and sanity-check the result ---
	{
		std::string convertedCsvPath = mslPath + ".converted.csv";
		std::error_code ec2;
		std::filesystem::remove(convertedCsvPath, ec2);

		ASSERT_TRUE(convertMslToCsv(mslPath.c_str(), convertedCsvPath.c_str()))
			<< "convertMslToCsv failed for " << mslPath;

		std::string convertedHeader = readFirstLine(convertedCsvPath);
		ASSERT_FALSE(convertedHeader.empty())
			<< "Converted CSV is empty: " << convertedCsvPath;
		// Strong assertion: header derived from MLG field metadata is
		// fully deterministic. Time appears as "Time (sec)" (units in
		// parentheses, no leading "time_us" column since the binary log
		// has no separate microsecond timestamp column), followed by the
		// same fixed dozen always-on fields in the declared order.
		const std::string expectedConvertedHeaderPrefix =
			"Time (sec),SD: Present,SD: Logging,triggerScopeReady,"
			"antilagTriggered,isO2HeaterOn,checkEngine,needBurn,SD: MSD,"
			"Tooth Logger Ready,Error: TPS,Error: CLT,Error: MAP,Error: IAT,"
			"Error: Trigger";
		ASSERT_GE(convertedHeader.size(), expectedConvertedHeaderPrefix.size())
			<< "Converted CSV header shorter than expected prefix: ["
			<< convertedHeader << "]";
		EXPECT_EQ(expectedConvertedHeaderPrefix,
			convertedHeader.substr(0, expectedConvertedHeaderPrefix.size()))
			<< "Converted CSV header prefix mismatch. Full header: ["
			<< convertedHeader << "]";

		// Exactly 3 data rows are expected. createUnitTestLog() writes a
		// first record at t=0 (the initial writeSdLogLine() call); the
		// three writeUnitTestLogLine() calls above then add three more
		// records, but the very last one is dropped because EngineTestHelper's
		// destructor closes/flushes the file after the loop. Net result is
		// 3 rows in the binary MLG => 3 rows in the converted CSV with
		// Time = 0, 0.001, 0.002 seconds respectively.
		std::ifstream cf(convertedCsvPath);
		std::string cline;
		std::vector<std::string> dataRows;
		std::getline(cf, cline); // header
		while (std::getline(cf, cline)) {
			if (!cline.empty()) {
				dataRows.push_back(cline);
			}
		}
		ASSERT_EQ(3u, dataRows.size())
			<< "Converted CSV should have exactly 3 data rows; got "
			<< dataRows.size() << " in " << convertedCsvPath;
		// The 15 always-on boolean fields after Time start at zero on a
		// fresh EngineTestHelper. The Time (sec) column is declared with
		// 3-digit precision in the MLG field descriptor (TIME_PRECISION=1000,
		// i.e. millisecond precision), so the first CSV column must be
		// formatted as "0.000" / "0.002" / "0.003" rather than bare "0".
		// (Defect: prior to fix, Time was rendered with digits=0, yielding
		// "0" with zero-digit precision for every row.)
		const std::string booleanPrefix =
			"0,0,0,0,0,0,0,0,0,0,0,0,0,0,0";
		for (size_t r = 0; r < dataRows.size(); r++) {
			// Defect guard #1: Time column must contain a decimal point.
			size_t firstComma = dataRows[r].find(',');
			ASSERT_NE(std::string::npos, firstComma)
				<< "Converted CSV row " << r << " missing any comma: ["
				<< dataRows[r] << "]";
			std::string timeCol = dataRows[r].substr(0, firstComma);
			EXPECT_NE(std::string::npos, timeCol.find('.'))
				<< "Converted CSV row " << r
				<< " Time (sec) column ['" << timeCol
				<< "'] has no decimal point — must use millisecond precision";
			// Defect guard #2: 3-digit precision => exactly 3 chars after '.'.
			size_t dotPos = timeCol.find('.');
			if (dotPos != std::string::npos) {
				EXPECT_EQ(3u, timeCol.size() - dotPos - 1)
					<< "Converted CSV row " << r
					<< " Time (sec) column ['" << timeCol
					<< "'] does not have exactly 3 fractional digits";
			}
			// And the remaining 15 boolean fields must all be zero.
			EXPECT_EQ(firstComma + 1, dataRows[r].find(booleanPrefix))
				<< "Converted CSV row " << r
				<< " boolean prefix mismatch after Time. Got ["
				<< dataRows[r] << "]";
		}
	}

	// --- CSV (.csv) ---
	{
		std::string header = readFirstLine(csvPath);
		ASSERT_FALSE(header.empty()) << "CSV file empty: " << csvPath;
		// Strong assertion: the first ~120 bytes of the CSV header are
		// stable — they are the first dozen always-on log fields in the
		// declared order. If anyone reorders, renames, or removes one of
		// these columns, we want this test to scream.
		const std::string expectedCsvHeaderPrefix =
			"time_us,Time[sec],SD: Present,SD: Logging,triggerScopeReady,"
			"antilagTriggered,isO2HeaterOn,checkEngine,needBurn,SD: MSD,"
			"Tooth Logger Ready,Error: TPS,Error: CLT,Error: MAP,Error: IAT,"
			"Error: Trigger";
		ASSERT_GE(header.size(), expectedCsvHeaderPrefix.size())
			<< "CSV header shorter than expected prefix: [" << header << "]";
		EXPECT_EQ(expectedCsvHeaderPrefix, header.substr(0, expectedCsvHeaderPrefix.size()))
			<< "CSV header prefix mismatch. Full header: [" << header << "]";

		// And at least one data line should exist, and it must start with the
		// timestamp we advanced to (1000us on the first writeUnitTestLogLine()).
		std::ifstream f(csvPath);
		std::string line;
		std::getline(f, line); // header
		std::getline(f, line); // first data row
		ASSERT_FALSE(line.empty()) << "CSV missing data row: " << csvPath;
		EXPECT_EQ(0u, line.find("1000,"))
			<< "CSV first data row should start with '1000,' (time_us): [" << line << "]";
	}

	// --- NDJSON (.ndjson) ---
	{
		std::string firstLine = readFirstLine(ndjsonPath);
		ASSERT_FALSE(firstLine.empty()) << "NDJSON file empty: " << ndjsonPath;
		// Strong assertion: the first ~150 bytes of the first NDJSON record are
		// fully deterministic given a fresh EngineTestHelper and a single
		// moveTimeForwardUs(1000) before the first writeUnitTestLogLine().
		const std::string expectedNdjsonPrefix =
			"{\"t_us\":1000,\"Time\":0.00100000005,\"SD: Present\":0,"
			"\"SD: Logging\":0,\"triggerScopeReady\":0,\"antilagTriggered\":0,"
			"\"isO2HeaterOn\":0,\"checkEngine\":0";
		ASSERT_GE(firstLine.size(), expectedNdjsonPrefix.size())
			<< "NDJSON line shorter than expected prefix: [" << firstLine << "]";
		EXPECT_EQ(expectedNdjsonPrefix, firstLine.substr(0, expectedNdjsonPrefix.size()))
			<< "NDJSON prefix mismatch. Full line: [" << firstLine << "]";
		EXPECT_EQ('}', firstLine.back())
			<< "NDJSON line should end with '}', got: [" << firstLine << "]";
	}

	useNdJSONFiles = wasUsing;
}
