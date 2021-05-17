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
#include "thread_priority.h"

#if (BOARD_MC33972_COUNT > 0)

/*
 * TODO list:
 * - add irq support with fallback to polling mode (now polling mode only)
 */

/*==========================================================================*/
/* Driver local definitions.												*/
/*==========================================================================*/

#define DRIVER_NAME				"mc33972"

typedef enum {
	MC33972_DISABLED = 0,
	MC33972_WAIT_INIT,
	MC33972_READY,
	MC33972_FAILED
} mc33972_drv_state;

#define SP_BANK						0
#define SG_BANK						1

/* all commands and reply data are 24 bit */
#define CMD(cmd, data)				(((cmd) << 16) | ((data) << 0))

#define CMD_STATUS					CMD(0x00,		0x00)
#define CMD_SETTINGS(mask)			CMD(0x01,		(mask))
#define CMD_WAKEUPEN(i, mask)		CMD(0x02 + (i),	(mask))
#define CMD_METALLIC(i, mask)		CMD(0x04 + (i),	(mask))
#define CMD_ANALOG(curr, ch)		CMD(0x06,		(((curr) & 0x3) << 5) | (((ch) & 0x1f) << 0))
#define CMD_WETTING_TMR(i, mask)	CMD(0x07 + (i),	(mask))
#define CMD_TRI_STATE(i, mask)		CMD(0x09 + (i),	(mask))
#define CMD_CALIB					CMD(0x0b,		0x00)
#define CMD_SLEEP(int_t, scan_t)	CMD(0x0c,		(((int_t) & 0x7) << 3) | (((scan_t) & 0x7) << 0))
#define CMD_RST						CMD(0x7f,		0x00)

/* all switch to battery or ground pins mask */
#define SP_PINS_MASK				0x00ff
#define SP_PINS_EXTRACT(pins)		(((pins) >> 14) & SP_PINS_MASK)
/* all switch-to-ground inputs mask */
#define SG_PINS_MASK				0x3fff
#define SG_PINS_EXTRACT(pins)		(((pins) >>  0) & SG_PINS_MASK)

/* reply is allways same 24 status bits */
#define FLAG_THERM					(1 << 23)
#define FLAG_INT					(1 << 22)
/* from LSB to MSB: SG0..SG13, SP0..SP7 */
#define PIN_MASK(pin)				(BIT(pin))

/*==========================================================================*/
/* Driver exported variables.												*/
/*==========================================================================*/

/*==========================================================================*/
/* Driver local variables and types.										*/
/*==========================================================================*/

/* Driver */
struct mc33972_priv {
	const struct mc33972_config	*cfg;

	/* thread stuff */
	thread_t 					*thread;
	THD_WORKING_AREA(thread_wa, 256);
	semaphore_t					wake;

	/* last input state from chip */
	uint32_t					i_state;
	/* currently selected analog input */
	uint8_t						analog_ch;
	/* enabled inputs */
	uint32_t					en_pins;

	mc33972_drv_state			drv_state;
};

static mc33972_priv chips[BOARD_MC33972_COUNT];

/*==========================================================================*/
/* Driver local functions.													*/
/*==========================================================================*/

static SPIDriver *get_bus(mc33972_priv *chip)
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

static int mc33972_spi_w(mc33972_priv *chip, uint32_t tx)
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

static int mc33972_update_status(mc33972_priv *chip)
{
	int ret;

	/* TODO: lock? */

	ret = mc33972_spi_w(chip, CMD_STATUS);
	
	/* TODO: unlock? */

	return ret;
}

static int mc33972_update_pullups(mc33972_priv *chip)
{
	int ret;

	/* enable tri-state for all unused pins */
	ret = mc33972_spi_w(chip, CMD_TRI_STATE(SP_BANK, SP_PINS_EXTRACT(~chip->en_pins)));
	if (ret)
		return ret;
	ret = mc33972_spi_w(chip, CMD_TRI_STATE(SG_BANK, SG_PINS_EXTRACT(~chip->en_pins)));

	return ret;
}

static int mc33972_comm_test(mc33972_priv *chip)
{
	int ret;

	/* After an input has been selected as the analog,
	 * the corresponding bit in the next SO data stream
	 * will be logic [0] */

	/* go throught all inputs, read inputs status and
	 * check that muxed input bit is zero */
	for (int i = 0; i < MC33972_INPUTS; i++) {
		/* indexed starting from 1 */
		ret = mc33972_spi_w(chip, CMD_ANALOG(0, i + 1));
		if (ret)
			return ret;
		ret = mc33972_update_status(chip);
		if (ret)
			return ret;

		if (chip->i_state & PIN_MASK(i))
			return -1;
	}

	return 0;
}

/**
 * @brief MC33972 chip init.
 * @details There is no way to check communication.
 *  Performs reset.
 */

static int mc33972_chip_init(mc33972_priv *chip)
{
	int ret;

	/* reset first */
	ret = mc33972_spi_w(chip, CMD_RST);
	if (ret)
		return ret;

	/* is it enought? */
	chThdSleepMilliseconds(3);

	/*
	 * Default settings from Power-ON Reset via V PWR or the
	 * Reset Command are as follows:
	 * * Programmable switch - set to switch to battery
	 * * All inputs set as wake-up
	 * * Wetting current on (16 mA)
	 * * Wetting current timer on (20 ms)
	 * * All inputs tri-state
	 * * Analog select 00000 (no input channel selected)
	 */

	/* check communication */
	ret = mc33972_comm_test(chip);
	if (ret)
		return ret;

	/* disable tri-state for used pins only */
	ret = mc33972_update_pullups(chip);
	if (ret)
		return ret;

	/* Set wetting current to 2 mA */
	ret = mc33972_spi_w(chip, CMD_METALLIC(SP_BANK, 0));
	if (ret)
		return ret;
	ret = mc33972_spi_w(chip, CMD_METALLIC(SG_BANK, 0));
	if (ret)
		return ret;

	return 0;
}

/**
 * @brief MC33972 chip driver wakeup.
 * @details Wake up driver. Will cause input and diagnostic
 *  update
 */
static int mc33972_wake_driver(mc33972_priv *chip)
{
	/* Entering a reentrant critical zone.*/
	syssts_t sts = chSysGetStatusAndLockX();
	chSemSignalI(&chip->wake);
	if (!port_is_isr_context()) {
		/**
		 * chSemSignalI above requires rescheduling
		 * interrupt handlers have implicit rescheduling
		 */
		chSchRescheduleS();
	}
	/* Leaving the critical zone.*/
	chSysRestoreStatusX(sts);

	return 0;
}

/*==========================================================================*/
/* Driver thread.															*/
/*==========================================================================*/

static THD_FUNCTION(mc33972_driver_thread, p)
{
	int ret;
	mc33972_priv *chip = reinterpret_cast<mc33972_priv*>(p);

	chRegSetThreadName(DRIVER_NAME);

	/* repeat init until success */
	do {
		ret = mc33972_chip_init(chip);
		if (ret) {
			chThdSleepMilliseconds(1000);
			continue;
		}
		/* else */
		chip->drv_state = MC33972_READY;
	} while (chip->drv_state != MC33972_READY);

	while(1) {
		msg_t msg = chSemWaitTimeout(&chip->wake, TIME_MS2I(MC33972_POLL_INTERVAL_MS));

		if ((chip->cfg == NULL) ||
			(chip->drv_state == MC33972_DISABLED) ||
			(chip->drv_state == MC33972_FAILED))
			continue;

		if (msg == MSG_TIMEOUT) {
			/* only input state update */
			ret = mc33972_update_status(chip);
		} else {
			/* someone waked thread and asks us to update pin config */
			/* inputs state is also readed */
			ret = mc33972_update_pullups(chip);
		}

		if (ret) {
			/* set state to MC33972_FAILED? */
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

static int mc33972_setPadMode(void *data, unsigned int pin, iomode_t mode) {
	mc33972_priv *chip;

	if ((pin >= MC33972_INPUTS) || (data == NULL))
		return -1;

	chip = (mc33972_priv *)data;

	/* currently driver doesn't know how to hanlde different modes */
	(void)mode;

	/* NOTE: currently this driver supports only input mode...
	 * while chip can output by manipulating with pull-up and
	 * pull-down modes....
	 * if this function is called for pin, that means someone
	 * wants to read this pin and we can enable pull-up
	 * Also pull down is not supported yet */
	chip->en_pins |= PIN_MASK(pin);

	/* ask for reinit */
	mc33972_wake_driver(chip);

	return 0;
}

static int mc33972_readPad(void *data, unsigned int pin) {
	mc33972_priv *chip;

	if ((pin >= MC33972_INPUTS) || (data == NULL))
		return -1;

	chip = (mc33972_priv *)data;

	/* convert to some common enum? */
	return !!(chip->i_state & PIN_MASK(pin));
}

static brain_pin_diag_e mc33972_getDiag(void *data, unsigned int pin) {
	brain_pin_diag_e diag = PIN_OK;
	mc33972_priv *chip;

	if ((pin >= MC33972_INPUTS) || (data == NULL))
		return PIN_INVALID;

	chip = (mc33972_priv *)data;

	/* one diag bit for all pins */
	if (chip->i_state & FLAG_THERM)
		diag = PIN_DRIVER_OVERTEMP;

	return diag;
}

static int mc33972_init(void * data)
{
	mc33972_priv *chip;

	chip = (mc33972_priv *)data;

	/* no pins enabled yet */
	chip->en_pins = 0x0000;

	/* init semaphore */
	chSemObjectInit(&chip->wake, 0);

	/* start thread */
	chip->thread = chThdCreateStatic(chip->thread_wa, sizeof(chip->thread_wa),
									 PRIO_GPIOCHIP, mc33972_driver_thread, chip);

	return 0;
}

static int mc33972_deinit(void *data)
{
	mc33972_priv *chip;

	chip = (mc33972_priv *)data;

	/* TODO: disable pulls for all pins? */

	/* stop thread */
	chThdTerminate(chip->thread);

	return 0;
}

struct gpiochip_ops mc33972_ops = {
	.setPadMode	= mc33972_setPadMode,
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

int mc33972_add(brain_pin_e base, unsigned int index, const struct mc33972_config *cfg)
{
	mc33972_priv *chip;

	/* no config or no such chip */
	if ((!cfg) || (!cfg->spi_bus) || (index >= BOARD_MC33972_COUNT))
		return -1;

	/* check for valid cs.
	 * DOTO: remove this check? CS can be driven by SPI */
	if (!cfg->spi_config.ssport) {
		return -1;
	}

	chip = &chips[index];

	/* already initted? */
	if (chip->cfg != NULL)
		return -1;

	chip->cfg = cfg;
	chip->i_state = 0;
	chip->drv_state = MC33972_WAIT_INIT;

	/* register, return gpio chip base */
	return gpiochip_register(base, DRIVER_NAME, &mc33972_ops, MC33972_INPUTS, chip);
}

#else /* BOARD_MC33972_COUNT > 0 */

int mc33972_add(brain_pin_e base, unsigned int index, const struct mc33972_config *cfg)
{
	(void)base; (void)index; (void)cfg;

	return -1;
}

#endif /* BOARD_MC33972_COUNT */
