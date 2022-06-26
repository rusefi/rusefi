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
 * @todo    Add your board-specific code, if any.
 */
void setBoardDefaultConfiguration() {
	setSerialConfigurationOverrides();

	/* Battery voltage */
	engineConfiguration->vbattAdcChannel = EFI_ADC_6;
	/* Vbat divider: 10K + 1K */
	engineConfiguration->vbattDividerCoeff = (1.0 + 10.0) / 1.0;

	/* Throttle position */
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_12;

	/* MAP: stock car dows not have MAP
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

	engineConfiguration->communicationLedPin = Gpio::G6;	/* LD1 - green */
	engineConfiguration->runningLedPin = Gpio::G8; /* LD3 - yellow */
	engineConfiguration->warningLedPin = Gpio::Unassigned; 	/* LD3 - yellow*/
	//engineConfiguration->unusedErrorPin = LED_ERROR_BRAIN_PIN;	/* LD2 - red */

	/* IF you have BOTH camshaft position sensor and crankshaft position sensor
	 * camshaft is always trigger#1 input and then crankshaft is trigger#2. */
	engineConfiguration->triggerInputPins[0] = Gpio::H12;	/* cam */
	engineConfiguration->triggerInputPins[1] = Gpio::H10;	/* crank pos #1 */
	engineConfiguration->camInputs[0] = Gpio::Unassigned;

	/* SPI devices: mess of board and engine configurations */
	/* TLE6240 */
	engineConfiguration->tle6240spiDevice = SPI_DEVICE_4;
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
	engineConfiguration->displayMode = DM_NONE;
	engineConfiguration->HD44780_rs = Gpio::Unassigned;
	engineConfiguration->HD44780_e = Gpio::Unassigned;
	engineConfiguration->HD44780_db4 = Gpio::Unassigned;
	engineConfiguration->HD44780_db5 = Gpio::Unassigned;
	engineConfiguration->HD44780_db6 = Gpio::Unassigned;
	engineConfiguration->HD44780_db7 = Gpio::Unassigned;
	engineConfiguration->digitalPotentiometerChipSelect[0] = Gpio::Unassigned;
	engineConfiguration->digitalPotentiometerChipSelect[1] = Gpio::Unassigned;
	engineConfiguration->digitalPotentiometerChipSelect[2] = Gpio::Unassigned;
	engineConfiguration->digitalPotentiometerChipSelect[3] = Gpio::Unassigned;
	engineConfiguration->vehicleSpeedSensorInputPin = Gpio::Unassigned;

	/* SPIs */
	engineConfiguration->is_enabled_spi_1 = true;
	engineConfiguration->is_enabled_spi_2 = false;
	engineConfiguration->is_enabled_spi_3 = true;
	engineConfiguration->is_enabled_spi_4 = true;

	engineConfiguration->spi1mosiPin = Gpio::Unassigned;
	engineConfiguration->spi1MosiMode = PO_DEFAULT;
	engineConfiguration->spi1misoPin = Gpio::Unassigned;
	engineConfiguration->spi1MisoMode = PO_DEFAULT;
	engineConfiguration->spi1sckPin = Gpio::Unassigned;
	engineConfiguration->spi1SckMode = PO_DEFAULT;

	engineConfiguration->spi3mosiPin = Gpio::C12;
	engineConfiguration->spi3MosiMode = PO_DEFAULT;
	engineConfiguration->spi3misoPin = Gpio::C11;
	engineConfiguration->spi3MisoMode = PO_DEFAULT;
	engineConfiguration->spi3sckPin = Gpio::C10;
	engineConfiguration->spi3SckMode = PO_DEFAULT;

	engineConfiguration->is_enabled_spi_1 = false;
	engineConfiguration->sdCardSpiDevice = SPI_DEVICE_1;
	engineConfiguration->sdCardCsPin = Gpio::A2;
	engineConfiguration->isSdCardEnabled = false;

	/* TODO: add settings for SPI4 */

	/* Knock sensor */
	/* Interface settings */
	engineConfiguration->hip9011SpiDevice = SPI_DEVICE_4;
	engineConfiguration->hip9011CsPin = Gpio::E11;	/* SPI4_NSS1 */
	engineConfiguration->hip9011CsPinMode = OM_OPENDRAIN;
	engineConfiguration->hip9011IntHoldPin = Gpio::H8;
	engineConfiguration->hip9011IntHoldPinMode = OM_OPENDRAIN;
	engineConfiguration->hipOutputChannel = EFI_ADC_7; /* PA7 */
	engineConfiguration->isHip9011Enabled = true;
	/* this board has TPIC8101, that supports advanced mode */
	engineConfiguration->useTpicAdvancedMode = true;
	/* Chip settings */
	engineConfiguration->hip9011PrescalerAndSDO = (0x6 << 1); //HIP_16MHZ_PRESCALER;
	engineConfiguration->hip9011Gain = 1.0;
	engineConfiguration->knockBandCustom = 0.0;
	engineConfiguration->cylinderBore = 96.9;

	/* Cylinder to knock bank mapping */
	engineConfiguration->knockBankCyl1 = 0;
	engineConfiguration->knockBankCyl2 = 1;
	engineConfiguration->knockBankCyl3 = 0;
	engineConfiguration->knockBankCyl4 = 1;
	engineConfiguration->knockBankCyl5 = 0;
	engineConfiguration->knockBankCyl6 = 1;

	/* Cylinder to bank mapping */
	engineConfiguration->cylinderBankSelect[1 - 1] = 0;
	engineConfiguration->cylinderBankSelect[2 - 1] = 1;
	engineConfiguration->cylinderBankSelect[3 - 1] = 0;
	engineConfiguration->cylinderBankSelect[4 - 1] = 1;
	engineConfiguration->cylinderBankSelect[5 - 1] = 0;
	engineConfiguration->cylinderBankSelect[6 - 1] = 1;

	/* Misc settings */
	engineConfiguration->acSwitch = Gpio::MC33972_PIN_22;
	engineConfiguration->acSwitchMode = PI_DEFAULT;

	/* This board also has AC clutch output: */
	engineConfiguration->acRelayPin = Gpio::TLE6240_PIN_15;
	engineConfiguration->maxAcRpm = 3000;
	engineConfiguration->acIdleRpmBump = 200;

	engineConfiguration->isCJ125Enabled = false;

	/* CAN */
	engineConfiguration->canTxPin = Gpio::D1;
	engineConfiguration->canRxPin = Gpio::D0;

	/* not used pins with testpads */
	engineConfiguration->triggerSimulatorPins[0] = Gpio::H2;
	engineConfiguration->triggerSimulatorPins[1] = Gpio::H3;
	engineConfiguration->triggerSimulatorPins[2] = Gpio::H4;
	engineConfiguration->triggerSimulatorPinModes[0] = OM_DEFAULT;
	engineConfiguration->triggerSimulatorPinModes[1] = OM_DEFAULT;
	engineConfiguration->triggerSimulatorPinModes[2] = OM_DEFAULT;

	//!!!!!!!!!!!!!!!!!!!
	//engineConfiguration->silentTriggerError = true;

	if (engineConfiguration->fuelAlgorithm == LM_REAL_MAF)
		setAlgorithm(LM_SPEED_DENSITY);
	if (engineConfiguration->fuelAlgorithm == LM_ALPHA_N)
		setAlgorithm(LM_ALPHA_N);
}

/* Schematic RefDef DA3 */
static const struct mc33810_config mc33810_odd = {
	.spi_bus = &SPID5,
	.spi_config = {
		.circular = false,
		.end_cb = NULL,
		.ssport = GPIOF,
		.sspad = 1,
		.cr1 =
			//SPI_CR1_16BIT_MODE |
			SPI_CR1_SSM |
			SPI_CR1_SSI |
			((3 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |	/* div = 16 */
			SPI_CR1_MSTR |
			/* SPI_CR1_CPOL | */ // = 0
			SPI_CR1_CPHA | // = 1
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
		/* ignition pre-dirvers */
		[4] = {.port = GPIOB, .pad = 3},	/* IGN 4 */
		[5] = {.port = GPIOB, .pad = 4},	/* IGN 3 */
		[6] = {.port = GPIOB, .pad = 5},	/* IGN 7 */
		[7] = {.port = GPIOB, .pad = 8},	/* IGN 5 */
	},
	/* en shared between two chips */
	.en = {.port = GPIOI, .pad = 7}
};

/* Schematic RefDef DA22 */
static const struct mc33810_config mc33810_even = {
	.spi_bus = &SPID5,
	.spi_config = {
		.circular = false,
		.end_cb = NULL,
		.ssport = GPIOF,
		.sspad = 2,
		.cr1 =
			//SPI_CR1_16BIT_MODE |
			SPI_CR1_SSM |
			SPI_CR1_SSI |
			((3 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |	/* div = 16 */
			SPI_CR1_MSTR |
			/* SPI_CR1_CPOL | */ // = 0
			SPI_CR1_CPHA | // = 1
			0,
		.cr2 = //SPI_CR2_16BIT_MODE |
			SPI_CR2_DS_3 | SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0
	},
	.direct_io = {
		/* injector drivers */
		[0] = {.port = GPIOE, .pad = 3},	/* INJ 2 */
		[1] = {.port = GPIOE, .pad = 4},	/* INJ 4 */
		[2] = {.port = GPIOE, .pad = 5},	/* INJ 6 */
		[3] = {.port = GPIOE, .pad = 6},	/* INJ 8 */
		/* ignition pre-dirvers */
		[4] = {.port = GPIOI, .pad = 9},	/* IGN 8 */
		[5] = {.port = GPIOC, .pad = 15},	/* IGN 6 */
		[6] = {.port = GPIOC, .pad = 14},	/* IGN 2 */
		[7] = {.port = GPIOC, .pad = 13},	/* IGN 1 */
	},
	/* en shared between two chips */
	.en = {.port = nullptr, .pad = 0}
};

static void board_init_ext_gpios()
{
	int ret;

	ret = mc33810_add(Gpio::MC33810_0_OUT_0, 0, &mc33810_odd);
	if (ret < 0) {
		/* error */
	}
	ret = mc33810_add(Gpio::MC33810_1_OUT_0, 1, &mc33810_even);
	if (ret < 0) {
		/* error */
	}
}

/**
 * @brief Board-specific initialization code.
 * @todo  Add your board-specific code, if any.
 */
void boardInit(void)
{
	board_init_ext_gpios();
}
