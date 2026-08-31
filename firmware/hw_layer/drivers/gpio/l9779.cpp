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
 * short chunks so the WDA ISR's exchange is deferred for at most a
 * few frames (see spi_busy). */
#define DIAG_REFRESH_REGS			(3)

/* WDA response timing. RESPTIME=10 is written at init for a DETERMINISTIC
 * short window: response (1+101*10)/f_clk = 15.8 ms @ 64 kHz (25.9 ms @
 * 39 kHz), window [resp, resp+12.6] = [15.8, 28.4] ms @ 64 kHz / [25.9,
 * 38.5] ms @ 39 kHz. These are the PROVEN values from the working executor
 * feed (ce32509e, car-validated 2026-08-24/26: ec=4 wda_int=0): the same
 * +-5% CLK1 drift that moved the default-RESPTIME window by +-5 ms now
 * moves this short window by only +-0.8 ms, so a centered feed essentially
 * cannot miss, and any transient recovers ~5x faster.
 *
 * The feed runs one atomic burst per cycle on the free-running TMR7 timer
 * (priority 5, BELOW the trigger handoff - see EFI_IRQ_L9779_WDA_PRIORITY).
 * The delay
 * is adapted ONLY by the REQUHI timing verdicts (+-5 ms per miss, the same
 * policy as the proven executor feed) - there is NO delay walk: the values
 * stay clamped to the window below.
 *
 * The WDA kill is real: EC > 4 -> WDA_INT -> the L9779 forces OUT1-4/IGN1-4
 * off AND pulls WDA low -> Q5B -> TLE9201 DIS -> the blade dies. With no
 * loads the bench never showed it; with the injectors/coils/pump connected
 * everything went dead. SPI stays clean throughout - the kill is a
 * chip-internal state, not an SPI fault (the output diagnosis 'Ok' also
 * does not reflect it). */
#define WDA_RESPTIME				(10)
/* BYTE0-to-BYTE0 answer period. 27 ms sits INSIDE the RESPTIME=10 window on
 * BOTH chip clock rates: [15.8, 28.4] ms @ 64 kHz (the car chip - 22 ms was
 * the window center there) and [25.9, 38.5] ms @ 39 kHz (the bench chip).
 * 22 ms was the 64 kHz center but lands BEFORE the 39 kHz window opens - the
 * 21:46 session proved it: RESPTIME readback 0x0A (the write takes effect),
 * delay 22, miss=0 wrong=0, yet the question froze for 30+ cycles at a time
 * (silent early rejection, reqhi=0xC0) and EC stayed pinned at 7 with the
 * power stage killed. 27 ms is accepted by both rates with +-5% CLK1 drift
 * margin, so no delay walking is needed. */
#define WDA_DELAY_INIT_MS			(27)
/* Fixed adaptation range: covers the RESPTIME=10 window wherever the chip's
 * f_clk sits, [15.8, 28.4] ms @ 64 kHz and [25.9, 38.5] ms @ 39 kHz, plus the
 * +-5% CLK1 drift. The REQUHI verdicts (NO_RESP / RESP_TO_EARLY, +-5 ms
 * steps) walk the delay inside this range; the clamps keep a misread verdict
 * stream from running away. */
#define WDA_DELAY_MIN_MS			(17)
#define WDA_DELAY_MAX_MS			(38)
/* EC-saturation escape: when the chip's error counter stays pinned at the
 * top for this many consecutive clean on-time cycles, the feed is in a
 * SILENT-REJECTION zone (the answer period sits outside the window, so the
 * chip's verdict flags are cleared by its next sequencer run before our
 * read - reqhi stays 0xC0 and the REQUHI servo can never walk the delay
 * back). Step the delay toward WDA_DELAY_INIT_MS (27 ms - inside BOTH
 * candidate windows, proven healthy: the 12:41 bench run held EC=0 there).
 * The 12:42 state (delay walked 27->32 on one pins-perturbed verdict, EC=7,
 * reqhi=0xC0 forever) is exactly what this recovers from. */
#define WDA_EC_SAT_ESCAPE_CYCLES	(8)
/* Duration of the feed ISR from dispatch to the END of its RESP_BYTE0 write
 * (3 pipelined reads + 4 answer frames, ~7 x 10 us, plus any handoff
 * preemption). The period is set this much short of the full
 * answer period so the BYTE0 lands at the window center; the exact value is
 * irrelevant against the 12.6 ms window. */
#define WDA_BURST_LEAD_US			(80)
/* CONFIG_REG9 (SPI RESPTIME) register address, per the datasheet register
 * descriptions: REG5=0x05, REG6=0x06, REG7=0x07, REG8/WD_ANSW=0x0e,
 * REG9/RESPTIME=0x11, REG10/CPS=0x12 (the summary table's 0x07 row is a
 * misaligned REG7 duplicate). Writing 0x07 hits CONFIG_REG7 (output enable
 * latches) and does NOT change the response time - observed 2026-08-24
 * 17:28: every answer missed EARLY with the period pegged at the 27 ms
 * clamp. */
#define L9779_WD_RESPTIME_REG		(0x11)

/* CONFIG_REG6 (power management + WDA time base, datasheet 6.15) - the
 * value applied ONCE at init and re-applied after a chip reset. Bit map:
 *   [5] PWL_EN_N = 0: power latch ENABLED (the stock's choice - keeps VCC
 *       held across quick key cycles on m74_9)
 *   [4] PSOFF = 0
 *   [3] VDD5_UV RST mask = 0 (default): a long VDD5 undervoltage generates
 *       RST (CRK_RST) - safety, keep it
 *   [2] VDD5_UV WDA mask = 1: a VDD5 undervoltage does NOT pull WDA low -
 *       avoids blade kills on cranking rail dips (the stock's choice)
 *   [1] WDA time base = 1 (64 kHz) - REQUIRED: the stock's steady-state
 *       choice; the chip's WDA window (~[99, 112] ms with the default
 *       RESPTIME) scales from f_clk. NOTE the chip may ignore this bit
 *       (the 39 kHz bench observations) - the EC walk finds the window
 *       either way.
 *   [0] PWL/SEO timeout priority = 0 (default)
 * 0x06 is exactly the stock's steady-state value (the stock steps this
 * register 0x07 -> 0x05 -> 0x06 during its config script - but for rusEFI
 * the time base must never flip; it is applied ONCE and never stepped). */
#define L9779_CONFIG6_PWR			(0x06)
/* CONFIG_REG6 with PSOFF (bit 4) set: power stages off, chip logic +
 * regulators + SPI + WDA monitoring + KEY_ON input all stay alive. Written
 * by the ignition gate on key-off; the MCU keeps running and isIgnVoltage()
 * keeps seeing the key via DIA_REG9. The next ignition-on re-init (SW_RST
 * via need_init) wipes it and reapplies L9779_CONFIG6_PWR. */
#define L9779_CONFIG6_PSOFF			(0x16)

/* DIA_REG10 (datasheet 6.14) bits, verified against the register layout:
 * [7] TNL_RST, [6] F1, [5] CRK_RST, [4] F2, [3] VDD5_OV, [2] V3V3_UV,
 * [1] OUT_DIS, [0] OV_RST. F1/F2 are output-fault flags (0x50 is the
 * steady-state value - they do NOT indicate a reset); TNL_RST/OV_RST/
 * CRK_RST are generated by the smart-reset unit and mean the chip asserted
 * RST: all configuration registers are wiped and OUT_DIS goes to 1.
 * VDD5_OV/V3V3_UV report rail faults that cut the drivers WITHOUT a config
 * wipe (datasheet: 'switches off the drivers... to switch on again send the
 * START command and write the CONTROL registers'). */
#define L9779_DIA10_OV_RST			0x01
#define L9779_DIA10_OUT_DIS			0x02
#define L9779_DIA10_V3V3_UV			0x04
#define L9779_DIA10_VDD5_OV			0x08
#define L9779_DIA10_F2				0x10
#define L9779_DIA10_CRK_RST			0x20
#define L9779_DIA10_F1				0x40
#define L9779_DIA10_TNL_RST			0x80
/* reset events: the chip asserted RST, configuration registers are gone */
#define L9779_DIA10_RESET_EVENTS	(L9779_DIA10_TNL_RST | L9779_DIA10_CRK_RST | L9779_DIA10_OV_RST)

/* OUT_DIS heal rate limit. A persisting fault re-latches OUT_DIS, and the
 * heal re-applies START (+ RESPTIME + VRS on a reset event); at the ~100 ms
 * DIA10 refresh cadence this bounds the heal to ~5/s. Each full heal costs
 * one EC and re-anchors the WDA cycle, so do not make it faster. */
#define OUT_DIS_HEAL_MS				(200)

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
#define L9779_WD_RESPTIME_SUB			0x0d	/* WDA response-time readback (DIA_REG13) */
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
	int chip_heal_out_dis(bool configWiped);
	int chip_power_off();
	int vrs_configure();
	/* ISR-safe polled SPI (TMR7 WDA ISR context, no bus mutex) */
	int spi_frame_isr(uint16_t tx, uint16_t *rx_ptr);
	void wdFeedFromExecutor();
	/* Set the TMR7 period for the next WDA burst (ISR and thread) */
	void wdArmIsr(int delayMs);
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
	/* The WDA feed runs as a FREE-RUNNING auto-reload TMR7 ISR (priority 5,
	 * the TIM5 pattern): the answer must land inside the chip's ~12.6 ms
	 * window, and a thread wakeup can be delayed past that by trigger-decode
	 * ISR load at cranking (the observed wd_timing_miss/EC>4 kills). TMR7
	 * runs BELOW the trigger handoff on purpose: the old executor feed
	 * (priority 3) preempted the handoff and delayed decode/scheduling by up
	 * to ~100 us per cycle. The 12.6 ms window absorbs any handoff preemption
	 * of the burst.
	 *
	 * ONE burst per monitoring cycle: pipelined status/question reads
	 * (REQUHI/REQULO), answer-period adaptation, then the whole 4-byte
	 * response (RESP_BYTE3..0) as a single atomic burst positioned so
	 * RESP_BYTE0's END lands at the window center. The burst must NOT be
	 * split: the chip tracks the response progress in RESP_CNT and compares
	 * every byte against the expected one for the CURRENT position - a byte
	 * landing one position late (the old two-phase prepare/BYTE0 scheme)
	 * desynchronizes the stream permanently and pins EC at 7.
	 *
	 * The timer is started ONCE (driver thread, after chip_init) and never
	 * stopped between fires (the TIM5 pattern - free-run with auto-reload,
	 * fire on UIF). The 2026-08-31 2x-period mystery was NOT the one-shot's
	 * stop/re-arm cycle: it was the SLEEP-MODE CLOCK GATE - rccEnableTIM7
	 * was called with lp=false, which on the AT32 port clears the APB1LPENR
	 * bit and gates the timer clock off whenever the CPU sleeps (the pins
	 * cross-measurement read exact because its busy-wait keeps the CPU
	 * awake). lp=true is now set; the free-run stays as the principled
	 * structure (no per-cycle stop, no UG spurious-fire hazard, the thread's
	 * !CEN heal can only fire on a genuinely dead peripheral). The period
	 * is changed only when the delay actually changes (wdTimerArm's ARR + UG
	 * latch). */
	efitick_t					wd_next_moment;	/* next burst dispatch, for the thread's burst-imminent check */
	bool						wd_running;
	int						wd_poll_timeouts; /* polled-SPI frames that hit the RXNE watchdog (diagnostic) */
	int						wd_ok_cnt;		/* cycles answered correctly */
	int						wd_fail_cnt;	/* cycles missed (SPI-level failures) */
	int					wd_timing_miss_cnt;	/* responses outside the window (REQUHI flags) - the EC climbs on these too */
	int					wd_ec_sat_cycles;	/* consecutive clean on-time cycles with EC >= 6 (the escape arming counter) */
	int						wd_wrong_cnt;	/* responses rejected on VALUE (REQUHI W_RESP) */
	int						wd_cnt_bad;		/* RESP_CNT != 11 at read time - the answer stream desynced */
	uint8_t					wd_last_requhi;	/* raw REQUHI byte of the last cycle */
	int						wd_defer_cnt;	/* feeds deferred by the spi_busy flag */
	int						wd_kill_cnt;	/* WDA_INT rising edges (watchdog kill pulses) */
	int						wd_last_miss_dir;	/* last REQUHI miss: 1=early 2=late */
	bool					wd_prev_int;
	int					wd_bad_value_cnt; /* consecutive cycles with W_RESP/RESP_Z0/RESP_ERR - diagnostic only, NO reset action (the burst self-realigns) */
	bool					wd_prev_cycle_clean; /* the previous burst went out as one clean atomic stream - REQUHI verdicts are only trusted when this is true */
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
	 * sets spi_busy around its frames and the WDA ISR exchange defers
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

	/* Ignition-gated power stage (l9779_setPowerStage): power_stage_on is
	 * written by the board's periodic callback (SysTick ISR) and read by the
	 * driver thread - volatile, single-word, one core. power_stage_applied is
	 * what the thread has actually applied to the chip. OFF transition =
	 * chip_power_off() (PSOFF + WDA feed stop); ON transition = need_init
	 * (full SW_RST re-init). Both default true so boards that never call the
	 * gate keep today's behavior. power_stage_on must NOT be reset in init():
	 * m74_9 sets it false before the driver thread starts. */
	volatile bool				power_stage_on = true;
	bool						power_stage_applied = true;

	/* WDA-ISR/thread SPI serialization. The WDA feed runs in the TMR7 ISR -
	 * chSysLock/CriticalSectionLocker do NOT mask it (kernel priority), so a
	 * critical section does not stop the ISR from preempting the driver
	 * thread mid-batch. spi_busy is set for the whole thread-side batch
	 * (spi_rw/spi_rw_array); the WDA ISR checks it and defers. This matters
	 * because spiStart() briefly clears SPE (spi_lld_start re-programs CR1),
	 * and the polled exchange busy-waits on RXNE - with the peripheral
	 * disabled it spins forever inside the ISR and the board bricks right
	 * after boot (no fuel pump, no console). */
	volatile bool				spi_busy;
	/* CR1/CR2 persist from the first spiStart; re-running it on every batch
	 * only re-opens the SPE=0 window, so spi_rw/spi_rw_array call it once. */
	bool						spi_configured;

	/* OUT_DIS latch (DIA_REG10 bit 1): the chip disables OUT1..4/IGN1..4 and
	 * only the START command clears it. Tracked by refresh_diag_cache() so a
	 * latch event is logged WITH its fault flags (reading DIA_REG10 clears
	 * them) and healed promptly, rate-limited by out_dis_heal_ts: a chip
	 * reset (TNL_RST/OV_RST/CRK_RST) wiped the whole config, so START +
	 * CONFIG_REG6 + RESPTIME + the VRS full-adaptive config + the output
	 * registers are re-applied (the datasheet's recovery recipe); a plain
	 * driver cut (VDD5_OV/V3V3_UV/output faults, no reset) keeps its config
	 * and only needs START + CONTR. */
	bool						out_dis_latched;
	/* Last OUT_DIS heal attempt - the heal is retried while the latch
	 * persists (a first attempt can land while the chip is still resetting),
	 * rate-limited so a persisting fault does not SPI-flood and does not
	 * re-anchor the WDA cycle too often (each RESPTIME write costs one EC). */
	systime_t					out_dis_heal_ts;
	/* VRS conditioner: configured once (vrs_configure) into fully adaptive
	 * mode - auto hysteresis + auto time filter both adapt to rpm inside the
	 * chip, no software ramp. Re-applied by chip_init() and
	 * chip_heal_out_dis() (a chip reset reverts the write-only registers). */


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
		int *delay_ms, int *defer_cnt, int *kill_cnt, uint8_t *requhi, int *wrong_cnt, int *cnt_bad)
{
	/* WDA counters are written by the TMR7 feed ISR; the reads below
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
	if (requhi)
		*requhi = chip->wd_last_requhi;
	if (wrong_cnt)
		*wrong_cnt = chip->wd_wrong_cnt;
	if (cnt_bad)
		*cnt_bad = chip->wd_cnt_bad;
	return true;
}

void l9779_setPowerStage(bool on)
{
	L9779 *chip = &chips[0];

	chip->power_stage_on = on;

	/* The wake semaphore only exists once init() has created the driver
	 * thread; before that the thread picks the flag up on its first pass. */
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

	/* CONTENT-ADDRESSED matching (2026-08-30 night): the chip's reply
	 * stream can shift by one reply (the chip skips a reply while its SPI
	 * block is busy - the 20:20 bench session proved it: 23% addr_err, the
	 * feed reading REQUHI bytes (0xC0/0xC2) as REQULO, fail flood, all
	 * without any reset). Matching replies to requests BY POSITION (the
	 * FIFO pop) turns a single skipped reply into a PERMANENT off-by-one:
	 * every subsequent reply pops the wrong entry. Matching by the reply's
	 * OWN sub-address field self-heals the shift on the first mismatch:
	 * the reply finds its request wherever it sits in the queue, and a
	 * reply for an unknown/consumed request is simply dropped (no pop), so
	 * the queue can never be misaligned again. */
	uint8_t reply_sub = MSG_GET_ADDR(rx);
	for (int i = 0; i < rd_pending_cnt; i++) {
		int idx = (rd_pending_head + i) % (int)efi::size(rd_pending);
		if (rd_pending[idx] != reply_sub)
			continue;

		/* remove the matched entry, shifting the tail down one slot */
		for (int j = i; j + 1 < rd_pending_cnt; j++) {
			int a = (rd_pending_head + j) % (int)efi::size(rd_pending);
			int b = (rd_pending_head + j + 1) % (int)efi::size(rd_pending);
			rd_pending[a] = rd_pending[b];
		}
		rd_pending_cnt--;

		rx_subaddr = reply_sub;
		return 0;
	}

	/* no outstanding request carries this sub-address: drop the reply
	 * without popping anything (a stale reply after a chip reset, or a
	 * shift already healed) */
	spi_err++;
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

	/* WDA-ISR exclusion (see the spi_busy comment in the struct): the WDA
	 * feed defers while any thread-side batch is in flight, so the ISR
	 * can never land in spiStart()'s SPE=0 window. */
	spi_busy = true;

	/* Acquire ownership of the bus. */
	spiAcquireBus(spi);
	/* Setup transfer parameters - first call only: the config never changes
	 * and the bus is dedicated to this chip. Re-running spiStart() on every
	 * batch re-executes spi_lld_start(), which clears SPE before re-enabling
	 * it - a window in which the polled exchange (used by the WDA ISR
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

	/* WDA-ISR exclusion, same as spi_rw(). */
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
 * spi_rw) keeps the WDA ISR exchange out of the batch. A failed
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
				bool out_dis = (d10 & L9779_DIA10_OUT_DIS) != 0;

				if (out_dis && !out_dis_latched) {
					efiPrintf(DRIVER_NAME " OUT_DIS set! DIA10=0x%02x (F1=%d F2=%d OV_RST=%d VDD5_OV=%d V3V3_UV=%d TNL_RST=%d CRK_RST=%d)",
						d10, (d10 >> 6) & 1, (d10 >> 4) & 1, d10 & 1,
						(d10 >> 3) & 1, (d10 >> 2) & 1, (d10 >> 7) & 1, (d10 >> 5) & 1);
				} else if (!out_dis && out_dis_latched) {
					efiPrintf(DRIVER_NAME " OUT_DIS cleared (DIA10=0x%02x)", d10);
				}

				/* Heal the latch promptly (rate-limited, see OUT_DIS_HEAL_MS).
				 * The old one-shot gate required !fault_flags && !wd_int and
				 * blocked the heal on ANY flag - F1/F2 are set in the steady
				 * state (0x50), so a real power event (the 18:04 DIA10=0x8F)
				 * stayed latched ~574 ms and the engine died before the EC=7
				 * path recovered it. Now: a reset event re-applies the whole
				 * config (a chip reset wipes RESPTIME/VRS/CONTR - without it the
				 * WDA feed misses the ~112 ms default window every cycle and
				 * the VRS conditioner runs unfiltered at speed); a driver cut
				 * keeps its config and only needs START + CONTR.
				 * Gated on power_stage_on: chip_heal_out_dis(true) writes
				 * REG6=0x06 (PSOFF=0) and would fight the ignition gate while
				 * parked; the latch heals anyway on the next ignition-on
				 * re-init (SW_RST wipes it). */
				if (out_dis && power_stage_on) {
					systime_t now = chVTGetSystemTimeX();
					if (now - out_dis_heal_ts >= TIME_MS2I(OUT_DIS_HEAL_MS)) {
						out_dis_heal_ts = now;
						bool configWiped = (d10 & L9779_DIA10_RESET_EVENTS) != 0;
						if (chip_heal_out_dis(configWiped) == 0) {
							efiPrintf(DRIVER_NAME " OUT_DIS heal: %s (DIA10=0x%02x)",
								configWiped ? "config re-applied (START+CONFIG6+RESPTIME+VRS+CONTR)"
								            : "re-issued START+CONTR", d10);
						}
					}
				} else {
					out_dis_heal_ts = 0;
				}

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

	/* The chip's reply pipeline is gone with the reset: drop the outstanding
	 * read requests too, otherwise the post-reset replies are matched against
	 * stale queue entries and every frame misattributes for up to the queue
	 * depth - the addr_err flood + REQULO-miss fail storm that followed every
	 * SW_RST on the 2026-08-30 19:26 bench session. */
	rd_pending_cnt = 0;
	rd_pending_head = 0;

	chThdSleepMilliseconds(3);

	return 0;
}

/*==========================================================================*/
/* Driver thread.								   	*/
/*==========================================================================*/

/* The WDA feed runs in a FREE-RUNNING auto-reload TMR7 ISR, see the
 * wd_running/wd_next_moment comment in the struct. Single chip instance on
 * this board. */
static L9779 *s_wda_chip;

/* Direct register access to TMR7, NOT the ChibiOS GPT driver: the GPT
 * build bricked the bench ECU at boot (flash verified, then no console link)
 * and the driver API adds nothing here - a periodic update event needs no
 * state machine, no kernel locks and no asserts. The pattern is the proven
 * angle clock (TMR2) one: plain register writes, a bare Vector handler,
 * nvicEnableVector at init.
 *
 * WHY TMR7 (APB1) AND NOT TMR10 (APB2): the TMR10 saga's "APB2 clock
 * drifts 144..117 MHz run to run" reading was NOT an APB2 clock problem -
 * it was the SAME sleep-mode clock gate (rccEnableTIM10(false) cleared the
 * APB2LPENR bit, so the counter only advanced while the CPU was awake; the
 * effective rate tracked the console-poll wake duty and looked like drift).
 * TMR7 is kept because it shares the APB1 timer clock with TIM5 (the NT
 * domain, physics-validated 4 MHz) - the same domain the proven TMR2 angle
 * clock measures clean - and TIM7 is free (GPT FALSE, no PWM use), has its
 * own vector (IRQ 55) and its 16-bit ARR covers 262 ms at 250 kHz.
 *
 * CLOCK: the counter rate is MEASURED at init against the NT domain (TIM5):
 * newPsc = acDelta * 144 * 16 / ntDelta - 1 makes TMR7 tick at exactly
 * NT/16 = 250 kHz (4 us) BY CONSTRUCTION, because TMR7 and TIM5 share the
 * same APB1 clock. No absolute clock assumption survives - a wrong APB1
 * rate cancels out of the ratio. The 4 us tick is then fixed, so the arm
 * uses a plain intervalUs/4 tick count (no runtime re-calibration - that
 * was the patch for the drifting APB2 clock, not needed here). */
#define WDA_TIMER			TIM7
/* Provisional prescaler for the rate measurement: 2 MHz ticks under the
 * measured 288 MHz APB1 clock (PSC 143) - 20000 counts over the 10 ms NT
 * window, below the 16-bit wrap. */
#define WDA_TIMER_PSC_PROV	(143)

static void wdaTimerInit() {
	/* lp=true is LOAD-BEARING on the AT32 port: rccEnableTIMx(false) CLEARS
	 * the APB1LPENR bit (the fork's rccEnableAPB1 macro), which gates the
	 * timer clock OFF in SLEEP mode. ChibiOS idles (WFI) constantly on an
	 * idle bench, so the counter only advanced during CPU-awake windows -
	 * the per=2.1..2.6x armed-interval mystery of the whole TMR10/TMR7 saga
	 * (the effective rate tracked the console-polling wake duty, which is
	 * why it looked like run-to-run drift). TIM5 keeps running in sleep
	 * because the PWM LLD enables it with lp=true; the masked pins
	 * cross-measurement always read the exact 250 kHz because its busy-wait
	 * keeps the CPU awake. Keep the bit set so the feed period is exact
	 * while the CPU sleeps. */
	rccEnableTIM7(true);

	WDA_TIMER->PSC = WDA_TIMER_PSC_PROV;
	WDA_TIMER->ARR = 0xFFFF;
	WDA_TIMER->CR1 = 0;				/* stopped */
	WDA_TIMER->DIER = 0;
	WDA_TIMER->EGR = STM32_TIM_EGR_UG;	/* latch PSC/ARR (also sets UIF) */
	WDA_TIMER->SR = 0;				/* clear the UG-generated UIF */

	/* Measure the counter rate against the NT domain (TIM5, the shared APB1
	 * clock): free-run for 10 ms of NT time and divide the deltas. Because
	 * TMR7 and TIM5 share the APB1 timer clock, the RATIO is exact - the
	 * PSC lands on a 250 kHz tick (NT/16) whatever the absolute APB1 rate
	 * is. The old TMR10 saga's "the measurement is circular" concern does
	 * not apply here: circularity only matters when the measured timer and
	 * the reference run on DIFFERENT clock domains. */
	WDA_TIMER->CR1 = STM32_TIM_CR1_CEN;
	uint32_t ac0 = WDA_TIMER->CNT;
	efitick_t nt0 = getTimeNowNt();
	do { } while (getTimeNowNt() - nt0 < MS2NT(10));
	uint32_t acDelta = WDA_TIMER->CNT - ac0;
	uint32_t ntDelta = getTimeNowNt() - nt0;
	WDA_TIMER->CR1 = 0;

	uint32_t newPsc = (uint32_t)(((uint64_t)acDelta * (WDA_TIMER_PSC_PROV + 1) * 16) / ntDelta) - 1;
	if (newPsc > 0xFFFF)
		newPsc = 0xFFFF;

	WDA_TIMER->PSC = newPsc;
	WDA_TIMER->CNT = 0;
	WDA_TIMER->EGR = STM32_TIM_EGR_UG;	/* latch the measured PSC */
	WDA_TIMER->SR = 0;

	/* SECOND measurement at the FINAL PSC: verifies the latch and the
	 * runtime rate. A correct setup prints ~2500 counts over ~10 ms of NT
	 * (250 kHz = 4 us ticks, NT/16). */
	WDA_TIMER->CR1 = STM32_TIM_CR1_CEN;
	ac0 = WDA_TIMER->CNT;
	nt0 = getTimeNowNt();
	do { } while (getTimeNowNt() - nt0 < MS2NT(10));
	uint32_t acDelta2 = WDA_TIMER->CNT - ac0;
	uint32_t ntDelta2 = getTimeNowNt() - nt0;
	WDA_TIMER->CR1 = 0;
	WDA_TIMER->CNT = 0;
	WDA_TIMER->SR = 0;

	nvicEnableVector(STM32_TIM7_NUMBER, EFI_IRQ_L9779_WDA_PRIORITY);

	/* DBGMCU APB1 pause bit: set TMR7's bit to MATCH TIM5's configured state
	 * (microsecond timer init sets bit 3). Without a debugger these bits are
	 * inert - they only gate the timer clock when a debugger halts the core.
	 * Setting the bit makes TMR7 and TIM5 behave identically under a future
	 * debug session, whichever polarity this silicon implements. NOTE: the
	 * bench runs WITHOUT a debugger (user-confirmed, the Java console is the
	 * only runtime tool), so this never explained the ~2.5x runtime stretch -
	 * that was the SLEEP-MODE clock gate (rccEnableTIM7(false) cleared the
	 * APB1LPENR bit; fixed with lp=true). */
	DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_TIM7_STOP;

	efiPrintf(DRIVER_NAME " wda: TMR7 measured %lu/%lu NT ticks, input %lu MHz (PSC 143 -> %lu)",
		(unsigned long)acDelta, (unsigned long)ntDelta,
		(unsigned long)((newPsc + 1) / 4), (unsigned long)newPsc);
	efiPrintf(DRIVER_NAME " wda: TMR7 final rate %lu/%lu NT ticks (want 2500/40000 = 250 kHz)",
		(unsigned long)acDelta2, (unsigned long)ntDelta2);
}

/* FREE-RUNNING auto-reload timer (the TIM5 pattern), started once by the
 * boot kick and never stopped between fires. Plain register writes - no
 * locks, no asserts - callable from the thread (boot kick / reset self-heal)
 * and the ISR (period change). "Arm" = set the period of the NEXT fire:
 *
 * - Running (CEN=1): ARR is PR-latched by the update event on the AT32 (PR
 *   is preload-only, like the PWM LLD's ARPE pattern - pwm_lld_init writes
 *   PSC/ARR then EGR|UG). The UG event activates the new ARR AND re-inits
 *   CNT=0, so the next fire lands one full period from now - the exact
 *   from-now semantics the old one-shot had. When the period is UNCHANGED
 *   the function is a no-op: the counter auto-reloads and keeps its phase,
 *   and an unneeded UG would both disturb the in-flight phase and set a
 *   spurious UIF.
 * - Stopped (CEN=0, boot kick or a system reset that killed the peripheral
 *   while the RAM flags survived): full start, DIER|UIE re-armed.
 *
 * THE REAL 2x ROOT CAUSE (2026-08-31, settled by this build's pins data):
 * not the one-shot's stop/re-arm and not a clock drift - the timer clock was
 * gated OFF IN SLEEP MODE. rccEnableTIM7(false) clears the AT32 APB1LPENR
 * bit (unlike STM32, where the LLDs pass lp=true to keep the bit set), so
 * the counter only advanced during CPU-awake windows - per=2.1..2.6x of the
 * armed interval, tracking the console-poll wake duty (the fake "drift").
 * The pins cross-measurement's busy-wait kept the CPU awake, which is why
 * it always printed the exact rate. The fix is rccEnableTIM7(true) in
 * wdaTimerInit; the free-run structure is kept because it is the TIM5
 * pattern and removes the per-cycle stop (and the CR1=0/CNT=0 window that
 * let the spurious ~1 ms immediate fire happen). CRITICAL (2026-08-30
 * 18:17 bench): the ARR write is PR-latched by the update event on the
 * AT32 - the 18:17 run armed 21919 ticks but fired at the WRAP (~51.9 ms
 * later) because the ARR write never became active (the counter kept the
 * init PR=0xFFFF). The EGR|UG below latches PR. */

/* Fire-to-fire period measured in NT ticks (4 MHz, the validated TIM5
 * domain): the 1 Hz liveness print shows it, so the TMR7 armed interval is
 * visible on the console. At init the ratio measurement slaved TMR7 to
 * exactly NT/16 (4 us ticks, verified twice), so the arm uses a plain
 * intervalUs/4 count with no runtime re-calibration. */
static efitick_t s_lastFireNt;
static efitick_t s_firePeriodNt;
/* Previous cycle's timing: the REQUHI verdicts read in a feed describe the
 * burst sent at the PREVIOUS fire, so the verdict gate needs the previous
 * cycle's fire period and armed delay, not the current ones. */
static efitick_t s_prevFirePeriodNt;
static int s_prevFireDelayMs;
/* Fires in the last 1 Hz liveness window whose period was off the armed
 * delay by more than +-50% (deferred/retry cycles armed with a short period,
 * or any residual stretch). The delay servo must not trust the verdicts of
 * those cycles. */
static int s_wdaStretched;

static void wdaTimerArm(uint32_t ticks) {
	if (ticks < 2)
		ticks = 2;
	if (ticks > 65535)
		ticks = 65535;

	if (WDA_TIMER->CR1 & STM32_TIM_CR1_CEN) {
		/* Running: latch a new period only when it actually changed. */
		if (WDA_TIMER->ARR == (ticks - 1U))
			return;
		WDA_TIMER->ARR = ticks - 1U;
		WDA_TIMER->EGR = STM32_TIM_EGR_UG;	/* activate PR + re-init CNT (sets UIF) */
		WDA_TIMER->SR = 0;					/* clear the UG-generated UIF */
	} else {
		/* Stopped: boot kick or post-reset self-heal - full start. */
		WDA_TIMER->CR1 = 0;
		WDA_TIMER->CNT = 0;
		WDA_TIMER->ARR = ticks - 1U;
		WDA_TIMER->EGR = STM32_TIM_EGR_UG;	/* latch PR + re-init CNT (sets UIF) */
		WDA_TIMER->SR = 0;					/* clear the UG-generated UIF */
		WDA_TIMER->DIER = STM32_TIM_DIER_UIE;
		WDA_TIMER->CR1 = STM32_TIM_CR1_CEN;
	}
}

static void wdaTimerStop() {
	WDA_TIMER->CR1 = 0;
	WDA_TIMER->DIER = 0;
	WDA_TIMER->SR = 0;
}

/* Last N one-second liveness lines. The 1 Hz line is NOT printed to the
 * console any more (user request 2026-08-30 - it flooded the log); it is
 * stored here and dumped by the 'pins' diagnostic (L9779::debug). */
#define WDA_LIVENESS_LINES		(10)
#define WDA_LIVENESS_LEN		(192)
static char s_wdaLiveness[WDA_LIVENESS_LINES][WDA_LIVENESS_LEN];
static int s_wdaLivenessNext;	/* next slot to fill */
static int s_wdaLivenessCnt;	/* slots filled, saturates at WDA_LIVENESS_LINES */

/* Per-cycle WDA event ring: every feed cycle records what the chip reported
 * (question, EC, WDA_INT, raw REQUHI) plus the cycle outcome, so the chip's
 * acceptance/rejection semantics are visible in ONE pins dump. The ec=7
 * mystery (2026-08-30: EC pinned at 7 while reqhi stays 0xC0 and the
 * question changes - contradictory with the datasheet's 'EC decrements on
 * accepted answers') cannot be settled from 1 Hz samples; the ring shows
 * the per-cycle EC transitions and the exact REQUHI bytes at rejections. */
#define WDA_EVT_LINES			(64)
struct wda_evt {
	uint32_t	ntMs;		/* NT milliseconds at the read batch */
	uint8_t		req;		/* question as reported by REQULO */
	uint8_t		ec;		/* error counter as reported by REQULO */
	uint8_t		intf;		/* WDA_INT */
	uint8_t		requhi;		/* raw REQUHI byte */
	uint8_t		delay;		/* wd_delay_ms after this cycle's adaptation */
	uint8_t		flags;		/* bit0: fire period > +-25% off the armed delay (off-time cycle - cause-agnostic), bit1: burst sent cleanly, bit2: SPI-level fail, bit3: deferred */
};
static wda_evt s_wdaEvt[WDA_EVT_LINES];
static int s_wdaEvtNext;		/* next slot to fill */
static int s_wdaEvtCnt;			/* slots filled, saturates at WDA_EVT_LINES */
/* EC transition counters across recorded cycles (reset on dump) */
static int s_wdaEcUp;
static int s_wdaEcDown;
static int s_wdaEcSame;
static int s_wdaReqChg;

/* Dispatch-path diagnostics (printed by the 'pins' dump and reset after it):
 * the 22:08 bench session (on the old TMR10 timer) showed per=27ms (the
 * timer fires every armed period) while ok climbed at ~18/s (the feed
 * completes only every other fire) with fail/defer/cntbad all ~0 - the
 * missing half of the cycles leaves no trace in the existing counters, so
 * count every stage of the path to pin down where they vanish. */
static int s_wdaIrrRuns;		/* handler entries (the dedicated TMR7 vector) */
static int s_wdaUifSeen;		/* handler entries with the TMR7 UIF set */
static int s_wdaForeign;		/* handler entries WITHOUT the TMR7 UIF (should stay 0 - nothing else is on this vector) */
static int s_wdaDisp;			/* feeds dispatched from the handler */
static int s_wdaFeedRuns;		/* feeds entered */
static int s_wdaNotRunning;		/* feeds that fizzled on !wd_running (the only silent early return) */
static bool s_wdaKillActive;		/* EC>4 kill state for the root-cause console print */

/* ISR-latency probes, both printed by the 1 Hz line:
 *  - lateUs: NT domain, ISR entry minus the expected dispatch moment
 *    (wd_next_moment). Wrong when the NT clock is wrong (it is not - it is
 *    physics-validated).
	 *  - cnLatUs: the TMR7 counter read at ISR entry. The counter wraps to 0
	 *    at ARR (the UEV point), so CNT at entry = the ISR latency mod
	 *    (ARR+1) in 4 us ticks: small = prompt entry, anything else = the
	 *    entry was delayed (preemption, a long critical section, or a core
	 *    halt if a debugger is ever attached). */
static int s_lateUs;
static int s_lateUsMax;
static int s_cnLatUs;
static int s_cnLatUsMax;

/* TMR7 has its own vector (IRQ 55), nothing else on m74_9 uses it. */
CH_IRQ_HANDLER(STM32_TIM7_HANDLER) {
	OSAL_IRQ_PROLOGUE();

	s_wdaIrrRuns++;

	if (WDA_TIMER->SR & STM32_TIM_SR_UIF) {
		s_wdaUifSeen++;
		/* Read CNT first: the counter wraps to 0 at the UEV, so the
		 * current CNT is the ISR entry latency mod (ARR+1) in 4 us ticks. */
		int32_t cnLat = (int32_t)WDA_TIMER->CNT;
		s_cnLatUs = (int)cnLat * 4;
		if (cnLat * 4 > s_cnLatUsMax)
			s_cnLatUsMax = (int)cnLat * 4;

		WDA_TIMER->SR = ~STM32_TIM_SR_UIF;
		/* FREE-RUN: do NOT stop the timer here (the TIM5 pattern). The
		 * auto-reload restarts the count at the wrap and the next fire lands
		 * ARR+1 ticks later. CNT was read above BEFORE the clear for the
		 * latency probe - after the clear nothing else disturbs the run. */
		efitick_t nowNt = getTimeNowNt();
		/* Stash the previous cycle's timing BEFORE overwriting it - the feed's
		 * verdict gate compares the previous fire period against the delay
		 * that cycle was armed with (still the current wd_delay_ms: nothing
		 * has touched it since the previous feed's arm). */
		s_prevFirePeriodNt = s_firePeriodNt;
		s_prevFireDelayMs = s_wda_chip ? s_wda_chip->wd_delay_ms : 0;
		if (s_lastFireNt != 0) {
			s_firePeriodNt = nowNt - s_lastFireNt;
			/* Count off-time cycles for the liveness line: the period is
			 * compared against the delay this fire was armed with (+-25%,
			 * the same threshold as the feed's on-time verdict gate). */
			if (s_prevFireDelayMs > 0) {
				uint32_t perUs = (uint32_t)(s_firePeriodNt / US_TO_NT_MULTIPLIER);
				uint32_t armUs = (uint32_t)s_prevFireDelayMs * 1000;
				if ((perUs < (armUs * 3) / 4) || (perUs > (armUs * 5) / 4))
					s_wdaStretched++;
			}
		}
		s_lastFireNt = nowNt;

		if (s_wda_chip) {
			efitick_t late = (nowNt - s_wda_chip->wd_next_moment) / US_TO_NT_MULTIPLIER;
			if (late < 0)
				late = 0;
			if (late > 60000)
				late = 60000;
			s_lateUs = (int)late;
			if (late > s_lateUsMax)
				s_lateUsMax = (int)late;
		}
		if (s_wda_chip) {
			s_wdaDisp++;
			s_wda_chip->wdFeedFromExecutor();
		}
	} else {
		s_wdaForeign++;
	}

	OSAL_IRQ_EPILOGUE();
}

void L9779::wdArmIsr(int delayMs) {
	uint32_t intervalUs = MS2US(delayMs) - WDA_BURST_LEAD_US;
	wd_next_moment = getTimeNowNt() + MS2NT(delayMs) - US2NT(WDA_BURST_LEAD_US);
	wdaTimerArm((intervalUs + 3) / 4);
}

/* Thread-context arm (boot kick, reset self-heal). */
static void wdArmThread(L9779 *chip, int delayMs) {
	uint32_t intervalUs = MS2US(delayMs) - WDA_BURST_LEAD_US;
	chip->wd_next_moment = getTimeNowNt() + MS2NT(delayMs) - US2NT(WDA_BURST_LEAD_US);
	wdaTimerArm((intervalUs + 3) / 4);
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

		/* Ignition-gated power stage (m74_9): the board's 20 Hz slow callback
		 * (SysTick ISR) flips power_stage_on via l9779_setPowerStage(); all SPI
		 * work lands here in thread context. The OFF transition must PSOFF the
		 * chip BEFORE anything else - chip_init() below writes REG6=0x06
		 * (PSOFF=0) and would re-arm the stages. */
		if (chip->power_stage_on != chip->power_stage_applied) {
			chip->power_stage_applied = chip->power_stage_on;
			if (chip->power_stage_on) {
				/* ignition on: full re-init via the proven need_init path
				 * (SW_RST re-arms the WDA at EC=6 and wipes the parked
				 * PSOFF register state) */
				chip->need_init = true;
			} else {
				/* ignition off: PSOFF the power stages and stop the WDA
				 * feed (see chip_power_off) */
				chip->chip_power_off();
			}
		}

		/* Power-stage gates: while the ignition gate holds the stages off,
		 * do NOT run chip_init (it writes REG6=0x06 PSOFF=0 and re-arms the
		 * stages), do NOT kick the WDA feed and do NOT push output registers.
		 * The diag refresh below stays active either way - it is what feeds
		 * the KEY_ON cache for isIgnVoltage(). */
		if (chip->power_stage_on) {
			if (chip->need_init) {
				/* clear first, as flag can be raised again during init */
				chip->need_init = false;
				/* Full chip reset (SW_RST) before re-init: the only runtime
				 * path that resets the chip, and ONLY on the ignition key-on
				 * transition - it wipes the parked PSOFF state and re-arms the
				 * watchdog (EC=6). The WDA feed itself NEVER requests this
				 * (user directive: the L9779 runs without resets; a torn burst
				 * or a latched EC self-heals on correct answers). The WDA ISR
				 * exchange is kept out of the batch by spi_busy (kernel IRQs
				 * are NOT masked by critical sections), no CS locker is needed
				 * for that. */
				chip->chip_reset();
				chip->chip_init();
				/* sync pins state */
				chip->update_output();
			}

			/* Kick the TMR7 WDA feed once after the chip is up. The first
			 * burst lands its RESP_BYTE0 at wd_delay_ms (27 ms) after the
			 * chip_init cycle anchor - the RESPTIME=10 write started a fresh
			 * cycle, so the first window opens ~15.8 ms (64 kHz) or ~25.9 ms
			 * (39 kHz) after init; 27 ms lands inside BOTH, so the very first
			 * answer is accepted and the boot-accumulated EC (6 -> 7 while the
			 * chip was un-fed during MCU boot) decrements 7 -> 6 -> 5 -> 4
			 * within ~3 cycles (~0.1 s), clearing the WDA kill. The timer then
			 * free-runs: each feed re-arms only a CHANGED period, and the
			 * auto-reload fires every cycle on its own. */
			if (!chip->wd_running) {
				chip->wd_running = true;
				efiPrintf("l9779 wda: TMR7 kick +%d ms (CR1=0x%08lx)", chip->wd_delay_ms, (unsigned long)WDA_TIMER->CR1);
				wdArmThread(chip, chip->wd_delay_ms);
				efiPrintf("l9779 wda: TMR7 armed (CR1=0x%08lx DIER=0x%08lx)", (unsigned long)WDA_TIMER->CR1, (unsigned long)WDA_TIMER->DIER);
			}

			/* WDA feed liveness monitor (diagnostic, 1 Hz): ok/fail/defer
			 * counters climb only when the TMR7 ISR actually fires, so a
			 * silent wedged feed shows up as frozen counters, and per= prints
			 * the measured fire-to-fire period in us (the NT-domain check of
			 * the armed interval - a wrong PSC shows up here as per != delay).
			 *
			 * Self-heal: a system reset kills the TMR7 peripheral while the
			 * RAM flags survive (the bench debug resets do exactly this) - the
			 * timer then never fires and the feed dies silently. Re-arm when
			 * the timer is not running; the feed ISR re-arms at its end, so
			 * this only triggers on a genuinely dead timer. */
			static systime_t last_wda_print = 0;
			if (chip->wd_running && !(WDA_TIMER->CR1 & STM32_TIM_CR1_CEN)) {
				wdArmThread(chip, chip->wd_delay_ms);
			}
			if (chip->wd_running && (now - last_wda_print >= TIME_MS2I(1000))) {
				last_wda_print = now;
				/* Store the one-second liveness line in the ring buffer - it is
				 * printed by the 'pins' diagnostic, not the console (the 1 Hz
				 * efiPrintf flooded the log; user request 2026-08-30). */
				chsnprintf(s_wdaLiveness[s_wdaLivenessNext], WDA_LIVENESS_LEN,
					"ok=%d fail=%d defer=%d pollto=%d delay=%d per=%dus late=%d/%dus cnlat=%d/%dus stretch=%d ratio=%d%% DBG1=0x%lx DBG2=0x%lx CR1=0x%lx DIER=0x%lx PSC=0x%lx ARR=0x%lx CNT=0x%lx",
					chip->wd_ok_cnt, chip->wd_fail_cnt, chip->wd_defer_cnt,
					chip->wd_poll_timeouts, chip->wd_delay_ms,
					(int)(s_firePeriodNt / US_TO_NT_MULTIPLIER),
					s_lateUs, s_lateUsMax,
					s_cnLatUs, s_cnLatUsMax,
					s_wdaStretched,
					(s_firePeriodNt != 0 && chip->wd_delay_ms > 0)
						? (int)(((uint64_t)s_firePeriodNt * 100) / (US_TO_NT_MULTIPLIER * (uint64_t)chip->wd_delay_ms * 1000))
						: 0,
					(unsigned long)DBGMCU->APB1FZ, (unsigned long)DBGMCU->APB2FZ,
					(unsigned long)WDA_TIMER->CR1, (unsigned long)WDA_TIMER->DIER,
					(unsigned long)WDA_TIMER->PSC, (unsigned long)WDA_TIMER->ARR,
					(unsigned long)WDA_TIMER->CNT);
				/* reset the max-latency probes after storing */
				s_lateUsMax = 0;
				s_cnLatUsMax = 0;
				s_wdaStretched = 0;
				s_wdaLivenessNext = (s_wdaLivenessNext + 1) % WDA_LIVENESS_LINES;
				if (s_wdaLivenessCnt < WDA_LIVENESS_LINES)
					s_wdaLivenessCnt++;
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
		}

		/* The VRS conditioner runs fully adaptive (see vrs_configure): the
		 * auto hysteresis and the auto time filter (Tfilter = 1/32*Tn) both
		 * adapt to rpm inside the chip, so there is no software ramp and
		 * nothing to re-arm per start attempt. */

		/* Refresh the power-stage diagnosis cache. Reading a DIA register
		 * clears its fault bits on the chip, so this runs at a low rate;
		 * getOutputDiag() reads the cache from other threads. The refresh
		 * is CHUNKED (DIAG_REFRESH_REGS per pass): each pass sets spi_busy
		 * around its frames, and the WDA ISR must not be
		 * deferred for the whole ~20-frame batch. */
		if (chip->diag_ts <= now) {
			chip->diag_pending = 8 + 2;	/* DIA_REG1..8 + REG9 + REG10 */
			chip->diag_ts = chTimeAddX(chVTGetSystemTimeX(), TIME_MS2I(DIAG_REFRESH_MS));
		}
		if (chip->diag_pending > 0) {
			/* The WDA burst must not be deferred: the feed re-arms +1 ms
			 * when spi_busy is set, and at delay=27 the window-close margin
			 * is only ~1.4 ms - a deferred BYTE0 lands late, shifts the answer
			 * stream and costs EC increments (the 20:28 session:
			 * defer=123 -> late bytes -> cntbad=11 -> kills=8). Skip this
			 * pass's chunk while the burst is imminent; the refresh is
			 * 100 ms-cadenced, so a skipped chunk costs ~7 ms. */
			efitick_t nowNt = getTimeNowNt();
			efitick_t wdMomentNt = chip->wd_next_moment;
			bool burstImminent = (wdMomentNt != 0) &&
				(wdMomentNt > nowNt) &&
				(wdMomentNt < nowNt + MS2NT(2));
			if (!burstImminent) {
				chip->diag_pending -= chip->refresh_diag_cache(DIAG_REFRESH_REGS);
				if (chip->diag_pending < 0)
					chip->diag_pending = 0;
			}
		}
	}
}

RUSEFI_STACK_ROOT_EXPLICIT(l9779_driver_thread, 256);

/* ISR-safe polled single-frame exchange: raw LLD calls only - no bus
 * mutex, no blocking. Callable from the TMR7 WDA ISR (priority 5, below
 * the trigger handoff); the thread side calls it only inside
 * spi_rw/spi_rw_array with spi_busy set. Mirrors spi_rw() minus the
 * acquire/start/release wrapper. */
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
	/* data transfer - bounded poll: the LLD's spi_lld_polled_exchange()
	 * busy-waits on RXNE forever when the peripheral is disabled (SPE=0),
	 * which inside this ISR bricks the board with no console. Bound the
	 * wait (1 ms of NT ticks) and fail the frame instead, so a wedged SPI
	 * degrades to WDA misses (visible on the console) rather than a lockup.
	 * NT-based on purpose: the AT32 port never enables DWT CYCCNT. */
	efitick_t t0 = getTimeNowNt();
	spi->spi->DR = tx;
	while ((spi->spi->SR & SPI_SR_RXNE) == 0) {
		if (getTimeNowNt() - t0 > US2NT(1000)) {
			wd_poll_timeouts++;
			spiUnselectI(spi);
			return -3;
		}
	}
	rx = spi->spi->DR;
	/* DWT CYCCNT is disabled on the AT32 port - the per-frame cycle stat
	 * is meaningless here (kept zero). */
	recent_frame_cycles = 0;
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

/* ISR-context VDA 2.0 level 3 query-answer watchdog feed (datasheet 6.15),
 * ONE burst per monitoring cycle:
 *
 *  - pipelined reads of REQUHI/REQULO (previous answer verdict + current
 *    question; the DO replies arrive 1-2 frames later, so a filler REQUHI
 *    read flushes the pipeline),
 *  - answer-period adaptation from the REQUHI flags,
 *  - the response itself: RESP_BYTE3, RESP_BYTE2, RESP_BYTE1, RESP_BYTE0
 *    written back-to-back as ONE atomic burst, positioned so the END of
 *    RESP_BYTE0 lands at the window center.
 *
 * The chip generates a 4-bit question per monitoring cycle and repeats it
 * until answered correctly in value and time. A wrong value or a response
 * outside the window increments EC; EC > 4 sets WDA_INT and forces
 * OUT1..4 + IGN1..4 off. EC starts at 6 after reset.
 *
 * WHY A SINGLE BURST (root cause of the 2026-08-24 19:09 failure): the chip
 * tracks the response progress in the 2-bit RESP_CNT counter ('11' waiting
 * for BYTE3 ... '00' waiting for BYTE0) and compares EVERY received byte
 * against the expected byte of the CURRENT counter position. The old
 * two-phase scheme (BYTE3..1 in a "prepare" event, BYTE0 5 ms later) left
 * a gap in which a cycle could end unanswered (a deferred/failed BYTE0) -
 * the window end then resets RESP_CNT to '11' and the late BYTE0 is
 * compared as if it were BYTE3: wrong value, and EVERY subsequent byte is
 * off by one position forever. Each cycle then completes with a wrong
 * value, the EC pins at 7 and the WDA kills the blade until a SW_RST.
 * An atomic 4-byte burst cannot desynchronize: a perturbation costs at
 * most one missed cycle, and the next burst re-aligns because RESP_CNT
 * resets at every sequencer run.
 *
 * The DO reply to a request arrives in one of the frames that follow the
 * request (datasheet 6.16.2), so the reads are pipelined as before and the
 * burst writes follow immediately after. */
void L9779::wdFeedFromExecutor()
{
	s_wdaFeedRuns++;

	/* Ignition gate: the driver thread clears wd_running (and stops the
	 * free-running timer) when the power stages are PSOFF'd. If the timer had
	 * already fired this callback, fizzle out WITHOUT touching the bus or
	 * changing the period - a plain stop is racy because the feed re-arms a
	 * changed period at its end (a concurrent fire would resurrect the feed
	 * after the stop). */
	if (!wd_running) {
		s_wdaNotRunning++;
		return;
	}

	/* Never run before the driver thread's first spiStart(): with the
	 * peripheral disabled the polled exchange would spin (the bounded poll
	 * above turns it into a miss, but the SPI never works before the first
	 * chip_init anyway). Defense in depth, not expected to fire: the boot
	 * kick is issued after chip_init() in the driver thread. */
	if (!spi_configured) {
		wd_defer_cnt++;
		wd_prev_cycle_clean = false;
		wdArmIsr(10);
		return;
	}

	/* Never run while the thread owns an SPI batch: spiStart() briefly clears
	 * SPE, and spi_lld_polled_exchange() then spins on RXNE forever inside this
	 * ISR (board bricked right after boot - no fuel pump, no console).
	 * Deferring a whole cycle costs one miss at worst. */
	if (spi_busy) {
		wd_defer_cnt++;
		wd_prev_cycle_clean = false;
		wdArmIsr(1);
		return;
	}

	/* No spi_lld_start here: the bus configuration (CR1/CR2) persists from
	 * the driver thread's spiStart() - the bus is dedicated to this chip,
	 * the config never changes, and the first feed is only kicked after
	 * the thread's chip_init(). */

	/* Mask ISRs at priority >= 4 (the trigger handoff and below) around the
	 * SPI sequences: the chip's frame state (RESP_CNT, the reply pipeline)
	 * must not be torn mid-sequence by a preempting handoff - a stretched
	 * frame or a paused burst scrambles the chip's question engine (the
	 * W_RESP/RESP_Z0/addr_err storms). The executor (priority 3) stays
	 * unmasked: its callbacks are us-scale and it fires the spark. The
	 * debugger halt is unmaskable - a torn burst there self-heals: the
	 * RESP_CNT resets at the next sequencer run and the next atomic burst
	 * answers the freshly re-read question (no chip reset needed). */
#define WDA_BURST_BASEPRI		(4u << (8u - __NVIC_PRIO_BITS))

	/* Pipelined status/question reads. FOUR frames, not three: the DO reply
	 * to a read request arrives one or TWO frames after the request (the
	 * init() IDENT probe sees the same), and the REQULO reply must land
	 * within this batch - with only three frames a 2-frame-delayed reply
	 * lands in the first answer write, the question is lost and the cycle
	 * fails. The observed failure mode (2026-08-24 19:56 log, pre-VRS
	 * build): fail 9 -> 816 and miss 10 -> 712 in ~20 s of driving - the
	 * reads started missing the reply once the phase destabilized, each
	 * failed cycle then expires unanswered, and the collapse feeds itself. */
	static const uint16_t req_tx[] = {
		L9779_WD_REQUHI,	/* status of the previous response */
		L9779_WD_REQULO,	/* current question + error counter */
		L9779_WD_REQUHI,	/* filler: the REQULO reply (1 frame late) */
		L9779_WD_REQUHI,	/* filler: the REQULO reply (2 frames late) */
	};
	int ret = 0;
	uint16_t rx;
	bool requlo_received = false;
	uint8_t requhi = 0;
	uint8_t requlo = 0;

	uint32_t basepri = __get_BASEPRI();
	__set_BASEPRI(WDA_BURST_BASEPRI);
	/* Stale-entry guard: with content-addressed matching a skipped reply
	 * leaves its request queued forever (it can no longer be misattributed,
	 * but it can accumulate). A healthy queue holds <=2 leftovers at batch
	 * start (the previous batch's last reads); more means replies were
	 * skipped - drop the stale entries so the queue cannot fill up. The
	 * thread's diag reads retry on their own. */
	if (rd_pending_cnt > 4) {
		rd_pending_cnt = 0;
		rd_pending_head = 0;
	}
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
	__set_BASEPRI(basepri);

	if ((ret >= 0) && !requlo_received) {
		/* the question was not received within this cycle */
		ret = -1;
	}

	if (ret < 0) {
		/* SPI-level failure: retry soon. No bytes were written, the cycle
		 * expires unanswered (one miss) and the next burst re-aligns. The
		 * chip state is unknown on this path, so consecutive-fault evidence
		 * must not accumulate (and stale clean-cycle flags must not be
		 * trusted for the delay adaptation). */
		wd_fail_cnt++;
		wd_bad_value_cnt = 0;
		wd_prev_cycle_clean = false;
		wdArmIsr(10);
		return;
	}

	/* REQUHI flags report the timing of the previous response and are used
	 * to keep the response delay centered in the answer window.
	 *
	 * TRUST ONLY A CLEAN PREVIOUS CYCLE: a torn burst (bench debugger halt,
	 * a chip reset mid-stream) or a desynced reply pipeline makes these
	 * flags garbage - the 19:02 session walked the delay in the wrong
	 * direction (22 -> 17 on NO_RESP while the window sat ABOVE) chasing
	 * verdicts from a scrambled stream. With wd_prev_cycle_clean false the
	 * verdicts are ignored and the delay stays where it was.
	 *
	 * ORDER MATTERS: a too-late response sets BOTH NO_RESP and RESP_TO_EARLY
	 * (datasheet 6.15: a too-late response is at the same time a too-early
	 * response of the next cycle, the NO_RESP monitoring is overwritten by
	 * the RESP_TO_EARLY one). Check NO_RESP FIRST - with the old order the
	 * both-flags case was classified as EARLY and the delay walked UP (+5)
	 * until it pegged at the clamp, sitting on the window edge and never
	 * recovering (the 20:12 session: delay=27ms, reqhi=0xDB).
	 *
	 * The correction step is 5 ms (well under the 12.6 ms window):
	 * a single miss jumps the phase from just-outside to near center
	 * without overshooting to the other edge. With the shortened RESPTIME
	 * the window drifts by only ~+-0.8 ms (CLK1 +-5% of 15.8 ms), so a lock
	 * loss should be a rare event. */
	/* The verdicts below describe the PREVIOUS burst - consume the clean
	 * flag now; it is re-armed only when the current burst completes cleanly. */
	bool prevClean = wd_prev_cycle_clean;
	wd_prev_cycle_clean = false;

	/* ON-TIME GATE: the verdicts describe the previous burst, sent at the
	 * previous fire. If THAT cycle fired off its armed delay by more than
	 * +-25%, the verdict is real - the answer really missed the chip's
	 * window - but it is NOT actionable: the delay itself was right, the
	 * miss was the stretch, and stepping the delay would walk it the wrong
	 * way (the 19:02/23:13 wrong-way walks chased exactly such verdicts;
	 * the 12:41 bench run added a NEW case: the pins cross-measurement's
	 * masked window delayed one answer ~10 ms = 1.37x the 27 ms arm, the
	 * old +-50% gate passed it, and its verdict walked the delay 27->32
	 * out of the acceptance zone - +-25% catches it). The gate is purely
	 * evidence-based: off-time cycles carry no phase information about the
	 * chip window, so their verdicts are ignored and the next on-time
	 * cycles re-center the delay. With the sleep-mode clock gate fixed
	 * (rccEnableTIM7(true)) the feed period is exact in sleep too, so the
	 * gate now only flags the short defer/retry cycles (armed 1/10 ms on
	 * spi_busy / SPI fail) and any residual preemption - those verdicts
	 * are garbage for exactly the same reason. */
	if ((s_prevFirePeriodNt != 0) && (s_prevFireDelayMs > 0)) {
		uint32_t prevPerUs = (uint32_t)(s_prevFirePeriodNt / US_TO_NT_MULTIPLIER);
		uint32_t armUs = (uint32_t)s_prevFireDelayMs * 1000;
		if ((prevPerUs < (armUs * 3) / 4) || (prevPerUs > (armUs * 5) / 4))
			prevClean = false;
	}

	if (prevClean) {
		if (requhi & 0x02) {
			/* NO_RESP (and NO_RESP+EARLY): response after the window closed */
			wd_timing_miss_cnt++;
			wd_last_miss_dir = 2;
			wd_delay_ms -= 5;
		} else if (requhi & 0x01) {
			/* RESP_TO_EARLY alone: response before the window opened */
			wd_timing_miss_cnt++;
			wd_last_miss_dir = 1;
			wd_delay_ms += 5;
		}
	}

	uint8_t req_now = requlo & 0x0f;
	uint8_t ec_now = (requlo >> 4) & 0x07;

	/* EC transition counters for the event ring (pins diagnostic): does
	 * this chip's EC decrement on accepted answers (as the datasheet
	 * claims)? The 20:16 bench run answered it: over 2233 clean cycles
	 * ecUp=2, ecDown=1 - EC moves only on rejections/acceptances and sits
	 * saturated at 7 while our answers land outside the window. */
	if (ec_now > wd_last_ec)
		s_wdaEcUp++;
	else if (ec_now < wd_last_ec)
		s_wdaEcDown++;
	else
		s_wdaEcSame++;
	if (req_now != wd_last_req)
		s_wdaReqChg++;

	/* The delay stays inside the fixed principled range [17, 38] ms. The
	 * adaptation is ONLY the REQUHI verdicts above (a real chip-timing
	 * signal); there is no question-freeze servo here - the question is the
	 * chip's internal noise, NOT an acceptance detector (proven on the bench:
	 * it is not a deterministic counter, and chasing it walked the delay to
	 * 47 ms while the chip kept rejecting). The chip's f_clk drift is handled
	 * by the verdicts, which is exactly what they exist for. */
	if (wd_delay_ms < WDA_DELAY_MIN_MS)
		wd_delay_ms = WDA_DELAY_MIN_MS;
	if (wd_delay_ms > WDA_DELAY_MAX_MS)
		wd_delay_ms = WDA_DELAY_MAX_MS;

	/* EC-SATURATION ESCAPE (see WDA_EC_SAT_ESCAPE_CYCLES): the REQUHI servo
	 * is blind in a silent-rejection zone (flags cleared between reads), so
	 * a persistent EC >= 6 on clean on-time cycles is the only evidence left.
	 * EC starts at 6 on a chip reset and decrements on accepted answers
	 * (proven 12:41: 7 -> 0), so a HEALTHY feed at 27 ms never arms this; a
	 * feed parked in a rejection zone pins EC at 7 and steps back toward
	 * 27 ms, where the chip accepts (the 12:42 state). The step is +-5 ms;
	 * once EC recovers below 6 (or a verdict fires) the counter resets. */
	if (prevClean && (ec_now >= 6) && (wd_delay_ms != WDA_DELAY_INIT_MS)) {
		if (++wd_ec_sat_cycles >= WDA_EC_SAT_ESCAPE_CYCLES) {
			wd_ec_sat_cycles = 0;
			if (wd_delay_ms > WDA_DELAY_INIT_MS)
				wd_delay_ms -= 5;
			else
				wd_delay_ms += 5;
		}
	} else {
		wd_ec_sat_cycles = 0;
	}

	wd_last_req = req_now;
	wd_last_ec  = ec_now;
	wd_int      = !!(requlo & 0x80);
	/* A rising WDA_INT edge is a watchdog kill pulse: EC crossed 4 and the
	 * chip forced its outputs off until correct answers bring it back. */
	if (wd_int && !wd_prev_int)
		wd_kill_cnt++;
	wd_prev_int = wd_int;

	/* ROOT-CAUSE console print on the kill-state transition (EC crossing 4):
	 * one clear line the moment the chip forces the power stage off, and one
	 * when it re-enables it. This is the state that makes injectors/coils/
	 * pump/blade look dead while SPI and the output diagnostics stay clean. */
	{
		bool killed = (ec_now > 4);
		if (killed != s_wdaKillActive) {
			s_wdaKillActive = killed;
			if (killed) {
				efiPrintf(DRIVER_NAME " WDA KILL: EC=%d > 4 - power stage FORCED OFF by the chip (blade via WDA->Q5B->DIS too)", (int)ec_now);
			} else {
				efiPrintf(DRIVER_NAME " WDA RECOVERED: EC=%d <= 4 - power stage re-enabled", (int)ec_now);
			}
		}
	}

	/* Instrument the REQUHI byte (datasheet DIA_REG15): [7:6] RESP_CNT,
	 * [5] RESP_ERR, [4] RESP_Z0, [3] CHRT, [2] W_RESP (wrong value),
	 * [1] NO_RESP (late), [0] RESP_TO_EARLY. W_RESP and a RESP_CNT != 11
	 * are the desync/wrong-value signatures - they increment the EC without
	 * setting the timing flags, which made the 19:09 'ec=7 miss=2' session
	 * look healthy. Exposed via l9779_getWdaCounters(). */
	wd_last_requhi = requhi;
	if (requhi & (0x04 | 0x10 | 0x20)) {	/* W_RESP | RESP_Z0 | RESP_ERR */
		wd_wrong_cnt++;
		/* A value rejection means the previous burst was not accepted. Do NOT
		 * reset the chip for it (user directive 2026-08-30: NO runtime
		 * reloads - the L9779 runs without resets). The atomic burst cannot
		 * stay desynced: RESP_CNT resets at every sequencer run and the
		 * question is re-read fresh every cycle, so the next burst answers
		 * the current question in the correct byte order - the stream
		 * re-aligns by itself. The old SW_RST escape turned the chip's own
		 * rejections into a reset storm (each reset desynced the reply
		 * pipeline and re-ran chip_init/VRS - the reloads seen at 19:48:39
		 * and 19:51:09), and it did not even stop the rejections (wrong
		 * kept climbing after each reset). Count only; the consecutive-run
		 * counter stays as a diagnostic. */
		wd_bad_value_cnt++;
		/* Recenter the delay on a value rejection (ce32509e policy): W_RESP
		 * carries no timing flags, so the +-5 adaptation above cannot move
		 * the delay back from a thin-edge state - a value-only miss is the
		 * chip rejecting the answer while the timing verdicts stay quiet. */
		wd_delay_ms = WDA_DELAY_INIT_MS;
	} else {
		wd_bad_value_cnt = 0;
	}

	/* RESP_CNT != 11: the chip's answer stream is SHIFTED by one byte (a
	 * stray late byte landed in the wrong position). Writing the burst into
	 * a shifted stream would complete a wrong-value response (EC++) AND keep
	 * the shift for every following cycle. Skip the burst instead: the
	 * window expires unanswered (one EC via NO_RESP), the sequencer resets
	 * RESP_CNT to 11 and the next burst re-aligns deterministically.
	 * Recenter the delay too (ce32509e policy) - the shift implies the
	 * timing sat at the window edge. */
	if ((requhi & 0xc0) != 0xc0) {
		wd_cnt_bad++;
		wd_delay_ms = WDA_DELAY_INIT_MS;
		wdArmIsr(wd_delay_ms);
		return;
	}

	/* EC=7 + WDA_INT: the chip's error counter is at the top and the kill
	 * flag is set. This is what a core halt (bench debugger) leaves behind:
	 * the chip's WDA cycle runs on ITS OWN clock, so every halt lets several
	 * cycles expire unanswered and EC climbs to 7. It is NOT a permanent
	 * latch: the question keeps repeating and a correct atomic burst is
	 * accepted at any later cycle, decrementing EC back down - so NO SW_RST
	 * here, ever. The feed NEVER resets the chip at runtime (user directive:
	 * the L9779 runs without resets; only the ignition gate's key-on
	 * transition re-inits it). The old latch-triggered need_init turned every
	 * bench halt into a SW_RST every ~220 ms: each reset desynced the reply
	 * pipeline (addr_err flood, REQULO-miss fail storm, the 19:26 log) and
	 * re-ran the whole chip_init/VRS re-init the user saw as the endless
	 * reload loop. The wrong-value burst escape was removed for the same
	 * reason (2026-08-30 night: the reloads at 19:48:39/19:51:09 did not even
	 * stop the rejections - wrong kept climbing after each reset). */

	/* The response: all four bytes back-to-back, masked against the handoff
	 * like the reads. The cycle restarts at the END of the RESP_BYTE0 write,
	 * and the period is set so the next BYTE0 lands wd_delay_ms later. */
	const uint8_t *resp = wd_resp_table[wd_last_req];
	basepri = __get_BASEPRI();
	__set_BASEPRI(WDA_BURST_BASEPRI);
	for (int i = 0; i < 4; i++) {
		ret = spi_frame_isr(L9779_WD_ANSW(resp[i]), NULL);
		if (ret < 0)
			break;
	}
	__set_BASEPRI(basepri);
	if (ret == 0) {
		wd_ok_cnt++;
		/* The whole cycle (reads + aligned burst) completed cleanly: the
		 * NEXT cycle's REQUHI verdicts describe this burst and may be
		 * trusted for the delay adaptation. */
		wd_prev_cycle_clean = true;
	} else {
		wd_fail_cnt++;
	}

	/* Record the cycle in the per-cycle event ring (pins diagnostic).
	 * bit0 = the fire-to-fire NT period was off the armed delay by > +-25%
	 * (an off-time cycle - the same test the delay-servo gate uses, so a
	 * flagged cycle's verdicts are ignored by the adaptation). */
	{
		wda_evt *e = &s_wdaEvt[s_wdaEvtNext];
		s_wdaEvtNext = (s_wdaEvtNext + 1) % WDA_EVT_LINES;
		if (s_wdaEvtCnt < WDA_EVT_LINES)
			s_wdaEvtCnt++;
		e->ntMs = (uint32_t)(getTimeNowNt() / (US2NT(1000)));
		e->req = wd_last_req;
		e->ec = wd_last_ec;
		e->intf = wd_int ? 1 : 0;
		e->requhi = wd_last_requhi;
		e->delay = (uint8_t)wd_delay_ms;
		e->flags = (uint8_t)(
			((s_firePeriodNt / US_TO_NT_MULTIPLIER) > (uint32_t)((5 * wd_delay_ms * 1000) / 4) ? 0x01 : 0) |
			(ret == 0 ? 0x02 : 0x00));
	}

	/* Set the next period from the BYTE0 write end (now): the burst's
	 * BYTE0 lands ~WDA_BURST_LEAD_US after the fire, so set the period a
	 * burst-lead short of the full delay. When the delay is unchanged the
	 * free-running auto-reload keeps its phase (wdaTimerArm no-op). */
	wdArmIsr(wd_delay_ms);
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

/* Runtime cross-measurement of the APB1 timer clocks against the CORE
 * DWT->CYCCNT (the core cycle counter, clocks at HCLK = 288 MHz). Why
 * CYCCNT: it is a pure core register that counts HCLK cycles, owned by
 * nobody, and needs no configuration beyond a one-time enable - unlike
 * the core SysTick peripheral, which the ChibiOS system tick on this port
 * provably runs on (measured 2026-08-31: STM32_ST_USE_TIMER=2 is NOT
 * honored - TIM2 sits clock-disabled with PSC=0/CR1=0, and the virtual
 * tick froze when the core SysTick was disabled). Hijacking SysTick for
 * a measurement therefore breaks the kernel tick - CYCCNT does not.
 * HCLK itself is validated end-to-end by the working USB (48 MHz PLL
 * output) and CAN timing.
 *
 * Measurement A (masked, ~10 ms): the WDA ISR (priority 5) is masked so
 * TMR7 free-runs without period changes, and TIM5 (NT) + TMR7 are compared
 * against CYCCNT in one window. Healthy clock tree: NT ~4 MHz, TMR7 tick
 * ~250 kHz (input 288 MHz). NOTE what this instrument CANNOT see: its
 * busy-wait keeps the CPU awake, so it always samples the awake-phase rate -
 * it printed EXACT clocks while the live feed ran at 2x period, which is
 * what finally proved the 2x lived in SLEEP: the timer clock was gated off
 * in sleep mode (rccEnableTIM7(false) cleared the AT32 APB1LPENR bit) and
 * only advanced during CPU-awake windows. Fixed with rccEnableTIM7(true).
 * The handoff (4) and executor (3) stay unmasked and do not touch these
 * counters, so the window is car-safe-ish; bench tool.
 *
 * Measurement B (unmasked, ~100 ms): the ChibiOS virtual tick vs CYCCNT -
 * expect ~100 ms in 100 ms. This validates the kernel tick config; both
 * derive from HCLK, so it cannot detect an HCLK drift (USB/CAN do that).
 *
	 * One-off, called from the 'pins' diagnostic. Non-destructive: the
	 * measurement no longer masks the WDA ISR (see measurement A), so a
	 * pins call does not perturb the feed's answer timing. */
static void wdaTimerCrossMeasure() {
	/* ---- register dump first (no timing side effects) ---- */
	uint32_t cfgr = RCC->CFGR;
	const char *ppre1 = "?";
	switch ((cfgr >> 10) & 7) {
		case 0: ppre1 = "DIV1"; break;
		case 4: ppre1 = "DIV2"; break;
		case 5: ppre1 = "DIV4"; break;
		case 6: ppre1 = "DIV8"; break;
		case 7: ppre1 = "DIV16"; break;
	}
	const char *ppre2 = "?";
	switch ((cfgr >> 13) & 7) {
		case 0: ppre2 = "DIV1"; break;
		case 4: ppre2 = "DIV2"; break;
		case 5: ppre2 = "DIV4"; break;
		case 6: ppre2 = "DIV8"; break;
		case 7: ppre2 = "DIV16"; break;
	}
	efiPrintf(DRIVER_NAME " tmr: CFGR=0x%08x (SW=%u SWS=%u HPRE=%u PPRE1=%s PPRE2=%s) PLLCFG=0x%08x MISC1=0x%08x MISC2=0x%08x APB1EN=0x%08x",
		(unsigned)cfgr, (unsigned)(cfgr & 3), (unsigned)((cfgr >> 2) & 3),
		(unsigned)((cfgr >> 4) & 0xf), ppre1, ppre2,
		(unsigned)RCC->PLLCFGR, (unsigned)RCC->MISC1, (unsigned)RCC->MISC2,
		(unsigned)RCC->APB1ENR);
	efiPrintf(DRIVER_NAME " tmr: TIM5 PSC=%lu CNT=%lu CR1=0x%lx | TIM2 PSC=%lu CNT=%lu CR1=0x%lx | TMR7 PSC=%lu ARR=%lu CNT=%lu CR1=0x%lx",
		(unsigned long)TIM5->PSC, (unsigned long)TIM5->CNT, (unsigned long)TIM5->CR1,
		(unsigned long)TIM2->PSC, (unsigned long)TIM2->CNT, (unsigned long)TIM2->CR1,
		(unsigned long)WDA_TIMER->PSC, (unsigned long)WDA_TIMER->ARR,
		(unsigned long)WDA_TIMER->CNT, (unsigned long)WDA_TIMER->CR1);

	/* One-time enable of the DWT cycle counter (HCLK): idempotent, does not
	 * disturb anything - unlike the core SysTick, which the kernel tick
	 * owns on this port. */
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

	/* ---- measurement B: ChibiOS virtual tick vs HCLK CYCCNT, unmasked ---- */
	{
		uint32_t c0 = DWT->CYCCNT;
		systime_t vt0 = chVTGetSystemTimeX();
		uint32_t c1;
		do {
			c1 = DWT->CYCCNT;
		} while ((c1 - c0) < 28800000U);       /* 100 ms at 288 MHz HCLK */
		uint32_t cycDelta = c1 - c0;
		systime_t vtDelta = chVTGetSystemTimeX() - vt0;
		efiPrintf(DRIVER_NAME " tmr: ST %u ms per %lu HCLK cycles (want ~100 ms)",
			(unsigned)vtDelta, (unsigned long)cycDelta);
	}

	/* ---- measurement A: NT (TIM5) + TMR7 vs HCLK CYCCNT, UNMASKED ----
	 * The old version masked the WDA ISR (BASEPRI 5) for the 10 ms window.
	 * That mask was DESTRUCTIVE on the bench (12:41 run): the delayed
	 * answer (~10 ms late) missed the chip's window, the NO_RESP cascade
	 * knocked EC 0->6 (KILL pulse) and one perturbed cycle's verdict
	 * walked the delay out of the acceptance zone. The mask is no longer
	 * needed: the free-run feed does not touch the timer on normal cycles
	 * (wdaTimerArm no-ops when the period is unchanged), and the ISR's
	 * UIF clear does not disturb CNT. A rare period change (defer/verdict)
	 * mid-window would corrupt that one dump's tmDelta - self-evident from
	 * the ARR line. */
	{
		uint32_t c0 = DWT->CYCCNT;
		uint32_t tm0 = WDA_TIMER->CNT;
		uint32_t nt0 = TIM5->CNT;
		uint32_t c1;
		do {
			c1 = DWT->CYCCNT;
		} while ((c1 - c0) < 2880000U);        /* 10 ms at 288 MHz HCLK */
		uint32_t tm1 = WDA_TIMER->CNT;
		uint32_t nt1 = TIM5->CNT;

		uint32_t cycDelta = c1 - c0;
		uint32_t ntDelta = nt1 - nt0;        /* TIM5 free-runs: unsigned wrap is exact */
		/* TMR7 wraps at ARR; the free-run period is >= 26.9 ms at every normal
		 * delay, but a deferred cycle can carry a 1 ms ARR into the window
		 * start - the single-wrap correction below is then wrong for that one
		 * dump. Self-evident: the ARR register is printed in the register
		 * line, and the measurement is a bench tool. */
		uint32_t arr = WDA_TIMER->ARR;
		uint32_t tmDelta;
		if (tm1 < tm0)
			tmDelta = tm1 + arr + 1 - tm0;
		else
			tmDelta = tm1 - tm0;
		if (cycDelta == 0)
			cycDelta = 1;

		/* Rates with one decimal, scaled by 10 to stay in integer math:
		 * HCLK = 288 MHz is the CYCCNT rate. */
		uint32_t ntMhz10 = (uint32_t)(((uint64_t)ntDelta * 2880) / cycDelta);   /* x10 */
		uint32_t wdaKhz10 = (uint32_t)(((uint64_t)tmDelta * 2880000) / cycDelta); /* x10 */
		uint32_t wdaInMhz10 = (uint32_t)(((uint64_t)wdaKhz10 * (WDA_TIMER->PSC + 1)) / 1000); /* x10 */
		efiPrintf(DRIVER_NAME " tmr: NT %lu.%lu MHz (want 4.0) | TMR7 tick %lu.%lu kHz input %lu.%lu MHz (want 250.0/288.0) | tm=%lu nt=%lu cyc=%lu",
			(unsigned long)(ntMhz10 / 10), (unsigned long)(ntMhz10 % 10),
			(unsigned long)(wdaKhz10 / 10), (unsigned long)(wdaKhz10 % 10),
			(unsigned long)(wdaInMhz10 / 10), (unsigned long)(wdaInMhz10 % 10),
			(unsigned long)tmDelta, (unsigned long)ntDelta, (unsigned long)cycDelta);
	}
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
	efiPrintf(DRIVER_NAME " WDA: req=0x%x ec=%d wda_int=%d ok=%d fail=%d miss=%d wrong=%d cntbad=%d delay=%dms reqhi=0x%02x",
		wd_last_req, wd_last_ec, wd_int ? 1 : 0, wd_ok_cnt, wd_fail_cnt, wd_timing_miss_cnt, wd_wrong_cnt, wd_cnt_bad, wd_delay_ms, wd_last_requhi);

	/* WDA feed liveness history: the last ten one-second samples, oldest
	 * first. This replaces the 1 Hz console print (see the ring buffer). */
	int lvStart = s_wdaLivenessNext - s_wdaLivenessCnt;
	for (int i = 0; i < s_wdaLivenessCnt; i++) {
		efiPrintf(DRIVER_NAME " wda[%d]: %s", i,
			s_wdaLiveness[(lvStart + i + 2 * WDA_LIVENESS_LINES) % WDA_LIVENESS_LINES]);
	}

	/* Per-cycle EC/question transitions since the last dump: the datasheet
	 * says EC decrements on accepted answers, but the 2026-08-30 bench
	 * shows ec=7 pinned with clean reqhi - ecUp/ecDown settles which model
	 * this chip follows (a healthy feed should show ecDown dominating). */
	efiPrintf(DRIVER_NAME " WDA transitions: ecUp=%d ecDown=%d ecSame=%d reqChg=%d (since last pins)",
		s_wdaEcUp, s_wdaEcDown, s_wdaEcSame, s_wdaReqChg);
	s_wdaEcUp = s_wdaEcDown = s_wdaEcSame = s_wdaReqChg = 0;

	/* Dispatch-path accounting (the 2:1 fire:feed investigation): where do
	 * the TMR7 fires go? irq=handler entries, uif=entries with TMR7 UIF,
	 * foreign=entries without UIF (0 expected - the vector is dedicated),
	 * disp=feeds dispatched, feed=feeds entered, notrunning=silent
	 * !wd_running exits.
	 * A healthy 1:1 feed reads irq==uif==disp==feed==ok and foreign==0. */
	efiPrintf(DRIVER_NAME " WDA path: irq=%d uif=%d foreign=%d disp=%d feed=%d notrunning=%d (since last pins)",
		s_wdaIrrRuns, s_wdaUifSeen, s_wdaForeign, s_wdaDisp, s_wdaFeedRuns, s_wdaNotRunning);
	s_wdaIrrRuns = s_wdaUifSeen = s_wdaForeign = s_wdaDisp = s_wdaFeedRuns = s_wdaNotRunning = 0;

	/* The last cycles of the per-cycle event ring (newest LAST):
	 * t=NTms, req, ec, int, reqhi, delay, flags (1=off-time, 2=burst ok). */
	int evStart = s_wdaEvtNext - s_wdaEvtCnt;
	for (int i = 0; i < s_wdaEvtCnt; i++) {
		const wda_evt *e = &s_wdaEvt[(evStart + i + 2 * WDA_EVT_LINES) % WDA_EVT_LINES];
		efiPrintf(DRIVER_NAME " wda@%u: req=%x ec=%d int=%d reqhi=0x%02x d=%d f=%d",
			(unsigned)e->ntMs, e->req, e->ec, e->intf, e->requhi, e->delay, e->flags);
	}

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

	/* Runtime cross-measurement of the timer clocks against the HCLK SysTick
	 * (the open-question tool from the TMR7 drift saga, docs/report.md). */
	wdaTimerCrossMeasure();
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

/* Configure the flying-wheel (VRS) sensor interface per the DATASHEET's
 * own design (6.14), not the stock config script:
 *
 * - CONFIG_REG1 bit1 = 1: fully adaptive VRS mode. The auto-adaptive
 *   hysteresis (differential amp -> peak detector -> 5-level quantizer ->
 *   hysteresis selection) scales the hysteresis with the actual sensor
 *   amplitude.
 * - CONFIG_REG5 VRS_MODE[4:3] = 11: auto-adaptive hysteresis ON AND
 *   auto-adaptive time filter ON. The filter (Tfilter = 1/32 * Tn,
 *   4..200 us, adaptive to the tooth period) is the chip's designed
 *   noise-spike rejection for VR sensors; with it OFF short spikes pass
 *   straight through to OUT_VRS.
 * - CONFIG_REG5 VRS_HYST[2:0] = 000: 17 uA hysteresis floor.
 *
 * With both mechanisms adaptive there is NO software ramp - the chip tracks
 * the rpm itself. This replaces the previous stock-script ramp (VRS_MODE=01
 * = auto filter OFF, VRS_HYST stepped 100..111 = 32/51/17/0 uA where 111 is
 * "test purpose only"): with that config the car lost one tooth per
 * revolution at ~2000-3400 rpm (C9003 57/58, C9007/C9008 tooth errors).
 * The value 0xd8 for REG5 is exactly the chip's own reset default (VRS_MODE
 * 11 + 17 uA floor + VRS_DIAG off); VRS_DIAG (bit5) stays off like the
 * stock. CONFIG_REG4/REG6 are NOT written here (their reset defaults
 * match what we want; CONFIG_REG6 is applied separately in chip_init() /
 * chip_heal_out_dis() with the WDA time base pinned to 64 kHz). */
int L9779::vrs_configure(void)
{
	static const uint8_t cfg1 = 0x02;	/* full adaptive */
	static const uint8_t cfg5 = 0xd8;	/* auto hyst ON + auto filter ON, 17 uA floor */

	int ret = spi_rw(MSG_W(0x01, cfg1), NULL);
	if (ret)
		return ret;
	ret = spi_rw(MSG_W(0x05, cfg5), NULL);
	if (ret)
		return ret;

	efiPrintf(DRIVER_NAME " VRS: full adaptive (REG1=0x%02x REG5=0x%02x: auto-hyst + auto-filter, 17uA floor)", cfg1, cfg5);
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

	/* Power management + WDA time base (see L9779_CONFIG6_PWR). MUST be
	 * written before the RESPTIME anchor below: the response time is scaled
	 * by f_clk (64 kHz with bit1=1), and the RESPTIME write starts a fresh
	 * sequencer run on whatever time base is active at that moment. */
	ret = spi_rw(MSG_W(0x06, L9779_CONFIG6_PWR), NULL);
	if (ret)
		return ret;

	/* Set the WDA response time (CONFIG_REG9, address 0x11) to a short,
	 * DETERMINISTIC value - the chip's reset default is 0x3f and depends on
	 * the power-cycle history (the 21:18 session: 0x3f after the power
	 * cycle, window [163, 184] on the 39 kHz chip - the 105 ms feed was
	 * early every cycle, the question froze and the WDA killed the power
	 * stage). The write costs one EC increment (EC 6->7, recovered by the
	 * first accepted answers) and anchors a fresh cycle ~15.8 ms after this
	 * write on a 64 kHz chip. */
	ret = spi_rw(MSG_W(L9779_WD_RESPTIME_REG, WDA_RESPTIME), NULL);
	if (ret)
		return ret;

	/* Verify the write landed: DIA_REG13 (sub 0x0d) reads back the active
	 * response time. 0x0a = applied; 0x3f = default (the write went to the
	 * wrong address - the 0x07/0x11 trap - and the feed will miss). */
	uint16_t rptime = 0;
	if (read_diag_reg(L9779_WD_RESPTIME_SUB, &rptime) == 0) {
		efiPrintf(DRIVER_NAME " WDA RESPTIME readback = 0x%02x", MSG_GET_DATA(rptime) & 0x3f);
	}

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

/* Ignition-gated power-stage OFF (driver thread context, called on the
 * key-off edge by the thread's power_stage_on transition):
 *  - stop the TMR7 WDA feed FIRST (flag, then stop): the feed checks
 *    wd_running at its entry, so a feed already in flight when we get here
 *    fizzles out without re-arming (plain stop alone is racy - the feed
 *    self re-arms at its end).
 *  - write CONFIG_REG6 = 0x16 (PSOFF=1): the power stages die but the chip
 *    logic, regulators, SPI, the WDA monitoring and the KEY_ON input all
 *    stay alive, so the MCU keeps running and isIgnVoltage() keeps seeing
 *    the key via DIA_REG9. With no answers the EC climbs and WDA_INT
 *    latches - irrelevant while PSOFF holds the stages off; the next
 *    ignition-on re-init (SW_RST via need_init) clears it.
 * The CONTR1..4 enables stay latched in the chip and are re-applied by
 * chip_init()+update_output() on the next ignition-on. */
int L9779::chip_power_off()
{
	int ret;

	wd_running = false;
	wdaTimerStop();

	ret = spi_rw(MSG_W(0x06, L9779_CONFIG6_PSOFF), NULL);
	if (ret) {
		efiPrintf(DRIVER_NAME " PSOFF write failed (%d)", ret);
	}

	return ret;
}

/* OUT_DIS recovery (datasheet 6.14): OUT_DIS keeps the power stages dead
 * until START, and a chip reset (RST asserted by the smart-reset unit -
 * TNL_RST/OV_RST/CRK_RST) additionally wipes the configuration registers:
 * CONFIG_REG6 back to defaults (power latch off, VDD5_UV WDA unmasked),
 * RESPTIME back to 0x3f (the feed then misses the ~112 ms default
 * window every cycle), the VRS conditioner back to limited-adaptive +
 * filter OFF (noise storms at speed), the CONTR1..4 output enables cleared.
 *
 * With configWiped the whole config is re-applied, including the VRS
 * full-adaptive config (a chip reset reverts the write-only REG1/REG5).
 * Without a reset the config survived and only START + CONTR are needed
 * (the datasheet's recipe for a plain VDD5_OV driver cut).
 *
 * Callable only from the driver thread (plain spi_rw path). The WDA ISR's
 * exchange is kept out of each frame by spi_busy, and a chip reset
 * re-arms the WDA monitoring (EC=6), so the feed re-locks on its own after
 * the RESPTIME write re-anchors the cycle. */
int L9779::chip_heal_out_dis(bool configWiped)
{
	int ret = spi_rw(CMD_START_REACT(BIT(1)), NULL);
	if (ret)
		return ret;

	if (configWiped) {
		/* A chip reset reverts CONFIG_REG6 (power latch off, VDD5_UV WDA
		 * unmasked, time base default) - restore it BEFORE the RESPTIME write
		 * so the new cycle is anchored on the 64 kHz time base. */
		ret = spi_rw(MSG_W(0x06, L9779_CONFIG6_PWR), NULL);
		if (ret)
			return ret;

		/* Restore the short response time - a reset reverts it to the 0x3f
		 * default and the feed would miss the long window. The write costs
		 * one EC increment and starts a fresh cycle, which anchors the
		 * window for the feed. */
		ret = spi_rw(MSG_W(L9779_WD_RESPTIME_REG, WDA_RESPTIME), NULL);
		if (ret)
			return ret;

		/* VRS back to the full-adaptive config (a chip reset reverts the
		 * write-only REG1/REG5 to the limited-adaptive defaults). */
		ret = vrs_configure();
		if (ret)
			return ret;
	}

	/* The reset (or the driver cut) cleared the output enables - restore
	 * the CONTR1..4 registers. */
	ret = update_output();
	return ret;
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

	/* WDA watchdog: RESPTIME=10 is written at init (short deterministic
	 * window [15.8, 28.4] @ 64 kHz / [25.9, 38.5] @ 39 kHz). The feed is
	 * kicked by the driver thread after chip_init and then runs as a
	 * free-running auto-reload TMR7 ISR (priority 5, below the trigger
	 * handoff) with a single atomic burst per cycle; the delay is adapted by
	 * the REQUHI timing verdicts only (ce32509e policy, no walk). The timer
	 * rate is measured here against the NT domain (TIM5) and the PSC
	 * programmed so the counter ticks at exactly 250 kHz (4 us) on any
	 * silicon. */
	wd_delay_ms = WDA_DELAY_INIT_MS;
	wd_ec_sat_cycles = 0;
	wd_running = false;
	wd_next_moment = 0;
	spi_busy = false;
	spi_configured = false;
	wd_defer_cnt = 0;
	wd_poll_timeouts = 0;
	wd_kill_cnt = 0;
	wd_wrong_cnt = 0;
	wd_cnt_bad = 0;
	wd_last_requhi = 0;
	wd_last_miss_dir = 0;
	wd_prev_int = false;
	wd_bad_value_cnt = 0;
	wd_prev_cycle_clean = false;
	s_wda_chip = this;
	wdaTimerInit();

	/* power-stage diagnosis cache: nothing valid until the driver thread
	 * performs the first refresh (diag_ts = 0 -> immediate) */
	for (int i = 0; i < 8; i++)
		dia_valid[i] = false;
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
