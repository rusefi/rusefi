/**
 * @file	mmc_card.c
 *
 * @date Dec 28, 2013
 * @author Kot_dnz
 * @author Andrey Belomutskiy, (c) 2012-2014
 *
 * default pinouts in case of SPI2 connected to MMC: PB13 - SCK, PB14 - MISO, PB15 - MOSI, PD4 - CS, 3.3v
 * default pinouts in case of SPI3 connected to MMC: PB3  - SCK, PB4  - MISO, PB5  - MOSI, PD4 - CS, 3.3v
 *
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

extern board_configuration_s *boardConfiguration;

#define PUSHPULLDELAY 500

static THD_WORKING_AREA(mmcThreadStack,UTILITY_THREAD_STACK_SIZE);		// MMC monitor thread

/**
 * MMC driver instance.
 */
MMCDriver MMCD1;

static SPIConfig hs_spicfg = { NULL, SPI_SD_MODULE_PORT, SPI_SD_MODULE_PIN,
SPI_BaudRatePrescaler_8 };
static SPIConfig ls_spicfg = { NULL, SPI_SD_MODULE_PORT, SPI_SD_MODULE_PIN,
SPI_BaudRatePrescaler_256 };

/* MMC/SD over SPI driver configuration.*/
// don't forget check if STM32_SPI_USE_SPI2 defined and spi has init with correct GPIO in hardware.c
static MMCConfig mmccfg = { &MMC_CARD_SPI, &ls_spicfg, &hs_spicfg };

static bool fs_ready = false;

#define PUSHPULLDELAY 500

/**
 * fatfs MMC/SPI
 */
static FATFS MMC_FS;

static Logging logger;

// print FAT error function
static void printError(char *str, FRESULT f_error) {
	scheduleMsg(&logger, "FATfs Error \"%s\" %d", str, f_error);
}

static FIL FDLogFile;

static int totalLoggedBytes = 0;

static void printMmcPinout(void) {
	scheduleMsg(&logger, "MMC CS %s:%d", portname(SPI_SD_MODULE_PORT), SPI_SD_MODULE_PIN);
	// todo: we need to figure out the right SPI pinout, not just SPI2
//	scheduleMsg(&logger, "MMC SCK %s:%d", portname(EFI_SPI2_SCK_PORT), EFI_SPI2_SCK_PIN);
//	scheduleMsg(&logger, "MMC MISO %s:%d", portname(EFI_SPI2_MISO_PORT), EFI_SPI2_MISO_PIN);
//	scheduleMsg(&logger, "MMC MOSI %s:%d", portname(EFI_SPI2_MOSI_PORT), EFI_SPI2_MOSI_PIN);
}

static void sdStatistics(void) {
	printMmcPinout();
	scheduleMsg(&logger, "fs_ready=%d totalLoggedBytes=%d", fs_ready, totalLoggedBytes);
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
	FRESULT err = f_open(&FDLogFile, "rusefi.log", FA_OPEN_ALWAYS | FA_WRITE);				// Create new file
	if (err != FR_OK && err != FR_EXIST) {
		unlockSpi();
		printError("Card mounted...\r\nCan't create Log file, check your SD.\r\nFS mount failed", err);	// else - show error
		return;
	}

	err = f_lseek(&FDLogFile, f_size(&FDLogFile)); // Move to end of the file to append data
	if (err) {
		unlockSpi();
		printError("Seek error", err);
		return;
	}
	f_sync(&FDLogFile);
	fs_ready = true;						// everything Ok
	unlockSpi();
}

static void ff_cmd_dir(const char *pathx) {
	char *path = (char *)pathx; // todo: fix this hack!
	DIR dir;
	FILINFO fno;
	char *fn;

	if (!fs_ready) {
		scheduleMsg(&logger, "Error: No File system is mounted");
		return;
	}

	FRESULT res = f_opendir(&dir, path);

	if (res != FR_OK) {
		scheduleMsg(&logger, "Error opening directory %s", path);
		return;
	}

	int i = strlen(path);
	for (;;) {
		res = f_readdir(&dir, &fno);
		if (res != FR_OK || fno.fname[0] == 0)
			break;
		if (fno.lfname[0] == '.')
			continue;
		fn = fno.lfname;
		if (fno.fattrib & AM_DIR) {
			// TODO: WHAT? WE ARE APPENDING FILE NAME TO PARAMETER??? WEIRD!!!
			path[i++] = '/';
			strcpy(&path[i], fn);
			// res = ff_cmd_ls(path);
			if (res != FR_OK)
				break;
			path[i] = 0;
		} else {
			scheduleMsg(&logger, "%c%c%c%c%c %u/%02u/%02u %02u:%02u %9lu  %-12s", (fno.fattrib & AM_DIR) ? 'D' : '-',
					(fno.fattrib & AM_RDO) ? 'R' : '-', (fno.fattrib & AM_HID) ? 'H' : '-',
					(fno.fattrib & AM_SYS) ? 'S' : '-', (fno.fattrib & AM_ARC) ? 'A' : '-', (fno.fdate >> 9) + 1980,
					(fno.fdate >> 5) & 15, fno.fdate & 31, (fno.ftime >> 11), (fno.ftime >> 5) & 63, fno.fsize,
					fno.fname);
		}
	}
}

static int errorReported = FALSE; // this is used to report the error only once

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
	totalLoggedBytes += lineLength;
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
	f_mount(0, NULL);							// FATFS: Unregister work area prior to discard it
	memset(&FDLogFile, 0, sizeof(FIL));			// clear FDLogFile
	fs_ready = false;							// status = false
	scheduleMsg(&logger, "MMC/SD card removed");
}

/*
 * MMC card mount.
 */
static void MMCmount(void) {
//	printMmcPinout();

	if (fs_ready) {
		scheduleMsg(&logger, "Error: Already mounted. \"umountsd\" first");
		return;
	}
	// start to initialize MMC/SD
	mmcObjectInit(&MMCD1);						// Initializes an instance.
	mmcStart(&MMCD1, &mmccfg);					// Configures and activates the MMC peripheral.

	// Performs the initialization procedure on the inserted card.
	lockSpi(SPI_NONE);
	if (mmcConnect(&MMCD1) != CH_SUCCESS) {
		warning(OBD_PCM_Processor_Fault, "Can't connect or mount MMC/SD");
		unlockSpi();
		return;

	}
	unlockSpi();
	// if Ok - mount FS now
	memset(&MMC_FS, 0, sizeof(FATFS));			// reserve the memory
	if (f_mount(0, &MMC_FS) == FR_OK) {
		createLogFile();
		scheduleMsg(&logger, "MMC/SD mounted!\r\nDon't forget umountsd before remove to prevent lost your data");
	}
}

#if defined __GNUC__
__attribute__((noreturn))       static msg_t MMCmonThread(void)
#else
static msg_t MMCmonThread(void)
#endif
{
	chRegSetThreadName("MMC_Monitor");

	while (true) {
		// this returns TRUE if SD module is there, even without an SD card?
		if (blkIsInserted(&MMCD1)) {

			if (!fs_ready) {
				MMCmount();
			}
		}

		// this thread is activated 10 times per second
		chThdSleepMilliseconds(PUSHPULLDELAY);
	}
}

bool isSdCardAlive(void) {
	return fs_ready;
}

void initMmcCard(void) {
	initLogging(&logger, "mmcCard");
	if (!boardConfiguration->isSdCardEnabled) {
		return;
	}

	/**
	 * FYI: SPI does not work with CCM memory, be sure to have main() stack in RAM, not in CCMRAM
	 */

	// start to initialize MMC/SD
	mmcObjectInit(&MMCD1);
	mmcStart(&MMCD1, &mmccfg);

	chThdCreateStatic(mmcThreadStack, sizeof(mmcThreadStack), LOWPRIO, (tfunc_t) MMCmonThread, NULL);

	addConsoleAction("sdstat", sdStatistics);
	addConsoleAction("mountsd", MMCmount);
	addConsoleActionS("appendToLog", appendToLog);
	addConsoleAction("umountsd", MMCumount);
	addConsoleActionS("ls", ff_cmd_dir);
}

#endif /* EFI_FILE_LOGGING */
