#include "pch.h"

#if EFI_PROD_CODE && EFI_FILE_LOGGING

#include "past_codes_storage.h"
#include "past_codes.h"
#include "mmc_card_util.h"

// Only the MMC thread touches this descriptor/buffer, under board SD ownership.
// Separate from resources.fd since regular logging can have that file open.
static NO_CACHE FIL pastCodesFile;
static NO_CACHE char pastCodesCsv[PastCodeHistory::CsvBufferSize];
static bool pastCodesLoaded = false;
static bool pastCodesRetryPending = false;
static Timer pastCodesRetryTimer;
static constexpr const char* PastCodesFilename = "past-codes.csv";

FRESULT sdServicePastCodes() {
	if (pastCodesRetryPending && !pastCodesRetryTimer.hasElapsedSec(1)) {
		return FR_OK;
	}
	auto& history = engine->engineState.warnings.pastCodes;
	FRESULT result = FR_OK;
	if (!pastCodesLoaded) {
		result = f_open(&pastCodesFile, PastCodesFilename, FA_READ);
		if (result == FR_NO_FILE) {
			pastCodesLoaded = history.load("", 0);
			result = FR_OK;
		} else if (result == FR_OK) {
			UINT bytesRead = 0;
			result = f_read(&pastCodesFile, pastCodesCsv, sizeof(pastCodesCsv), &bytesRead);
			const bool complete = f_eof(&pastCodesFile);
			const FRESULT closeResult = f_close(&pastCodesFile);
			if (closeResult != FR_OK) {
				return closeResult;
			}
			if (result == FR_OK) {
				pastCodesLoaded = complete && history.load(pastCodesCsv, bytesRead);
				if (!pastCodesLoaded) {
					result = FR_INVALID_PARAMETER;
				}
			}
		}
	}

	PastCodeHistory::Snapshot snapshot;
	if (pastCodesLoaded && history.snapshotIfDirty(snapshot)) {
		const size_t size = PastCodeHistory::format(snapshot, pastCodesCsv);
		result = f_open(&pastCodesFile, PastCodesFilename, FA_CREATE_ALWAYS | FA_WRITE);
		if (result == FR_OK) {
			UINT bytesWritten = 0;
			result = f_write(&pastCodesFile, pastCodesCsv, size, &bytesWritten);
			const FRESULT closeResult = f_close(&pastCodesFile);
			if (closeResult != FR_OK) {
				return closeResult;
			}
			if (result == FR_OK && bytesWritten != size) {
				result = FR_DISK_ERR;
			}
			if (result == FR_OK) {
				history.markSaved(snapshot);
			}
		}
	}

	pastCodesRetryPending = result != FR_OK;
	if (pastCodesRetryPending) {
		// Keep the dirty state on failure; avoid tight retries or new warning codes.
		pastCodesRetryTimer.reset();
		printFatFsError("past codes", result);
	}
	return FR_OK;
}

#endif // EFI_PROD_CODE && EFI_FILE_LOGGING
