/*
 * mc33972.c
 *
 * Multiple Switch Detection Interface with Suppressed Wake-up
 *
 * The 33972 Multiple Switch Detection Interface with suppressed
 * wake-up is designed to detect the closing and opening of up to 22
 * switch contacts: 14 switch to ground detection and 8 switch to 
 * ground or battery detection, 
 *
 * SPI protocol 3.3/5.0V
 *
 * @date Apr 07, 2019
 * @author Andrey Gusakov <dron0gus@gmail.com>, (c) 2019
 */

#include "global.h"
#include "gpio/gpio_ext.h"
#include "gpio/mc33972.h"
#include "pin_repository.h"

#if (BOARD_MC33972_COUNT > 0)

/*
 * TODO list:
 * - add irq support with fallback to polling mode (now polling mode only)
 */

/*==========================================================================*/
/* Driver local definitions.												*/
/*==========================================================================*/

#define DRIVER_NAME				"mc33972"

static bool drv_task_ready = false;

typedef enum {
	MC33972_DISABLED = 0,
	MC33972_WAIT_INIT,
	MC33972_READY,
	MC33972_FAILED
} mc33972_drv_state;

/* all commands and reply data are 24 bit */
#define CMD(cmd, data)				(((cmd) << 16) | ((data) << 0))

#define CMD_STATUS					CMD(0x00, 0x00)
#define CMD_SETTINGS(mask)			CMD(0x01, (mask))
#define CMD_WAKEUPEN(i, mask)		CMD((i) ? 0x03 : 0x02, (mask))
#define CMD_METALLIC(i, mask)		CMD((i) ? 0x05 : 0x04, (mask))
#define CMD_ANALOG(cur, ch)			CMD(0x06, (((curr) & 0x3) << 5) | (((d) & 0x1f) << 0))
#define CMD_WETTING_TMR(i, mask)	CMD((i) ? 0x08 : 0x07, (mask))
#define CMD_TRI_STATE(i, mask)		CMD((i) ? 0x0a : 0x09, (mask))
#define CMD_CALIB					CMD(0x0b, 0x00)
#define CMD_SLEEP(int_t, scan_t)	CMD(0x0c, (((int_t) & 0x7) << 3) | (((scan_t) & 0x7) << 0))
#define CMD_RST						CMD(0x7f, 0x00)

/* reply is allways same 24 status bits */
#define FLAG_THERM					(1 << 23)
#define FLAG_INT					(1 << 22)
/* from LSB to MSB: SG0..SG13, SP0..SP7 */
#define FLAG_PIN(pin)				(1 << (pin))

/*==========================================================================*/
/* Driver exported variables.												*/
/*==========================================================================*/

/*==========================================================================*/
/* Driver local variables and types.										*/
/*==========================================================================*/

/* OS */
SEMAPHORE_DECL(mc33972_wake, 10 /* or BOARD_MC33972_COUNT ? */);
static THD_WORKING_AREA(mc33972_thread_1_wa, 256);

/* Driver */
struct mc33972_priv {
	const struct mc33972_config	*cfg;
	/* last input state from chip */
	uint32_t					i_state;
	/* currently selected analog input */
	uint8_t						analog_ch;

	mc33972_drv_state			drv_state;
};

static struct mc33972_priv chips[BOARD_MC33972_COUNT];

/*==========================================================================*/
/* Driver local functions.													*/
/*==========================================================================*/

static SPIDriver *get_bus(struct mc33972_priv *chip)
{
	/* return non-const SPIDriver* from const struct cfg */
	return chip->cfg->spi_bus;
}

/**
 * @brief MC33972 send cmd routine.
 * @details Sends 24 bits of data. CS asserted before and released
 *  after transaction. Chip allways reply with input state + 2 bits
 *  of diagnostic. This routine save it to chip->i_state
 */

static int mc33972_spi_w(struct mc33972_priv *chip, uint32_t tx)
{
	int i;
	uint8_t rxb[3];
	uint8_t txb[3];
	SPIDriver *spi = get_bus(chip);

	txb[0] = (tx >> 16) & 0xff;
	txb[1] = (tx >>  8) & 0xff;
	txb[2] = (tx >>  0) & 0xff;
	/* Acquire ownership of the bus. */
	spiAcquireBus(spi);
	/* Setup transfer parameters. */
	spiStart(spi, &chip->cfg->spi_config);
	/* Slave Select assertion. */
	spiSelect(spi);
	/* Atomic transfer operations. */
	/* TODO: check why spiExchange transfers invalid data on STM32F7xx, DMA issue? */
	//spiExchange(spi, 3, txb, rxb);
	for (i = 0; i < 3; i++)
		rxb[i] = spiPolledExchange(spi, txb[i]);
	/* Slave Select de-assertion. */
	spiUnselect(spi);
	/* Ownership release. */
	spiReleaseBus(spi);

	/* save received data */
	chip->i_state = (rxb[0] << 16) | (rxb[1] << 8) | (rxb[2] << 0);

	/* no errors for now */
	return 0;
}

/**
 * @brief MC33972 update status
 * @details Chip reply with input data and two bits of diag
 */

static int mc33972_update_status(struct mc33972_priv *chip)
{
	int ret;

	/* TODO: lock? */

	ret = mc33972_spi_w(chip, CMD_STATUS);
	
	/* TODO: unlock? */

	return ret;
}

/**
 * @brief MC33972 chip init.
 * @details There is no way to check communication.
 *  Performs reset.
 */

static int mc33972_chip_init(struct mc33972_priv *chip)
{
	int ret;

	ret = mc33972_spi_w(chip, CMD_RST);
	if (ret)
		goto err;

	/* is it enought? */
	chThdSleepMilliseconds(3);

	/*
	 * Default settings from Power-ON Reset via V PWR or the
	 * Reset Command are as follows:
	 * * Programmable switch – set to switch to battery
	 * * All inputs set as wake-up
	 * * Wetting current on (16 mA)
	 * * Wetting current timer on (20 ms)
	 * * All inputs tri-state
	 * * Analog select 00000 (no input channel selected)
	 */

	/* disable tri-state */
	ret  = mc33972_spi_w(chip, CMD_TRI_STATE(0, 0));
	ret |= mc33972_spi_w(chip, CMD_TRI_STATE(1, 0));
	if (ret)
		goto err;

	/* Set wetting current to 2 mA */
	ret  = mc33972_spi_w(chip, CMD_METALLIC(0, 0));
	ret |= mc33972_spi_w(chip, CMD_METALLIC(1, 0));
	if (ret)
		goto err;

err:
	return ret;
}

/**
 * @brief MC33972 chip driver wakeup.
 * @details Wake up driver. Will cause input and diagnostic
 *  update
 */

static int mc33972_wake_driver(struct mc33972_priv *chip)
{
	(void)chip;

	chSemSignal(&mc33972_wake);

	return 0;
}

/*==========================================================================*/
/* Driver thread.															*/
/*==========================================================================*/

static THD_FUNCTION(mc33972_driver_thread, p)
{
	int i;
	msg_t msg;

	(void)p;

	chRegSetThreadName(DRIVER_NAME);

	while(1) {
		msg = chSemWaitTimeout(&mc33972_wake, TIME_MS2I(MC33972_POLL_INTERVAL_MS));

		/* should we care about msg == MSG_TIMEOUT? */
		(void)msg;

		for (i = 0; i < BOARD_MC33972_COUNT; i++) {
			int ret;
			struct mc33972_priv *chip;

			chip = &chips[i];
			if ((chip->cfg == NULL) ||
				(chip->drv_state == MC33972_DISABLED) ||
				(chip->drv_state == MC33972_FAILED))
				continue;

			ret = mc33972_update_status(chip);
			if (ret) {
				/* set state to MC33972_FAILED? */
			}
		}
	}
}

/*==========================================================================*/
/* Driver interrupt handlers.												*/
/*==========================================================================*/

/* TODO: add IRQ support */

/*==========================================================================*/
/* Driver exported functions.												*/
/*==========================================================================*/

int mc33972_readPad(void *data, unsigned int pin)
{
	struct mc33972_priv *chip;

	if ((pin >= MC33972_INPUTS) || (data == NULL))
		return -1;

	chip = (struct mc33972_priv *)data;

	/* convert to some common enum? */
	return !!(chip->i_state & FLAG_PIN(pin));
}

int mc33972_getDiag(void *data, unsigned int pin)
{
	int diag;
	struct mc33972_priv *chip;

	if ((pin >= MC33972_INPUTS) || (data == NULL))
		return -1;

	chip = (struct mc33972_priv *)data;

	/* one diag for all pins */
	diag = !!(chip->i_state & FLAG_THERM);

	/* convert to some common enum? */
	return diag;
}

int mc33972_init(void * data)
{
	int ret;
	struct mc33972_priv *chip;
	const struct mc33972_config *cfg;

	chip = (struct mc33972_priv *)data;
	cfg = chip->cfg;

	/* mark pins used */
	ret = gpio_pin_markUsed(cfg->spi_config.ssport, cfg->spi_config.sspad, DRIVER_NAME " CS");

	ret = mc33972_chip_init(chip);
	if (ret)
		return ret;

	chip->drv_state = MC33972_READY;

	if (!drv_task_ready) {
		chThdCreateStatic(mc33972_thread_1_wa, sizeof(mc33972_thread_1_wa),
						  NORMALPRIO + 1, mc33972_driver_thread, NULL);
		drv_task_ready = true;
	}

	return 0;
}

int mc33972_deinit(void *data)
{
	(void)data;

	/* TODO: set all pins to inactive state, stop task? */
	return 0;
}

struct gpiochip_ops mc33972_ops = {
	.writePad	= NULL,	/* chip input only */
	.readPad	= mc33972_readPad,
	.getDiag	= mc33972_getDiag,
	.init		= mc33972_init,
	.deinit 	= mc33972_deinit,
};

/**
 * @brief MC33972 driver add.
 * @details Checks for valid config
 */

int mc33972_add(unsigned int index, const struct mc33972_config *cfg)
{
	struct mc33972_priv *chip;

	/* no config or no such chip */
	if ((!cfg) || (!cfg->spi_bus) || (index >= BOARD_MC33972_COUNT))
		return -1;

	/* check for valid cs.
	 * DOTO: remove this check? CS can be driven by SPI */
	if (cfg->spi_config.ssport == NULL)
		return -1;

	chip = &chips[index];

	/* already initted? */
	if (chip->cfg != NULL)
		return -1;

	chip->cfg = cfg;
	chip->i_state = 0;
	chip->drv_state = MC33972_WAIT_INIT;

	/* register, return gpio chip base */
	return gpiochip_register(DRIVER_NAME, &mc33972_ops, MC33972_INPUTS, chip);
}

#else /* BOARD_MC33972_COUNT > 0 */

int mc33972_add(unsigned int index, const struct mc33972_config *cfg)
{
	(void)index; (void)cfg;

	return -1;
}

#endif /* BOARD_MC33972_COUNT */
