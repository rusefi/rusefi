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

// This is dirty workaround to fix compilation without adding this function prototype
// to error_handling.h file that will also need to add "ff.h" include to same file and
// cause simulator fail to build.
extern void errorHandlerWriteReportFile(FIL *fd, int index);

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
static SD_MODE sdTargerMode = SD_MODE_ECU;

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

// Warning: shared between all FS users, please release it after use
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
			sdStatusName(sdStatus));
	printSpiConfig("SD", mmcSpiDevice);
#if HAL_USE_MMC_SPI && (defined(STM32F4XX) || defined(STM32F7XX))
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
	// Create new file. If file is exist - truncate and overwrite, we need header to be at zero offset.
	FRESULT err = f_open(&FDLogFile, logName, FA_CREATE_ALWAYS | FA_WRITE);
	if (err != FR_OK && err != FR_EXIST) {
		sdStatus = SD_STATUS_OPEN_FAILED;
		warning(ObdCode::CUSTOM_ERR_SD_MOUNT_FAILED, "SD: mount failed");
		printError("log file create", err);	// else - show error
		return;
	}

	//pre-allocate data ahead
	err = f_expand(&FDLogFile, 32 * 1024 * 1024, 1);
	if (err != FR_OK) {
		printError("pre-allocate", err);
	}

	setSdCardReady(true);						// everything Ok
}

static void removeFile(const char *pathx) {
	if (!isSdCardAlive()) {
		efiPrintf("Error: No File system is mounted");
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
	mmcObjectInit(&MMCD1);
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

#ifndef RE_SDC_MODE
#define RE_SDC_MODE SDC_MODE_4BIT
#endif // RE_SDC_MODE

// Some ECUs are wired for SDIO/SDMMC instead of SPI
#ifdef EFI_SDC_DEVICE
static const SDCConfig sdcConfig = {
	.bus_width = RE_SDC_MODE
};

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
  if (errorHandlerIsStartFromError()) {
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
		// We were able to connect the SD card, mount the filesystem
		memset(&MMC_FS, 0, sizeof(FATFS));
		ret = (f_mount(&MMC_FS, "/", /* Mount immediately */ 1) == FR_OK);

		if (ret == false) {
			sdStatus = SD_STATUS_MOUNT_FAILED;
			efiPrintf("MMC/SD card mount failed!");
		}
	}

	if (ret) {
		sdStatus = SD_STATUS_MOUNTED;
		efiPrintf("MMC/SD mounted!");
	}

#if EFI_TUNER_STUDIO
	engine->outputChannels.sd_logging_internal = ret;
#endif

	return ret;
}

/*
 * MMC card un-mount.
 * @return true if we had SD card alive
 */
static void unmountMmc() {
	if (!isSdCardAlive()) {
		efiPrintf("Error: No File system is mounted. \"mountsd\" first");
		return;
	}

	f_mount(NULL, 0, 0);						// FATFS: Unregister work area prior to discard it
	setSdCardReady(false);						// status = false

#if EFI_TUNER_STUDIO
	engine->outputChannels.sd_logging_internal = false;
#endif

	efiPrintf("SD card unmounted");
}

struct SdLogBufferWriter final : public BufferedWriter<512> {
	bool failed = false;

	size_t writeInternal(const char* buffer, size_t count) override {
		size_t bytesWritten;

		FRESULT err = f_write(&FDLogFile, buffer, count, &bytesWritten);

		if (err) {
			printError("log file write", err);
			failed = true;
			return 0;
		} else if (bytesWritten != count) {
			printError("log file write partitial", err);
			failed = true;
			return 0;
		} else {
			writeCounter++;
			totalWritesCounter++;
			totalLoggedBytes += count;
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

bool initMmc() {
	// Stub so the loop thinks the MMC is present
	return true;
}

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

#if EFI_PROD_CODE

// Log 'regular' ECU log to MLG file
static int mlgLogger();

// Log binary trigger log
static int sdTriggerLogger();

static bool sdLoggerInitDone = false;
static bool sdLoggerFailed = false;

static int sdLogger()
{
	int ret = 0;

	if (!sdLoggerInitDone) {
		incLogFileName();
		// TODO: Do this independently of SD mode, somewhere in the start of this task!
		errorHandlerWriteReportFile(&FDLogFile, logFileIndex);
		createLogFile();
		sdLoggerInitDone = true;
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
	}

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
	// TODO: truncate file to actual size

	f_close(&FDLogFile);						// close file
	f_sync(&FDLogFile);							// sync ALL

	memset(&FDLogFile, 0, sizeof(FIL));			// clear FDLogFile

#if EFI_TOOTH_LOGGER
	// TODO: cache this config option untill sdLoggerStop()
	if (engineConfiguration->sdTriggerLog) {
		DisableToothLogger();
	}
#endif
}

// TODO: optimal cluster size?
// buffer is needed only for f_mkfs, use some shared buffer?
#define FATFS_CLUSTER_SIZE	1024
static BYTE formatBuff[FATFS_CLUSTER_SIZE];

static bool sdFormat()
{
	//FRESULT ret = f_mkfs("", FM_ANY, 1024, formatBuff, sizeof(formatBuff));
	FRESULT ret = f_mkfs("", FM_FAT | FM_FAT32, 1024, formatBuff, sizeof(formatBuff));

	if (ret) {
		printError("format failed", ret);
		warning(ObdCode::CUSTOM_ERR_SD_MOUNT_FAILED, "SD: format failed");
		return false;
	}
	ret = f_setlabel(SD_CARD_LABEL);
	if (ret) {
		printError("setlabel failed", ret);
		// this is not critical
	}

	return true;
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
	if (sdTargerMode == SD_MODE_IDLE) {
		return 0;
	}

	if (sdMode == sdTargerMode) {
		// already here
		sdTargerMode = SD_MODE_IDLE;
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
	switch (sdTargerMode) {
	case SD_MODE_IDLE:
		return 0;
	case SD_MODE_UNMOUNT:
		// everithing is done in sdModeSwitchToIdle();
		sdMode = SD_MODE_UNMOUNT;
		sdTargerMode = SD_MODE_IDLE;
		return 0;
	case SD_MODE_ECU:
		if (mountMmc()) {
			sdLoggerStart();
			sdMode = SD_MODE_ECU;
		} else {
			// failed to mount SD card to ECU, go to idle
			sdMode = SD_MODE_IDLE;
		}
		sdTargerMode = SD_MODE_IDLE;
		return 0;
	case SD_MODE_PC:
		attachMsdSdCard(cardBlockDevice);
		sdStatus = SD_STATUS_MSD;
		sdMode = SD_MODE_PC;
		sdTargerMode = SD_MODE_IDLE;
		return 0;
	case SD_MODE_FORMAT:
		if (sdFormat()) {
			// formated ok
		}
		sdMode = SD_MODE_IDLE;
		// TODO: return to mode that was used before format was requested!
		sdTargerMode = SD_MODE_IDLE;
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
		chThdSleepMilliseconds(TIME_MS2I(100));
		return 0;
	case SD_MODE_ECU:
		// execute logger
		return sdLogger();
	}

	return 0;
}

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

	sdStatus = SD_STATUS_CONNECTING;
	if (!initMmc()) {
		efiPrintf("Card is not preset/failed to init");
		sdStatus = SD_STATUS_NOT_INSERTED;
		// give up until next boot
		goto die;
	}

#if HAL_USE_USB_MSD
	// Wait for the USB stack to wake up, or a 15 second timeout, whichever occurs first
	// If we have a device AND USB is connected, mount the card to USB, otherwise
	// mount the null device and try to mount the filesystem ourselves
	if (useMsdMode()) {
		sdTargerMode = SD_MODE_PC;
	}
#endif

	while (1) {
		sdModeSwitcher();
		sdModeExecuter();
	}

die:
	// bring SD interface to safe state
	deinitMmc();

	efiPrintf("SD logger has died!");

	// exiting thread will create zombie!
	while(1) {
		chThdSleepMilliseconds(100);
	}
}

int mlgLogger() {
	// TODO: move this check somewhere out of here!
	// if the SPI device got un-picked somehow, cancel SD card
	// Don't do this check at all if using SDMMC interface instead of SPI
#if EFI_PROD_CODE && !defined(EFI_SDC_DEVICE)
	if (engineConfiguration->sdCardSpiDevice == SPI_NONE) {
		return 0;
	}
#endif

	systime_t before = chVTGetSystemTime();

	writeSdLogLine(logBuffer);

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

	return 0;
}

static int sdTriggerLogger() {
#if EFI_TOOTH_LOGGER
	auto buffer = GetToothLoggerBufferBlocking();

	// can return nullptr
	if (buffer) {
		size_t ret = logBuffer.write(reinterpret_cast<const char*>(buffer->buffer), buffer->nextIdx * sizeof(composite_logger_s));

		ReturnToothLoggerBuffer(buffer);

		// Nothing is written
		if (ret == 0) {
			return -1;
		}
	}
#endif /* EFI_TOOTH_LOGGER */
	return 0;
}

#endif // EFI_PROD_CODE

bool isSdCardAlive(void) {
	return fs_ready;
}

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
	//incLogFileName() use same shared FDLogFile, calling it while FDLogFile is used by log writer will cause damage
	//addConsoleAction("incfilename", incLogFileName);
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
#if EFI_PROD_CODE
	chThdCreateStatic(mmcThreadStack, sizeof(mmcThreadStack), PRIO_MMC, (tfunc_t)(void*) MMCmonThread, NULL);
#endif // EFI_PROD_CODE
}

#if EFI_PROD_CODE

void sdCardRequestMode(SD_MODE mode)
{
	if (sdTargerMode == SD_MODE_IDLE) {
		sdTargerMode = mode;
	}
}
#endif // EFI_PROD_CODE

#endif /* EFI_FILE_LOGGING */
