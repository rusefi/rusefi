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
struct Mc33972 : public GpioChip {
	int init() override;
	int deinit() override;

	// These functions need not be implemented if not supported by the particular chip.
	/* pin argument is pin number within gpio chip, not a global number */
	int setPadMode(size_t pin, iomode_t mode) override;
	int readPad(size_t pin) override;
	brain_pin_diag_e getDiag(size_t pin) override;

	// internal functions

	int chip_init();
	int update_pullups();
	int update_status();

	void wake_driver();

	int spi_w(uint32_t tx);

	int comm_test();



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

static Mc33972 chips[BOARD_MC33972_COUNT];

/*==========================================================================*/
/* Driver local functions.													*/
/*==========================================================================*/

/**
 * @brief MC33972 send cmd routine.
 * @details Sends 24 bits of data. CS asserted before and released
 *  after transaction. Chip allways reply with input state + 2 bits
 *  of diagnostic. This routine save it to chip->i_state
 */

int Mc33972::spi_w(uint32_t tx)
{
	int i;
	uint8_t rxb[3];
	uint8_t txb[3];
	SPIDriver *spi = cfg->spi_bus;

	txb[0] = (tx >> 16) & 0xff;
	txb[1] = (tx >>  8) & 0xff;
	txb[2] = (tx >>  0) & 0xff;
	/* Acquire ownership of the bus. */
	spiAcquireBus(spi);
	/* Setup transfer parameters. */
	spiStart(spi, &cfg->spi_config);
	/* Slave Select assertion. */
	spiSelect(spi);
	/* Atomic transfer operations. */
	for (i = 0; i < 3; i++)
		rxb[i] = spiPolledExchange(spi, txb[i]);
	/* Slave Select de-assertion. */
	spiUnselect(spi);
	/* Ownership release. */
	spiReleaseBus(spi);

	/* save received data */
	i_state = (rxb[0] << 16) | (rxb[1] << 8) | (rxb[2] << 0);

	/* no errors for now */
	return 0;
}

/**
 * @brief MC33972 update status
 * @details Chip reply with input data and two bits of diag
 */

int Mc33972::update_status()
{
	return spi_w(CMD_STATUS);
}

int Mc33972::update_pullups()
{
	int ret;

	/* enable tri-state for all unused pins */
	ret = spi_w(CMD_TRI_STATE(SP_BANK, SP_PINS_EXTRACT(~en_pins)));
	if (ret)
		return ret;
	ret = spi_w(CMD_TRI_STATE(SG_BANK, SG_PINS_EXTRACT(~en_pins)));

	return ret;
}

int Mc33972::comm_test()
{
	int ret;

	/* After an input has been selected as the analog,
	 * the corresponding bit in the next SO data stream
	 * will be logic [0] */

	/* go throught all inputs, read inputs status and
	 * check that muxed input bit is zero */
	for (int i = 0; i < MC33972_INPUTS; i++) {
		/* indexed starting from 1 */
		ret = spi_w(CMD_ANALOG(0, i + 1));
		if (ret)
			return ret;
		ret = update_status();
		if (ret)
			return ret;

		if (i_state & PIN_MASK(i))
			return -1;
	}

	return 0;
}

/**
 * @brief MC33972 chip init.
 * @details There is no way to check communication.
 *  Performs reset.
 */

int Mc33972::chip_init()
{
	int ret;

	/* reset first */
	ret = spi_w(CMD_RST);
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
	ret = comm_test();
	if (ret)
		return ret;

	/* disable tri-state for used pins only */
	ret = update_pullups();
	if (ret)
		return ret;

	/* Set wetting current to 2 mA */
	ret = spi_w(CMD_METALLIC(SP_BANK, 0));
	if (ret)
		return ret;
	ret = spi_w(CMD_METALLIC(SG_BANK, 0));
	if (ret)
		return ret;

	return 0;
}

/**
 * @brief MC33972 chip driver wakeup.
 * @details Wake up driver. Will cause input and diagnostic
 *  update
 */
void Mc33972::wake_driver()
{
	/* Entering a reentrant critical zone */
	chibios_rt::CriticalSectionLocker csl;

	chSemSignalI(&wake);
	if (!port_is_isr_context()) {
		/**
		 * chSemSignalI above requires rescheduling
		 * interrupt handlers have implicit rescheduling
		 */
		chSchRescheduleS();
	}
}

/*==========================================================================*/
/* Driver thread.															*/
/*==========================================================================*/

static THD_FUNCTION(mc33972_driver_thread, p)
{
	int ret;
	Mc33972 *chip = reinterpret_cast<Mc33972*>(p);

	chRegSetThreadName(DRIVER_NAME);

	/* repeat init until success */
	do {
		ret = chip->chip_init();
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
			ret = chip->update_status();
		} else {
			/* someone waked thread and asks us to update pin config */
			/* inputs state is also read */
			ret = chip->update_pullups();
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

int Mc33972::setPadMode(size_t pin, iomode_t mode) {
	if (pin >= MC33972_INPUTS)
		return -1;

	/* currently driver doesn't know how to hanlde different modes */
	(void)mode;

	/* NOTE: currently this driver supports only input mode...
	 * while chip can output by manipulating with pull-up and
	 * pull-down modes....
	 * if this function is called for pin, that means someone
	 * wants to read this pin and we can enable pull-up
	 * Also pull down is not supported yet */
	en_pins |= PIN_MASK(pin);

	/* ask for reinit */
	wake_driver();

	return 0;
}

int Mc33972::readPad(size_t pin) {
	if (pin >= MC33972_INPUTS)
		return -1;

	/* convert to some common enum? */
	return !!(i_state & PIN_MASK(pin));
}

brain_pin_diag_e Mc33972::getDiag(size_t pin) {
	brain_pin_diag_e diag = PIN_OK;

	if (pin >= MC33972_INPUTS)
		return PIN_INVALID;

	/* one diag bit for all pins */
	if (i_state & FLAG_THERM)
		diag = PIN_DRIVER_OVERTEMP;

	return diag;
}

int Mc33972::init()
{
	/* no pins enabled yet */
	en_pins = 0x0000;

	/* init semaphore */
	chSemObjectInit(&wake, 0);

	/* start thread */
	thread = chThdCreateStatic(thread_wa, sizeof(thread_wa),
									 PRIO_GPIOCHIP, mc33972_driver_thread, this);

	return 0;
}

int Mc33972::deinit()
{
	/* TODO: disable pulls for all pins? */

	/* stop thread */
	chThdTerminate(thread);

	return 0;
}

/**
 * @brief MC33972 driver add.
 * @details Checks for valid config
 */

int mc33972_add(brain_pin_e base, unsigned int index, const struct mc33972_config *cfg)
{
	/* no config or no such chip */
	if ((!cfg) || (!cfg->spi_bus) || (index >= BOARD_MC33972_COUNT))
		return -1;

	/* check for valid cs.
	 * DOTO: remove this check? CS can be driven by SPI */
	if (!cfg->spi_config.ssport) {
		return -1;
	}

	Mc33972& chip = chips[index];

	/* already initted? */
	if (chip.cfg != NULL)
		return -1;

	chip.cfg = cfg;
	chip.i_state = 0;
	chip.drv_state = MC33972_WAIT_INIT;

	/* register, return gpio chip base */
	return gpiochip_register(base, DRIVER_NAME, chip, MC33972_INPUTS);
}

#else /* BOARD_MC33972_COUNT > 0 */

int mc33972_add(brain_pin_e base, unsigned int index, const struct mc33972_config *cfg)
{
	(void)base; (void)index; (void)cfg;

	return -1;
}

#endif /* BOARD_MC33972_COUNT */
