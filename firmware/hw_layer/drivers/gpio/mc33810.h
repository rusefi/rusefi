/*
 * mc33810.h
 *
 * Automotive Engine Control IC
 *
 * @date Dec 29, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 * @date Jan 03, 2020
 * @author Andrey Gusakov <dron0gus@gmail.com>, (c) 2020
 */

#pragma once

#include "efifeatures.h"
#include <hal.h>
#include "rusefi_types.h"

#define MC33810_INJ_OUTPUTS			4
#define MC33810_IGN_OUTPUTS			4

#define MC33810_OUTPUTS				(MC33810_INJ_OUTPUTS + MC33810_IGN_OUTPUTS)
#define MC33810_DIRECT_OUTPUTS		MC33810_OUTPUTS

/* TODO: add irq support */
#define MC33810_POLL_INTERVAL_MS	100

struct mc33810_config {
#if HAL_USE_SPI
	SPIDriver	*spi_bus;
	SPIConfig	spi_config;
#endif
	/* First 4 is injector drivers, then 4 ignition pre-drivers */
	struct {
		ioportid_t		port;
		uint_fast8_t	pad;
	} direct_io[MC33810_DIRECT_OUTPUTS];
	struct {
		ioportid_t		port;
		uint_fast8_t	pad;
	} en;
	/* See datasheet:
	 * "To avoid any spurious data, it is essential the high-to-low and low-to-high
	 * transitions of the CS signal occur only when SCLK is in a logic low state."
	 * If sck.port != null, driver will wait until SCLK goes low before releasing CS
	 * at the end of each transaction.
	 * STM32 driver/hw is known to affected by this issue */
	struct {
		ioportid_t		port;
		uint_fast8_t	pad;
	} sck;

	/* TODO: fix mix of port+pad vs brain_pin_e */
	/* Spark duration signal input, active low */
	brain_pin_e			spkdur;
	/* Nominal ignition coil current flag signal input */
	brain_pin_e			nomi;
	/* Maximum ignition coil current flag signal input */
	brain_pin_e			maxi;
};

#if EFI_PROD_CODE && !EFI_BOOTLOADER && (BOARD_MC33810_COUNT > 0)

#include "gpio_ext.h"
#include "mc33810_state_generated.h"

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

/* Driver */
struct Mc33810 : public GpioChip, public mc33810_state_s {

	int init() override;

	int writePad(size_t pin, int value) override;
	brain_pin_diag_e getDiag(size_t pin) override;
	void debug() override;

	// TODO: make something private?
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

extern Mc33810 mc33810_chips[BOARD_MC33810_COUNT];
constexpr mc33810_state_s* mc33810getLiveData(size_t idx) {
	return (idx >= BOARD_MC33810_COUNT) ? nullptr : &mc33810_chips[idx];
}

/* debug */
void mc33810_req_init();
int getMc33810maxDwellTimer(mc33810maxDwellTimer_e value);

#endif // EFI_PROD_CODE && !EFI_BOOTLOADER && (BOARD_MC33810_COUNT > 0)

int mc33810_add(brain_pin_e base, unsigned int index, const mc33810_config *cfg);
