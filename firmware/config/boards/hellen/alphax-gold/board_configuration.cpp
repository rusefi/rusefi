#include "pch.h"
#include "hellen_meta.h"
#include "defaults.h"
#include "smart_gpio.h"
#include "drivers/gpio/tle9104.h"
#include "board_overrides.h"

// Configurable 2.7K pull-ups to 5V
static OutputPin alphaTempPullUp;
static OutputPin alphaKnock1PullUp;
static OutputPin alphaKnock2PullUp;

// Configurable 750R pull-up to 5V or 220+750R to 12V
static OutputPin alphaTachSelPullUp;

// Configurable 4.470K pull-ups to 12V
static OutputPin alphaHall1PullUp;
static OutputPin alphaHall2PullUp;
static OutputPin alphaHall3PullUp;
static OutputPin alphaHall4PullUp;
static OutputPin alphaHall5PullUp;
static OutputPin alphaHall6PullUp;

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
	setTPS1Inputs(H144_IN_TPS, H144_IN_TPS2);
	setPPSInputs(H144_IN_PPS, H144_IN_PPS2);

	engineConfiguration->boardEnTempPullUp = true;
	engineConfiguration->boardEnKnock1PullUp = false;
	engineConfiguration->boardEnKnock2PullUp = false;
	// default is 5V pull-up
	engineConfiguration->boardSelTachPullUp = false;
	engineConfiguration->boardEnHall1PullUp = false;
	engineConfiguration->boardEnHall2PullUp = true;
	engineConfiguration->boardEnHall3PullUp = false;
	engineConfiguration->boardEnHall4PullUp = false;
	engineConfiguration->boardEnHall5PullUp = false;
	engineConfiguration->boardEnHall6PullUp = false;

	engineConfiguration->vvtMode[0] = VVT_SINGLE_TOOTH;
	engineConfiguration->triggerInputPins[0] = Gpio::H144_IN_VSS; // VR Input 3 (MAX9924)
	engineConfiguration->camInputs[0] = Gpio::H144_IN_MAP1_DIGITAL; // cannot be used simultaneously with VR1

	// A18 + A22
	setTPS1Inputs(H144_IN_TPS, H144_IN_TPS2);
	// B19 + B20
	setPPSInputs(H144_IN_PPS, H144_IN_PPS2);

	engineConfiguration->map.sensor.hwChannel = H144_IN_O2S2; // external MAP
//	engineConfiguration->map.sensor.hwChannel = H144_IN_MAP2; // On-board MAP
	engineConfiguration->map.sensor.type = MT_MPXH6400;

	engineConfiguration->afr.hwChannel = EFI_ADC_NONE;
	engineConfiguration->clt.adcChannel = H144_IN_CLT;
	engineConfiguration->iat.adcChannel = H144_IN_IAT;
}

static void alphax_gold_boardConfigOverrides() {
	setHellenMegaEnPin();

	setHellenVbatt();
	setHellenCan();
	hellenMegaSdWithAccelerometer();
	setDefaultHellenAtPullUps(2700); // wow this board is unusual?!

	/* Four TLE9104 */
	enableHellenSpi2();
}

static void setDefaultETBPins() {
	// users would want to override those if using H-bridges for stepper idle control
	setupTLE9201IncludingStepper(/*PWM controlPin*/Gpio::H144_OUT_PWM1, /*direction*/Gpio::H144_OUT_PWM6, /*disable*/Gpio::H144_OUT_PWM5, /*dcIndex*/0);
	setupTLE9201IncludingStepper(/*PWM controlPin*/Gpio::H144_OUT_PWM2, /*direction*/Gpio::H144_OUT_PWM4, /*disable*/Gpio::H144_OUT_PWM3, /*dcIndex*/1);
}

/**
 * @brief   Board-specific configuration defaults.
 *

 *
 */
static void alphax_gold_boardDefaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();
	setHellenMMbaro();
	setDefaultSensorInputs();
	setDefaultETBPins();

	engineConfiguration->cylindersCount = 8;
	engineConfiguration->firingOrder = FO_1_8_7_2_6_5_4_3;

	engineConfiguration->displayLogicLevelsInEngineSniffer = true;
	engineConfiguration->isSdCardEnabled = true;

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

	engineConfiguration->vrThreshold[0].pin = Gpio::H144_OUT_PWM7;
	engineConfiguration->vrThreshold[1].pin = Gpio::H144_OUT_PWM8;
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
#ifdef _CHIBIOS_RT_CONF_VER_6_1_
			.end_cb = nullptr,
#else
			.slave = false,
			.data_cb = nullptr,
			.error_cb = nullptr,
#endif
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
#ifdef _CHIBIOS_RT_CONF_VER_6_1_
			.end_cb = nullptr,
#else
			.slave = false,
			.data_cb = nullptr,
			.error_cb = nullptr,
#endif
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
#ifdef _CHIBIOS_RT_CONF_VER_6_1_
			.end_cb = nullptr,
#else
			.slave = false,
			.data_cb = nullptr,
			.error_cb = nullptr,
#endif
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
#ifdef _CHIBIOS_RT_CONF_VER_6_1_
			.end_cb = nullptr,
#else
			.slave = false,
			.data_cb = nullptr,
			.error_cb = nullptr,
#endif
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
static void alphax_gold_boardInitHardware() {
	// TEMP_PULLUP
	alphaTempPullUp.initPin("Temp PullUp", Gpio::H144_GP_IO3);
	// IN_KNOCK1_PULLUP
	alphaKnock1PullUp.initPin("Knock1 PullUp", Gpio::H144_GP_IO4);
	// IN_KNOCK2_PULL
	alphaKnock2PullUp.initPin("Knock2 PullUp", Gpio::H144_GP_IO5);
	// TACH_PULLUP
	alphaTachSelPullUp.initPin("Tach PullUp", Gpio::H144_OUT_IO9);
	// VR_DISCRETE1+_PULLUP
	alphaHall1PullUp.initPin("Hall 1 PullUp", Gpio::H144_OUT_IO10);
	// VR_DISCRETE11_PULLUP
	alphaHall2PullUp.initPin("Hall 2 PullUp", Gpio::H144_OUT_IO11);
	// VR_DISCRETE2+_PULLUP
	alphaHall3PullUp.initPin("Hall 3 PullUp", Gpio::H144_OUT_IO12);
	// VR_DISCRETE2-_PULLUP
	alphaHall4PullUp.initPin("Hall 4 PullUp", Gpio::H144_OUT_IO13);
	// VR_9924+_PULLUP
	alphaHall5PullUp.initPin("Hall 5 PullUp", Gpio::H144_GP_IO2);
	// VR_9924+-_PULLUP
	alphaHall6PullUp.initPin("Hall 6 PullUp", Gpio::H144_GP_IO1);

	board_init_ext_gpios();
}

static void alphax_gold_OnConfigurationChange(engine_configuration_s * /*previousConfiguration*/) {
	alphaTempPullUp.setValue(engineConfiguration->boardEnTempPullUp);
	alphaKnock1PullUp.setValue(engineConfiguration->boardEnKnock1PullUp);
	alphaKnock2PullUp.setValue(engineConfiguration->boardEnKnock2PullUp);
	alphaTachSelPullUp.setValue(engineConfiguration->boardSelTachPullUp);
	alphaHall1PullUp.setValue(engineConfiguration->boardEnHall1PullUp);
	alphaHall2PullUp.setValue(engineConfiguration->boardEnHall2PullUp);
	alphaHall3PullUp.setValue(engineConfiguration->boardEnHall3PullUp);
	alphaHall4PullUp.setValue(engineConfiguration->boardEnHall4PullUp);
	alphaHall5PullUp.setValue(engineConfiguration->boardEnHall5PullUp);
	alphaHall6PullUp.setValue(engineConfiguration->boardEnHall6PullUp);
}

static Gpio OUTPUTS[] = {
	Gpio::TLE9104_0_OUT_3, // 1B - Injector 4
	Gpio::TLE9104_0_OUT_2, // 2B - Injector 3
	Gpio::TLE9104_0_OUT_1, // 3B - Injector 2
	Gpio::TLE9104_0_OUT_0, // 4B - Injector 1

	Gpio::TLE9104_2_OUT_3, // 14B - OUT_TACH
	Gpio::TLE9104_2_OUT_1, // 5B - OUT_AUX_LS1
	Gpio::TLE9104_2_OUT_0, // 6B - OUT_AUX_LS2
	// Gpio::TLE9104_2_OUT_2, // not testing P1

	Gpio::TLE9104_1_OUT_3, // 1A - Injector 8
	Gpio::TLE9104_1_OUT_2, // 2A - Injector 7
	Gpio::TLE9104_1_OUT_1, // 3A - Injector 6
	Gpio::TLE9104_1_OUT_0, // 4A - Injector 5

	Gpio::TLE9104_3_OUT_3, // 5A - OUT_MAIN_RELAY
	Gpio::TLE9104_3_OUT_1, // 6A - OUT_AUX_LS3
	Gpio::TLE9104_3_OUT_2, // 7A - OUT_AUX_LS4
	Gpio::TLE9104_3_OUT_0, // 8A - OUT_AUX_LS5
	Gpio::H144_IGN_1, // 13B Coil 1
	Gpio::H144_IGN_2, // 12B Coil 2
	Gpio::H144_IGN_3, // 11B Coil 3
	Gpio::H144_IGN_4, // 10B Coil 4
	Gpio::H144_IGN_5, // 13A Coil 5
	Gpio::H144_IGN_6, // 12A Coil 6
	Gpio::H144_IGN_7, // 11A Coil 7
	Gpio::H144_IGN_8, // 10A Coil 8
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

int getBoardMetaDcOutputsCount() {
    return 2;
}

void setup_custom_board_overrides() {
	custom_board_InitHardware = alphax_gold_boardInitHardware;
	custom_board_DefaultConfiguration = alphax_gold_boardDefaultConfiguration;
	custom_board_ConfigOverrides =  alphax_gold_boardConfigOverrides;
	custom_board_OnConfigurationChange = alphax_gold_OnConfigurationChange;
}

