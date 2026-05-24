#include "pch.h"

#include "unit_test_logger.h"
#include "binary_mlg_logging.h"
#include "mlg_field.h"

#include <filesystem>

// these NDJSON files are 10 times larger than MLS but very much machine readable
bool useNdJSONFiles = false;

FILE *mslFile = nullptr;
static FILE *csvFile = nullptr;
static FILE *ndjsonFile = nullptr;

// Paths of the per-test log files currently being written, used to remove
// them if a writer exceeds LOG_FILE_SIZE_LIMIT (LogsTooLargeException).
static std::string mslPathCurrent;
static std::string csvPathCurrent;
static std::string ndjsonPathCurrent;

// Aggregated stats for aborted log runs, surfaced via sayByeBye().
static size_t abortedLogsCount = 0;
static size_t abortedLogsTotalBytes = 0;

size_t getAbortedLogsCount() { return abortedLogsCount; }
size_t getAbortedLogsTotalBytes() { return abortedLogsTotalBytes; }

// Per-stream byte counters. Each writer increments its counter as it produces
// output and throws LogsTooLargeException once the counter exceeds
// LOG_FILE_SIZE_LIMIT. The caller (writeUnitTestLogLine) catches the exception
// and disables logging for the rest of the test run.
static size_t mslBytes = 0;
static size_t csvBytes = 0;
static size_t ndjsonBytes = 0;

static void checkSizeLimit(const char* which, size_t bytes) {
	if (bytes > LOG_FILE_SIZE_LIMIT) {
		throw LogsTooLargeException(std::string(which) + " log exceeded "
			+ std::to_string(LOG_FILE_SIZE_LIMIT) + " bytes (size="
			+ std::to_string(bytes) + ")");
	}
}

struct UnitTestLogBufferWriter final : public BufferedWriter<512> {
	bool failed = false;

	size_t writeInternal(const char *buffer, size_t count) override {
		size_t n = fwrite(buffer, 1, count, mslFile);
		mslBytes += n;
		return n;
	}
};

static UnitTestLogBufferWriter unitTestLogWriter;

// CSV escape: wrap in quotes if it contains comma/quote/newline; double internal quotes.
static std::string csvEscape(const char* s) {
	if (s == nullptr) {
		return "";
	}
	std::string in(s);
	bool needsQuotes = in.find_first_of(",\"\n\r") != std::string::npos;
	if (!needsQuotes) {
		return in;
	}
	std::string out;
	out.reserve(in.size() + 2);
	out.push_back('"');
	for (char c : in) {
		if (c == '"') {
			out.push_back('"');
		}
		out.push_back(c);
	}
	out.push_back('"');
	return out;
}

static std::string jsonEscape(const char* s) {
	if (s == nullptr) {
		return "";
	}
	std::string out;
	for (const char* p = s; *p; ++p) {
		unsigned char c = static_cast<unsigned char>(*p);
		switch (c) {
			case '"':  out += "\\\""; break;
			case '\\': out += "\\\\"; break;
			case '\b': out += "\\b"; break;
			case '\f': out += "\\f"; break;
			case '\n': out += "\\n"; break;
			case '\r': out += "\\r"; break;
			case '\t': out += "\\t"; break;
			default:
				if (c < 0x20) {
					char buf[8];
					snprintf(buf, sizeof(buf), "\\u%04x", c);
					out += buf;
				} else {
					out.push_back(static_cast<char>(c));
				}
		}
	}
	return out;
}

static void writeCsvHeader() {
	if (csvFile == nullptr) {
		return;
	}
	fputs("time_us", csvFile);
	MLG::forEachField([&](const MLG::Entries::Field& f) {
		fputc(',', csvFile);
		std::string name = f.getName() ? f.getName() : "";
		const char* units = f.getUnits();
		if (units && units[0] != '\0') {
			name += "[";
			name += units;
			name += "]";
		}
		fputs(csvEscape(name.c_str()).c_str(), csvFile);
	});
	fputc('\n', csvFile);
}

static void writeCsvLine() {
	if (csvFile == nullptr) {
		return;
	}
	int written = fprintf(csvFile, "%lld", static_cast<long long>(getTimeNowUs()));
	if (written > 0) {
		csvBytes += written;
	}
	MLG::forEachField([&](const MLG::Entries::Field& f) {
		void* offset = MLG::getUnitTestFieldOffset(f);
		double v = f.readValueAsDouble(offset);
		int digits = f.getDigits();
		if (digits < 0) {
			digits = 0;
		}
		if (digits > 9) {
			digits = 9;
		}
		int n = fprintf(csvFile, ",%.*f", digits, v);
		if (n > 0) {
			csvBytes += n;
		}
	});
	fputc('\n', csvFile);
	csvBytes += 1;
	checkSizeLimit("CSV", csvBytes);
}

static void writeNdjsonLine() {
	if (ndjsonFile == nullptr) {
		return;
	}
	int written = fprintf(ndjsonFile, "{\"t_us\":%lld", static_cast<long long>(getTimeNowUs()));
	if (written > 0) {
		ndjsonBytes += written;
	}
	MLG::forEachField([&](const MLG::Entries::Field& f) {
		void* offset = MLG::getUnitTestFieldOffset(f);
		double v = f.readValueAsDouble(offset);
		std::string name = jsonEscape(f.getName());
		// Always emit as a JSON number. NaN/Inf are emitted as null to keep JSON valid.
		// Note: the field name is never passed as a printf format string -- it is
		// emitted as a literal via "%s" so that '%' characters in names cannot be
		// misinterpreted (which previously tripped ASAN's printf interceptor).
		int n;
		if (std::isnan(v) || std::isinf(v)) {
			n = fprintf(ndjsonFile, ",\"%s\":null", name.c_str());
		} else {
			n = fprintf(ndjsonFile, ",\"%s\":%.9g", name.c_str(), v);
		}
		if (n > 0) {
			ndjsonBytes += n;
		}
	});
	fputs("}\n", ndjsonFile);
	ndjsonBytes += 2;
	checkSizeLimit("NDJSON", ndjsonBytes);
}

void writeUnitTestLogLine() {
	try {
		if (mslFile != nullptr) {
			MLG::writeSdLogLine(unitTestLogWriter);
			checkSizeLimit("MSL", mslBytes);
		}
		writeCsvLine();
		writeNdjsonLine();
	} catch (const LogsTooLargeException& e) {
		// Close the streams and delete the over-sized log files for the current
		// test (so we don't leave huge artifacts behind), bump the aborted
		// counter, then re-throw so the offending test fails loudly via gtest.
		// Logging is only disabled for this specific test (streams are closed);
		// the global setUnitTestCreateLogs flag is left unchanged so other tests
		// can still produce logs.
		closeUnitTestLog();
		auto removeAndCount = [](const std::string& path) {
			if (path.empty()) {
				return;
			}
			std::error_code ec;
			auto sz = std::filesystem::file_size(path, ec);
			if (!ec) {
				abortedLogsTotalBytes += static_cast<size_t>(sz);
			}
			std::error_code rmEc;
			std::filesystem::remove(path, rmEc);
		};
		removeAndCount(mslPathCurrent);
		removeAndCount(csvPathCurrent);
		removeAndCount(ndjsonPathCurrent);
		mslPathCurrent.clear();
		csvPathCurrent.clear();
		ndjsonPathCurrent.clear();
		abortedLogsCount++;
		throw;
	}
}

void createUnitTestLog() {
	auto testInfo = ::testing::UnitTest::GetInstance()->current_test_info();
	std::string base = std::string(TEST_RESULTS_DIR) + "/unittest_"
		+ testInfo->test_case_name() + "_" + testInfo->name();

	std::string mslPath = base + ".msl";
	std::string csvPath = base + ".csv";
	std::string ndjsonPath = base + ".ndjson";

	mslBytes = 0;
	csvBytes = 0;
	ndjsonBytes = 0;
	mslPathCurrent = mslPath;
	csvPathCurrent = csvPath;
	ndjsonPathCurrent = ndjsonPath;

	// fun fact: ASAN says not to extract 'fileName' into a variable, we must be doing something a bit not right?
	mslFile = fopen(mslPath.c_str(), "wb");
	if (mslFile != nullptr) {
		printf("Writing MLS [%s]\n", mslPath.c_str());
		MLG::resetFileLogging();
		MLG::writeSdLogLine(unitTestLogWriter);
	}

	csvFile = fopen(csvPath.c_str(), "w");
	if (csvFile != nullptr) {
		printf("Writing CSV [%s]\n", csvPath.c_str());
		writeCsvHeader();
	}

  if (useNdJSONFiles) {
	  ndjsonFile = fopen(ndjsonPath.c_str(), "w");
	  if (ndjsonFile != nullptr) {
  		printf("Writing NDJSON [%s]\n", ndjsonPath.c_str());
  	}
  }
}

void closeUnitTestLog() {
	if (mslFile != nullptr) {
		unitTestLogWriter.flush();
		fclose(mslFile);
		mslFile = nullptr;
	}
	if (csvFile != nullptr) {
		fclose(csvFile);
		csvFile = nullptr;
	}
	if (ndjsonFile != nullptr) {
		fclose(ndjsonFile);
		ndjsonFile = nullptr;
	}
}
