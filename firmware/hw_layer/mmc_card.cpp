/**
 * @file	mmc_card.cpp
 *
 * @date Dec 28, 2013
 * @author Kot_dnz
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 * SD card logging overview: docs/AI/sd_card_logging.md
 *
 * default pinouts in case of SPI2 connected to MMC: PB13 - SCK, PB14 - MISO, PB15 - MOSI, PD4 - CS, 3.3v
 * default pinouts in case of SPI3 connected to MMC: PB3  - SCK, PB4  - MISO, PB5  - MOSI, PD4 - CS, 3.3v
 *
 * Console commands registered by this module:
 *  - sdinfo
 *  - sdsuppresslogging
 *  - del <filename>
 *  - sd_test_write1mb
 *  - sdmode <pc|ecu|off|unmount|auto|format>
 *  - delreports
 *
 * SD/MMC status output channels updated by this module:
 *  - sd_present
 *  - sd_error
 *  - sd_formating
 *  - sdLoggingState
 *  - sd_logging_internal
 *  - sd_active_wr
 *  - sd_active_rd
 *
 *
 * todo: extract some logic into a controller file
 */

#include "pch.h"

#if EFI_FILE_LOGGING

#include "file_writer.h"
#include "status_loop.h"
#include "binary_mlg_logging.h"
#include "sd_log_trigger.h"

// Divide logs into 32Mb chunks.
// When defined, every log file is pre-allocated to this size with f_expand() on create
// and shrunk back to the actually-written size with f_truncate() on close - see
// sdLoggerCreateFile()/sdLoggerCloseFile(). While writes stay inside the pre-allocated
// area they never modify FAT/allocation structures, so a sudden power loss can lose
// buffered data but should not corrupt the filesystem itself.
// .mlg logs also roll over to a new file at this size (see sdLoggerMlg()); .teeth files
// have no such cap and just degrade to normal allocation past this point.
// See docs/AI/sd_card_logging.md for the full overview.
#define LOGGER_MAX_FILE_SIZE	(32 * 1024 * 1024)

// at about 20Hz we write about 2Kb per second, looks like we flush once every ~2 seconds
#define F_SYNC_FREQUENCY 10

#if EFI_PROD_CODE

#include <cstdio>
#include <cstring>
#include "mmc_card.h"
#include "ff.h"
#include "mmc_card_util.h"
#include "mass_storage_init.h"
#include "hellen_meta.h"

#include "rtc_helper.h"
#include "backup_ram.h"

#if EFI_STORAGE_SD == TRUE
#include "storage_sd.h"
#endif // EFI_STORAGE_SD

#endif

static NO_CACHE FileBufferedWriter logBuffer;

#if EFI_PROD_CODE

// This is dirty workaround to fix compilation without adding this function prototype
// to error_handling.h file that will also need to add "ff.h" include to same file and
// cause simulator fail to build.
extern void errorHandlerWriteReportFile(FIL *fd);
extern int errorHandlerCheckReportFiles();
extern void errorHandlerDeleteReports();

// see also SD_MODE
typedef enum : uint8_t {
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

static const char *sdStatusName(SD_STATUS status) {
	if ((uint8_t)status < efi::size(sdStatusNames)) {
		return sdStatusNames[status];
	}
	return "INVALID";
}

static const char *sdModeNames[] =
{
	"Idle",
	"ECU/logging",
	"PC/MSD",
	"Unmount",
	"Format"
};

static const char *sdModeName(SD_MODE mode) {
	if ((uint8_t)mode < efi::size(sdModeNames)) {
		return sdModeNames[mode];
	}
	return "INVALID";
}

static bool sdLoggerReady = false;
static SD_STATUS sdStatus = SD_STATUS_INIT;

static SD_MODE sdMode = SD_MODE_IDLE;
// by default we want SD card for logs
static SD_MODE sdTargetMode = SD_MODE_ECU;
static bool sdTargetModeRequested = false;

static bool sdNeedRemoveReports = false;

#define RUSEFI_LOG_PREFIX "re_"
#define PREFIX_LEN 3
#define SHORT_TIME_LEN 13

#define FILE_LIST_MAX_COUNT 20

#if HAL_USE_MMC_SPI
/**
 * Don't re-read SD card spi device after boot - it could change mid transaction (TS thread could preempt),
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

// When MMC_USE_MUTUAL_EXCLUSION is enabled the ChibiOS MMC driver serializes SPI bus
// access itself, so these become no-ops; otherwise we must lock the bus manually since
// other devices (and threads) may share the same SPI peripheral.
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

/**
 * SD card state persisted in battery-backed RAM (a single 32-bit word) so that after
 * a reset we can tell whether the previous power-off happened while the filesystem
 * was still mounted - i.e. an "unsafe unmount" that risks FAT corruption.
 */
union SdBackupState {
	struct {
		SD_MODE mode;
		SD_STATUS status;
		// how many times power was lost while the card was mounted, saturates at 255
		uint8_t unsafeUnmountCnt;
		// magic marker 0x5A: anything else means backup RAM content is uninitialized/lost
		uint8_t valid;
	} __attribute__((packed));
	// raw access for backupRamLoad()/backupRamSave()
	uint32_t x;
};

static_assert(sizeof(SdBackupState) == sizeof(uint32_t));

static void sdCardUpdateBackupState() {
	SdBackupState state;
	state.x = backupRamLoad(backup_ram_e::MccStatus);
	state.mode = sdMode;
	state.status = sdStatus;
	state.valid = 0x5A;
	backupRamSave(backup_ram_e::MccStatus, state.x);
}

static void sdCardShowBackupState() {
	SdBackupState state;
	state.x = backupRamLoad(backup_ram_e::MccStatus);

	efiPrintf("SD state at last power off %s / %s", sdModeName(state.mode), sdStatusName(state.status));
	efiPrintf("total unsafe power offs %d", state.unsafeUnmountCnt);
}

/**
 * Inspect the backup RAM state left over from the previous boot and re-initialize it.
 * If the card was left in a mounted mode (anything but IDLE/UNMOUNT) the previous
 * shutdown was unsafe, so bump the unsafe unmount counter.
 *
 * @return true if backup RAM contained a valid state from a previous boot
 */
static bool sdCardInitBackupState() {
	SdBackupState state;
	state.x = backupRamLoad(backup_ram_e::MccStatus);

	bool valid = state.valid == 0x5A;
	if (valid) {
		if ((state.mode != SD_MODE_IDLE) && (state.mode != SD_MODE_UNMOUNT)) {
			if (state.unsafeUnmountCnt < 255) {
				state.unsafeUnmountCnt++;
			}
		}
	} else {
		state.mode = SD_MODE_IDLE;
		state.status = SD_STATUS_INIT;
		state.unsafeUnmountCnt = 0;
		state.valid = 0x5A;
	}

	backupRamSave(backup_ram_e::MccStatus, state.x);

	return valid;
}

static void sdCardSetCurrentMode(SD_MODE mode) {
	sdMode = mode;

	sdCardUpdateBackupState();
}

static void sdSetCurrentStatus(SD_STATUS status) {
	sdStatus = status;

	sdCardUpdateBackupState();
}

static void sdLoggerSetReady(bool value) {
#if EFI_TUNER_STUDIO
	engine->outputChannels.sd_logging_internal = value;
#endif
	sdLoggerReady = value;
}

static bool sdLoggerIsReady() {
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

const char *getFatFsErrorDescription(FRESULT f_error) {
	if (f_error <= FR_INVALID_PARAMETER)
		return fatErrors[f_error];
	return "unknown";
}

// print FAT error function
void printFatFsError(const char *str, FRESULT f_error) {
	static int fatFsErrors = 0;

	if (fatFsErrors++ > 16) {
		// no reason to spam the console
		return;
	}

	efiPrintf("%s FATfs Error %d %s", str, f_error, getFatFsErrorDescription(f_error));
}

// format, file access and MSD are used exclusively, we can union.
// All members are only touched from the MMC thread (see MMCmonThread), which is what
// makes the union safe - see sdTestWrite1Mb() for what to do from other threads.
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
	efiPrintf("SD enabled=%s status=%s target mode=%s%s", boolToString(engineConfiguration->isSdCardEnabled),
			sdStatusName(sdStatus), sdModeName(sdTargetMode), sdTargetModeRequested ? " user selected" : " auto");
#if HAL_USE_MMC_SPI
	printSpiConfig("SD", mmcSpiDevice);
#if defined(STM32F4XX) || defined(STM32F7XX)
	efiPrintf("HS clock %d Hz", spiGetBaseClock(mmccfg.spip) / (2 << ((mmc_hs_spicfg.cr1 & SPI_CR1_BR_Msk) >> SPI_CR1_BR_Pos)));
	efiPrintf("LS clock %d Hz", spiGetBaseClock(mmccfg.spip) / (2 << ((mmc_ls_spicfg.cr1 & SPI_CR1_BR_Msk) >> SPI_CR1_BR_Pos)));
#else
	efiPrintf("HS clock %d Hz", spiGetBaseClock(mmccfg.spip) / (2 << ((mmc_hs_spicfg.cfg1 & SPI_CFG1_MBR_Msk) >> SPI_CFG1_MBR_Pos)));
	efiPrintf("LS clock %d Hz", spiGetBaseClock(mmccfg.spip) / (2 << ((mmc_ls_spicfg.cfg1 & SPI_CFG1_MBR_Msk) >> SPI_CFG1_MBR_Pos)));
#endif
#else
	efiPrintf("SDIO mode");
#endif
	if (sdLoggerIsReady()) {
		efiPrintf("filename=%s size=%d", logName, logBuffer.size());
	}
#if EFI_FILE_LOGGING
	efiPrintf("%d SD card fields", MLG::getSdCardFieldsCount());
#endif
#if HAL_USE_USB_MSD
	printMsdDiagnostics();
#endif
}

static void sdSetMode(const char *mode) {
	if (strcmp(mode, "pc") == 0) {
		sdCardRequestMode(SD_MODE_PC);
	} else if (strcmp(mode, "ecu") == 0) {
		sdCardRequestMode(SD_MODE_ECU);
	} else if ((strcmp(mode, "off") == 0) || (strcmp(mode, "unmount") == 0)) {
		sdCardRequestMode(SD_MODE_UNMOUNT);
	} else if (strcmp(mode, "auto") == 0) {
		sdCardRequestMode(SD_MODE_IDLE);
	} else if (strcmp(mode, "format") == 0) {
		sdCardRequestMode(SD_MODE_FORMAT);
	} else {
		efiPrintf("Invalid mode %s allowed modes pc, ecu, off, unmount, auto, format", mode);
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
		sdSetCurrentStatus(SD_STATUS_OPEN_FAILED);
		warning(ObdCode::CUSTOM_ERR_SD_MOUNT_FAILED, "SD: file open failed");
		printFatFsError("log file create", err);
		return -1;
	}

#ifdef LOGGER_MAX_FILE_SIZE
	// Pre-allocate the whole file as one contiguous cluster chain (opt=1: allocate now,
	// not just find; requires FF_USE_EXPAND in ffconf.h). All FAT updates happen here,
	// up-front: subsequent f_write() calls inside this area only touch data sectors,
	// which protects the filesystem from corruption at sudden power loss.
	err = f_expand(fd, LOGGER_MAX_FILE_SIZE, /* Find and allocate */ 1);
	if (err != FR_OK) {
		printFatFsError("pre-allocate", err);
		// Not critical: happens on a fragmented card with no 32Mb contiguous run.
		// FatFS falls back to growing the file cluster-by-cluster on write - logging
		// still works, just without the power-loss protection above.
	}
#endif

	// SD logger is running
	sdLoggerSetReady(true);

	return 0;
}

static void sdLoggerCloseFile(FIL *fd)
{
#ifdef LOGGER_MAX_FILE_SIZE
	// Shrink the file from the 32Mb f_expand() pre-allocation back to the size actually
	// written, returning the unused tail to free space. A file that never got this
	// treatment (power loss) stays at full 32Mb with trailing garbage - log readers
	// stop at the last valid record.
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

#define SD_TEST_FILE_NAME "test1mb.bin"
#define SD_TEST_FILE_SIZE (1024 * 1024)

// Writes a 1Mb test file filled with an incrementing 32 bit counter, reports throughput
static void sdTestWrite1Mb() {
	if (sdMode != SD_MODE_ECU) {
		efiPrintf("SD card should be mounted to ECU");
		return;
	}

	// own descriptor and buffer: shared 'resources' union belongs to the logger thread
	static NO_CACHE FIL testFd;
	static NO_CACHE uint32_t testBuffer[128];

	memset(&testFd, 0, sizeof(testFd));
	FRESULT err = f_open(&testFd, SD_TEST_FILE_NAME, FA_CREATE_ALWAYS | FA_WRITE);
	if (err != FR_OK) {
		printFatFsError("test file create", err);
		return;
	}

	efiPrintf("SD: writing %d bytes to %s...", SD_TEST_FILE_SIZE, SD_TEST_FILE_NAME);

	uint32_t counter = 0;
	size_t written = 0;
	systime_t before = chVTGetSystemTime();

	while (written < SD_TEST_FILE_SIZE) {
		for (size_t i = 0; i < efi::size(testBuffer); i++) {
			testBuffer[i] = counter++;
		}

		UINT bytesWritten = 0;
		err = f_write(&testFd, testBuffer, sizeof(testBuffer), &bytesWritten);
		if ((err != FR_OK) || (bytesWritten != sizeof(testBuffer))) {
			printFatFsError("test file write", err);
			break;
		}
		written += bytesWritten;
	}

	f_close(&testFd);

	sysinterval_t elapsed = chVTTimeElapsedSinceX(before);
	uint32_t elapsedMs = TIME_I2MS(elapsed);
	if (elapsedMs == 0) {
		elapsedMs = 1;
	}
	efiPrintf("SD: wrote %d bytes in %lu ms, %lu Kb/sec", written, elapsedMs, (uint32_t)(written / elapsedMs * 1000 / 1024));
}

#if HAL_USE_USB_MSD

static volatile bool usbConnected = false;

void onUsbConnectedNotifyMmcI(bool connected) {
	usbConnected = connected;
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
	// TODO: change to 26 MHz for some H7 devices where SPI PCLK is 52 MHz
	// so we can get 26MHz instead of current 13MHz. This is 4% overclock.
	spiCalcClockDiv(mmccfg.spip, &mmc_hs_spicfg, 25 * 1000 * 1000);
	// and 250 KHz during initialization
	spiCalcClockDiv(mmccfg.spip, &mmc_ls_spicfg, 250 * 1000);

	// We think we have everything for the card, let's try to mount it!
	mmcObjectInit(&MMCD1, mmcbuf);
	mmcStart(&MMCD1, &mmccfg);

	// Performs the initialization procedure on the inserted card.
	LOCK_SD_SPI();
	if (blkConnect(&MMCD1) != HAL_SUCCESS) {
		sdSetCurrentStatus(SD_STATUS_MMC_FAILED);
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
	FRESULT ret = FR_NOT_READY;

	// if no card, don't try to mount FS
	if (cardBlockDevice != nullptr) {
		// clean shared buffer
		memset(&resources, 0x00, sizeof(resources));
		// We were able to connect the SD card, mount the filesystem
		memset(&MMC_FS, 0, sizeof(FATFS));
		ret = f_mount(&MMC_FS, "", /* Mount immediately */ 1);

		if (ret != FR_OK) {
			printFatFsError("Mount failed", ret);
		}
	}

#if EFI_STORAGE_SD == TRUE
	if (ret == FR_OK) {
		// notificate storage subsystem
		initStorageSD();
	}
#endif // EFI_STORAGE_SD

#if EFI_TUNER_STUDIO
	engine->outputChannels.sd_error = (uint8_t) ret;
#endif

	return (ret == FR_OK);
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
	engine->outputChannels.sd_error = (uint8_t) ret;
#endif
	sdLoggerSetReady(false);

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

static bool sdLoggerInitDone = false;
static bool sdLoggerFailed = false;

static bool sdLoggedSuppressed = false;

#if EFI_TOOTH_LOGGER
/**
 * One iteration of trigger tooth logging: lazily creates a .teeth file once tooth
 * data shows up, appends whatever the tooth logger has buffered, and closes the file
 * when the data stream ends so the next burst starts a fresh file.
 *
 * @return positive number of bytes written, 0 if there was nothing to do, negative on error
 */
static int sdLoggerTooth(FIL *fd) {
	int ret = 0;

	// file is not created yet?
	if (!sdLoggerInitDone) {
		// do we have some data ready?
		if (!ToothLoggerHasData()) {
			// nothing to log
			// wait another 100mS for tooth data
			chThdSleepMilliseconds(100);
			return 0;
		}

		// Ok, lets create file
		incLogFileName(fd);

		ret = sdLoggerCreateFile(fd);
		if (ret != 0) {
			sdLoggerFailed = true;
			return -1;
		}

		ret = logBuffer.init(fd);
		if (ret != 0) {
			// TODO: close file
			sdLoggerFailed = true;
			return -2;
		}

		sdLoggerInitDone = true;
	}

	// we have file... do we have some data to write?
	ret = ToothLoggerWriter(logBuffer);
	if (ret > 0) {
		// we have data, we have successfully wrote it
		return ret;
	}

	if (ret < 0) {
		// some error
		sdLoggerFailed = true;
	}

	// some error or no more data...
	// in both cases: close file
	logBuffer.stop();
	sdLoggerCloseFile(fd);

	// need to start new file
	sdLoggerInitDone = false;

	// error or size of wroten data
	return ret;
}
#endif

/**
 * One iteration of MLG logging: lazily creates the log file on first call, writes one
 * log line (rate-limited inside mlgLogger()), and rolls over to a new file when the
 * current one reaches LOGGER_MAX_FILE_SIZE.
 *
 * @return positive number of bytes written, negative on error
 */
static int sdLoggerMlg(FIL *fd) {
	int ret = 0;

	if (!sdLoggerInitDone) {
		incLogFileName(fd);
		MLG::resetFileLogging();

		ret = sdLoggerCreateFile(fd);
		if (ret != 0) {
			sdLoggerFailed = true;
			return -1;
		}
		ret = logBuffer.init(fd);
		if (ret != 0) {
			// TODO: close file
			sdLoggerFailed = true;
			return -2;
		}

		sdLoggerInitDone = true;
	}

	ret = mlgLogger();

	if (ret < 0) {
		logBuffer.stop();
		sdLoggerCloseFile(fd);
		sdLoggerFailed = true;
		return ret;
	}

#ifdef LOGGER_MAX_FILE_SIZE
	// check if we need to start next log file
	// in next write (assume same size as current) will cross LOGGER_MAX_FILE_SIZE boundary
	// TODO: use f_tell() instead ?
	if (logBuffer.size() + ret > LOGGER_MAX_FILE_SIZE) {
		logBuffer.stop();
		sdLoggerCloseFile(fd);

		//need to start new file
		sdLoggerInitDone = false;
	}
#endif

	return ret;
}

static void sdLoggerStart()
{
	sdLoggerInitDone = false;
	sdLoggerFailed = false;

#if EFI_TOOTH_LOGGER
	// TODO: cache this config option until sdLoggerStop()
	if (engineConfiguration->sdTriggerLog) {
		EnableToothLogger();
	}
#endif
}

static void sdLoggerStop()
{
	logBuffer.stop();
	sdLoggerCloseFile(&resources.fd);
#if EFI_TOOTH_LOGGER
	// TODO: pick this config option from cached
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

	// FS should be mounted to change label
	memset(&MMC_FS, 0, sizeof(FATFS));
	ret = f_mount(&MMC_FS, "", /* Mount immediately */ 1);
	if (ret) {
		printFatFsError("mount after format failed", ret);
		warning(ObdCode::CUSTOM_ERR_SD_MOUNT_FAILED, "SD: mount failed");
		goto exit;
	}

	ret = f_setlabel(SD_CARD_LABEL);
	if (ret) {
		printFatFsError("setlabel failed", ret);
		// this is not critical
		ret = FR_OK;
	}

	ret = f_unmount("");
	if (ret != FR_OK) {
		printFatFsError("Umount failed", ret);
	}

exit:
#if EFI_TUNER_STUDIO
	engine->outputChannels.sd_formating = false;
	engine->outputChannels.sd_error = (uint8_t) ret;
#endif

	return (ret ? false : true);
}

/**
 * Tear down whatever the current mode was using (logger + FS mount for ECU mode,
 * USB mass storage attachment for PC mode) so the card is free for the next mode.
 * All mode transitions go through IDLE - see sdModeSwitcher().
 *
 * @return 0 on success
 */
static int sdModeSwitchToIdle(SD_MODE from)
{
	switch (from) {
	case SD_MODE_IDLE:
		return 0;
	case SD_MODE_ECU:
		sdLoggerStop();
		unmountMmc();
		sdSetCurrentStatus(SD_STATUS_INIT);
		return 0;
	case SD_MODE_PC:
		deattachMsdSdCard();
		return 0;
	case SD_MODE_UNMOUNT:
		return 0;
	case SD_MODE_FORMAT:
		// format is finished in sdModeSwitcher() and we are free to switch to idle
		return 0;
	}

	efiPrintf("Invalid SD card thread state: %d", static_cast<int>(from));
	return -1;
}

/**
 * Decide which mode the SD card should be in right now.
 * A user request (sdmode console command / TS) always wins; otherwise the mode is
 * derived from the power state: unmount when both USB and ignition are gone (we are
 * about to lose power), expose the card to the PC when USB is connected, log otherwise.
 */
static SD_MODE sdModeSelector() {
	if (sdTargetModeRequested) {
		// user force selected mode
		// preserve it until power off
		return sdTargetMode;
	}

	if (!usbConnected && !isIgnVoltage()) {
		// No USB connection, no ignition voltage
		// Are we about to switch off?
		return SD_MODE_UNMOUNT;
	}

	if (engineConfiguration->alwaysWriteSdCard) {
		return SD_MODE_ECU;
	}

#if HAL_USE_USB_MSD
	if (usbConnected) {
		return SD_MODE_PC;
	}
#endif

	return SD_MODE_ECU;
}

/**
 * Move the SD card from 'mode' towards 'target'. Any transition first passes through
 * IDLE (releasing the resources of the old mode) and then enters the target mode.
 *
 * @return the mode actually reached - equal to 'target' on success, otherwise the
 * mode we ended up stuck in (e.g. IDLE if the mount failed)
 */
static SD_MODE sdModeSwitcher(SD_MODE mode, SD_MODE target) {
	// No request to switch to any mode
	if (target == SD_MODE_IDLE) {
		return mode;
	}

	if (mode == target) {
		// already here
		return target;
	}

	if (mode != SD_MODE_IDLE) {
		int ret = sdModeSwitchToIdle(mode);
		if (ret) {
			// failed to switch
			efiPrintf("SD: failed to switch to Idle before %s: %d", sdModeName(target), ret);
			return mode;
		}
		mode = SD_MODE_IDLE;
	}

	// Now SD card is in idle state, we can switch into target state
	switch (target) {
	case SD_MODE_IDLE:
		[[fallthrough]];
	case SD_MODE_UNMOUNT:
		// everything is done in sdModeSwitchToIdle();
		return target;
	case SD_MODE_ECU:
		if (mountMmc()) {
			sdSetCurrentStatus(SD_STATUS_MOUNTED);
			sdLoggerStart();
			return SD_MODE_ECU;
		} else {
			sdSetCurrentStatus(SD_STATUS_MOUNT_FAILED);
			// failed to mount SD card to ECU, go to idle
			return SD_MODE_IDLE;
		}
	case SD_MODE_PC:
		attachMsdSdCard(cardBlockDevice, resources.blkbuf, sizeof(resources.blkbuf));
		sdSetCurrentStatus(SD_STATUS_MSD);
		return SD_MODE_PC;
	case SD_MODE_FORMAT:
		if (sdFormat()) {
			// formated ok
		}
		sdSetCurrentStatus(SD_STATUS_INIT);
		return SD_MODE_FORMAT;
	}

	// should not happen
	return mode;
}

/**
 * Do one iteration of work for the current mode. Only ECU (logging) mode has ongoing
 * work: checking the log trigger and writing log data. All other modes just sleep.
 *
 * @return positive number of bytes written, 0 if idle (caller sleeps), negative on error
 */
static int sdModeExecuter(SD_MODE mode)
{
	switch (mode) {
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

		if ((sdLoggedSuppressed) || (sdLoggerFailed)) {
			// logger is dead or paused, do not waste CPU
			engine->outputChannels.sdLoggingState = (uint8_t)(sdLoggerFailed ? SD_LOG_FAILED : SD_LOG_SUPPRESSED);
			engine->outputChannels.sd_logging_internal = false;
			chThdSleepMilliseconds(100);
			return 0;
		}

		// Conditional logging: only write while the trigger conditions are met (phase 1)
		{
			auto trigger = engine->module<SdLogTrigger>();
			engine->outputChannels.sdLoggingState = (uint8_t)trigger->getState();
			if (!trigger->shouldLog()) {
				engine->outputChannels.sd_logging_internal = false;
				// close the current file so each logging session is its own file
				if (sdLoggerInitDone) {
					sdLoggerStop();
					sdLoggerInitDone = false;
				}
				chThdSleepMilliseconds(100);
				return 0;
			}
			engine->outputChannels.sd_logging_internal = true;
		}

		// execute one of logger
#if EFI_TOOTH_LOGGER
		if (engineConfiguration->sdTriggerLog) {
			return sdLoggerTooth(&resources.fd);
		} else
#endif
		{
			return sdLoggerMlg(&resources.fd);
		}
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

/**
 * The SD card thread: owns the card and the 'resources' union for its whole lifetime.
 * After one-time init (backup RAM check, card detection, crash report handling) it
 * runs the mode state machine forever: sdModeSelector() picks the desired mode,
 * sdModeSwitcher() transitions to it, sdModeExecuter() does that mode's work.
 * If the card fails to initialize the thread parks itself until the next boot.
 */
static THD_FUNCTION(MMCmonThread, arg) {
	(void)arg;

	chRegSetThreadName("SD Card Logger");

	while (!boardSdCardEnable()) {
		// wait until board enables peripheral
		chThdSleepMilliseconds(100);
	}

	if (sdCardInitBackupState()) {
		sdCardShowBackupState();
	} else {
		efiPrintf("SD backup RAM state is not valid");
	}

	sdSetCurrentStatus(SD_STATUS_CONNECTING);
	if (!initMmc()) {
		efiPrintf("Card is not preset/failed to init");
		sdSetCurrentStatus(SD_STATUS_NOT_INSERTED);
		// give up until next boot
		goto die;
	}

	// Try to mount SD card, drop critical report if needed and check for previously stored reports
	if (mountMmc()) {
		sdReportStorageInit();

#if EFI_STORAGE_SD == TRUE
		// Give some time for storage manager to load settings from SD
		chThdSleepMilliseconds(1000);
#endif

		unmountMmc();
	}

	while (1) {
		// get SD card mode
		SD_MODE target = sdModeSelector();

		// target mode is valid and not reached yet
		if ((target != SD_MODE_IDLE) && (sdMode != target)) {
			// Try to swith to this adjusted mode
			SD_MODE current = sdModeSwitcher(sdMode, target);

			// Target mode is valid and we have failed to switch to it
			if (current != target) {
				efiPrintf("SD: failed to switch from %s to %s", sdModeName(sdMode), sdModeName(target));

				sdTargetMode = SD_MODE_IDLE;
				sdTargetModeRequested = false;

				chThdSleepMilliseconds(1000);
				sdCardSetCurrentMode(SD_MODE_IDLE);
			} else {
				efiPrintf("SD: switched from %s to %s", sdModeName(sdMode), sdModeName(target));
				sdCardSetCurrentMode(target);
			}
		}

		if (sdModeExecuter(sdMode) == 0) {
			chThdSleepMilliseconds(100);
		}
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

/**
 * Write one MLG log line and pace the logger: sleeps so that lines are written at
 * engineConfiguration->sdCardLogFrequency Hz (clamped to 1..250), and syncs the file
 * to media every F_SYNC_FREQUENCY lines rather than after every write.
 *
 * @return number of bytes written, or negative if the buffer writer has failed
 */
static int mlgLogger() {
	static size_t writeCounter = 0;
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

	writeCounter++;
	if (writeCounter >= F_SYNC_FREQUENCY) {
		/**
		 * Performance optimization: not f_sync after each line, sync is probably a heavy operation
		 * todo: one day someone should actually measure the relative cost of sync
		 */
		logBuffer.sync();
		writeCounter = 0;
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
	addConsoleAction("sdsuppresslogging",  [](){
	  sdLoggedSuppressed = true;
    efiPrintf("Suppressed!");
  });
	addConsoleActionS("del", removeFile);
	addConsoleAction("sd_test_write1mb", sdTestWrite1Mb);
	// sdmode pc
	// sdmode ecu
	// sdmode off
	// sdmode auto
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

/**
 * Ask the SD thread to switch to the given mode (from console 'sdmode' command or TS).
 * The switch is asynchronous - it happens on the next MMCmonThread loop iteration.
 * A user-requested mode sticks until power off; requesting SD_MODE_IDLE clears the
 * user override and returns mode selection to automatic (see sdModeSelector()).
 *
 * @return 0 if the request was accepted
 */
int sdCardRequestMode(SD_MODE mode)
{
	if (!isSdCardEnabled()) {
		efiPrintf("SD card is not enabled in config!");
		return -1;
	}

	if (cardBlockDevice == nullptr) {
		efiPrintf("SD card is not inserted/failed to init");
		return -2;
	}

	// Check if SD is not in transition state...
	if (sdMode != mode) {
		efiPrintf("sdCardRequestMode %s", sdModeName(mode));
		sdTargetModeRequested = true;
		sdTargetMode = mode;
	}
	if (mode == SD_MODE_IDLE) {
		sdTargetModeRequested = false;
	}

	return 0;
}

SD_MODE sdCardGetCurrentMode()
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
