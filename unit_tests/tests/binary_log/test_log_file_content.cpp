/*
 * test_log_file_content.cpp
 *
 * Asserts the per-test artifact log files (NDJSON, CSV, and binary MLG)
 * created by setUnitTestCreateLogs(true) have the expected basic content.
 */

#include "pch.h"

#include "unit_test_logger.h"
#include "msl_to_csv.h"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace {

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
		// Every always-on field starts at zero on a fresh EngineTestHelper.
		// Note: the MLG "Time" field is *not* the unit-test microsecond
		// timestamp -- it is the engine "Time (sec)" live-data field which
		// remains 0 in this no-trigger test. So every row's prefix is
		// "0,0,0,0,...,0" (Time + 14 zero booleans matching the header
		// prefix asserted above).
		const std::string expectedRowPrefix =
			"0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0";
		for (size_t r = 0; r < dataRows.size(); r++) {
			EXPECT_EQ(0u, dataRows[r].find(expectedRowPrefix))
				<< "Converted CSV row " << r << " prefix mismatch. "
				<< "Expected leading [" << expectedRowPrefix
				<< "]; got [" << dataRows[r] << "]";
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
