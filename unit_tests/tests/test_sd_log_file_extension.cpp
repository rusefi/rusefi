#include "pch.h"

#include "sd_log_file_extension.h"

// [tag:tooth_log_extension] binary and CSV trigger logs must not share an extension
TEST(SdLogFileExtension, TriggerLogFormatsDiffer) {
	EXPECT_STREQ(".teeth", getSdLogFileExtension(SDLoggerMode::ToothCsv));
	EXPECT_STREQ(".teethbin", getSdLogFileExtension(SDLoggerMode::ToothBin));
	EXPECT_STRNE(getSdLogFileExtension(SDLoggerMode::ToothCsv), getSdLogFileExtension(SDLoggerMode::ToothBin));
}

TEST(SdLogFileExtension, OperatingDataLogsAreMlg) {
	EXPECT_STREQ(".mlg", getSdLogFileExtension(SDLoggerMode::Mlg));
	EXPECT_STREQ(".mlg", getSdLogFileExtension(SDLoggerMode::Dtc));
}
