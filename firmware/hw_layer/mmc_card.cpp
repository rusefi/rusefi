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
#include "binary_mlg_logging.h"

// Divide logs into 32Mb chunks.
// With this opstion defined SW will pre-allocate file with given size and
// should not touch FAT structures until file is fully filled
// This should protect FS from corruption at sudden power loss
#define LOGGER_MAX_FILE_SIZE	(32 * 1024 * 1024)

// at about 20Hz we write about 2Kb per second, looks like we flush once every ~2 seconds
#define F_SYNC_FREQUENCY 10

static bool sdLoggerReady = false;

#if EFI_PROD_CODE

#include <cstdio>
#include <cstring>
#include "mmc_card.h"
#include "ff.h"
#include "mmc_card_util.h"
#include "mass_storage_init.h"
#include "hellen_meta.h"

#include "rtc_helper.h"

#if EFI_STORAGE_SD == TRUE
#include "storage_sd.h"
#endif // EFI_STORAGE_SD

// TODO: do we need this additioal layer of buffering?
// FIL structure already have buffer of FF_MAX_SS size
// check if it is better to increase FF_MAX_SS and drop BufferedWriter?
struct SdLogBufferWriter final : public BufferedWriter<512> {
	bool failed = false;

	int start(FIL *fd) {
		if (m_fd) {
			efiPrintf("SD logger already started!");
			return -1;
		}

		totalLoggedBytes = 0;
		writeCounter = 0;

		m_fd = fd;

		return 0;
	}

	void stop() {
		m_fd = nullptr;

		flush();

		totalLoggedBytes = 0;
		writeCounter = 0;
	}

	size_t writen() {
		return totalLoggedBytes;
	}

	size_t writeInternal(const char* buffer, size_t count) override {
		if ((!m_fd) || (failed)) {
			return 0;
		}

		size_t bytesWritten;
		efiAssert(ObdCode::CUSTOM_STACK_6627, hasLotsOfRemainingStack(), "sdlow#3", 0);
		FRESULT err = f_write(m_fd, buffer, count, &bytesWritten);

		if (err) {
			printFatFsError("log file write", err);
			failed = true;
			return 0;
		} else if (bytesWritten != count) {
			printFatFsError("log file write partitial", err);
			failed = true;
			return 0;
		} else {
			writeCounter++;
			totalLoggedBytes += count;
			if (writeCounter >= F_SYNC_FREQUENCY) {
				/**
				 * Performance optimization: not f_sync after each line, f_sync is probably a heavy operation
				 * todo: one day someone should actually measure the relative cost of f_sync
				 */
				f_sync(m_fd);
				writeCounter = 0;
			}
		}

		return bytesWritten;
	}

private:
	FIL *m_fd = nullptr;

	size_t totalLoggedBytes = 0;
	size_t writeCounter = 0;
};

#else // not EFI_PROD_CODE (simulator)

#include <fstream>

class SdLogBufferWriter final : public BufferedWriter<512> {
public:
	bool failed = false;

	SdLogBufferWriter()
		: m_stream("rusefi_simulator_log.mlg", std::ios::binary | std::ios::trunc)
	{
		sdLoggerReady = true;
	}

	size_t writeInternal(const char* buffer, size_t count) override {
		m_stream.write(buffer, count);
		m_stream.flush();
		return count;
	}

private:
	std::ofstream m_stream;
};

#endif

static NO_CACHE SdLogBufferWriter logBuffer;

#if EFI_PROD_CODE

// This is dirty workaround to fix compilation without adding this function prototype
// to error_handling.h file that will also need to add "ff.h" include to same file and
// cause simulator fail to build.
extern void errorHandlerWriteReportFile(FIL *fd);
extern int errorHandlerCheckReportFiles();
extern void errorHandlerDeleteReports();

typedef enum {
	SD_STATUS_INIT = 0,
	SD_STATUS_MOUNTED,
	SD_STATUS_MOUNT_FAILED,
	SD_STATUS_OPEN_FAILED,
	SD_STATUS_SEEK_FAILED,
	SD_STATUS_NOT_INSERTED,
	SD_STATUS_CONNECTING,
	SD_STATUS_MSD,
	SD_STATUS_MMC_FAILED
} SD_STATUS;

// todo: shall we migrate to enum with enum2string for consistency? maybe not until we start reading sdStatus?
static const char *sdStatusNames[] =
{
	"INIT",
	"MOUNTED",
	"MOUNT_FAILED",
	"OPEN_FAILED",
	"SEEK_FAILED",
	"NOT_INSERTED",
	"CONNECTING",
	"MSD",
	"MMC_CONNECT_FAILED"
};

static const char *sdStatusName(SD_STATUS status)
{
	return sdStatusNames[status];
}

static SD_STATUS sdStatus = SD_STATUS_INIT;

static SD_MODE sdMode = SD_MODE_IDLE;
// by default we want SD card for logs
static SD_MODE sdTargetMode = SD_MODE_ECU;

static bool sdNeedRemoveReports = false;

#define RUSEFI_LOG_PREFIX "re_"
#define PREFIX_LEN 3
#define SHORT_TIME_LEN 13

#define FILE_LIST_MAX_COUNT 20

#if HAL_USE_MMC_SPI
/**
 * on't re-read SD card spi device after boot - it could change mid transaction (TS thread could preempt),
 * which will cause disaster (usually multiple-unlock of the same mutex in UNLOCK_SD_SPI)
 */
static spi_device_e mmcSpiDevice = SPI_NONE;

/**
 * MMC driver instance.
 */
static NO_CACHE uint8_t mmcbuf[MMC_BUFFER_SIZE];
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

static void sdLoggerSetReady(bool value) {
	sdLoggerReady = value;
}

static bool sdLoggerIsReady(void) {
	return sdLoggerReady;
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
void printFatFsError(const char *str, FRESULT f_error) {
	static int fatFsErrors = 0;

	if (fatFsErrors++ > 16) {
		// no reason to spam the console
		return;
	}

	efiPrintf("%s FATfs Error %d %s", str, f_error, f_error <= FR_INVALID_PARAMETER ? fatErrors[f_error] : "unknown");
}

// format, file access and MSD are used exclusively, we can union.
static union {
	// Warning: shared between all FS users, please release it after use
	FIL fd;
	// TODO: optimal cluster size?
	#define FATFS_CLUSTER_SIZE	1024
	BYTE formatBuff[FATFS_CLUSTER_SIZE];
	// MSD read/write buffer
	uint8_t blkbuf[4 * MMCSD_BLOCK_SIZE];
} resources NO_CACHE;

extern int logFileIndex;
static char logName[_MAX_FILLER + 20];

static void printMmcPinout() {
#if HAL_USE_MMC_SPI
	efiPrintf("MMC CS %s", hwPortname(engineConfiguration->sdCardCsPin));
	// todo: we need to figure out the right SPI pinout, not just SPI2
//	efiPrintf("MMC SCK %s:%d", portname(EFI_SPI2_SCK_PORT), EFI_SPI2_SCK_PIN);
//	efiPrintf("MMC MISO %s:%d", portname(EFI_SPI2_MISO_PORT), EFI_SPI2_MISO_PIN);
//	efiPrintf("MMC MOSI %s:%d", portname(EFI_SPI2_MOSI_PORT), EFI_SPI2_MOSI_PIN);
#else
  // not sure if we need to print SDIO pinout
#endif
}

static void sdStatistics() {
	printMmcPinout();
	efiPrintf("SD enabled=%s status=%s", boolToString(engineConfiguration->isSdCardEnabled),
			sdStatusName(sdStatus));
#if HAL_USE_MMC_SPI
	printSpiConfig("SD", mmcSpiDevice);
 #if defined(STM32F4XX) || defined(STM32F7XX)
	efiPrintf("HS clock %d Hz", spiGetBaseClock(mmccfg.spip) / (2 << ((mmc_hs_spicfg.cr1 & SPI_CR1_BR_Msk) >> SPI_CR1_BR_Pos)));
	efiPrintf("LS clock %d Hz", spiGetBaseClock(mmccfg.spip) / (2 << ((mmc_ls_spicfg.cr1 & SPI_CR1_BR_Msk) >> SPI_CR1_BR_Pos)));
 #else
  efiPrintf("not implemented");
 #endif
#else
 efiPrintf("SDIO mode");
#endif
	if (sdLoggerIsReady()) {
		efiPrintf("filename=%s size=%d", logName, logBuffer.writen());
	}
#if EFI_FILE_LOGGING
	efiPrintf("%d SD card fields", MLG::getSdCardFieldsCount());
#endif
}

static void sdSetMode(const char *mode) {
	if (strcmp(mode, "pc") == 0) {
		sdCardRequestMode(SD_MODE_PC);
	} else if (strcmp(mode, "ecu") == 0) {
		sdCardRequestMode(SD_MODE_ECU);
	} else {
		efiPrintf("Invalid mode %s allowed modes pc and ecu", mode);
	}
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
static int sdLoggerCreateFile(FIL *fd) {
	// turn off indicator
	sdLoggerSetReady(false);

	// clear the memory
	memset(fd, 0, sizeof(FIL));
	prepareLogFileName();

	efiPrintf("starting log file %s", logName);
	// Create new file. If file is exist - truncate and overwrite, we need header to be at zero offset.
	FRESULT err = f_open(fd, logName, FA_CREATE_ALWAYS | FA_WRITE);
	if (err == FR_EXIST) {
		err = FR_OK;
	}
#if EFI_TUNER_STUDIO
	// Show error to TS
	engine->outputChannels.sd_error = (uint8_t)err;
#endif
	if (err != FR_OK) {
		sdStatus = SD_STATUS_OPEN_FAILED;
		warning(ObdCode::CUSTOM_ERR_SD_MOUNT_FAILED, "SD: file open failed");
		printFatFsError("log file create", err);
		return -1;
	}

#ifdef LOGGER_MAX_FILE_SIZE
	//pre-allocate data ahead
	err = f_expand(fd, LOGGER_MAX_FILE_SIZE, /* Find and allocate */ 1);
	if (err != FR_OK) {
		printFatFsError("pre-allocate", err);
		// this is not critical
	}
#endif

	// SD logger is ok
	sdLoggerSetReady(true);

	return 0;
}

static void sdLoggerCloseFile(FIL *fd)
{
#ifdef LOGGER_MAX_FILE_SIZE
	// truncate file to actual size
	f_truncate(fd);
#endif

	// close file
	f_close(fd);

	// SD logger is inactive
	sdLoggerSetReady(false);
}

static void removeFile(const char *pathx) {
	if (sdMode != SD_MODE_ECU) {
		efiPrintf("SD card should be mounted to ECU");
		return;
	}

	f_unlink(pathx);
}

#if HAL_USE_USB_MSD

static chibios_rt::BinarySemaphore usbConnectedSemaphore(/* taken =*/ true);

void onUsbConnectedNotifyMmcI() {
	usbConnectedSemaphore.signalI();
}

#endif /* HAL_USE_USB_MSD */

#if HAL_USE_MMC_SPI

static bool isSdCardEnabled() {
	return ((engineConfiguration->isSdCardEnabled) &&
			(engineConfiguration->sdCardSpiDevice != SPI_NONE) &&
			(isBrainPinValid(engineConfiguration->sdCardCsPin)));
}

/*
 * Attempts to initialize the MMC card connected over SPI.
 * Returns a BaseBlockDevice* corresponding to the SD card if successful, otherwise nullptr.
 */
static BaseBlockDevice* initializeMmcBlockDevice() {
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
	mmcObjectInit(&MMCD1, mmcbuf);
	mmcStart(&MMCD1, &mmccfg);

	// Performs the initialization procedure on the inserted card.
	LOCK_SD_SPI();
	if (blkConnect(&MMCD1) != HAL_SUCCESS) {
		sdStatus = SD_STATUS_MMC_FAILED;
		UNLOCK_SD_SPI();
		return nullptr;
	}
	// We intentionally never unlock in case of success, we take exclusive access of that spi device for SD use

	return reinterpret_cast<BaseBlockDevice*>(&MMCD1);
}

static void deinitializeMmcBlockDevide() {
	blkDisconnect(&MMCD1);						// Brings the driver in a state safe for card removal.
	mmcStop(&MMCD1);							// Disables the MMC peripheral.
	UNLOCK_SD_SPI();
}
#endif /* HAL_USE_MMC_SPI */

// Some ECUs are wired for SDIO/SDMMC instead of SPI
#ifdef EFI_SDC_DEVICE

#ifndef RE_SDC_MODE
	#define RE_SDC_MODE SDC_MODE_4BIT
#endif // RE_SDC_MODE

static const SDCConfig sdcConfig = {
	.bus_width = RE_SDC_MODE,
	.slowdown = 0U
};

static bool isSdCardEnabled() {
	return (engineConfiguration->isSdCardEnabled);
}

/*
 * Attempts to initialize the MMC card connected over SDIO.
 * Returns a BaseBlockDevice* corresponding to the SD card if successful, otherwise nullptr.
 */
static BaseBlockDevice* initializeMmcBlockDevice() {
	sdcStart(&EFI_SDC_DEVICE, &sdcConfig);
	if (blkConnect(&EFI_SDC_DEVICE) != HAL_SUCCESS) {
		return nullptr;
	}

	return reinterpret_cast<BaseBlockDevice*>(&EFI_SDC_DEVICE);
}

static void deinitializeMmcBlockDevide() {
	blkDisconnect(&EFI_SDC_DEVICE);
	sdcStop(&EFI_SDC_DEVICE);
}

#endif /* EFI_SDC_DEVICE */

#if HAL_USE_USB_MSD
static bool useMsdMode() {
	if (engineConfiguration->alwaysWriteSdCard) {
		return false;
	}
	if (isIgnVoltage()) {
	  // if we have battery voltage let's give priority to logging not reading
	  // this gives us a chance to SD card log cranking
	  return false;
	}
	// Wait for the USB stack to wake up, or a 15 second timeout, whichever occurs first
	msg_t usbResult = usbConnectedSemaphore.wait(TIME_MS2I(15000));

	return usbResult == MSG_OK;
}
#endif // HAL_USE_USB_MSD

static BaseBlockDevice* cardBlockDevice = nullptr;

// Initialize SD card.
static bool initMmc() {
	// Don't try to mount SD card in case of fatal error - hardware may be in an unexpected state
	if (hasFirmwareError()) {
		return false;
	}

	cardBlockDevice = initializeMmcBlockDevice();

#if EFI_TUNER_STUDIO
	// If not null, card is present
	engine->outputChannels.sd_present = cardBlockDevice != nullptr;
#endif

	return (cardBlockDevice != nullptr);
}

static void deinitMmc() {
	if (cardBlockDevice) {
		deinitializeMmcBlockDevide();
	}
	cardBlockDevice = nullptr;
	engine->outputChannels.sd_present = false;
}

// Mount the SD card.
// Returns true if the filesystem was successfully mounted for writing.
static bool mountMmc() {
	bool ret = false;

	// if no card, don't try to mount FS
	if (cardBlockDevice != nullptr) {
		// clean shared buffer
		memset(&resources, 0x00, sizeof(resources));
		// We were able to connect the SD card, mount the filesystem
		memset(&MMC_FS, 0, sizeof(FATFS));
		ret = (f_mount(&MMC_FS, "", /* Mount immediately */ 1) == FR_OK);

		if (ret == false) {
			sdStatus = SD_STATUS_MOUNT_FAILED;
			efiPrintf("SD card mount failed!");
		}
	}

	if (ret) {
		sdStatus = SD_STATUS_MOUNTED;
		efiPrintf("SD card mounted!");
	}

#if EFI_STORAGE_SD == TRUE
	// notificate storage subsystem
	initStorageSD();
#endif // EFI_STORAGE_SD

#if EFI_TUNER_STUDIO
	engine->outputChannels.sd_logging_internal = ret;
#endif

	return ret;
}

/*
 * SD card un-mount.
 * @return true if we had SD card alive
 */
static void unmountMmc() {
	FRESULT ret;

#if EFI_STORAGE_SD == TRUE
	// notificate storage subsystem
	deinitStorageSD();
#endif // EFI_STORAGE_SD

	// FATFS: Unregister work area prior to discard it
	ret = f_unmount("");
	if (ret != FR_OK) {
		printFatFsError("Umount failed", ret);
	}

#if EFI_TUNER_STUDIO
	engine->outputChannels.sd_logging_internal = false;
#endif

	efiPrintf("SD card unmounted");
}

#else // not EFI_PROD_CODE (simulator)

bool initMmc() {
	// Stub so the loop thinks the SD is present
	return true;
}

bool mountMmc() {
	// Stub so the loop thinks the SD mounted OK
	return true;
}

#endif // EFI_PROD_CODE

#if EFI_PROD_CODE

// Log 'regular' ECU log to MLG file
static int mlgLogger();

// Log binary trigger log
static int sdTriggerLogger();

static bool sdLoggerInitDone = false;
static bool sdLoggerFailed = false;

static int sdLogger(FIL *fd)
{
	int ret = 0;

	if (!sdLoggerInitDone) {
		incLogFileName(fd);
		MLG::resetFileLogging();

		ret = sdLoggerCreateFile(fd);
		if (ret == 0) {
			ret = logBuffer.start(fd);
		}

		sdLoggerInitDone = true;

		if (ret < 0) {
			sdLoggerFailed = true;
			return ret;
		}
	}

	if (!sdLoggerFailed) {
		if (engineConfiguration->sdTriggerLog) {
			ret = sdTriggerLogger();
		} else {
			ret = mlgLogger();
		}
	}

	if (ret < 0) {
		sdLoggerFailed = true;
		return ret;
	}

	if (sdLoggerFailed) {
		// logger is dead until restart, do not waste CPU
		chThdSleepMilliseconds(100);
		return -1;
	}

#ifdef LOGGER_MAX_FILE_SIZE
	// check if we need to start next log file
	// in next write (assume same size as current) will cross LOGGER_MAX_FILE_SIZE boundary
	// TODO: use f_tell() instead ?
	if (logBuffer.writen() + ret > LOGGER_MAX_FILE_SIZE) {
		logBuffer.stop();
		sdLoggerCloseFile(fd);

		//need to start new file
		sdLoggerInitDone = false;
	}
#endif

	return ret;
}

static void sdLoggerStart(void)
{
	sdLoggerInitDone = false;
	sdLoggerFailed = false;

#if EFI_TOOTH_LOGGER
	// TODO: cache this config option untill sdLoggerStop()
	if (engineConfiguration->sdTriggerLog) {
		EnableToothLogger();
	}
#endif
}

static void sdLoggerStop(void)
{
	sdLoggerCloseFile(&resources.fd);
#if EFI_TOOTH_LOGGER
	// TODO: cache this config option untill sdLoggerStop()
	if (engineConfiguration->sdTriggerLog) {
		DisableToothLogger();
	}
#endif
}

static bool sdFormat()
{
#if EFI_TUNER_STUDIO
	engine->outputChannels.sd_formating = true;
	engine->outputChannels.sd_error = 0;
#endif
	//FRESULT ret = f_mkfs("", nullptr, resources.formatBuff, sizeof(resources.formatBuff));
	FRESULT ret = f_mkfs("", nullptr, resources.formatBuff, sizeof(resources.formatBuff));

	if (ret) {
		printFatFsError("format failed", ret);
		warning(ObdCode::CUSTOM_ERR_SD_MOUNT_FAILED, "SD: format failed");
		goto exit;
	}
	ret = f_setlabel(SD_CARD_LABEL);
	if (ret) {
		printFatFsError("setlabel failed", ret);
		// this is not critical
		ret = FR_OK;
	}

exit:
#if EFI_TUNER_STUDIO
	engine->outputChannels.sd_formating = false;
	engine->outputChannels.sd_error = (uint8_t) ret;
#endif

	return (ret ? false : true);
}

static int sdModeSwitchToIdle(SD_MODE from)
{
	switch (from) {
	case SD_MODE_IDLE:
		return 0;
	case SD_MODE_ECU:
		sdLoggerStop();
		unmountMmc();
		return 0;
	case SD_MODE_PC:
		deattachMsdSdCard();
		return 0;
	case SD_MODE_UNMOUNT:
		return 0;
	case SD_MODE_FORMAT:
		//not allowed to interrupt formating process
		return -1;
	}

	efiPrintf("Invalid SD card thread state: %d", static_cast<int>(from));
	return -1;
}

static int sdModeSwitcher()
{
	if (sdTargetMode == SD_MODE_IDLE) {
		return 0;
	}

	if (sdMode == sdTargetMode) {
		// already here
		sdTargetMode = SD_MODE_IDLE;
		return 0;
	}

	if (sdMode != SD_MODE_IDLE) {
		int ret = sdModeSwitchToIdle(sdMode);
		if (ret) {
			return ret;
		}
		sdMode = SD_MODE_IDLE;
	}

	if (sdMode != SD_MODE_IDLE) {
		return -1;
	}

	// Now SD card is in idle state, we can switch into target state
	switch (sdTargetMode) {
	case SD_MODE_IDLE:
		return 0;
	case SD_MODE_UNMOUNT:
		// everything is done in sdModeSwitchToIdle();
		sdMode = SD_MODE_UNMOUNT;
		sdTargetMode = SD_MODE_IDLE;
		return 0;
	case SD_MODE_ECU:
		if (mountMmc()) {
			sdLoggerStart();
			sdMode = SD_MODE_ECU;
		} else {
			// failed to mount SD card to ECU, go to idle
			sdMode = SD_MODE_IDLE;
		}
		sdTargetMode = SD_MODE_IDLE;
		return 0;
	case SD_MODE_PC:
		attachMsdSdCard(cardBlockDevice, resources.blkbuf, sizeof(resources.blkbuf));
		sdStatus = SD_STATUS_MSD;
		sdMode = SD_MODE_PC;
		sdTargetMode = SD_MODE_IDLE;
		return 0;
	case SD_MODE_FORMAT:
		if (sdFormat()) {
			// formated ok
		}
		sdMode = SD_MODE_IDLE;
		// TODO: return to mode that was used before format was requested!
		sdTargetMode = SD_MODE_IDLE;
		return 0;
	}

	// should not happen
	return -1;
}

static int sdModeExecuter()
{
	switch (sdMode) {
	case SD_MODE_IDLE:
	case SD_MODE_PC:
	case SD_MODE_UNMOUNT:
	case SD_MODE_FORMAT:
		// nothing to do in these state, just sleep
		chThdSleepMilliseconds(100);
		return 0;
	case SD_MODE_ECU:
		if (sdNeedRemoveReports) {
			errorHandlerDeleteReports();
			sdNeedRemoveReports = false;
		}
		// execute logger
		return sdLogger(&resources.fd);
	}

	return 0;
}

static int sdReportStorageInit()
{
	// write error report file if needed
	errorHandlerWriteReportFile(&resources.fd);

	// check for any exist reports
	errorHandlerCheckReportFiles();

	return 0;
}

PUBLIC_API_WEAK bool boardSdCardEnable() {
	// assume powered and ready
	return true;
}

PUBLIC_API_WEAK bool boardSdCardDisable() {
	return true;
}

static THD_WORKING_AREA(mmcThreadStack, 3 * UTILITY_THREAD_STACK_SIZE);		// MMC monitor thread
static THD_FUNCTION(MMCmonThread, arg) {
	(void)arg;

	chRegSetThreadName("SD Card Logger");

	while (!boardSdCardEnable()) {
		// wait until board enables peripheral
		chThdSleepMilliseconds(100);
	}

	sdStatus = SD_STATUS_CONNECTING;
	if (!initMmc()) {
		efiPrintf("Card is not preset/failed to init");
		sdStatus = SD_STATUS_NOT_INSERTED;
		// give up until next boot
		goto die;
	}

	// Try to mount SD card, drop critical report if needed and check for previously stored reports
	if (mountMmc()) {
		sdReportStorageInit();

		sdMode = SD_MODE_ECU;

#if EFI_STORAGE_SD == TRUE
		// Give some time for storage manager to load settings from SD
		chThdSleepMilliseconds(1000);
#endif
	}

#if HAL_USE_USB_MSD
	// Wait for the USB stack to wake up, or a 15 second timeout, whichever occurs first
	// If we have a device AND USB is connected, mount the card to USB, otherwise
	// mount the null device and try to mount the filesystem ourselves
	if (useMsdMode()) {
		sdTargetMode = SD_MODE_PC;
	}
#endif

	while (1) {
		sdModeSwitcher();
		sdModeExecuter();
	}

die:
	// bring SD interface to safe state
	deinitMmc();
	boardSdCardDisable();

	efiPrintf("SD logger has died!");

	// exiting thread will create zombie!
	while(1) {
		chThdSleepMilliseconds(100);
	}
}

static int mlgLogger() {
	// TODO: move this check somewhere out of here!
	// if the SPI device got un-picked somehow, cancel SD card
	// Don't do this check at all if using SDMMC interface instead of SPI
#if EFI_PROD_CODE && !defined(EFI_SDC_DEVICE)
	if (engineConfiguration->sdCardSpiDevice == SPI_NONE) {
		return 0;
	}
#endif

	systime_t before = chVTGetSystemTime();

	size_t writen = MLG::writeSdLogLine(logBuffer);

	// Something went wrong (already handled), so cancel further writes
	if (logBuffer.failed) {
		return -1;
	}

	auto freq = engineConfiguration->sdCardLogFrequency;
	if (freq > 250) {
		freq = 250;
	} else if (freq < 1) {
		freq = 1;
	}

	systime_t period = CH_CFG_ST_FREQUENCY / freq;
	chThdSleepUntilWindowed(before, before + period);

	return writen;
}

static int sdTriggerLogger() {
	size_t toWrite = 0;
#if EFI_TOOTH_LOGGER
	auto buffer = GetToothLoggerBufferBlocking();

	// can return nullptr
	if (buffer) {
		toWrite = buffer->nextIdx * sizeof(composite_logger_s);
		logBuffer.write(reinterpret_cast<const char*>(buffer->buffer), toWrite);
		if (logBuffer.failed) {
			return -1;
		}

		ReturnToothLoggerBuffer(buffer);
	}
#endif /* EFI_TOOTH_LOGGER */
	return toWrite;
}

#endif // EFI_PROD_CODE

void updateSdCardLiveFlags() {
#if EFI_PROD_CODE
	if (cardBlockDevice) {
		engine->outputChannels.sd_active_wr = (blkGetDriverState(cardBlockDevice) == BLK_WRITING);
		engine->outputChannels.sd_active_rd = (blkGetDriverState(cardBlockDevice) == BLK_READING);
	} else
#endif // EFI_PROD_CODE
	{
		engine->outputChannels.sd_active_wr = false;
		engine->outputChannels.sd_active_rd = false;
	}
}

// Pre-config load init
void initEarlyMmcCard() {
#if EFI_PROD_CODE
	logName[0] = 0;

	addConsoleAction("sdinfo", sdStatistics);
	addConsoleActionS("del", removeFile);
	// sdmode pc
	// sdmode ecu
	addConsoleActionS("sdmode", sdSetMode);
	addConsoleAction("delreports", sdCardRemoveReportFiles);
	//incLogFileName() use same shared FDLogFile, calling it while FDLogFile is used by log writer will cause damage
	//addConsoleAction("incfilename", incLogFileName);
#endif // EFI_PROD_CODE
}

void initMmcCard() {
#if EFI_PROD_CODE
	if (!isSdCardEnabled()) {
		// do not even bother starting the thread if SD card is not enabled & configured on start-up
		return;
	}
	chThdCreateStatic(mmcThreadStack, sizeof(mmcThreadStack), PRIO_MMC, (tfunc_t)(void*) MMCmonThread, NULL);
#endif // EFI_PROD_CODE
}

#if EFI_PROD_CODE

void sdCardRequestMode(SD_MODE mode)
{
	// Check if SD is not in transition state...
	if (sdTargetMode == SD_MODE_IDLE) {
		efiPrintf("sdCardRequestMode %d", (int)mode);
		sdTargetMode = mode;
	}
}

SD_MODE sdCardGetCurrentMode(void)
{
	return sdMode;
}

void sdCardRemoveReportFiles() {
	if (sdMode != SD_MODE_ECU) {
		efiPrintf("SD card should be mounted to ECU");
		return;
	}

	sdNeedRemoveReports = true;
}

#endif // EFI_PROD_CODE

#endif /* EFI_FILE_LOGGING */
