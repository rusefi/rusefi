/*
 * drv8860.c
 *
 * DRV8860 Smart 8/16-Channel Low-Side Switch
 *
 * All channels are controlled via the serial interface (SPI).
 * <200 kHz SPI
 *
 * @date Apr 6, 2020
 *
 * @author andreika, (c) 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "global.h"
#include "gpio/gpio_ext.h"
#include "gpio/drv8860.h"
#include "pin_repository.h"
#include "os_util.h"
#include "thread_priority.h"

#if (BOARD_DRV8860_COUNT > 0)

/*==========================================================================*/
/* Driver local definitions.												*/
/*==========================================================================*/

#define DRIVER_NAME				"drv8860"

static bool drv_task_ready = false;

typedef enum {
	DRV8860_DISABLED = 0,
	DRV8860_WAIT_INIT,
	DRV8860_READY,
	DRV8860_FAILED
} drv8860_drv_state;

/*==========================================================================*/
/* Driver exported variables.												*/
/*==========================================================================*/

/*==========================================================================*/
/* Driver local variables and types.										*/
/*==========================================================================*/

/* OS */
SEMAPHORE_DECL(drv8860_wake, 10 /* or BOARD_DRV8860_COUNT ? */);
static THD_WORKING_AREA(drv8860_thread_1_wa, 256);

/* Driver */
struct drv8860_priv {
	const drv8860_config		*cfg;
	/* cached output state - state last send to chip */
	uint16_t					o_state_cached;
	/* state to be sended to chip */
	uint16_t					o_state;

	drv8860_drv_state			drv_state;
};

static drv8860_priv chips[BOARD_DRV8860_COUNT];

static const char* drv8860_pin_names[DRV8860_OUTPUTS] = {
	"drv8860.OUT1",		"drv8860.OUT2",		"drv8860.OUT3",		"drv8860.OUT4",
	"drv8860.OUT5",		"drv8860.OUT6",		"drv8860.OUT7",		"drv8860.OUT8",
	"drv8860.OUT9",		"drv8860.OUT10",	"drv8860.OUT11",	"drv8860.OUT12",
	"drv8860.OUT13",	"drv8860.OUT14",	"drv8860.OUT15",	"drv8860.OUT16",
};

/*==========================================================================*/
/* Driver local functions.													*/
/*==========================================================================*/

static SPIDriver *get_bus(drv8860_priv *chip) {
	/* return non-const SPIDriver* from const struct cfg */
	return chip->cfg->spi_bus;
}

/**
 * @brief DRV8860 send routine.
 * @details Sends 8/16 bits. CS asserted before and released after transaction.
 */

static void drv8860_spi_send(drv8860_priv *chip, uint16_t tx) {
	SPIDriver *spi = get_bus(chip);
	
	/* Acquire ownership of the bus. */
	spiAcquireBus(spi);
	/* Setup transfer parameters. */
	spiStart(spi, &chip->cfg->spi_config);
	/* Slave Select assertion. */
	spiSelect(spi);
	/* Atomic transfer operations. */
	spiPolledExchange(spi, tx);
	/* Slave Select de-assertion. */
	spiUnselect(spi);
	/* Ownership release. */
	spiReleaseBus(spi);
}

/**
 * @brief DRV8860 send output data.
 */

static void drv8860_update_outputs(drv8860_priv *chip) {
	/* TODO: lock? */

	/* atomic */
	/* set value only for non-direct driven pins */
	drv8860_spi_send(chip, chip->o_state & 0xffff);

	/* atomic */
	chip->o_state_cached = chip->o_state;
	
	/* TODO: unlock? */
}

/**
 * @brief DRV8860 chip init.
 * @details Marks all used pins.
 * @todo: Checks direct io signals integrity, read initial diagnostic state.
 */

static int drv8860_chip_init(drv8860_priv *chip) {
	/* upload pin states */
	drv8860_update_outputs(chip);

	return 0;
}

/**
 * @brief DRV8860 chip driver wakeup.
 * @details Wake up driver. Will cause output register update.
 */

static int drv8860_wake_driver(drv8860_priv *chip) {
	(void)chip;

    /* Entering a reentrant critical zone.*/
    syssts_t sts = chSysGetStatusAndLockX();
	chSemSignalI(&drv8860_wake);
    /* Leaving the critical zone.*/
    chSysRestoreStatusX(sts);

	return 0;
}

/*==========================================================================*/
/* Driver thread.															*/
/*==========================================================================*/

static THD_FUNCTION(drv8860_driver_thread, p) {
	int i;
	msg_t msg;

	(void)p;

	chRegSetThreadName(DRIVER_NAME);

	while (1) {
		msg = chSemWaitTimeout(&drv8860_wake, TIME_MS2I(DRV8860_POLL_INTERVAL_MS));

		/* should we care about msg == MSG_TIMEOUT? */
		(void)msg;

		for (i = 0; i < BOARD_DRV8860_COUNT; i++) {
			drv8860_priv *chip;

			chip = &chips[i];
			if ((chip->cfg == NULL) ||
				(chip->drv_state == DRV8860_DISABLED) ||
				(chip->drv_state == DRV8860_FAILED))
				continue;

			drv8860_update_outputs(chip);
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

int drv8860_writePad(void *data, unsigned int pin, int value) {
	drv8860_priv *chip;

	if ((pin >= DRV8860_OUTPUTS) || (data == NULL))
		return -1;

	chip = (drv8860_priv *)data;

	/* TODO: lock */
	if (value)
		chip->o_state |=  (1 << pin);
	else
		chip->o_state &= ~(1 << pin);
	/* TODO: unlock */
	drv8860_wake_driver(chip);
	
	return 0;
}

brain_pin_diag_e drv8860_getDiag(void* /*data*/, unsigned int /*pin*/) {
	// todo: implement diag
	return PIN_OK;
}

int drv8860_init(void * data) {
	int ret;
	drv8860_priv *chip;

	chip = (drv8860_priv *)data;

	ret = drv8860_chip_init(chip);
	if (ret)
		return ret;

	chip->drv_state = DRV8860_READY;

	if (!drv_task_ready) {
		chThdCreateStatic(drv8860_thread_1_wa, sizeof(drv8860_thread_1_wa),
						  PRIO_GPIOCHIP, drv8860_driver_thread, NULL);
		drv_task_ready = true;
	}

	return 0;
}

int drv8860_deinit(void *data) {
	(void)data;

	/* TODO: set all pins to inactive state, stop task? */
	return 0;
}

struct gpiochip_ops drv8860_ops = {
	.setPadMode = nullptr,
	.writePad	= drv8860_writePad,
	.readPad	= NULL,	/* chip outputs only */
	.getDiag	= drv8860_getDiag,
	.init		= drv8860_init,
	.deinit 	= drv8860_deinit,
};

/**
 * @brief DRV8860 driver add.
 * @details Checks for valid config
 */

int drv8860_add(brain_pin_e base, unsigned int index, const drv8860_config *cfg) {
	int ret;
	drv8860_priv *chip;

	/* no config or no such chip */
	if ((!cfg) || (!cfg->spi_bus) || (index >= BOARD_DRV8860_COUNT))
		return -1;

	/* check for valid cs.
	 * TODO: remove this check? CS can be driven by SPI */
	//if (cfg->spi_config.ssport == NULL)
	//	return -1;

	chip = &chips[index];

	/* already initted? */
	if (chip->cfg != NULL)
		return -1;

	chip->cfg = cfg;
	chip->o_state = 0;
	chip->o_state_cached = 0;
	chip->drv_state = DRV8860_WAIT_INIT;

	/* register, return gpio chip base */
	ret = gpiochip_register(base, DRIVER_NAME, &drv8860_ops, DRV8860_OUTPUTS, chip);
	if (ret < 0)
		return ret;

	/* set default pin names, board init code can rewrite */
	gpiochips_setPinNames(static_cast<brain_pin_e>(ret), drv8860_pin_names);

	return ret;
}

#else /* BOARD_DRV8860_COUNT > 0 */

int drv8860_add(brain_pin_e base, unsigned int index, const drv8860_config *cfg) {
	(void)base; (void)index; (void)cfg;

	return -1;
}

#endif /* BOARD_DRV8860_COUNT */
