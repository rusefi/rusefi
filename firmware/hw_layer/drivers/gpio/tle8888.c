/*
 * tle8888.c
 *
 * TLE8888 Engine Machine System IC driver
 *
 * @date Mar 25, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 *
 * Andrey Gusakov, (c) 2019
 */

#include "global.h"

#if EFI_TLE8888 || defined(__DOXYGEN__)

/* to be removed */
#include "engine_configuration.h"

#include "gpio/gpio_ext.h"
#include "gpio/tle8888.h"
#include "pin_repository.h"

/* TODO: move to board.h file */
#define BOARD_TLE8888_COUNT 	1

#ifndef  BOARD_TLE8888_COUNT
	#define BOARD_TLE8888_COUNT 0
#endif

#if (BOARD_TLE8888_COUNT > 0)

/*
 * TODO list:
 */

/*==========================================================================*/
/* Driver local definitions.												*/
/*==========================================================================*/

#define DRIVER_NAME				"tle8888"

/*==========================================================================*/
/* Driver exported variables.												*/
/*==========================================================================*/

/* C0 */
#define CMD_READ			(0 << 0)
#define CMD_WRITE			(1 << 0)
/* C7:1 */
#define CMD_REG_ADDR(a)		(((a) & 0x7f) << 1)
/* CD7:0 */
#define CMD_REG_DATA(d)		(((d) & 0xff) << 8)

#define CMD_WR(a, d)		(CMD_WRITE | CMD_REG_ADDR(a) | CMD_REG_DATA(d))

#define CMD_SR				CMD_WR(0x1a, 0x03)
#define CMD_OE_SET			CMD_WR(0x1c, 0x02)
#define CMD_OE_CLR			CMD_WR(0x1c, 0x01)
#define CMD_LOCK			CMD_WR(0x1e, 0x02)
#define CMD_UNLOCK			CMD_WR(0x1e, 0x01)
#define CMD_INCONFIG(n, d)	CMD_WR(0x53 + (n & 0x03), d)
#define CMD_DDCONFIG(n, d)	CMD_WR(0x57 + (n & 0x03), d)
#define CMD_OECONFIG(n, d)	CMD_WR(0x5b + (n & 0x03), d)

/*==========================================================================*/
/* Driver local variables and types.										*/
/*==========================================================================*/

/* Driver private data */
struct tle8888_priv {
	const struct tle8888_config	*cfg;
};

static struct tle8888_priv chips[BOARD_TLE8888_COUNT];

/*==========================================================================*/
/* Driver local functions.													*/
/*==========================================================================*/

static SPIDriver *get_bus(struct tle8888_priv *chip)
{
	/* return non-const SPIDriver* from const struct cfg */
	return chip->cfg->spi_bus;
}

static int tle8888_spi_rw(struct tle8888_priv *chip, uint16_t tx, uint16_t *rx)
{

	uint16_t rxb;
	SPIDriver *spi = get_bus(chip);

	/**
	 * 15.1 SPI Protocol
	 *
	 * after a read or write command: the address and content of the selected register
	 * is transmitted with the next SPI transmission (for not existing addresses or
	 * wrong access mode the data is always 0)
	 */

	/* Acquire ownership of the bus. */
	spiAcquireBus(spi);
	/* Setup transfer parameters. */
	spiStart(spi, &chip->cfg->spi_config);
	/* Slave Select assertion. */
	spiSelect(spi);
	/* Atomic transfer operations. */
	// todo 16 bit for F4? rxb = spiPolledExchange(spi, tx);
	spiExchange(spi, 2, &tx, &rxb);
	/* Slave Select de-assertion. */
	spiUnselect(spi);
	/* Ownership release. */
	spiReleaseBus(spi);

	if (rx)
		*rx = rxb;

	/* no errors for now */
	return 0;
}


/*==========================================================================*/
/* Driver thread.															*/
/*==========================================================================*/

/*==========================================================================*/
/* Driver interrupt handlers.												*/
/*==========================================================================*/

/*==========================================================================*/
/* Driver exported functions.												*/
/*==========================================================================*/

int tle8888_chip_init(void * data)
{
	int i;
	int ret;
	struct tle8888_priv *chip = (struct tle8888_priv *)data;
	const struct tle8888_config	*cfg = chip->cfg;
	uint8_t dd[4] = {0, 0, 0, 0};
	uint8_t oe[4] = {0, 0, 0, 0};

	/* mark pins used */
	ret = markUsed(cfg->spi_config.ssport, cfg->spi_config.sspad, DRIVER_NAME " CS");
	if (cfg->reset.port != NULL)
		ret |= markUsed(cfg->reset.port, cfg->reset.pad, DRIVER_NAME " RST");
	for (i = 0; i < TLE8888_DIRECT_OUTPUTS; i++)
		if (cfg->direct_io[i].port)
			ret |= markUsed(cfg->direct_io[i].port, cfg->direct_io[i].pad, DRIVER_NAME " DIRECT IO");
	if (ret) {
		ret = -1;
		goto err_gpios;
	}

	/* Software reset */
	tle8888_spi_rw(chip, CMD_SR, NULL);

	/* delay? */

	/* Set LOCK bit to 0 */
	tle8888_spi_rw(chip, CMD_UNLOCK, NULL);

	/* enable direct drive of OUTPUT4..1
	 * ...still need INJEN signal */
	dd[0] |= 0x0f;
	oe[0] |= 0x0f;
	/* enable direct drive of IGN4..1
	 * ...still need IGNEN signal */
	dd[3] |= 0x0f;
	oe[3] |= 0x0f;

	/* map and enable outputs for direct driven channels */
	for (i = 0; i < TLE8888_DIRECT_MISC; i++) {
		int reg;
		int mask;
		int out = cfg->direct_map[i];

		/* OUT1..4 driven direct only */
		if (out < 5)
			return -1;

		reg = (out - 1)/ 8;
		mask = 1 << ((out - 1) % 8);

		/* check if output already ocupied */
		if (dd[reg] & mask) {
			/* incorrect config? */
			return -1;
		}

		/* enable direct drive and output enable */
		dd[reg] |= mask;
		oe[reg] |= mask;

		tle8888_spi_rw(chip, CMD_INCONFIG(reg, out - 5), NULL);
	}

	/* set registers */
	for (i = 0; i < 4; i++) {
		tle8888_spi_rw(chip, CMD_OECONFIG(i, oe[i]), NULL);
		tle8888_spi_rw(chip, CMD_DDCONFIG(i, dd[i]), NULL);
	}

	/* enable outputs */
	tle8888_spi_rw(chip, CMD_OE_SET, NULL);

err_gpios:
	/* unmark pins */
	markUnused(cfg->spi_config.ssport, cfg->spi_config.sspad);
	if (cfg->reset.port != NULL)
		markUnused(cfg->reset.port, cfg->reset.pad);
	for (i = 0; i < TLE8888_DIRECT_OUTPUTS; i++)
		if (cfg->direct_io[i].port)
			markUnused(cfg->direct_io[i].port, cfg->direct_io[i].pad);

	return ret;
}

/**
 * @brief TLE8888 driver add.
 * @details Checks for valid config
 */

int tle8888_add(unsigned int index, const struct tle8888_config *cfg)
{
	struct tle8888_priv *chip;

	/* no config or no such chip */
	osalDbgCheck((cfg != NULL) && (cfg->spi_bus != NULL) && (index < BOARD_TLE8888_COUNT));

	/* check for valid cs.
	 * DOTO: remove this check? CS can be driven by SPI */
	if (cfg->spi_config.ssport == NULL)
		return -1;

	chip = &chips[index];

	/* already initted? */
	if (chip->cfg != NULL)
		return -1;

	/* TODO: remove this when gpiochips integrated */
	return tle8888_chip_init(chip);
}

#else /* BOARD_TLE8888_COUNT > 0 */

int tle8888_add(unsigned int index, const struct tle8888_config *cfg)
{
	(void)index; (void)cfg;

	return -1;
}

#endif /* BOARD_TLE8888_COUNT */

/*********TO BE REMOVED FROM THIS FILE***************/

/* this should be in board file */
static const struct tle8888_config tle8888_cfg = {
	.spi_bus = NULL/*&SPID4*/,
	.spi_config = {
		.circular = false,
		.end_cb = NULL,
		.ssport = GPIOF,
		.sspad = 0U,
		.cr1 =
			/* TODO: set LSB first mode !!!! */
			/* SPI_CR1_CPOL | */ // = 0
			SPI_CR1_CPHA | // = 1
			(((3 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |
			SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSTR),
		/* 16-bit transfer */
		.cr2 = 0/* not for F4? SPI_CR2_DS_3 | SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0*/
	},
	/* not implemented yet, use STM32 gpios directly */
	.direct_io = {
		[0] = {.port = NULL},
		[1] = {.port = NULL},
		[2] = {.port = NULL},
		[3] = {.port = NULL},
	},
	.direct_map = {
		/* to be fixed */
		9, 10, 11, 12
	},
};

void initTle8888(DECLARE_ENGINE_PARAMETER_SIGNATURE)
{
	tle8888_add(0, &tle8888_cfg);
}

/*********TO BE REMOVED FROM THIS FILE ENDS***********/

#endif /* EFI_TLE8888 */
