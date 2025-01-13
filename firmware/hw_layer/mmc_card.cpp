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

#include "buffered_writer.h"
#include "status_loop.h"
#include "binary_logging.h"

static bool fs_ready = false;

#if EFI_PROD_CODE

int totalLoggedBytes = 0;
static int writeCounter = 0;
static int totalWritesCounter = 0;
static int totalSyncCounter = 0;

#include <stdio.h>
#include <string.h>
#include "mmc_card.h"
#include "ff.h"
#include "mmc_card_util.h"
#include "mass_storage_init.h"
#include "hellen_meta.h"

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
#define SD_STATE_MMC_FAILED "MMC_CONNECT_FAILED"

// todo: shall we migrate to enum with enum2string for consistency? maybe not until we start reading sdStatus?
static const char *sdStatus = SD_STATE_INIT;

// at about 20Hz we write about 2Kb per second, looks like we flush once every ~2 seconds
#define F_SYNC_FREQUENCY 10

/**
 * on't re-read SD card spi device after boot - it could change mid transaction (TS thread could preempt),
 * which will cause disaster (usually multiple-unlock of the same mutex in UNLOCK_SD_SPI)
 */
static spi_device_e mmcSpiDevice = SPI_NONE;

#define RUSEFI_LOG_PREFIX "re_"
#define PREFIX_LEN 3
#define SHORT_TIME_LEN 13

#define FILE_LIST_MAX_COUNT 20

#if HAL_USE_MMC_SPI
/**
 * MMC driver instance.
 */
MMCDriver MMCD1;

/* MMC/SD over SPI driver configuration.*/
static MMCConfig mmccfg = {
	.spip = NULL,
	.lscfg = &mmc_ls_spicfg,
	.hscfg = &mmc_hs_spicfg
};

#if MMC_USE_MUTUAL_EXCLUSION == TRUE
#define LOCK_SD_SPI()
#define UNLOCK_SD_SPI()
#else
#define LOCK_SD_SPI() lockSpi(mmcSpiDevice)
#define UNLOCK_SD_SPI() unlockSpi(mmcSpiDevice)
#endif

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

/* See ff.h FRESULT enum */
static const char *fatErrors[] = {
	"FR_OK: Succeeded",
	"FR_DISK_ERR: A hard error occurred in the low level disk I/O layer",
	"FR_INT_ERR: Assertion failed",
	"FR_NOT_READY: The physical drive cannot work",
	"FR_NO_FILE: Could not find the file",
	"FR_NO_PATH: Could not find the path",
	"FR_INVALID_NAME: The path name format is invalid",
	"FR_DENIED: Access denied due to prohibited access or directory full",
	"FR_EXIST: Access denied due to prohibited access",
	"FR_INVALID_OBJECT: The file/directory object is invalid",
	"FR_WRITE_PROTECTED: The physical drive is write protected",
	"FR_INVALID_DRIVE: The logical drive number is invalid",
	"FR_NOT_ENABLED: The volume has no work area",
	"FR_NO_FILESYSTEM: There is no valid FAT volume",
	"FR_MKFS_ABORTED: The f_mkfs() aborted due to any problem",
	"FR_TIMEOUT: Could not get a grant to access the volume within defined period",
	"FR_LOCKED: The operation is rejected according to the file sharing policy",
	"FR_NOT_ENOUGH_CORE: LFN working buffer could not be allocated",
	"FR_TOO_MANY_OPEN_FILES: Number of open files > FF_FS_LOCK",
	"FR_INVALID_PARAMETER: Given parameter is invalid"
};

// print FAT error function
void printError(const char *str, FRESULT f_error) {
	if (fatFsErrors++ > 16) {
		// no reason to spam the console
		return;
	}

	efiPrintf("FATfs Error \"%s\" %d %s", str, f_error, f_error <= FR_INVALID_PARAMETER ? fatErrors[f_error] : "unknown");
}

static FIL FDLogFile NO_CACHE;

extern int logFileIndex;
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
#if defined(STM32F4XX) || defined(STM32F7XX)
	efiPrintf("HS clock %d Hz", spiGetBaseClock(mmccfg.spip) / (2 << ((mmc_hs_spicfg.cr1 & SPI_CR1_BR_Msk) >> SPI_CR1_BR_Pos)));
	efiPrintf("LS clock %d Hz", spiGetBaseClock(mmccfg.spip) / (2 << ((mmc_ls_spicfg.cr1 & SPI_CR1_BR_Msk) >> SPI_CR1_BR_Pos)));
#endif
	if (isSdCardAlive()) {
		efiPrintf("filename=%s size=%d", logName, totalLoggedBytes);
	}
#if EFI_FILE_LOGGING
	efiPrintf("%d SD card fields", getSdCardFieldsCount());
#endif
}

static void prepareLogFileName() {
	strcpy(logName, RUSEFI_LOG_PREFIX);
	char *ptr;

	// TS SD protocol supports only short 8 symbol file names, good thing that we do not use TS SD protocol!
	bool result = dateToStringShort(&logName[PREFIX_LEN]);

	if (result) {
		ptr = &logName[PREFIX_LEN + SHORT_TIME_LEN];
	} else {
		ptr = itoa10(&logName[PREFIX_LEN], logFileIndex);
	}

	if (engineConfiguration->sdTriggerLog) {
		strcat(ptr, ".teeth");
	} else {
		strcat(ptr, DOT_MLG);
	}
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

	efiPrintf("starting log file %s", logName);
	FRESULT err = f_open(&FDLogFile, logName, FA_OPEN_ALWAYS | FA_WRITE);				// Create new file
	if (err != FR_OK && err != FR_EXIST) {
		sdStatus = SD_STATE_OPEN_FAILED;
		warning(ObdCode::CUSTOM_ERR_SD_MOUNT_FAILED, "SD: mount failed");
		printError("log file create", err);	// else - show error
		return;
	}

	err = f_lseek(&FDLogFile, f_size(&FDLogFile)); // Move to end of the file to append data
	if (err) {
		sdStatus = SD_STATE_SEEK_FAILED;
		warning(ObdCode::CUSTOM_ERR_SD_SEEK_FAILED, "SD: seek failed");
		printError("log file seek", err);
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

// todo: shall we remove this method?
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
		efiPrintf("logfile %lu:%s", (uint32_t)fno.fsize, fno.fname);
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
	blkDisconnect(&MMCD1);						// Brings the driver in a state safe for card removal.
	mmcStop(&MMCD1);							// Disables the MMC peripheral.
	UNLOCK_SD_SPI();
#endif
#ifdef EFI_SDC_DEVICE
	blkDisconnect(&EFI_SDC_DEVICE);
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

	// Configures and activates the MMC peripheral.
	mmcSpiDevice = engineConfiguration->sdCardSpiDevice;

	// todo: reuse initSpiCs method?
	mmc_hs_spicfg.ssport = mmc_ls_spicfg.ssport = getHwPort("mmc", engineConfiguration->sdCardCsPin);
	mmc_hs_spicfg.sspad = mmc_ls_spicfg.sspad = getHwPin("mmc", engineConfiguration->sdCardCsPin);
	mmccfg.spip = getSpiDevice(mmcSpiDevice);

	// Invalid SPI device, abort.
	if (!mmccfg.spip) {
		return nullptr;
	}

	// max SPI rate is 25 MHz after init
	spiCalcClockDiv(mmccfg.spip, &mmc_hs_spicfg, 25 * 1000 * 1000);
	// and 250 KHz during initialization
	spiCalcClockDiv(mmccfg.spip, &mmc_ls_spicfg, 250 * 1000);

	// We think we have everything for the card, let's try to mount it!
	mmcObjectInit(&MMCD1);
	mmcStart(&MMCD1, &mmccfg);

	// Performs the initialization procedure on the inserted card.
	LOCK_SD_SPI();
	sdStatus = SD_STATE_CONNECTING;
	if (blkConnect(&MMCD1) != HAL_SUCCESS) {
		sdStatus = SD_STATE_MMC_FAILED;
		UNLOCK_SD_SPI();
		return nullptr;
	}
	// We intentionally never unlock in case of success, we take exclusive access of that spi device for SD use

	return reinterpret_cast<BaseBlockDevice*>(&MMCD1);
}
#endif /* HAL_USE_MMC_SPI */

#ifndef RE_SDC_MODE
#define RE_SDC_MODE SDC_MODE_4BIT
#endif // RE_SDC_MODE

// Some ECUs are wired for SDIO/SDMMC instead of SPI
#ifdef EFI_SDC_DEVICE
static const SDCConfig sdcConfig = {
	.bus_width = RE_SDC_MODE
};

static BaseBlockDevice* initializeMmcBlockDevice() {
	sdcStart(&EFI_SDC_DEVICE, &sdcConfig);
	sdStatus = SD_STATE_CONNECTING;
	if (blkConnect(&EFI_SDC_DEVICE) != HAL_SUCCESS) {
		sdStatus = SD_STATE_NOT_CONNECTED;
		return nullptr;
	}

	return reinterpret_cast<BaseBlockDevice*>(&EFI_SDC_DEVICE);
}
#endif /* EFI_SDC_DEVICE */

#if HAL_USE_USB_MSD
static bool useMsdMode() {
  if (needsToWriteReportFile()) {
    return false;
  }
  if (engineConfiguration->alwaysWriteSdCard) {
    return false;
  }
	// Wait for the USB stack to wake up, or a 15 second timeout, whichever occurs first
	msg_t usbResult = usbConnectedSemaphore.wait(TIME_MS2I(15000));

	return usbResult == MSG_OK;
}
#endif // HAL_USE_USB_MSD

// Initialize and mount the SD card.
// Returns true if the filesystem was successfully mounted for writing.
static bool mountMmc() {
	auto cardBlockDevice = initializeMmcBlockDevice();

#if EFI_TUNER_STUDIO
	// If not null, card is present
	engine->outputChannels.sd_present = cardBlockDevice != nullptr;
#endif

	// if no card, don't try to mount FS
	if (!cardBlockDevice) {
		return false;
	}

#if HAL_USE_USB_MSD
	// If we have a device AND USB is connected, mount the card to USB, otherwise
	// mount the null device and try to mount the filesystem ourselves
	if (useMsdMode()) {
		// Mount the real card to USB
		attachMsdSdCard(cardBlockDevice);

		sdStatus = SD_STATE_MSD;
		// At this point we're done: don't try to write files ourselves
		return false;
	}
#endif

	// We were able to connect the SD card, mount the filesystem
	memset(&MMC_FS, 0, sizeof(FATFS));
	if (f_mount(&MMC_FS, "/", 1) == FR_OK) {
		efiPrintf("MMC/SD mounted!");
		sdStatus = SD_STATE_MOUNTED;
		incLogFileName();
		writeErrorReportFile();
		createLogFile();
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
			printError("log file write", err);

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

#else // not EFI_PROD_CODE (simulator)

#include <fstream>

bool mountMmc() {
	// Stub so the loop thinks the MMC mounted OK
	return true;
}

class SdLogBufferWriter final : public BufferedWriter<512> {
public:
	bool failed = false;

	SdLogBufferWriter()
		: m_stream("rusefi_simulator_log.mlg", std::ios::binary | std::ios::trunc)
	{
		fs_ready = true;
	}

	size_t writeInternal(const char* buffer, size_t count) override {
		m_stream.write(buffer, count);
		m_stream.flush();
		return count;
	}

private:
	std::ofstream m_stream;
};

#endif // EFI_PROD_CODE

static NO_CACHE SdLogBufferWriter logBuffer;

// Log 'regular' ECU log to MLG file
static void mlgLogger();

// Log binary trigger log
static void sdTriggerLogger();

static THD_WORKING_AREA(mmcThreadStack, 3 * UTILITY_THREAD_STACK_SIZE);		// MMC monitor thread
static THD_FUNCTION(MMCmonThread, arg) {
	(void)arg;
	chRegSetThreadName("MMC Card Logger");

#if HW_HELLEN && EFI_PROD_CODE
	// on mega-module we manage SD card power supply
	while (!getHellenBoardEnabled()) {
		// wait until board enables peripheral
		chThdSleepMilliseconds(100);
		if (getTimeNowS() > 4 && !isIgnVoltage()) {
			// looks like vehicle is OFF and we are hooked to USB - turn on peripheral to get Mass Storage Device USB profile
			efiPrintf("    *** turning board ON to power SD card ***");
			hellenEnableEn();
			break;
		}
	}
	chThdSleepMilliseconds(300);
#endif

	if (mountMmc()) {
		#if EFI_TUNER_STUDIO
			engine->outputChannels.sd_logging_internal = true;
		#endif

		if (engineConfiguration->sdTriggerLog) {
			sdTriggerLogger();
		} else {
			mlgLogger();
		}
	} else {
		// no card present (or mounted via USB), don't do internal logging
	}

	efiPrintf("SD logger has died!");

	// exiting thread will create zombie!
	while(1) {
		chThdSleepMilliseconds(100);
	}
}

void mlgLogger() {
	while (true) {
		// if the SPI device got un-picked somehow, cancel SD card
		// Don't do this check at all if using SDMMC interface instead of SPI
#if EFI_PROD_CODE && !defined(EFI_SDC_DEVICE)
		if (engineConfiguration->sdCardSpiDevice == SPI_NONE) {
			return;
		}
#endif

		systime_t before = chVTGetSystemTime();

		writeSdLogLine(logBuffer);

		// Something went wrong (already handled), so cancel further writes
		if (logBuffer.failed) {
			return;
		}

		auto freq = engineConfiguration->sdCardLogFrequency;
		if (freq > 250) {
			freq = 250;
		} else if (freq < 1) {
			freq = 1;
		}

		systime_t period = CH_CFG_ST_FREQUENCY / freq;
		chThdSleepUntilWindowed(before, before + period);
	}
}

static void sdTriggerLogger() {
#if EFI_TOOTH_LOGGER
	EnableToothLogger();

	while (true) {
		auto buffer = GetToothLoggerBufferBlocking();

		// can return nullptr
		if (buffer) {
			logBuffer.write(reinterpret_cast<const char*>(buffer->buffer), buffer->nextIdx * sizeof(composite_logger_s));

			ReturnToothLoggerBuffer(buffer);
		}
	}
#endif /* EFI_TOOTH_LOGGER */
}

bool isSdCardAlive(void) {
	return fs_ready;
}

void updateSdCardLiveFlags() {
#if HAL_USE_MMC_SPI
	engine->outputChannels.sd_active_wr = (MMCD1.state == BLK_WRITING);
	engine->outputChannels.sd_active_rd = (MMCD1.state == BLK_READING);
#endif
#ifdef EFI_SDC_DEVICE
	engine->outputChannels.sd_active_wr = (EFI_SDC_DEVICE.state == BLK_WRITING);
	engine->outputChannels.sd_active_rd = (EFI_SDC_DEVICE.state == BLK_READING);
#endif
}

// Pre-config load init
void initEarlyMmcCard() {
#if EFI_PROD_CODE
	logName[0] = 0;

	addConsoleAction("sdinfo", sdStatistics);
	addConsoleActionS("ls", listDirectory);
	addConsoleActionS("del", removeFile);
	addConsoleAction("incfilename", incLogFileName);
#endif // EFI_PROD_CODE
}

static bool isSdCardEnabled() {
  return engineConfiguration->isSdCardEnabled &&
    engineConfiguration->sdCardSpiDevice != SPI_NONE &&
    isBrainPinValid(engineConfiguration->sdCardCsPin);
}

void initMmcCard() {
	if (!isSdCardEnabled()) {
		// do not even bother starting the thread if SD card is not enabled & configured on start-up
		return;
	}
	chThdCreateStatic(mmcThreadStack, sizeof(mmcThreadStack), PRIO_MMC, (tfunc_t)(void*) MMCmonThread, NULL);
}

#endif /* EFI_FILE_LOGGING */
