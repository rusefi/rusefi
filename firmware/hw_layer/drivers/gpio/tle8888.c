/*
 * tle8888.c
 *
 * TLE8888 Engine Machine System IC driver
 *
 * This has worked on a bench - see https://youtu.be/yjs5dh_NKo4
 * All SPI and CS pin in OM_DEFAULT mode
 *
 * @date Mar 25, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 *
 * 3.2 Pin Definitions and Functions
 *
 * IN1-4  Parallel input; Input pin for direct control of power stage OUT1-4
 *
 * IN5-8  Parallel input; Input pin for direct control of push pull state IGN1-IGN4
 *
 * Table 24 Direct Drive Input Assignment to Output Stages
 * IN9 to IN12 OUT5 to OUT24
 *
 * Masks/inputs bits:
 * 0..3   - OUT1 .. 4 - INJ - OpenDrain: 2.2A - direct
 * 4..6   - OUT5 .. 7 -       OpenDrain: 4.5A
 * 7..12  - OUT8 ..13 -       Push/Pull: 20mA - quite weak
 * 13..19 - OUT14..20 -       OpenDrain: 0.6A
 * 20..23 - OUT21..24 -       Push/Pull: 0.6A - half bridge (GND or +12v)
 * 24..27 - IGN1 .. 4 -       Push/Pull: 20mA - direct
 *
 * Andrey Gusakov, (c) 2019
 */

#include "global.h"

#include "gpio/tle8888.h"

#if (BOARD_TLE8888_COUNT > 0)

#include "hardware.h"
#include "gpio/gpio_ext.h"
#include "pin_repository.h"
#include "rfiutil.h"

/* to be removed  */
#if EFI_TUNER_STUDIO
#include "engine_configuration.h"
EXTERN_CONFIG;
#include "tunerstudio.h"
extern TunerStudioOutputChannels tsOutputChannels;
#endif /* EFI_TUNER_STUDIO */

/*
 * TODO list:
 */

/*==========================================================================*/
/* Driver local definitions.												*/
/*==========================================================================*/

#define DRIVER_NAME				"tle8888"

static bool drv_task_ready = false;

typedef enum {
	TLE8888_DISABLED = 0,
	TLE8888_WAIT_INIT,
	TLE8888_READY,
	TLE8888_FAILED
} tle8888_drv_state;

/* C0 */
#define CMD_READ			(0 << 0)
#define CMD_WRITE			(1 << 0)
/* C7:1 */
#define CMD_REG_ADDR(a)		(((a) & 0x7f) << 1)
/* CD7:0 */
#define CMD_REG_DATA(d)		(((d) & 0xff) << 8)

#define CMD_WR(a, d)		(CMD_WRITE | CMD_REG_ADDR(a) | CMD_REG_DATA(d))

#define CMD_SR				CMD_WR(0x1a, 0x03)
// 0x238 = 568
#define CMD_OE_SET			CMD_WR(0x1c, 0x02)
/* not used
#define CMD_OE_CLR			CMD_WR(0x1c, 0x01)
#define CMD_LOCK			CMD_WR(0x1e, 0x02)
*/
#define CMD_UNLOCK			CMD_WR(0x1e, 0x01)
#define CMD_INCONFIG(n, d)	CMD_WR(0x53 + (n & 0x03), d)
#define CMD_DDCONFIG(n, d)	CMD_WR(0x57 + (n & 0x03), d)
#define CMD_OECONFIG(n, d)	CMD_WR(0x5b + (n & 0x03), d)
#define CMD_CONT(n, d)		CMD_WR(0x7b + (n & 0x03), d)
/*==========================================================================*/
/* Driver exported variables.												*/
/*==========================================================================*/

/*==========================================================================*/
/* Driver local variables and types.										*/
/*==========================================================================*/

/* OS */
SEMAPHORE_DECL(tle8888_wake, 10 /* or BOARD_TLE8888_COUNT ? */);
static THD_WORKING_AREA(tle8888_thread_1_wa, 256);

static int tle8888SpiCounter = 0;
static int initResponses = 0;

/* Driver private data */
struct tle8888_priv {
	const struct tle8888_config	*cfg;
	/* cached output state - state last send to chip */
	uint32_t					o_state_cached;
	/* state to be sended to chip */
	uint32_t					o_state;
	/* direct driven output mask */
	uint32_t					o_direct_mask;
	/* output enabled mask */
	uint32_t					o_oe_mask;

	tle8888_drv_state			drv_state;
};

static struct tle8888_priv chips[BOARD_TLE8888_COUNT];

static const char* tle8888_pin_names[TLE8888_OUTPUTS] = {
	"TLE8888.INJ1",		"TLE8888.INJ2",		"TLE8888.INJ3",		"TLE8888.INJ4",
	"TLE8888.OUT5",		"TLE8888.OUT6",		"TLE8888.OUT7",		"TLE8888.OUT8",
	"TLE8888.OUT9",		"TLE8888.OUT10",	"TLE8888.OUT11",	"TLE8888.OUT12",
	"TLE8888.OUT13",	"TLE8888.OUT14",	"TLE8888.OUT15",	"TLE8888.OUT16",
	"TLE8888.OUT17",	"TLE8888.OUT18",	"TLE8888.OUT19",	"TLE8888.OUT20",
	"TLE8888.OUT21",	"TLE8888.OUT22",	"TLE8888.OUT23",	"TLE8888.OUT24",
	"TLE8888.IGN1",		"TLE8888.IGN2",		"TLE8888.IGN3",		"TLE8888.IGN4"
};

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
	uint16_t rxb = spiPolledExchange(spi, tx);
	//spiExchange(spi, 2, &tx, &rxb); 8 bit version just in case?
	/* Slave Select de-assertion. */
	spiUnselect(spi);
	/* Ownership release. */
	spiReleaseBus(spi);

#if EFI_TUNER_STUDIO
	if (engineConfiguration->debugMode == DBG_TLE8888) {
		tsOutputChannels.debugIntField1 = tle8888SpiCounter++;
		tsOutputChannels.debugIntField2 = tx;
		tsOutputChannels.debugIntField3 = rxb;
		tsOutputChannels.debugIntField4 = initResponses;
	}
#endif /* EFI_TUNER_STUDIO */

	if (rx)
		*rx = rxb;

	/* no errors for now */
	return 0;
}

/**
 * @brief TLE8888 send output registers data.
 * @details Sends ORed data to register, also receive 2-bit diagnostic.
 */

static int tle8888_update_output(struct tle8888_priv *chip)
{
	int ret = 0;

	/* TODO: lock? */

	/* set value only for non-direct driven pins */
	uint32_t out_data = chip->o_state & (~chip->o_direct_mask);
	for (int i = 0; i < 4; i++) {
		uint8_t od;

		od = (out_data >> (8 * i)) & 0xff;
		ret |= tle8888_spi_rw(chip, CMD_CONT(i, od), NULL);
	}

	if (ret == 0) {
		/* atomic */
		chip->o_state_cached = out_data;
	}

	/* TODO: unlock? */

	return ret;
}

static int tle8888_update_direct_output(struct tle8888_priv *chip, int pin, int value)
{
	const struct tle8888_config	*cfg = chip->cfg;

	/* find direct drive gpio */
	for (int i = 0; i < TLE8888_DIRECT_MISC; i++) {
		/* again: outputs in cfg counted starting from 1 - hate this */
		if (cfg->direct_io[i].output == pin + 1) {
			if (value)
				palSetPort(cfg->direct_io[i].port,
						   PAL_PORT_BIT(cfg->direct_io[i].pad));
			else
				palClearPort(cfg->direct_io[i].port,
						   PAL_PORT_BIT(cfg->direct_io[i].pad));
			return 0;
		}
	}

	/* direct gpio not found */
	return -1;
}

/**
 * @brief TLE8888 chip driver wakeup.
 * @details Wake up driver. Will cause output register update
 */

static int tle8888_wake_driver(struct tle8888_priv *chip)
{
	(void)chip;

//	if (isIsrContext()) {
		// this is for normal runtime
		int wasLocked = lockAnyContext();
		chSemSignalI(&tle8888_wake);
		if (!wasLocked) {
			unlockAnyContext();
		}
//	} else {
//		// this is for start-up to not hang up
//		chSemSignal(&tle8888_wake);
//	}

	return 0;
}

/*==========================================================================*/
/* Driver thread.															*/
/*==========================================================================*/

static THD_FUNCTION(tle8888_driver_thread, p)
{
	msg_t msg;

	(void)p;

	chRegSetThreadName(DRIVER_NAME);

	while (1) {
		msg = chSemWaitTimeout(&tle8888_wake, TIME_MS2I(TLE8888_POLL_INTERVAL_MS));

		/* should we care about msg == MSG_TIMEOUT? */
		(void)msg;

		for (int i = 0; i < BOARD_TLE8888_COUNT; i++) {
			struct tle8888_priv *chip;

			chip = &chips[i];
			if ((chip->cfg == NULL) ||
				(chip->drv_state == TLE8888_DISABLED) ||
				(chip->drv_state == TLE8888_FAILED))
				continue;

			int ret = tle8888_update_output(chip);
			if (ret) {
				/* set state to TLE8888_FAILED? */
			}
		}
	}
}

/*==========================================================================*/
/* Driver interrupt handlers.												*/
/*==========================================================================*/

/*==========================================================================*/
/* Driver exported functions.												*/
/*==========================================================================*/

int tle8888_writePad(void *data, unsigned int pin, int value)
{
	struct tle8888_priv *chip;

	if ((pin >= TLE8888_OUTPUTS) || (data == NULL))
		return -1;

	chip = (struct tle8888_priv *)data;

	/* TODO: lock */
	if (value)
		chip->o_state |=  (1 << pin);
	else
		chip->o_state &= ~(1 << pin);
	/* TODO: unlock */
	/* direct driven? */
	if (chip->o_direct_mask & (1 << pin)) {
		return tle8888_update_direct_output(chip, pin, value);
	} else {
		return tle8888_wake_driver(chip);
	}
	return 0;
}

int tle8888_chip_init(void * data)
{
	struct tle8888_priv *chip = (struct tle8888_priv *)data;
	const struct tle8888_config	*cfg = chip->cfg;

	int ret = 0;
	/* mark pins used */
	// we do not initialize CS pin so we should not be marking it used - i'm sad
	//ret = gpio_pin_markUsed(cfg->spi_config.ssport, cfg->spi_config.sspad, DRIVER_NAME " CS");
	if (cfg->reset.port != NULL)
		ret |= gpio_pin_markUsed(cfg->reset.port, cfg->reset.pad, DRIVER_NAME " RST");
	for (int i = 0; i < TLE8888_DIRECT_MISC; i++)
		if (cfg->direct_io[i].port)
			ret |= gpio_pin_markUsed(cfg->direct_io[i].port, cfg->direct_io[i].pad, DRIVER_NAME " DIRECT IO");

	if (ret) {
		ret = -1;
		goto err_gpios;
	}

	/* Software reset */
	// first packet: 0x335=821 > 0xFD=253
	uint16_t response;
	tle8888_spi_rw(chip, CMD_SR, &response);
	if (response == 253) {
		initResponses += 4;
	}

	/**
	 * Table 8. Reset Times. All reset times not more than 20uS
	 *
	 */
	chThdSleepMilliseconds(3);

	/* Set LOCK bit to 0 */
	// second 0x13D=317 => 0x35=53
	tle8888_spi_rw(chip, CMD_UNLOCK, &response);
	if (response == 53) {
		initResponses += 8;
	}

	chip->o_direct_mask = 0;
	chip->o_oe_mask		= 0;
	/* enable direct drive of OUTPUT4..1
	 * ...still need INJEN signal */
	chip->o_direct_mask	|= 0x0000000f;
	chip->o_oe_mask		|= 0x0000000f;
	/* enable direct drive of IGN4..1
	 * ...still need IGNEN signal */
	chip->o_direct_mask |= 0x0f000000;
	chip->o_oe_mask		|= 0x0f000000;

	/* map and enable outputs for direct driven channels */
	for (int i = 0; i < TLE8888_DIRECT_MISC; i++) {
		int out;
		uint32_t mask;

		out = cfg->direct_io[i].output;

		/* not used? */
		if (out == 0)
			continue;

		/* OUT1..4 driven direct only through dedicated pins */
		if (out < 5)
			return -1;

		/* in config counted from 1 */
		mask = (1 << (out - 1));

		/* check if output already ocupied */
		if (chip->o_direct_mask & mask) {
			/* incorrect config? */
			return -1;
		}

		/* enable direct drive and output enable */
		chip->o_direct_mask	|= mask;
		chip->o_oe_mask		|= mask;

		/* set INCONFIG - aux input mapping */
		tle8888_spi_rw(chip, CMD_INCONFIG(i, out - 5), NULL);
	}

	/* enable all ouputs
	 * TODO: add API to enable/disable? */
	chip->o_oe_mask		|= 0x0ffffff0;

	/* set OE and DD registers */
	for (int i = 0; i < 4; i++) {
		uint8_t oe, dd;

		oe = (chip->o_oe_mask >> (8 * i)) & 0xff;
		dd = (chip->o_direct_mask >> (8 * i)) & 0xff;
		tle8888_spi_rw(chip, CMD_OECONFIG(i, oe), NULL);
		tle8888_spi_rw(chip, CMD_DDCONFIG(i, dd), NULL);
	}

	/* enable outputs */
	tle8888_spi_rw(chip, CMD_OE_SET, NULL);

err_gpios:
	/* unmark pins */
	//gpio_pin_markUnused(cfg->spi_config.ssport, cfg->spi_config.sspad);
	if (cfg->reset.port != NULL)
		gpio_pin_markUnused(cfg->reset.port, cfg->reset.pad);
	for (int i = 0; i < TLE8888_DIRECT_MISC; i++)
		if (cfg->direct_io[i].port)
			gpio_pin_markUnused(cfg->direct_io[i].port, cfg->direct_io[i].pad);

	return ret;
}

int tle8888_init(void * data)
{
	int ret;
	struct tle8888_priv *chip;

	chip = (struct tle8888_priv *)data;

	/* check for multiple init */
	if (chip->drv_state != TLE8888_WAIT_INIT)
		return -1;

	ret = tle8888_chip_init(chip);
	if (ret)
		return ret;

	chip->drv_state = TLE8888_READY;

	/* one task for all TLE8888 instances, so create only once */
	if (!drv_task_ready) {
		chThdCreateStatic(tle8888_thread_1_wa, sizeof(tle8888_thread_1_wa),
						  NORMALPRIO + 1, tle8888_driver_thread, NULL);
		drv_task_ready = true;
	}

	return 0;
}

int tle8888_deinit(void *data)
{
	(void)data;

	/* TODO: set all pins to inactive state, stop task? */
	return 0;
}

struct gpiochip_ops tle8888_ops = {
	.writePad	= tle8888_writePad,
	.readPad	= NULL,	/* chip outputs only */
	//.getDiag	= tle8888_getDiag,
	.init		= tle8888_init,
	.deinit 	= tle8888_deinit,
};

/**
 * @brief TLE8888 driver add.
 * @details Checks for valid config
 * @return return gpio chip base
 */

int tle8888_add(unsigned int index, const struct tle8888_config *cfg)
{
	int ret;
	struct tle8888_priv *chip;

	efiAssert(OBD_PCM_Processor_Fault, cfg != NULL, "8888CFG", 0)

	/* no config or no such chip */
	if ((!cfg) || (!cfg->spi_bus) || (index >= BOARD_TLE8888_COUNT))
		return -1;

	/* check for valid cs.
	 * TODO: remove this check? CS can be driven by SPI */
	if (cfg->spi_config.ssport == NULL)
		return -1;

	chip = &chips[index];

	/* already initted? */
	if (chip->cfg != NULL)
		return -1;

	chip->cfg = cfg;
	chip->o_state = 0;
	chip->o_state_cached = 0;
	chip->o_direct_mask = 0;
	chip->drv_state = TLE8888_WAIT_INIT;

	/* register, return gpio chip base */
	ret = gpiochip_register(DRIVER_NAME, &tle8888_ops, TLE8888_OUTPUTS, chip);

	/* set default pin names, board init code can rewrite */
	gpiochips_setPinNames(ret, tle8888_pin_names);

	return ret;
}

#else /* BOARD_TLE8888_COUNT > 0 */

int tle8888_add(unsigned int index, const struct tle8888_config *cfg)
{
	(void)index; (void)cfg;

	return -1;
}

#endif /* (BOARD_TLE8888_COUNT > 0) */
