#include "pch.h"
#include "hellen_meta.h"
#include "defaults.h"
#include "hellen_leds_100.cpp"
#include "smart_gpio.h"
#include "drivers/gpio/tle9104.h"

//static void setInjectorPins() {
//
//}


static void setupDefaultSensorInputs() {
	engineConfiguration->tps1_1AdcChannel = MM100_IN_TPS_ANALOG;

	engineConfiguration->map.sensor.hwChannel = H144_IN_MAP1;
	engineConfiguration->clt.adcChannel = MM100_IN_CLT_ANALOG;
	engineConfiguration->iat.adcChannel = MM100_IN_IAT_ANALOG;
}

void setBoardConfigOverrides() {
	/* Force PWR_EN as TLE9104s are powered from +5VA */
	setHellenMegaEnPin();

	setHellenVbatt();
	setHellenCan();
	hellenMegaSdWithAccelerometer();
	setDefaultHellenAtPullUps();

	/* Two TLE9104 */
	enableHellenSpi3();
}

void setBoardDefaultConfiguration() {
	setHellenMMbaro();
	setupDefaultSensorInputs();
	setInline4();
}

/*
 * RESET and EN signals of both TLE9104 are driven by same gpios,
 * Do not allow TLE driver to drive this pins as it will reset
 * first chip while initing second.
 * Set pins to proper once.
 * TODO: improve?
 */
static const tle9104_config tle9104_cfg[BOARD_TLE9104_COUNT] = {
	{
		.spi_bus = &SPID3,
		.spi_config = {
			.circular = false,
			.end_cb = NULL,
			.ssport = GPIOA,
			.sspad = 15,
			.cr1 =
				SPI_CR1_16BIT_MODE |
				SPI_CR1_SSM |
				SPI_CR1_SSI |
				((3 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |	// div = 16
				SPI_CR1_MSTR |
				SPI_CR1_CPHA |
				0,
			.cr2 = SPI_CR2_16BIT_MODE
		},
		.direct_io = {
			{ .port = GPIOD, .pad = 3 },
			{ .port = GPIOA, .pad = 9 },
			{ .port = GPIOD, .pad = 11 },
			{ .port = GPIOD, .pad = 10 }
		},
		.resn = Gpio::Unassigned, //Gpio::B14,
		.en   = Gpio::Unassigned //Gpio::B15
	},
	{
		.spi_bus = &SPID3,
		.spi_config = {
			.circular = false,
			.end_cb = NULL,
			.ssport = GPIOB,
			.sspad = 12,
			.cr1 =
				SPI_CR1_16BIT_MODE |
				SPI_CR1_SSM |
				SPI_CR1_SSI |
				((3 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |	// div = 16
				SPI_CR1_MSTR |
				SPI_CR1_CPHA |
				0,
			.cr2 = SPI_CR2_16BIT_MODE
		},
		.direct_io = {
			{ .port = GPIOA, .pad = 8 },
			{ .port = GPIOD, .pad = 15 },
			{ .port = GPIOD, .pad = 2 },
			{ .port = GPIOD, .pad = 12 }
		},
		.resn = Gpio::Unassigned, //Gpio::B14,
		.en   = Gpio::Unassigned //Gpio::B15
	}
};

static OutputPin TleReset;
static OutputPin TleEn;
/* Waste of RAM, switch to palSetPadMode() and palSetPort() */
static OutputPin TleCs0;
static OutputPin TleCs1;

static void board_init_ext_gpios()
{
	int ret;

	TleCs0.initPin("TLE9104 CS0", Gpio::A15);
	TleCs0.setValue(1);
	TleCs1.initPin("TLE9104 CS1", Gpio::B12);
	TleCs1.setValue(1);

	TleReset.initPin("TLE9104 Reset", Gpio::B14);
	TleEn.initPin("TLE9104 En", Gpio::B15);
	TleReset.setValue(1);
	TleEn.setValue(1);

	ret = tle9104_add(Gpio::TLE9104_0_OUT_0, 0, &tle9104_cfg[0]);
	if (ret < 0) {
		/* error */
	}

	ret = tle9104_add(Gpio::TLE9104_1_OUT_0, 1, &tle9104_cfg[1]);
	if (ret < 0) {
		/* error */
	}
}

/**
 * @brief Board-specific initialization code.
 * @todo  Add your board-specific code, if any.
 */
void boardInitHardware(void)
{
	board_init_ext_gpios();
}
