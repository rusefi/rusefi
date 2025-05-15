/**
 * @file board_configuration.cpp
 */

#include "pch.h"

static void setDefaultFrankensoStepperIdleParameters() {
	engineConfiguration->idle.stepperDirectionPin = Gpio::E10;
	engineConfiguration->idle.stepperStepPin = Gpio::E12;
	engineConfiguration->stepperEnablePin = Gpio::E14;
	engineConfiguration->idleStepperReactionTime = 10;
	engineConfiguration->idleStepperTotalSteps = 150;
}

static void setCanFrankensoDefaults() {
	engineConfiguration->canTxPin = Gpio::B6;
	engineConfiguration->canRxPin = Gpio::B12;
}

Gpio getWarningLedPin() {
    // open question if we need those LEDs at all? shall those be configurable?
	return Gpio::Unassigned;
}

Gpio getCommsLedPin() {
#ifdef HW_NOT_COMMUNITY_FRANKENSO
  // not f407-discovery but f407-discovery while reusing board file
  return Gpio::D15;
#else
  // f407-discovery community board not frankenso
	return config->communityCommsLedPin;
#endif
}

Gpio getRunningLedPin() {
    // open question if we need those LEDs at all? shall those be configurable?
	return Gpio::Unassigned;
}

#if EFI_ONBOARD_MEMS
static void configureAccelerometerPins() {
//	engineConfiguration->accelerometerCsPin = Gpio::E3; // we have a conflict with VVT output on Miata
// 	engineConfiguration->is_enabled_spi_1 = true; // we have a conflict with PA5 input pin

	// stm32f4discovery defaults
	engineConfiguration->spi1mosiPin = Gpio::A7;
	engineConfiguration->spi1misoPin = Gpio::A6;
	engineConfiguration->spi1sckPin = Gpio::A5;
}
#endif // EFI_ONBOARD_MEMS

/**
 * @brief	Hardware board-specific default configuration (GPIO pins, ADC channels, SPI configs etc.)
 */
void setBoardDefaultConfiguration() {
	setDefaultFrankensoStepperIdleParameters();
	setCanFrankensoDefaults();

#ifndef HW_NOT_COMMUNITY_FRANKENSO
  // f407-discovery community board not frankenso
	config->communityCommsLedPin = Gpio::D15;  // blue LED on discovery
#endif

	// set optional subsystem configs
#if EFI_ONBOARD_MEMS
	// this would override some values from above
	configureAccelerometerPins();
#endif /* EFI_ONBOARD_MEMS */



	engineConfiguration->map.sensor.hwChannel = EFI_ADC_4;
	engineConfiguration->clt.adcChannel = EFI_ADC_6;
	engineConfiguration->iat.adcChannel = EFI_ADC_7;
	engineConfiguration->afr.hwChannel = EFI_ADC_14;

	engineConfiguration->accelerometerSpiDevice = SPI_DEVICE_1;

	engineConfiguration->triggerSimulatorPins[0] = Gpio::D1;
	engineConfiguration->triggerSimulatorPins[1] = Gpio::D2;

	engineConfiguration->triggerInputPins[0] = Gpio::C6;
//	engineConfiguration->triggerInputPins[1] = Gpio::A5;

	// set this to SPI_DEVICE_3 to enable stimulation
	//engineConfiguration->digitalPotentiometerSpiDevice = SPI_DEVICE_3;
	engineConfiguration->digitalPotentiometerChipSelect[0] = Gpio::D7;
	engineConfiguration->digitalPotentiometerChipSelect[1] = Gpio::Unassigned;
	engineConfiguration->digitalPotentiometerChipSelect[2] = Gpio::D5;
	engineConfiguration->digitalPotentiometerChipSelect[3] = Gpio::Unassigned;

	engineConfiguration->spi1mosiPin = Gpio::B5;
	engineConfiguration->spi1misoPin = Gpio::B4;
	engineConfiguration->spi1sckPin = Gpio::B3; // please note that this pin is also SWO/SWD - Single Wire debug Output

	engineConfiguration->spi2mosiPin = Gpio::B15;
	engineConfiguration->spi2misoPin = Gpio::B14;
	engineConfiguration->spi2sckPin = Gpio::B13;

	engineConfiguration->spi3mosiPin = Gpio::B5;
	engineConfiguration->spi3misoPin = Gpio::B4;
	engineConfiguration->spi3sckPin = Gpio::B3;



#if EFI_FILE_LOGGING
	setDefaultSdCardParameters();
#endif /* EFI_FILE_LOGGING */

	engineConfiguration->is_enabled_spi_1 = false;
	engineConfiguration->is_enabled_spi_2 = false;
	engineConfiguration->is_enabled_spi_3 = true;
}

// weak linkage
void boardInitHardware() {

static const struct mc33810_config mc33810 = {
	.spi_bus = &SPID3,
	.spi_config = {
		.circular = false,
		.end_cb = NULL,
		// todo: use engineConfiguration->mc33810_cs
		.ssport = GPIOC,
		.sspad = 5,
		.cr1 =
			SPI_CR1_16BIT_MODE |
			SPI_CR1_SSM |
			SPI_CR1_SSI |
			((3 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |	/* div = 16 */
			SPI_CR1_MSTR |
			/* SPI_CR1_CPOL | */ // = 0
			SPI_CR1_CPHA | // = 1
			0,
		.cr2 = SPI_CR2_16BIT_MODE
	},
	.direct_io = {
		/* injector drivers */
		[0] = {.port = GPIOA, .pad = 3},
		[1] = {.port = GPIOA, .pad = 4},
		[2] = {.port = nullptr, .pad = 0},
		[3] = {.port = nullptr, .pad = 0},
		/* ignition pre-drivers */
		[4] = {.port = GPIOA, .pad = 0},
		[5] = {.port = GPIOA, .pad = 1},
//GPGD mode is not supported yet, ignition mode does not support spi on/off commands
//	  so ignition signals should be directly driven
		[6] = {.port = GPIOD, .pad = 0},
// meaning even if we do not use it we need a pin for now
		[7] = {.port = GPIOD, .pad = 1},
	},
	.en = {.port = GPIOA, .pad = 6}, // copy-paste with setMode code!
	// TODO: pick from engineConfiguration->spi3sckPin or whatever SPI is used
	.sck = {.port = GPIOB, .pad = 3},
	.spkdur = Gpio::Unassigned,
	.nomi = Gpio::Unassigned,
	.maxi = Gpio::Unassigned
};

    if (engineConfiguration->engineType == engine_type_e::FRANKENSO_TEST_33810) {
	    int ret = mc33810_add(Gpio::MC33810_0_OUT_0, 0, &mc33810);
	    efiPrintf("*****************+ mc33810_add %d +*******************", ret);

#ifndef EFI_BOOTLOADER
	    // todo: add to more appropriate location?
	    addConsoleAction("injinfo", [](){
	        efiPrintf("injinfo index=%d", engine->fuelComputer.brokenInjector);
	    });
#endif // EFI_BOOTLOADER
	}
}
