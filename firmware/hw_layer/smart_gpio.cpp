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
	extern struct tle6240_config tle6240;
#endif

#if (BOARD_MC33972_COUNT > 0)
	extern struct mc33972_config mc33972;
#endif

#if (BOARD_TLE8888_COUNT > 0)
/* this is homeless tle8888, please find better place for it */
static struct tle8888_config tle8888_cfg = {
	.spi_bus = NULL,
	.spi_config = {
		.circular = false,
		.end_cb = NULL,
		.ssport = GPIOF,
		.sspad = 0U,
#if defined(STM_F4_FAMILY)
		.cr1 =
			SPI_CR1_16BIT_MODE |
			SPI_CR1_SSM |
			SPI_CR1_SSI |
			SPI_CR1_LSBFIRST |	//LSB first
			((3 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |	// div = 16
			SPI_CR1_MSTR |
			SPI_CR1_CPHA |
			0,
		.cr2 = SPI_CR2_16BIT_MODE
#elif defined(STM_F7_FAMILY)
		.cr1 =
			SPI_CR1_16BIT_MODE |
			SPI_CR1_SSM |
			SPI_CR1_SSI |
			SPI_CR1_LSBFIRST |
			SPI_CR1_MSTR |
			((3 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |
			SPI_CR1_CPHA |
			0,
		.cr2 = SPI_CR2_16BIT_MODE
#else
		unexpected platform
#endif

	},
	.direct_io = {
		[0] = {.port = NULL,	.pad = 0,	.output = 9},
		[1] = {.port = NULL,	.pad = 0,	.output = 10},
		[2] = {.port = NULL,	.pad = 0,	.output = 11},
		[3] = {.port = NULL,	.pad = 0,	.output = 12},
	},
};
#endif

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
		// SPI pins are enabled in initSpiModules()
		tle8888Cs.initPin("tle8888 CS", engineConfiguration->tle8888_cs,
					&engineConfiguration->tle8888_csPinMode);

		// todo: reuse initSpiCs method?
		tle8888_cfg.spi_config.ssport = getHwPort("tle8888 CS", engineConfiguration->tle8888_cs);
		tle8888_cfg.spi_config.sspad = getHwPin("tle8888 CS", engineConfiguration->tle8888_cs);
		tle8888_cfg.spi_bus = getSpiDevice(engineConfiguration->tle8888spiDevice);

		/* spi_bus == null checked in _add function */
		ret = tle8888_add(0, &tle8888_cfg);

		efiAssertVoid(OBD_PCM_Processor_Fault, ret == TLE8888_PIN_1, "tle8888");
	}
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
