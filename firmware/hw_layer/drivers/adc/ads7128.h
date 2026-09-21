#pragma once

#include "efifeatures.h"
#include <hal.h>
#include "rusefi_types.h"

#define ADS7128_IOS				8
/* this may be too slow? */
#define ADS7128_POLL_INTERVAL_MS	100
/*
 * Always assume 16 bits resolution as HW registers are aligned like that and
 * with enabled oversampling/averaging it actually corresponds to 16 bits.
 */
#define ADS71x8_RES_BITS		16
#define ADS7128_MAX_ADC_VALUE	((1 << ADS71x8_RES_BITS) - 1)

#include "i2c.h"

struct ads7128_config {
	i2c_bus_e	i2c_bus;
	uint8_t		i2c_addr;
	float		vref;
};

#if EFI_PROD_CODE && !EFI_BOOTLOADER && (BOARD_ADS7128_COUNT > 0)

#include "gpio_ext.h"
#include "ads7128_state_generated.h"

typedef enum {
	ADS7128_DISABLED = 0,
	ADS7128_WAIT_INIT,
	ADS7128_READY,
	ADS7128_FAILED
} ads7128_drv_state;

/* Driver */
struct Ads7128 : public GpioChip, public ads7128_state_s {
	int init() override;

	int setPadMode(size_t pin, iomode_t mode) override;
	int writePad(size_t pin, int value) override;
	int readPad(size_t pin) override;

	/* ADC part */
	float readAnalog(size_t pin);

	void debug() override;

	int chip_reset();
	int chip_init();

	int read_inputs();
	int update_gpios();

	void wake_driver();

	const ads7128_config	*cfg;

	i2cBus		*i2c;

	/* mask of IOs used in GPIO mode - PIN_CFG[7:0] */
	uint8_t		gpio_mask;
	/* mask of GPIO used in output mode - GPIO_CFG[7:0] */
	uint8_t		gpio_out_mask;
	/* mask of output GPIOs used in Push-Pull mode - GPO_DRIVE_CFG[7:0] */
	uint8_t		gpio_pp_mask;
	/* output GPOs state - GPO_VALUE[7:0] */
	uint8_t		gpio_out_state;
	/* input GPIs state - GPI_VALUE[7:0] */
	uint8_t		gpio_in_state;

	uint8_t		status;

	ads7128_drv_state		drv_state;
	int			errors = 0;

private:
	int regs_read(uint8_t reg, uint8_t *regs, size_t n);
	int regs_write(uint8_t reg, const uint8_t *regs, size_t n);
};

extern Ads7128 ads7128_chips[BOARD_ADS7128_COUNT];
constexpr ads7128_state_s* ads7128getLiveData(size_t idx) {
	return (idx >= BOARD_ADS7128_COUNT) ? nullptr : &ads7128_chips[idx];
}

#endif // EFI_PROD_CODE && !EFI_BOOTLOADER && (BOARD_ADS7128_COUNT > 0)

int ads7128_add(brain_pin_e base, unsigned int index, const ads7128_config *cfg);
