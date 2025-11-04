/**
 * @file boards/hellen/hellen154hyundai/board_configuration.cpp
 *
 *
 * @brief Configuration defaults for the hellen154hyundai board
 *
 * See https://rusefi.com/s/hellen154hyundai
 *
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "defaults.h"
#include "hellen_meta.h"
#include "board_overrides.h"

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = Gpio::MC33810_0_OUT_0;
	engineConfiguration->injectionPins[1] = Gpio::MC33810_0_OUT_1;
	engineConfiguration->injectionPins[2] = Gpio::MC33810_0_OUT_2;
	engineConfiguration->injectionPins[3] = Gpio::MC33810_0_OUT_3;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = Gpio::MC33810_0_GD_0;
	engineConfiguration->ignitionPins[1] = Gpio::MC33810_0_GD_1;
	engineConfiguration->ignitionPins[2] = Gpio::MC33810_0_GD_2;
	engineConfiguration->ignitionPins[3] = Gpio::MC33810_0_GD_3;
}

static void setupDefaultSensorInputs() {
	engineConfiguration->vvtMode[0] = VVT_SINGLE_TOOTH;
	engineConfiguration->vvtMode[1] = VVT_SINGLE_TOOTH;

    engineConfiguration->vehicleSpeedSensorInputPin = Gpio::H144_IN_D_4;

	setTPS1Inputs(H144_IN_TPS, H144_IN_TPS2);
	setPPSInputs(H144_IN_PPS, H144_IN_PPS2);

	engineConfiguration->map.sensor.hwChannel = H144_IN_MAP1;

	engineConfiguration->afr.hwChannel = EFI_ADC_NONE;

	engineConfiguration->clt.adcChannel = H144_IN_CLT;

	engineConfiguration->iat.adcChannel = H144_IN_IAT;
}

static struct tle9201_config tle9201 = {
	.spi_bus = &SPID3,
	.spi_config = {
		.circular = false,
#ifdef _CHIBIOS_RT_CONF_VER_6_1_
		.end_cb = nullptr,
#else
		.slave = false,
		.data_cb = nullptr,
		.error_cb = nullptr,
#endif
		// H_SPI3_CS
		.ssport = GPIOA,
		.sspad = 15,
		.cr1 = TLE9201_CONFIG_CR1,
		.cr2 = TLE9201_CONFIG_CR2
	}
};

static void hellen154hyundai_f7_boardConfigOverrides() {
	/* Force 3.3V PWR_EN as MC33810 is powered from this power line */
	setHellenMegaEnPin();

	hellenMegaModule();

	/* MC33810, ETB1 and WASTGATE1 */
	enableHellenSpi3();

	// trigger inputs
	engineConfiguration->triggerInputPins[0] = Gpio::H144_IN_D_1;
	// Direct hall-only cam input
	engineConfiguration->camInputs[0] = Gpio::H144_IN_D_2; // intake
	engineConfiguration->camInputs[1] = Gpio::H144_IN_D_3; // exhaust

		// todo You would not believe how you invert TLE9201 #4579
		engineConfiguration->stepperDcInvertedPins = true;

	  setupTLE9201(/*controlPin*/Gpio::H144_OUT_PWM2, Gpio::H144_OUT_PWM3, Gpio::H144_OUT_IO12);

		// wastegate DC motor
	  setupTLE9201(/*controlPin*/Gpio::H144_OUT_PWM4, Gpio::H144_OUT_PWM5, Gpio::H144_OUT_IO13, 1);
}

/**
 * @brief   Board-specific configuration defaults.
 *

 *

 */
static void hellen154hyundai_f7_boardDefaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();

	setHellenCan();

	engineConfiguration->fuelPumpPin = Gpio::H144_OUT_IO9;
	engineConfiguration->fanPin = Gpio::H144_OUT_IO7;
	engineConfiguration->mainRelayPin = Gpio::H144_OUT_IO3;	// pin: 111a
	// BK1 uses wire, BK2 uses CANbus
	engineConfiguration->malfunctionIndicatorPin = Gpio::H144_OUT_PWM8;

	engineConfiguration->brakePedalPin = Gpio::H144_IN_RES3;
	engineConfiguration->clutchUpPin = Gpio::H144_IN_RES2;
	engineConfiguration->acSwitch = Gpio::H144_IN_RES1;

#ifndef EFI_BOOTLOADER
  setCommonNTCSensor(&engineConfiguration->oilTempSensor, HELLEN_DEFAULT_AT_PULLUP); // random calibration for now
#endif
  engineConfiguration->oilTempSensor.adcChannel = H144_IN_AT2;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	engineConfiguration->etbFunctions[1] = DC_Wastegate;
}

static Gpio OUTPUTS[] = {
	Gpio::MC33810_0_OUT_0, // 0: INJ_1 k25
	Gpio::MC33810_0_OUT_1, // INJ_2 k26
	Gpio::MC33810_0_OUT_2, // INJ_3 k27
	Gpio::MC33810_0_OUT_3, // 3: INJ_4 k28
	Gpio::H144_LS_5, // VVT1
	Gpio::H144_LS_6, // 5: VVT2
	Gpio::H144_LS_7, // K47 BK1 Wastegate Solenoid
	Gpio::H144_OUT_IO7, // Fan Relay Low
	Gpio::H144_OUT_IO3, // Main Relay K64
	Gpio::H144_OUT_IO9, // Fuel Pump K70
	Gpio::H144_OUT_IO10, // 10: K87 AC Relay
	Gpio::H144_OUT_IO8, // Fan Relay HI
	Gpio::MC33810_0_GD_0, // Coil 1
	Gpio::MC33810_0_GD_1, // Coil 2
	Gpio::MC33810_0_GD_2, // Coil 3
	Gpio::MC33810_0_GD_3, // Coil 4
	Gpio::H144_OUT_PWM8, // MIL
//	QC procedure seems to not work for that one Gpio::H144_OUT_PWM7, // low side? Tacho unused CAN tachometer right?
//	Gpio::H_SPI1_SCK, // X8 AuxLS1
};

static const struct mc33810_config mc33810 = {
	.spi_bus = &SPID3,
	.spi_config = {
		.circular = false,
#ifdef _CHIBIOS_RT_CONF_VER_6_1_
		.end_cb = nullptr,
#else
		.slave = false,
		.data_cb = nullptr,
		.error_cb = nullptr,
#endif
		// SPI3_CS_33810 OUT_PWM1 H144_OUT_PWM1
		.ssport = GPIOD,
		.sspad = 13,
		.cr1 =
			SPI_CR1_16BIT_MODE |
			SPI_CR1_SSM |
			SPI_CR1_SSI |
//			((3 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |	/* div = 16 */
			((0b110 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |	/* div = 128 */
			SPI_CR1_MSTR |
			0,
		.cr2 = SPI_CR2_16BIT_MODE
	},
	.direct_io = {
		/* injector drivers */
		[0] = {.port = GPIOG, .pad = 7},	/* H144_LS_1 inj 1 */
		[1] = {.port = GPIOG, .pad = 8},	/* H144_LS_2 inj 2 */
		[2] = {.port = GPIOD, .pad = 11},	/* H144_LS_3 inj 3 */
		[3] = {.port = GPIOD, .pad = 10},	/* H144_LS_4 inj 4 */
		/* ignition pre-drivers */
		[4] = {.port = GPIOG, .pad = 5},	/* H144_OUT_IO4 */
		[5] = {.port = GPIOD, .pad = 2},	/* H144_OUT_IO5 */
		[6] = {.port = GPIOG, .pad = 11},	/* H144_OUT_IO6 */
		[7] = {.port = GPIOG, .pad = 2},	/* H144_OUT_IO11 */
	},
	.en = {.port = GPIOG, .pad = 9}, // H144_GP_IO4 hopefully
	// TODO: pick from engineConfiguration->spi3sckPin or whatever SPI is used
	.sck = {.port = GPIOC, .pad = 10},
	.spkdur = Gpio::Unassigned,
	.nomi = Gpio::Unassigned,
	.maxi = Gpio::Unassigned
};

static void hellen154hyundai_f7_boardInitHardware() {
	static OutputPin spi3CsWastegate;

	spi3CsWastegate.initPin("spi3-cs-wg", Gpio::H144_GP_IO6);
	spi3CsWastegate.setValue(1);
	// mc33810 takes care of the CS on it's own
//	static OutputPin spi3CsMc33810;
//	spi3CsMc33810.initPin("spi3-cs-mc33810", Gpio::H144_OUT_PWM1);
//	spi3CsMc33810.setValue(1);

          gpio_pin_markUsed(tle9201.spi_config.ssport, tle9201.spi_config.sspad, "TLE9201 ETB CS");
          palSetPadMode(tle9201.spi_config.ssport, tle9201.spi_config.sspad, PAL_MODE_OUTPUT_PUSHPULL);
          int retTle = tle9201_add(0, &tle9201);
          efiPrintf("*****************+ tle9201_add %d +*******************", retTle);

    #if (BOARD_MC33810_COUNT > 0)
      gpio_pin_markUsed(mc33810.spi_config.ssport, mc33810.spi_config.sspad, "mc33810 CS");
      palSetPadMode(mc33810.spi_config.ssport, mc33810.spi_config.sspad, PAL_MODE_OUTPUT_PUSHPULL);

      int ret = mc33810_add(Gpio::MC33810_0_OUT_0, 0, &mc33810);
      efiPrintf("*****************+ mc33810_add %d +*******************", ret);
    #else
      efiPrintf("**********------------- mc33810 not configured yet ------------************");
    #endif
}

int getBoardMetaOutputsCount() {
    return efi::size(OUTPUTS);
}

int getBoardMetaLowSideOutputsCount() {
    return getBoardMetaOutputsCount();
}

int getBoardMetaDcOutputsCount() {
    return 2;
}

Gpio* getBoardMetaOutputs() {
    return OUTPUTS;
}

void setup_custom_board_overrides() {
	custom_board_InitHardware = hellen154hyundai_f7_boardInitHardware;
	custom_board_DefaultConfiguration = hellen154hyundai_f7_boardDefaultConfiguration;
	custom_board_ConfigOverrides = hellen154hyundai_f7_boardConfigOverrides;
}

