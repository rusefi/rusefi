// file unit_test_logger.h

#pragma once

#include <stdexcept>

#define TEST_RESULTS_DIR "test_results"

// Per-file size limit for unit test log artifacts. When a writer's output
// exceeds this threshold the writer throws LogsTooLargeException and the
// caller disables logging for the remainder of the run.
static constexpr size_t LOG_FILE_SIZE_LIMIT = 16 * 1024 * 1024; // 16 MB

class LogsTooLargeException : public std::runtime_error {
public:
	explicit LogsTooLargeException(const std::string& msg)
		: std::runtime_error(msg) {}
};

void createUnitTestLog();
void writeUnitTestLogLine();
void closeUnitTestLog();
