/*
 * tle8888.c
 *
 * TLE8888 Engine Machine System IC driver
 *
 * This has worked on a bench - see https://youtu.be/yjs5dh_NKo4
 * All SPI and CS pin in OM_DEFAULT mode
 *
 * @date Mar 25, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 * 3.2 Pin Definitions and Functions
 *
 * IN1-4  Parallel input; Input pin for direct control of power stage OUT1-4
 *
 * IN5-8  Parallel input; Input pin for direct control of push pull state IGN1-IGN4
 *
 * Table 24 Direct Drive Input Assignment to Output Stages
 * IN9 to IN12 OUT5 to OUT24
 *
 * Masks/inputs bits:
 * 0..3   - OUT1 .. 4 - INJ - OpenDrain: 2.2A - direct
 * 4..6   - OUT5 .. 7 -       OpenDrain: 4.5A
 * 7..12  - OUT8 ..13 -       Push/Pull: 20mA - quite weak
 * 13..19 - OUT14..20 -       OpenDrain: 0.6A
 * 20..23 - OUT21..24 -       Push/Pull: 0.6A - half bridge (GND or +12v)
 * 24..27 - IGN1 .. 4 -       Push/Pull: 20mA - direct
 *
 * Andrey Gusakov, (c) 2019
 */

#include "global.h"

#include "gpio/tle8888.h"

#if (BOARD_TLE8888_COUNT > 0)

#include "persistent_configuration.h"
#include "hardware.h"
#include "gpio/gpio_ext.h"
#include "pin_repository.h"
#include "os_util.h"
#include "voltage.h"

EXTERN_ENGINE_CONFIGURATION;

/*
 * TODO list:
 */

/*==========================================================================*/
/* Driver local definitions.												*/
/*==========================================================================*/

#define DRIVER_NAME				"tle8888"

static bool drv_task_ready = false;

typedef enum {
	TLE8888_DISABLED = 0,
	TLE8888_WAIT_INIT,
	TLE8888_READY,
	TLE8888_FAILED
} tle8888_drv_state;

/* C0 */
#define CMD_READ			(0 << 0)
#define CMD_WRITE			(1 << 0)
/* C7:1 */
#define CMD_REG_ADDR(a)		(((a) & 0x7f) << 1)
/* CD7:0 */
#define CMD_REG_DATA(d)		(((d) & 0xff) << 8)

#define CMD_WR(a, d)		(CMD_WRITE | CMD_REG_ADDR(a) | CMD_REG_DATA(d))
#define CMD_R(a)			(CMD_READ | CMD_REG_ADDR(a))

/* Window watchdog open WWDOWT window time = 12.8 mS - fixed value for TLE8888QK */
#define CMD_WWDServiceCmd   CMD_WR(0x15, 0x03)
#define FWDRespCmd(d)       CMD_WR(0x16, d)
#define FWDRespSyncCmd(d)   CMD_WR(0x17, d)

#define CMD_SR_CODE			0x1a
#define CMD_SR				CMD_WR(CMD_SR_CODE, 0x03)
// 0x238 = 568
#define CMD_OE_SET			CMD_WR(0x1c, 0x02)
/* not used
#define CMD_OE_CLR			CMD_WR(0x1c, 0x01)
#define CMD_LOCK			CMD_WR(0x1e, 0x02)
*/
#define CMD_UNLOCK			CMD_WR(0x1e, 0x01)

#define WWDStat             0x36
#define FWDStat0            0x37
#define FWDStat1            0x38

/* Status registers */
#define CMD_OPSTAT(n)		CMD_R(0x34 + ((n) & 0x01))
#define CMD_WWDSTAT			CMD_R(WWDStat)
#define CMD_FWDSTAT0  		CMD_R(FWDStat0)
#define CMD_FWDSTAT1        CMD_R(FWDStat1)
#define CMD_TECSTAT			CMD_R(0x39)
#define CMD_WdDiag			CMD_R(0x2e)

/* Diagnostic */
#define CMD_DIAG(n)			CMD_R(0x20 + ((n) & 0x01))
#define CMD_VRSDIAG(n)		CMD_R(0x22 + ((n) & 0x01))
#define CMD_COMDIAG			CMD_R(0x24)
#define CMD_OUTDIAG(n)		CMD_R(0x25 + ((n) & 0x07))
#define CMD_PPOVDIAG		CMD_R(0x2a)
#define CMD_BRIDIAG(n)		CMD_R(0x2b + ((n) & 0x01))
#define CMD_IGNDIAG			CMD_R(0x2d)
#define CMD_WDDIAG			CMD_R(0x2e)

#define CMD_OUTCONFIG(n, d)	CMD_WR(0x40 + (n), d)
#define CMD_BRICONFIG(n, d)	CMD_WR(0x46 + ((n) & 0x01), d)
#define CMD_IGNCONFIG(d)	CMD_WR(0x48, d)
#define CMD_VRSCONFIG(n, d)	CMD_WR(0x49 + ((n) & 0x03), d)
#define CMD_OPCONFIG0(d)	CMD_WR(0x4e, d)
#define CMD_INCONFIG(n, d)	CMD_WR(0x53 + ((n) & 0x03), d)
#define CMD_DDCONFIG(n, d)	CMD_WR(0x57 + ((n) & 0x03), d)
#define CMD_OECONFIG(n, d)	CMD_WR(0x5b + ((n) & 0x03), d)
#define CMD_CONT(n, d)		CMD_WR(0x7b + ((n) & 0x03), d)

const uint8_t watchDogResponses[16][4] = {
/* Reverse order:
 * RESP3,RESP2,RESP1,REST0 */
{0xFF, 0x0F, 0xF0, 0x00},
{0xB0, 0x40, 0xBF, 0x4F},
{0xE9, 0x19, 0xE6, 0x16},
{0xA6, 0x56, 0xA9, 0x59},
{0x75, 0x85, 0x7A, 0x8A},
{0x3A, 0xCA, 0x35, 0xC5},
{0x63, 0x93, 0x6C, 0x9C},
{0x2C, 0xDC, 0x23, 0xD3},
{0xD2, 0x22, 0xDD, 0x2D},
{0x9D, 0x6D, 0x92, 0x62},
{0xC4, 0x34, 0xCB, 0x3B},
{0x8B, 0x7B, 0x84, 0x74},
{0x58, 0xA8, 0x57, 0xA7},
{0x17, 0xE7, 0x18, 0xE8},
{0x4E, 0xBE, 0x41, 0xB1},
{0x01, 0xF1, 0x0E, 0xFE}
};

/*==========================================================================*/
/* Driver exported variables.												*/
/*==========================================================================*/

/*==========================================================================*/
/* Driver local variables and types.										*/
/*==========================================================================*/

/* OS */
SEMAPHORE_DECL(tle8888_wake, 10 /* or BOARD_TLE8888_COUNT ? */);
static THD_WORKING_AREA(tle8888_thread_1_wa, 256);

// todo: much of state is currently global while technically it should be per-chip. but we
// are lazy and in reality it's usually one chip per board


/**
 * For the timing check the microcontroller has to send periodically the window watchdog service command
 * WWDServiceCmd. The window watchdog is triggered correctly if the command is received inside the open
 * window of the window watchdog sequence.
 */
static efitick_t lastWindowWatchdogTimeNt = 0;

static efitick_t lastFunctionWatchdogTimeNt = 0;

static uint16_t WindowWatchdogErrorCounterValue;
static uint16_t FunctionalWatchdogPassCounterValue;
static uint16_t TotalErrorCounterValue;

static uint16_t maybeFirstResponse = 0;
static uint16_t functionWDrx = 0;
static uint16_t wdDiagResponse = 0;

//static_assert(TLE8888_POLL_INTERVAL_MS < Window_watchdog_open_window_time_ms)

static bool needInitialSpi = true;
static bool isWatchdogHappy = false;
static bool wasWatchdogHappy = false;

static int selfResetCounter = 0;
static int lowVoltageResetCounter = 0;
static int requestedResetCounter = 0;
int tle8888reinitializationCounter = 0;

float vBattForTle8888 = 0;

// set debug_mode 31
static int tle8888SpiCounter = 0;
static uint16_t spiRxb = 0, spiTxb = 0;


/* Driver private data */
struct tle8888_priv {
	const struct tle8888_config	*cfg;
	/* cached output state - state last send to chip */
	uint32_t					o_state_cached;
	/* state to be sent to chip */
	uint32_t					o_state;
	/* direct driven output mask */
	uint32_t					o_direct_mask;
	/* output enabled mask */
	uint32_t					o_oe_mask;
	/* push-pull enabled mask (for OUT21..OUT24 only) */
	/* this is overhead to store 4 bits in uint32_t
	 * but I don't want any magic shift math */
	uint32_t					o_pp_mask;

	tle8888_drv_state			drv_state;

	/* diagnostic registers */
	uint8_t						OutDiag[5];
	uint8_t						BriDiag[2];
	uint8_t						IgnDiag;
	/* status registers */
	uint8_t						OpStat[2];

	/* last diagnostick was read */
	systime_t					ts_diag;
};

static struct tle8888_priv chips[BOARD_TLE8888_COUNT];

static const char* tle8888_pin_names[TLE8888_OUTPUTS] = {
	"TLE8888.INJ1",		"TLE8888.INJ2",		"TLE8888.INJ3",		"TLE8888.INJ4",
	"TLE8888.OUT5",		"TLE8888.OUT6",		"TLE8888.OUT7",		"TLE8888.OUT8",
	"TLE8888.OUT9",		"TLE8888.OUT10",	"TLE8888.OUT11",	"TLE8888.OUT12",
	"TLE8888.OUT13",	"TLE8888.OUT14",	"TLE8888.OUT15",	"TLE8888.OUT16",
	"TLE8888.OUT17",	"TLE8888.OUT18",	"TLE8888.OUT19",	"TLE8888.OUT20",
	"TLE8888.OUT21",	"TLE8888.OUT22",	"TLE8888.OUT23",	"TLE8888.OUT24",
	"TLE8888.IGN1",		"TLE8888.IGN2",		"TLE8888.IGN3",		"TLE8888.IGN4"
};

#define getWindowWatchdog() ((WindowWatchdogErrorCounterValue >> 8) & 0x3f)
#define getFunctionalWatchdog() ((FunctionalWatchdogPassCounterValue >> 8) & 0x3f)
#define getTotalErrorCounter() ((TotalErrorCounterValue >> 8) & 0x3f)

#if EFI_TUNER_STUDIO
// set debug_mode 31
void tle8888PostState(TsDebugChannels *debugChannels) {

	debugChannels->debugIntField1 = getWindowWatchdog();
	debugChannels->debugIntField2 = getFunctionalWatchdog();
	debugChannels->debugIntField3 = getTotalErrorCounter();
	//debugChannels->debugIntField1 = tle8888SpiCounter;
	//debugChannels->debugIntField2 = spiTxb;
	//debugChannels->debugIntField3 = spiRxb;
	debugChannels->debugIntField5 = tle8888reinitializationCounter;

	debugChannels->debugFloatField3 = chips[0].OpStat[1];
	debugChannels->debugFloatField4 = selfResetCounter * 1000000 + requestedResetCounter * 10000 + lowVoltageResetCounter;
	debugChannels->debugFloatField5 = functionWDrx;
	debugChannels->debugFloatField6 = wdDiagResponse;
}
#endif /* EFI_TUNER_STUDIO */

/*==========================================================================*/
/* Driver local functions.													*/
/*==========================================================================*/

static SPIDriver *get_bus(struct tle8888_priv *chip)
{
	/* return non-const SPIDriver* from const struct cfg */
	return chip->cfg->spi_bus;
}

/**
 * @return always return 0 for now
 */
static int tle8888_spi_rw(struct tle8888_priv *chip, uint16_t tx, uint16_t *rx)
{
	SPIDriver *spi = get_bus(chip);

	/**
	 * 15.1 SPI Protocol
	 *
	 * after a read or write command: the address and content of the selected register
	 * is transmitted with the next SPI transmission (for not existing addresses or
	 * wrong access mode the data is always 0)
	 */

	/* Acquire ownership of the bus. */
	spiAcquireBus(spi);
	/* Setup transfer parameters. */
	spiStart(spi, &chip->cfg->spi_config);
	/* Slave Select assertion. */
	spiSelect(spi);
	/* Atomic transfer operations. */
	spiRxb = spiPolledExchange(spi, tx);
	//spiExchange(spi, 2, &tx, &rxb); 8 bit version just in case?
	/* Slave Select de-assertion. */
	spiUnselect(spi);
	/* Ownership release. */
	spiReleaseBus(spi);

	spiTxb = tx;
	tle8888SpiCounter++;

	if (rx)
		*rx = spiRxb;

	/* no errors for now */
	return 0;
}

/**
 * @brief TLE8888 send output registers data.
 * @details Sends ORed data to register.
 */

static int tle8888_update_output(struct tle8888_priv *chip)
{
	int i;
	int ret = 0;

	/* TODO: lock? */

	uint32_t briconfig0 = 0;
	uint32_t out_data = chip->o_state;

	/* calculate briconfig0 */
	uint32_t pp_low = out_data & chip->o_pp_mask;
	for (i = 20; i < 24; i++) {
		if (pp_low & BIT(i)) {
			/* low-side switch mode */
		} else {
			/* else enable high-side switch mode */
			briconfig0 |= BIT((i - 20) * 2);
		}
	}
	/* TODO: set freewheeling bits in briconfig0? */

	/* output for push-pull pins is allways enabled
	 * (at least until we start supporting hi-Z state) */
	out_data |= chip->o_pp_mask;
	/* TODO: apply hi-Z mask when support will be added */

	/* set value only for non-direct driven pins */
	out_data &= ~chip->o_direct_mask;
	for (int i = 0; i < 4; i++) {
		uint8_t od;

		od = (out_data >> (8 * i)) & 0xff;
		ret |= tle8888_spi_rw(chip, CMD_CONT(i, od), NULL);
	}

	if (ret == 0) {
		/* atomic */
		chip->o_state_cached = out_data;
	}

	/* TODO: unlock? */

	return ret;
}

/**
 * @brief read TLE8888 OpStat1 and diagnostic registers data.
 * @details Chained read of several registers
 */
static int tle8888_update_status_and_diag(struct tle8888_priv *chip)
{
	int ret = 0;
	uint16_t rx = 0;

	/* TODO: lock? */

	/* the address and content of the selected register is transmitted with the
	 * next SPI transmission (for not existing addresses or wrong access mode
	 * the data is always '0' */
	/* this is quite expensive to call tle8888_spi_rw on each register read
	 * TODO: implement tle8888_spi_rw_array ? */

//	/* request OutDiad0, ignore received */
//	if ((ret = tle8888_spi_rw(chip, CMD_OUTDIAG(0), NULL)))
//		return ret;
//
//	/* request OutDiad1, receive OutDiag0 */
//	if ((ret = tle8888_spi_rw(chip, CMD_OUTDIAG(1), &rx)))
//		return ret;
//	chip->OutDiag[0] = getDataFromResponse(rx);
//
//	/* request OutDiad2, receive OutDiag1 */
//	if ((ret = tle8888_spi_rw(chip, CMD_OUTDIAG(2), &rx)))
//		return ret;
//	chip->OutDiag[1] = getDataFromResponse(rx);
//
//	/* request OutDiad3, receive OutDiag2 */
//	if ((ret = tle8888_spi_rw(chip, CMD_OUTDIAG(3), &rx)))
//		return ret;
//	chip->OutDiag[2] = getDataFromResponse(rx);
//
//	/* request OutDiad4, receive OutDiag3 */
//	if ((ret = tle8888_spi_rw(chip, CMD_OUTDIAG(4), &rx)))
//		return ret;
//	chip->OutDiag[3] = getDataFromResponse(rx);
//
//	/* request BriDiag0, receive OutDiag4 */
//	if ((ret = tle8888_spi_rw(chip, CMD_BRIDIAG(0), &rx)))
//		return ret;
//	chip->OutDiag[4] = getDataFromResponse(rx);
//
//	/* request BriDiag1, receive BriDiag0 */
//	if ((ret = tle8888_spi_rw(chip, CMD_BRIDIAG(1), &rx)))
//		return ret;
//	chip->BriDiag[0] = getDataFromResponse(rx);
//
//	/* request IgnDiag, receive BriDiag1 */
//	if ((ret = tle8888_spi_rw(chip, CMD_IGNDIAG, &rx)))
//		return ret;
//	chip->BriDiag[1] = getDataFromResponse(rx);

	/* request OpStat0, receive IgnDiag */
	if ((ret = tle8888_spi_rw(chip, CMD_OPSTAT(0), &rx)))
		return ret;
//	chip->IgnDiag = getDataFromResponse(rx);

	/* request OpStat1, receive OpStat0 */
	if ((ret = tle8888_spi_rw(chip, CMD_OPSTAT(1), &rx)))
		return ret;
	chip->OpStat[0] = getDataFromResponse(rx);

	/* request OpStat1, receive OpStat1 */
	if ((ret = tle8888_spi_rw(chip, CMD_OPSTAT(1), &rx)))
		return ret;
	chip->OpStat[1] = getDataFromResponse(rx);

	/* TODO: unlock? */

	return ret;
}

static int tle8888_update_direct_output(struct tle8888_priv *chip, int pin, int value)
{
	const struct tle8888_config	*cfg = chip->cfg;

	/* find direct drive gpio */
	for (int i = 0; i < TLE8888_DIRECT_MISC; i++) {
		/* again: outputs in cfg counted starting from 1 - hate this */
		if (cfg->direct_io[i].output == pin + 1) {
			if (value)
				palSetPort(cfg->direct_io[i].port,
						   PAL_PORT_BIT(cfg->direct_io[i].pad));
			else
				palClearPort(cfg->direct_io[i].port,
						   PAL_PORT_BIT(cfg->direct_io[i].pad));
			return 0;
		}
	}

	/* direct gpio not found */
	return -1;
}

// ChibiOS does not offer this function so that's a copy-paste of 'chSemSignal' without locking
void chSemSignalS(semaphore_t *sp) {

  chDbgCheck(sp != NULL);

  chDbgAssert(((sp->cnt >= (cnt_t)0) && queue_isempty(&sp->queue)) ||
              ((sp->cnt < (cnt_t)0) && queue_notempty(&sp->queue)),
              "inconsistent semaphore");
  if (++sp->cnt <= (cnt_t)0) {
    chSchWakeupS(queue_fifo_remove(&sp->queue), MSG_OK);
  }
}


/**
 * @brief TLE8888 chip driver wakeup.
 * @details Wake up driver. Will cause output register update
 */

static int tle8888_wake_driver(struct tle8888_priv *chip)
{
	(void)chip;

    /* Entering a reentrant critical zone.*/
    syssts_t sts = chSysGetStatusAndLockX();
	chSemSignalI(&tle8888_wake);
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

static void handleFWDStat1(struct tle8888_priv *chip, int registerNum, int data) {
	if (registerNum != FWDStat1)
		return;
	uint8_t FWDQUEST = data & 0xF;
	uint8_t FWDRESPC = (data >> 4) & 3;
	/* Table lines are filled in reverse order (like in DS) */
	uint8_t response = watchDogResponses[FWDQUEST][3 - FWDRESPC];
	if (FWDRESPC) {
		tle8888_spi_rw(chip, FWDRespCmd(response), NULL);
	} else {
		/* to restart heartbeat timer, sync command should be used for response 0 */
		tle8888_spi_rw(chip, FWDRespSyncCmd(response), NULL);
	}
	tle8888_spi_rw(chip, CMD_WdDiag, NULL);
	tle8888_spi_rw(chip, CMD_WdDiag, &wdDiagResponse);
}

static int startupConfiguration(struct tle8888_priv *chip) {
	const struct tle8888_config	*cfg = chip->cfg;
	uint16_t response = 0;
	/* Set LOCK bit to 0 */
	// second 0x13D=317 => 0x35=53
	tle8888_spi_rw(chip, CMD_UNLOCK, &response);

	chip->o_direct_mask = 0;
	chip->o_oe_mask		= 0;
	/* HACK HERE if you want to enable PP for OUT21..OUT24
	 * without approprirate call to setPinMode */
	chip->o_pp_mask		= 0; /* = BIT(20) | BIT(21) | BIT(22) | BIT(23); */
	/* enable direct drive of OUTPUT4..1
	 * ...still need INJEN signal */
	chip->o_direct_mask	|= 0x0000000f;
	chip->o_oe_mask		|= 0x0000000f;
	/* enable direct drive of IGN4..1
	 * ...still need IGNEN signal */
	chip->o_direct_mask |= 0x0f000000;
	chip->o_oe_mask		|= 0x0f000000;

	/* map and enable outputs for direct driven channels */
	for (int i = 0; i < TLE8888_DIRECT_MISC; i++) {
		int out = cfg->direct_io[i].output;

		/* not used? */
		if (out == 0)
			continue;

		/* OUT1..4 driven direct only through dedicated pins */
		if (out < 5)
			return -1;

		/* in config counted from 1 */
		uint32_t mask = (1 << (out - 1));

		/* check if output already occupied */
		if (chip->o_direct_mask & mask) {
			/* incorrect config? */
			return -1;
		}

		/* enable direct drive and output enable */
		chip->o_direct_mask	|= mask;
		chip->o_oe_mask		|= mask;

		/* set INCONFIG - aux input mapping */
		tle8888_spi_rw(chip, CMD_INCONFIG(i, out - 5), NULL);
	}

	/* enable all ouputs
	 * TODO: add API to enable/disable? */
	chip->o_oe_mask		|= 0x0ffffff0;

	/* set OE and DD registers */
	for (int i = 0; i < 4; i++) {
		uint8_t oe, dd;

		oe = (chip->o_oe_mask >> (8 * i)) & 0xff;
		dd = (chip->o_direct_mask >> (8 * i)) & 0xff;
		tle8888_spi_rw(chip, CMD_OECONFIG(i, oe), NULL);
		tle8888_spi_rw(chip, CMD_DDCONFIG(i, dd), NULL);
	}

	/* Debug: disable diagnostic */
	for (int i = 0; i <= 5; i++) {
		tle8888_spi_rw(chip, CMD_OUTCONFIG(i, 0), NULL);
	}

	/* enable outputs */
	tle8888_spi_rw(chip, CMD_OE_SET, NULL);

	if (cfg->mode > 0) {
		tle8888_spi_rw(chip, CMD_VRSCONFIG1(cfg->mode << 2), NULL);
	}
	return 0;
}

void watchdogLogic(struct tle8888_priv *chip) {
	efitick_t nowNt = getTimeNowNt();
	if (nowNt - lastWindowWatchdogTimeNt > MS2NT(Window_watchdog_close_window_time_ms)) {
		tle8888_spi_rw(chip, CMD_WWDServiceCmd, &maybeFirstResponse);
		lastWindowWatchdogTimeNt = nowNt;
	}

	if (nowNt - lastFunctionWatchdogTimeNt > MS2NT(Functional_Watchdog_PERIOD_MS)) {
		// todo: extract helper method?
		/* the address and content of the selected register is transmitted with the
		 * next SPI transmission (for not existing addresses or wrong access mode
		 * the data is always '0' */
		tle8888_spi_rw(chip, CMD_FWDSTAT1, &maybeFirstResponse);
		// here we get response of the 'FWDStat1' above
		tle8888_spi_rw(chip, CMD_WdDiag, &functionWDrx);
		handleFWDStat1(chip, getRegisterFromResponse(functionWDrx), getDataFromResponse(functionWDrx));
		lastFunctionWatchdogTimeNt = nowNt;
	}

	tle8888_spi_rw(chip, CMD_WWDSTAT, NULL);
	tle8888_spi_rw(chip, CMD_FWDSTAT0, &WindowWatchdogErrorCounterValue);
	tle8888_spi_rw(chip, CMD_TECSTAT, &FunctionalWatchdogPassCounterValue);
	tle8888_spi_rw(chip, CMD_TECSTAT, &TotalErrorCounterValue);


	// sanity checking that we are looking at the right responses
	if (getRegisterFromResponse(WindowWatchdogErrorCounterValue) == WWDStat &&
			getRegisterFromResponse(FunctionalWatchdogPassCounterValue) == FWDStat0
			) {

		wasWatchdogHappy = isWatchdogHappy;
		// reset state for error counters has us start in Safe Mode
		isWatchdogHappy = (getWindowWatchdog() == 0 && getFunctionalWatchdog() == 0);
		if (!wasWatchdogHappy && isWatchdogHappy) {
			startupConfiguration(chip);
		}
	}
}

int tle8888SpiStartupExchange(struct tle8888_priv *chip);

static THD_FUNCTION(tle8888_driver_thread, p) {
	(void)p;

	chRegSetThreadName(DRIVER_NAME);

	while (1) {
		msg_t msg = chSemWaitTimeout(&tle8888_wake, TIME_MS2I(TLE8888_POLL_INTERVAL_MS));

		/* should we care about msg == MSG_TIMEOUT? */
		(void)msg;

		if (vBattForTle8888 < LOW_VBATT) {
			// we assume TLE8888 is down and we should not bother with SPI communication
			if (!needInitialSpi) {
				needInitialSpi = true;
				lowVoltageResetCounter++;
			}
			continue; // we should not bother communicating with TLE8888 until we have +12
		}

		if (needInitialSpi) {
			wasWatchdogHappy = isWatchdogHappy = needInitialSpi = false;

			for (int i = 0; i < BOARD_TLE8888_COUNT; i++) {
				struct tle8888_priv *chip = &chips[i];
				tle8888SpiStartupExchange(chip);
			}
		}

		// todo: super-lazy implementation with only first chip!
		watchdogLogic(&chips[0]);

		for (int i = 0; i < BOARD_TLE8888_COUNT; i++) {
			struct tle8888_priv *chip = &chips[i];
			if ((chip->cfg == NULL) ||
				(chip->drv_state == TLE8888_DISABLED) ||
				(chip->drv_state == TLE8888_FAILED))
				continue;

			int ret = tle8888_update_output(chip);
			if (ret) {
				/* set state to TLE8888_FAILED? */
			}

			if (chVTTimeElapsedSinceX(chip->ts_diag) >= TIME_MS2I(TLE8888_POLL_INTERVAL_MS)) {
				/* this is expensive call, will do a lot of spi transfers... */
				ret = tle8888_update_status_and_diag(chip);
				if (ret) {
					/* set state to TLE8888_FAILED or force reinit? */
				}

				chip->ts_diag = chVTGetSystemTimeX();
			}

			/* if bit OE is cleared - reset happened */
			if (!(chip->OpStat[1] & (1 << 6))) {
				needInitialSpi = true;
				selfResetCounter++;
			}
		}
	}
}

void requestTLE8888initialization(void) {
	needInitialSpi = true;
	requestedResetCounter++;
}

/*==========================================================================*/
/* Driver interrupt handlers.												*/
/*==========================================================================*/

/*==========================================================================*/
/* Driver exported functions.												*/
/*==========================================================================*/

static int tle8888_setPadMode(void *data, unsigned int pin, iomode_t mode) {

	if ((pin >= TLE8888_OUTPUTS) || (data == NULL))
		return -1;

	/* do not enalbe PP mode yet */
#if 0
	struct tle8888_priv *chip = (struct tle8888_priv *)data;

	/* only OUT21..OUT24 support mode change: PP vs OD */
	if ((pin < 20) || (pin > 23))
		return 0;

	/* this is absolutly confusing... we pass STM32 specific
	 * values to tle8888 driver... But this is how gpios
	 * currently implemented */
	if ((mode & PAL_STM32_OTYPE_MASK) == PAL_STM32_OTYPE_OPENDRAIN) {
		chip->o_pp_mask &= ~BIT(pin);
	} else {
		chip->o_pp_mask |=  BIT(pin);
	}
#endif

	return 0;
}

static int tle8888_writePad(void *data, unsigned int pin, int value) {

	if ((pin >= TLE8888_OUTPUTS) || (data == NULL))
		return -1;

	struct tle8888_priv *chip = (struct tle8888_priv *)data;

	/* TODO: lock */
	if (value) {
		chip->o_state |=  (1 << pin);
	} else {
		chip->o_state &= ~(1 << pin);
	}
	/* TODO: unlock */
	/* direct driven? */
	if (chip->o_direct_mask & (1 << pin)) {
		return tle8888_update_direct_output(chip, pin, value);
	} else {
		return tle8888_wake_driver(chip);
	}
	return 0;
}

static brain_pin_diag_e tle8888_2b_to_diag_no_temp(unsigned int bits)
{
	if (bits == 0x01)
		return PIN_SHORT_TO_BAT;
	if (bits == 0x02)
		return PIN_OPEN;
	if (bits == 0x03)
		return PIN_SHORT_TO_GND;
	return PIN_OK;
}

static brain_pin_diag_e tle8888_2b_to_diag_with_temp(unsigned int bits)
{
	brain_pin_diag_e diag = tle8888_2b_to_diag_no_temp(bits);

	if (diag == PIN_SHORT_TO_BAT)
		diag |= PIN_DRIVER_OVERTEMP;

	return diag;
}

static brain_pin_diag_e tle8888_getDiag(void *data, unsigned int pin)
{
	if ((pin >= TLE8888_OUTPUTS) || (data == NULL))
		return PIN_INVALID;

	struct tle8888_priv *chip = (struct tle8888_priv *)data;

	if (pin < 4)
		return tle8888_2b_to_diag_with_temp((chip->OutDiag[0] >> ((pin - 0) * 2)) & 0x03);
	if (pin < 8) {
		if (pin == 7)
			return tle8888_2b_to_diag_no_temp((chip->OutDiag[1] >> ((pin - 4) * 2)) & 0x03);
		else
			return tle8888_2b_to_diag_with_temp((chip->OutDiag[1] >> ((pin - 4) * 2)) & 0x03);
	}
	if (pin < 12)
		return tle8888_2b_to_diag_with_temp((chip->OutDiag[2] >> ((pin - 8) * 2)) & 0x03);
	if (pin < 16) {
		if (pin == 12)
			return tle8888_2b_to_diag_no_temp((chip->OutDiag[3] >> ((pin - 12) * 2)) & 0x03);
		else
			return tle8888_2b_to_diag_with_temp((chip->OutDiag[3] >> ((pin - 12) * 2)) & 0x03);
	}
	if (pin < 20)
		return tle8888_2b_to_diag_with_temp((chip->OutDiag[4] >> ((pin - 16) * 2)) & 0x03);
	if (pin < 24) {
		/* half bridges */
		brain_pin_diag_e diag;

		diag = tle8888_2b_to_diag_no_temp((chip->BriDiag[0] >> ((pin - 20) * 2)) & 0x03);
		if (((pin == 22) || (pin == 23)) &&
			(chip->BriDiag[1] & BIT(5)))
			diag |= PIN_DRIVER_OVERTEMP;
		if (((pin == 20) || (pin == 21)) &&
			(chip->BriDiag[1] & BIT(4)))
			diag |= PIN_DRIVER_OVERTEMP;
		if (chip->BriDiag[1] & BIT(pin - 20))
			diag |= PIN_OVERLOAD; /* overcurrent */

		return diag;
	}
	if (pin < 28)
		return tle8888_2b_to_diag_with_temp((chip->IgnDiag >> ((pin - 24) * 2)) & 0x03);

	return PIN_OK;
}

/**
 * @return 0 for valid configuration, -1 for invalid configuration
 */
int tle8888SpiStartupExchange(struct tle8888_priv *chip) {
	const struct tle8888_config	*cfg = chip->cfg;

	tle8888reinitializationCounter++;

	/**
	 * We need around 50ms to get reliable TLE8888 start if MCU is powered externally but +12 goes gown and then goes up
	 * again
	 */
	chThdSleepMilliseconds(50);

	watchdogLogic(chip);

	/* Software reset */
	uint16_t response = 0;
	tle8888_spi_rw(chip, CMD_SR, NULL);
	tle8888_spi_rw(chip, CMD_UNLOCK, &response);
	if (response != (CMD_WRITE | CMD_REG_ADDR(CMD_SR_CODE))) {
		firmwareError(CUSTOM_ERR_TLE8888_RESPONSE, "please check your VBatt wiring and TLE8888 soldering r=%x while vbatt=%f", response, vBattForTle8888);
	}

	/**
	 * Table 8. Reset Times. All reset times not more than 20uS
	 *
	 */
	chThdSleepMilliseconds(3);

	startupConfiguration(chip);

	/* enable pins */
	if (cfg->ign_en.port)
		palSetPort(cfg->ign_en.port, PAL_PORT_BIT(cfg->ign_en.pad));
	if (cfg->inj_en.port)
		palSetPort(cfg->inj_en.port, PAL_PORT_BIT(cfg->inj_en.pad));

	if (CONFIG(verboseTLE8888)) {
		tle8888_dump_regs();
	}

	return 0;
}

static int tle8888_chip_init(void * data) {
	struct tle8888_priv *chip = (struct tle8888_priv *)data;
	const struct tle8888_config	*cfg = chip->cfg;

	int ret = 0;
	/* mark pins used */
	// we do not initialize CS pin so we should not be marking it used - i'm sad
	//ret = gpio_pin_markUsed(cfg->spi_config.ssport, cfg->spi_config.sspad, DRIVER_NAME " CS");
	if (cfg->reset.port != NULL) {
		ret |= gpio_pin_markUsed(cfg->reset.port, cfg->reset.pad, DRIVER_NAME " RST");
		palSetPadMode(cfg->reset.port, cfg->reset.pad, PAL_MODE_OUTPUT_PUSHPULL);
		palSetPort(cfg->reset.port, PAL_PORT_BIT(cfg->reset.pad));
	}
	if (cfg->ign_en.port != NULL) {
		ret |= gpio_pin_markUsed(cfg->ign_en.port, cfg->ign_en.pad, DRIVER_NAME " IGN EN");
		palSetPadMode(cfg->ign_en.port, cfg->ign_en.pad, PAL_MODE_OUTPUT_PUSHPULL);
		palClearPort(cfg->ign_en.port, PAL_PORT_BIT(cfg->ign_en.pad));
	}
	if (cfg->inj_en.port != NULL) {
		ret |= gpio_pin_markUsed(cfg->inj_en.port, cfg->inj_en.pad, DRIVER_NAME " INJ EN");
		palSetPadMode(cfg->inj_en.port, cfg->inj_en.pad, PAL_MODE_OUTPUT_PUSHPULL);
		palClearPort(cfg->inj_en.port, PAL_PORT_BIT(cfg->inj_en.pad));
	}
	for (int i = 0; i < TLE8888_DIRECT_MISC; i++) {
		if (cfg->direct_io[i].port) {
// TODO: we need this but that's incompatible configuration change
//			ret |= gpio_pin_markUsed(cfg->direct_io[i].port, cfg->direct_io[i].pad, DRIVER_NAME " DIRECT IO");
//			palSetPadMode(cfg->direct_io[i].port, cfg->direct_io[i].pad, PAL_MODE_OUTPUT_PUSHPULL);
//			palClearPort(cfg->direct_io[i].port, PAL_PORT_BIT(cfg->direct_io[i].pad));
		}
	}


	if (ret) {
		ret = -1;
		goto err_gpios;
	}

	return 0;

err_gpios:
	/* unmark pins */
	//gpio_pin_markUnused(cfg->spi_config.ssport, cfg->spi_config.sspad);
	if (cfg->inj_en.port != NULL)
		gpio_pin_markUnused(cfg->inj_en.port, cfg->inj_en.pad);
	if (cfg->ign_en.port != NULL)
		gpio_pin_markUnused(cfg->ign_en.port, cfg->ign_en.pad);
	if (cfg->reset.port != NULL)
		gpio_pin_markUnused(cfg->reset.port, cfg->reset.pad);
	for (int i = 0; i < TLE8888_DIRECT_MISC; i++) {
		if (cfg->direct_io[i].port) {
// TODO: we need this but that's incompatible configuration change
//			gpio_pin_markUnused(cfg->direct_io[i].port, cfg->direct_io[i].pad);
		}
	}

	return ret;
}

/* DEBUG */
void tle8888_read_reg(uint16_t reg, uint16_t *val)
{
	struct tle8888_priv *chip = &chips[0];

	tle8888_spi_rw(chip, CMD_R(reg), val);
}

static int tle8888_init(void * data)
{
	int ret;
	struct tle8888_priv *chip;

	chip = (struct tle8888_priv *)data;

	/* check for multiple init */
	if (chip->drv_state != TLE8888_WAIT_INIT)
		return -1;

	ret = tle8888_chip_init(chip);
	if (ret)
		return ret;

	chip->drv_state = TLE8888_READY;

	/* one task for all TLE8888 instances, so create only once */
	if (!drv_task_ready) {
		chThdCreateStatic(tle8888_thread_1_wa, sizeof(tle8888_thread_1_wa),
						  NORMALPRIO + 1, tle8888_driver_thread, NULL);
		drv_task_ready = true;
	}

	return 0;
}

static int tle8888_deinit(void *data)
{
	struct tle8888_priv *chip = (struct tle8888_priv *)data;
	const struct tle8888_config	*cfg = chip->cfg;

	/* disable pins */
	if (cfg->ign_en.port)
		palClearPort(cfg->ign_en.port, PAL_PORT_BIT(cfg->ign_en.pad));
	if (cfg->inj_en.port)
		palClearPort(cfg->inj_en.port, PAL_PORT_BIT(cfg->inj_en.pad));

	/* TODO: stop task? */
	return 0;
}

struct gpiochip_ops tle8888_ops = {
	.setPadMode	= tle8888_setPadMode,
	.writePad	= tle8888_writePad,
	.readPad	= NULL,	/* chip outputs only */
	.getDiag	= tle8888_getDiag,
	.init		= tle8888_init,
	.deinit 	= tle8888_deinit,
};

/**
 * @brief TLE8888 driver add.
 * @details Checks for valid config
 * @return return gpio chip base
 */

int tle8888_add(unsigned int index, const struct tle8888_config *cfg) {

	efiAssert(OBD_PCM_Processor_Fault, cfg != NULL, "8888CFG", 0)

	/* no config or no such chip */
	if ((!cfg) || (!cfg->spi_bus) || (index >= BOARD_TLE8888_COUNT))
		return -1;

	/* check for valid chip select.
	 * TODO: remove this check? CS can be driven by SPI */
	if (cfg->spi_config.ssport == NULL)
		return -1;

	struct tle8888_priv *chip = &chips[index];

	/* already initted? */
	if (chip->cfg != NULL)
		return -1;

	chip->cfg = cfg;
	chip->o_state = 0;
	chip->o_state_cached = 0;
	chip->o_direct_mask = 0;
	chip->drv_state = TLE8888_WAIT_INIT;

	/* register, return gpio chip base */
	int ret = gpiochip_register(DRIVER_NAME, &tle8888_ops, TLE8888_OUTPUTS, chip);

	/* set default pin names, board init code can rewrite */
	gpiochips_setPinNames(ret, tle8888_pin_names);

	return ret;
}

#else /* BOARD_TLE8888_COUNT > 0 */

int tle8888_add(unsigned int index, const struct tle8888_config *cfg)
{
	(void)index; (void)cfg;

	return -1;
}

#endif /* (BOARD_TLE8888_COUNT > 0) */
