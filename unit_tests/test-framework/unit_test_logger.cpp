#include "pch.h"

#include "unit_test_logger.h"
#include "binary_mlg_logging.h"

FILE *mslFile = nullptr;

struct UnitTestLogBufferWriter final : public BufferedWriter<512> {
	bool failed = false;

	size_t writeInternal(const char *buffer, size_t count) override {
		return fwrite(buffer, 1, count, mslFile);
	}
};

static UnitTestLogBufferWriter unitTestLogWriter;

void writeUnitTestLogLine() {
	if (mslFile != nullptr) {
		MLG::writeSdLogLine(unitTestLogWriter);
	}
}

void createUnitTestLog() {
	auto testInfo = ::testing::UnitTest::GetInstance()->current_test_info();
	std::stringstream filePath;
	filePath << TEST_RESULTS_DIR << "/unittest_" << testInfo->test_case_name()
			<< "_" << testInfo->name() << ".msl";
	// fun fact: ASAN says not to extracjsonTracet 'fileName' into a variable, we must be doing something a bit not right?
	mslFile = fopen(filePath.str().c_str(), "wb");
	if (mslFile != nullptr) {
		printf("Writing [%s]\n", filePath.str().c_str());
		MLG::resetFileLogging();
		MLG::writeSdLogLine(unitTestLogWriter);
	}
}

void closeUnitTestLog() {
	if (mslFile != nullptr) {
	  unitTestLogWriter.flush();
		fclose(mslFile);
		mslFile = nullptr;
	}
}
