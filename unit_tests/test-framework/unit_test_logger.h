// file unit_test_logger.h

#pragma once

#include <stdexcept>

#define TEST_RESULTS_DIR "test_results"

// Default per-file size limit for unit test log artifacts. When a writer's
// output exceeds the active limit (see getLogFileSizeLimit) the writer throws
// LogsTooLargeException and the caller disables logging for the rest of the
// run.
static constexpr size_t LOG_FILE_SIZE_LIMIT_DEFAULT = 16 * 1024 * 1024;

// Active per-file size limit. Individual tests that legitimately produce
// larger artifacts may bump this via setLogFileSizeLimit() for the duration
// of the test (remember to restore the previous value afterwards).
size_t getLogFileSizeLimit();
void setLogFileSizeLimit(size_t bytes);

// RAII helper: bump the per-file unit-test log size limit for the duration of
// a test, restoring the previous value on scope exit. Use this for tests that
// legitimately produce log artifacts larger than LOG_FILE_SIZE_LIMIT_DEFAULT
// (16 MB) instead of disabling logs entirely.
struct ScopedLogFileSizeLimit {
	size_t saved;
	explicit ScopedLogFileSizeLimit(size_t bytes) : saved(getLogFileSizeLimit()) {
		setLogFileSizeLimit(bytes);
	}
	~ScopedLogFileSizeLimit() { setLogFileSizeLimit(saved); }
};

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
