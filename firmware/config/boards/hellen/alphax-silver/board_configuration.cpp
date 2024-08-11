#include "pch.h"
#include "hellen_meta.h"
#include "i2c_bb.h"
#include "defaults.h"
#include "hellen_leds_100.cpp"
#include "smart_gpio.h"
#include "drivers/gpio/tle9104.h"
#include "pca_board_id.h"

static OutputPin alphaTempPullUp;

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = Gpio::TLE9104_0_OUT_0;
	engineConfiguration->injectionPins[1] = Gpio::TLE9104_0_OUT_1;
	engineConfiguration->injectionPins[2] = Gpio::TLE9104_0_OUT_2;
	engineConfiguration->injectionPins[3] = Gpio::TLE9104_0_OUT_3;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = Gpio::MM100_IGN1;
	engineConfiguration->ignitionPins[1] = Gpio::MM100_IGN2;
	engineConfiguration->ignitionPins[2] = Gpio::MM100_IGN3;
	engineConfiguration->ignitionPins[3] = Gpio::MM100_IGN4;
}

static void setupDefaultSensorInputs() {
	engineConfiguration->tps1_1AdcChannel = MM100_IN_TPS_ANALOG;

    engineConfiguration->boardUseTempPullUp = true;


//	engineConfiguration->map.sensor.hwChannel = H144_IN_MAP1; // external MAP
  engineConfiguration->map.sensor.hwChannel = H144_IN_MAP2; // On-board MAP
  engineConfiguration->map.sensor.type = MT_MPXH6400;

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
	setInjectorPins();
	setIgnitionPins();
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
			{ .port = GPIOA, .pad = 8 }, // tach
			{ .port = GPIOD, .pad = 15 }, // pump relay
			{ .port = GPIOD, .pad = 2 }, // idle valve
			{ .port = GPIOD, .pad = 12 } // fan relay
		},
		.resn = Gpio::Unassigned, //Gpio::B14,
		.en   = Gpio::Unassigned //Gpio::B15
	}
};

static void board_init_ext_gpios() {
  {
    /* Waste of RAM, switch to palSetPadMode() and palSetPort() */
    static OutputPin TleCs0;
	  TleCs0.initPin("TLE9104 CS0", Gpio::A15);
	  TleCs0.setValue(1);
	}
	{
    static OutputPin TleCs1;
	  TleCs1.initPin("TLE9104 CS1", Gpio::B12);
	  TleCs1.setValue(1);
	}

	{
	  static OutputPin TleReset;
	  TleReset.initPin("TLE9104 Reset", Gpio::B14);
	  TleReset.setValue(1);

	  static OutputPin TleEn;
	  TleEn.initPin("TLE9104 En", Gpio::B15);
	  TleEn.setValue(1);
	}

  initAll9104(tle9104_cfg);
}

/**
 * @brief Board-specific initialization code.
 * @todo  Add your board-specific code, if any.
 */
void boardInitHardware(void)
{
	alphaTempPullUp.initPin("a-temp", Gpio::MM100_IGN8); //  E6
	board_init_ext_gpios();
	boardOnConfigurationChange(nullptr); // TODO? invoke this from main firmware code not from board file?
}

void boardOnConfigurationChange(engine_configuration_s * /*previousConfiguration*/) {
	alphaTempPullUp.setValue(engineConfiguration->boardUseTempPullUp);
}

static Gpio OUTPUTS[] = {
	Gpio::TLE9104_0_OUT_3, // 1A - Injector 4
	Gpio::TLE9104_0_OUT_2, // 2A - Injector 3
	Gpio::TLE9104_0_OUT_1, // 3A - Injector 2
	Gpio::TLE9104_0_OUT_0, // 4A - Injector 1
	Gpio::TLE9104_1_OUT_1, // 5A Fuel Pump Relay
	Gpio::TLE9104_1_OUT_2, // 6A Idle Output
	Gpio::TLE9104_1_OUT_3, // 28A Fan Relay
	Gpio::TLE9104_1_OUT_0, // 14A Tach Output
	Gpio::MM100_IGN1,
	Gpio::MM100_IGN2,
	Gpio::MM100_IGN3,
	Gpio::MM100_IGN4,
};

int getBoardMetaOutputsCount() {
    return efi::size(OUTPUTS);
}

int getBoardMetaLowSideOutputsCount() {
    return getBoardMetaOutputsCount() - 4;
}

Gpio* getBoardMetaOutputs() {
    return OUTPUTS;
}
