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

#include "pch.h"

#include "gpio/gpio_ext.h"
#include "gpio/drv8860.h"
#include "os_util.h"

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
struct Drv8860 : public GpioChip {
	int init() override;

	int writePad(size_t pin, int value) override;
	brain_pin_diag_e getDiag(size_t pin) override;

	// Internal helpers
	int chip_init();

	void spi_send(uint16_t tx);

	void update_outputs();
	int wake_driver();

	const drv8860_config		*cfg;
	/* cached output state - state last send to chip */
	uint16_t					o_state_cached;
	/* state to be sended to chip */
	uint16_t					o_state;

	drv8860_drv_state			drv_state;
};

static Drv8860 chips[BOARD_DRV8860_COUNT];

static const char* drv8860_pin_names[DRV8860_OUTPUTS] = {
	"drv8860.OUT1",		"drv8860.OUT2",		"drv8860.OUT3",		"drv8860.OUT4",
	"drv8860.OUT5",		"drv8860.OUT6",		"drv8860.OUT7",		"drv8860.OUT8",
	"drv8860.OUT9",		"drv8860.OUT10",	"drv8860.OUT11",	"drv8860.OUT12",
	"drv8860.OUT13",	"drv8860.OUT14",	"drv8860.OUT15",	"drv8860.OUT16",
};

/*==========================================================================*/
/* Driver local functions.													*/
/*==========================================================================*/

/**
 * @brief DRV8860 send routine.
 * @details Sends 8/16 bits. CS asserted before and released after transaction.
 */

void Drv8860::spi_send(uint16_t tx) {
	SPIDriver *spi = cfg->spi_bus;
	
	/* Acquire ownership of the bus. */
	spiAcquireBus(spi);
	/* Setup transfer parameters. */
	spiStart(spi, &cfg->spi_config);
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

void Drv8860::update_outputs() {
	/* TODO: lock? */

	/* atomic */
	/* set value only for non-direct driven pins */
	spi_send(o_state & 0xffff);

	/* atomic */
	o_state_cached = o_state;

	/* TODO: unlock? */
}

/**
 * @brief DRV8860 chip init.
 * @details Marks all used pins.
 * @todo: Checks direct io signals integrity, read initial diagnostic state.
 */

int Drv8860::chip_init() {
	/* upload pin states */
	update_outputs();

	return 0;
}

/**
 * @brief DRV8860 chip driver wakeup.
 * @details Wake up driver. Will cause output register update.
 */

int Drv8860::wake_driver() {
	/* Entering a reentrant critical zone.*/
	chibios_rt::CriticalSectionLocker csl;

	chSemSignalI(&drv8860_wake);

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
			auto chip = &chips[i];
			if ((chip->cfg == NULL) ||
				(chip->drv_state == DRV8860_DISABLED) ||
				(chip->drv_state == DRV8860_FAILED))
				continue;

			chip->update_outputs();
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

int Drv8860::writePad(size_t pin, int value) {
	if (pin >= DRV8860_OUTPUTS)
		return -1;

	/* TODO: lock */
	if (value)
		o_state |=  (1 << pin);
	else
		o_state &= ~(1 << pin);
	/* TODO: unlock */
	wake_driver();
	
	return 0;
}

brain_pin_diag_e Drv8860::getDiag(size_t /*pin*/) {
	// todo: implement diag
	return PIN_OK;
}

int Drv8860::init() {
	int ret;

	ret = chip_init();
	if (ret)
		return ret;

	drv_state = DRV8860_READY;

	if (!drv_task_ready) {
		chThdCreateStatic(drv8860_thread_1_wa, sizeof(drv8860_thread_1_wa),
						  PRIO_GPIOCHIP, drv8860_driver_thread, NULL);
		drv_task_ready = true;
	}

	return 0;
}

/**
 * @brief DRV8860 driver add.
 * @details Checks for valid config
 */

int drv8860_add(brain_pin_e base, unsigned int index, const drv8860_config *cfg) {
	int ret;

	/* no config or no such chip */
	if ((!cfg) || (!cfg->spi_bus) || (index >= BOARD_DRV8860_COUNT))
		return -1;

	/* check for valid cs.
	 * TODO: remove this check? CS can be driven by SPI */
	//if (cfg->spi_config.ssport == NULL)
	//	return -1;

	auto& chip = chips[index];

	/* already initted? */
	if (!chip.cfg)
		return -1;

	chip.cfg = cfg;
	chip.o_state = 0;
	chip.o_state_cached = 0;
	chip.drv_state = DRV8860_WAIT_INIT;

	/* register, return gpio chip base */
	ret = gpiochip_register(base, DRIVER_NAME, chip, DRV8860_OUTPUTS);
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
