/**
 * @file board_extra.cpp
 */

#include "pch.h"
#include "hip9011_logic.h"

/**
 * @brief   Board-specific pin configuration code overrides. Needed by bootloader code.
 * @todo    Add your board-specific code, if any.
 */
void setPinConfigurationOverrides() {
}

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
	return Gpio::D13; // orange LED on discovery
}

Gpio getCommsLedPin() {
	return Gpio::D15; // blue LED on discovery
}

Gpio getRunningLedPin() {
	return Gpio::D12; // green LED on discovery
}

#if EFI_HIP_9011
static void setHip9011FrankensoPinout() {
	/**
	 * SPI on PB13/14/15
	 */
	//	engineConfiguration->hip9011CsPin = Gpio::D0; // rev 0.1

	engineConfiguration->isHip9011Enabled = true;
	engineConfiguration->hip9011PrescalerAndSDO = HIP_8MHZ_PRESCALER; // 8MHz chip
	engineConfiguration->is_enabled_spi_2 = true;
	// todo: convert this to rusEfi, hardware-independent enum
#if EFI_PROD_CODE
#ifdef EFI_HIP_CS_PIN
	engineConfiguration->hip9011CsPin = EFI_HIP_CS_PIN;
#else
	engineConfiguration->hip9011CsPin = Gpio::B0; // rev 0.4
#endif
	engineConfiguration->hip9011CsPinMode = OM_OPENDRAIN;

	engineConfiguration->hip9011IntHoldPin = Gpio::B11;
	engineConfiguration->hip9011IntHoldPinMode = OM_OPENDRAIN;

	engineConfiguration->spi2SckMode = PO_OPENDRAIN; // 4
	engineConfiguration->spi2MosiMode = PO_OPENDRAIN; // 4
	engineConfiguration->spi2MisoMode = PO_PULLUP; // 32
#endif /* EFI_PROD_CODE */

	engineConfiguration->hip9011Gain = 1;

	if (!engineConfiguration->useTpicAdvancedMode) {
	    engineConfiguration->hipOutputChannel = EFI_ADC_10; // PC0
	}
}
#endif

#if EFI_MEMS
static void configureAccelerometerPins() {
//	engineConfiguration->LIS302DLCsPin = Gpio::E3; // we have a conflict with VVT output on Miata
// 	engineConfiguration->is_enabled_spi_1 = true; // we have a conflict with PA5 input pin

	// stm32f4discovery defaults
	engineConfiguration->spi1mosiPin = Gpio::A7;
	engineConfiguration->spi1misoPin = Gpio::A6;
	engineConfiguration->spi1sckPin = Gpio::A5;
}
#endif // EFI_MEMS

/**
 * @brief	Hardware board-specific default configuration (GPIO pins, ADC channels, SPI configs etc.)
 */
void setBoardDefaultConfiguration() {
	setDefaultFrankensoStepperIdleParameters();
	setCanFrankensoDefaults();

#if EFI_HIP_9011
	setHip9011FrankensoPinout();
#endif /* EFI_HIP_9011 */

	// set optional subsystem configs
#if EFI_MEMS
	// this would override some values from above
	configureAccelerometerPins();
#endif /* EFI_MEMS */



	engineConfiguration->map.sensor.hwChannel = EFI_ADC_4;
	engineConfiguration->clt.adcChannel = EFI_ADC_6;
	engineConfiguration->iat.adcChannel = EFI_ADC_7;
	engineConfiguration->afr.hwChannel = EFI_ADC_14;

	engineConfiguration->accelerometerSpiDevice = SPI_DEVICE_1;

	engineConfiguration->triggerSimulatorPins[0] = Gpio::D1;
	engineConfiguration->triggerSimulatorPins[1] = Gpio::D2;

	engineConfiguration->triggerInputPins[0] = Gpio::C6;
	engineConfiguration->triggerInputPins[1] = Gpio::A5;

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
