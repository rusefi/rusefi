/*
 * l9779.cpp
 *
 *  Created on: Jan 10, 2022
 *
 * Andrey Gusakov, (c) 2022
 *
 * Masks/inputs bits:
 * 0..3   - IGN1 .. 4 - Ignition pre-drivers
 * 						Driven by logical-AND of SPI control bit and dedicated parallel input IGNI1...IGNI4
 * 4..7   - OUT1 .. 4 - Protected low-side drivers with max current 2.2A
 *						Driven by logical-AND of SPI control bit and dedicated parallel input IN1...IN4
 * 8      - OUT5      - Protected low-side driver with max current 3A
 *						Driven by logical-AND of SPI control bit and dedicated parallel input IN5
 * 9..10  - OUT6,7    - Protected low-side drivers with max current 5A (O2 heaters)
 *						Driven by logical-AND of SPI control bit and dedicated parallel input IN6, IN7.
 * 11                 - Unused (no OUT8), IN8-PWM is used for stepper
 * 12..15 - OUT9..12  - Not exist on L9779WD-SPI, TODO: check L9779WD
 * 16..17 - OUT13..14 - Protected low side relay drivers with max current 600 mA and Low Battery Volatage function
 * 18..21 - OUT15..18 - Protected low side relay drivers with max current 600 mA
 * 22                 - Unused (no OUT19)
 * 23     - OUT20     - Protected low side low current driver with max current 50 mA
 * 24..27 - OUTA..D   - Configurable outputs (OD, PP) with max current 0.6 A (for low and high side FETs)
 *						Can be configured for stepper motor driving.
 *						Stepper is controlled by the logic AND between PWM (IN8) input pin and PWM SPI bit.
 * 28..31 - OUT25..27 - Unused on L9779WD-SPI, TODO for L9779WD
 * 32     - MR        - Main Relay low side driver with max current 0.6 A, automaticly controlled
 */

#include "pch.h"

#include "gpio/l9779.h"

#if (BOARD_L9779_COUNT > 0)

#include "persistent_configuration.h"
#include "hardware.h"
#include "gpio/gpio_ext.h"
/*
 * TODO list:
 *  - just write code
 */
/*==========================================================================*/
/* Driver local definitions.												*/
/*==========================================================================*/
#define DRIVER_NAME					"l9779"

#define DIAG_PERIOD_MS				(7)

typedef enum {
	L9779_DISABLED = 0,
	L9779_WAIT_INIT,
	L9779_READY,
	L9779_FAILED
} l9779_drv_state;

/* SPI communication helpers */
/* Out frame */
/* D0 - parity */
/* D8:D1 - DATA OUT or SUBADDRESS if ADD[4:0] = 0x10 (for read) */
#define MSG_SET_DATA(d)				(((d) & 0xff) <<  1)
/* sub-address is 5 bit */
#define MSG_SET_SUBADDR(s)			(((s) & 0x1f) <<  1)
/* D9 - x */
/* D14:D10 - ADDRESS */
#define MSG_SET_ADDR(a)				(((a) & 0x1f) << 10)
/* D15 - x */

/* ADD user for read commands */
#define MSG_READ_ADDR				(0x10)

#define MSG_W(a, d)					(static_cast<uint16_t>((MSG_SET_ADDR(a) | MSG_SET_DATA(d))))
#define MSG_R(a)					(static_cast<uint16_t>((MSG_SET_ADDR(MSG_READ_ADDR) | MSG_SET_SUBADDR(d))))

/* Both DIN and DO */
/* D0 - parity */
#define MSG_GET_PARITY(x)			(((x) >>  0) & 0x01)
/* D14:D10 - Addr of DATA IN or DATA OUT */
#define MSG_GET_ADDR(x)				(((x) >> 10) & 0x1f)
/* D8:D1 - DATA IN */
#define MSG_GET_DATA(x)				(((x) >>  1) & 0xff)

/* DIN / to chip */
/* D8:D1 or 5 bits of subaddr in case of read access */
#define MSG_GET_SUBADDR(tx)			(MSG_GET_DATA(tx) & 0x1f)

/* DOUT / from chip */
/* D 9 - W/R flag, 1 if we read */
#define MSG_GET_WR(rx)				(((rx) >>  9) & 0x01)
/* D15 - SPI error flag */
#define MSG_GET_SPIERROR(rx)		(((rx) >> 15) & 0x01)

/* register address that never can be replyed */
#define REG_INVALID					0xff

/* Write only registers */
#define CMD_CLOCK_UNLOCK_SW_RST(d)	MSG_W(0x0c, (d))
#define CMD_START_REACT(d)			MSG_W(0x0d, (d))
#define CMD_CONTR_REG(n, d)			MSG_W(0x08 + (n), (d))

/* Read only registers */

/* IGN1..4 + OUT1..7 */
#define OUT_DIRECT_DRIVE_MASK		0x7ff

/*==========================================================================*/
/* Driver exported variables.												*/
/*==========================================================================*/

/*==========================================================================*/
/* Driver local variables and types.										*/
/*==========================================================================*/

/* Driver private data */
struct L9779 : public GpioChip {
	int init() override;
	int deinit() override;

	int setPadMode(size_t pin, iomode_t mode) override;
	int writePad(size_t pin, int value) override;
	int readPad(size_t pin) override;
	brain_pin_diag_e getDiag(size_t pin) override;

	bool spi_parity_odd(uint16_t x);
	int spi_validate(uint16_t rx);
	int spi_rw(uint16_t tx, uint16_t *rx_ptr);
	int spi_rw_array(const uint16_t *tx, uint16_t *rx, int n);

	int update_output();
	int update_direct_output(size_t pin, int value);
	int wake_driver();

	int chip_reset();
	int chip_init_data();
	int chip_init();

	brain_pin_diag_e getOutputDiag(size_t pin);
	brain_pin_diag_e getInputDiag(size_t pin);

	const l9779_config	*cfg;

	/* thread stuff */
	thread_t 					*thread;
	THD_WORKING_AREA(thread_wa, 256);
	semaphore_t					wake;

	/* state to be sent to chip */
	uint32_t					o_state;
	/* output enabled mask */
	uint32_t					o_oe_mask;
	/* cached output registers state - value last send to chip */
	uint32_t					o_data_cached;

	l9779_drv_state				drv_state;

	/* last accesed register */
	uint8_t						last_addr;
	/* last requested subaddr in case of read */
	uint8_t						last_subaddr;

	/* chip needs reintialization due to some critical issue */
	bool						need_init;

	/* statistic */
	//int						por_cnt;
	//int						wdr_cnt;
	//int						comfe_cnt;
	int							init_cnt;
	//int						init_req_cnt;
	int							spi_cnt;
	int							spi_err_parity;		/* parity errors in rx data */
	int							spi_err_frame;		/* rx messages with bit 15 set */
	int							spi_err;			/* rx messages with incorrect ADDR or WR fields */
	uint16_t					tx;
	uint16_t					rx;
};

static L9779 chips[BOARD_L9779_COUNT];

static const char* l9779_pin_names[L9779_SIGNALS] = {
	"L9779.IGN1",	"L9779.IGN2",	"L9779.IGN3",	"L9779.IGN4",
	"L9779.OUT1",	"L9779.OUT2",	"L9779.OUT3",	"L9779.OUT4",
	"L9779.OUT5",	"L9779.OUT6",	"L9779.OUT7",	"L9779.OUT8",
	"L9779.OUT9",	"L9779.OUT10",	"L9779.OUT11",	"L9779.OUT12",
	"L9779.OUT13",	"L9779.OUT14",	"L9779.OUT15",	"L9779.OUT16",
	"L9779.OUT17",	"L9779.OUT18",	"L9779.OUT19",	"L9779.OUT20",
	"L9779.OUTA",	"L9779.OUTB",	"L9779.OUTC",	"L9779.OUTD",
	"L9779.OUT25",	"L9779.OUT26",	"L9779.OUT27",	"L9779.OUT28",
	"L9779.MRD",	"L9779.KEY"
};

/*==========================================================================*/
/* Driver local functions.													*/
/*==========================================================================*/

/* true if parity of input x is odd */
bool L9779::spi_parity_odd(uint16_t x)
{
	x ^= x >> 8;
	x ^= x >> 4;
	x ^= x >> 2;
	x ^= x >> 1;

	return (x & 1);
}

int L9779::spi_validate(uint16_t rx)
{
	if (!spi_parity_odd(rx)) {
		spi_err_parity++;
		return -1;
	}

	if (MSG_GET_SPIERROR(rx)) {
		/* not clear what does this means */
		spi_err_frame++;
		return -1;
	}

	/* check that correct register is returned */
	if (last_subaddr != REG_INVALID) {
		/* MISO DO returns 1 at D9 bit and 5bit sub address in
		 * ADD[4:0] field */
		if (!MSG_GET_WR(rx)) {
			return -2;
		}
		if (MSG_GET_ADDR(rx) != last_subaddr) {
			/* unexpected SPI answer */
			spi_err++;

			/* should ve restart? */
			//need_init = true;

			return -1;
		}
	}

	/* LOCK_UNLOCK_SW_RST */
	if (last_addr == 0x0c) {
		/* BIT(0) = LOCK flag */
	/* START_REACT */
	} else if (last_addr == 0x0d) {
		/* BIT(0) = OUT_DIS */
	}

	return 0;
}

/**
 * @returns -1 in case of communication error
 */
int L9779::spi_rw(uint16_t tx, uint16_t *rx_ptr)
{
	int ret;
	uint16_t rx;
	SPIDriver *spi = cfg->spi_bus;

	/* set parity */
	tx |= !spi_parity_odd(tx);

	/* Acquire ownership of the bus. */
	spiAcquireBus(spi);
	/* Setup transfer parameters. */
	spiStart(spi, &cfg->spi_config);
	/* Slave Select assertion. */
	spiSelect(spi);
	/* Atomic transfer operations. */
	rx = spiPolledExchange(spi, tx);
	/* Slave Select de-assertion. */
	spiUnselect(spi);
	/* Ownership release. */
	spiReleaseBus(spi);

	/* statisctic and debug */
	this->tx = tx;
	this->rx = rx;
	this->spi_cnt++;

	if (rx_ptr)
		*rx_ptr = rx;

	/* validate reply */
	ret = spi_validate(rx);
	/* save last accessed register */
	last_addr = MSG_GET_ADDR(this->tx);
	if (last_addr == MSG_READ_ADDR)
		last_subaddr = MSG_GET_SUBADDR(this->tx);
	else
		last_subaddr = REG_INVALID;

	return ret;
}
/**
 * @return -1 in case of communication error
 */
int L9779::spi_rw_array(const uint16_t *tx, uint16_t *rx, int n)
{
	int ret;
	uint16_t rxdata;
	SPIDriver *spi = cfg->spi_bus;

	if (n <= 0) {
		return -2;
	}

	/* Acquire ownership of the bus. */
	spiAcquireBus(spi);
	/* Setup transfer parameters. */
	spiStart(spi, &cfg->spi_config);

	for (int i = 0; i < n; i++) {
		/* Slave Select assertion. */
		spiSelect(spi);
		/* data transfer */
		rxdata = spiPolledExchange(spi, tx[i]);

		if (rx)
			rx[i] = rxdata;
		/* Slave Select de-assertion. */
		spiUnselect(spi);

		/* statistic and debug */
		this->tx = tx[i];
		this->rx = rxdata;
		this->spi_cnt++;

		/* validate reply  */
		ret = spi_validate(rxdata);
		/* save last accessed register */
		last_addr = MSG_GET_ADDR(this->tx);
		if (last_addr == MSG_READ_ADDR)
			last_subaddr = MSG_GET_SUBADDR(this->tx);
		else
			last_subaddr = REG_INVALID;

		if (ret < 0)
			break;
	}
	/* Ownership release. */
	spiReleaseBus(spi);

	/* no errors for now */
	return ret;
}

/* use datasheet numbering, starting from 1, skip 4 ignition channels */
#define OUT_ENABLED(n)			(!!(o_state & BIT((n) + L9779_OUTPUTS_IGN - 1)))
#define SHIFT_N_OUT_TO_M(n, m)	(OUT_ENABLED(n) << (m))

/* use datasheet numbering, starting from 1 */
#define IGN_ENABLED(n)			(!!(o_state & BIT((n) - 1)))
#define SHIFT_N_IGN_TO_M(n, m)	(IGN_ENABLED(n) << (m))

int L9779::update_output()
{
	int ret;
	uint8_t regs[4];

	/* set value only for non-direct driven pins */
	uint32_t o_data = o_state & ~OUT_DIRECT_DRIVE_MASK;
	/* direct driven outputs are logicaly-AND spi bit and dedicated input
	 * set bits to all enabled direct driven outputs */
	o_data = o_state | (o_oe_mask & OUT_DIRECT_DRIVE_MASK);

	/* nightmare... briliant mapping */
	regs[0] =
		SHIFT_N_OUT_TO_M( 1, 7) |	/* bit 7 - OUT1 */
		SHIFT_N_OUT_TO_M( 2, 6) |	/* and so on, refer to datasheet */
		SHIFT_N_OUT_TO_M( 3, 5) |
		SHIFT_N_OUT_TO_M( 4, 4) |
		SHIFT_N_OUT_TO_M( 5, 3) |
		SHIFT_N_OUT_TO_M(20, 2);
	regs[1] =
		SHIFT_N_OUT_TO_M(15, 7) |
		SHIFT_N_OUT_TO_M(14, 6) |
		/* reserved + don't care */
		SHIFT_N_IGN_TO_M( 1, 3) |
		SHIFT_N_IGN_TO_M( 2, 2) |
		SHIFT_N_IGN_TO_M( 3, 1) |
		SHIFT_N_IGN_TO_M( 4, 0);
	regs[2] =
		SHIFT_N_OUT_TO_M(22, 7) |	/* TODO: stepper DIR */
		SHIFT_N_OUT_TO_M(21, 6) |	/* TODO: stepper enable */
		SHIFT_N_OUT_TO_M(16, 5) |
		SHIFT_N_OUT_TO_M(14, 4) |
		SHIFT_N_OUT_TO_M(17, 3) |
		SHIFT_N_OUT_TO_M(18, 2) |
		SHIFT_N_OUT_TO_M( 7, 1) |
		SHIFT_N_OUT_TO_M( 6, 0);
	regs[3] =
		SHIFT_N_OUT_TO_M(28, 5) |
		SHIFT_N_OUT_TO_M(27, 4) |
		SHIFT_N_OUT_TO_M(26, 3) |
		SHIFT_N_OUT_TO_M(25, 2) |
		SHIFT_N_OUT_TO_M(24, 1) |
		SHIFT_N_OUT_TO_M(23, 0);	/* TODO: stepper PWM */
	uint16_t tx[] = {
		/* output enables */
		CMD_CONTR_REG(0, regs[0]),
		CMD_CONTR_REG(1, regs[1]),
		CMD_CONTR_REG(2, regs[2]),
		CMD_CONTR_REG(3, regs[3])
	};
	ret = spi_rw_array(tx, NULL, ARRAY_SIZE(tx));

	if (ret == 0) {
		/* atomic */
		o_data_cached = o_data;
	}

	return ret;
}

int L9779::update_direct_output(size_t pin, int value)
{
	/* no direct-drive gpio is allocated for this output */
	if (cfg->direct_gpio[pin].port == NULL)
		return -1;

	if (value)
		palSetPort(cfg->direct_gpio[pin].port,
				   PAL_PORT_BIT(cfg->direct_gpio[pin].pad));
	else
		palClearPort(cfg->direct_gpio[pin].port,
				   PAL_PORT_BIT(cfg->direct_gpio[pin].pad));
	return 0;
}

/**
 * @brief L9779 chip driver wakeup.
 * @details Wake up driver. Will cause output register update
 */

int L9779::wake_driver()
{
	/* Entering a reentrant critical zone.*/
	chibios_rt::CriticalSectionLocker csl;
	chSemSignalI(&wake);
	if (!port_is_isr_context()) {
		/**
		 * chSemSignalI above requires rescheduling
		 * interrupt handlers have implicit rescheduling
		 */
		chSchRescheduleS();
	}

	return 0;
}

int L9779::chip_reset() {
	int ret;

	ret = spi_rw(CMD_CLOCK_UNLOCK_SW_RST(BIT(1)), NULL);
	/**
	 * ???
	 */
	chThdSleepMilliseconds(3);

	last_addr = REG_INVALID;
	last_subaddr = REG_INVALID;

	return ret;
}

/*==========================================================================*/
/* Driver thread.															*/
/*==========================================================================*/

static THD_FUNCTION(l9779_driver_thread, p) {
	L9779 *chip = reinterpret_cast<L9779*>(p);
	sysinterval_t poll_interval = 0;

	chRegSetThreadName(DRIVER_NAME);

	while (1) {
		int ret;
		msg_t msg = chSemWaitTimeout(&chip->wake, poll_interval);

		/* should we care about msg == MSG_TIMEOUT? */
		(void)msg;

		/* default polling interval */
		poll_interval = TIME_MS2I(DIAG_PERIOD_MS);

		if ((chip->cfg == NULL) ||
			(chip->drv_state == L9779_DISABLED) ||
			(chip->drv_state == L9779_FAILED))
			continue;

#if 0
		bool wd_happy = chip->wd_happy;

		/* update outputs only if WD is happy */
		if ((wd_happy) || (1)) {
			ret = chip->update_output();
			if (ret) {
				/* set state to L9779_FAILED? */
			}
		}

		ret = chip->wd_feed();
		if (ret < 0) {
			/* WD is not happy */
			continue;
		}
		/* happiness state has changed! */
		if ((chip->wd_happy != wd_happy) && (chip->wd_happy)) {
			chip->need_init = true;
		}
#endif

		if (chip->need_init) {
			/* clear first, as flag can be raised again during init */
			chip->need_init = false;
			/* re-init chip! */
			chip->chip_init();
			/* sync pins state */
			chip->update_output();
		}

		/* Chip is ready to rock? */
		if (chip->need_init == false) {
			/* Just update outputs state */
			ret = chip->update_output();
			if (ret) {
				/* set state to L9779_FAILED? */
			}
		}
#if 0
		if (chip->diag_ts <= chVTGetSystemTimeX()) {
			/* this is expensive call, will do a lot of spi transfers... */
			ret = chip->update_status_and_diag();
			if (ret) {
				/* set state to L9779_FAILED or force reinit? */
			} else {
				diagResponse.reset();
			}
			/* TODO:
			 * Procedure to switch on after failure condition occurred:
			 *  - Read out of diagnosis bits
			 *  - Second read out to verify that the failure conditions are not
			 *    remaining
			 *  - Set of the dedicated output enable bit of the affected channel
			 *    if the diagnosis bit is not active anymore
			 *  - Switch on of the channel */

			chip->diag_ts = chTimeAddX(chVTGetSystemTimeX(), TIME_MS2I(DIAG_PERIOD_MS));
		}
		poll_interval = chip->calc_sleep_interval();	
#endif
		/* default poll_interval */
	}
}

/*==========================================================================*/
/* Driver interrupt handlers.												*/
/*==========================================================================*/
/*==========================================================================*/
/* Driver exported functions.												*/
/*==========================================================================*/

int L9779::setPadMode(unsigned int pin, iomode_t mode) {
	if (pin >= L9779_SIGNALS)
		return -1;

	(void)mode;

	return 0;
}

int L9779::writePad(unsigned int pin, int value) {
	if (pin >= L9779_OUTPUTS)
		return -1;

	{
		chibios_rt::CriticalSectionLocker csl;

		if (value) {
			o_state |=  (1 << pin);
		} else {
			o_state &= ~(1 << pin);
		}
	}

	/* direct driven? */
	if (OUT_DIRECT_DRIVE_MASK & BIT(pin)) {
		return update_direct_output(pin, value);
	} else {
		return wake_driver();
	}

	return 0;
}

brain_pin_diag_e L9779::getOutputDiag(size_t pin)
{
	(void)pin;

	return PIN_OK;
}

brain_pin_diag_e L9779::getInputDiag(unsigned int pin)
{
	(void)pin;

	return PIN_OK;
}

int L9779::readPad(size_t pin) {
	if (pin >= L9779_SIGNALS)
		return -1;

	/* unknown pin */
	return -1;
}

brain_pin_diag_e L9779::getDiag(size_t pin)
{
	if (pin >= L9779_SIGNALS)
		return PIN_INVALID;

	if (pin < L9779_OUTPUTS)
		return getOutputDiag(pin);
	else
		return getInputDiag(pin);
}


int L9779::chip_init_data(void)
{
	int i;
	int ret = 0;

	o_oe_mask = 0;

	for (i = 0; i < L9779_DIRECT_OUTPUTS; i++) {
		if (cfg->direct_gpio[i].port == NULL)
			continue;

		/* configure source gpio */
		ret = gpio_pin_markUsed(cfg->direct_gpio[i].port, cfg->direct_gpio[i].pad, DRIVER_NAME " DIRECT IO");
		if (ret) {
			ret = -1;
			goto err_gpios;
		}
		palSetPadMode(cfg->direct_gpio[i].port, cfg->direct_gpio[i].pad, PAL_MODE_OUTPUT_PUSHPULL);
		palClearPort(cfg->direct_gpio[i].port, PAL_PORT_BIT(cfg->direct_gpio[i].pad));

		/* enable output */
		o_oe_mask |= BIT(i);
	}

	/* enable all spi-driven ouputs
	 * TODO: add API to enable/disable? */
	o_oe_mask |= ~OUT_DIRECT_DRIVE_MASK;

	return 0;

err_gpios:
	/* unmark pins */
	for (int i = 0; i < L9779_DIRECT_OUTPUTS; i++) {
		if (cfg->direct_gpio[i].port) {
			gpio_pin_markUnused(cfg->direct_gpio[i].port, cfg->direct_gpio[i].pad);
		}
	}

	return ret;
}

int L9779::chip_init()
{
	int ret;

	/* statistic */
	init_cnt++;

	/* Unlock, while unlocked by default. */
	ret = spi_rw(CMD_CLOCK_UNLOCK_SW_RST(0), NULL);
	if (ret)
		return ret;

	/* Enable power stages */
	ret = spi_rw(CMD_START_REACT(BIT(1)), NULL);
	if (ret)
		return ret;

	/* TODO: add spi communication test: read IDENT_REG */

	return ret;
}

int L9779::init()
{
	int ret;

	/* check for multiple init */
	if (drv_state != L9779_WAIT_INIT)
		return -1;

	ret = chip_reset();
	if (ret)
		return ret;

	ret = chip_init_data();
	if (ret)
		return ret;

	/* force chip init from driver thread */
	need_init = true;

	/* instance is ready */
	drv_state = L9779_READY;

	/* init semaphore */
	chSemObjectInit(&wake, 10);

	/* start thread */
	thread = chThdCreateStatic(thread_wa, sizeof(thread_wa),
									 PRIO_GPIOCHIP, l9779_driver_thread, this);

	return 0;
}

int L9779::deinit()
{
	return 0;
}

/**
 * @brief L9779 driver add.
 * @details Checks for valid config
 * @return return gpio chip base
 */

int l9779_add(brain_pin_e base, unsigned int index, const l9779_config *cfg) {

	efiAssert(OBD_PCM_Processor_Fault, cfg != NULL, "L9779CFG", 0)

	/* no config or no such chip */
	if ((!cfg) || (!cfg->spi_bus) || (index >= BOARD_L9779_COUNT))
		return -1;

	L9779* chip = &chips[index];

	/* already initted? */
	if (chip->cfg)
		return -1;

	/* config */
	chip->cfg = cfg;
	/* reset to defaults */


	/* register */
	int ret = gpiochip_register(base, DRIVER_NAME, *chip, L9779_SIGNALS);
	if (ret < 0)
		return ret;

	/* set default pin names, board init code can rewrite */
	gpiochips_setPinNames(base, l9779_pin_names);

	return ret;
}

#endif /* (BOARD_L9779_COUNT > 0) */
