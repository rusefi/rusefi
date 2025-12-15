/*
 * mc33810.c
 *
 * Automotive Engine Control IC
 * MCZ33810
 * eight channel output driver: four integrated low-side drivers and four low-side gate pre-drivers.
 *
 *
 * @date Dec 29, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 * @date Jan 03, 2020
 * @author Andrey Gusakov <dron0gus@gmail.com>, (c) 2020
 *
 * Masks bits/inputs numbers:
 * 0..3   - OUT1 .. 3 - Low-Side Injector drivers, 4.5A max
 *						driven through DIN0 .. 3 or SPI (not supported)
 * 4..7   -  GD0 .. 3 - Gate Driver outputs - IGBT or MOSFET pre-drivers,
 *						driven throug GIN0 .. 3 or SPI in GPGD mode (not supported)
 */

#include "pch.h"
#include "gpio/gpio_ext.h"
#include "gpio/mc33810.h"

#include "mc33810_state_generated.h"

#if EFI_PROD_CODE && (BOARD_MC33810_COUNT > 0)

// For exti irq
#include "digital_input_exti.h"

/*
 * TODO list:
 *
 */

/*==========================================================================*/
/* Driver local definitions.												*/
/*==========================================================================*/

#define DRIVER_NAME				"mc33810"

typedef enum {
	MC33810_DISABLED = 0,
	MC33810_WAIT_INIT,
	MC33810_READY,
	MC33810_FAILED
} mc33810_drv_state;

typedef enum {
	COIL_IDLE = 0,
	COIL_WAIT_SPARK_START,
	COIL_WAIT_SPARK_END
} mc33810_coil_state;

#define MC_CMD_READ_REG(reg)			(0x0a00 | (((reg) & 0x0f) << 4))
#define MC_CMD_SPI_CHECK				(0x0f00)
#define MC_CMD_MODE_SELECT(mode)		(0x1000 | ((mode) & 0x0fff))
/* unused
#define MC_CMD_LSD_FAULT(en)			(0x2000 | ((en) & 0x0fff))
*/
#define MC_CMD_DRIVER_EN(en)			(0x3000 | ((en) & 0x00ff))
#define MC_CMD_SPARK(spark)				(0x4000 | ((spark) & 0x0fff))
/* unused
#define MC_CMD_END_SPARK_FILTER(filt)	(0x5000 | ((filt) & 0x0003))
*/
#define MC_CMD_DAC(dac)					(0x6000 | ((dac) & 0x0fff))
/* unused
#define MC_CMD_GPGD_SHORT_THRES(sh)		(0x7000 | ((sh) & 0x0fff))
#define MC_CMD_GPGD_SHORT_DUR(dur)		(0x8000 | ((dur) & 0x0fff))
#define MC_CMD_GPGD_FAULT_OP(op)		(0x9000 | ((op) & 0x0f0f))
*/
#define MC_CMD_PWM(pwm)					(0xa000 | ((pwm) & 0x0fff))
/* unused
#define MC_CMD_CLK_CALIB				(0xe000)
*/

#define MC_CMD_INVALID					(0xf000)

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

#define REP_FLAG_RESET				BIT(15)
/* Command Out of Range */
#define REP_FLAG_COR				BIT(14)
/* Supply Out of Range */
#define REP_FLAG_SOR				BIT(13)
/* Over Voltage (Vpwr > 39V) */
#define REP_FLAG_OV					BIT(13)
/* Low Voltage (Vpwr < 5.3 .. 8.99V) */
#define REP_FLAG_LV					BIT(12)
/* Some V10-specific Fault */
#define REP_FLAG_NMF				BIT(12)

/*==========================================================================*/
/* Driver exported variables.												*/
/*==========================================================================*/

/*==========================================================================*/
/* Driver local variables and types.										*/
/*==========================================================================*/

/* OS */
static thread_t *mc33810_thread = NULL;
SEMAPHORE_DECL(mc33810_wake, 10 /* or BOARD_MC33810_COUNT ? */);
static THD_WORKING_AREA(mc33810_thread_wa, 256);

#define INJ_MASK		0x0f
#define IGN_MASK		0xf0

/* Driver */
struct Mc33810 : public GpioChip, public mc33810_state_s {
	int init() override;

	int writePad(size_t pin, int value) override;
	brain_pin_diag_e getDiag(size_t pin) override;
	void debug() override;

	// internal functions
	int spi_unselect();
	int spi_rw(uint16_t tx, uint16_t* rx);
	int spi_rw_array(const uint16_t *tx, uint16_t *rx, int n);
	int update_output_and_diag();

	int chip_init();
	void wake_driver();

	void ign_event(size_t pin, int value);
	void on_spkdur(efitick_t now);

	int chip_init_data();

	const mc33810_config	*cfg;

	/* cached output state - state last send to chip */
	uint8_t					o_state_cached;
	/* state to be sent to chip */
	uint8_t					o_state;
	/* direct driven output mask */
	uint8_t					o_direct_mask;
	/* IGN/GPGD mode bits: [7:4] - GP3..GP0 */
	uint8_t 				o_gpgd_mask;

	/* ALL STATUS RESPONSE value and flags */
	bool					all_status_updated;
	uint16_t				all_status_value;

	/* OUTx fault registers */
	uint16_t				out_fault[2];
	/* GP mode fault register */
	/* TODO: check documentation if these faults also applied to GPx outputs in IGN mode */
	uint16_t				gp_fault;
	/* IGN mode fault register */
	uint16_t				ign_fault;

	uint16_t				recentTx;

	/* SPKDUR handling */
	struct {
		ioportid_t		port;
		uint_fast8_t	pad;
	} spkdur;
	mc33810_coil_state 		coil_state;
	uint8_t					active_coil_idx;	/* zero based, used as index of spark[] array */
	uint8_t					spark_fault_mask;	/* 4 LSB bits are not used */
	efitick_t				spartStart[MC33810_IGN_OUTPUTS];
	int						spark_sync_err;

	/* statistic */
	int						rst_cnt;
	int						cor_cnt;
	int 					sor_cnt;
	int 					ov_cnt;
	int 					lv_cnt;

	mc33810_drv_state		drv_state;

	bool hadSuccessfulInit = false;
};

static Mc33810 chips[BOARD_MC33810_COUNT];

static const char* mc33810_pin_names[MC33810_OUTPUTS] = {
	"mc33810.OUT1",		"mc33810.OUT2",		"mc33810.OUT3",		"mc33810.OUT4",
	"mc33810.GD0",		"mc33810.GD1",		"mc33810.GD2",		"mc33810.GD3",
};

/*==========================================================================*/
/* Driver local functions.													*/
/*==========================================================================*/

inline bool isCor(uint16_t rx) {
	return rx & REP_FLAG_COR;
}

static void mc33810_spkdur_cb(void *ptr, efitick_t now);

/**
 * @brief MC33810 spi CS release helper with workaround
 * @details Will wait until SCK = low before releasing CS
 */

int Mc33810::spi_unselect()
{
	int retry = 0;
	SPIDriver *spi = cfg->spi_bus;

	if (cfg->sck.port) {
		/* Lets poll for SCK=0... spiPolledExchange() returns while SPI HW is
		 * still active and did not set SCK low yet. So do ot drive CS high until
		 * SCK is low. This polling should not take much time. But anyway we have
		 * timeout exit. */
		while (palReadPad(cfg->sck.port, cfg->sck.pad) && (++retry < 1000)) {
			/* NOP */
		}
	}

	/* Slave Select de-assertion. */
	spiUnselect(spi);

	if (retry < 1000) {
		return 0;
	}

	efiPrintf(DRIVER_NAME "failed wait for SCK = 0");

	return -1;
}


/**
 * @brief MC33810 send and receive routine.
 * @details Sends and receives 16 bits. CS asserted before and released
 * after transaction.
 */

int Mc33810::spi_rw(uint16_t tx, uint16_t *rx_ptr)
{
	uint16_t rx;
	SPIDriver *spi = cfg->spi_bus;

	/* Acquire ownership of the bus. */
	spiAcquireBus(spi);
	/* Setup transfer parameters. */
	spiStart(spi, &cfg->spi_config);
	/* Slave Select assertion. */
	spiSelect(spi);
	/* Atomic transfer operations. */
	/* TODO: check why spiExchange transfers invalid data on STM32F7xx, DMA issue? */
	//spiExchange(spi, 2, &tx, &rxb);
	rx = spiPolledExchange(spi, tx);
	/* Slave Select de-assertion. */
	spi_unselect();
	/* Ownership release. */
	spiReleaseBus(spi);

	if (rx_ptr)
		*rx_ptr = rx;

	if (recentTx != MC_CMD_INVALID) {
		/* update statistic counters - common flags */
		if (rx & REP_FLAG_RESET)
			rst_cnt++;
		if (isCor(rx))
			cor_cnt++;

		if (((TX_GET_CMD(recentTx) >= 0x1) && (TX_GET_CMD(recentTx) <= 0xa)) ||
			 (recentTx == MC_CMD_READ_REG(REG_ALL_STAT))) {
			/* if reply on previous command is ALL STATUS RESPONSE */
			all_status_value = rx;
			all_status_updated = true;
			/* update statistic counters - ALL STATUS flags */
			if (rx & REP_FLAG_SOR)
				sor_cnt++;
			/* ignore NFM */
		} else {
			/* Some READ REGISTER reply with address != REG_ALL_STAT */
			if (rx & REP_FLAG_OV)
				ov_cnt++;
			if (rx & REP_FLAG_LV)
				lv_cnt++;
		}
	}

	/* store currently tx'ed value to know what to expect on next rx */
	recentTx = tx;
#if 0
	efiPrintf(DRIVER_NAME "SPI [%x][%x]", tx, rx);
#endif
	/* no errors for now */
	return 0;
}

/**
 * @return <0 in case of communication error or invalid argument
 */
int Mc33810::spi_rw_array(const uint16_t *tx, uint16_t *rx, int n)
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
		/* data transfer */
		uint16_t rxdata = spiPolledExchange(spi, tx[i]);
		if (rx)
			rx[i] = rxdata;
		/* Slave Select de-assertion. */
		spi_unselect();

		/* Parse reply */
		if (recentTx != MC_CMD_INVALID) {
			/* update statistic counters - common flags */
			if (rxdata & REP_FLAG_RESET)
				rst_cnt++;
			if (isCor(rxdata))
				cor_cnt++;

			if (((TX_GET_CMD(recentTx) >= 0x1) && (TX_GET_CMD(recentTx) <= 0xa)) ||
				 (recentTx == MC_CMD_READ_REG(REG_ALL_STAT))) {
				/* if reply on previous command is ALL STATUS RESPONSE */
				all_status_value = rxdata;
				all_status_updated = true;
				/* update statistic counters - ALL STATUS flags */
				if (rxdata & REP_FLAG_SOR)
					sor_cnt++;
				/* ignore NFM */
			} else {
				/* Some READ REGISTER reply with address != REG_ALL_STAT */
				if (rxdata & REP_FLAG_OV)
					ov_cnt++;
				if (rxdata & REP_FLAG_LV)
					lv_cnt++;
			}
		}

		/* store currently tx'ed value to know what to expect on next rx */
		recentTx = tx[i];

		if (ret < 0) {
			recentTx = MC_CMD_INVALID;
			break;
		}
	}

	/* Ownership release. */
	spiReleaseBus(spi);

	/* no errors for now */
	return ret;
}

/**
 * @brief MC33810 send output state data.
 * @details Sends ORed data to register, also receive diagnostic.
 */

int Mc33810::update_output_and_diag()
{
	int ret = 0;

	uint16_t out_data = o_state & (~o_direct_mask);
	const uint16_t tx[] = {
		// we will get ALL STATUS RESPONSE as reply on following commad
		// value will be stored inside spi_rw_array() call, no need to care
		// TODO: WTF?
		(uint16_t)MC_CMD_DRIVER_EN(out_data),
		MC_CMD_READ_REG(REG_OUT10_FAULT),
		MC_CMD_READ_REG(REG_OUT32_FAULT),
		MC_CMD_READ_REG(REG_GPGD_FAULT),
		MC_CMD_READ_REG(REG_IGN_FAULT),
		MC_CMD_READ_REG(REG_ALL_STAT)
	};
	uint16_t rx[efi::size(tx)];

	/* we need to get updated status */
	all_status_updated = false;

	ret = spi_rw_array(tx, rx, efi::size(tx));

	if (ret == 0) {
		/* the content of the requested register is transmitted with the
		 * next SPI transmission */

		/* TODO: lock? */
		out_fault[0] = rx[1 + 1];
		out_fault[1] = rx[2 + 1];
		gp_fault = rx[3 + 1];
		ign_fault = rx[4 + 1];

		alive_cnt++;
		/* TODO: unlock? */
	}

	return ret;
}

int Mc33810::chip_init_data()
{
	int ret;

	/* mark pins used */
	//ret = gpio_pin_markUsed(cfg->spi_config.ssport, cfg->spi_config.sspad, DRIVER_NAME " CS");
	ret = 0;
	if (cfg->en.port) {
		ret |= gpio_pin_markUsed(cfg->en.port, cfg->en.pad, DRIVER_NAME " EN");
		palSetPadMode(cfg->en.port, cfg->en.pad, PAL_MODE_OUTPUT_PUSHPULL);
		palSetPort(cfg->en.port, PAL_PORT_BIT(cfg->en.pad));
	}

	for (int n = 0; n < MC33810_DIRECT_OUTPUTS; n++) {
		if (cfg->direct_io[n].port) {
			ret |= gpio_pin_markUsed(cfg->direct_io[n].port, cfg->direct_io[n].pad, DRIVER_NAME " DIRECT IO");
			palSetPadMode(cfg->direct_io[n].port, cfg->direct_io[n].pad, PAL_MODE_OUTPUT_PUSHPULL);
			palClearPort(cfg->direct_io[n].port, PAL_PORT_BIT(cfg->direct_io[n].pad));
		}
	}

	if (ret) {
		ret = -6;
		efiPrintf(DRIVER_NAME " error binding pin(s)");
		goto err_gpios;
	}

	/* check if we support SPKDUR */
	if (isBrainPinValid(cfg->spkdur) && brain_pin_is_onchip(cfg->spkdur)) {
		ret = efiExtiEnablePin(DRIVER_NAME "SPKDUR", cfg->spkdur, PAL_EVENT_MODE_BOTH_EDGES, mc33810_spkdur_cb,
			reinterpret_cast<void*>(this));
		if (ret) {
			efiPrintf(DRIVER_NAME " error requesting SPKDUR input IRQ: %d", ret);
			// This is not critical
			ret = 0;
			goto exit;
		}
		spkdur.port = getHwPort(DRIVER_NAME, cfg->spkdur);
		spkdur.pad = getHwPin(DRIVER_NAME, cfg->spkdur);
	} else {
		spkdur.port = nullptr;
	}

	return 0;

err_gpios:
	/* unmark pins */
	//gpio_pin_markUnused(cfg->spi_config.ssport, cfg->spi_config.sspad);
#if SMART_CHIPS_UNMARK_ON_FAIL
	if (cfg->en.port) {
		/* disable and mark unused */
		palSetPort(cfg->en.port,
				   PAL_PORT_BIT(cfg->en.pad));
		gpio_pin_markUnused(cfg->en.port, cfg->en.pad);
	}

	for (int n = 0; n < MC33810_DIRECT_OUTPUTS; n++) {
		if (cfg->direct_io[n].port) {
			gpio_pin_markUnused(cfg->direct_io[n].port, cfg->direct_io[n].pad);
		}
	}
#endif

exit:
	return ret;
}

/**
 * @brief MC33810 chip init.
 * @details Checks communication. Check chip presence.
 */

int Mc33810::chip_init()
{
	int ret;
	uint16_t rx;
	uint16_t rxSpiCheck;

// duplication with mc33810spiErrorCounter?
	init_cnt++;

	/* we do not know last CMD was sent (if was) */
	recentTx = MC_CMD_INVALID;

	/* check SPI communication */
	/* 0. set echo mode, chip number - don't care,
	 * NOTE: chip replyes on NEXT spi transaction */
	ret  = spi_rw(MC_CMD_SPI_CHECK, &rxSpiCheck);
	/* 1. check loopback */
	ret |= spi_rw(MC_CMD_READ_REG(REG_REV), &rx);
	if (ret) {
		ret = -7;
		efiPrintf(DRIVER_NAME " first SPI RX failed");
		goto err_exit;
	}
	if (rx != SPI_CHECK_ACK) {
		engine->outputChannels.mc33810spiErrorCounter++;
		static Timer needBatteryMessage;
		float vBatt = Sensor::getOrZero(SensorType::BatteryVoltage);
		if (vBatt > 6 || needBatteryMessage.getElapsedSeconds() > 7) {
			needBatteryMessage.reset();
			const char *msg;
			if (rx == 0xffff) {
				msg = "No power?";
			} else if (isCor(rx)) {
				msg = "COR";
			} else {
				msg = "unexpected";
			}
			efiPrintf(DRIVER_NAME " spi loopback test failed [first 0x%04x][spi check 0x%04x][%s] vBatt=%f count=%d", rxSpiCheck, rx, msg, vBatt,
			  engine->outputChannels.mc33810spiErrorCounter);
		}
		ret = -2;
		goto err_exit;
	}

	/* 2. read revision */
	ret  = spi_rw(MC_CMD_READ_REG(REG_ALL_STAT), &rx);
	if (ret) {
		ret = -8;
		efiPrintf(DRIVER_NAME " revision failed");
		goto err_exit;
	}
	if (isCor(rx)) {
		efiPrintf(DRIVER_NAME " spi COR status");
		ret = -3;
		goto err_exit;
	}

	/* TODO:
	 * - setup
	 * - enable output drivers
	 * - read diagnostic
	 */

	{
		uint16_t spark_cmd =
			// Table 11. Maximum Dwell Timer
			(engineConfiguration->mc33810maxDwellTimer << 9) |
			BIT(8) |	/* enable max dwell control */
			(3 << 2) |	/* Open Secondary OSFLT = 100 uS, default */
			(1 << 0) |	/* End Spark THreshold: VPWR +5.5V, default */
			0;
		ret = spi_rw(MC_CMD_SPARK(spark_cmd), NULL);
		if (ret) {
			efiPrintf(DRIVER_NAME " cmd spark");
			goto err_exit;
		}

		uint16_t nomi_current = 0x0a;	// default = 5.5 A
		float nomi = engineConfiguration->mc33810Nomi;
		if ((nomi >= 3.0) && (nomi <= 10.75)) {
			nomi_current = (nomi - 3.0) / 0.25;
		}

		uint16_t maxi_current = 0x08;	// default = 14.0 A
		float maxi = engineConfiguration->mc33810Maxi;
		if ((maxi >= 6.0) && (maxi <= 21.0)) {
			maxi_current = maxi - 6.0;
		}
		uint16_t dac_cmd =
			// Table 12. Nominal Current DAC Select
			((nomi_current & 0x1f) << 0) |
			// Table 10. Overlapping Dwell Compensation, defaul 35%
			(0x4 << 5) |
			// Table 13. Maximum Current DAC Select
			((maxi_current & 0xf) << 8) |
			0;
		ret = spi_rw(MC_CMD_DAC(dac_cmd), NULL);
		if (ret) {
			efiPrintf(DRIVER_NAME " cmd dac");
			goto err_exit;
		}

		/* update local configuration mask */
		o_gpgd_mask =
			(engineConfiguration->mc33810Gpgd0Mode << 4) |
			(engineConfiguration->mc33810Gpgd1Mode << 5) |
			(engineConfiguration->mc33810Gpgd2Mode << 6) |
			(engineConfiguration->mc33810Gpgd3Mode << 7);

		// TODO: add check of o_direct_mask against o_gpgd_mask
		// Check is some of IGN mode channels have no direct drive gpio assigned

		uint16_t mode_select_cmd =
			/* set IGN/GP mode for GPx outputs: [7:4] to [11:8] */
			((o_gpgd_mask & 0xf0) << 4) |
			/* disable/enable retry after recovering from under/overvoltage */
			(engineConfiguration->mc33810DisableRecoveryMode << 6) |
			0;
		ret = spi_rw(MC_CMD_MODE_SELECT(mode_select_cmd) , NULL);
		if (ret) {
			efiPrintf(DRIVER_NAME " cmd mode select");
			goto err_exit;
		}
	}

	/* n. set EN pin low - active */
	if (cfg->en.port) {
		palClearPort(cfg->en.port,
					 PAL_PORT_BIT(cfg->en.pad));
	}

	if (!hadSuccessfulInit) {
		efiPrintf(DRIVER_NAME " Successful Init");
		hadSuccessfulInit = true;
	}

	return 0;

err_exit:
	return ret;
}

/**
 * @brief MC33810 chip driver wakeup.
 * @details Wake up driver. Will cause output register and
 * diagnostic update.
 */

void Mc33810::wake_driver()
{
	/* Entering a reentrant critical zone.*/
	chibios_rt::CriticalSectionLocker csl;
	chSemSignalI(&mc33810_wake);
	if (!port_is_isr_context()) {
		/**
		 * chSemSignalI above requires rescheduling
		 * interrupt handlers have implicit rescheduling
		 */
		chSchRescheduleS();
	}
}

/**
 * @brief MC33810 SPKDUR event hook.
 * @details Called on falling and rising edges of SPKDUR input.
 */

void Mc33810::on_spkdur(efitick_t now)
{
	if (coil_state == COIL_IDLE) {
		/* ignore spurious events */
		return;
	}

	bool edge = palReadPad(spkdur.port, spkdur.pad);

	/* signal is active low */
	if ((!edge) && (coil_state == COIL_WAIT_SPARK_START)) {
		/* expected falling edge */
		spartStart[active_coil_idx] = now;
		coil_state = COIL_WAIT_SPARK_END;
	} else if ((edge) && (coil_state == COIL_WAIT_SPARK_END)) {
		/* expected rise edge */
		sparkDuration[active_coil_idx] = USF2MS(NT2USF(now - spartStart[active_coil_idx]));
		/* clear fault flag */
		spark_fault_mask &= ~BIT(MC33810_INJ_OUTPUTS + active_coil_idx);
		coil_state = COIL_IDLE;
	} else {
		/* unexpected event */
		spark_sync_err++;
		sparkDuration[active_coil_idx] = 0;
		spark_fault_mask |= BIT(MC33810_INJ_OUTPUTS + active_coil_idx);
		coil_state = COIL_IDLE;
	}
}

/**
 * @brief MC33810 ignition inputs event handler.
 * @details Called right before ignition input (GIN0..GIN3) changes its state.
 */

void Mc33810::ign_event(size_t pin, int value)
{
	/* SPKDUR not routed to MCU */
	if (spkdur.port == nullptr) {
		return;
	}

	uint8_t pin_mask = BIT(pin);
	uint8_t new_o_state = o_state;

	if (value) {
		new_o_state |=  pin_mask;
	} else {
		new_o_state &= ~pin_mask;
	}

	/* nothing's going change */
	if (o_state == new_o_state)
		return;

	if (value) {
		/* coil charge starting */
		/* nothing to do here, we can still wait SPKDUR event from another coil */
	} else {
		size_t idx = pin - MC33810_INJ_OUTPUTS;
		/* coil firing */
		/* if we did not get some event for previously fired coil... */
		if (coil_state != COIL_IDLE) {
			/* ...mark this coil as failed */
			spark_fault_mask |= BIT(MC33810_INJ_OUTPUTS + active_coil_idx);
			sparkDuration[active_coil_idx] = 0;
		}

		active_coil_idx = idx;
		coil_state = COIL_WAIT_SPARK_START;
	}
}

/*==========================================================================*/
/* Driver thread.															*/
/*==========================================================================*/

static THD_FUNCTION(mc33810_driver_thread, p) {
	(void)p;

	chRegSetThreadName(DRIVER_NAME);

	chThdSleepMilliseconds(2); // let's wait BatteryVoltage to appear. TODO: more proper way of synchronization with BatteryVoltage!


	while (true) {
		msg_t msg = chSemWaitTimeout(&mc33810_wake, TIME_MS2I(MC33810_POLL_INTERVAL_MS));

		/* should we care about msg == MSG_TIMEOUT? */
		(void)msg;

		for (int i = 0; i < BOARD_MC33810_COUNT; i++) {
			auto chip = &chips[i];

			if (i == 0) {
					engine->engineState.smartChipRestartCounter = chip->init_cnt;
					engine->engineState.smartChipAliveCounter = chip->alive_cnt;
			}

			if (chip->need_init) {
				int ret = chip->chip_init();
				if (ret == 0) {
					chip->drv_state = MC33810_READY;
					chip->need_init = false;
				}
			}

			if ((chip->cfg == NULL) ||
				(chip->drv_state == MC33810_DISABLED) ||
				(chip->drv_state == MC33810_FAILED)) {
				chip->need_init = true;
				continue;
			}

			/* TODO: implement indirect driven gpios */
			int ret = chip->update_output_and_diag();
			if (ret) {
				/* set state to MC33810_FAILED? */
			}
		}
	}
}

/*==========================================================================*/
/* Driver interrupt handlers.												*/
/*==========================================================================*/

static void mc33810_spkdur_cb(void *ptr, efitick_t now)
{
	Mc33810 *chip = (Mc33810 *)ptr;

	chip->on_spkdur(now);
}

/*==========================================================================*/
/* Driver exported functions.												*/
/*==========================================================================*/

int Mc33810::writePad(size_t pin, int value) {
	uint8_t pin_mask = BIT(pin);

	if (pin >= MC33810_OUTPUTS) {
		return -12;
	}

	{
		// mutate driver state under lock
		chibios_rt::CriticalSectionLocker csl;

		if (pin_mask & IGN_MASK) {
			ign_event(pin, value);
		}

		if (value) {
			o_state |=  pin_mask;
		} else {
			o_state &= ~pin_mask;
		}
	}

	/* direct driven? */
	if (o_direct_mask & pin_mask) {
		/* TODO: ensure that output driver enabled */
#if MC33810_VERBOSE
		int pad = PAL_PORT_BIT(cfg->direct_io[pin].pad);
		efiPrintf(DRIVER_NAME "writePad pad %d", pad);
#endif
		if (value) {
			palSetPort(cfg->direct_io[pin].port,
					   PAL_PORT_BIT(cfg->direct_io[pin].pad));
		} else {
			palClearPort(cfg->direct_io[pin].port,
						 PAL_PORT_BIT(cfg->direct_io[pin].pad));
		}
	} else {
		wake_driver();
	}

	return 0;
}

brain_pin_diag_e Mc33810::getDiag(size_t pin)
{
	uint16_t val;
	int diag = PIN_OK;

	if (pin >= MC33810_DIRECT_OUTPUTS)
		return PIN_UNKNOWN;

	if (pin < MC33810_INJ_OUTPUTS) {
		/* OUT drivers */
		val = out_fault[(pin < 2) ? 0 : 1] >> (4 * (pin & 0x01));

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
		/* Commom GP faults */
		val = gp_fault >> (2 * (pin - 4));

		if (val & BIT(0))
			diag |= PIN_OPEN;
		if (val & BIT(1))
			diag |= PIN_SHORT_TO_GND;

		/* IGN mode faults, only if not in GPGD mode */
		if ((o_gpgd_mask & BIT(pin)) == 0) {
			val = ign_fault >> (3 * (pin - 4));

			/* open load */
			if (val & BIT(0))
				diag |= PIN_OPEN;
			/* max Dwell fault - too long coil charge time */
			if (val & BIT(1))
				diag |= PIN_OVERLOAD;
			/* MAXI fault - too high coil current */
			if (val & BIT(2))
				diag |= PIN_OVERLOAD;

			/* no SPKDUR detected */
			if (spark_fault_mask & BIT(pin))
				diag |= PIN_OPEN;

			/* too short spark time means there is oscilation on coil,
			 * that usualy because of open secondary (disconnected spark plug) */
			if (sparkDuration[pin - MC33810_IGN_OUTPUTS] < 0.150)
				diag |= PIN_OPEN;
		}
	}
	/* convert to some common enum? */
	return static_cast<brain_pin_diag_e>(diag);
}

void Mc33810::debug() {
	efiPrintf("rst_cnt %d cor_cnt %d sor_cnt %d ov_cnt %d lv_cnt %d\n",
		rst_cnt, cor_cnt, sor_cnt, ov_cnt, lv_cnt);

	for (size_t i = 0; i < MC33810_IGN_OUTPUTS; i++) {
		efiPrintf("Ign %d spark fault %d last duration %f mS\n",
			i, !!(spark_fault_mask & BIT(MC33810_INJ_OUTPUTS + i)),
			sparkDuration[i]);
	}
}

int Mc33810::init() {
	int ret;

	/* check for multiple init */
	if (drv_state != MC33810_WAIT_INIT)
		return -1;

	ret = chip_init_data();
	if (ret)
		return ret;

	/* force init from driver thread */
	need_init = true;

	/* instance is ready */
	drv_state = MC33810_READY;

	if (!mc33810_thread) {
		mc33810_thread = chThdCreateStatic(mc33810_thread_wa, sizeof(mc33810_thread_wa),
										   PRIO_GPIOCHIP, mc33810_driver_thread, nullptr);
	}

	return 0;
}

/**
 * @brief MC33810 driver add.
 * @details Checks for valid config
 */

int mc33810_add(brain_pin_e base, unsigned int index, const mc33810_config *cfg) {

	/* no config or no such chip */
	if ((!cfg) || (!cfg->spi_bus) || (index >= BOARD_MC33810_COUNT))
		return -16;

	/* check for valid cs.
	 * TODO: remove this check? CS can be driven by SPI */
	//if (cfg->spi_config.ssport == NULL)
	//	return -1;

	Mc33810& chip = chips[index];

	/* already initted? */
	if (chip.cfg != NULL)
		return -13;

	chip.cfg = cfg;
	chip.o_state = 0;
	chip.o_state_cached = 0;
	chip.o_direct_mask = 0;
	for (int i = 0; i < MC33810_DIRECT_OUTPUTS; i++) {
		if (cfg->direct_io[i].port != 0)
			chip.o_direct_mask |= BIT(i);
	}

#if 0
	// Some Ignition channels may be unused, so skip this check for now
	/* GPGD mode is not supported yet, ignition mode does not support spi on/off commands
	 * so ignition signals should be directly driven */
	if ((chip.o_direct_mask & 0xf0) != 0xf0)
		return -4;
#endif

	/* register, return gpio chip base */
	int ret = gpiochip_register(base, DRIVER_NAME, chip, MC33810_OUTPUTS);
	if (ret < 0)
		return ret;

	/* set default pin names, board init code can rewrite */
	gpiochips_setPinNames(static_cast<brain_pin_e>(ret), mc33810_pin_names);

	chip.drv_state = MC33810_WAIT_INIT;

	return ret;
}

/*==========================================================================*/
/* Driver exported debug functions.												*/
/*==========================================================================*/
void mc33810_req_init() {
	size_t i;

	for (i = 0; i < BOARD_MC33810_COUNT; i++) {
		auto& chip = chips[i];

		chip.need_init = true;
	}
}

int getMc33810maxDwellTimer(mc33810maxDwellTimer_e value) {
switch(value) {
case DWELL_16MS:
  return 16;
case DWELL_2MS:
  return 2;
case DWELL_32MS:
  return 32;
case DWELL_4MS:
  return 4;
case DWELL_64MS:
  return 64;
case DWELL_8MS:
  return 8;
  }
 return 0;
}

const mc33810_state_s* mc33810getLiveData(size_t idx) {
	if (idx >= BOARD_MC33810_COUNT)
		return nullptr;
	return &chips[idx];
}

#else /* BOARD_MC33810_COUNT > 0 */

int mc33810_add(brain_pin_e base, unsigned int index, const mc33810_config *cfg)
{
	(void)base; (void)index; (void)cfg;

	return -5;
}

const mc33810_state_s* mc33810getLiveData(size_t) {
	return nullptr;
}

#endif /* BOARD_MC33810_COUNT */
