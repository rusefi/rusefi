// file unit_test_logger.h

#pragma once

#include <stdexcept>

#define TEST_RESULTS_DIR "test_results"

class LogsTooLargeException : public std::runtime_error {
public:
	explicit LogsTooLargeException(const std::string& msg)
		: std::runtime_error(msg) {}
};

void createUnitTestLog();
void writeUnitTestLogLine();
void closeUnitTestLog();

// Number of unit test runs whose logs were aborted because a writer exceeded
// LOG_FILE_SIZE_LIMIT (the .msl/.csv/.ndjson files for that run are removed
// when this happens).
size_t getAbortedLogsCount();
// Total size (in bytes) of the log files that were removed due to exceeding
// LOG_FILE_SIZE_LIMIT across all aborted runs.
size_t getAbortedLogsTotalBytes();
