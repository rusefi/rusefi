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
#include "common.h"

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

#define REG_INVALID			0x00

/* C0 */
#define CMD_READ			(0 << 0)
#define CMD_WRITE			(1 << 0)
/* C7:1 */
#define CMD_REG_ADDR(a)		(((a) & 0x7f) << 1)
/* CD7:0 */
#define CMD_REG_DATA(d)		(((d) & 0xff) << 8)

#define CMD_WR(a, d)		(CMD_WRITE | CMD_REG_ADDR(a) | CMD_REG_DATA(d))
#define CMD_R(a)			(CMD_READ | CMD_REG_ADDR(a))

#define CMD_CMD0(d)			CMD_WR(0x01, d)
/* Window watchdog open WWDOWT window time = 12.8 mS - fixed value for TLE8888QK */
#define CMD_WWDSERVICECMD	CMD_WR(0x15, 0x03)
#define CMD_FWDRESPCMD(d)	CMD_WR(0x16, d)
#define CMD_FWDRESPSYNCCMD(d)	CMD_WR(0x17, d)

#define CMD_SR_CODE			0x1a
#define CMD_SR				CMD_WR(CMD_SR_CODE, 0x03)
// 0x238 = 568
#define CMD_OE_SET			CMD_WR(0x1c, 0x02)
#define CMD_OE_CLR			CMD_WR(0x1c, 0x01)
#define CMD_UNLOCK			CMD_WR(0x1e, 0x01)
#define CMD_LOCK			CMD_WR(0x1e, 0x02)

/* Status registers */
#define REG_OPSTAT(n)		(0x34 + ((n) & 0x01))
#define CMD_OPSTAT(n)		CMD_R(REG_OPSTAT(n))
#define REG_WWDSTAT			0x36
#define CMD_WWDSTAT			CMD_R(REG_WWDSTAT)
#define REG_FWDSTAT(n)		(0x37 + ((n) & 0x01))
#define CMD_FWDSTAT(n)		CMD_R(REG_FWDSTAT(n))
#define REG_TECSTAT			0x39
#define CMD_TECSTAT			CMD_R(REG_TECSTAT)

/* Diagnostic */
#define REG_DIAG(n)			(0x20 + ((n) & 0x01))
#define CMD_DIAG(n)			CMD_R(REG_DIAG(n))
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

// Looks like reset value is 113.6ms? 1.6ms * 0x47
#define FWD_PERIOD_MS		(20)

/* Default closed window time is 0b0011.1111 * 1.6 = 100.8mS
 * Default open window time is 0b0011 * 3.2 = 12.8 mS */
#define WWD_PERIOD_MS		(100.8 + (12.8 / 2))

/* DOTO: add irq support */
#define DIAG_PERIOD_MS		(7)

const uint8_t tle8888_fwd_responses[16][4] = {
	/* Reverse order:
	 * RESP3,RESP2,RESP1,RESP0 */
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

	/* direct drive mapping registers */
	uint32_t					InConfig[TLE8888_DIRECT_MISC];

	/* last accessed register, for validation on next SPI access */
	uint8_t						last_reg;

	/* diagnostic registers */
	uint8_t						OutDiag[5];
	uint8_t						BriDiag[2];
	uint8_t						IgnDiag;
	/* status registers */
	uint8_t						OpStat[2];

	/* last diagnostic was read */
	systime_t					diag_ts;

	/* WD stuff */
	uint8_t						wwd_err_cnt;
	uint8_t						fwd_err_cnt;
	uint8_t						tot_err_cnt;
	uint8_t						wd_diag;
	bool						wd_happy;
	systime_t					wwd_ts;
	systime_t					fwd_ts;

	/* chip needs reintialization due to some critical issue */
	bool						need_init;

	/* statistic */
	int							por_cnt;
	int							wdr_cnt;
	int							comfe_cnt;
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

#define getWindowWatchdog()		(chips[0].wwd_err_cnt)
#define getFunctionalWatchdog()	(chips[0].fwd_err_cnt)
#define getTotalErrorCounter()	(chips[0].tot_err_cnt)

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
	debugChannels->debugFloatField4 = chips[0].por_cnt * 1000000 + requestedResetCounter * 10000 + lowVoltageResetCounter;
	debugChannels->debugFloatField5 = 0;
	debugChannels->debugFloatField6 = 0;
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

	uint8_t reg = getRegisterFromResponse(spiRxb);
	if ((chip->last_reg != REG_INVALID) && (chip->last_reg != reg)) {
		/* unexpected SPI answers */
		if (reg == REG_OPSTAT(0)) {
			/* after power on reset: the address and the content of the
			 * status register OpStat0 is transmitted with the next SPI
			 * transmission */
			chip->por_cnt++;
		} else if (reg == REG_FWDSTAT(1)) {
			/* after watchdog reset: the address and the content of the
			 * diagnosis register FWDStat1 is transmitted with the first
			 * SPI transmission after the low to high transition of RST */
			chip->wdr_cnt++;
		} else if (reg == REG_DIAG(0)) {
			/* after an invalid communication frame: the address and the
			 * content of the diagnosis register Diag0 is transmitted
			 * with the next SPI transmission and the bit COMFE in
			 * diagnosis register ComDiag is set to "1" */
			chip->comfe_cnt++;
		}
		/* during power on reset: SPI commands are ignored, SDO is always
		 * tristate */
		/* during watchdog reset: SPI commands are ignored, SDO has the
		 * value of the status flag */
		chip->need_init = true;
	}

	chip->last_reg = getRegisterFromResponse(tx);

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

	/* set value only for non-direct driven pins */
	out_data &= ~chip->o_direct_mask;

	/* output for push-pull pins is allways enabled
	 * (at least until we start supporting hi-Z state) */
	out_data |= chip->o_pp_mask;
	/* TODO: apply hi-Z mask when support will be added */

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

	/* request OutDiad0, ignore received */
	if ((ret = tle8888_spi_rw(chip, CMD_OUTDIAG(0), NULL)))
		return ret;

	/* request OutDiad1, receive OutDiag0 */
	if ((ret = tle8888_spi_rw(chip, CMD_OUTDIAG(1), &rx)))
		return ret;
	chip->OutDiag[0] = getDataFromResponse(rx);

	/* request OutDiad2, receive OutDiag1 */
	if ((ret = tle8888_spi_rw(chip, CMD_OUTDIAG(2), &rx)))
		return ret;
	chip->OutDiag[1] = getDataFromResponse(rx);

	/* request OutDiad3, receive OutDiag2 */
	if ((ret = tle8888_spi_rw(chip, CMD_OUTDIAG(3), &rx)))
		return ret;
	chip->OutDiag[2] = getDataFromResponse(rx);

	/* request OutDiad4, receive OutDiag3 */
	if ((ret = tle8888_spi_rw(chip, CMD_OUTDIAG(4), &rx)))
		return ret;
	chip->OutDiag[3] = getDataFromResponse(rx);

	/* request BriDiag0, receive OutDiag4 */
	if ((ret = tle8888_spi_rw(chip, CMD_BRIDIAG(0), &rx)))
		return ret;
	chip->OutDiag[4] = getDataFromResponse(rx);

	/* request BriDiag1, receive BriDiag0 */
	if ((ret = tle8888_spi_rw(chip, CMD_BRIDIAG(1), &rx)))
		return ret;
	chip->BriDiag[0] = getDataFromResponse(rx);

	/* request IgnDiag, receive BriDiag1 */
	if ((ret = tle8888_spi_rw(chip, CMD_IGNDIAG, &rx)))
		return ret;
	chip->BriDiag[1] = getDataFromResponse(rx);

	/* request OpStat0, receive IgnDiag */
	if ((ret = tle8888_spi_rw(chip, CMD_OPSTAT(0), &rx)))
		return ret;
	chip->IgnDiag = getDataFromResponse(rx);

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
	int index = -1;
	const struct tle8888_config	*cfg = chip->cfg;

	if (pin < 4) {
		/* OUT1..4 */
		index = pin;
	} else if (pin > 24) {
		/* IGN1..4 */
		index = (pin - 24) + 4;
	} else {
		/* find remapable direct drive gpio */
		for (int i = 0; i < TLE8888_DIRECT_MISC; i++) {
			/* again: outputs in cfg counted starting from 1 - hate this */
			if (cfg->direct_maps[i].output == pin + 1) {
				index = 8 + i;
				break;
			}
		}
	}

	/* direct gpio not found */
	if (index < 0)
		return -1;

	if (value)
		palSetPort(cfg->direct_gpio[index].port,
				   PAL_PORT_BIT(cfg->direct_gpio[index].pad));
	else
		palClearPort(cfg->direct_gpio[index].port,
				   PAL_PORT_BIT(cfg->direct_gpio[index].pad));
	return 0;
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

static int tle8888_chip_reset(struct tle8888_priv *chip) {
	int ret;

	ret = tle8888_spi_rw(chip, CMD_SR, NULL);
	/**
	 * Table 8. Reset Times. All reset times not more than 20uS
	 */
	chThdSleepMilliseconds(3);

	chip->last_reg = REG_INVALID;

	return ret;
}

static int tle8888_chip_set_lock(struct tle8888_priv *chip, int lock) {
	/* Set/Clear LOCK bit */
	return tle8888_spi_rw(chip, lock ? CMD_LOCK : CMD_UNLOCK, NULL);
}

static int tle8888_chip_init(struct tle8888_priv *chip) {
	const struct tle8888_config	*cfg = chip->cfg;

#if 0
	/* Read diagnostic and status to clear pending errors */
	tle8888_update_status_and_diag(chip);
#endif

#if 0
	/* Debug: disable diagnostic */
	for (int i = 0; i <= 5; i++) {
		tle8888_spi_rw(chip, CMD_OUTCONFIG(i, 0), NULL);
	}
#else
	/* defaults from datasheet */
	tle8888_spi_rw(chip, CMD_OUTCONFIG(0, 0xff), NULL);
	tle8888_spi_rw(chip, CMD_OUTCONFIG(1, 0x3f), NULL);
	tle8888_spi_rw(chip, CMD_OUTCONFIG(2, 0x3f), NULL);
	tle8888_spi_rw(chip, CMD_OUTCONFIG(3, 0x30), NULL);
	tle8888_spi_rw(chip, CMD_OUTCONFIG(4, 0x3f), NULL);
	tle8888_spi_rw(chip, CMD_OUTCONFIG(5, 0x3f), NULL);
#endif

	/* map direct driven channels */
	for (int i = 0; i < TLE8888_DIRECT_MISC; i++) {
		/* set INCONFIG - aux input mapping */
		tle8888_spi_rw(chip, CMD_INCONFIG(i, chip->InConfig[i]), NULL);
	}

	/* set OE and DD registers */
	for (int i = 0; i < 4; i++) {
		uint8_t dd;

		dd = (chip->o_direct_mask >> (8 * i)) & 0xff;
		tle8888_spi_rw(chip, CMD_DDCONFIG(i, dd), NULL);
	}
	for (int i = 0; i < 4; i++) {
		uint8_t oe;

		oe = (chip->o_oe_mask >> (8 * i)) & 0xff;
		tle8888_spi_rw(chip, CMD_OECONFIG(i, oe), NULL);
	}

	/* set VR mode: VRS/Hall */
	tle8888_spi_rw(chip, CMD_VRSCONFIG(1, chip->cfg->mode << 2), NULL);

	/* enable outputs */
	tle8888_spi_rw(chip, CMD_OE_SET, NULL);

	/* enable pins */
	if (cfg->ign_en.port)
		palSetPort(cfg->ign_en.port, PAL_PORT_BIT(cfg->ign_en.pad));
	if (cfg->inj_en.port)
		palSetPort(cfg->inj_en.port, PAL_PORT_BIT(cfg->inj_en.pad));

	//if (CONFIG(verboseTLE8888)) {
		tle8888_dump_regs();
	//}

#if 0
	/* Set LOCK bit to 1 */
	tle8888_spi_rw(chip, CMD_LOCK, NULL);
#endif

	return 0;
}

static int tle8888_wwd_feed(struct tle8888_priv *chip) {
	tle8888_spi_rw(chip, CMD_WWDSERVICECMD, NULL);

	return 0;
}

static int tle8888_fwd_feed(struct tle8888_priv *chip) {
	uint16_t reg;

	tle8888_spi_rw(chip, CMD_FWDSTAT(1), NULL);
	/* here we get response of the 'FWDStat1' above */
	tle8888_spi_rw(chip, CMD_WDDIAG, &reg);

	uint8_t data = getDataFromResponse(reg);
	uint8_t fwdquest = data & 0xF;
	uint8_t fwdrespc = (data >> 4) & 3;
	/* Table lines are filled in reverse order (like in DS) */
	uint8_t response = tle8888_fwd_responses[fwdquest][3 - fwdrespc];
	if (fwdrespc != 0) {
		tle8888_spi_rw(chip, CMD_FWDRESPCMD(response), NULL);
	} else {
		/* to restart heartbeat timer, sync command should be used for response 0 */
		tle8888_spi_rw(chip, CMD_FWDRESPSYNCCMD(response), NULL);
	}

	return 0;
}

static int tle8888_wd_get_status(struct tle8888_priv *chip) {
	uint16_t reg;

	tle8888_spi_rw(chip, CMD_WDDIAG, NULL);
	tle8888_spi_rw(chip, CMD_WDDIAG, &reg);

	chip->wd_diag = getDataFromResponse(reg);

	if (chip->wd_diag & 0x70) {
		/* Reset caused by TEC
		 * Reset caused by FWD
		 * Reset caused by WWD */
		 return -1;
	}
	if (chip->wd_diag & 0x0f) {
		/* Some error in WD handling */
		return 1;
	}

	return 0;
}

static int tle8888_wd_feed(struct tle8888_priv *chip) {
	bool update_status;

	if (chip->wwd_ts <= chVTGetSystemTimeX()) {
		update_status = true;
		if (tle8888_wwd_feed(chip) == 0) {
			chip->wwd_ts = chTimeAddX(chVTGetSystemTimeX(), TIME_MS2I(WWD_PERIOD_MS));
		}
	}

	if (chip->fwd_ts <= chVTGetSystemTimeX()) {
		update_status = true;
		if (tle8888_fwd_feed(chip) == 0) {
			chip->fwd_ts = chTimeAddX(chVTGetSystemTimeX(), TIME_MS2I(FWD_PERIOD_MS));
		}
	}

	if (update_status) {
		uint16_t wwd_reg, fwd_reg, tec_reg;

		tle8888_spi_rw(chip, CMD_WWDSTAT, NULL);
		tle8888_spi_rw(chip, CMD_FWDSTAT(0), &wwd_reg);
		tle8888_spi_rw(chip, CMD_TECSTAT, &fwd_reg);
		tle8888_spi_rw(chip, CMD_TECSTAT, &tec_reg);

		chip->wwd_err_cnt = getDataFromResponse(wwd_reg) & 0x7f;
		chip->fwd_err_cnt = getDataFromResponse(fwd_reg) & 0x7f;
		chip->tot_err_cnt = getDataFromResponse(tec_reg) & 0x7f;

		chip->wd_happy = ((chip->wwd_err_cnt == 0) &&
						  (chip->fwd_err_cnt == 0));

		return tle8888_wd_get_status(chip);
	} else {
		return 0;
	}
}

static int tle8888_calc_sleep_interval(struct tle8888_priv *chip) {
	systime_t now = chVTGetSystemTimeX();

	sysinterval_t wwd_delay = chTimeDiffX(now, chip->wwd_ts);
	sysinterval_t fwd_delay = chTimeDiffX(now, chip->fwd_ts);
	sysinterval_t diag_delay = chTimeDiffX(now, chip->diag_ts);

	sysinterval_t min = MIN(wwd_delay, MIN(fwd_delay, diag_delay));

	return (min > 0) ? min : 0;
}

/*==========================================================================*/
/* Driver thread.															*/
/*==========================================================================*/

static THD_FUNCTION(tle8888_driver_thread, p) {
	(void)p;

	chRegSetThreadName(DRIVER_NAME);

	while (1) {
		msg_t msg = chSemWaitTimeout(&tle8888_wake, tle8888_calc_sleep_interval(&chips[0]));

		/* should we care about msg == MSG_TIMEOUT? */
		(void)msg;

#if 0
		if (vBattForTle8888 < LOW_VBATT) {
			// we assume TLE8888 is down and we should not bother with SPI communication
			if (!needInitialSpi) {
				needInitialSpi = true;
				lowVoltageResetCounter++;
			}
			continue; // we should not bother communicating with TLE8888 until we have +12
		}
#endif
		// todo: super-lazy implementation with only first chip!
		//watchdogLogic(&chips[0]);

		for (int i = 0; i < BOARD_TLE8888_COUNT; i++) {
			int ret;
			struct tle8888_priv *chip = &chips[i];
			bool wd_happy = chip->wd_happy;

			if ((chip->cfg == NULL) ||
				(chip->drv_state == TLE8888_DISABLED) ||
				(chip->drv_state == TLE8888_FAILED))
				continue;

			/* update outputs only if WD is happy */
			if (wd_happy) {
				ret = tle8888_update_output(chip);
				if (ret) {
					/* set state to TLE8888_FAILED? */
				}
			}

			ret = tle8888_wd_feed(chip);
			if (ret < 0) {
				/* WD is not happy */
				continue;
			}
			/* happiness state has changed! */
			if ((chip->wd_happy != wd_happy) && (chip->wd_happy)) {
				chip->need_init = true;
			}

			if (chip->need_init) {
				/* clear first, as flag can be raised again during init */
				chip->need_init = false;
				/* re-init chip! */
				tle8888_chip_init(chip);
				/* sync pins state */
				tle8888_update_output(chip);
			}

			if (chip->diag_ts <= chVTGetSystemTimeX()) {
				/* this is expensive call, will do a lot of spi transfers... */
				ret = tle8888_update_status_and_diag(chip);
				if (ret) {
					/* set state to TLE8888_FAILED or force reinit? */
				}

				chip->diag_ts = chTimeAddX(chVTGetSystemTimeX(), TIME_MS2I(DIAG_PERIOD_MS));
			}
		}
	}
}

void requestTLE8888initialization(void) {
//	needInitialSpi = true;
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
#else
	(void)mode;
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

static int tle8888_chip_init_data(void * data) {
	int i;
	struct tle8888_priv *chip = (struct tle8888_priv *)data;
	const struct tle8888_config	*cfg = chip->cfg;

	int ret = 0;

	chip->o_direct_mask = 0;
	chip->o_oe_mask		= 0;
	chip->o_pp_mask		= 0;

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

	for (i = 0; i < TLE8888_DIRECT_OUTPUTS; i++) {
		int out = -1;
		uint32_t mask;

		if (i < 4) {
			/* injector */
			out = i;
		} else if (i < 8) {
			/* ignition */
			out = (i - 4) + 24;
		} else {
			/* remappable */
			/* in config counted from 1 */
			out = cfg->direct_maps[i - 8].output - 1;
		}

		if ((out < 0) || (cfg->direct_gpio[i].port == NULL))
			continue;

		/* calculate mask */
		mask = BIT(out);

		/* check if output already occupied */
		if (chip->o_direct_mask & mask) {
			/* incorrect config? */
			ret = -1;
			goto err_gpios;
		}

		/* configure source gpio */
		ret = gpio_pin_markUsed(cfg->direct_gpio[i].port, cfg->direct_gpio[i].pad, DRIVER_NAME " DIRECT IO");
		if (ret) {
			ret = -1;
			goto err_gpios;
		}
		palSetPadMode(cfg->direct_gpio[i].port, cfg->direct_gpio[i].pad, PAL_MODE_OUTPUT_PUSHPULL);
		palClearPort(cfg->direct_gpio[i].port, PAL_PORT_BIT(cfg->direct_gpio[i].pad));

		/* enable direct drive */
		chip->o_direct_mask	|= mask;

		/* calculate INCONFIG - aux input mapping for IN9..IN12 */
		if (i >= 8) {
			if ((out < 4) || (out >= 24)) {
				ret = -1;
				goto err_gpios;
			}
			chip->InConfig[i - 8] = out - 4;
		}
	}

	/* HACK HERE if you want to enable PP for OUT21..OUT24
	 * without approprirate call to setPinMode */
	//chip->o_pp_mask		|= BIT(20) | BIT(21) | BIT(22) | BIT(23);

	/* enable all direct driven */
	chip->o_oe_mask		|= chip->o_direct_mask;

	/* enable all ouputs
	 * TODO: add API to enable/disable? */
	chip->o_oe_mask		|= 0x0ffffff0;

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
	for (int i = 0; i < TLE8888_DIRECT_OUTPUTS; i++) {
		if (cfg->direct_gpio[i].port) {
			gpio_pin_markUnused(cfg->direct_gpio[i].port, cfg->direct_gpio[i].pad);
		}
	}

	return ret;
}

static int tle8888_init(void * data)
{
	int ret;
	struct tle8888_priv *chip;

	chip = (struct tle8888_priv *)data;

	/* check for multiple init */
	if (chip->drv_state != TLE8888_WAIT_INIT)
		return -1;

	ret = tle8888_chip_reset(chip);
	if (ret)
		return ret;

	ret = tle8888_chip_set_lock(chip, 0);
	if (ret)
		return ret;

	ret = tle8888_chip_init_data(chip);
	if (ret)
		return ret;

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

/*==========================================================================*/
/* Driver exported debug functions.												*/
/*==========================================================================*/
void tle8888_read_reg(uint16_t reg, uint16_t *val)
{
	struct tle8888_priv *chip = &chips[0];

	tle8888_spi_rw(chip, CMD_R(reg), val);
}

#else /* BOARD_TLE8888_COUNT > 0 */

int tle8888_add(unsigned int index, const struct tle8888_config *cfg)
{
	(void)index; (void)cfg;

	return -1;
}

#endif /* (BOARD_TLE8888_COUNT > 0) */
