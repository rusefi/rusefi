/*
 * test_log_file_content.cpp
 *
 * Asserts the per-test artifact log files (NDJSON, CSV, and binary MLG)
 * created by setUnitTestCreateLogs(true) have the expected basic content.
 */

#include "pch.h"

#include "unit_test_logger.h"

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

TEST(unitTestLog, ndjsonCsvAndBinaryContent) {
	bool wasLogging = getUnitTestCreateLogs();
	setUnitTestCreateLogs(true);

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
		ASSERT_GE(blob.size(), 6u) << "MLG file too small: " << mslPath;
		// File format magic: "MLVLG\0" — see firmware/console/binary_mlg_log/binary_mlg_logging.cpp
		const char expectedMagic[6] = {'M', 'L', 'V', 'L', 'G', '\0'};
		EXPECT_EQ(0, std::memcmp(blob.data(), expectedMagic, sizeof(expectedMagic)))
			<< "Unexpected MLG header magic in " << mslPath;
	}

	// --- CSV (.csv) ---
	{
		std::string header = readFirstLine(csvPath);
		ASSERT_FALSE(header.empty()) << "CSV file empty: " << csvPath;
		EXPECT_EQ(0u, header.find("time_us"))
			<< "CSV header must start with 'time_us', got: [" << header << "]";
		// Header should have multiple columns (time_us + at least one field).
		EXPECT_NE(std::string::npos, header.find(','))
			<< "CSV header should have multiple columns: [" << header << "]";

		// And at least one data line should exist.
		std::ifstream f(csvPath);
		std::string line;
		std::getline(f, line); // header
		std::getline(f, line); // first data row
		EXPECT_FALSE(line.empty()) << "CSV missing data row: " << csvPath;
	}

	// --- NDJSON (.ndjson) ---
	{
		std::string firstLine = readFirstLine(ndjsonPath);
		ASSERT_FALSE(firstLine.empty()) << "NDJSON file empty: " << ndjsonPath;
		// Every line is a JSON object beginning with the timestamp key.
		EXPECT_EQ(0u, firstLine.find("{\"t_us\":"))
			<< "NDJSON line should start with {\"t_us\": ..., got: [" << firstLine << "]";
		EXPECT_EQ('}', firstLine.back())
			<< "NDJSON line should end with '}', got: [" << firstLine << "]";
	}

	setUnitTestCreateLogs(wasLogging);
}
