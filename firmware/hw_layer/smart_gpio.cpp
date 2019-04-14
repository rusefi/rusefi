/*
 * @file smart_gpio.cpp
 *
 * @date Apr 13, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "global.h"

#if EFI_PROD_CODE
#include "smart_gpio.h"
#include "efi_gpio.h"
#include "engine_configuration.h"
#include "hardware.h"
#include "mpu_util.h"
#include "gpio_ext.h"
#include "drivers/gpio/tle6240.h"
#include "drivers/gpio/mc33972.h"
#include "drivers/gpio/tle8888.h"

EXTERN_CONFIG;

// todo: migrate to TS or board config
#ifndef TLE6240_SS_PORT
#define TLE6240_SS_PORT GPIOF
#endif /* TLE6240_SS_PORT */
#ifndef TLE6240_SS_PAD
#define TLE6240_SS_PAD 0U
#endif /* TLE6240_SS_PAD */
#ifndef TLE6240_RESET_PORT
#define TLE6240_RESET_PORT GPIOG
#endif /* TLE6240_RESET_PORT */
#ifndef TLE6240_RESET_PAD
#define TLE6240_RESET_PAD 3U
#endif /* TLE6240_RESET_PAD */
#ifndef TLE6240_DIRECT_IO
#define TLE6240_DIRECT_IO \
		/* IN1  - D_TACH_OUT */ \
		[0] = {.port = GPIOG, .pad = 2}, \
		/* IN2..4 grounded */ \
		[1] = {.port = NULL, .pad = 0}, \
		[2] = {.port = NULL, .pad = 0}, \
		[3] = {.port = NULL, .pad = 0}, \
		/* IN9  - D_INJ_5 */ \
		[4] = {.port = GPIOD, .pad = 15}, \
		/* IN10 - D_WASTGATE */ \
		[5] = {.port = GPIOD, .pad = 14}, \
		/* IN11 - D_IDLE_OPEN */ \
		[6] = {.port = GPIOC, .pad = 6}, \
		/* IN12 - D_IDLE_CLOSE */ \
		[7] = {.port = GPIOC, .pad = 7},
#endif /* TLE6240_DIRECT_IO */

#if (BOARD_TLE6240_COUNT > 0)
struct tle6240_config tle6240 = {
	.spi_bus = NULL /* TODO software lookup &SPID4 */,
	.spi_config = {
		.circular = false,
		.end_cb = NULL,
		.ssport = TLE6240_SS_PORT,
		.sspad = TLE6240_SS_PAD,
		.cr1 =
			SPI_CR1_16BIT_MODE |
			SPI_CR1_SSM |
			SPI_CR1_SSI |
			/* SPI_CR1_LSBFIRST | */
			((3 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |	/* div = 16 */
			SPI_CR1_MSTR |
			/* SPI_CR1_CPOL | */ // = 0
			SPI_CR1_CPHA | // = 1
			0,
		.cr2 = SPI_CR2_16BIT_MODE
	},
	.direct_io = {
		TLE6240_DIRECT_IO
	},
	.reset = {.port = TLE6240_RESET_PORT, .pad = TLE6240_RESET_PAD}
};
#endif /* (BOARD_TLE6240_COUNT > 0) */

#if (BOARD_MC33972_COUNT > 0)
struct mc33972_config mc33972 = {
	.spi_bus = NULL /* TODO software lookup &SPID4 */,
	.spi_config = {
		.circular = false,
		.end_cb = NULL,
		.ssport = GPIOB,
		.sspad = 4U,
		.cr1 =
			SPI_CR1_SSM |
			SPI_CR1_SSI |
			/* SPI_CR1_LSBFIRST | */
			((3 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |	/* div = 16 */
			SPI_CR1_MSTR |
			/* SPI_CR1_CPOL | */ /* = 0 */
			SPI_CR1_CPHA | /* = 1 */
			0,
		.cr2 = SPI_CR2_24BIT_MODE
	},
};
#endif /* (BOARD_MC33972_COUNT > 0) */

void initSmartGpio() {
	int ret;

#if (BOARD_TLE6240_COUNT > 0)
	tle6240.spi_bus = getSpiDevice(engineConfiguration->tle6240spiDevice);
	ret = tle6240_add(0, &tle6240);
	if (ret < 0)
#endif /* (BOARD_TLE6240_COUNT > 0) */
		/* whenever chip is disabled or error returned - occupy its gpio range */
		gpiochip_use_gpio_base(TLE6240_OUTPUTS);

#if (BOARD_MC33972_COUNT > 0)
	mc33972.spi_bus = getSpiDevice(engineConfiguration->mc33972spiDevice);
	// todo: propogate 'basePinOffset' parameter
	ret = mc33972_add(0, &mc33972);
	if (ret < 0)
#endif /* (BOARD_MC33972_COUNT > 0) */
		/* whenever chip is disabled or error returned - occupy its gpio range */
		gpiochip_use_gpio_base(MC33972_INPUTS);

#if (BOARD_TLE8888_COUNT > 0)
	if (engineConfiguration->tle8888_cs != GPIO_UNASSIGNED) {
		static OutputPin tle8888Cs;
//		// SPI pins are enabled in initSpiModules()
		tle8888Cs.initPin("tle8888 CS", engineConfiguration->tle8888_cs,
					&engineConfiguration->tle8888_csPinMode);
	}

	ret = initTle8888(PASS_ENGINE_PARAMETER_SIGNATURE);
	if (ret < 0)
#endif /* (BOARD_TLE6240_COUNT > 0) */
		/* whenever chip is disabled or error returned - occupy its gpio range */
		gpiochip_use_gpio_base(TLE8888_OUTPUTS);

#if (BOARD_EXT_GPIOCHIPS > 0)
	/* external chip init */
	gpiochips_init();
#endif
}

#endif /* EFI_PROD_CODE */
