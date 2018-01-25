/**
 * @file	mmc_card.cpp
 *
 * @date Dec 28, 2013
 * @author Kot_dnz
 * @author Andrey Belomutskiy, (c) 2012-2018
 *
 * default pinouts in case of SPI2 connected to MMC: PB13 - SCK, PB14 - MISO, PB15 - MOSI, PD4 - CS, 3.3v
 * default pinouts in case of SPI3 connected to MMC: PB3  - SCK, PB4  - MISO, PB5  - MOSI, PD4 - CS, 3.3v
 *
 *
 * todo: extract some logic into a controller file
 */

#include "main.h"

#if EFI_FILE_LOGGING || defined(__DOXYGEN__)

#include <stdio.h>
#include <string.h>
#include "mmc_card.h"
#include "pin_repository.h"
#include "ff.h"
#include "hardware.h"
#include "engine_configuration.h"
#include "status_loop.h"
#include "hal_usb_msd.h"
#include "usb_msd_cfg.h"

#include "rtc_helper.h"

#define SD_STATE_INIT "init"
#define SD_STATE_MOUNTED "MOUNTED"
#define SD_STATE_MOUNT_FAILED "MOUNT_FAILED"
#define SD_STATE_OPEN_FAILED "OPEN_FAILED"
#define SD_STATE_SEEK_FAILED "SEEK_FAILED"
#define SD_STATE_NOT_INSERTED "NOT_INSERTED"
#define SD_STATE_CONNECTING "CONNECTING"
#define SD_STATE_NOT_CONNECTED "NOT_CONNECTED"

static const char *sdStatus = SD_STATE_INIT;
static bool fs_ready = false;

EXTERN_ENGINE;

#define LOG_INDEX_FILENAME "index.txt"

#define RUSEFI_LOG_PREFIX "rus"
#define PREFIX_LEN 3
#define SHORT_TIME_LEN 11

#define LS_RESPONSE "ls_result"
#define FILE_LIST_MAX_COUNT 20

extern board_configuration_s *boardConfiguration;

#if HAL_USE_USB_MSD
#if STM32_USB_USE_OTG2
  USBDriver *usb_driver = &USBD2;
#else
  USBDriver *usb_driver = &USBD1;
#endif
extern const USBConfig msdusbcfg;
#endif /* HAL_USE_USB_MSD */


#define PUSHPULLDELAY 500

static THD_WORKING_AREA(mmcThreadStack,3 * UTILITY_THREAD_STACK_SIZE);		// MMC monitor thread

/**
 * MMC driver instance.
 */
MMCDriver MMCD1;

static SPIConfig hs_spicfg = { NULL, NULL, 0,
SPI_BaudRatePrescaler_8 };
static SPIConfig ls_spicfg = { NULL, NULL, 0,
SPI_BaudRatePrescaler_256 };

/* MMC/SD over SPI driver configuration.*/
// don't forget check if STM32_SPI_USE_SPI2 defined and spi has init with correct GPIO in hardware.cpp
static MMCConfig mmccfg = { NULL, &ls_spicfg, &hs_spicfg };

#define FILE_LOG_DELAY 200

/**
 * fatfs MMC/SPI
 */
static FATFS MMC_FS;

static LoggingWithStorage logger("mmcCard");

static int fatFsErrors = 0;

// print FAT error function
static void printError(const char *str, FRESULT f_error) {
	if (fatFsErrors++ > 16) {
		// no reason to spam the console
		return;
	}

	scheduleMsg(&logger, "FATfs Error \"%s\" %d", str, f_error);
}

static FIL FDLogFile;
static FIL FDCurrFile;
static int logFileIndex = 1;
static char logName[_MAX_FILLER + 20];

static void printMmcPinout(void) {
	scheduleMsg(&logger, "MMC CS %s", hwPortname(boardConfiguration->sdCardCsPin));
	// todo: we need to figure out the right SPI pinout, not just SPI2
//	scheduleMsg(&logger, "MMC SCK %s:%d", portname(EFI_SPI2_SCK_PORT), EFI_SPI2_SCK_PIN);
//	scheduleMsg(&logger, "MMC MISO %s:%d", portname(EFI_SPI2_MISO_PORT), EFI_SPI2_MISO_PIN);
//	scheduleMsg(&logger, "MMC MOSI %s:%d", portname(EFI_SPI2_MOSI_PORT), EFI_SPI2_MOSI_PIN);
}

static void sdStatistics(void) {
	printMmcPinout();
	scheduleMsg(&logger, "SD enabled=%s status=%s", boolToString(boardConfiguration->isSdCardEnabled),
			sdStatus);
	if (fs_ready) {
		scheduleMsg(&logger, "filename=%s size=%d", logName, engine->engineState.totalLoggedBytes);
	}
}

static void incLogFileName(void) {
	lockSpi(SPI_NONE);
	memset(&FDCurrFile, 0, sizeof(FIL));						// clear the memory
	FRESULT err = f_open(&FDCurrFile, LOG_INDEX_FILENAME, FA_READ);				// This file has the index for next log file name

	char data[_MAX_FILLER];
	UINT result = 0;
	if (err != FR_OK && err != FR_EXIST) {
			logFileIndex = 1;
			scheduleMsg(&logger, "%s: not found or error: %d", LOG_INDEX_FILENAME, err);
	} else {
		f_read(&FDCurrFile, (void*)data, sizeof(data), &result);

		scheduleMsg(&logger, "Got content [%s] size %d", data, result);
		f_close(&FDCurrFile);
		if (result < 5) {
                      data[result] = 0;
			logFileIndex = atoi(data);
			if (absI(logFileIndex) == ERROR_CODE) {
				logFileIndex = 1;
			} else {
				logFileIndex++; // next file would use next file name
			}
		} else {
			logFileIndex = 1;
		}
	}

	err = f_open(&FDCurrFile, LOG_INDEX_FILENAME, FA_OPEN_ALWAYS | FA_WRITE);
	itoa10(data, logFileIndex);
	f_write(&FDCurrFile, (void*)data, strlen(data), &result);
	f_close(&FDCurrFile);
	scheduleMsg(&logger, "Done %d", logFileIndex);
	unlockSpi();
}

static void prepareLogFileName(void) {
	strcpy(logName, RUSEFI_LOG_PREFIX);
	bool result = dateToStringShort(&logName[PREFIX_LEN]);
	char *ptr;
	if (result) {
		ptr = &logName[PREFIX_LEN + SHORT_TIME_LEN];
	} else {
		ptr = itoa10(&logName[PREFIX_LEN], logFileIndex);
	}
	strcat(ptr, ".msl");

}

/**
 * @brief Create a new file with the specified name
 *
 * This function saves the name of the file in a global variable
 * so that we can later append to that file
 */
static void createLogFile(void) {
	lockSpi(SPI_NONE);
	memset(&FDLogFile, 0, sizeof(FIL));						// clear the memory
	prepareLogFileName();

	FRESULT err = f_open(&FDLogFile, logName, FA_OPEN_ALWAYS | FA_WRITE);				// Create new file
	if (err != FR_OK && err != FR_EXIST) {
		unlockSpi();
		sdStatus = SD_STATE_OPEN_FAILED;
		warning(CUSTOM_ERR_SD_MOUNT_FAILED, "SD: mount failed");
		printError("FS mount failed", err);	// else - show error
		return;
	}

	err = f_lseek(&FDLogFile, f_size(&FDLogFile)); // Move to end of the file to append data
	if (err) {
		unlockSpi();
		sdStatus = SD_STATE_SEEK_FAILED;
		warning(CUSTOM_ERR_SD_SEEK_FAILED, "SD: seek failed");
		printError("Seek error", err);
		return;
	}
	f_sync(&FDLogFile);
	fs_ready = true;						// everything Ok
	unlockSpi();
}

static void removeFile(const char *pathx) {
	if (!fs_ready) {
		scheduleMsg(&logger, "Error: No File system is mounted");
		return;
	}
	lockSpi(SPI_NONE);
	f_unlink(pathx);

	unlockSpi();
}

int
    mystrncasecmp(const char *s1, const char *s2, size_t n)
    {

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

	if (!fs_ready) {
		scheduleMsg(&logger, "Error: No File system is mounted");
		return;
	}
	lockSpi(SPI_NONE);

	DIR dir;
	FRESULT res = f_opendir(&dir, path);

	if (res != FR_OK) {
		scheduleMsg(&logger, "Error opening directory %s", path);
		unlockSpi();
		return;
	}

	scheduleMsg(&logger, LS_RESPONSE);

	int i = strlen(path);
	for (int count = 0;count < FILE_LIST_MAX_COUNT;) {
		FILINFO fno;

		res = f_readdir(&dir, &fno);
		if (res != FR_OK || fno.fname[0] == 0)
			break;
		if (fno.fname[0] == '.')
			continue;
		if ((fno.fattrib & AM_DIR) || mystrncasecmp(RUSEFI_LOG_PREFIX, fno.fname, sizeof(RUSEFI_LOG_PREFIX) - 1)) {
			continue;
		}
		scheduleMsg(&logger, "logfile%lu:%s", fno.fsize, fno.fname);
		count++;

//			scheduleMsg(&logger, "%c%c%c%c%c %u/%02u/%02u %02u:%02u %9lu  %-12s", (fno.fattrib & AM_DIR) ? 'D' : '-',
//					(fno.fattrib & AM_RDO) ? 'R' : '-', (fno.fattrib & AM_HID) ? 'H' : '-',
//					(fno.fattrib & AM_SYS) ? 'S' : '-', (fno.fattrib & AM_ARC) ? 'A' : '-', (fno.fdate >> 9) + 1980,
//					(fno.fdate >> 5) & 15, fno.fdate & 31, (fno.ftime >> 11), (fno.ftime >> 5) & 63, fno.fsize,
//					fno.fname);
	}
	unlockSpi();
}

static int errorReported = FALSE; // this is used to report the error only once

void readLogFileContent(char *buffer, short fileId, short offset, short length) {

}


/**
 * @brief Appends specified line to the current log file
 */
void appendToLog(const char *line) {
	UINT bytesWrited;

	if (!fs_ready) {
		if (!errorReported)
			scheduleMsg(&logger, "appendToLog Error: No File system is mounted");
		errorReported = TRUE;
		return;
	}
	UINT lineLength = strlen(line);
	engine->engineState.totalLoggedBytes += lineLength;
	lockSpi(SPI_NONE);
	FRESULT err = f_write(&FDLogFile, line, lineLength, &bytesWrited);
	if (bytesWrited < lineLength) {
		printError("write error or disk full", err); // error or disk full
	}
	f_sync(&FDLogFile);
	unlockSpi();
}

/*
 * MMC card umount.
 */
static void MMCumount(void) {
	if (!fs_ready) {
		scheduleMsg(&logger, "Error: No File system is mounted. \"mountsd\" first");
		return;
	}
	f_close(&FDLogFile);						// close file
	f_sync(&FDLogFile);							// sync ALL
	mmcDisconnect(&MMCD1);						// Brings the driver in a state safe for card removal.
	mmcStop(&MMCD1);							// Disables the MMC peripheral.
	f_mount(NULL, 0, 0);						// FATFS: Unregister work area prior to discard it
	memset(&FDLogFile, 0, sizeof(FIL));			// clear FDLogFile
	fs_ready = false;							// status = false
	scheduleMsg(&logger, "MMC/SD card removed");
}

#define RAMDISK_BLOCK_SIZE    512U
static uint8_t blkbuf[RAMDISK_BLOCK_SIZE];

/*
 * MMC card mount.
 */
static void MMCmount(void) {
//	printMmcPinout();

	if (fs_ready) {
		scheduleMsg(&logger, "Error: Already mounted. \"umountsd\" first");
		return;
	}
	if ((MMCD1.state == BLK_STOP) || (MMCD1.state == BLK_ACTIVE)) {
		// looks like we would only get here after manual unmount with mmcStop? Do we really need to ever mmcStop?
		// not sure if this code is needed
		// start to initialize MMC/SD
		mmcStart(&MMCD1, &mmccfg);					// Configures and activates the MMC peripheral.
	}

	// Performs the initialization procedure on the inserted card.
	lockSpi(SPI_NONE);
	sdStatus = SD_STATE_CONNECTING;
	if (mmcConnect(&MMCD1) != HAL_SUCCESS) {
		sdStatus = SD_STATE_NOT_CONNECTED;
		warning(CUSTOM_OBD_MMC_ERROR, "Can't connect or mount MMC/SD");
		unlockSpi();
		return;
	}

//	if (engineConfiguration->storageMode == MS_ALWAYS) {
#if HAL_USE_USB_MSD
	  msdObjectInit(&USBMSD1);

	BaseBlockDevice *bbdp = (BaseBlockDevice*)&MMCD1;
	  msdStart(&USBMSD1, usb_driver, bbdp, blkbuf, NULL);

//		  const usb_msd_driver_state_t msd_driver_state = msdInit(ms_usb_driver, bbdp, &UMSD1, USB_MS_DATA_EP, USB_MSD_INTERFACE_NUMBER);
	//	  UMSD1.chp = NULL;

		  /*Disconnect the USB Bus*/
		  usbDisconnectBus(usb_driver);
		  chThdSleepMilliseconds(200);
//
//		  /*Start the useful functions*/
//		  msdStart(&UMSD1);
		  usbStart(usb_driver, &msdusbcfg);
//
		  /*Connect the USB Bus*/
		  usbConnectBus(usb_driver);
#endif
	//}


	unlockSpi();
#if HAL_USE_USB_MSD
	sdStatus = SD_STATE_MOUNTED;
	return;
#endif

	// if Ok - mount FS now
	memset(&MMC_FS, 0, sizeof(FATFS));
	if (f_mount(&MMC_FS, "/", 1) == FR_OK) {
		sdStatus = SD_STATE_MOUNTED;
		incLogFileName();
		createLogFile();
		scheduleMsg(&logger, "MMC/SD mounted!");
	} else {
		sdStatus = SD_STATE_MOUNT_FAILED;
	}
}

static THD_FUNCTION(MMCmonThread, arg) {
	chRegSetThreadName("MMC_Monitor");

	while (true) {
		// this returns TRUE if SD module is there, even without an SD card?
		if (blkIsInserted(&MMCD1)) {

			if (!fs_ready) {
				MMCmount();
			}
		} else {
			sdStatus = SD_STATE_NOT_INSERTED;
		}

		if (isSdCardAlive())
			writeLogLine();

		chThdSleepMilliseconds(FILE_LOG_DELAY);
	}
}

bool isSdCardAlive(void) {
	return fs_ready;
}

void initMmcCard(void) {
	logName[0] = 0;
	addConsoleAction("sdinfo", sdStatistics);
	if (!boardConfiguration->isSdCardEnabled) {
		return;
	}

	hs_spicfg.ssport = ls_spicfg.ssport = getHwPort("mmc", boardConfiguration->sdCardCsPin);
	hs_spicfg.sspad = ls_spicfg.sspad = getHwPin("mmc", boardConfiguration->sdCardCsPin);
	mmccfg.spip = getSpiDevice(engineConfiguration->sdCardSpiDevice);

	/**
	 * FYI: SPI does not work with CCM memory, be sure to have main() stack in RAM, not in CCMRAM
	 */

	// start to initialize MMC/SD
	mmcObjectInit(&MMCD1); 						// Initializes an instance.
	mmcStart(&MMCD1, &mmccfg);

	chThdCreateStatic(mmcThreadStack, sizeof(mmcThreadStack), LOWPRIO, (tfunc_t) MMCmonThread, NULL);

	addConsoleAction("mountsd", MMCmount);
	addConsoleActionS("appendtolog", appendToLog);
	addConsoleAction("umountsd", MMCumount);
	addConsoleActionS("ls", listDirectory);
	addConsoleActionS("del", removeFile);
	addConsoleAction("incfilename", incLogFileName);
}

#endif /* EFI_FILE_LOGGING */
