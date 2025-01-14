#include "pch.h"

#if EFI_PROD_CODE && EFI_FILE_LOGGING

#include "ff.h"
#include "mmc_card_util.h"
#include "backup_ram.h"
#include "mmc_card.h"

#define LOG_INDEX_FILENAME "index.txt"
#define HARD_FAULT_PREFIX "hard_"

// 10 because we want at least 4 character name (is that about TS protocol which we do not use any more?)
#define MIN_FILE_INDEX 10
int logFileIndex = MIN_FILE_INDEX;

static FIL FDLogFile NO_CACHE;

void incLogFileName() {
	memset(&FDLogFile, 0, sizeof(FIL));						// clear the memory
	FRESULT ret = f_open(&FDLogFile, LOG_INDEX_FILENAME, FA_READ);				// This file has the index for next log file name

	char data[_MAX_FILLER];
	memset(data, 0, sizeof(data));

	if (ret == FR_OK) {
		UINT readed = 0;
		// leave one byte for terminating 0
		ret = f_read(&FDLogFile, (void*)data, sizeof(data) - 1, &readed);

		if (ret != FR_OK) {
			printError("log index file read", ret);
			logFileIndex = MIN_FILE_INDEX;
		} else {
			efiPrintf("Got content [%s] size %d", data, readed);
			logFileIndex = maxI(MIN_FILE_INDEX, atoi(data));
			if (absI(logFileIndex) == ATOI_ERROR_CODE) {
				logFileIndex = MIN_FILE_INDEX;
			} else {
				logFileIndex++; // next file would use next file name
			}
		}
		f_close(&FDLogFile);
	} else if (ret == FR_NO_FILE) {
		// no index file - this is not an error, just an empty SD
		logFileIndex = MIN_FILE_INDEX;
	} else {
		printError("log index file open", ret);
		efiPrintf("%s: not found or error: %d", LOG_INDEX_FILENAME, ret);
		logFileIndex = MIN_FILE_INDEX;
	}

	// truncate or create new
	ret = f_open(&FDLogFile, LOG_INDEX_FILENAME, FA_CREATE_ALWAYS | FA_WRITE);
	if (ret == FR_OK) {
		UINT writen = 0;
		size_t len = itoa10(data, logFileIndex) - data;
		ret = f_write(&FDLogFile, (void*)data, len, &writen);
		if ((ret != FR_OK) || (len != writen)) {
			printError("log index write", ret);
		}
		f_close(&FDLogFile);
	} else {
		printError("log index file write", ret);
	}

	efiPrintf("New log file index %d", logFileIndex);
}

bool needsToWriteReportFile() {
#if EFI_BACKUP_SRAM
extern ErrorCookie errorCookieOnStart;
  return errorCookieOnStart == ErrorCookie::HardFault;
#else
  return false;
#endif // EFI_BACKUP_SRAM
}

PUBLIC_API_WEAK void onBoardWriteErrorFile(FIL *file) {
  UNUSED(file);
}

#if EFI_BACKUP_SRAM
// todo: unit test? or better file stdlib method to do same?
static int mystrncasecmp(const char *s1, const char *s2, size_t n) {
	if (n != 0) {
		const char *us1 = (const char *)s1;
		const char *us2 = (const char *)s2;

		do {
			if (mytolower(*us1) != mytolower(*us2))
				return (mytolower(*us1) - mytolower(*us2));
			if (*us1++ == '\0')
				break;
			us2++;
		} while (--n != 0);
	}
	return (0);
}

static bool findFile(const char *path, const char *prefix) {
	if (!isSdCardAlive()) {
		efiPrintf("Error: No File system is mounted");
		return false;
	}

	DIR dir;
	FRESULT res = f_opendir(&dir, path);

	if (res != FR_OK) {
		efiPrintf("Error opening directory %s", path);
		return false;
	}

  // todo: do we need paranoia loop limit?
	while (true) {
		FILINFO fno;

		res = f_readdir(&dir, &fno);
		if (res != FR_OK || fno.fname[0] == 0) {
		  // done!
			break;
		}
		if (fno.fname[0] == '.') {
			continue;
		}
		if ((fno.fattrib & AM_DIR) || mystrncasecmp(prefix, fno.fname, strlen(prefix) - 1)) {
		  // skipping folders and files with wrong names
			continue;
		}
		efiPrintf("file found %lu:%s", (uint32_t)fno.fsize, fno.fname);
		return true;

//			efiPrintf("%c%c%c%c%c %u/%02u/%02u %02u:%02u %9lu  %-12s", (fno.fattrib & AM_DIR) ? 'D' : '-',
//					(fno.fattrib & AM_RDO) ? 'R' : '-', (fno.fattrib & AM_HID) ? 'H' : '-',
//					(fno.fattrib & AM_SYS) ? 'S' : '-', (fno.fattrib & AM_ARC) ? 'A' : '-', (fno.fdate >> 9) + 1980,
//					(fno.fdate >> 5) & 15, fno.fdate & 31, (fno.ftime >> 11), (fno.ftime >> 5) & 63, fno.fsize,
//					fno.fname);
	}
	return false;
}
#endif // EFI_BACKUP_SRAM

void writeErrorReportFile() {
#if EFI_BACKUP_SRAM
extern ErrorCookie errorCookieOnStart;
  static char fileName[_MAX_FILLER + 20];
  auto sramState = getBackupSram();
  if (errorCookieOnStart == ErrorCookie::HardFault) {
  	memset(&FDLogFile, 0, sizeof(FIL));						// clear the memory
  	sprintf(fileName, "%s%d.txt", HARD_FAULT_PREFIX, logFileIndex);
  	FRESULT ret = f_open(&FDLogFile, fileName, FA_CREATE_ALWAYS | FA_WRITE);
  	if (ret == FR_OK) {
  	  onBoardWriteErrorFile(&FDLogFile);
  	  f_printf(&FDLogFile, "type=%d\n", sramState->Err.FaultType);
  	  // todo: figure out what else would be useful
  	  f_close(&FDLogFile);
  	}
  }
  findFile(".", HARD_FAULT_PREFIX);
#endif // EFI_BACKUP_SRAM
}

#endif // EFI_PROD_CODE && EFI_FILE_LOGGING
