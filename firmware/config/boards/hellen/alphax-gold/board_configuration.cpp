#include "pch.h"
#include "hellen_meta.h"
#include "defaults.h"
#include "smart_gpio.h"
#include "drivers/gpio/tle9104.h"

static OutputPin alphaTempPullUp;

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = Gpio::TLE9104_0_OUT_0;
	engineConfiguration->injectionPins[1] = Gpio::TLE9104_0_OUT_1;
	engineConfiguration->injectionPins[2] = Gpio::TLE9104_0_OUT_2;
	engineConfiguration->injectionPins[3] = Gpio::TLE9104_0_OUT_3;
	engineConfiguration->injectionPins[4] = Gpio::TLE9104_1_OUT_3;
	engineConfiguration->injectionPins[5] = Gpio::TLE9104_1_OUT_2;
	engineConfiguration->injectionPins[6] = Gpio::TLE9104_1_OUT_1;
	engineConfiguration->injectionPins[7] = Gpio::TLE9104_1_OUT_0;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = Gpio::H144_IGN_1;
	engineConfiguration->ignitionPins[1] = Gpio::H144_IGN_2;
	engineConfiguration->ignitionPins[2] = Gpio::H144_IGN_3;
	engineConfiguration->ignitionPins[3] = Gpio::H144_IGN_4;
	engineConfiguration->ignitionPins[4] = Gpio::H144_IGN_5;
	engineConfiguration->ignitionPins[5] = Gpio::H144_IGN_6;
	engineConfiguration->ignitionPins[6] = Gpio::H144_IGN_7;
	engineConfiguration->ignitionPins[7] = Gpio::H144_IGN_8;
}

static void setDefaultSensorInputs() {
	engineConfiguration->tps1_1AdcChannel = MM100_IN_TPS_ANALOG;

    engineConfiguration->boardUseTempPullUp = true;


//	engineConfiguration->map.sensor.hwChannel = H144_IN_MAP1; // external MAP
	engineConfiguration->map.sensor.hwChannel = H144_IN_MAP2; // On-board MAP
	engineConfiguration->map.sensor.type = MT_MPXH6400;

	engineConfiguration->clt.adcChannel = H144_IN_CLT;
	engineConfiguration->iat.adcChannel = H144_IN_IAT;
}

void setBoardConfigOverrides() {
	setHellenMegaEnPin();

	setHellenVbatt();
	setHellenCan();
	hellenMegaSdWithAccelerometer();
	setDefaultHellenAtPullUps();

	/* Four TLE9104 */
	enableHellenSpi2();
}

static void setDefaultETBPins() {
	// users would want to override those if using H-bridges for stepper idle control
	setupTLE9201IncludingStepper(/*PWM controlPin*/Gpio::H144_OUT_PWM1, Gpio::H144_OUT_PWM6, Gpio::H144_OUT_PWM5, 0);
	setupTLE9201IncludingStepper(/*PWM controlPin*/Gpio::H144_OUT_PWM2, Gpio::H144_OUT_PWM4, Gpio::H144_OUT_PWM3, 1);
}

/**
 * @brief   Board-specific configuration defaults.
 *
 * See also setDefaultEngineConfiguration
 *
 */
void setBoardDefaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();
	setHellenMMbaro();
	setDefaultSensorInputs();
	setDefaultETBPins();

	engineConfiguration->cylindersCount = 8;
	engineConfiguration->firingOrder = FO_1_8_7_2_6_5_4_3;

	engineConfiguration->displayLogicLevelsInEngineSniffer = true;
	engineConfiguration->isSdCardEnabled = true;

	engineConfiguration->globalTriggerAngleOffset = 0;

	engineConfiguration->enableSoftwareKnock = true;

	// Done in setBoardConfigOverrides() -> setHellenCan() ?
	engineConfiguration->canTxPin = H144_CAN_TX;
	engineConfiguration->canRxPin = H144_CAN_RX;

	engineConfiguration->mainRelayPin = Gpio::TLE9104_3_OUT_3;

	// Done in setBoardConfigOverrides() -> enableHellenSpi2() ?
	// SPI2 for TLE9104 diagnostics
	engineConfiguration->is_enabled_spi_2 = true;
	engineConfiguration->spi2mosiPin = H_SPI2_MOSI;
	engineConfiguration->spi2misoPin = H_SPI2_MISO;
	engineConfiguration->spi2sckPin = H_SPI2_SCK;
}

/*
 * RESET and EN signals of both TLE9104 are driven by same gpios,
 * Do not allow TLE driver to drive this pins as it will reset
 * first chip while initing second.
 * Set pins to proper state only once.
 * TODO: improve?
 */
static const tle9104_config tle9104_cfg[BOARD_TLE9104_COUNT] = {
	// INJ_LO
	{
		.spi_bus = &SPID2,
		.spi_config = {
			.circular = false,
			.end_cb = NULL,
			.ssport = GPIOC,
			.sspad = 10,		// 9104_CSN_INJ_LO <- SPI3_SCK
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
			{ .port = GPIOG, .pad = 7 },	// OUT_INJ1
			{ .port = GPIOG, .pad = 8 },	// OUT_INJ2
			{ .port = GPIOD, .pad = 11 },	// OUT_INJ3
			{ .port = GPIOD, .pad = 10 }	// OUT_INJ4
		},
		.resn = Gpio::Unassigned,	// Gpio::B12,	// 9104_EN <- SPI2_CS/CAN2_RX
		.en   = Gpio::Unassigned	// Gpio::A10	// 9104_RESN <- USBID
	},
	// INJ_HI
	{
		.spi_bus = &SPID2,
		.spi_config = {
			.circular = false,
			.end_cb = NULL,
			.ssport = GPIOC,
			.sspad = 11,		// 9104_CSN_INJ_HI <- SPI3_MISO
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
			{ .port = GPIOD, .pad = 9 },	// OUT_INJ5
			{ .port = GPIOF, .pad = 12 },	// OUT_INJ6
			{ .port = GPIOF, .pad = 13 },	// OUT_INJ7
			{ .port = GPIOF, .pad = 14 }	// OUT_INJ8
		},
		.resn = Gpio::Unassigned,	// Gpio::B12,	// 9104_EN <- SPI2_CS/CAN2_RX
		.en   = Gpio::Unassigned	// Gpio::A10	// 9104_RESN <- USBID
	},
	// AUX1-4
	{
		.spi_bus = &SPID2,
		.spi_config = {
			.circular = false,
			.end_cb = NULL,
			.ssport = GPIOC,
			.sspad = 12,		// 9104_CSN_AUX1 <- SPI3_MOSI
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
			{ .port = GPIOD, .pad = 3 },	// AUX_LS2 <- OUT_IO1
			{ .port = GPIOA, .pad = 9 },	// AUX_LS1 <- OUT_IO2
			{ .port = GPIOG, .pad = 14 },	// IO3 <- OUT_IO3
			{ .port = GPIOG, .pad = 5 }		// TACH <- OUT_IO4
		},
		.resn = Gpio::Unassigned,	// Gpio::B12,	// 9104_EN <- SPI2_CS/CAN2_RX
		.en   = Gpio::Unassigned	// Gpio::A10	// 9104_RESN <- USBID
	},
	// MISC
	{
		.spi_bus = &SPID2,
		.spi_config = {
			.circular = false,
			.end_cb = NULL,
			.ssport = GPIOA,
			.sspad = 15,		// 9104_CSN_AUX2 <- SPI3_CS
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
			{ .port = GPIOD, .pad = 2 },	// AUX_LS5 <- OUT_IO5
			{ .port = GPIOG, .pad = 11 },	// AUX_LS3 <- OUT_IO6
			{ .port = GPIOG, .pad = 3 },	// AUX_LS4 <- OUT_IO7
			{ .port = GPIOG, .pad = 4 }		// MAIN_RELAY <- OUT_IO8
		},
		.resn = Gpio::Unassigned,	// Gpio::B12,	// 9104_EN <- SPI2_CS/CAN2_RX
		.en   = Gpio::Unassigned	// Gpio::A10	// 9104_RESN <- USBID
	}
};

static void board_init_ext_gpios() {
	/* Waste of RAM, switch to palSetPadMode() and palSetPort() */
	{
		static OutputPin TleCs0;
		TleCs0.initPin("TLE9104 CS0", Gpio::C10);
		TleCs0.setValue(1);
	}
	{
		static OutputPin TleCs1;
		TleCs1.initPin("TLE9104 CS1", Gpio::C11);
		TleCs1.setValue(1);
	}
	{
		static OutputPin TleCs2;
		TleCs2.initPin("TLE9104 CS2", Gpio::C12);
		TleCs2.setValue(1);
	}
	{
		static OutputPin TleCs3;
		TleCs3.initPin("TLE9104 CS3", Gpio::A15);
		TleCs3.setValue(1);
	}

	{
		static OutputPin TleReset;
		TleReset.initPin("TLE9104 Reset", Gpio::B12);
		TleReset.setValue(1);

		static OutputPin TleEn;
		TleEn.initPin("TLE9104 En", Gpio::A10);
		TleEn.setValue(1);
	}

	initAll9104(tle9104_cfg);
}

/**
 * @brief Board-specific initialization code.
 */
void boardInitHardware() {
	alphaTempPullUp.initPin("a-temp", Gpio::H144_GP_IO3);	// TEMP_PULLUP <- IO3
	board_init_ext_gpios();
}

void boardOnConfigurationChange(engine_configuration_s * /*previousConfiguration*/) {
	alphaTempPullUp.setValue(engineConfiguration->boardUseTempPullUp);
}

static Gpio OUTPUTS[] = {
	Gpio::TLE9104_0_OUT_3, // 1A - Injector 4
	Gpio::TLE9104_0_OUT_2, // 2A - Injector 3
	Gpio::TLE9104_0_OUT_1, // 3A - Injector 2
	Gpio::TLE9104_0_OUT_0, // 4A - Injector 1
	Gpio::TLE9104_2_OUT_3, // 14A - OUT_TACH
	Gpio::TLE9104_2_OUT_1, // 5A - OUT_AUX_LS1
	Gpio::TLE9104_2_OUT_0, // 6A - OUT_AUX_LS2
	Gpio::TLE9104_1_OUT_3, // 1B - Injector 8
	Gpio::TLE9104_1_OUT_2, // 2B - Injector 7
	Gpio::TLE9104_1_OUT_1, // 3B - Injector 6
	Gpio::TLE9104_1_OUT_0, // 4B - Injector 5
	Gpio::TLE9104_3_OUT_3, // 5B - OUT_MAIN_RELAY
	Gpio::TLE9104_3_OUT_2, // 6B - OUT_AUX_LS3
	Gpio::TLE9104_3_OUT_1, // 7B - OUT_AUX_LS4
	Gpio::TLE9104_3_OUT_0, // 8B - OUT_AUX_LS5
	Gpio::TLE9104_2_OUT_2, // P1
	Gpio::H144_IGN_1,
	Gpio::H144_IGN_2,
	Gpio::H144_IGN_3,
	Gpio::H144_IGN_4,
	Gpio::H144_IGN_5,
	Gpio::H144_IGN_6,
	Gpio::H144_IGN_7,
	Gpio::H144_IGN_8,
};

int getBoardMetaOutputsCount() {
    return efi::size(OUTPUTS);
}

int getBoardMetaLowSideOutputsCount() {
    return getBoardMetaOutputsCount() - 8;
}

Gpio* getBoardMetaOutputs() {
    return OUTPUTS;
}
