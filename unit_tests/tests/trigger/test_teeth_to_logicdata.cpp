/*
 * @file test_teeth_to_logicdata.cpp
 *
 * Reads a .teeth CSV file (sampled digital channels over time) and writes a
 * corresponding .logicdata file using the existing writeLogicDataFile()
 * helper. This lets us visualize captured trigger/cam/coil/injector traces
 * in Saleae Logic.
 */

#include "pch.h"

#include "logicdata.h"
#include "unit_test_logger.h"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace {

// Split a single CSV line into trimmed string tokens.
static std::vector<std::string> splitCsv(const std::string& line) {
	std::vector<std::string> out;
	std::string cur;
	std::istringstream ss(line);
	while (std::getline(ss, cur, ',')) {
		size_t a = cur.find_first_not_of(" \t\r\n");
		size_t b = cur.find_last_not_of(" \t\r\n");
		if (a == std::string::npos) {
			out.emplace_back();
		} else {
			out.emplace_back(cur.substr(a, b - a + 1));
		}
	}
	return out;
}

} // namespace

// Teeth CSV format (header):
//   Time[s], Primary, Cam 1, Cam 2, Cam 3, Cam 4, Sync, TDC, Coils, Injectors
// We collapse Cam1..Cam4 into the single secondaryTrigger channel via OR
// (typical .teeth captures only use Cam 1), and feed the remaining columns
// straight into CompositeEvent.
TEST(teethToLogicData, hdReSd) {
	const std::string inPath =
		std::string("tests/trigger/resources/hd-re-sd.teeth");
	const std::string outPath =
		std::string(TEST_RESULTS_DIR) + "/hd-re-sd.logicdata";

	std::ifstream in(inPath);
	ASSERT_TRUE(in.good()) << "Cannot open " << inPath;

	std::string headerLine;
	ASSERT_TRUE(static_cast<bool>(std::getline(in, headerLine)))
		<< "Empty teeth file: " << inPath;
	auto header = splitCsv(headerLine);
	ASSERT_GE(header.size(), 10u)
		<< "Unexpected teeth header column count: [" << headerLine << "]";

	std::vector<CompositeEvent> events;
	events.reserve(8192);

	std::string line;
	size_t lineNo = 1;
	while (std::getline(in, line)) {
		lineNo++;
		if (line.empty()) {
			continue;
		}
		auto cols = splitCsv(line);
		if (cols.size() < 10) {
			// Tolerate a truncated trailing row (the .teeth file is a raw
			// capture and may end mid-line).
			continue;
		}
		double t = std::atof(cols[0].c_str());
		auto bit = [&](size_t idx) {
			return !cols[idx].empty() && cols[idx][0] == '1';
		};
		CompositeEvent ev{};
		// Convert seconds to microseconds for logicdata timestamps.
		ev.timestamp = static_cast<uint32_t>(t * 1'000'000.0);
		ev.primaryTrigger   = bit(1);
		ev.secondaryTrigger = bit(2) || bit(3) || bit(4) || bit(5);
		ev.sync             = bit(6);
		ev.isTDC            = bit(7);
		ev.coil             = bit(8);
		ev.injector         = bit(9);
		events.push_back(ev);
	}

	ASSERT_GT(events.size(), 100u)
		<< "Too few events parsed from " << inPath
		<< " (got " << events.size() << ")";

	writeLogicDataFile(outPath.c_str(), events);

	std::error_code ec;
	auto sz = std::filesystem::file_size(outPath, ec);
	ASSERT_FALSE(ec) << "Output not written: " << outPath;
	EXPECT_GT(sz, 256u) << "Output suspiciously small: " << outPath;
	printf("Wrote %s (%zu events, %zu bytes)\n",
		outPath.c_str(), events.size(), static_cast<size_t>(sz));
}
