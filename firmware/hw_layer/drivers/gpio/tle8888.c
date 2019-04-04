/*
 * tle8888.c
 *
 * TLE8888 Engine Machi ne System IC driver
 *
 *
 * @date Apr 04, 2019
 * @author Andrey Gusakov, (c) 2019
 */

#include "global.h"
#include "gpio/gpio_ext.h"
#include "pin_repository.h"

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

/* !!!LSB first!!! */

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
#define CMD_DDCONFIG(n, d)	CMD_WR(0x57 + (n & 0x03), d)
#define CMD_OECONFIG(n, d)	CMD_WR(0x5b + (n & 0x03), d)

/*==========================================================================*/
/* Driver local variables and types.										*/
/*==========================================================================*/


struct tle8888_config {
	SPIDriver		*spi_bus;
	const SPIConfig	spi_config;
};
/* Driver */
struct tle8888_priv {
	const struct tle8888_config	*cfg;
};

static struct tle8888_priv chips[BOARD_TLE6240_COUNT];

/*==========================================================================*/
/* Driver local functions.													*/
/*==========================================================================*/

static SPIDriver *get_bus(struct tle6240_priv *chip)
{
	/* return non-const SPIDriver* from const struct cfg */
	return chip->cfg->spi_bus;
}

static int tle8888_spi_rw(struct tle8888_priv *chip, uint16_t tx, uint16_t *rx)
{

	uint16_t rxb;
	SPIDriver *spi = get_bus(chip);

	/* Acquire ownership of the bus. */
	spiAcquireBus(spi);
	/* Setup transfer parameters. */
	spiStart(spi, &chip->cfg->spi_config);
	/* Slave Select assertion. */
	spiSelect(spi);
	/* Atomic transfer operations. */
	rxb = spiPolledExchange(spi, tx);
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

int tle8888_init(void)
{
	/* Software reset */
	tle8888_spi_rw(chip, CMD_SR, NULL);

	/* Set LOCK bit to 0 */
	tle8888_spi_rw(chip, CMD_UNLOCK, NULL);

	/* IN9 drives OUTPUT5 */
	tle8888_spi_rw(chip, CMD_INCONFIG(0, 0), NULL);
	/* IN10 drives OUTPUT6 */
	tle8888_spi_rw(chip, CMD_INCONFIG(1, 1), NULL);
	/* IN11 drives OUTPUT7 */
	tle8888_spi_rw(chip, CMD_INCONFIG(2, 2), NULL);
	/* IN12 drives OUTPUT8 */
	tle8888_spi_rw(chip, CMD_INCONFIG(3, 3), NULL);

	/* enable direct drive of OUTPUT8..1
	 * See settings above...
	 * OUTPUT4..1 still need INJEN signal */
	tle8888_spi_rw(chip, CMD_DDCONFIG(0, 0xff), NULL);
	tle8888_spi_rw(chip, CMD_OECONFIG(0, 0xff), NULL);
	/* enable direct drive of IGN4..1
	 * still need IGNEN signal */
	tle8888_spi_rw(chip, CMD_DDCONFIG(3, 0x0f), NULL);
	tle8888_spi_rw(chip, CMD_OECONFIG(3, 0x0f), NULL);

	/* enable outputs for spi driven channels */
	tle8888_spi_rw(chip, CMD_OECONFIG(1, 0xff), NULL);
	tle8888_spi_rw(chip, CMD_OECONFIG(2, 0xff), NULL);

	/* enable outputs */
	tle8888_spi_rw(chip, CMD_OE_SET, NULL);
}

#else /* BOARD_TLE8888_COUNT > 0 */


#endif /* BOARD_TLE8888_COUNT */
