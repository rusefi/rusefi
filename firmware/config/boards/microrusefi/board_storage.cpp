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

/* SPI storage */
#define EFI_FLASH_SPI_CS_GPIO	GPIOE
#define EFI_FLASH_SPI_CS_PIN	15

#define EFI_FLASH_SPI_AF		5U
#define EFI_FLASH_SPI_SCK		Gpio::B13
#define EFI_FLASH_SPI_MISO		Gpio::B14
#define EFI_FLASH_SPI_MOSI		Gpio::B15
#define EFI_FLASH_SDPID			SPID2

#define EFI_FLASH_WP			Gpio::B10
#define EFI_FLASH_HOLD			Gpio::B11

/* 8 Mbytes */
/* Not used, just FYI */
#define EFI_FLASH_SIZE (8 * 1024 * 1024)

/* Some fields in following struct are used for DMA transfers, so do not cache */
/* TODO: can we drop NO_CACHE for snor1 since snor1buf? */
static NO_CACHE SNORDriver snor1;
static NO_CACHE snor_nocache_buffer_t snor1buf;

/*
 * Maximum speed SPI configuration (Clock = Fpclk / 2 =  21 MHz, CPHA=0, CPOL=0, MSb first).
 * SPI2 is clocked from APB1, APB1 clock is 42MHz
 * W25Qxx supports up to 133MHz in single SPI mode
 */
static const SPIConfig W25SpiCfg = {
	.circular = false,
#ifdef _CHIBIOS_RT_CONF_VER_6_1_
	.end_cb = NULL,
#else
	.slave = false,
	.data_cb = NULL,
	.error_cb = NULL,
#endif
	.ssport = EFI_FLASH_SPI_CS_GPIO,
	.sspad = EFI_FLASH_SPI_CS_PIN,
	.cr1 =
		SPI_CR1_8BIT_MODE |
		((0 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |
		0,
	.cr2 =
		SPI_CR2_8BIT_MODE |
		0,
};

/*
 * Flash driver configuration.
 */
static const SNORConfig W25FlashConfig = {
	.busp = &EFI_FLASH_SDPID,
	.buscfg = &W25SpiCfg
};

const MFSConfig mfsd_nor_config = {
	.flashp			= (BaseFlash *)&snor1,
	.erased			= 0xFFFFFFFFU,
#if 1
	/* it takes:
	 * 147 mS to write 25K of settings whithout garbage collector (when there is free space in current bank)
	 * 4750 mS to write settings with garbage collection (packing and moving to another bank, erasing old one)
	 * GC happens rougly every ((512 / 25) - 1) ~= 19 write */
	.bank_size		= 512 * 1024U,
	.bank0_start	= 0U,
	.bank0_sectors	= 128U,	/* 128 * 4 K = 0.5 Mb */
	.bank1_start	= 128U,
	.bank1_sectors	= 128U
#else
	/* it takes:
	 * same 147 mS to write setting without GC
	 * 1500 mS to write setting with GC, but GC happens every time we write settings */
	.bank_size		= 64 * 1024U,
	.bank0_start	= 0U,
	.bank0_sectors	= 16U,	/* 16 * 4 K = 64 Kb */
	.bank1_start	= 16U,
	.bank1_sectors	= 16U
#endif
};

void boardInitMfs()
{
#if SNOR_SHARED_BUS == FALSE
	spiStart(&EFI_FLASH_SDPID, &W25SpiCfg);
#endif

	palSetPad(EFI_FLASH_SPI_CS_GPIO, EFI_FLASH_SPI_CS_PIN);
	palSetPadMode(EFI_FLASH_SPI_CS_GPIO, EFI_FLASH_SPI_CS_PIN,
		PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);
	palSetPadMode(getBrainPinPort(EFI_FLASH_SPI_SCK), getBrainPinIndex(EFI_FLASH_SPI_SCK),
		PAL_MODE_ALTERNATE(EFI_FLASH_SPI_AF) | PAL_STM32_OSPEED_HIGHEST);
	palSetPadMode(getBrainPinPort(EFI_FLASH_SPI_MISO), getBrainPinIndex(EFI_FLASH_SPI_MISO),
		PAL_MODE_ALTERNATE(EFI_FLASH_SPI_AF) | PAL_STM32_OSPEED_HIGHEST);
	palSetPadMode(getBrainPinPort(EFI_FLASH_SPI_MOSI), getBrainPinIndex(EFI_FLASH_SPI_MOSI),
		PAL_MODE_ALTERNATE(EFI_FLASH_SPI_AF) | PAL_STM32_OSPEED_HIGHEST);
	/* Deactivate WP */
	palSetPad(getBrainPinPort(EFI_FLASH_WP), getBrainPinIndex(EFI_FLASH_WP));
	palSetPadMode(getBrainPinPort(EFI_FLASH_WP), getBrainPinIndex(EFI_FLASH_WP),
		PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);
	/* Deactivate HOLD */
	palSetPad(getBrainPinPort(EFI_FLASH_HOLD), getBrainPinIndex(EFI_FLASH_HOLD));
	palSetPadMode(getBrainPinPort(EFI_FLASH_HOLD), getBrainPinIndex(EFI_FLASH_HOLD),
		PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);

	/*
	 * Initializing and starting flash driver.
	 */
	snorObjectInit(&snor1, &snor1buf);
	snorStart(&snor1, &W25FlashConfig);
}

const MFSConfig *boardGetMfsConfig()
{
	return &mfsd_nor_config;
}

#endif /* EFI_STORAGE_MFS == TRUE */
