/*
 * @file test_logicdata_to_csv.cpp
 *
 * Verifies the new readLogicDataFile() parser (inverse of
 * writeLogicDataFile()) by:
 *  - round-tripping a synthetic CompositeEvent stream;
 *  - taking the real 4b11-running.csv trace, writing it to .logicdata via
 *    writeLogicDataFile(), reading it back and writing a fresh CSV.
 */

#include "pch.h"

#include "logicdata.h"
#include "logicdata_reader.h"
#include "unit_test_logger.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace {

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

// Loads 4b11-running.csv (Time[s], Channel 0, Channel 1) into CompositeEvent.
// Primary <- Channel 0, Secondary <- Channel 1. Other channels stay zero.
static std::vector<CompositeEvent> load4b11Csv(const std::string& inPath) {
	std::ifstream in(inPath);
	std::vector<CompositeEvent> events;
	std::string line;
	bool firstLine = true;
	while (std::getline(in, line)) {
		if (firstLine) { firstLine = false; continue; }
		if (line.empty()) continue;
		auto cols = splitCsv(line);
		if (cols.size() < 3) continue;
		CompositeEvent ev{};
		double t = std::atof(cols[0].c_str());
		ev.timestamp = static_cast<uint32_t>(t * 1'000'000.0);
		ev.primaryTrigger   = !cols[1].empty() && cols[1][0] == '1';
		ev.secondaryTrigger = !cols[2].empty() && cols[2][0] == '1';
		events.push_back(ev);
	}
	return events;
}

static void writeCsv(const std::string& outPath, const std::vector<CompositeEvent>& events) {
	std::ofstream out(outPath);
	ASSERT_TRUE(out.is_open()) << "Cannot write " << outPath;
	out << "Time[s], Primary, Cam 1, Cam 2, Cam 3, Cam 4, Sync, TDC, Coils, Injectors\n";
	out << std::fixed << std::setprecision(15);
	for (const auto& ev : events) {
		double t = ev.timestamp / 1000000.0;
		out << t << ", "
			<< (ev.primaryTrigger ? 1 : 0) << ", "
			<< (ev.secondaryTrigger ? 1 : 0) << ", "
			<< 0 << ", " << 0 << ", " << 0 << ", "
			<< (ev.sync ? 1 : 0) << ", "
			<< (ev.isTDC ? 1 : 0) << ", "
			<< (ev.coil ? 1 : 0) << ", "
			<< (ev.injector ? 1 : 0) << "\n";
	}
}

} // namespace

// Take the captured 4b11-running.csv, write it through writeLogicDataFile(),
// then read the resulting .logicdata back via the new reader and emit a CSV.
// This exercises the full write+read pipeline on real-world trigger data.
TEST(logicDataToCsv, read4b11Running) {
	const std::string csvIn  = std::string("tests/trigger/resources/4b11-running.csv");
	const std::string ldOut  = std::string(TEST_RESULTS_DIR) + "/4b11-running.logicdata";
	const std::string csvOut = std::string(TEST_RESULTS_DIR) + "/4b11-running.from-logicdata.csv";

	std::error_code ec;
	ASSERT_TRUE(std::filesystem::exists(csvIn, ec)) << "Missing input: " << csvIn;

	// 1) Load CSV -> CompositeEvent stream
	auto srcEvents = load4b11Csv(csvIn);
	ASSERT_GT(srcEvents.size(), 100u);

	// 2) Write CompositeEvent stream -> .logicdata (binary)
	writeLogicDataFile(ldOut.c_str(), srcEvents);
	auto ldSize = std::filesystem::file_size(ldOut, ec);
	ASSERT_FALSE(ec);
	EXPECT_GT(ldSize, 256u);

	// 3) Read .logicdata -> CompositeEvent stream
	std::vector<CompositeEvent> readEvents;
	ASSERT_NO_THROW({ readEvents = readLogicDataFile(ldOut.c_str()); });
	ASSERT_GT(readEvents.size(), 100u);

	// Monotonic timestamps are expected (one composite event per unique edge ts).
	for (size_t i = 1; i < readEvents.size(); i++) {
		ASSERT_LT(readEvents[i - 1].timestamp, readEvents[i].timestamp)
			<< "Non-monotonic at " << i;
	}

	// 4) Write a CSV version of what we read back.
	writeCsv(csvOut, readEvents);
	auto csvSize = std::filesystem::file_size(csvOut, ec);
	ASSERT_FALSE(ec);
	EXPECT_GT(csvSize, 256u);

	printf("4b11: csv->%zu events ->logicdata(%zu bytes)->read %zu events->csv(%zu bytes)\n",
		srcEvents.size(), static_cast<size_t>(ldSize),
		readEvents.size(), static_cast<size_t>(csvSize));
}

// Round-trip sanity: write a small synthetic CompositeEvent stream, then read
// it back and check that core fields match.
TEST(logicDataToCsv, roundTripSynthetic) {
	const std::string outPath =
		std::string(TEST_RESULTS_DIR) + "/roundtrip.logicdata";

	std::vector<CompositeEvent> in;
	auto add = [&](uint32_t ts, bool p, bool s, bool tdc, bool sync, bool coil, bool inj) {
		CompositeEvent e{};
		e.timestamp = ts;
		e.primaryTrigger = p;
		e.secondaryTrigger = s;
		e.isTDC = tdc;
		e.sync = sync;
		e.coil = coil;
		e.injector = inj;
		in.push_back(e);
	};
	add(0,        0, 0, 0, 0, 0, 0);
	add(1000,     1, 0, 0, 0, 0, 0);
	add(2000,     1, 1, 0, 0, 0, 0);
	add(3000,     0, 1, 1, 0, 0, 0);
	add(4000,     0, 0, 0, 1, 0, 0);
	add(5000,     0, 0, 0, 0, 1, 0);
	add(6000,     0, 0, 0, 0, 0, 1);
	add(7000,     1, 1, 1, 1, 1, 1);
	add(8000,     0, 0, 0, 0, 0, 0);

	writeLogicDataFile(outPath.c_str(), in);

	std::vector<CompositeEvent> out;
	ASSERT_NO_THROW({ out = readLogicDataFile(outPath.c_str()); });

	ASSERT_GE(out.size(), 1u);
	for (const auto& ev : out) {
		bool found = false;
		for (const auto& src : in) {
			if (src.timestamp == ev.timestamp) {
				found = true;
				EXPECT_EQ(src.primaryTrigger,   ev.primaryTrigger)   << "ts=" << ev.timestamp;
				EXPECT_EQ(src.secondaryTrigger, ev.secondaryTrigger) << "ts=" << ev.timestamp;
				EXPECT_EQ(src.isTDC,            ev.isTDC)            << "ts=" << ev.timestamp;
				EXPECT_EQ(src.sync,             ev.sync)             << "ts=" << ev.timestamp;
				EXPECT_EQ(src.coil,             ev.coil)             << "ts=" << ev.timestamp;
				EXPECT_EQ(src.injector,         ev.injector)         << "ts=" << ev.timestamp;
				break;
			}
		}
		EXPECT_TRUE(found) << "Unexpected timestamp " << ev.timestamp;
	}
}
