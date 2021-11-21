/**
 * @file	mmc_card.cpp
 *
 * @date Dec 28, 2013
 * @author Kot_dnz
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 * default pinouts in case of SPI2 connected to MMC: PB13 - SCK, PB14 - MISO, PB15 - MOSI, PD4 - CS, 3.3v
 * default pinouts in case of SPI3 connected to MMC: PB3  - SCK, PB4  - MISO, PB5  - MOSI, PD4 - CS, 3.3v
 *
 *
 * todo: extract some logic into a controller file
 */

#include "pch.h"

#if EFI_FILE_LOGGING

#include "ch.hpp"
#include <stdio.h>
#include <string.h>
#include "mmc_card.h"
#include "ff.h"
#include "hardware.h"
#include "status_loop.h"
#include "buffered_writer.h"
#include "mass_storage_init.h"

#include "rtc_helper.h"

#define SD_STATE_INIT "init"
#define SD_STATE_MOUNTED "MOUNTED"
#define SD_STATE_MOUNT_FAILED "MOUNT_FAILED"
#define SD_STATE_OPEN_FAILED "OPEN_FAILED"
#define SD_STATE_SEEK_FAILED "SEEK_FAILED"
#define SD_STATE_NOT_INSERTED "NOT_INSERTED"
#define SD_STATE_CONNECTING "CONNECTING"
#define SD_STATE_MSD "MSD"
#define SD_STATE_NOT_CONNECTED "NOT_CONNECTED"

// todo: shall we migrate to enum with enum2string for consistency? maybe not until we start reading sdStatus?
static const char *sdStatus = SD_STATE_INIT;
static bool fs_ready = false;

// at about 20Hz we write about 2Kb per second, looks like we flush once every ~2 seconds
#define F_SYNC_FREQUENCY 10

int totalLoggedBytes = 0;
static int fileCreatedCounter = 0;
static int writeCounter = 0;
static int totalWritesCounter = 0;
static int totalSyncCounter = 0;

/**
 * on't re-read SD card spi device after boot - it could change mid transaction (TS thread could preempt),
 * which will cause disaster (usually multiple-unlock of the same mutex in UNLOCK_SD_SPI)
 */
spi_device_e mmcSpiDevice = SPI_NONE;

#define LOG_INDEX_FILENAME "index.txt"

#define RUSEFI_LOG_PREFIX "re_"
#define PREFIX_LEN 3
#define SHORT_TIME_LEN 13

#define LS_RESPONSE "ls_result"
#define FILE_LIST_MAX_COUNT 20

static THD_WORKING_AREA(mmcThreadStack, 3 * UTILITY_THREAD_STACK_SIZE);		// MMC monitor thread

#if HAL_USE_MMC_SPI
/**
 * MMC driver instance.
 */
MMCDriver MMCD1;

/* MMC/SD over SPI driver configuration.*/
static MMCConfig mmccfg = { NULL, &mmc_ls_spicfg, &mmc_hs_spicfg };

#define LOCK_SD_SPI lockSpi(mmcSpiDevice)
#define UNLOCK_SD_SPI unlockSpi(mmcSpiDevice)

#endif /* HAL_USE_MMC_SPI */

/**
 * fatfs MMC/SPI
 */
static NO_CACHE FATFS MMC_FS;

static int fatFsErrors = 0;

static void mmcUnMount();

static void setSdCardReady(bool value) {
	fs_ready = value;
}

// print FAT error function
static void printError(const char *str, FRESULT f_error) {
	if (fatFsErrors++ > 16) {
		// no reason to spam the console
		return;
	}

	efiPrintf("FATfs Error \"%s\" %d", str, f_error);
}

static FIL FDLogFile NO_CACHE;

// 10 because we want at least 4 character name
#define MIN_FILE_INDEX 10
static int logFileIndex = MIN_FILE_INDEX;
static char logName[_MAX_FILLER + 20];

static void printMmcPinout() {
	efiPrintf("MMC CS %s", hwPortname(engineConfiguration->sdCardCsPin));
	// todo: we need to figure out the right SPI pinout, not just SPI2
//	efiPrintf("MMC SCK %s:%d", portname(EFI_SPI2_SCK_PORT), EFI_SPI2_SCK_PIN);
//	efiPrintf("MMC MISO %s:%d", portname(EFI_SPI2_MISO_PORT), EFI_SPI2_MISO_PIN);
//	efiPrintf("MMC MOSI %s:%d", portname(EFI_SPI2_MOSI_PORT), EFI_SPI2_MOSI_PIN);
}

static void sdStatistics() {
	printMmcPinout();
	efiPrintf("SD enabled=%s status=%s", boolToString(engineConfiguration->isSdCardEnabled),
			sdStatus);
	printSpiConfig("SD", mmcSpiDevice);
	if (isSdCardAlive()) {
		efiPrintf("filename=%s size=%d", logName, totalLoggedBytes);
	}
}

static void incLogFileName() {
	memset(&FDLogFile, 0, sizeof(FIL));						// clear the memory
	FRESULT err = f_open(&FDLogFile, LOG_INDEX_FILENAME, FA_READ);				// This file has the index for next log file name

	char data[_MAX_FILLER];
	UINT result = 0;
	if (err != FR_OK && err != FR_EXIST) {
			logFileIndex = MIN_FILE_INDEX;
			efiPrintf("%s: not found or error: %d", LOG_INDEX_FILENAME, err);
	} else {
		f_read(&FDLogFile, (void*)data, sizeof(data), &result);

		efiPrintf("Got content [%s] size %d", data, result);
		f_close(&FDLogFile);
		if (result < 5) {
            data[result] = 0;
			logFileIndex = maxI(MIN_FILE_INDEX, atoi(data));
			if (absI(logFileIndex) == ERROR_CODE) {
				logFileIndex = MIN_FILE_INDEX;
			} else {
				logFileIndex++; // next file would use next file name
			}
		} else {
			logFileIndex = MIN_FILE_INDEX;
		}
	}

	err = f_open(&FDLogFile, LOG_INDEX_FILENAME, FA_OPEN_ALWAYS | FA_WRITE);
	itoa10(data, logFileIndex);
	f_write(&FDLogFile, (void*)data, strlen(data), &result);
	f_close(&FDLogFile);
	efiPrintf("Done %d", logFileIndex);
}

static void prepareLogFileName() {
	strcpy(logName, RUSEFI_LOG_PREFIX);
	char *ptr;

#if HAL_USE_USB_MSD
	bool result = dateToStringShort(&logName[PREFIX_LEN]);
#else 
	// TS SD protocol supports only short 8 symbol file names :(
	bool result = false;
#endif

	if (result) {
		ptr = &logName[PREFIX_LEN + SHORT_TIME_LEN];
	} else {
		ptr = itoa10(&logName[PREFIX_LEN], logFileIndex);
	}

	strcat(ptr, DOT_MLG);
}

/**
 * @brief Create a new file with the specified name
 *
 * This function saves the name of the file in a global variable
 * so that we can later append to that file
 */
static void createLogFile() {
	memset(&FDLogFile, 0, sizeof(FIL));						// clear the memory
	prepareLogFileName();

	FRESULT err = f_open(&FDLogFile, logName, FA_OPEN_ALWAYS | FA_WRITE);				// Create new file
	if (err != FR_OK && err != FR_EXIST) {
		sdStatus = SD_STATE_OPEN_FAILED;
		warning(CUSTOM_ERR_SD_MOUNT_FAILED, "SD: mount failed");
		printError("FS mount failed", err);	// else - show error
		return;
	}

	err = f_lseek(&FDLogFile, f_size(&FDLogFile)); // Move to end of the file to append data
	if (err) {
		sdStatus = SD_STATE_SEEK_FAILED;
		warning(CUSTOM_ERR_SD_SEEK_FAILED, "SD: seek failed");
		printError("Seek error", err);
		return;
	}
	f_sync(&FDLogFile);
	setSdCardReady(true);						// everything Ok
}

static void removeFile(const char *pathx) {
	if (!isSdCardAlive()) {
		efiPrintf("Error: No File system is mounted");
		return;
	}

	f_unlink(pathx);
}

int mystrncasecmp(const char *s1, const char *s2, size_t n) {
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

static void listDirectory(const char *path) {

	if (!isSdCardAlive()) {
		efiPrintf("Error: No File system is mounted");
		return;
	}

	DIR dir;
	FRESULT res = f_opendir(&dir, path);

	if (res != FR_OK) {
		efiPrintf("Error opening directory %s", path);
		return;
	}

	efiPrintf(LS_RESPONSE);

	for (int count = 0;count < FILE_LIST_MAX_COUNT;) {
		FILINFO fno;

		res = f_readdir(&dir, &fno);
		if (res != FR_OK || fno.fname[0] == 0) {
			break;
		}
		if (fno.fname[0] == '.') {
			continue;
		}
		if ((fno.fattrib & AM_DIR) || mystrncasecmp(RUSEFI_LOG_PREFIX, fno.fname, sizeof(RUSEFI_LOG_PREFIX) - 1)) {
			continue;
		}
		efiPrintf("logfile%lu:%s", fno.fsize, fno.fname);
		count++;

//			efiPrintf("%c%c%c%c%c %u/%02u/%02u %02u:%02u %9lu  %-12s", (fno.fattrib & AM_DIR) ? 'D' : '-',
//					(fno.fattrib & AM_RDO) ? 'R' : '-', (fno.fattrib & AM_HID) ? 'H' : '-',
//					(fno.fattrib & AM_SYS) ? 'S' : '-', (fno.fattrib & AM_ARC) ? 'A' : '-', (fno.fdate >> 9) + 1980,
//					(fno.fdate >> 5) & 15, fno.fdate & 31, (fno.ftime >> 11), (fno.ftime >> 5) & 63, fno.fsize,
//					fno.fname);
	}
}

/*
 * MMC card un-mount.
 */
static void mmcUnMount() {
	if (!isSdCardAlive()) {
		efiPrintf("Error: No File system is mounted. \"mountsd\" first");
		return;
	}
	f_close(&FDLogFile);						// close file
	f_sync(&FDLogFile);							// sync ALL

#if HAL_USE_MMC_SPI
	mmcDisconnect(&MMCD1);						// Brings the driver in a state safe for card removal.
	mmcStop(&MMCD1);							// Disables the MMC peripheral.
	UNLOCK_SD_SPI;
#endif
#ifdef EFI_SDC_DEVICE
	sdcDisconnect(&EFI_SDC_DEVICE);
	sdcStop(&EFI_SDC_DEVICE);
#endif
	f_mount(NULL, 0, 0);						// FATFS: Unregister work area prior to discard it
	memset(&FDLogFile, 0, sizeof(FIL));			// clear FDLogFile
	setSdCardReady(false);						// status = false
	efiPrintf("MMC/SD card removed");
}

#if HAL_USE_USB_MSD

static chibios_rt::BinarySemaphore usbConnectedSemaphore(/* taken =*/ true);

void onUsbConnectedNotifyMmcI() {
	usbConnectedSemaphore.signalI();
}

#endif /* HAL_USE_USB_MSD */

#if HAL_USE_MMC_SPI
/*
 * Attempts to initialize the MMC card.
 * Returns a BaseBlockDevice* corresponding to the SD card if successful, otherwise nullptr.
 */
static BaseBlockDevice* initializeMmcBlockDevice() {
	// Don't try to mount SD card in case of fatal error - hardware may be in an unexpected state
	if (hasFirmwareError()) {
		return nullptr;
	}
	
	if (!engineConfiguration->isSdCardEnabled) {
		return nullptr;
	}

	// Configures and activates the MMC peripheral.
	mmcSpiDevice = engineConfiguration->sdCardSpiDevice;

	efiAssert(OBD_PCM_Processor_Fault, mmcSpiDevice != SPI_NONE, "SD card enabled, but no SPI device configured!", nullptr);

	// todo: reuse initSpiCs method?
	mmc_hs_spicfg.ssport = mmc_ls_spicfg.ssport = getHwPort("mmc", engineConfiguration->sdCardCsPin);
	mmc_hs_spicfg.sspad = mmc_ls_spicfg.sspad = getHwPin("mmc", engineConfiguration->sdCardCsPin);
	mmccfg.spip = getSpiDevice(mmcSpiDevice);

	// Invalid SPI device, abort.
	if (!mmccfg.spip) {
		return nullptr;
	}

	// We think we have everything for the card, let's try to mount it!
	mmcObjectInit(&MMCD1);
	mmcStart(&MMCD1, &mmccfg);

	// Performs the initialization procedure on the inserted card.
	LOCK_SD_SPI;
	sdStatus = SD_STATE_CONNECTING;
	if (mmcConnect(&MMCD1) != HAL_SUCCESS) {
		sdStatus = SD_STATE_NOT_CONNECTED;
		UNLOCK_SD_SPI;
		return nullptr;
	}
	// We intentionally never unlock in case of success, we take exclusive access of that spi device for SD use

	return reinterpret_cast<BaseBlockDevice*>(&MMCD1);
}
#endif /* HAL_USE_MMC_SPI */

// Some ECUs are wired for SDIO/SDMMC instead of SPI
#ifdef EFI_SDC_DEVICE
static const SDCConfig sdcConfig = {
	SDC_MODE_4BIT
};

static BaseBlockDevice* initializeMmcBlockDevice() {
	if (!engineConfiguration->isSdCardEnabled) {
		return nullptr;
	}

	sdcStart(&EFI_SDC_DEVICE, &sdcConfig);
	sdStatus = SD_STATE_CONNECTING;
	if (sdcConnect(&EFI_SDC_DEVICE) != HAL_SUCCESS) {
		sdStatus = SD_STATE_NOT_CONNECTED;
		return nullptr;
	}

	return reinterpret_cast<BaseBlockDevice*>(&EFI_SDC_DEVICE);
}
#endif /* EFI_SDC_DEVICE */

// Initialize and mount the SD card.
// Returns true if the filesystem was successfully mounted for writing.
static bool mountMmc() {
	auto cardBlockDevice = initializeMmcBlockDevice();

#if EFI_TUNER_STUDIO
	// If not null, card is present
	tsOutputChannels.sd_present = cardBlockDevice != nullptr;
#endif

#if HAL_USE_USB_MSD
	// Wait for the USB stack to wake up, or a 5 second timeout, whichever occurs first
	msg_t usbResult = usbConnectedSemaphore.wait(TIME_MS2I(5000));

	bool hasUsb = usbResult == MSG_OK;

	// If we have a device AND USB is connected, mount the card to USB, otherwise
	// mount the null device and try to mount the filesystem ourselves
	if (cardBlockDevice && hasUsb) {
		// Mount the real card to USB
		attachMsdSdCard(cardBlockDevice);

		sdStatus = SD_STATE_MSD;
		// At this point we're done: don't try to write files ourselves
		return false;
	}
#endif

	// if no card, don't try to mount FS
	if (!cardBlockDevice) {
		return false;
	}

	// We were able to connect the SD card, mount the filesystem
	memset(&MMC_FS, 0, sizeof(FATFS));
	if (f_mount(&MMC_FS, "/", 1) == FR_OK) {
		sdStatus = SD_STATE_MOUNTED;
		incLogFileName();
		createLogFile();
		fileCreatedCounter++;
		efiPrintf("MMC/SD mounted!");
		return true;
	} else {
		sdStatus = SD_STATE_MOUNT_FAILED;
		return false;
	}
}

struct SdLogBufferWriter final : public BufferedWriter<512> {
	bool failed = false;

	size_t writeInternal(const char* buffer, size_t count) override {
		size_t bytesWritten;

		totalLoggedBytes += count;

		FRESULT err = f_write(&FDLogFile, buffer, count, &bytesWritten);

		if (bytesWritten != count) {
			printError("write error or disk full", err);

			// Close file and unmount volume
			mmcUnMount();
			failed = true;
			return 0;
		} else {
			writeCounter++;
			totalWritesCounter++;
			if (writeCounter >= F_SYNC_FREQUENCY) {
				/**
				 * Performance optimization: not f_sync after each line, f_sync is probably a heavy operation
				 * todo: one day someone should actually measure the relative cost of f_sync
				 */
				f_sync(&FDLogFile);
				totalSyncCounter++;
				writeCounter = 0;
			}
		}

		return bytesWritten;
	}
};

static NO_CACHE SdLogBufferWriter logBuffer;

static THD_FUNCTION(MMCmonThread, arg) {
	(void)arg;
	chRegSetThreadName("MMC Card Logger");

	if (!mountMmc()) {
		// no card present (or mounted via USB), don't do internal logging
		return;
	}

	#if EFI_TUNER_STUDIO
		tsOutputChannels.sd_logging_internal = true;
	#endif

	while (true) {
		// if the SPI device got un-picked somehow, cancel SD card
		if (engineConfiguration->sdCardSpiDevice == SPI_NONE) {
			return;
		}

		if (engineConfiguration->debugMode == DBG_SD_CARD) {
			tsOutputChannels.debugIntField1 = totalLoggedBytes;
			tsOutputChannels.debugIntField2 = totalWritesCounter;
			tsOutputChannels.debugIntField3 = totalSyncCounter;
			tsOutputChannels.debugIntField4 = fileCreatedCounter;
		}

		writeLogLine(logBuffer);

		// Something went wrong (already handled), so cancel further writes
		if (logBuffer.failed) {
			return;
		}

		auto period = engineConfiguration->sdCardPeriodMs;
		if (period > 0) {
			chThdSleepMilliseconds(period);
		}
	}
}

bool isSdCardAlive(void) {
	return fs_ready;
}

// Pre-config load init
void initEarlyMmcCard() {
	logName[0] = 0;

	addConsoleAction("sdinfo", sdStatistics);
	addConsoleActionS("ls", listDirectory);
	addConsoleActionS("del", removeFile);
	addConsoleAction("incfilename", incLogFileName);
}

void initMmcCard() {
	chThdCreateStatic(mmcThreadStack, sizeof(mmcThreadStack), PRIO_MMC, (tfunc_t)(void*) MMCmonThread, NULL);
}

#endif /* EFI_FILE_LOGGING */
