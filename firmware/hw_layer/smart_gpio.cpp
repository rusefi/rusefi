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
#include "gpio_ext.h"
#include "drivers/gpio/tle6240.h"
#include "drivers/gpio/mc33972.h"
#include "drivers/gpio/tle8888.h"

EXTERN_CONFIG;

#if (BOARD_TLE6240_COUNT > 0)
struct tle6240_config tle6240 = {
	.spi_bus = NULL /* TODO software lookup &SPID4 */,
	.spi_config = {
		.circular = false,
		.end_cb = NULL,
		.ssport = GPIOF,
		.sspad = 0U,
		.cr1 =
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
		/* IN1  - D_TACH_OUT */
		[0] = {.port = GPIOG, .pad = 2},
		/* IN2..4 grounded */
		[1] = {.port = NULL},
		[2] = {.port = NULL},
		[3] = {.port = NULL},
		/* IN9  - D_INJ_5 */
		[4] = {.port = GPIOD, .pad = 15},
		/* IN10 - D_WASTGATE */
		[5] = {.port = GPIOD, .pad = 14},
		/* IN11 - D_IDLE_OPEN */
		[6] = {.port = GPIOC, .pad = 6},
		/* IN12 - D_IDLE_CLOSE */
		[7] = {.port = GPIOC, .pad = 7},
	},
	.reset = {.port = GPIOG, .pad = 3}
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

#if (BOARD_TLE6240_COUNT > 0)
	tle6240.spi_bus = getSpiDevice(engineConfiguration->tle6240spiDevice);
	tle6240_add(0, &tle6240);
#elif (BOARD_EXT_GPIOCHIPS > 0)
	gpiochip_use_gpio_base(TLE6240_OUTPUTS);
#endif /* (BOARD_TLE6240_COUNT > 0) */

#if (BOARD_MC33972_COUNT > 0)
	mc33972.spi_bus = getSpiDevice(engineConfiguration->mc33972spiDevice);
	// todo: propogate 'basePinOffset' parameter
	mc33972_add(0, &mc33972);
#elif (BOARD_EXT_GPIOCHIPS > 0)
	gpiochip_use_gpio_base(MC33972_INPUTS);
#endif /* (BOARD_MC33972_COUNT > 0) */


#if (BOARD_TLE8888_COUNT > 0)
	if (engineConfiguration->tle8888_cs != GPIO_UNASSIGNED) {
		static OutputPin tle8888Cs;
//		// SPI pins are enabled in initSpiModules()
		tle8888Cs.initPin("tle8888 CS", engineConfiguration->tle8888_cs,
					&engineConfiguration->tle8888_csPinMode);
	}

	initTle8888(PASS_ENGINE_PARAMETER_SIGNATURE);
#endif /* (BOARD_TLE6240_COUNT > 0) */

#if (BOARD_EXT_GPIOCHIPS > 0)
	/* external chip init */
	gpiochips_init();
#endif
}

#endif /* EFI_PROD_CODE */
