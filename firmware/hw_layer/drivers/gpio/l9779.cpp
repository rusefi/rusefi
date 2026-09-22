/*
 * l9779.cpp
 *
 *  Created on: Jan 10, 2022
 *
 * Andrey Gusakov, (c) 2022
 *
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
#define DIAG_REFRESH_MS			(125)
#define DIAG_REFRESH_REGS			(3)
#define OUT_DIS_HEAL_MS				(200)

/* VDA 2.0 level 3 watchdog timing. RESPTIME=10 gives an answer window of
 * [15.8, 28.4] ms at 64 kHz and [25.9, 38.5] ms at 39 kHz. A 27 ms answer
 * period is accepted at either rate, while REQUHI timing verdicts can move it
 * within the union of both windows. */
#define WDA_RESPTIME					(10)
#define WDA_DELAY_INIT_MS			(27)
#define WDA_EC_SAT_ESCAPE_CYCLES	(8)
#define WDA_BURST_LEAD_US			(80)
#define L9779_CONFIG6_PWR			(0x06)
#define L9779_CONFIG6_PSOFF			(0x16)
#define L9779_WD_RESPTIME_REG		(0x11)

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
#define L9779_WD_ANSW(d)			MSG_W(0x0e, (d))

#define L9779_CONFIG_REG1			(0x01)
#define L9779_CONFIG_REG5			(0x05)

/* Read only registers (common address 0x10 plus a 5-bit sub-address in the
 * MOSI data field; the reply carries the sub-address in its address field). */
#define L9779_IDENT_SUB				(0x00)
#define L9779_DIA_REG1_SUB			(0x01)
#define L9779_DIA_REG9_SUB			(0x09)
#define L9779_DIA_REG10_SUB			(0x0a)
#define L9779_WD_RESPTIME_SUB		(0x0d)
#define L9779_WD_REQULO_SUB			(0x0e)
#define L9779_WD_REQUHI_SUB			(0x0f)
#define L9779_IDENT					(MSG_SET_ADDR(MSG_READ_ADDR) | MSG_SET_SUBADDR(L9779_IDENT_SUB))
#define L9779_WD_REQULO				(MSG_SET_ADDR(MSG_READ_ADDR) | MSG_SET_SUBADDR(L9779_WD_REQULO_SUB))
#define L9779_WD_REQUHI				(MSG_SET_ADDR(MSG_READ_ADDR) | MSG_SET_SUBADDR(L9779_WD_REQUHI_SUB))

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
	int spi_frame_isr(uint16_t tx, uint16_t *rx_ptr);
	int read_diag_reg(uint8_t subaddress, uint16_t *value);
	int refresh_diag_cache(int maxRegisters);
	void wd_feed();
	void wd_arm(int delayMs);

	int update_output();
	int update_direct_output(size_t pin, int value);
	int wake_driver();
	void logSpiFrame(uint16_t tx, uint16_t rx, int result);

	int chip_reset();
	int chip_init_data();
	int chip_init();
	int chip_heal_out_dis(bool configurationLost);
	int chip_power_off();
	int vrs_configure();

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
	bool						o_dirty;

	l9779_drv_state				drv_state;

	/* Read replies are delayed and can arrive after intervening frames.
	 * Match them by the sub-address returned by the chip, not by position. */
	L9779ReadTracker				read_requests;
	/* Sub-address answered by the most recently validated frame. */
	uint8_t						rx_subaddress = REG_INVALID;
	volatile bool				spi_busy;
	bool						spi_configured;

	volatile bool				wd_running;
	uint8_t						wd_last_req;
	uint8_t						wd_last_ec;
	bool						wd_int;
	int							wd_delay_ms;
	int							wd_ok_cnt;
	int							wd_fail_cnt;
	int							wd_timing_miss_cnt;
	int							wd_wrong_cnt;
	int							wd_cnt_bad;
	int							wd_defer_cnt;
	int							wd_kill_cnt;
	int							wd_poll_timeouts;
	int							wd_ec_sat_cycles;
	uint8_t						wd_last_requhi;
	bool						wd_prev_int;
	bool						wd_prev_cycle_clean;


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
	uint16_t					dia_cache[8];
	bool						dia_valid[8];
	uint8_t						dia10_cache;
	bool						dia10_valid;
	volatile bool				key_on_status;
	volatile bool				key_on_valid;
	sysinterval_t				diag_ts;
	int							diag_next_reg;
	int							diag_pending;
	bool						out_dis_latched;
	systime_t					out_dis_heal_ts;
	/* Written by the board callback and applied by the driver thread. Keep
	 * the defaults on so boards without an ignition gate retain old behavior. */
	volatile bool				power_stage_on = true;
	bool						power_stage_applied = true;
	L9779SpiFrameLog				frame_log;
};

static L9779 chips[BOARD_L9779_COUNT];

bool l9779_getWdaCounters(uint8_t *ec, bool *wdaInt, int *ok, int *fail,
	int *timingMiss, uint8_t *dia10, int *delayMs, int *deferCount,
	int *killCount, uint8_t *requhi, int *wrongCount, int *countBad)
{
	L9779 *chip = &chips[0];

	if (chip->cfg == nullptr) {
		return false;
	}

	if (ec != nullptr) { *ec = chip->wd_last_ec; }
	if (wdaInt != nullptr) { *wdaInt = chip->wd_int; }
	if (ok != nullptr) { *ok = chip->wd_ok_cnt; }
	if (fail != nullptr) { *fail = chip->wd_fail_cnt; }
	if (timingMiss != nullptr) { *timingMiss = chip->wd_timing_miss_cnt; }
	if (dia10 != nullptr) { *dia10 = chip->dia10_cache; }
	if (delayMs != nullptr) { *delayMs = chip->wd_delay_ms; }
	if (deferCount != nullptr) { *deferCount = chip->wd_defer_cnt; }
	if (killCount != nullptr) { *killCount = chip->wd_kill_cnt; }
	if (requhi != nullptr) { *requhi = chip->wd_last_requhi; }
	if (wrongCount != nullptr) { *wrongCount = chip->wd_wrong_cnt; }
	if (countBad != nullptr) { *countBad = chip->wd_cnt_bad; }

	return true;
}

void l9779_setPowerStage(bool on)
{
	L9779 *chip = &chips[0];

	if (chip->cfg == nullptr) {
		return;
	}

	chip->power_stage_on = on;
	if (chip->thread != nullptr) {
		chip->wake_driver();
	}
}

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

static brain_pin_diag_e l9779DecodeOutputDiag(uint8_t field)
{
	switch (l9779DecodeDiagField(field)) {
	case L9779DiagResult::ShortToGround:
		return PIN_SHORT_TO_GND;
	case L9779DiagResult::OpenLoad:
		return PIN_OPEN;
	case L9779DiagResult::ShortToBattery:
		return PIN_SHORT_TO_BAT;
	case L9779DiagResult::Ok:
	default:
		return PIN_OK;
	}
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
	spi_busy = true;

	/* Acquire ownership of the bus. */
	spiAcquireBus(spi);
	/* The bus is dedicated to this chip. Avoid reopening the brief SPE=0
	 * window that spiStart() creates while the watchdog ISR is active. */
	if (!spi_configured) {
		spiStart(spi, &cfg->spi_config);
		spi_configured = true;
	}
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
	spi_busy = false;

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
	spi_busy = true;

	/* Acquire ownership of the bus. */
	spiAcquireBus(spi);
	if (!spi_configured) {
		spiStart(spi, &cfg->spi_config);
		spi_configured = true;
	}

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
	spi_busy = false;

	/* no errors for now */
	return ret;
}

/* ISR-safe single frame. The thread-side SPI setup persists because this bus
 * is dedicated to the L9779; a bounded RXNE poll turns a disabled peripheral
 * into a visible missed feed instead of an infinite ISR lockup. */
int L9779::spi_frame_isr(uint16_t tx, uint16_t *rx_ptr)
{
	SPIDriver *spi = cfg->spi_bus;
	tx = l9779PrepareSpiWord(tx);

	spiSelectI(spi);
	l9779DelayUs(L9779_TLEAD_DELAY_US);

	const efitick_t start = getTimeNowNt();
	spi->spi->DR = tx;
	while ((spi->spi->SR & SPI_SR_RXNE) == 0) {
		if (getTimeNowNt() - start > US2NT(1000)) {
			wd_poll_timeouts++;
			spiUnselectI(spi);
			return -3;
		}
	}

	const uint16_t rx = spi->spi->DR;
	spiUnselectI(spi);
	l9779DelayUs(L9779_TCSN_DELAY_US);

	recentTx = tx;
	recentRx = rx;
	spi_cnt++;
	if (rx_ptr != nullptr) {
		*rx_ptr = rx;
	}

	const int ret = spi_validate(rx);
	if (MSG_GET_ADDR(tx) == MSG_READ_ADDR && !read_requests.push(MSG_GET_SUBADDR(tx))) {
		spi_err++;
	}
	logSpiFrame(tx, rx, ret);
	return ret;
}

/* L9779 read replies can arrive after intervening frames. Keep issuing the
 * same request until its content-addressed reply arrives, with a hard bound
 * so a missing chip cannot stall the driver thread. */
int L9779::read_diag_reg(uint8_t subaddress, uint16_t *value)
{
	for (int attempt = 0; attempt < 3; attempt++) {
		uint16_t rx = 0;
		const int ret = spi_rw(
			MSG_SET_ADDR(MSG_READ_ADDR) | MSG_SET_SUBADDR(subaddress), &rx);
		if (ret == 0 && rx_subaddress == subaddress) {
			*value = rx;
			return 0;
		}
	}

	return -1;
}

/* Refresh a bounded chunk of DIA_REG1..8, KEY_ON in REG9, and DIA_REG10.
 * Diagnosis reads clear their fault bits, so only the driver thread accesses
 * the hardware; other contexts consume the cache. */
int L9779::refresh_diag_cache(int maxRegisters)
{
	constexpr int RegisterCount = 10;
	int processed = 0;

	for (; processed < maxRegisters && diag_next_reg < RegisterCount;
		 processed++, diag_next_reg++) {
		if (diag_next_reg < 8) {
			uint16_t value = 0;
			if (read_diag_reg(L9779_DIA_REG1_SUB + diag_next_reg, &value) == 0) {
				dia_cache[diag_next_reg] = value;
				dia_valid[diag_next_reg] = true;
			}
			continue;
		}

		if (diag_next_reg == 8) {
			uint16_t value = 0;
			if (read_diag_reg(L9779_DIA_REG9_SUB, &value) == 0) {
				key_on_status = (MSG_GET_DATA(value) & 0x80U) != 0;
				key_on_valid = true;
			}
			continue;
		}

		uint16_t value = 0;
		if (read_diag_reg(L9779_DIA_REG10_SUB, &value) != 0) {
			continue;
		}

		const uint8_t dia10 = MSG_GET_DATA(value);
		dia10_cache = dia10;
		dia10_valid = true;
		const bool outDis = l9779Dia10HasOutDis(dia10);

		if (outDis && !out_dis_latched) {
			efiPrintf(DRIVER_NAME " OUT_DIS set: DIA10=0x%02x", dia10);
		} else if (!outDis && out_dis_latched) {
			efiPrintf(DRIVER_NAME " OUT_DIS cleared: DIA10=0x%02x", dia10);
		}

		if (outDis && power_stage_on) {
			const systime_t now = chVTGetSystemTimeX();
			if (now - out_dis_heal_ts >= TIME_MS2I(OUT_DIS_HEAL_MS)) {
				out_dis_heal_ts = now;
				const bool configurationLost = l9779Dia10LostConfiguration(dia10);
				if (chip_heal_out_dis(configurationLost) == 0) {
					efiPrintf(DRIVER_NAME " OUT_DIS recovery: %s (DIA10=0x%02x)",
						configurationLost ? "reinitialized" : "START restored", dia10);
				}
			}
		} else {
			out_dis_heal_ts = 0;
		}

		out_dis_latched = outDis;
	}

	if (diag_next_reg >= RegisterCount) {
		diag_next_reg = 0;
	}

	return processed;
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
		o_dirty = false;
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

/* TIM7 shares the validated APB1 time domain with the firmware NT clock.
 * Measure it once against NT and program a 250 kHz (4 us) free-running tick.
 * lp=true is required on AT32 so the clock continues while the CPU sleeps. */
#define WDA_TIMER TIM7
#define WDA_TIMER_PSC_PROVISIONAL 143

static L9779 *s_wda_chip;
static efitick_t s_wda_last_fire_nt;
static efitick_t s_wda_fire_period_nt;
static efitick_t s_wda_previous_period_nt;
static int s_wda_previous_delay_ms;

static void wdaTimerInit()
{
	rccEnableTIM7(true);

	WDA_TIMER->PSC = WDA_TIMER_PSC_PROVISIONAL;
	WDA_TIMER->ARR = 0xffff;
	WDA_TIMER->CR1 = 0;
	WDA_TIMER->DIER = 0;
	WDA_TIMER->EGR = STM32_TIM_EGR_UG;
	WDA_TIMER->SR = 0;

	WDA_TIMER->CR1 = STM32_TIM_CR1_CEN;
	const uint32_t timerStart = WDA_TIMER->CNT;
	const efitick_t ntStart = getTimeNowNt();
	do {
	} while (getTimeNowNt() - ntStart < MS2NT(10));
	const uint32_t timerDelta = WDA_TIMER->CNT - timerStart;
	const efitick_t ntDelta = getTimeNowNt() - ntStart;
	WDA_TIMER->CR1 = 0;

	uint32_t prescaler = static_cast<uint32_t>(
		(static_cast<uint64_t>(timerDelta) * (WDA_TIMER_PSC_PROVISIONAL + 1) * 16) /
		ntDelta) - 1;
	if (prescaler > 0xffff) {
		prescaler = 0xffff;
	}

	WDA_TIMER->PSC = prescaler;
	WDA_TIMER->CNT = 0;
	WDA_TIMER->EGR = STM32_TIM_EGR_UG;
	WDA_TIMER->SR = 0;
	nvicEnableVector(STM32_TIM7_NUMBER, EFI_IRQ_L9779_WDA_PRIORITY);

	efiPrintf(DRIVER_NAME " WDA TIM7: %lu counts/%lu NT, PSC=%lu",
		(unsigned long)timerDelta, (unsigned long)ntDelta, (unsigned long)prescaler);
}

static void wdaTimerArm(uint32_t ticks)
{
	if (ticks < 2) {
		ticks = 2;
	} else if (ticks > 65535) {
		ticks = 65535;
	}

	const uint32_t reload = ticks - 1;
	if ((WDA_TIMER->CR1 & STM32_TIM_CR1_CEN) != 0) {
		if (WDA_TIMER->ARR == reload) {
			return;
		}

		WDA_TIMER->ARR = reload;
		WDA_TIMER->EGR = STM32_TIM_EGR_UG;
		WDA_TIMER->SR = 0;
		return;
	}

	WDA_TIMER->CNT = 0;
	WDA_TIMER->ARR = reload;
	WDA_TIMER->EGR = STM32_TIM_EGR_UG;
	WDA_TIMER->SR = 0;
	WDA_TIMER->DIER = STM32_TIM_DIER_UIE;
	WDA_TIMER->CR1 = STM32_TIM_CR1_CEN;
}

static void wdaTimerStop()
{
	WDA_TIMER->CR1 = 0;
	WDA_TIMER->DIER = 0;
	WDA_TIMER->SR = 0;
}

void L9779::wd_arm(int delayMs)
{
	const uint32_t intervalUs = static_cast<uint32_t>(delayMs * 1000 - WDA_BURST_LEAD_US);
	wdaTimerArm((intervalUs + 3) / 4);
}

CH_IRQ_HANDLER(STM32_TIM7_HANDLER)
{
	OSAL_IRQ_PROLOGUE();

	if ((WDA_TIMER->SR & STM32_TIM_SR_UIF) != 0) {
		WDA_TIMER->SR = ~STM32_TIM_SR_UIF;
		const efitick_t now = getTimeNowNt();
		s_wda_previous_period_nt = s_wda_fire_period_nt;
		s_wda_previous_delay_ms = s_wda_chip != nullptr ? s_wda_chip->wd_delay_ms : 0;
		if (s_wda_last_fire_nt != 0) {
			s_wda_fire_period_nt = now - s_wda_last_fire_nt;
		}
		s_wda_last_fire_nt = now;

		if (s_wda_chip != nullptr) {
			s_wda_chip->wd_feed();
		}
	}

	OSAL_IRQ_EPILOGUE();
}

/* One atomic VDA 2.0 level-3 feed. Four reads cover the chip's one- or
 * two-frame reply delay; all four response bytes then follow back-to-back so
 * RESP_CNT cannot remain shifted after a missed cycle. */
void L9779::wd_feed()
{
	if (!wd_running) {
		return;
	}

	if (!spi_configured || spi_busy) {
		wd_defer_cnt++;
		wd_prev_cycle_clean = false;
		wd_arm(spi_configured ? 1 : 10);
		return;
	}

	static constexpr uint16_t RequestFrames[] = {
		L9779_WD_REQUHI,
		L9779_WD_REQULO,
		L9779_WD_REQUHI,
		L9779_WD_REQUHI,
	};
	constexpr uint32_t BurstBasepri = 4u << (8u - __NVIC_PRIO_BITS);

	if (read_requests.size() > 4) {
		read_requests.clear();
	}

	uint8_t requhi = 0;
	uint8_t requlo = 0;
	bool requloReceived = false;
	int ret = 0;
	const uint32_t previousBasepri = __get_BASEPRI();
	__set_BASEPRI(BurstBasepri);
	for (size_t i = 0; i < efi::size(RequestFrames); i++) {
		uint16_t rx = 0;
		ret = spi_frame_isr(RequestFrames[i], &rx);
		if (ret < 0) {
			break;
		}

		if (rx_subaddress == L9779_WD_REQUHI_SUB) {
			requhi = MSG_GET_DATA(rx);
		} else if (rx_subaddress == L9779_WD_REQULO_SUB) {
			requlo = MSG_GET_DATA(rx);
			requloReceived = true;
		}
	}
	__set_BASEPRI(previousBasepri);

	if (ret < 0 || !requloReceived) {
		wd_fail_cnt++;
		wd_prev_cycle_clean = false;
		wd_arm(10);
		return;
	}

	bool previousClean = wd_prev_cycle_clean;
	wd_prev_cycle_clean = false;
	if (s_wda_previous_period_nt != 0 && s_wda_previous_delay_ms > 0) {
		const uint32_t periodUs = static_cast<uint32_t>(NT2US(s_wda_previous_period_nt));
		const uint32_t armedUs = static_cast<uint32_t>(s_wda_previous_delay_ms * 1000);
		if (periodUs < (armedUs * 3) / 4 || periodUs > (armedUs * 5) / 4) {
			previousClean = false;
		}
	}

	if (previousClean) {
		if ((requhi & 0x03U) != 0) {
			wd_timing_miss_cnt++;
		}
		const int adjustedDelay = l9779AdjustWdaDelay(wd_delay_ms, requhi);
		if (adjustedDelay != wd_delay_ms) {
			wd_delay_ms = adjustedDelay;
		}
	}

	const L9779WdaStatus status = l9779DecodeWdaStatus(requlo);
	wd_last_req = status.question;
	wd_last_ec = status.errorCount;
	wd_int = status.interrupt;
	wd_last_requhi = requhi;
	if (wd_int && !wd_prev_int) {
		wd_kill_cnt++;
	}
	wd_prev_int = wd_int;

	if ((requhi & (0x04U | 0x10U | 0x20U)) != 0) {
		wd_wrong_cnt++;
		wd_delay_ms = WDA_DELAY_INIT_MS;
	}

	if (previousClean && status.errorCount >= 6 && wd_delay_ms != WDA_DELAY_INIT_MS) {
		wd_ec_sat_cycles++;
		if (wd_ec_sat_cycles >= WDA_EC_SAT_ESCAPE_CYCLES) {
			wd_ec_sat_cycles = 0;
			wd_delay_ms += wd_delay_ms > WDA_DELAY_INIT_MS ? -5 : 5;
		}
	} else {
		wd_ec_sat_cycles = 0;
	}

	if (!l9779WdaResponseCounterAligned(requhi)) {
		wd_cnt_bad++;
		wd_delay_ms = WDA_DELAY_INIT_MS;
		wd_arm(wd_delay_ms);
		return;
	}

	const uint8_t *response = L9779_WDA_RESPONSES[wd_last_req];
	const uint32_t responseBasepri = __get_BASEPRI();
	__set_BASEPRI(BurstBasepri);
	for (size_t i = 0; i < 4; i++) {
		ret = spi_frame_isr(L9779_WD_ANSW(response[i]), nullptr);
		if (ret < 0) {
			break;
		}
	}
	__set_BASEPRI(responseBasepri);

	if (ret == 0) {
		wd_ok_cnt++;
		wd_prev_cycle_clean = true;
	} else {
		wd_fail_cnt++;
	}

	wd_arm(wd_delay_ms);
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
		const systime_t now = chVTGetSystemTimeX();

		/* default polling interval */
		poll_interval = TIME_MS2I(DIAG_PERIOD_MS);

		if ((chip->cfg == NULL) ||
			(chip->drv_state == L9779_DISABLED) ||
			(chip->drv_state == L9779_FAILED)) {
			continue;
		}

		if (chip->power_stage_on != chip->power_stage_applied) {
			chip->power_stage_applied = chip->power_stage_on;
			if (chip->power_stage_on) {
				chip->need_init = true;
			} else {
				chip->chip_power_off();
			}
		}

		if (chip->power_stage_on) {
			if (chip->need_init) {
				/* A key-on reset clears the parked PSOFF state and watchdog EC. */
				chip->need_init = false;
				chip->chip_reset();
				chip->chip_init();
				chip->update_output();
			}

			if (!chip->wd_running) {
				chip->wd_running = true;
				chip->wd_arm(chip->wd_delay_ms);
			}

			/* Recover if a system reset stopped the timer while driver RAM survived. */
			if ((WDA_TIMER->CR1 & STM32_TIM_CR1_CEN) == 0) {
				chip->wd_arm(chip->wd_delay_ms);
			}

			if (chip->o_dirty) {
				ret = chip->update_output();
				if (ret != 0) {
					/* set state to L9779_FAILED? */
				}
			}
		}

		if (chip->diag_pending == 0 && chip->diag_ts <= now) {
			chip->diag_next_reg = 0;
			chip->diag_pending = 10;
			chip->diag_ts = chTimeAddX(now, TIME_MS2I(DIAG_REFRESH_MS));
		}

		if (chip->diag_pending > 0) {
			const uint32_t timerCount = WDA_TIMER->CNT;
			const uint32_t timerReload = WDA_TIMER->ARR;
			const bool burstImminent = chip->wd_running && timerReload >= timerCount &&
				(timerReload - timerCount) < 500; // 2 ms at 250 kHz.
			if (!burstImminent) {
				chip->diag_pending -= chip->refresh_diag_cache(DIAG_REFRESH_REGS);
			}
		}
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

		if ((L9779_DIRECT_DRIVE_MASK & BIT(pin)) == 0) {
			o_dirty = true;
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
	if (pin >= L9779_OUTPUTS) {
		return PIN_UNKNOWN;
	}

	const L9779DiagLocation location = l9779GetDiagLocation(pin);
	if (!location.supported()) {
		return PIN_UNKNOWN;
	}

	if (!dia_valid[location.registerIndex]) {
		return PIN_UNKNOWN;
	}

	const uint8_t diagnosis = MSG_GET_DATA(dia_cache[location.registerIndex]);
	return l9779DecodeOutputDiag((diagnosis >> location.shift) & 0x03U);
}

brain_pin_diag_e L9779::getInputDiag(unsigned int pin)
{
	(void)pin;

	return PIN_OK;
}

int L9779::readPad(size_t pin) {
	if (pin >= L9779_SIGNALS)
		return -1;

	/* The only L9779 input exposed as a GPIO is KEY_ON from DIA_REG9. */
	if (pin == L9779_OUTPUTS) {
		return key_on_valid ? (key_on_status ? 1 : 0) : -1;
	}

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

/* Configure the crank VR conditioner for the L9779's fully adaptive mode
 * (datasheet section 6.14). CONFIG_REG1 enables full adaptation, while
 * CONFIG_REG5 enables both amplitude-based hysteresis and the adaptive time
 * filter (Tfilter = 1/32 of the tooth period) with a 17 uA floor. The chip
 * performs both adaptations internally, so no RPM-driven software ramp is
 * needed. These registers are write-only and reset with the chip. */
int L9779::vrs_configure()
{
	constexpr L9779VrsConfiguration vrsConfig = l9779FullAdaptiveVrsConfiguration();

	int ret = spi_rw(MSG_W(L9779_CONFIG_REG1, vrsConfig.config1), NULL);
	if (ret != 0) {
		return ret;
	}

	ret = spi_rw(MSG_W(L9779_CONFIG_REG5, vrsConfig.config5), NULL);
	if (ret != 0) {
		return ret;
	}

	efiPrintf(DRIVER_NAME " VRS full adaptive: REG1=0x%02x REG5=0x%02x",
		vrsConfig.config1, vrsConfig.config5);
	return 0;
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

	/* Pin the watchdog clock configuration before anchoring its response
	 * window, then select the short window accepted by the 27 ms feed. */
	ret = spi_rw(MSG_W(0x06, L9779_CONFIG6_PWR), NULL);
	if (ret != 0) {
		return ret;
	}

	ret = spi_rw(MSG_W(L9779_WD_RESPTIME_REG, WDA_RESPTIME), NULL);
	if (ret != 0) {
		return ret;
	}

	uint16_t responseTime = 0;
	if (read_diag_reg(L9779_WD_RESPTIME_SUB, &responseTime) == 0) {
		efiPrintf(DRIVER_NAME " WDA RESPTIME=0x%02x",
			MSG_GET_DATA(responseTime) & 0x3f);
	}

	/* A power-on or smart-reset event restores the write-only VRS registers
	 * to their defaults. chip_init() serves both initial setup and the
	 * configuration-lost OUT_DIS recovery path, so reapply them here. */
	ret = vrs_configure();
	if (ret != 0) {
		return ret;
	}

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

/* Stop watchdog traffic before parking the power stages. CONFIG_REG6 PSOFF
 * leaves chip logic, SPI, and KEY_ON monitoring alive for the wake edge. */
int L9779::chip_power_off()
{
	wd_running = false;
	wdaTimerStop();

	const int ret = spi_rw(MSG_W(0x06, L9779_CONFIG6_PSOFF), NULL);
	if (ret != 0) {
		efiPrintf(DRIVER_NAME " PSOFF write failed (%d)", ret);
	}

	return ret;
}

int L9779::chip_heal_out_dis(bool configurationLost)
{
	if (configurationLost) {
		/* Reapply all write-only configuration, including the VRS setup. */
		const int ret = chip_init();
		if (ret != 0) {
			return ret;
		}
	} else {
		const int ret = spi_rw(CMD_START_REACT(BIT(1)), NULL);
		if (ret != 0) {
			return ret;
		}
	}

	return update_output();
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

	o_dirty = true;
	wd_running = false;
	wd_last_req = 0;
	wd_last_ec = 0;
	wd_int = false;
	wd_delay_ms = WDA_DELAY_INIT_MS;
	wd_ok_cnt = 0;
	wd_fail_cnt = 0;
	wd_timing_miss_cnt = 0;
	wd_wrong_cnt = 0;
	wd_cnt_bad = 0;
	wd_defer_cnt = 0;
	wd_kill_cnt = 0;
	wd_poll_timeouts = 0;
	wd_ec_sat_cycles = 0;
	wd_last_requhi = 0;
	wd_prev_int = false;
	wd_prev_cycle_clean = false;
	s_wda_last_fire_nt = 0;
	s_wda_fire_period_nt = 0;
	s_wda_previous_period_nt = 0;
	s_wda_previous_delay_ms = 0;
	s_wda_chip = this;
	wdaTimerInit();

	for (size_t i = 0; i < efi::size(dia_valid); i++) {
		dia_valid[i] = false;
	}
	dia10_valid = false;
	key_on_valid = false;
	diag_ts = 0;
	diag_next_reg = 0;
	diag_pending = 0;
	out_dis_latched = false;
	out_dis_heal_ts = 0;

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
	wd_running = false;
	wdaTimerStop();
	return 0;
}

void L9779::debug()
{
	efiPrintf(DRIVER_NAME " spi=%d parity_err=%d frame_err=%d addr_err=%d",
		spi_cnt, spi_err_parity, spi_err_frame, spi_err);
	efiPrintf(DRIVER_NAME " lastTx=0x%04x lastRx=0x%04x ident=0x%04x",
		recentTx, recentRx, ident_reg);
	if (dia10_valid) {
		efiPrintf(DRIVER_NAME " DIA10=0x%02x OUT_DIS=%d config_lost=%d",
			dia10_cache,
			l9779Dia10HasOutDis(dia10_cache),
			l9779Dia10LostConfiguration(dia10_cache));
	}
	efiPrintf(DRIVER_NAME " WDA ec=%d int=%d ok=%d fail=%d miss=%d wrong=%d cntbad=%d defer=%d kill=%d pollto=%d delay=%d",
		wd_last_ec, wd_int, wd_ok_cnt, wd_fail_cnt, wd_timing_miss_cnt,
		wd_wrong_cnt, wd_cnt_bad, wd_defer_cnt, wd_kill_cnt,
		wd_poll_timeouts, wd_delay_ms);
	efiPrintf(DRIVER_NAME " WDA TIM7 CR1=0x%08lx DIER=0x%08lx PSC=%lu ARR=%lu CNT=%lu",
		(unsigned long)WDA_TIMER->CR1, (unsigned long)WDA_TIMER->DIER,
		(unsigned long)WDA_TIMER->PSC, (unsigned long)WDA_TIMER->ARR,
		(unsigned long)WDA_TIMER->CNT);

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
