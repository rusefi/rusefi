/**
 * @file sd_log_file_extension.h
 * @brief File name extension of an SD card log, per logger mode
 */

#pragma once

#include "rusefi_enums.h"

#define DOT_MLG ".mlg"
// [tag:tooth_log_extension] The two trigger tooth log formats need different extensions
// so a file can be handed to the right tool without inspecting its contents (#10697).
// CSV keeps ".teeth": that is what unit-test captures and CSV readers already use.
// Raw composite_logger_s records are for misc/tooth_log_converter.
#define DOT_TEETH_CSV ".teeth"
#define DOT_TEETH_BIN ".teethbin"

constexpr const char* getSdLogFileExtension(SDLoggerMode mode) {
	switch (mode) {
	case SDLoggerMode::ToothBin:
		return DOT_TEETH_BIN;
	case SDLoggerMode::ToothCsv:
		return DOT_TEETH_CSV;
	default:
		return DOT_MLG;
	}
}
