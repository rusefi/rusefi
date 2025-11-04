/**
 * @file boards/subaru_eg33/board_configuration.h
 *
 * @brief In this file we can override engine_configuration.cpp.
 *
 * @date Feb 06, 2021
 * @author Andrey Gusakov, 2021
 */

#include "pch.h"
#include "smart_gpio.h"
#include "drivers/gpio/mc33810.h"
#include "device_mpu_util.h"
#include "board_overrides.h"

Gpio getCommsLedPin() {
	return Gpio::G6;	/* LD1 - green */
}

Gpio getRunningLedPin() {
	return Gpio::G8; /* LD3 - yellow */
}

Gpio getWarningLedPin() {
	// this board has no warning led
	return Gpio::Unassigned;
}

static void setSerialConfigurationOverrides() {
	engineConfiguration->binarySerialTxPin = Gpio::E1;
	engineConfiguration->binarySerialRxPin = Gpio::E0;
	/* actually Bluetooth/WiFi interface */
	//engineConfiguration->consoleSerialTxPin = Gpio::C10;
	//engineConfiguration->consoleSerialRxPin = Gpio::C11;
	engineConfiguration->tunerStudioSerialSpeed = SERIAL_SPEED;
	engineConfiguration->uartConsoleSerialSpeed = SERIAL_SPEED;
}

/**
 * @brief   Board-specific configuration defaults.

 */
static void subaru_eg33_boardDefaultConfiguration() {
	setSerialConfigurationOverrides();

	/* Battery voltage */
	engineConfiguration->vbattAdcChannel = EFI_ADC_6;
	/* Vbat divider: 10K + 1K */
	engineConfiguration->vbattDividerCoeff = (1.0 + 10.0) / 1.0;

	/* Throttle position */
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_12;

	/* MAP: stock car does not have MAP
	 * but EFI_ADC_10 is reserved for this purpose */
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_NONE;

	/* MAF */
	engineConfiguration->mafAdcChannel = EFI_ADC_3;

	/* coolant t */
	engineConfiguration->clt.adcChannel = EFI_ADC_14;
	/* 1K pull-up plus 20K pull-down to get ~4.75V when no sensor connected */
	engineConfiguration->clt.config.bias_resistor = 1000;

	/* No IAT sensor on stock engine */
	engineConfiguration->iat.adcChannel = EFI_ADC_NONE;
	//sengineConfiguration->iat.config.bias_resistor = 2700;

	/* TODO: add both narrow sensors */
	engineConfiguration->afr.hwChannel = EFI_ADC_NONE;

	engineConfiguration->adcVcc = ADC_VCC;

	/* No barro */
	engineConfiguration->baroSensor.hwChannel = EFI_ADC_NONE;

	/* No pedal position */
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_NONE;

	/* Injectors */
	engineConfiguration->injectionPins[1 - 1] = Gpio::MC33810_0_OUT_0;
	engineConfiguration->injectionPins[2 - 1] = Gpio::MC33810_1_OUT_0;
	engineConfiguration->injectionPins[3 - 1] = Gpio::MC33810_0_OUT_1;
	engineConfiguration->injectionPins[4 - 1] = Gpio::MC33810_1_OUT_1;
	engineConfiguration->injectionPins[5 - 1] = Gpio::MC33810_0_OUT_2;
	engineConfiguration->injectionPins[6 - 1] = Gpio::MC33810_1_OUT_2;
	/* Additional, not used for EG33 */
	engineConfiguration->injectionPins[7 - 1] = Gpio::MC33810_0_OUT_3;
	engineConfiguration->injectionPins[8 - 1] = Gpio::MC33810_1_OUT_3;

	/* Ignition */
	engineConfiguration->ignitionPins[1 - 1] = Gpio::MC33810_1_GD_3;
	engineConfiguration->ignitionPins[2 - 1] = Gpio::MC33810_1_GD_2;
	engineConfiguration->ignitionPins[3 - 1] = Gpio::MC33810_0_GD_1;
	engineConfiguration->ignitionPins[4 - 1] = Gpio::MC33810_0_GD_0;
	engineConfiguration->ignitionPins[5 - 1] = Gpio::MC33810_0_GD_3;
	engineConfiguration->ignitionPins[6 - 1] = Gpio::MC33810_1_GD_1;
	/* Additional, not used for EG33 */
	engineConfiguration->ignitionPins[7 - 1] = Gpio::MC33810_0_GD_2;
	engineConfiguration->ignitionPins[8 - 1] = Gpio::MC33810_1_GD_0;
	//engineConfiguration->ignitionPinMode = OM_INVERTED;

	// Idle configuration
	engineConfiguration->useStepperIdle = false;
	engineConfiguration->isDoubleSolenoidIdle = true;
	engineConfiguration->idle.solenoidPin = Gpio::TLE6240_PIN_11;
	engineConfiguration->secondSolenoidPin = Gpio::TLE6240_PIN_12;

	/* IF you have BOTH camshaft position sensor and crankshaft position sensor
	 * camshaft is always trigger#1 input and then crankshaft is trigger#2. */
	engineConfiguration->triggerInputPins[0] = Gpio::H12;	/* cam */
	engineConfiguration->triggerInputPins[1] = Gpio::H10;	/* crank pos #1 */
	engineConfiguration->camInputs[0] = Gpio::Unassigned;

	/* SPI devices: mess of board and engine configurations */
	/* TLE6240 */
	engineConfiguration->tle6240spiDevice = SPI_DEVICE_4;
	// todo: hard-code SPI4 pins into spi4mosiPin etc!
	engineConfiguration->tle6240_cs = Gpio::E15;	/* SPI4_NSS0 */
	engineConfiguration->tle6240_csPinMode = OM_OPENDRAIN;
	/* MC33972 */
	engineConfiguration->mc33972spiDevice = SPI_DEVICE_4;
	engineConfiguration->mc33972_cs = Gpio::E10;	/* SPI4_NSS2 */
	engineConfiguration->mc33972_csPinMode = OM_DEFAULT;

	/* TLE6240 - OUT3, also PG2 through 3.3V-> 5.0V level translator - not installed */
	engineConfiguration->tachOutputPin = Gpio::TLE6240_PIN_2;
	engineConfiguration->tachOutputPinMode = OM_DEFAULT;

	/* spi driven - TLE6240 - OUT5 */
	engineConfiguration->fuelPumpPin = Gpio::TLE6240_PIN_5;
	engineConfiguration->fuelPumpPinMode = OM_DEFAULT;

	/* Self shutdown ouput:
	 * High level on this pin will keep Main Relay enabled in any position of ignition key
	 * This cause inability to stop engine by key.
	 * From other side main relay is powered from key position "IGN" OR this output (through diodes)
	 * So ECU does not need to drive this signal.
	 * The only puprose of this output is to keep ECU powered to finish some stuff before power off itself
	 * To support this we need to sense ING input from key switch */
	//engineConfiguration->mainRelayPin = Gpio::H7;
	//engineConfiguration->mainRelayPinMode = OM_DEFAULT;

	/* spi driven - TLE6240 - OUT1, OUT2 */
	engineConfiguration->fanPin = Gpio::TLE6240_PIN_1;
	engineConfiguration->fanPinMode = OM_DEFAULT;
	/* TODO: second fan */
	//engineConfiguration->fanPin[1] = Gpio::TLE6240_2;
	//engineConfiguration->fanPinMode[1] = OM_DEFAULT;
	/* spi driven - TLE6240 - OUT8 */
	engineConfiguration->malfunctionIndicatorPin = Gpio::TLE6240_PIN_7;
	engineConfiguration->malfunctionIndicatorPinMode = OM_DEFAULT;

	/* not used */
	engineConfiguration->digitalPotentiometerChipSelect[0] = Gpio::Unassigned;
	engineConfiguration->digitalPotentiometerChipSelect[1] = Gpio::Unassigned;
	engineConfiguration->digitalPotentiometerChipSelect[2] = Gpio::Unassigned;
	engineConfiguration->digitalPotentiometerChipSelect[3] = Gpio::Unassigned;
	engineConfiguration->vehicleSpeedSensorInputPin = Gpio::Unassigned;

	/* SPIs */
	/* SPI4, 5 are always enabled and its configuration is set in setBoardConfigOverrides() */
	engineConfiguration->is_enabled_spi_1 = false;
	engineConfiguration->is_enabled_spi_3 = false;
	engineConfiguration->is_enabled_spi_6 = false;
	engineConfiguration->spi1mosiPin = Gpio::Unassigned;
	engineConfiguration->spi1misoPin = Gpio::Unassigned;
	engineConfiguration->spi1sckPin = Gpio::Unassigned;
	engineConfiguration->spi3mosiPin = Gpio::Unassigned;
	engineConfiguration->spi3misoPin = Gpio::Unassigned;
	engineConfiguration->spi3sckPin = Gpio::Unassigned;
	engineConfiguration->spi6mosiPin = Gpio::Unassigned;
	engineConfiguration->spi6misoPin = Gpio::Unassigned;
	engineConfiguration->spi6sckPin = Gpio::Unassigned;
	/* Use PP mode as default for optional display SPI bus */
	engineConfiguration->spi2SckMode = PO_DEFAULT;
	engineConfiguration->spi2MosiMode = PO_DEFAULT;
	engineConfiguration->spi2MisoMode = PO_DEFAULT;
	/* User can disable this bus */
	engineConfiguration->is_enabled_spi_2 = true;

	/* SD card is located on SDIO interface */
	engineConfiguration->isSdCardEnabled = true;
	engineConfiguration->sdCardCsPin = Gpio::Unassigned;

	engineConfiguration->cylinderBore = 96.9;

	/* Cylinder to knock bank mapping */
	engineConfiguration->knockBankCyl1 = 0;
	engineConfiguration->knockBankCyl2 = 1;
	engineConfiguration->knockBankCyl3 = 0;
	engineConfiguration->knockBankCyl4 = 1;
	engineConfiguration->knockBankCyl5 = 0;
	engineConfiguration->knockBankCyl6 = 1;

	/* Misc settings */
	engineConfiguration->acSwitch = Gpio::MC33972_PIN_22;
	engineConfiguration->acSwitchMode = PI_DEFAULT;

	/* This board also has AC clutch output: */
	engineConfiguration->acRelayPin = Gpio::TLE6240_PIN_15;

	/* CAN */
	engineConfiguration->canTxPin = Gpio::D1;
	engineConfiguration->canRxPin = Gpio::D0;

	/* not used pins with testpads */
	engineConfiguration->triggerSimulatorPins[0] = Gpio::H2;
	engineConfiguration->triggerSimulatorPins[1] = Gpio::H3;

	if (engineConfiguration->fuelAlgorithm == engine_load_mode_e::LM_REAL_MAF)
		setAlgorithm(engine_load_mode_e::LM_SPEED_DENSITY);
	if (engineConfiguration->fuelAlgorithm == engine_load_mode_e::LM_ALPHA_N)
		setAlgorithm(engine_load_mode_e::LM_ALPHA_N);
}

static void subaru_eg33_boardConfigOverrides() {
	/* Optional SPI display */
	engineConfiguration->spi2sckPin = Gpio::I1;
	engineConfiguration->spi2misoPin = Gpio::I2;
	engineConfiguration->spi2mosiPin = Gpio::I3;
	/* User can disable this bus and change pin mode, but not pins itself */

	/* Smart chip */
	engineConfiguration->spi4sckPin = Gpio::E12;
	engineConfiguration->spi4misoPin = Gpio::E13;
	engineConfiguration->spi4mosiPin = Gpio::E14;
	engineConfiguration->spi4SckMode = PO_DEFAULT;
	engineConfiguration->spi4MosiMode = PO_OPENDRAIN;
	engineConfiguration->spi4MisoMode = PO_DEFAULT;
	/* This is mandatory to have this bus enabled */
	engineConfiguration->is_enabled_spi_4 = true;

	/* Smart ignition chips */
	engineConfiguration->spi5sckPin = Gpio::F7;
	engineConfiguration->spi5misoPin = Gpio::F8;
	engineConfiguration->spi5mosiPin = Gpio::F9;
	engineConfiguration->spi5SckMode = PO_DEFAULT;
	engineConfiguration->spi5MosiMode = PO_DEFAULT;
	engineConfiguration->spi5MisoMode = PO_DEFAULT;
	/* This is mandatory to have this bus enabled */
	engineConfiguration->is_enabled_spi_5 = true;

}

/* Schematic RefDef DA3 */
static const struct mc33810_config mc33810_odd = {
	.spi_bus = &SPID5,
	.spi_config = {
	.circular = false,
#ifdef _CHIBIOS_RT_CONF_VER_6_1_
	.end_cb = nullptr,
#else
	.slave = false,
	.data_cb = nullptr,
	.error_cb = nullptr,
#endif
		.ssport = GPIOF,
		.sspad = 1,
		.cr1 =
			//SPI_CR1_16BIT_MODE |
			SPI_CR1_SSM |
			SPI_CR1_SSI |
			((3 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |	/* div = 16 */
			SPI_CR1_MSTR |
			/* SPI_CR1_CPOL | */ // = 0
			/*
			https://github.com/rusefi/rusefi/issues/6538 says that should be zero
			SPI_CR1_CPHA | // = 1
			*/
			0,
		.cr2 = //SPI_CR2_16BIT_MODE |
			SPI_CR2_DS_3 | SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0
	},
	.direct_io = {
		/* injector drivers */
		[0] = {.port = GPIOI, .pad = 6},	/* INJ 1 */
		[1] = {.port = GPIOI, .pad = 5},	/* INJ 3 */
		[2] = {.port = GPIOI, .pad = 4},	/* INJ 5 */
		[3] = {.port = GPIOB, .pad = 9},	/* INJ 7 */
		/* ignition pre-drivers */
		[4] = {.port = GPIOB, .pad = 3},	/* IGN 4 */
		[5] = {.port = GPIOB, .pad = 4},	/* IGN 3 */
		[6] = {.port = GPIOB, .pad = 5},	/* IGN 7 */
		[7] = {.port = GPIOB, .pad = 8},	/* IGN 5 */
	},
	/* en shared between two chips */
	.en = {.port = GPIOI, .pad = 7},
	// TODO: pick from engineConfiguration->spi5sckPin or whatever SPI is used
	.sck = {.port = GPIOF, .pad = 7},
	/* TODO: */
	.spkdur = Gpio::Unassigned,
	.nomi = Gpio::Unassigned,
	.maxi = Gpio::Unassigned
};

/* Schematic RefDef DA22 */
static const struct mc33810_config mc33810_even = {
	.spi_bus = &SPID5,
	.spi_config = {
	.circular = false,
#ifdef _CHIBIOS_RT_CONF_VER_6_1_
	.end_cb = nullptr,
#else
	.slave = false,
	.data_cb = nullptr,
	.error_cb = nullptr,
#endif
		.ssport = GPIOF,
		.sspad = 2,
		.cr1 =
			SPI_CR1_16BIT_MODE |
			SPI_CR1_SSM |
			SPI_CR1_SSI |
			((3 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |	/* div = 16 */
			SPI_CR1_MSTR |
			/* SPI_CR1_CPOL | */ // = 0
			/*
			https://github.com/rusefi/rusefi/issues/6538 says that should be zero
			SPI_CR1_CPHA | // = 1
			*/
			0,
		.cr2 = SPI_CR2_16BIT_MODE
	},
	.direct_io = {
		/* injector drivers */
		[0] = {.port = GPIOE, .pad = 3},	/* INJ 2 */
		[1] = {.port = GPIOE, .pad = 4},	/* INJ 4 */
		[2] = {.port = GPIOE, .pad = 5},	/* INJ 6 */
		[3] = {.port = GPIOE, .pad = 6},	/* INJ 8 */
		/* ignition pre-drivers */
		[4] = {.port = GPIOI, .pad = 9},	/* IGN 8 */
		[5] = {.port = GPIOC, .pad = 15},	/* IGN 6 */
		[6] = {.port = GPIOC, .pad = 14},	/* IGN 2 */
		[7] = {.port = GPIOC, .pad = 13},	/* IGN 1 */
	},
	/* en shared between two chips */
	.en = {.port = nullptr, .pad = 0},
	// TODO: pick from engineConfiguration->spi5sckPin or whatever SPI is used
	.sck = {.port = GPIOF, .pad = 7},
	/* TODO: */
	.spkdur = Gpio::Unassigned,
	.nomi = Gpio::Unassigned,
	.maxi = Gpio::Unassigned
};

static void board_init_ext_gpios()
{
#ifndef EFI_BOOTLOADER
	int ret;

	ret = mc33810_add(Gpio::MC33810_0_OUT_0, 0, &mc33810_odd);
	if (ret < 0) {
		/* error */
	}
	ret = mc33810_add(Gpio::MC33810_1_OUT_0, 1, &mc33810_even);
	if (ret < 0) {
		/* error */
	}
#endif // EFI_BOOTLOADER
}

/**
 * @brief Board-specific initialization code.
 */
void boardInit() {
	board_init_ext_gpios();
}

void setup_custom_board_overrides() {
	custom_board_DefaultConfiguration = subaru_eg33_boardDefaultConfiguration;
	custom_board_ConfigOverrides = subaru_eg33_boardConfigOverrides;
}
