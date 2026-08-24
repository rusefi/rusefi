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

/* Refresh period for the power-stage diagnosis cache. Must be longer than
 * one chip monitoring cycle (~112 ms): reading a DIA register clears its
 * fault bits on the chip, so a faster poll would mask latched faults. */
#define DIAG_REFRESH_MS				(100)
/* Registers refreshed per driver-thread pass. The refresh is split into
 * short chunks so the executor's WDA exchange is deferred for at most a
 * few frames (see spi_busy). */
#define DIAG_REFRESH_REGS			(3)

/* L9779WD-SPI timing requirements (datasheet Table 53):
 *  - tlead >= 525 ns: CS low to first SCK edge
 *  - tcsn  >= 640 ns: CS high between two frames
 * With a GPIO-driven CS both gaps are only tens of nanoseconds, which the
 * chip flags as failed receptions (SPI_ERR). The delays below give a
 * comfortable margin; they are small enough to be negligible at the
 * frame rates used here. */
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

/* WDA monitoring (VDA 2.0 level 3, datasheet section 6.15) */
#define L9779_WD_ANSW_REG			0x0e	/* write-only: one byte of the 32-bit response */
#define L9779_WD_ANSW(d)			MSG_W(L9779_WD_ANSW_REG, (d))
/* read-only registers use the common address 0x10 plus a 5-bit sub-address
 * (datasheet Table 55); the reply carries the sub-address in the ADD field */
#define L9779_WD_REQULO_SUB			0x0e	/* WDA question + error counter (DIA_REG14) */
#define L9779_WD_REQUHI_SUB			0x0f	/* WDA response status (DIA_REG15) */
#define L9779_IDENT_SUB				0x00	/* identifier register */
#define L9779_DIA_REG1_SUB			0x01	/* OUT1..4 diagnosis */
#define L9779_DIA_REG6_SUB			0x06	/* OUT21..24 diagnosis */
#define L9779_DIA_REG7_SUB			0x07	/* OUT25..28 diagnosis */
#define L9779_DIA_REG8_SUB			0x08	/* IGN1..4 diagnosis */
#define L9779_DIA_REG9_SUB			0x09	/* KEY_ON_STATUS + VTRK1/2 diagnosis */
#define L9779_DIA_REG10_SUB			0x0a	/* OUT_DIS + power-stage fault/reset flags */
#define L9779_WD_REQULO				(MSG_SET_ADDR(MSG_READ_ADDR) | MSG_SET_SUBADDR(L9779_WD_REQULO_SUB))
#define L9779_WD_REQUHI				(MSG_SET_ADDR(MSG_READ_ADDR) | MSG_SET_SUBADDR(L9779_WD_REQUHI_SUB))
#define L9779_IDENT					(MSG_SET_ADDR(MSG_READ_ADDR) | MSG_SET_SUBADDR(L9779_IDENT_SUB))

/* Read only registers */

/* IGN1..4 + OUT1..7 */
#define OUT_DIRECT_DRIVE_MASK		0x7ff

/*==========================================================================*/
/* Driver exported variables.												*/
/*==========================================================================*/

/* Expected 32-bit response for every 4-bit WDA question.
 * Layout: [question] -> { RESP_BYTE3, RESP_BYTE2, RESP_BYTE1, RESP_BYTE0 }
 * (datasheet Table 51, values re-verified against the RESP_SOLL7..0 formulas
 * in section 6.15.2) */
static const uint8_t wd_resp_table[16][4] = {
	{0xff, 0x0f, 0xf0, 0x00},	/* 0 */
	{0xb0, 0x40, 0xbf, 0x4f},	/* 1 */
	{0xe9, 0x19, 0xe6, 0x16},	/* 2 */
	{0xa6, 0x56, 0xa9, 0x59},	/* 3 */
	{0x75, 0x85, 0x7a, 0x8a},	/* 4 */
	{0x3a, 0xca, 0x35, 0xc5},	/* 5 */
	{0x63, 0x93, 0x6c, 0x9c},	/* 6 */
	{0x2c, 0xdc, 0x23, 0xd3},	/* 7 */
	{0xd2, 0x22, 0xdd, 0x2d},	/* 8 */
	{0x9d, 0x6d, 0x92, 0x62},	/* 9 */
	{0xc4, 0x34, 0xcb, 0x3b},	/* a */
	{0x8b, 0x7b, 0x84, 0x74},	/* b */
	{0x58, 0xa8, 0x57, 0xa7},	/* c */
	{0x17, 0xe7, 0x18, 0xe8},	/* d */
	{0x4e, 0xbe, 0x41, 0xb1},	/* e */
	{0x01, 0xf1, 0x0e, 0xfe},	/* f */
};

/* Decode the 2-bit power-stage diagnosis field (datasheet 6.14):
 * 00 SCG (short-circuit to ground), 01 OL (open load),
 * 10 SCB (short-circuit to battery), 11 OK */
static const char *l9779_diag_str(uint8_t d)
{
	switch (d) {
	case 0:  return "SCG";
	case 1:  return "OL";
	case 2:  return "SCB";
	default: return "OK";
	}
}

/* Map a 2-bit power-stage diagnosis field to the shared output-fault bitmask
 * consumed by getOutputDiag()/SensorChecker: 00 SCG, 01 OL, 10 SCB, 11 OK */
static brain_pin_diag_e l9779_diag_decode(uint8_t d)
{
	switch (d) {
	case 0:  return PIN_SHORT_TO_GND;
	case 1:  return PIN_OPEN;
	case 2:  return PIN_SHORT_TO_BAT;
	default: return PIN_OK;
	}
}

/*==========================================================================*/
/* Driver private data.														*/
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
	void spi_queue_read(uint8_t subaddr);
	int spi_validate(uint16_t rx);
	int spi_rw(uint16_t tx, uint16_t *rx_ptr);
	int spi_rw_array(const uint16_t *tx, uint16_t *rx, int n);
	int read_diag_reg(uint8_t sub, uint16_t *out);
	int refresh_diag_cache(int maxRegs);

	int update_output();
	int update_direct_output(size_t pin, int value);
	int wake_driver();
	void dbg_add_frame(uint16_t tx, uint16_t rx, uint8_t sub, int ret);
	void dbg_print_frames();

	int chip_reset();
	int chip_init_data();
	int chip_init();
	int vrs_configure();
	int vrs_ramp_to_step(int step);
	/* ISR-safe polled SPI (executor context, no bus mutex) */
	int spi_frame_isr(uint16_t tx, uint16_t *rx_ptr);
	int wd_feed_isr();
	void wdFeedFromExecutor();
	void debug() override;

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

	/* Outstanding read requests, matched against the replies in order:
	 * the DO reply to a request arrives in one of the frames that follow
	 * the request (datasheet 6.16.2), so the request is remembered here
	 * and the reply is matched when it shows up */
	uint8_t						rd_pending[8];
	int							rd_pending_head;	/* index of the oldest outstanding read */
	int							rd_pending_cnt;		/* number of outstanding reads */
	/* sub-address answered by the last received frame (REG_INVALID when
	 * the frame was a write echo / idle pattern) */
	uint8_t						rx_subaddr;
	/* output registers changed since the last successful SPI update */
	bool						o_dirty;

	/* WDA (VDA 2.0) query-answer watchdog state */
	uint8_t						wd_last_req;	/* last question from the chip */
	uint8_t						wd_last_ec;		/* error counter as reported by the chip */
	bool						wd_int;			/* WDA_INT flag (EC > 4) */
	int							wd_delay_ms;	/* response delay, aimed at window center */
	/* The WDA feed runs on the TIM5 executor (ISR context): the answer must
	 * land inside the chip's ~12.6 ms window, and a thread wakeup can be
	 * delayed past that by trigger-decode ISR load at cranking (the observed
	 * wd_timing_miss/EC>4 kills). The executor (priority 3) preempts the
	 * trigger handoff, so the feed stays on time whenever the CPU executes
	 * at all. The callback self-reschedules via wd_sched; the driver thread
	 * only kicks the first feed after chip_init. */
	scheduling_s				wd_sched;
	bool						wd_running;
	int						wd_ok_cnt;		/* cycles answered correctly */
	int						wd_fail_cnt;	/* cycles missed (SPI-level failures) */
	int						wd_timing_miss_cnt;	/* responses outside the window (REQUHI flags) - the EC climbs on these too */
	int						wd_defer_cnt;	/* feeds deferred by the spi_busy flag */
	int						wd_kill_cnt;	/* WDA_INT rising edges (watchdog kill pulses) */
	int						wd_last_miss_dir;	/* last REQUHI miss: 1=early 2=late */
	bool						wd_prev_int;
	uint16_t					ident_reg;		/* IDENT_REG readback (0x10 | 0x00) */

	/* Cached power-stage diagnosis, DIA_REG1..8 (datasheet 6.14). Refreshed
	 * from the driver thread only: getOutputDiag() is called from the
	 * SensorChecker/console context where SPI is not available. Reading a DIA
	 * register clears its fault bits on the chip, hence the throttled refresh
	 * (DIAG_REFRESH_MS, see refresh_diag_cache()). */
	uint16_t					dia_cache[8];
	bool						dia_valid[8];
	sysinterval_t				diag_ts;	/* when to refresh the cache next */
	/* The refresh is chunked (a few registers per thread pass): each pass
	 * sets spi_busy around its frames and the executor's WDA exchange defers
	 * while it runs. diag_next_reg is the cursor, diag_pending the
	 * registers left in the current 100 ms refresh cycle. */
	int							diag_next_reg;
	int							diag_pending;
	/* DIA_REG10 byte: OUT_DIS + power-event flags (CRK_RST, V3V3_UV, OV_RST,
	 * VDD5_OV, TNL_RST, F1/F2). Cached together with REG1..8 - the flags are
	 * cleared by the read, so cross-driver consumers (the TLE9201 warning)
	 * see the last known value. */
	uint8_t						dia10_cache;
	bool						dia10_valid;

	/* KEY_ON input level (DIA_REG9 bit 7, datasheet 6.14), cached by the
	 * driver thread together with the power-stage diagnosis. Unlike the
	 * DIA_REG1..8 faults, reading it clears nothing on the chip. */
	bool						key_on_status;
	bool						key_on_valid;

	/* Executor/thread SPI serialization. The WDA feed runs in the TIM5 ISR at
	 * kernel priority - chSysLock/CriticalSectionLocker do NOT mask kernel
	 * IRQs, so a critical section does not stop the executor from preempting
	 * the driver thread mid-batch. spi_busy is set for the whole thread-side
	 * batch (spi_rw/spi_rw_array); the executor feed checks it and defers.
	 * This matters because spiStart() briefly clears SPE (spi_lld_start
	 * re-programs CR1), and spi_lld_polled_exchange() busy-waits on RXNE -
	 * with the peripheral disabled it spins forever inside the kernel ISR and
	 * the board bricks right after boot (no fuel pump, no console). */
	volatile bool				spi_busy;
	/* CR1/CR2 persist from the first spiStart; re-running it on every batch
	 * only re-opens the SPE=0 window, so spi_rw/spi_rw_array call it once. */
	bool						spi_configured;

	/* OUT_DIS latch (DIA_REG10 bit 1): the chip disables OUT1..4/IGN1..4 and
	 * only the START command clears it. Tracked by refresh_diag_cache() so a
	 * latch event is logged WITH its fault flags (reading DIA_REG10 clears
	 * them) and a stale latch (no active fault, watchdog healthy) is healed
	 * with a single START re-issue. */
	bool						out_dis_latched;
	bool						out_dis_clear_tried;
	/* VRS hysteresis ramp: step 0 = stock ramp START written (chip_init /
	 * re-arm), steps 1..3 = the stock config script ramp (see
	 * vrs_configure/vrs_ramp_to_step). Advanced by rpm thresholds from the
	 * driver thread, re-armed per start attempt. */
	int							vrs_step = 0;
	/* Last moment the engine was NOT stopped - debounces the ramp re-arm
	 * (see the driver thread: a trigger storm flaps rpm 0/300+ at ~1 kHz
	 * and would toggle the VRS config at the same rate). */
	systime_t					vrs_stop_ts = 0;


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
	/* last frame exchange CPU cycles */
	uint32_t					recent_frame_cycles;

	/* SPI frame debug ring buffer: last L9779_DBG_FRAMES exchanges with the
	 * sub-address they were matched to and the validate result */
	enum { L9779_DBG_FRAMES = 32 };
	uint16_t					dbg_tx[L9779_DBG_FRAMES];
	uint16_t					dbg_rx[L9779_DBG_FRAMES];
	uint8_t						dbg_sub[L9779_DBG_FRAMES];
	int8_t						dbg_ret[L9779_DBG_FRAMES];
	uint8_t						dbg_cnt;	/* frames stored, saturates at L9779_DBG_FRAMES */
	uint8_t						dbg_next;	/* index of the next slot */
};

static L9779 chips[BOARD_L9779_COUNT];

bool l9779_getWdaCounters(uint8_t *ec, bool *wda_int, int *ok, int *fail, int *timing_miss, uint8_t *dia10,
		int *delay_ms, int *defer_cnt, int *kill_cnt)
{
	/* WDA counters are written by the executor feed (ISR); the reads below
	 * are single-word atomic accesses, safe from other threads. */
	L9779 *chip = &chips[0];
	if (ec)
		*ec = chip->wd_last_ec;
	if (wda_int)
		*wda_int = chip->wd_int;
	if (ok)
		*ok = chip->wd_ok_cnt;
	if (fail)
		*fail = chip->wd_fail_cnt;
	if (timing_miss)
		*timing_miss = chip->wd_timing_miss_cnt;
	if (dia10)
		*dia10 = chip->dia10_cache;
	if (delay_ms)
		*delay_ms = chip->wd_delay_ms;
	if (defer_cnt)
		*defer_cnt = chip->wd_defer_cnt;
	if (kill_cnt)
		*kill_cnt = chip->wd_kill_cnt;
	return true;
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

/* Driver pin index -> power-stage diagnosis source: reg_off is the offset
 * from L9779_DIA_REG1_SUB (0..7 = DIA_REG1..DIA_REG8), shift is the bit
 * position of the 2-bit diagnosis field inside the register data byte
 * (datasheet 6.14). reg_off -1 means the pin has no power-stage diagnosis:
 * OUT8..12, OUT19 and MRD are not present on the L9779WD-SPI, KEY is an
 * input. */
static const int8_t l9779_pin_diag[L9779_SIGNALS][2] = {
	/* IGN1..4 -> DIA_REG8: [1:0] [3:2] [5:4] [7:6] */
	{7, 0}, {7, 2}, {7, 4}, {7, 6},
	/* OUT1..4 -> DIA_REG1: [1:0] [3:2] [5:4] [7:6] */
	{0, 0}, {0, 2}, {0, 4}, {0, 6},
	/* OUT5..7 -> DIA_REG2: [1:0] [3:2] [5:4] */
	{1, 0}, {1, 2}, {1, 4},
	/* OUT8..12 - no such power stages */
	{-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0},
	/* OUT13..14 -> DIA_REG3: [5:4] [7:6] */
	{2, 4}, {2, 6},
	/* OUT15..18 -> DIA_REG4: [1:0] [3:2] [5:4] [7:6] */
	{3, 0}, {3, 2}, {3, 4}, {3, 6},
	/* OUT19 - no such power stage */
	{-1, 0},
	/* OUT20 -> DIA_REG5: [3:2] */
	{4, 2},
	/* OUT21..24 -> DIA_REG6: [1:0] [3:2] [5:4] [7:6] */
	{5, 0}, {5, 2}, {5, 4}, {5, 6},
	/* OUT25..28 -> DIA_REG7: [1:0] [3:2] [5:4] [7:6] */
	{6, 0}, {6, 2}, {6, 4}, {6, 6},
	/* MRD - no power-stage diagnosis */
	{-1, 0},
	/* KEY - input */
	{-1, 0},
};

/*==========================================================================*/
/* Driver local functions.													*/
/*==========================================================================*/

/* Busy-wait delay using the DWT cycle counter (Cortex-M4). The core runs
 * at SystemCoreClock; the counter is enabled on first use. */
static void l9779_delay_us(int us)
{
	static bool dwt_ready = false;

	if (!dwt_ready) {
		CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
		DWT->CYCCNT = 0;
		DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
		dwt_ready = true;
	}

	uint32_t start = DWT->CYCCNT;
	uint32_t cycles = (uint32_t)us * (SystemCoreClock / 1000000);
	while ((DWT->CYCCNT - start) < cycles)
		;
}

/* true if parity of input x is odd */
bool L9779::spi_parity_odd(uint16_t x)
{
	x ^= x >> 8;
	x ^= x >> 4;
	x ^= x >> 2;
	x ^= x >> 1;

	return (x & 1);
}

/* remember a read request so its reply can be matched when it arrives */
void L9779::spi_queue_read(uint8_t subaddr)
{
	if (rd_pending_cnt >= (int)efi::size(rd_pending)) {
		/* the reply stream would misalign - this must never happen with
		 * the fixed access patterns used by this driver */
		spi_err++;
		return;
	}

	rd_pending[(rd_pending_head + rd_pending_cnt) % efi::size(rd_pending)] = subaddr;
	rd_pending_cnt++;
}

/* store one SPI exchange in the debug ring buffer */
void L9779::dbg_add_frame(uint16_t tx, uint16_t rx, uint8_t sub, int ret)
{
	uint8_t slot = dbg_next;
	dbg_tx[slot] = tx;
	dbg_rx[slot] = rx;
	dbg_sub[slot] = sub;
	dbg_ret[slot] = (int8_t)ret;
	dbg_next = (uint8_t)((dbg_next + 1) % L9779_DBG_FRAMES);
	if (dbg_cnt < L9779_DBG_FRAMES)
		dbg_cnt++;
}

/* print the stored exchanges, oldest first */
void L9779::dbg_print_frames()
{
	uint8_t start = (uint8_t)((dbg_next + L9779_DBG_FRAMES - dbg_cnt) % L9779_DBG_FRAMES);
	for (uint8_t i = 0; i < dbg_cnt; i++) {
		uint8_t s = (uint8_t)((start + i) % L9779_DBG_FRAMES);
		efiPrintf(DRIVER_NAME " dbg: tx=0x%04x rx=0x%04x sub=%02x ret=%d",
			dbg_tx[s], dbg_rx[s], dbg_sub[s], (int)dbg_ret[s]);
	}
}

int L9779::spi_validate(uint16_t rx)
{
	/* by default this frame does not answer any outstanding read */
	rx_subaddr = REG_INVALID;

	if (!spi_parity_odd(rx)) {
		spi_err_parity++;
		return -1;
	}

	if (MSG_GET_SPIERROR(rx)) {
		/* the chip flags a failed reception with bit 15 */
		spi_err_frame++;
		return -1;
	}

	/* Read replies have W/R (D9) set and carry the register sub-address in
	 * the ADD field. Write echoes (write-only registers return all zeroes
	 * in the next frame, datasheet 6.16.2) and the SW_RST/START_REACT
	 * status frames have W/R = 0 and are not replies to a read. */
	if (!MSG_GET_WR(rx))
		return 0;

	if (rd_pending_cnt <= 0) {
		/* read reply with no outstanding request */
		spi_err++;
		return 0;
	}

	/* The chip answers outstanding reads in order, so the oldest pending
	 * request is the one this reply belongs to. */
	rx_subaddr = rd_pending[rd_pending_head];
	rd_pending_head = (rd_pending_head + 1) % efi::size(rd_pending);
	rd_pending_cnt--;

	if (MSG_GET_ADDR(rx) != rx_subaddr) {
		/* unexpected content, the link is alive though */
		spi_err++;
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

	/* Executor exclusion (see the spi_busy comment in the struct): the WDA
	 * feed defers while any thread-side batch is in flight, so the executor
	 * can never land in spiStart()'s SPE=0 window. */
	spi_busy = true;

	/* Acquire ownership of the bus. */
	spiAcquireBus(spi);
	/* Setup transfer parameters - first call only: the config never changes
	 * and the bus is dedicated to this chip. Re-running spiStart() on every
	 * batch re-executes spi_lld_start(), which clears SPE before re-enabling
	 * it - a window in which spi_lld_polled_exchange() (used by the executor
	 * feed) spins on RXNE forever. */
	if (!spi_configured) {
		spiStart(spi, &cfg->spi_config);
		spi_configured = true;
	}
	/* Slave Select assertion. */
	spiSelect(spi);
	/* meet tlead: CS low to first SCK edge */
	l9779_delay_us(L9779_TLEAD_DELAY_US);
	/* Atomic transfer operations. */
	uint32_t cyc0 = DWT->CYCCNT;
	rx = spiPolledExchange(spi, tx);
	recent_frame_cycles = DWT->CYCCNT - cyc0;
	/* Slave Select de-assertion. */
	spiUnselect(spi);
	/* meet tcsn: CS high between frames */
	l9779_delay_us(L9779_TCSN_DELAY_US);
	/* Ownership release. */
	spiReleaseBus(spi);

	spi_busy = false;

	/* statistics and debug */
	recentTx = tx;
	recentRx = rx;
	this->spi_cnt++;

	if (rx_ptr)
		*rx_ptr = rx;

	/* the reply to THIS request arrives in one of the following frames,
	 * remember the request; then validate the reply received in this
	 * frame, which answers a previously issued request */
	if (MSG_GET_ADDR(tx) == MSG_READ_ADDR)
		spi_queue_read(MSG_GET_SUBADDR(tx));

	ret = spi_validate(rx);
	dbg_add_frame(recentTx, rx, rx_subaddr, ret);

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

	/* Executor exclusion, same as spi_rw(). */
	spi_busy = true;

	/* Acquire ownership of the bus. */
	spiAcquireBus(spi);
	/* Setup transfer parameters - first call only, see spi_rw(). */
	if (!spi_configured) {
		spiStart(spi, &cfg->spi_config);
		spi_configured = true;
	}

	for (int i = 0; i < n; i++) {
		/* set parity, same as spi_rw(): frames with an even number of set
		 * bits are rejected by the chip (e.g. every WD_ANSW write below), so
		 * the parity bit must be forced before transmission */
		uint16_t txdata = tx[i];
		txdata |= !spi_parity_odd(txdata);

		/* Slave Select assertion. */
		spiSelect(spi);
		/* meet tlead: CS low to first SCK edge */
		l9779_delay_us(L9779_TLEAD_DELAY_US);
		/* data transfer */
		uint32_t cyc0 = DWT->CYCCNT;
		uint16_t rxdata = spiPolledExchange(spi, txdata);
		recent_frame_cycles = DWT->CYCCNT - cyc0;

		if (rx)
			rx[i] = rxdata;
		/* Slave Select de-assertion. */
		spiUnselect(spi);
		/* meet tcsn: CS high between frames */
		l9779_delay_us(L9779_TCSN_DELAY_US);

		/* statistic and debug */
		recentTx = txdata;
		recentRx = rxdata;
		this->spi_cnt++;

		/* see spi_rw() for the reply/request pipelining */
		if (MSG_GET_ADDR(txdata) == MSG_READ_ADDR)
			spi_queue_read(MSG_GET_SUBADDR(txdata));

		/* validate reply  */
		ret = spi_validate(rxdata);
		dbg_add_frame(recentTx, rxdata, rx_subaddr, ret);

		if (ret < 0)
			break;
	}
	/* Ownership release. */
	spiReleaseBus(spi);

	spi_busy = false;

	/* no errors for now */
	return ret;
}

/* Read a read-only diagnostic register. The reply to a read request
 * arrives one frame later (the frames are pipelined and the WDA thread
 * interleaves its own reads), so keep issuing the request until a frame
 * carrying the expected sub-address comes back. Returns 0 on success. */
int L9779::read_diag_reg(uint8_t sub, uint16_t *out)
{
	for (int i = 0; i < 3; i++) {
		uint16_t rx;
		int ret = spi_rw(MSG_SET_ADDR(MSG_READ_ADDR) | MSG_SET_SUBADDR(sub), &rx);
		if (ret == 0 && rx_subaddr == sub) {
			*out = rx;
			return 0;
		}
	}
	return -1;
}

/* Refresh up to maxRegs registers of the cached power-stage diagnosis
 * (DIA_REG1..8 + REG9 + REG10), advancing the diag_next_reg cursor.
 * Returns how many registers were processed. Must only be called from the
 * driver thread: the reads are pipelined through the same
 * rd_pending/rx_subaddr state as the WDA traffic, and spi_busy (set by
 * spi_rw) keeps the executor's WDA exchange out of the batch. A failed
 * read leaves the previous cache value in place. */
int L9779::refresh_diag_cache(int maxRegs)
{
	int done = 0;
	for (; done < maxRegs && diag_next_reg < 10; done++, diag_next_reg++) {
		if (diag_next_reg < 8) {
			uint16_t val;
			if (read_diag_reg(L9779_DIA_REG1_SUB + diag_next_reg, &val) == 0) {
				dia_cache[diag_next_reg] = val;
				dia_valid[diag_next_reg] = true;
			}
		} else if (diag_next_reg == 8) {
			/* KEY_ON input level (DIA_REG9 bit 7, KEY_ON_STATUS). This is the
			 * ignition switch line on boards that route IGN_KEY to the L9779
			 * KEY_ON pin (e.g. m74_9); isIgnVoltage() reads it via readPad(). */
			uint16_t key;
			if (read_diag_reg(L9779_DIA_REG9_SUB, &key) == 0) {
				key_on_status = !!(MSG_GET_DATA(key) & 0x80);
				key_on_valid = true;
			}
		} else {
			/* DIA_REG10: OUT_DIS + power-stage fault/reset flags (datasheet
			 * 6.14). Reading it CLEARS the fault flags, so print the raw byte
			 * right here - this is the only place that sees the flags before
			 * they vanish. */
			uint16_t dia10 = 0;
			if (read_diag_reg(L9779_DIA_REG10_SUB, &dia10) == 0) {
				uint8_t d10 = MSG_GET_DATA(dia10);
				dia10_cache = d10;
				dia10_valid = true;
				bool out_dis = (d10 >> 1) & 1;
				/* any bit except OUT_DIS means a fault is (or was) reported */
				bool fault_flags = (d10 & ~0x02u) != 0;

				if (out_dis && !out_dis_latched) {
					efiPrintf(DRIVER_NAME " OUT_DIS set! DIA10=0x%02x (F1=%d F2=%d OV_RST=%d VDD5_OV=%d V3V3_UV=%d TNL_RST=%d CRK_RST=%d)",
						d10, (d10 >> 6) & 1, (d10 >> 4) & 1, d10 & 1,
						(d10 >> 3) & 1, (d10 >> 2) & 1, (d10 >> 7) & 1, (d10 >> 5) & 1);
				} else if (!out_dis && out_dis_latched) {
					efiPrintf(DRIVER_NAME " OUT_DIS cleared (DIA10=0x%02x)", d10);
				}

				/* Self-heal a stale latch: the chip only clears OUT_DIS on START, so
				 * a transient event (e.g. a watchdog EC excursion) would keep the
				 * injector/ignition stages dead until a power cycle. With no fault
				 * flags and a healthy watchdog, re-issue START once per latch. */
				if (out_dis && !fault_flags && !wd_int && !out_dis_clear_tried) {
					if (spi_rw(CMD_START_REACT(BIT(1)), NULL) == 0) {
						efiPrintf(DRIVER_NAME " OUT_DIS stale - re-issued START");
					}
					out_dis_clear_tried = true;
				}
				if (!out_dis)
					out_dis_clear_tried = false;

				out_dis_latched = out_dis;
			}
		}
	}

	if (diag_next_reg >= 10)
		diag_next_reg = 0;

	return done;
}

/* use datasheet numbering, starting from 1, skip 4 ignition channels.
 * NOTE: these macros expand inside update_output() and read the LOCAL
 * o_data (permanent enables for direct pins, live state for the rest),
 * NOT the o_state member - reading the member here was a bug that made
 * the 'permanent enable' fix dead code (6b3a686c30d). */
#define OUT_ENABLED(n)			(!!(o_data & BIT((n) + L9779_OUTPUTS_IGN - 1)))
#define SHIFT_N_OUT_TO_M(n, m)	(OUT_ENABLED(n) << (m))

/* use datasheet numbering, starting from 1 */
#define IGN_ENABLED(n)			(!!(o_data & BIT((n) - 1)))
#define SHIFT_N_IGN_TO_M(n, m)	(IGN_ENABLED(n) << (m))

int L9779::update_output()
{
	int ret;
	uint8_t regs[4];

	/* set value only for non-direct driven pins */
	uint32_t o_data = o_state & ~OUT_DIRECT_DRIVE_MASK;
	/* direct driven outputs are logically-AND of the spi enable bit and the
	 * dedicated parallel input: keep the enable PERMANENTLY set (o_oe_mask),
	 * the parallel MCU pin alone switches the channel in real time. Writing
	 * o_state here instead disabled the channel whenever the pin was low, and
	 * with the thread not woken for direct writes the enable arrived only on
	 * the next watchdog cycle (~105 ms) - bench clicks and injection pulses
	 * were truncated or never happened at all. */
	o_data |= (o_oe_mask & OUT_DIRECT_DRIVE_MASK);

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
		SHIFT_N_OUT_TO_M(13, 4) |
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
	ret = spi_rw_array(tx, NULL, efi::size(tx));

	if (ret == 0) {
		/* atomic */
		o_data_cached = o_data;
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
	/* SW_RST = frame bit 2 = data bit 1 (datasheet Table 56). The reply
	 * (LOCK status) arrives in the following frame; whatever the DO line
	 * returns here is the pre-reset state and must not fail the init */
	(void)spi_rw(CMD_CLOCK_UNLOCK_SW_RST(BIT(1)), NULL);

	chThdSleepMilliseconds(3);

	return 0;
}

/*==========================================================================*/
/* Driver thread.															*/
/*==========================================================================*/

/* Executor trampoline: the WDA feed runs in the TIM5 ISR, see the
 * wd_running/wd_sched comment in the struct. */
static void l9779WdaFeedExec(L9779 *chip) {
	chip->wdFeedFromExecutor();
}

static THD_FUNCTION(l9779_driver_thread, p) {
	L9779 *chip = reinterpret_cast<L9779*>(p);
	sysinterval_t poll_interval = 0;

	chRegSetThreadName(DRIVER_NAME);

	while (1) {
		int ret;
		msg_t msg = chSemWaitTimeout(&chip->wake, poll_interval);

		/* should we care about msg == MSG_TIMEOUT? */
		(void)msg;

		systime_t now = chVTGetSystemTimeX();

		/* default polling interval */
		poll_interval = TIME_MS2I(DIAG_PERIOD_MS);

		if ((chip->cfg == NULL) ||
			(chip->drv_state == L9779_DISABLED) ||
			(chip->drv_state == L9779_FAILED))
			continue;

		if (chip->need_init) {
			/* clear first, as flag can be raised again during init */
			chip->need_init = false;
			/* re-init chip! The executor's WDA exchange is kept out of the
			 * batch by spi_busy (kernel IRQs are NOT masked by critical
			 * sections), no CS locker is needed for that. */
			chip->chip_init();
			/* sync pins state */
			chip->update_output();
		}

		/* Kick the executor-side WDA feed once after the chip is up. The
		 * callback self-reschedules from then on. */
		if (!chip->wd_running) {
			chip->wd_running = true;
			engine->scheduler.schedule("l9779wda", &chip->wd_sched,
				getTimeNowNt() + MS2NT(5),
				action_s::make<l9779WdaFeedExec, L9779*>(chip));
		}

		/* send the output registers only when the pin state changed: with the
		 * watchdog armed the thread wakes up every millisecond, re-writing
		 * all four CONTR registers on every wakeup would saturate the SPI bus */
		if (chip->o_dirty) {
			ret = chip->update_output();
			if (ret) {
				/* o_dirty stays set - retry on the next loop */
			}
		}

		/* Stock-style VRS hysteresis ramp: the chip's peak detector is not
		 * readable (write-only registers), so rpm is the only proxy for the
		 * growing cranking signal amplitude. Step the hysteresis floor up
		 * through the stock's 4-step config script; the last step lands
		 * exactly at cranking_rpm, so the conditioner is in its final state
		 * the moment the engine is declared running. */
		static const float VRS_RAMP_RPM[3] = { 150.0f, 300.0f, 600.0f };
		bool ignitionOff = chip->key_on_valid && !chip->key_on_status;
		/* Step up only while the key is on: after an ignition-off re-arm the
		 * wind-down rpm still crosses 150 and would immediately re-step the
		 * ramp, toggling start/step-1 at driver-thread rate (SPI flood, seen
		 * at 23:16:08 - the 800-frames storm class from the 14:34 log). */
		if (!ignitionOff && chip->vrs_step < 3 &&
				Sensor::getOrZero(SensorType::Rpm) >= VRS_RAMP_RPM[chip->vrs_step]) {
			chip->vrs_step++;
			chip->vrs_ramp_to_step(chip->vrs_step);
		}

		/* The ramp must follow every start attempt: on a quick key cycle
		 * neither the L9779 nor the MCU is power-cycled, so the chip's
		 * write-only VRS registers keep the ramp END (maximum hysteresis
		 * floor) from the previous run, and the next cranking's low-amplitude
		 * teeth get swallowed by the floor -> C9002 + sync storm at the
		 * catch. Re-arm the ramp START whenever the engine has been stopped
		 * for a while or the ignition key goes off.
		 *
		 * The stop side is debounced: during a trigger storm the rpm sensor
		 * flaps 0/300+ at ~1 kHz, and an undebounced rpm==0 reset toggled
		 * the VRS config start/end at the same rate (~800 SPI frames in
		 * 1.6 s, see the 14:34 log) - saturating the SPI bus and yanking
		 * the hysteresis floor, which fed the storm it was reacting to. */
		bool engineStopped = engine->rpmCalculator.isStopped();
		if (!engineStopped) {
			chip->vrs_stop_ts = now;
		}
		bool stoppedLongEnough = engineStopped &&
			(now - chip->vrs_stop_ts) >= TIME_MS2I(500);
		if (chip->vrs_step > 0 && (stoppedLongEnough || ignitionOff)) {
			chip->vrs_configure();
			chip->vrs_step = 0;
		}

		/* Refresh the power-stage diagnosis cache. Reading a DIA register
		 * clears its fault bits on the chip, so this runs at a low rate;
		 * getOutputDiag() reads the cache from other threads. The refresh
		 * is CHUNKED (DIAG_REFRESH_REGS per pass): each pass sets spi_busy
		 * around its frames, and the executor's WDA exchange must not be
		 * deferred for the whole ~20-frame batch. */
		if (chip->diag_ts <= now) {
			chip->diag_pending = 8 + 2;	/* DIA_REG1..8 + REG9 + REG10 */
			chip->diag_ts = chTimeAddX(chVTGetSystemTimeX(), TIME_MS2I(DIAG_REFRESH_MS));
		}
		if (chip->diag_pending > 0) {
			chip->diag_pending -= chip->refresh_diag_cache(DIAG_REFRESH_REGS);
			if (chip->diag_pending < 0)
				chip->diag_pending = 0;
		}
	}
}

RUSEFI_STACK_ROOT_EXPLICIT(l9779_driver_thread, 256);

/* ISR-safe polled single-frame exchange: raw LLD calls only - no bus
 * mutex, no blocking. Callable from the TIM5 executor ISR (which preempts
 * all threads); the thread side calls it only inside spi_rw/spi_rw_array
 * with spi_busy set. Mirrors spi_rw() minus the acquire/start/release
 * wrapper. */
int L9779::spi_frame_isr(uint16_t tx, uint16_t *rx_ptr)
{
	SPIDriver *spi = cfg->spi_bus;
	uint16_t rx;

	/* set parity */
	tx |= !spi_parity_odd(tx);

	/* Slave Select assertion (I-class: ISR-safe in every CS mode). */
	spiSelectI(spi);
	/* meet tlead: CS low to first SCK edge */
	l9779_delay_us(L9779_TLEAD_DELAY_US);
	/* data transfer */
	uint32_t cyc0 = DWT->CYCCNT;
	rx = spi_lld_polled_exchange(spi, tx);
	recent_frame_cycles = DWT->CYCCNT - cyc0;
	/* Slave Select de-assertion. */
	spiUnselectI(spi);
	/* meet tcsn: CS high between frames */
	l9779_delay_us(L9779_TCSN_DELAY_US);

	/* statistics and debug */
	recentTx = tx;
	recentRx = rx;
	this->spi_cnt++;

	if (rx_ptr)
		*rx_ptr = rx;

	/* the reply to THIS request arrives in one of the following frames,
	 * remember the request; then validate the reply received in this
	 * frame, which answers a previously issued request */
	if (MSG_GET_ADDR(tx) == MSG_READ_ADDR)
		spi_queue_read(MSG_GET_SUBADDR(tx));

	int ret = spi_validate(rx);
	dbg_add_frame(recentTx, rx, rx_subaddr, ret);

	return ret;
}

/* ISR-context VDA 2.0 level 3 query-answer watchdog feed (datasheet 6.15).
 * Identical protocol to the old thread-side wd_feed(), but polled-LLD SPI
 * only, so it runs on the TIM5 executor (priority 3, above the trigger
 * handoff) where a late scheduler wakeup can no longer miss the answer
 * window.
 *
 * The chip generates a new 4-bit question every monitoring cycle
 * (response time + fixed window, ~112 ms with default RESPTIME). A wrong
 * value or a response outside the window increments the error counter EC;
 * with EC > 4 the WDA_INT flag is set and OUT1..4 + IGN1..4 are forced
 * off. EC starts at 6 after any reset, so the watchdog must be answered
 * correctly (value AND timing) at least twice before the outputs get
 * enabled, and must keep being answered afterwards.
 *
 * The DO reply to a request arrives in one of the frames that follow the
 * request (datasheet 6.16.2), so the reads are pipelined: REQUHI/REQULO
 * are requested first and their replies are collected while the four
 * answer bytes are being sent. The filler REQUHI reads flush the pipeline
 * without side effects and work with a one or two frame reply delay. */
int L9779::wd_feed_isr()
{
	int ret = 0;
	uint16_t rx;
	bool requlo_received = false;
	uint8_t requhi = 0;
	uint8_t requlo = 0;

	static const uint16_t req_tx[] = {
		L9779_WD_REQUHI,	/* status of the previous response */
		L9779_WD_REQULO,	/* current question + error counter */
		L9779_WD_REQUHI,	/* filler */
		L9779_WD_REQUHI,	/* filler */
	};

	/* No spi_lld_start here: the bus configuration (CR1/CR2) persists from
	 * the driver thread's spiStart() - the bus is dedicated to this chip,
	 * the config never changes, and the first feed is only kicked after
	 * the thread's chip_init(). */

	for (size_t i = 0; i < efi::size(req_tx); i++) {
		ret = spi_frame_isr(req_tx[i], &rx);
		if (ret < 0)
			break;

		if (rx_subaddr == L9779_WD_REQUHI_SUB)
			requhi = MSG_GET_DATA(rx);
		else if (rx_subaddr == L9779_WD_REQULO_SUB) {
			requlo = MSG_GET_DATA(rx);
			requlo_received = true;
		}
	}

	if ((ret >= 0) && !requlo_received) {
		/* the question was not received within this cycle */
		ret = -1;
	}

	if (ret < 0) {
		/* keep the current response delay but retry sooner than a full cycle */
		wd_fail_cnt++;
		return -1;
	}

	/* REQUHI flags report the timing of the previous response and are used
	 * to keep the response delay centered in the answer window. Both flags
	 * mean the previous answer was NOT accepted (outside the window), so
	 * the chip's EC incremented - count them separately from wd_fail_cnt,
	 * which only tracks SPI-level failures.
	 *
	 * The correction step is 5 ms (just under half of the ~12.6 ms window):
	 * a single miss jumps the phase from just-outside to near center without
	 * overshooting to the other edge, so a lock loss costs 1-2 misses instead
	 * of the 5-7 that the old 1 ms steps needed. Each miss sets EC > 4 and
	 * fires the chip's WDA kill output (the blade drop), so the number of
	 * CONSECUTIVE misses is what turns a momentary pulse into an engine
	 * stall - converging fast is the whole game. */
	if (requhi & 0x01) {
		/* RESP_TO_EARLY: response before the window opened */
		wd_timing_miss_cnt++;
		wd_last_miss_dir = 1;
		wd_delay_ms += 5;
	} else if (requhi & 0x02) {
		/* NO_RESP: response after the window closed */
		wd_timing_miss_cnt++;
		wd_last_miss_dir = 2;
		wd_delay_ms -= 5;
	}
	/* keep the delay in a sane range: 60..150 ms */
	if (wd_delay_ms < 60)
		wd_delay_ms = 60;
	if (wd_delay_ms > 150)
		wd_delay_ms = 150;

	wd_last_req = requlo & 0x0f;
	wd_last_ec  = (requlo >> 4) & 0x07;
	wd_int      = !!(requlo & 0x80);
	/* A rising WDA_INT edge is a watchdog kill pulse: EC crossed 4 and the
	 * chip forced its outputs off until correct answers bring it back. */
	if (wd_int && !wd_prev_int)
		wd_kill_cnt++;
	wd_prev_int = wd_int;

	/* write the expected 32-bit response: RESP_BYTE3..0 via WD_ANSW */
	const uint8_t *resp = wd_resp_table[wd_last_req];
	for (int i = 0; i < 4; i++) {
		ret = spi_frame_isr(L9779_WD_ANSW(resp[i]), NULL);
		if (ret < 0)
			break;
	}
	if (ret == 0)
		wd_ok_cnt++;
	else
		wd_fail_cnt++;

	return ret;
}

/* Executor callback (TIM5 ISR): run the ISR-safe feed, then re-arm the
 * next one. On success the monitoring cycle restarts at the end of the
 * RESP_BYTE0 write - aim for the middle of the response window. */
void L9779::wdFeedFromExecutor()
{
	/* Never run while the thread owns an SPI batch: spiStart() briefly clears
	 * SPE, and spi_lld_polled_exchange() then spins on RXNE forever inside this
	 * kernel-priority ISR (board bricked right after boot - no fuel pump, no
	 * console). Defer a couple of ms - the answer window is ~12.6 ms wide, so
	 * a 2 ms deferral can not miss it. */
	if (spi_busy) {
		wd_defer_cnt++;
		engine->scheduler.schedule("l9779wda", &wd_sched,
			getTimeNowNt() + MS2NT(2),
			action_s::make<l9779WdaFeedExec, L9779*>(this));
		return;
	}

	int ret = wd_feed_isr();

	efitick_t nextNt = getTimeNowNt() + MS2NT(ret < 0 ? 20 : wd_delay_ms);

	engine->scheduler.schedule("l9779wda", &wd_sched, nextNt,
		action_s::make<l9779WdaFeedExec, L9779*>(this));
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

		/* Direct-driven pins keep their SPI enable bit permanently set
		 * (o_oe_mask -> o_data in update_output): their CONTR bits do not
		 * change with o_state, so pushing o_dirty for them would only waste
		 * SPI bandwidth (8 pins toggling ~20 times/s each while running).
		 * Non-direct pins mirror o_state into CONTR and must be pushed. */
		if (!(OUT_DIRECT_DRIVE_MASK & BIT(pin))) {
			o_dirty = true;
		}
	}

	/* direct driven? */
	if (OUT_DIRECT_DRIVE_MASK & BIT(pin)) {
		update_direct_output(pin, value);
	}

	/* Wake the driver regardless: the SPI enable bits are permanent
	 * (o_oe_mask, see update_output), but the thread must stay informed so
	 * the CONTR registers stay consistent and the watchdog keeps a healthy
	 * bus owner. */
	return wake_driver();
}

brain_pin_diag_e L9779::getOutputDiag(size_t pin)
{
	if (pin >= L9779_OUTPUTS)
		return PIN_UNKNOWN;

	const int8_t *src = l9779_pin_diag[pin];
	if (src[0] < 0)
		return PIN_OK;	/* pin has no power-stage diagnosis */

	/* the cache is filled by the driver thread; until the first refresh
	 * there is nothing to report (SensorChecker ignores PIN_UNKNOWN) */
	if (!dia_valid[src[0]])
		return PIN_UNKNOWN;

	uint8_t d = MSG_GET_DATA(dia_cache[src[0]]);
	return l9779_diag_decode((uint8_t)((d >> src[1]) & 0x03));
}

brain_pin_diag_e L9779::getInputDiag(unsigned int pin)
{
	(void)pin;

	return PIN_OK;
}

int L9779::readPad(size_t pin) {
	if (pin >= L9779_SIGNALS)
		return -1;

	/* KEY (pin 11, KEY_ON input): level read back over SPI via DIA_REG9
	 * bit 7 (KEY_ON_STATUS), cached by the driver thread. This is how
	 * isIgnVoltage() sees the ignition switch on boards that route IGN_KEY
	 * to the L9779 instead of an MCU pin. */
	if (pin == L9779_OUTPUTS)
		return key_on_valid ? (key_on_status ? 1 : 0) : -1;

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

void L9779::debug() {
	efiPrintf(DRIVER_NAME " spi=%d parity_err=%d frame_err=%d addr_err=%d",
		spi_cnt, spi_err_parity, spi_err_frame, spi_err);
	efiPrintf(DRIVER_NAME " lastTx=0x%04x lastRx=0x%04x ident=0x%04x",
		recentTx, recentRx, ident_reg);
	efiPrintf(DRIVER_NAME " frame=%dus SPI1: CR1=0x%08x CR2=0x%08x SR=0x%08x",
		(int)(recent_frame_cycles / (SystemCoreClock / 1000000)),
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
	if (cfg->spi_config.ssport != NULL) {
		/* CS pad state: moder=1 is output, odr is the driven level and idr
		 * is what the chip actually sees (should be 1 between frames) */
		efiPrintf(DRIVER_NAME " CS: moder=%d odr=%d idr=%d",
			(int)((cfg->spi_config.ssport->MODER >> (cfg->spi_config.sspad * 2)) & 0x3),
			(int)((cfg->spi_config.ssport->ODR >> cfg->spi_config.sspad) & 1),
			(int)((cfg->spi_config.ssport->IDR >> cfg->spi_config.sspad) & 1));
	}
	efiPrintf(DRIVER_NAME " WDA: req=0x%x ec=%d wda_int=%d ok=%d fail=%d miss=%d delay=%dms",
		wd_last_req, wd_last_ec, wd_int ? 1 : 0, wd_ok_cnt, wd_fail_cnt, wd_timing_miss_cnt, wd_delay_ms);

	/* Power-stage status (DIA_REG10, datasheet section 6.14): OUT_DIS must
	 * be 0 for OUTx/IGNx to switch at all; F1/F2 report output faults,
	 * OV_RST says the stages were cut off due to battery overvoltage and
	 * VDD5_OV/V3V3_UV flag regulator problems. */
	uint16_t dia10 = 0;
	read_diag_reg(L9779_DIA_REG10_SUB, &dia10);
	uint8_t d10 = MSG_GET_DATA(dia10);
	efiPrintf(DRIVER_NAME " DIA10: OUT_DIS=%d F1=%d F2=%d OV_RST=%d VDD5_OV=%d V3V3_UV=%d TNL_RST=%d CRK_RST=%d",
		(d10 >> 1) & 1, (d10 >> 6) & 1, (d10 >> 4) & 1, d10 & 1,
		(d10 >> 3) & 1, (d10 >> 2) & 1, (d10 >> 7) & 1, (d10 >> 5) & 1);

	/* Per-channel power-stage diagnosis (datasheet 6.14): DIA_REG1 = OUT1..4
	 * (injectors), DIA_REG6/7 = OUT21..28, DIA_REG8 = IGN1..4 (coils). */
	uint16_t dreg = 0;
	if (read_diag_reg(L9779_DIA_REG1_SUB, &dreg) == 0) {
		uint8_t d = MSG_GET_DATA(dreg);
		efiPrintf(DRIVER_NAME " OUT1-4: OUT1:%s OUT2:%s OUT3:%s OUT4:%s",
			l9779_diag_str((d >> 0) & 3), l9779_diag_str((d >> 2) & 3),
			l9779_diag_str((d >> 4) & 3), l9779_diag_str((d >> 6) & 3));
	}
	if (read_diag_reg(L9779_DIA_REG8_SUB, &dreg) == 0) {
		uint8_t d = MSG_GET_DATA(dreg);
		efiPrintf(DRIVER_NAME " IGN1-4: IGN1:%s IGN2:%s IGN3:%s IGN4:%s",
			l9779_diag_str((d >> 0) & 3), l9779_diag_str((d >> 2) & 3),
			l9779_diag_str((d >> 4) & 3), l9779_diag_str((d >> 6) & 3));
	}
	if (read_diag_reg(L9779_DIA_REG6_SUB, &dreg) == 0) {
		uint8_t d = MSG_GET_DATA(dreg);
		efiPrintf(DRIVER_NAME " OUT21-24: OUT21:%s OUT22:%s OUT23:%s OUT24:%s",
			l9779_diag_str((d >> 0) & 3), l9779_diag_str((d >> 2) & 3),
			l9779_diag_str((d >> 4) & 3), l9779_diag_str((d >> 6) & 3));
	}
	if (read_diag_reg(L9779_DIA_REG7_SUB, &dreg) == 0) {
		uint8_t d = MSG_GET_DATA(dreg);
		efiPrintf(DRIVER_NAME " OUT25-28: OUT25:%s OUT26:%s OUT27:%s OUT28:%s",
			l9779_diag_str((d >> 0) & 3), l9779_diag_str((d >> 2) & 3),
			l9779_diag_str((d >> 4) & 3), l9779_diag_str((d >> 6) & 3));
	}

	/* Ignition key input level (KEY_ON_STATUS, DIA_REG9 bit 7) */
	uint16_t dreg9 = 0;
	if (read_diag_reg(L9779_DIA_REG9_SUB, &dreg9) == 0) {
		efiPrintf(DRIVER_NAME " KEY_ON_STATUS=%d", (MSG_GET_DATA(dreg9) >> 7) & 1);
	}

	/* dump the last SPI exchanges - protocol debugging */
	dbg_print_frames();
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

/* Configure the flying-wheel (VRS) sensor interface for fully adaptive
 * operation (datasheet 6.14). The power-on defaults are limited adaptive
 * mode (CONFIG_REG1 reset 0x08: VRS_mode = 0) with the auto-adaptive
 * temporal filter switched OFF (CONFIG_REG5 reset 0xd8: VRS_MODE = 01)
 * and a 17 uA hysteresis floor. In that mode the interface behaves almost
 * like a plain fixed-threshold comparator: noise bursts pass through to
 * OUT_VRS and false-sync the crank decoder during cranking.
 *
 * Fully adaptive mode scales the hysteresis with the actual sensor
 * amplitude (peak detector + 5-level quantizer) and enables the adaptive
 * masking filter - the way the stock ECU conditions the same sensor
 * through the same chip.
 *
 * CONFIG_REG5 bit5 doubles as the VRS diagnosis enable in this mode
 * (open/short detection of the sensor, datasheet 6.14).
 */
/*
 * Stock VRS hysteresis ramp, extracted from the Lada M74 stock firmware
 * (Read_FULLFLASH_I865LB52_w2404b1____(240626_103727).bin, the L9779
 * "IC_EMS" module, config script at 0x4EF8C):
 *   REG1: 0x02 (full adaptive only)
 *   REG5: 0x0C -> 0x0D -> 0x0E -> 0x0F  (VRS_HYST 100..111)
 *   REG4: 0x0B -> 0x0A -> 0x09 -> 0x08
 *   REG6: 0x07 -> 0x05 -> 0x06
 * The stock steps the hysteresis floor up as the cranking signal amplitude
 * grows and ends at VRS_HYST 111, handing the conditioner over to the
 * fully-adaptive loop once the amplitude is established.
 *
 * Step 0 is the ramp START (low floor - low cranking signal amplitude),
 * written by vrs_configure() at init and on every start re-arm; steps 1..3
 * are advanced by rpm thresholds from the driver thread.
 */
static const uint8_t vrs_ramp_cfg4[] = { 0x0b, 0x0a, 0x09, 0x08 };
static const uint8_t vrs_ramp_cfg5[] = { 0x0c, 0x0d, 0x0e, 0x0f };
static const uint8_t vrs_ramp_cfg6[] = { 0x07, 0x05, 0x05, 0x06 };

int L9779::vrs_configure(void)
{
	/* Full adaptive mode + the stock's ramp START. */
	static const uint8_t cfg1 = 0x02;

	int ret = spi_rw(MSG_W(0x01, cfg1), NULL);
	if (ret)
		return ret;
	ret = vrs_ramp_to_step(0);
	if (ret)
		return ret;

	efiPrintf(DRIVER_NAME " VRS: stock ramp start (REG1=0x%02x REG4=0x%02x REG5=0x%02x REG6=0x%02x)", cfg1, vrs_ramp_cfg4[0], vrs_ramp_cfg5[0], vrs_ramp_cfg6[0]);
	return 0;
}

int L9779::vrs_ramp_to_step(int step)
{
	/* 0 = ramp start, 3 = ramp end (maximum floor -> fully-adaptive
	 * handover). Steps 1..3 mirror the stock config script. */
	if (step < 0 || step > 3)
		return -1;

	int ret = spi_rw(MSG_W(0x04, vrs_ramp_cfg4[step]), NULL);
	if (ret)
		return ret;
	ret = spi_rw(MSG_W(0x05, vrs_ramp_cfg5[step]), NULL);
	if (ret)
		return ret;
	ret = spi_rw(MSG_W(0x06, vrs_ramp_cfg6[step]), NULL);
	if (ret)
		return ret;

	efiPrintf(DRIVER_NAME " VRS: stock ramp step %d (REG4=0x%02x REG5=0x%02x REG6=0x%02x)", step, vrs_ramp_cfg4[step], vrs_ramp_cfg5[step], vrs_ramp_cfg6[step]);
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

	/* Enable the power stages: the START command (frame bit 2 = data bit 1,
	 * datasheet Table 57) clears OUT_DIS. With OUT_DIS = 1 all control
	 * register writes are ignored and the power stages stay off. */
	ret = spi_rw(CMD_START_REACT(BIT(1)), NULL);
	if (ret)
		return ret;

	/* Enable the flying-wheel interface in fully adaptive mode before the
	 * engine can run: the reset defaults let VR noise storms through. */
	ret = vrs_configure();
	if (ret)
		return ret;

	/* Verify that START really cleared OUT_DIS. OUT_DIS lives in DIA_REG10
	 * (bit 1, datasheet section 6.14): with OUT_DIS = 1 all control
	 * register writes are ignored and the power stages stay off. */
	uint16_t dia10 = 0;
	bool got10 = read_diag_reg(L9779_DIA_REG10_SUB, &dia10) == 0;

	if (!got10) {
		efiPrintf(DRIVER_NAME " DIA_REG10 read failed (OUT_DIS unknown)");
	} else if (MSG_GET_DATA(dia10) & 0x02) {
		efiPrintf(DRIVER_NAME " OUT_DIS still set after START (DIA_REG10=0x%02x)", MSG_GET_DATA(dia10));
	} else {
		efiPrintf(DRIVER_NAME " OUT_DIS cleared (DIA_REG10=0x%02x)", MSG_GET_DATA(dia10));
	}

	return 0;
}

int L9779::init()
{
	int ret;

	/* check for multiple init */
	if (drv_state != L9779_WAIT_INIT)
		return -1;

	/* Configure the SPI chip-select as a push-pull output, idle high.
	 * This ChibiOS SPI HAL (SPI_SELECT_MODE_PAD) only toggles the CS pad
	 * level and never sets its pad mode; an unconfigured pad floats, the
	 * L9779 sees CS permanently asserted, its 16-bit SCK counter never
	 * gets a CS edge to frame the transfer and every frame is rejected
	 * with SPI_ERR (DO = 0x8000). Must happen before the first frame. */
	if (cfg->spi_config.ssport != NULL) {
		gpio_pin_markUsed(cfg->spi_config.ssport, cfg->spi_config.sspad, DRIVER_NAME " CS");
		palSetPadMode(cfg->spi_config.ssport, cfg->spi_config.sspad, PAL_MODE_OUTPUT_PUSHPULL);
		palSetPad(cfg->spi_config.ssport, cfg->spi_config.sspad);
	}

	/* reset the chip; a bad first frame on the DO line (pre-reset idle
	 * state) is not fatal - the link is verified below */
	chip_reset();

	ret = chip_init_data();
	if (ret)
		return ret;

	/* Verify the SPI link by reading the identification register. The DO
	 * reply to a read arrives in one of the frames that follow the request,
	 * so issue the read three times and use the last reply: this captures
	 * the IDENT_REG content with a one or two frame reply delay. */
	uint16_t rx;
	spi_rw(L9779_IDENT, NULL);
	spi_rw(L9779_IDENT, NULL);
	if (spi_rw(L9779_IDENT, &rx) == 0) {
		ident_reg = rx;
		efiPrintf(DRIVER_NAME " IDENT_REG = 0x%02x", MSG_GET_DATA(ident_reg));
	} else {
		efiPrintf(DRIVER_NAME " IDENT read failed: SPI link problem?");
	}

	/* WDA watchdog: with the default RESPTIME (0x3f) the response time is
	 * ~99 ms and the fixed answer window is ~12.6 ms. The feed is kicked
	 * by the driver thread after chip_init and then runs on the TIM5
	 * executor; the delay is adapted from REQUHI flags.
	 *
	 * 115 ms is the MEASURED window-center delay on m74_9 (16:21:41 log:
	 * starting from 105 the first answers landed BEFORE the window, the
	 * loop stepped 5 ms per miss and locked at delay=115 with zero further
	 * misses over 206 answers). Starting locked avoids the boot-convergence
	 * misses entirely - each such miss sets EC > 4 and fires a WDA kill
	 * pulse (kills=2 observed: the second pulse hit a running engine). */
	wd_delay_ms = 115;
	wd_running = false;
	spi_busy = false;
	spi_configured = false;
	wd_defer_cnt = 0;
	wd_kill_cnt = 0;
	wd_last_miss_dir = 0;
	wd_prev_int = false;

	/* power-stage diagnosis cache: nothing valid until the driver thread
	 * performs the first refresh (diag_ts = 0 -> immediate) */
	for (int i = 0; i < 8; i++)
		dia_valid[i] = false;
	key_on_valid = false;
	diag_ts = 0;
	diag_next_reg = 0;
	diag_pending = 0;
	out_dis_latched = false;
	out_dis_clear_tried = false;

	/* force chip init from driver thread */
	need_init = true;

	/* instance is ready */
	drv_state = L9779_READY;

	/* init semaphore */
	chSemObjectInit(&wake, 10);

	/* start thread.
	 * Priority matters: the VDA 2.0 answer window is only ~12.6 ms wide
	 * (default RESPTIME), and a response outside the window increments the
	 * chip error counter - EC > 4 forces OUT1..4 + IGN1..4 off in hardware
	 * (observed on the car: the cranking workload at main-loop/ADC/ETB/CAN-RX
	 * priorities preempted the PRIO_GPIOCHIP thread past the window, EC pegged
	 * at 7 with wda_int set - no fuel/spark while every ECU-side counter
	 * looked healthy, and the bench worked again as soon as cranking stopped).
	 * Per-cycle work is a handful of 16-bit SPI frames, so running above all
	 * of those threads costs a negligible CPU fraction. */
	thread = chThdCreateStatic(thread_wa, sizeof(thread_wa),
											NORMALPRIO + 12, l9779_driver_thread, this);

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
