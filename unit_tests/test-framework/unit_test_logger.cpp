#include "pch.h"

#include "unit_test_logger.h"
#include "binary_mlg_logging.h"
#include "mlg_field.h"

FILE *mslFile = nullptr;
static FILE *csvFile = nullptr;
static FILE *ndjsonFile = nullptr;

struct UnitTestLogBufferWriter final : public BufferedWriter<512> {
	bool failed = false;

	size_t writeInternal(const char *buffer, size_t count) override {
		return fwrite(buffer, 1, count, mslFile);
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
	bool first = true;
	fputs("time_us", csvFile);
	first = false;
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
	(void)first;
}

static void writeCsvLine() {
	if (csvFile == nullptr) {
		return;
	}
	fprintf(csvFile, "%lld", static_cast<long long>(getTimeNowUs()));
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
		fprintf(csvFile, ",%.*f", digits, v);
	});
	fputc('\n', csvFile);
}

static void writeNdjsonLine() {
	if (ndjsonFile == nullptr) {
		return;
	}
	fprintf(ndjsonFile, "{\"t_us\":%lld", static_cast<long long>(getTimeNowUs()));
	MLG::forEachField([&](const MLG::Entries::Field& f) {
		void* offset = MLG::getUnitTestFieldOffset(f);
		double v = f.readValueAsDouble(offset);
		std::string name = jsonEscape(f.getName());
		// Always emit as a JSON number. NaN/Inf are emitted as null to keep JSON valid.
		if (std::isnan(v) || std::isinf(v)) {
			fprintf(ndjsonFile, ",\"%s\":null", name.c_str());
		} else {
			fprintf(ndjsonFile, ",\"%s\":%.9g", name.c_str(), v);
		}
	});
	fputs("}\n", ndjsonFile);
}

void writeUnitTestLogLine() {
	if (mslFile != nullptr) {
		MLG::writeSdLogLine(unitTestLogWriter);
	}
	writeCsvLine();
	writeNdjsonLine();
}

void createUnitTestLog() {
	auto testInfo = ::testing::UnitTest::GetInstance()->current_test_info();
	std::string base = std::string(TEST_RESULTS_DIR) + "/unittest_"
		+ testInfo->test_case_name() + "_" + testInfo->name();

	std::string mslPath = base + ".msl";
	std::string csvPath = base + ".csv";
	std::string ndjsonPath = base + ".ndjson";

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

	ndjsonFile = fopen(ndjsonPath.c_str(), "w");
	if (ndjsonFile != nullptr) {
		printf("Writing NDJSON [%s]\n", ndjsonPath.c_str());
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
