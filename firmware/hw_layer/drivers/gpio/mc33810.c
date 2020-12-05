/*
 * mc33810.c
 *
 * Automotive Engine Control IC
 *
 * @date Dec 29, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 * @date Jan 03, 2020
 * @author Andrey Gusakov <dron0gus@gmail.com>, (c) 2020
 */

#include "global.h"
#include "gpio/gpio_ext.h"
#include "gpio/mc33810.h"
#include "pin_repository.h"
#include "os_util.h"

#if (BOARD_MC33810_COUNT > 0)

/*
 * TODO list:
 *
 */

/*==========================================================================*/
/* Driver local definitions.												*/
/*==========================================================================*/

#define DRIVER_NAME				"mc33810"

static bool drv_task_ready = false;

typedef enum {
	MC33810_DISABLED = 0,
	MC33810_WAIT_INIT,
	MC33810_READY,
	MC33810_FAILED
} mc33810_drv_state;

#define MC_CMD_READ_REG(reg)			(0x0a00 | (((reg) & 0x0f) << 4))
#define MC_CMD_SPI_CHECK				(0x0f00)
#define MC_CMD_MODE_SELECT(mode)		(0x1000 | ((mode) & 0x0fff))
#define MC_CMD_LSD_FAULT(en)			(0x2000 | ((en) & 0x0fff))
#define MC_CMD_DRIVER_EN(en)			(0x3000 | ((en) & 0x00ff))
#define MC_CMD_SPARK(spark)				(0x4000 | ((spark) & 0x0fff))
#define MC_CMD_END_SPARK_FILTER(filt)	(0x5000 | ((filt) & 0x0003))
#define MC_CMD_DAC(dac)					(0x6000 | ((dac) & 0x0fff))
#define MC_CMD_GPGD_SHORT_THRES(sh)		(0x7000 | ((sh) & 0x0fff))
#define MC_CMD_GPGD_SHORT_DUR(dur)		(0x8000 | ((dur) & 0x0fff))
#define MC_CMD_GPGD_FAULT_OP(op)		(0x9000 | ((op) & 0x0f0f))
#define MC_CMD_PWM(pwm)					(0xa000 | ((pwm) & 0x0fff))
#define MC_CMD_CLK_CALIB				(0xe000)

/* get command code from TX value */
#define TX_GET_CMD(v)				(((v) >> 12) & 0x000f)

/* enum? */
#define REG_ALL_STAT				(0x0)
#define REG_OUT10_FAULT				(0x1)
#define REG_OUT32_FAULT				(0x2)
#define REG_GPGD_FAULT				(0x3)
#define REG_IGN_FAULT				(0x4)
#define REG_MODE_CMD				(0x5)
#define REG_LSD_FAULT_CMD			(0x6)
#define REG_DRIVER_EN				(0x7)
#define REG_SPARK_CMD				(0x8)
#define REG_END_SPARK_FILTER		(0x9)
#define REG_DAC_CMD					(0xa)
#define REG_GPGD_SHORT_THRES		(0xb)
#define REG_GPGD_SHORT_TIMER		(0xc)
#define REG_GPGD_FAULT_OP			(0xd)
#define REG_PWM						(0xe)
#define REG_REV						(0xf)

/* 0000.1101.0000.1010b */
#define SPI_CHECK_ACK				(0x0d0a)

/*==========================================================================*/
/* Driver exported variables.												*/
/*==========================================================================*/

/*==========================================================================*/
/* Driver local variables and types.										*/
/*==========================================================================*/

/* OS */
SEMAPHORE_DECL(mc33810_wake, 10 /* or BOARD_MC33810_COUNT ? */);
static THD_WORKING_AREA(mc33810_thread_1_wa, 256);

/* Driver */
struct mc33810_priv {
	const struct mc33810_config	*cfg;
	/* cached output state - state last send to chip */
	uint8_t					o_state_cached;
	/* state to be sended to chip */
	uint8_t					o_state;
	/* direct driven output mask */
	uint8_t					o_direct_mask;

	/* ALL STATUS RESPONSE value and flags */
	bool					all_status_requested;
	bool					all_status_updated;
	uint16_t				all_status_value;

	/* OUTx fault registers */
	uint16_t				out_fault[2];
	/* GPGD mode fault register */
	uint16_t				gpgd_fault;
	/* IGN mode fault register */
	uint16_t				ign_fault;

	mc33810_drv_state		drv_state;
};

static struct mc33810_priv chips[BOARD_MC33810_COUNT];

static const char* mc33810_pin_names[MC33810_OUTPUTS] = {
	"mc33810.OUT1",		"mc33810.OUT2",		"mc33810.OUT3",		"mc33810.OUT4",
	"mc33810.GD0",		"mc33810.GD1",		"mc33810.GD2",		"mc33810.GD3",
};

/*==========================================================================*/
/* Driver local functions.													*/
/*==========================================================================*/

static SPIDriver *get_bus(struct mc33810_priv *chip)
{
	/* return non-const SPIDriver* from const struct cfg */
	return chip->cfg->spi_bus;
}

/**
 * @brief MC33810 send and receive routine.
 * @details Sends and receives 16 bits. CS asserted before and released
 * after transaction.
 */

static int mc33810_spi_rw(struct mc33810_priv *chip, uint16_t tx, uint16_t *rx)
{
	uint16_t rxb;
	SPIDriver *spi = get_bus(chip);

	/* Acquire ownership of the bus. */
	spiAcquireBus(spi);
	/* Setup transfer parameters. */
	spiStart(spi, &chip->cfg->spi_config);
	/* Slave Select assertion. */
	spiSelect(spi);
	/* Atomic transfer operations. */
	/* TODO: check why spiExchange transfers invalid data on STM32F7xx, DMA issue? */
	//spiExchange(spi, 2, &tx, &rxb);
	rxb = spiPolledExchange(spi, tx);
	/* Slave Select de-assertion. */
	spiUnselect(spi);
	/* Ownership release. */
	spiReleaseBus(spi);

	if (rx)
		*rx = rxb;

	/* if reply on previous command is ALL STATUS RESPONSE */
	if (chip->all_status_requested) {
		chip->all_status_value = rxb;
		chip->all_status_updated = true;
	}

	/* if next reply will be ALL STATUS RESPONSE */
	chip->all_status_requested =
		(((TX_GET_CMD(tx) >= 0x1) && (TX_GET_CMD(tx) <= 0xa)) ||
		 (tx == MC_CMD_READ_REG(REG_ALL_STAT)));

	/* no errors for now */
	return 0;
}

/**
 * @brief MC33810 send output state data.
 * @details Sends ORed data to register, also receive diagnostic.
 */

static int mc33810_update_output_and_diag(struct mc33810_priv *chip)
{
	int ret = 0;

	/* TODO: lock? */

	/* we need to get updates status */
	chip->all_status_updated = false;

	/* if any pin is driven over SPI */
	if (chip->o_direct_mask != 0xff) {
		uint16_t out_data;

		out_data = chip->o_state & (~chip->o_direct_mask);
		ret = mc33810_spi_rw(chip, MC_CMD_DRIVER_EN(out_data), NULL);
		if (ret)
			return ret;
		chip->o_state_cached = chip->o_state;
	}

	/* this comlicated logic to save few spi transfers in case we will receive status as reply on other command */
	if (!chip->all_status_requested) {
		ret = mc33810_spi_rw(chip, MC_CMD_READ_REG(REG_ALL_STAT), NULL);
		if (ret)
			return ret;
	}
	/* get reply */
	if (!chip->all_status_updated) {
		ret = mc33810_spi_rw(chip, MC_CMD_READ_REG(REG_ALL_STAT), NULL);
		if (ret)
			return ret;
	}
	/* now we have updated ALL STATUS register in chip data */

	/* check OUT (injectors) first */
	if (chip->all_status_value & 0x000f) {
		/* request diagnostic of OUT0 and OUT1 */
		ret = mc33810_spi_rw(chip, MC_CMD_READ_REG(REG_OUT10_FAULT), NULL);
		if (ret)
			return ret;
		/* get diagnostic for OUT0 and OUT1 and request diagnostic for OUT2 and OUT3 */
		ret = mc33810_spi_rw(chip, MC_CMD_READ_REG(REG_OUT32_FAULT), &chip->out_fault[0]);
		if (ret)
			return ret;
		/* get diagnostic for OUT2 and OUT2 and requset ALL STATUS */
		ret = mc33810_spi_rw(chip, MC_CMD_READ_REG(REG_ALL_STAT), &chip->out_fault[1]);
		if (ret)
			return ret;
	}
	/* check GPGD - mode not supported yet */
	if (chip->all_status_value & 0x00f0) {
		/* request diagnostic of GPGD */
		ret = mc33810_spi_rw(chip, MC_CMD_READ_REG(REG_GPGD_FAULT), NULL);
		if (ret)
			return ret;
		/* get diagnostic for GPGD and requset ALL STATUS */
		ret = mc33810_spi_rw(chip, MC_CMD_READ_REG(REG_ALL_STAT), &chip->gpgd_fault);
		if (ret)
			return ret;
	}
	/* check IGN  */
	if (chip->all_status_value & 0x0f00) {
		/* request diagnostic of IGN */
		ret = mc33810_spi_rw(chip, MC_CMD_READ_REG(REG_IGN_FAULT), NULL);
		if (ret)
			return ret;
		/* get diagnostic for IGN and requset ALL STATUS */
		ret = mc33810_spi_rw(chip, MC_CMD_READ_REG(REG_ALL_STAT), &chip->ign_fault);
		if (ret)
			return ret;
	}

	/* TODO: unlock? */

	return ret;
}

/**
 * @brief MC33810 chip init.
 * @details Checks communication. Check chip presense.
 */

static int mc33810_chip_init(struct mc33810_priv *chip)
{
	int n;
	int ret;
	uint16_t rx;
	const struct mc33810_config *cfg = chip->cfg;

	/* mark pins used */
	//ret = gpio_pin_markUsed(cfg->spi_config.ssport, cfg->spi_config.sspad, DRIVER_NAME " CS");
	ret = 0;
	if (cfg->en.port != NULL)
		ret |= gpio_pin_markUsed(cfg->en.port, cfg->en.pad, DRIVER_NAME " EN");
	for (n = 0; n < MC33810_DIRECT_OUTPUTS; n++)
		if (cfg->direct_io[n].port)
			ret |= gpio_pin_markUsed(cfg->direct_io[n].port, cfg->direct_io[n].pad, DRIVER_NAME " DIRECT IO");

	if (ret) {
		ret = -1;
		goto err_gpios;
	}

	/* check SPI communication */
	/* 0. set echo mode, chip number - don't care */
	ret  = mc33810_spi_rw(chip, MC_CMD_SPI_CHECK, NULL);
	/* 1. check loopback */
	ret |= mc33810_spi_rw(chip, MC_CMD_READ_REG(REG_REV), &rx);
	if (ret) {
		ret = -1;
		goto err_gpios;
	}
	if (rx != SPI_CHECK_ACK) {
		//print(DRIVER_NAME " spi loopback test failed\n");
		ret = -2;
		goto err_gpios;
	}

	/* 2. read revision */
	ret  = mc33810_spi_rw(chip, MC_CMD_READ_REG(REG_ALL_STAT), &rx);
	if (ret) {
		ret = -1;
		goto err_gpios;
	}
	if (rx & (1 << 14)) {
		//print(DRIVER_NAME " spi COR status\n");
		ret = -3;
		goto err_gpios;
	}

	/* TODO:
	 * - setup
	 * - enable output drivers
	 * - read diagnostic
	 */

	/* n. sen EN pin high */
	if (cfg->en.port != NULL) {
		palSetPort(cfg->en.port,
				   PAL_PORT_BIT(cfg->en.pad));
	}

	return 0;

err_gpios:
	/* unmark pins */
	//gpio_pin_markUnused(cfg->spi_config.ssport, cfg->spi_config.sspad);
	if (cfg->en.port != NULL)
		gpio_pin_markUnused(cfg->en.port, cfg->en.pad);
	for (n = 0; n < MC33810_DIRECT_OUTPUTS; n++)
		if (cfg->direct_io[n].port)
			gpio_pin_markUnused(cfg->direct_io[n].port, cfg->direct_io[n].pad);

	return ret;
}

/**
 * @brief MC33810 chip driver wakeup.
 * @details Wake up driver. Will cause output register and
 * diagnostic update.
 */

static int mc33810_wake_driver(struct mc33810_priv *chip)
{
	(void)chip;

	if (isIsrContext()) {
		// this is for normal runtime
		syssts_s sts = chSysGetStatusAndLockX();
		chSemSignalI(&mc33810_wake);
		chSysRestoreStatusX(sts);
	} else {
		// this is for start-up to not hang up
		chSemSignal(&mc33810_wake);
	}

	return 0;
}

/*==========================================================================*/
/* Driver thread.															*/
/*==========================================================================*/

static THD_FUNCTION(mc33810_driver_thread, p)
{
	int i;
	msg_t msg;

	(void)p;

	chRegSetThreadName(DRIVER_NAME);

	while(1) {
		msg = chSemWaitTimeout(&mc33810_wake, TIME_MS2I(MC33810_POLL_INTERVAL_MS));

		/* should we care about msg == MSG_TIMEOUT? */
		(void)msg;

		for (i = 0; i < BOARD_MC33810_COUNT; i++) {
			int ret;
			struct mc33810_priv *chip;

			chip = &chips[i];
			if ((chip->cfg == NULL) ||
				(chip->drv_state == MC33810_DISABLED) ||
				(chip->drv_state == MC33810_FAILED))
				continue;

			/* TODO: implemet indirect driven gpios */
			ret = mc33810_update_output_and_diag(chip);
			if (ret) {
				/* set state to MC33810_FAILED? */
			}
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

int mc33810_writePad(void *data, unsigned int pin, int value)
{
	struct mc33810_priv *chip;

	if ((pin >= MC33810_OUTPUTS) || (data == NULL))
		return -1;

	chip = (struct mc33810_priv *)data;

	/* TODO: lock */
	if (value)
		chip->o_state |=  (1 << pin);
	else
		chip->o_state &= ~(1 << pin);
	/* TODO: unlock */
	/* direct driven? */
	if (chip->o_direct_mask & (1 << pin)) {
		/* TODO: ensure that output driver enabled */
		if (value)
			palSetPort(chip->cfg->direct_io[pin].port,
					   PAL_PORT_BIT(chip->cfg->direct_io[pin].pad));
		else
			palClearPort(chip->cfg->direct_io[pin].port,
					   PAL_PORT_BIT(chip->cfg->direct_io[pin].pad));
	} else {
		mc33810_wake_driver(chip);
	}

	return 0;
}

brain_pin_diag_e mc33810_getDiag(void *data, unsigned int pin)
{
	int val;
	struct mc33810_priv *chip;
	brain_pin_diag_e diag = PIN_OK;

	if ((pin >= MC33810_DIRECT_OUTPUTS) || (data == NULL))
		return PIN_INVALID;

	chip = (struct mc33810_priv *)data;

	if (pin < 4) {
		/* OUT drivers */
		val = chip->out_fault[pin < 2 ? 0 : 1] >> (4 * (pin & 0x01));

		/* ON open fault */
		if (val & BIT(0))
			diag |= PIN_OPEN;
		/* OFF open fault */
		if (val & BIT(1))
			diag |= PIN_OPEN;
		if (val & BIT(2))
			diag |= PIN_SHORT_TO_BAT;
		if (val & BIT(3))
			diag |= PIN_DRIVER_OVERTEMP;
	} else {
		/* INJ drivers, GPGD mode is not supported */
		val = chip->ign_fault >> (3 * (pin - 4));

		/* open load */
		if (val & BIT(0))
			diag |= PIN_OPEN;
		/* max Dwell fault - too long coil charge time */
		if (val & BIT(1))
			diag |= PIN_OVERLOAD;
		/* MAXI fault - too high coil current */
		if (val & BIT(2))
			diag |= PIN_OVERLOAD;
	}
	/* convert to some common enum? */
	return diag;
}

int mc33810_init(void * data)
{
	int ret;
	struct mc33810_priv *chip;

	chip = (struct mc33810_priv *)data;

	ret = mc33810_chip_init(chip);
	if (ret)
		return ret;

	chip->drv_state = MC33810_READY;

	if (!drv_task_ready) {
		chThdCreateStatic(mc33810_thread_1_wa, sizeof(mc33810_thread_1_wa),
						  NORMALPRIO + 1, mc33810_driver_thread, NULL);
		drv_task_ready = true;
	}

	return 0;
}

int mc33810_deinit(void *data)
{
	(void)data;

	/* TODO: set all pins to inactive state, stop task? */
	return 0;
}

struct gpiochip_ops mc33810_ops = {
	.writePad	= mc33810_writePad,
	.readPad	= NULL,	/* chip outputs only */
	.getDiag	= mc33810_getDiag,
	.init		= mc33810_init,
	.deinit 	= mc33810_deinit,
};

/**
 * @brief MC33810 driver add.
 * @details Checks for valid config
 */

int mc33810_add(unsigned int index, const struct mc33810_config *cfg)
{
	int i;
	int ret;
	struct mc33810_priv *chip;

	/* no config or no such chip */
	if ((!cfg) || (!cfg->spi_bus) || (index >= BOARD_MC33810_COUNT))
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
	chip->o_direct_mask = 0;
	chip->drv_state = MC33810_WAIT_INIT;
	for (i = 0; i < MC33810_DIRECT_OUTPUTS; i++) {
		if (cfg->direct_io[i].port != 0)
			chip->o_direct_mask |= (1 << i);
	}

	/* GPGD mode is not supported yet, ignition mode does not support spi on/off commands
	 * so ignition signals should be directly driven */
	if ((chip->o_direct_mask & 0xf0) != 0xf0)
		return -1;

	/* register, return gpio chip base */
	ret = gpiochip_register(DRIVER_NAME, &mc33810_ops, MC33810_OUTPUTS, chip);
	if (ret < 0)
		return ret;

	/* set default pin names, board init code can rewrite */
	gpiochips_setPinNames(ret, mc33810_pin_names);

	chip->drv_state = MC33810_WAIT_INIT;

	return ret;
}

#else /* BOARD_MC33810_COUNT > 0 */

int mc33810_add(unsigned int index, const struct mc33810_config *cfg)
{
	(void)index; (void)cfg;

	return -1;
}

#endif /* BOARD_MC33810_COUNT */
