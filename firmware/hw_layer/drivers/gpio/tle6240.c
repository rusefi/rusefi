/*
 * tle6240.cpp
 *
 * TLE6240GP Smart 16-Channel Low-Side Switch
 *
 * All 16 channels can be controlled via the serial interface (SPI).
 * In addition to the serial control it is possible to control channel 1 to 4 
 * and 9 to 12 direct in parallel with a separate input pin.
 *
 * Looks like 3.3v SI and SCLK are NOT possible (H above 0.7Vs required, that's 3.5v for 5.0Vs)
 * 5 MHz SPI
 * Update: looks like possible:
 * DS page 3: "Compatible with 3 V Microcontrollers"
 * DS page 12: "Input High Voltage 2.0 V min"
 *
 * @date Dec 29, 2018
 * @author Andrey Belomutskiy, (c) 2012-2018
 *
 * @date Mar 06, 2019
 * @author Andrey Gusakov, (c) 2019
 */

#include <hal.h>
#include "gpio/gpio_ext.h"
#include "gpio/tle6240.h"
#include "pin_repository.h"

#ifndef  BOARD_TLE6240_COUNT
	#define BOARD_TLE6240_COUNT 0
#endif

#if (BOARD_TLE6240_COUNT > 0)

/*==========================================================================*/
/* Driver local definitions.												*/
/*==========================================================================*/

#define DRIVER_NAME				"tle6240"

static bool drv_task_ready = false;

typedef enum {
	TLE6240_DISABLED = 0,
	TLE6240_WAIT_INIT,
	TLE6240_READY,
	TLE6240_FAILED
} tle6240_drv_state;

/* set 0000b for channes == 0..7 and 1111b for channels 8..15 */
#define CMD_CHIP(ch)			((ch < 8) ? 0x00 : 0x0f)
/* Full Diagnoscit, data byte ignored */
#define CMD_FULL_DIAG(ch)		(((0x00 | CMD_CHIP(ch)) << 8) | 0x00)
/* Get state of 8 paralled inputs and 1-bit Diagnostic, data byte ignored */
#define CMD_IO_SHORTDIAG(ch)	(((0xc0 | CMD_CHIP(ch)) << 8) | 0x00)
/* Echo function test of SPI, SI will be connected to SO on next access */
#define CMD_ECHO(ch)			(((0xA0 | CMD_CHIP(ch)) << 8) | 0x00)
/* in data ORed, Full diagnostic output on next access */
#define CMD_OR_DIAG(ch, data)	(((0x30 | CMD_CHIP(ch)) << 8) | (data & 0xff))
/* in data ANDed, Full diagnostic output on next access */
#define CMD_AND_DIAG(ch, data)	(((0xf0 | CMD_CHIP(ch)) << 8) | (data & 0xff))

/*==========================================================================*/
/* Driver exported variables.												*/
/*==========================================================================*/

/*==========================================================================*/
/* Driver local variables and types.										*/
/*==========================================================================*/

/* OS */
SEMAPHORE_DECL(tle6240_wake, 10 /* or BOARD_TLE6240_COUNT ? */);
static THD_WORKING_AREA(tle6240_thread_1_wa, 256);

/* Driver */
struct tle6240_priv {
	const struct tle6240_config	*cfg;
	/* cached output state - state last send to chip */
	uint16_t					o_state_cached;
	/* state to be sended to chip */
	uint16_t					o_state;
	/* direct driven output mask */
	uint16_t					o_direct_mask;
	/* full diagnostic status */
	uint16_t					diag[2];
	/* diagnostic for ch 8..15 was requsted by last access
	 * can skip one transaction next time */
	bool						diag_8_reguested;

	tle6240_drv_state			drv_state;
};

static struct tle6240_priv chips[BOARD_TLE6240_COUNT];

/*==========================================================================*/
/* Driver local functions.													*/
/*==========================================================================*/

static SPIDriver *get_bus(struct tle6240_priv *chip)
{
	/* return non-const SPIDriver* from const struct cfg */
	return chip->cfg->spi_bus;
}

/**
 * @brief TLE6240 send and receive routine.
 * @details Sends and receives 16 bits. CS asserted before and released
 * after transaction.
 */

static int tle6240_spi_rw(struct tle6240_priv *chip, uint16_t tx, uint16_t *rx)
{
	static uint8_t txb[2];
	static uint8_t rxb[2];
	SPIDriver *spi = get_bus(chip);

	/* fill tx */
	txb[0] = (tx >> 8) & 0xff;
	txb[1] = (tx >> 0) & 0xff;
	/* Acquire ownership of the bus. */
	spiAcquireBus(spi);
	/* Setup transfer parameters. */
	spiStart(spi, &chip->cfg->spi_config);
	/* Slave Select assertion. */
	spiSelect(spi);
	/* Atomic transfer operations. */
	/* TODO: check why spiExchange transfers invalid data on STM32F7xx */
	//spiExchange(spi, 2, txb, rxb);
	rxb[0] = spiPolledExchange(spi, txb[0]);
	rxb[1] = spiPolledExchange(spi, txb[1]);
	/* Slave Select de-assertion. */
	spiUnselect(spi);
	/* Ownership release. */
	spiReleaseBus(spi);

	if (rx)
		*rx = (rxb[0] << 8) | rxb[1];

	/* no errors for now */
	return 0;
}

/**
 * @brief TLE6240 send output registers data.
 * @details Sends ORed data to register, also receive 2-bit diagnostic.
 */

static int tle6240_update_output_and_diag(struct tle6240_priv *chip)
{
	int ret;
	uint16_t out_data;

	out_data = chip->o_state & chip->o_direct_mask;
	if (chip->diag_8_reguested) {
		/* diagnostic for OUT8..15 was requested on prev access */
		ret  = tle6240_spi_rw(chip, CMD_OR_DIAG(0, (out_data >> 0) & 0xff), &chip->diag[1]);
		ret |= tle6240_spi_rw(chip, CMD_OR_DIAG(8, (out_data >> 8) & 0xff), &chip->diag[0]);
	} else {
		ret  = tle6240_spi_rw(chip, CMD_OR_DIAG(0, (out_data >> 0) & 0xff), NULL);
		ret |= tle6240_spi_rw(chip, CMD_OR_DIAG(8, (out_data >> 8) & 0xff), &chip->diag[0]);
		/* send same one more time to receive OUT8..15 diagnostic */
		ret  = tle6240_spi_rw(chip, CMD_OR_DIAG(8, (out_data >> 8) & 0xff), &chip->diag[1]);
	}

	chip->diag_8_reguested = false;
	if (ret == 0) {
		chip->o_state_cached = out_data;
		chip->diag_8_reguested = true;
	}

	return ret;
}

/**
 * @brief TLE6240 chip init.
 * @details Checks communication. Mark all used pins.
 * Checks direct io signals integrity using test cmd.
 * Reads initial diagnostic state.
 */

static int tle6240_chip_init(struct tle6240_priv *chip)
{
	int n;
	int ret;
	uint16_t rx;
	const struct tle6240_config *cfg = chip->cfg;

	/* mask pins */
	ret = markUsed(cfg->spi_config.ssport, cfg->spi_config.sspad, DRIVER_NAME " CS");
	if (cfg->reset.ssport != NULL)
		ret |= markUsed(cfg->reset.ssport, cfg->reset.sspad, DRIVER_NAME " RST");
	for (n = 0; n < TLE6240_DIRECT_OUTPUTS; n++)
		if (cfg->direct_io[n].ssport)
			ret |= markUsed(cfg->direct_io[n].ssport, cfg->direct_io[n].sspad, DRIVER_NAME " DIRECT IO");

	if (ret) {
		ret = -1;
		goto err_gpios;
	}

	/* release reset */
	if (cfg->reset.ssport != NULL) {
		palClearPort(cfg->reset.ssport,
					 PAL_PORT_BIT(cfg->reset.sspad));
		chThdSleepMilliseconds(1);
		palSetPort(cfg->reset.ssport,
				   PAL_PORT_BIT(cfg->reset.sspad));
		chThdSleepMilliseconds(10);
	}

	/* check SPI communication */
	/* 0. set echo mode, chip number - don't care */
	ret  = tle6240_spi_rw(chip, CMD_ECHO(0), NULL);
	/* 1. check loopback */
	ret |= tle6240_spi_rw(chip, 0x5555, &rx);
	if (ret || (rx != 0x5555)) {
		//print(DRIVER_NAME " spi loopback test failed\n");
		ret = -2;
		goto err_gpios;
	}

	/* check direct io communication */
	/* 0. set all direct out to 0 */
	for (n = 0; n < TLE6240_DIRECT_OUTPUTS; n++) {
		int i = (n < 4) ? n : (n + 4);
		if (chip->o_direct_mask & (1 << i)) {
			palClearPort(cfg->direct_io[n].ssport,
						 PAL_PORT_BIT(cfg->direct_io[n].sspad));
		}
	}
	/* 1. disable IN0..7 outputs first (ADNed with 0x00)
	 *    also will get full diag on next access */
	ret  = tle6240_spi_rw(chip, CMD_AND_DIAG(0, 0x00), NULL);
	/* 2. get diag for OUT0..7 and send disable OUT8..15 */
	ret |= tle6240_spi_rw(chip, CMD_AND_DIAG(8, 0x00), &chip->diag[0]);
	/* 3. get diag for OUT8..15 adn readback input status */
	ret |= tle6240_spi_rw(chip, CMD_IO_SHORTDIAG(0), &chip->diag[1]);
	/* 4. send dummy short diag command and get 8 bit of input data and
	 *    8 bit of short diag */
	ret |= tle6240_spi_rw(chip, CMD_IO_SHORTDIAG(0), &rx);
	rx = ((rx >> 4) & 0x0f00) | ((rx >> 8) & 0x000f);
	if (ret || (rx & chip->o_direct_mask)) {
		//print(DRIVER_NAME " direct io test #1 failed (invalid io mask %04x)\n", (rx ^ chip->o_direct_mask));
		ret = -3;
		goto err_gpios;
	}

	/* 5. set all direct io to 1 */
	for (n = 0; n < TLE6240_DIRECT_OUTPUTS; n++) {
		int i = (n < 4) ? n : (n + 4);
		if (chip->o_direct_mask & (1 << i)) {
			palSetPort(cfg->direct_io[n].ssport,
					   PAL_PORT_BIT(cfg->direct_io[n].sspad));
		}
	}
	/* 6. read chort diagnostic again */
	ret |= tle6240_spi_rw(chip, CMD_IO_SHORTDIAG(0), &rx);
	rx = ((rx >> 4) & 0x0f00) | ((rx >> 8) & 0x000f);
	rx &= chip->o_direct_mask;
	if (ret || (rx != chip->o_direct_mask)) {
		//print(DRIVER_NAME " direct io test #2 failed (invalid io mask %04x)\n", (rx & chip->o_direct_mask));
		ret = -4;
		goto err_gpios;
	}

	/* 7. set all all pins to OR mode, and upload pin states */
	ret = tle6240_update_output_and_diag(chip);
	if (ret) {
		//print(DRIVER_NAME " final setup error\n");
		ret = -5;
		goto err_gpios;
	}

	return 0;

err_gpios:
	/* mask pins */
	markUnused(cfg->spi_config.ssport, cfg->spi_config.sspad);
	if (cfg->reset.ssport != NULL)
		markUnused(cfg->reset.ssport, cfg->reset.sspad);
	for (n = 0; n < TLE6240_DIRECT_OUTPUTS; n++)
		if (cfg->direct_io[n].ssport)
			markUnused(cfg->direct_io[n].ssport, cfg->direct_io[n].sspad);

	return ret;
}

/**
 * @brief TLE6240 chip driver wakeup.
 * @details Wake up driver. Will cause output register and
 * diagnostic update.
 */

static int tle6240_wake_driver(struct tle6240_priv *chip)
{
	(void)chip;

	chSemSignal(&tle6240_wake);

	return 0;
}

/*==========================================================================*/
/* Driver thread.															*/
/*==========================================================================*/

static THD_FUNCTION(tle6240_driver_thread, p)
{
	int i;
	msg_t msg;

	(void)p;

	chRegSetThreadName(DRIVER_NAME);

	while(1) {
		msg = chSemWaitTimeout(&tle6240_wake, TIME_MS2I(TLE6240_POLL_INTERVAL_MS));

		/* should we care about msg == MSG_TIMEOUT? */
		(void)msg;

		for (i = 0; i < BOARD_TLE6240_COUNT; i++) {
			int ret;
			struct tle6240_priv *chip;

			chip = &chips[i];
			if ((chip->cfg == NULL) ||
				(chip->drv_state == TLE6240_DISABLED) ||
				(chip->drv_state == TLE6240_FAILED))
				continue;

			ret = tle6240_update_output_and_diag(chip);
			if (ret) {
				/* set state to TLE6240_FAILED? */
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

int tle6240_SetPort(void *data, unsigned int sspad)
{
	struct tle6240_priv *chip;

	if ((sspad >= TLE6240_DIRECT_OUTPUTS) || (data == NULL))
		return -1;

	chip = (struct tle6240_priv *)data;

	/* TODO: lock */
	chip->o_state |= (1 << sspad);
	/* TODO: unlock */
	/* direct driven? */
	if (chip->o_direct_mask & (1 << sspad)) {
		int n = (sspad < 8) ? sspad : (sspad - 4);
		palSetPort(chip->cfg->direct_io[n].ssport,
				   PAL_PORT_BIT(chip->cfg->direct_io[n].sspad));
	} else {
		tle6240_wake_driver(chip);
	}

	return 0;
}

int tle6240_ClearPort(void *data, unsigned int sspad)
{
	struct tle6240_priv *chip;

	if ((sspad >= TLE6240_DIRECT_OUTPUTS) || (data == NULL))
		return -1;

	chip = (struct tle6240_priv *)data;

	/* TODO: lock */
	chip->o_state &= ~(1 << sspad);
	/* TODO: unlock */
	/* direct driven? */
	if (chip->o_direct_mask & (1 << sspad)) {
		int n = (sspad < 8) ? sspad : (sspad - 4);
		palClearPort(chip->cfg->direct_io[n].ssport,
					 PAL_PORT_BIT(chip->cfg->direct_io[n].sspad));
	} else {
		tle6240_wake_driver(chip);
	}

	return 0;
}

int tle6240_GetDiag(void *data, unsigned int sspad)
{
	int diag;
	struct tle6240_priv *chip;

	if ((sspad >= TLE6240_DIRECT_OUTPUTS) || (data == NULL))
		return -1;

	chip = (struct tle6240_priv *)data;

	diag = (chip->diag[(sspad > 7) ? 1 : 0] >> ((sspad % 8) * 2)) & 0x03;

	/* convert? */
	return diag;
}

int tle6240_init(void * data)
{
	int ret;
	struct tle6240_priv *chip;

	chip = (struct tle6240_priv *)data;

	ret = tle6240_chip_init(chip);
	if (ret)
		return ret;

	chip->drv_state = TLE6240_READY;

	if (!drv_task_ready) {
		chThdCreateStatic(tle6240_thread_1_wa, sizeof(tle6240_thread_1_wa),
						  NORMALPRIO + 1, tle6240_driver_thread, NULL);
		drv_task_ready = true;
	}

	return 0;
}

int tle6240_deinit(void *data)
{
	(void)data;

	/* TODO */
	return 0;
}

struct gpiochip_ops tle6240_ops = {
	.setPad		= tle6240_SetPort,
	.clearPad	= tle6240_ClearPort,
	.getPad		= NULL,
	.getDiag	= tle6240_GetDiag,
	.init		= tle6240_init,
	.deinit 	= tle6240_deinit,
};

/**
 * @brief TLE6240 driver add.
 * @details Checks for valid config
 */

int tle6240_add(unsigned int n, const struct tle6240_config *cfg)
{
	int i;
	struct tle6240_priv *chip;

	/* no config or no such chip */
	osalDbgCheck((cfg != NULL) && (cfg->spi_bus != NULL) && (n < BOARD_TLE6240_COUNT));

	/* check for valid cs */
	if (cfg->spi_config.ssport == NULL)
		return -1;

	chip = &chips[n];

	/* already initted? */
	if (chip->cfg != NULL)
		return -1;

	chip->cfg = cfg;
	chip->o_state = 0;
	chip->o_state_cached = 0;
	chip->o_direct_mask = 0;
	chip->drv_state = TLE6240_WAIT_INIT;
	for (i = 0; i < TLE6240_DIRECT_OUTPUTS; i++) {
		if (cfg->direct_io[i].ssport != 0)
			chip->o_direct_mask |= (1 << ((i < 4) ? i : (i + 4)));
	}

	chip->drv_state = TLE6240_WAIT_INIT;

	/* register, return gpio chip base */
	return gpiochip_register(DRIVER_NAME, &tle6240_ops, -1, TLE6240_OUTPUTS, chip);
}

#else /* BOARD_TLE6240_COUNT > 0 */

int tle6240_add(unsigned int n, const struct tle6240_config *cfg)
{
	(void)n; (void)cfg;

	return -1;
}

#endif /* BOARD_TLE6240_COUNT */
