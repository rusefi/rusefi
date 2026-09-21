/**
 * @file boards/microrusefi/board_storage.cpp
 *
 * @brief Storage configuration file
 *
 * @date May 27, 2024
 * @author Andrey Gusakov, 2023
 */

#include "pch.h"

/* This board stores settings in external SPI flash */
#if !defined(EFI_BOOTLOADER) && (EFI_STORAGE_MFS == TRUE)

#include "hal_serial_nor.h"
#include "hal_mfs.h"

/* QSPI storage */
#define EFI_FLASH_QSPI_AF		9U
#define EFI_FLASH_QSPI_CS		Gpio::B10
#define EFI_FLASH_QSPI_CLK		Gpio::B2
#define EFI_FLASH_QSPI_IO0		Gpio::C9
#define EFI_FLASH_QSPI_IO1		Gpio::D12
#define EFI_FLASH_QSPI_IO2		Gpio::E2
#define EFI_FLASH_QSPI_IO3		Gpio::D13

/* 16 Mbytes */
/* Not used, just FYI */
#define EFI_FLASH_SIZE (16 * 1024 * 1024)

/* Some fields in following struct are used for DMA transfers, so do not cache */
/* TODO: can we drop NO_CACHE for snor1 since snor1buf? */
static NO_CACHE SNORDriver snor1;
static NO_CACHE snor_nocache_buffer_t snor1buf;

/*
 * QSPI interface driver configuration.
 */
const WSPIConfig WSPIcfg1 = {
	.end_cb			= NULL,
	.error_cb		= NULL,
	.dcr			= STM32_DCR_FSIZE(24U) |	/* 16MB device.         */
					  STM32_DCR_CSHT(1U)		/* NCS 2 cycles delay.  */
};

/*
 * Flash driver configuration.
 */
const SNORConfig snorcfg1 = {
	.busp			= &WSPID1,
	.buscfg			= &WSPIcfg1
};

const MFSConfig mfsd_nor_config = {
	.flashp			= (BaseFlash *)&snor1,
	.erased			= 0xFFFFFFFFU,
	.bank_size		= 512 * 1024U,
	.bank0_start	= 0U,
	.bank0_sectors	= 128U,	/* 128 * 4 K = 0.5 Mb */
	.bank1_start	= 128U,
	.bank1_sectors	= 128U
};

bool boardInitMfs() {
	palSetPadMode(getBrainPinPort(EFI_FLASH_QSPI_CS), getBrainPinIndex(EFI_FLASH_QSPI_CS),
		PAL_MODE_ALTERNATE(EFI_FLASH_QSPI_AF) | PAL_STM32_OSPEED_HIGHEST);
	palSetPadMode(getBrainPinPort(EFI_FLASH_QSPI_CLK), getBrainPinIndex(EFI_FLASH_QSPI_CLK),
		PAL_MODE_ALTERNATE(EFI_FLASH_QSPI_AF) | PAL_STM32_OSPEED_HIGHEST);
	palSetPadMode(getBrainPinPort(EFI_FLASH_QSPI_IO0), getBrainPinIndex(EFI_FLASH_QSPI_IO0),
		PAL_MODE_ALTERNATE(EFI_FLASH_QSPI_AF) | PAL_STM32_OSPEED_HIGHEST);
	palSetPadMode(getBrainPinPort(EFI_FLASH_QSPI_IO1), getBrainPinIndex(EFI_FLASH_QSPI_IO1),
		PAL_MODE_ALTERNATE(EFI_FLASH_QSPI_AF) | PAL_STM32_OSPEED_HIGHEST);
	palSetPadMode(getBrainPinPort(EFI_FLASH_QSPI_IO2), getBrainPinIndex(EFI_FLASH_QSPI_IO2),
		PAL_MODE_ALTERNATE(EFI_FLASH_QSPI_AF) | PAL_STM32_OSPEED_HIGHEST);
	palSetPadMode(getBrainPinPort(EFI_FLASH_QSPI_IO3), getBrainPinIndex(EFI_FLASH_QSPI_IO3),
		PAL_MODE_ALTERNATE(EFI_FLASH_QSPI_AF) | PAL_STM32_OSPEED_HIGHEST);

	/*
	 * Initializing and starting flash driver.
	 */
#if SNOR_SHARED_BUS == FALSE
	wspiStart(&WSPID1, &WSPIcfg1);
#endif
	snorObjectInit(&snor1, &snor1buf);
	snorStart(&snor1, &snorcfg1);

	// Non-zero chip size meanst its ID is correct
	if (snor_descriptor.size) {
		return true;
	}

	return false;
}

const MFSConfig *boardGetMfsConfig() {
	return &mfsd_nor_config;
}

#endif /* EFI_STORAGE_MFS == TRUE */
