#include "pch.h"
#include "defaults.h"
#include "epicECU_meta.h"

// extern OutputPin accelerometerChipSelect;

Gpio getCommsLedPin() {
	return Gpio::Unassigned;
}

Gpio getRunningLedPin() {
	return Gpio::Unassigned;
}

Gpio getWarningLedPin() {
	return Gpio::Unassigned;
}

// board-specific configuration setup
void setBoardDefaultConfiguration() {
//	EFI_ADC_NONE = 0,
//
//  EFI_ADC_0 = 1, // PA0
//  EFI_ADC_1 = 2, // PA1
//  EFI_ADC_2 = 3, // PA2
//  EFI_ADC_3 = 4, // PA3
//  EFI_ADC_4 = 5, // PA4
//  EFI_ADC_5 = 6, // PA5
//  EFI_ADC_6 = 7, // PA6
//  EFI_ADC_7 = 8, // PA7
//  EFI_ADC_8 = 9, // PB0
//  EFI_ADC_9 = 10, // PB1
//  EFI_ADC_10 = 11, // PC0
//  EFI_ADC_11 = 12, // PC1
//  EFI_ADC_12 = 13, // PC2
//  EFI_ADC_13 = 14, // PC3
//  EFI_ADC_14 = 15, // PC4
//  EFI_ADC_15 = 16, // PC5
//
//    // engineConfiguration->injectionPins[0] = Gpio::F13;
    // engineConfiguration->ignitionPins[0] = Gpio::E15;

//   engineConfiguration->triggerInputPins[0] = Gpio::B1;
//	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;

	engineConfiguration->map.sensor.hwChannel = EFI_ADC_6;
	engineConfiguration->clt.adcChannel = EFI_ADC_13;
	engineConfiguration->iat.adcChannel = EFI_ADC_11;
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_1;
	engineConfiguration->tps1_2AdcChannel = EFI_ADC_0;
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_2;
	engineConfiguration->throttlePedalPositionSecondAdcChannel = EFI_ADC_7;
    engineConfiguration->analogInputDividerCoefficient = 2.0f;
    engineConfiguration->vbattDividerCoeff = 8.8f;
    engineConfiguration->fixSyncMisfire = 1;
    engineConfiguration->alwaysInstantRpm = 1;
    engineConfiguration->allowDFUwithIgn = 1;

    engineConfiguration->is_enabled_spi_2 = 1;
    engineConfiguration->isSdCardEnabled = 1;
    engineConfiguration->alwaysWriteSdCard = 1;
    engineConfiguration->sdCardCsPin = Gpio::B12;
    engineConfiguration->sdCardSpiDevice = SPI_DEVICE_2;
	engineConfiguration->spi2mosiPin = Gpio::B15;
	engineConfiguration->spi2misoPin = Gpio::B14;
	engineConfiguration->spi2sckPin  = Gpio::B13;


    engineConfiguration->is_enabled_spi_1 = 1;
	engineConfiguration->spi1mosiPin = Gpio::B5;
	engineConfiguration->spi1misoPin = Gpio::B4;
	engineConfiguration->spi1sckPin  = Gpio::B3;


//	if (!accelerometerChipSelect.isInitialized()) {
//		accelerometerChipSelect.initPin("mm-CS2", Gpio::D15);
//		accelerometerChipSelect.setValue(1);
//	}
    // accelerometer cs PD15



    engineConfiguration->canTxPin = Gpio::D1;
	engineConfiguration->canRxPin = Gpio::D0;
    // actually can3 by the spec sheet, maybe fine ?
	engineConfiguration->can2TxPin = Gpio::A15;
	engineConfiguration->can2RxPin = Gpio::A8;

	engineConfiguration->adcVcc = 3.3f;
	engineConfiguration->is_enabled_spi_3 = true;

	engineConfiguration->clt.config.bias_resistor = 2490;
	engineConfiguration->iat.config.bias_resistor = 2490;

	engineConfiguration->vbattAdcChannel = EFI_ADC_9;

	setupTLE9201IncludingStepper(/*PWM controlPin*/Gpio::J10, Gpio::J9, Gpio::J11);
	setupTLE9201IncludingStepper(/*PWM controlPin*/Gpio::J7, Gpio::J6, Gpio::J8, 1);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;
	engineConfiguration->tunerStudioSerialSpeed = 115200;
#ifdef TS_SECONDARY_UxART_PORT
	engineConfiguration->binarySerialTxPin = Gpio::B10;
	engineConfiguration->binarySerialRxPin = Gpio::B11;
#endif // TS_SECONDARY_UxART_PORT
}

bool isBoardAllowingLackOfPps() {
	return engineConfiguration->allowIdenticalPps;
}

#include "board_configuration_generated.h"

int getBoardMetaOutputsCount() {
	return efi::size(OUTPUTS);
}

int getBoardMetaLowSideOutputsCount() {
	return getBoardMetaOutputsCount() - 6;
}

Gpio* getBoardMetaOutputs() {
	return OUTPUTS;
}

int getBoardMetaDcOutputsCount() {
	if (engineConfiguration->engineType == engine_type_e::HONDA_OBD1 ||
	  engineConfiguration->engineType == engine_type_e::MAZDA_MIATA_NA6 ||
	  engineConfiguration->engineType == engine_type_e::MAZDA_MIATA_NA94 ||
	  engineConfiguration->engineType == engine_type_e::MAZDA_MIATA_NA96 ||
	  engineConfiguration->engineType == engine_type_e::MAZDA_MIATA_NB1 ||
	  engineConfiguration->engineType == engine_type_e::MAZDA_MIATA_NB2) {
		return 0;
	  }
	return 2;
}
