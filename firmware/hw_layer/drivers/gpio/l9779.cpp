/*
 * l9779.cpp
 *
 *  Created on: Jan 10, 2022
 *
 * Andrey Gusakov, (c) 2022
 *
 * TODO(at32-wip picking order - keep each item an atomic commit):
 * [x] SPI framing: CS timing and content-addressed delayed reply matching.
 * [x] SPI observability: bounded IDENT reads, timing, and recent-frame diagnostics.
 * [x] Output mapping: correct register packing and permanent direct-drive enables.
 * [ ] Power-stage diagnostics: DIA cache, per-pin status, and OUT_DIS recovery.
 * [ ] VRS configuration: stock full-adaptive setup and reset reconfiguration.
 * [ ] VDA 2.0 watchdog: challenge/response feed, timer, counters, and recovery.
 * [ ] Ignition-gated power-stage lifecycle: PSOFF, wake, and board integration.
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
#include "gpio/l9779_spi.h"

#if EFI_PROD_CODE && (BOARD_L9779_COUNT > 0)

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

/* L9779WD-SPI timing requirements (datasheet table 53):
 *  - tlead >= 525 ns: CS low to first SCK edge
 *  - tcsn  >= 640 ns: CS high between frames
 * Two microseconds provides margin without materially affecting throughput. */
#define L9779_TLEAD_DELAY_US		(2)
#define L9779_TCSN_DELAY_US			(2)

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

/* Read only registers (common address 0x10 plus a 5-bit sub-address in the
 * MOSI data field; the reply carries the sub-address in its address field). */
#define L9779_IDENT_SUB				(0x00)
#define L9779_IDENT					(MSG_SET_ADDR(MSG_READ_ADDR) | MSG_SET_SUBADDR(L9779_IDENT_SUB))

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
	void debug() override;

	bool spi_parity_odd(uint16_t x);
	int spi_validate(uint16_t rx);
	int spi_rw(uint16_t tx, uint16_t *rx_ptr);
	int spi_rw_array(const uint16_t *tx, uint16_t *rx, int n);

	int update_output();
	int update_direct_output(size_t pin, int value);
	int wake_driver();
	void logSpiFrame(uint16_t tx, uint16_t rx, int result);

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

	/* Read replies are delayed and can arrive after intervening frames.
	 * Match them by the sub-address returned by the chip, not by position. */
	L9779ReadTracker				read_requests;
	/* Sub-address answered by the most recently validated frame. */
	uint8_t						rx_subaddress = REG_INVALID;


	/* statistic */
	//int						por_cnt;
	//int						wdr_cnt;
	//int						comfe_cnt;
	//int						init_req_cnt;
	int							spi_cnt;
	int							spi_err_parity;		/* parity errors in rx data */
	int							spi_err_frame;		/* rx messages with bit 15 set */
	int							spi_err;			/* rx messages with incorrect ADDR or WR fields */
	uint16_t					recentTx;
	uint16_t					recentRx;
	uint32_t					recent_frame_ticks;
	uint16_t					ident_reg;
	L9779SpiFrameLog				frame_log;
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

static void l9779DelayUs(uint32_t microseconds)
{
	chSysPolledDelayX(US2RTC(SystemCoreClock, microseconds));
}

/* true if parity of input x is odd */
bool L9779::spi_parity_odd(uint16_t x)
{
	return l9779HasOddParity(x);
}

void L9779::logSpiFrame(uint16_t tx, uint16_t rx, int result)
{
	frame_log.record(tx, rx, rx_subaddress, result);
}

int L9779::spi_validate(uint16_t rx)
{
	rx_subaddress = REG_INVALID;

	if (!spi_parity_odd(rx)) {
		spi_err_parity++;
		return -1;
	}

	if (MSG_GET_SPIERROR(rx)) {
		/* not clear what does this means */
		spi_err_frame++;
		return -1;
	}

	/* A write/status frame does not answer an outstanding read. */
	if (!MSG_GET_WR(rx)) {
		return 0;
	}

	/* Read replies carry their own sub-address. Content-addressed matching
	 * lets the stream recover after a delayed or skipped response instead of
	 * permanently shifting every subsequent reply by one request. */
	const uint8_t reply_subaddress = MSG_GET_ADDR(rx);
	if (!read_requests.consume(reply_subaddress)) {
		spi_err++;
	} else {
		rx_subaddress = reply_subaddress;
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
	tx = l9779PrepareSpiWord(tx);

	/* Acquire ownership of the bus. */
	spiAcquireBus(spi);
	/* Setup transfer parameters. */
	spiStart(spi, &cfg->spi_config);
	/* Slave Select assertion. */
	spiSelect(spi);
	/* Meet tlead: CS low to first SCK edge. */
	l9779DelayUs(L9779_TLEAD_DELAY_US);
	/* Atomic transfer operations. */
	const rtcnt_t frame_start = chSysGetRealtimeCounterX();
	rx = spiPolledExchange(spi, tx);
	recent_frame_ticks = chSysGetRealtimeCounterX() - frame_start;
	/* Slave Select de-assertion. */
	spiUnselect(spi);
	/* Meet tcsn before another frame can assert CS. */
	l9779DelayUs(L9779_TCSN_DELAY_US);
	/* Ownership release. */
	spiReleaseBus(spi);

	/* statistics and debug */
	recentTx = tx;
	recentRx = rx;
	this->spi_cnt++;

	if (rx_ptr)
		*rx_ptr = rx;

	/* validate reply */
	ret = spi_validate(rx);

	/* This request can only be answered by a later frame, so record it after
	 * validating the reply received in the current frame. */
	if (MSG_GET_ADDR(tx) == MSG_READ_ADDR && !read_requests.push(MSG_GET_SUBADDR(tx))) {
		spi_err++;
	}
	logSpiFrame(recentTx, rx, ret);

	return ret;
}
/**
 * @return -1 in case of communication error
 */
int L9779::spi_rw_array(const uint16_t *tx, uint16_t *rx, int n)
{
	int ret = 0;
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
		/* Meet tlead: CS low to first SCK edge. */
		l9779DelayUs(L9779_TLEAD_DELAY_US);
		/* data transfer */
		uint16_t txdata = l9779PrepareSpiWord(tx[i]);
		const rtcnt_t frame_start = chSysGetRealtimeCounterX();
		uint16_t rxdata = spiPolledExchange(spi, txdata);
		recent_frame_ticks = chSysGetRealtimeCounterX() - frame_start;

		if (rx)
			rx[i] = rxdata;
		/* Slave Select de-assertion. */
		spiUnselect(spi);
		/* Meet tcsn before the next frame. */
		l9779DelayUs(L9779_TCSN_DELAY_US);

		/* statistic and debug */
		recentTx = txdata;
		recentRx = rxdata;
		this->spi_cnt++;

		/* validate reply  */
		ret = spi_validate(rxdata);

		if (MSG_GET_ADDR(txdata) == MSG_READ_ADDR && !read_requests.push(MSG_GET_SUBADDR(txdata))) {
			spi_err++;
		}
		logSpiFrame(recentTx, rxdata, ret);

		if (ret < 0)
			break;
	}
	/* Ownership release. */
	spiReleaseBus(spi);

	/* no errors for now */
	return ret;
}

int L9779::update_output()
{
	const L9779OutputRegisters packed = l9779PackOutputRegisters(o_state, o_oe_mask);
	uint16_t tx[] = {
		/* output enables */
		CMD_CONTR_REG(0, packed.control[0]),
		CMD_CONTR_REG(1, packed.control[1]),
		CMD_CONTR_REG(2, packed.control[2]),
		CMD_CONTR_REG(3, packed.control[3])
	};
	const int ret = spi_rw_array(tx, NULL, efi::size(tx));

	if (ret == 0) {
		/* atomic */
		o_data_cached = packed.enabledState;
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

	read_requests.clear();

	ret = spi_rw(CMD_CLOCK_UNLOCK_SW_RST(BIT(1)), NULL);
	/**
	 * ???
	 */
	chThdSleepMilliseconds(3);

	read_requests.clear();

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

RUSEFI_STACK_ROOT_EXPLICIT(l9779_driver_thread, 256);

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

int L9779::writePad(size_t pin, int value) {
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
	if (L9779_DIRECT_DRIVE_MASK & BIT(pin)) {
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
		return PIN_UNKNOWN;

	if (pin < L9779_OUTPUTS)
		return getOutputDiag(pin);
	else
		return getInputDiag(pin);
}


int L9779::chip_init_data(void)
{
	int ret = 0;

	o_oe_mask = 0;

	for (int i = 0; i < L9779_DIRECT_OUTPUTS; i++) {
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
	o_oe_mask |= ~L9779_DIRECT_DRIVE_MASK;

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

	/* Verify the link without assuming an exact reply-frame delay. Each
	 * attempt is bounded to one frame; stop only when IDENT's sub-address is
	 * matched by spi_validate(). A failed probe is diagnostic, not fatal. */
	bool ident_received = false;
	for (int attempt = 0; attempt < 3; attempt++) {
		uint16_t rx = 0;
		int ident_ret = spi_rw(L9779_IDENT, &rx);
		if (ident_ret == 0 && rx_subaddress == L9779_IDENT_SUB) {
			ident_reg = rx;
			ident_received = true;
			break;
		}
	}

	if (ident_received) {
		efiPrintf(DRIVER_NAME " IDENT_REG = 0x%02x", MSG_GET_DATA(ident_reg));
	} else {
		efiPrintf(DRIVER_NAME " IDENT read failed: SPI link problem?");
	}

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

void L9779::debug()
{
	efiPrintf(DRIVER_NAME " spi=%d parity_err=%d frame_err=%d addr_err=%d",
		spi_cnt, spi_err_parity, spi_err_frame, spi_err);
	efiPrintf(DRIVER_NAME " lastTx=0x%04x lastRx=0x%04x ident=0x%04x",
		recentTx, recentRx, ident_reg);

	const uint32_t frame_us = recent_frame_ticks == 0
		? 0
		: RTC2US(SystemCoreClock, recent_frame_ticks);
	efiPrintf(DRIVER_NAME " frame=%luus SPI1: CR1=0x%08x CR2=0x%08x SR=0x%08x",
		(unsigned long)frame_us,
		(unsigned)cfg->spi_bus->spi->CR1,
		(unsigned)cfg->spi_bus->spi->CR2,
		(unsigned)cfg->spi_bus->spi->SR);
	efiPrintf(DRIVER_NAME " SPI1: DFF=%d BR=%d CPOL=%d CPHA=%d LSBFIRST=%d SPE=%d",
		(cfg->spi_bus->spi->CR1 & SPI_CR1_DFF) ? 1 : 0,
		(int)((cfg->spi_bus->spi->CR1 & SPI_CR1_BR) >> SPI_CR1_BR_Pos),
		(cfg->spi_bus->spi->CR1 & SPI_CR1_CPOL) ? 1 : 0,
		(cfg->spi_bus->spi->CR1 & SPI_CR1_CPHA) ? 1 : 0,
		(cfg->spi_bus->spi->CR1 & SPI_CR1_LSBFIRST) ? 1 : 0,
		(cfg->spi_bus->spi->CR1 & SPI_CR1_SPE) ? 1 : 0);

	if (cfg->spi_config.ssport != nullptr) {
		efiPrintf(DRIVER_NAME " CS: moder=%d odr=%d idr=%d",
			(int)((cfg->spi_config.ssport->MODER >> (cfg->spi_config.sspad * 2)) & 0x3),
			(int)((cfg->spi_config.ssport->ODR >> cfg->spi_config.sspad) & 1),
			(int)((cfg->spi_config.ssport->IDR >> cfg->spi_config.sspad) & 1));
	}

	for (size_t i = 0; i < frame_log.size(); i++) {
		const L9779SpiFrame* frame = frame_log.get(i);
		efiPrintf(DRIVER_NAME " dbg: tx=0x%04x rx=0x%04x sub=%02x ret=%d",
			frame->tx, frame->rx, frame->subaddress, (int)frame->result);
	}
}

/**
 * @brief L9779 driver add.
 * @details Checks for valid config
 * @return return gpio chip base
 */

int l9779_add(brain_pin_e base, unsigned int index, const l9779_config *cfg) {

	efiAssert(ObdCode::OBD_PCM_Processor_Fault, cfg != NULL, "L9779CFG", 0)

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
	chip->drv_state = L9779_WAIT_INIT;

	/* register */
	int ret = gpiochip_register(base, DRIVER_NAME, *chip, L9779_SIGNALS);
	if (ret < 0)
		return ret;

	/* set default pin names, board init code can rewrite */
	gpiochips_setPinNames(base, l9779_pin_names);

	return ret;
}

#endif /* (BOARD_L9779_COUNT > 0) */
