#include "pch.h"
#include "defaults.h"
#include "board_overrides.h"

Gpio getCommsLedPin() {
    return Gpio::Unassigned;
}
Gpio getRunningLedPin() {
    return Gpio::Unassigned; 
}
Gpio getWarningLedPin() {
    return Gpio::Unassigned;
}

static void KRC_ECU_V3_boardDefaultConfiguration() {

	//Digital out
	engineConfiguration->boostControlPin = Gpio::A8;
	engineConfiguration->mainRelayPin = Gpio::C13;
	engineConfiguration->tachOutputPin = Gpio::D7;
	engineConfiguration->fuelPumpPin = Gpio::E11;

	//Input pin
	engineConfiguration->vehicleSpeedSensorInputPin = Gpio::D6;
	engineConfiguration->triggerInputPins[0] = Gpio::D3;
	engineConfiguration->camInputs[0] = Gpio::D4;

	//Idle configuration
  engineConfiguration->useStepperIdle = false;

	//Analog
	engineConfiguration->clt.adcChannel = EFI_ADC_0;
	engineConfiguration->iat.adcChannel = EFI_ADC_1;
	engineConfiguration->vbattAdcChannel = EFI_ADC_4;

  //Baro sensor
  engineConfiguration->baroSensor.hwChannel = EFI_ADC_NONE;
  engineConfiguration->lps25BaroSensorScl = Gpio::B10;
	engineConfiguration->lps25BaroSensorSda = Gpio::B11;

	//DBW throotle
	engineConfiguration->etbIo[0].directionPin1 = Gpio::C6;   // DIR pin
	engineConfiguration->etbIo[0].disablePin = Gpio::C7;      // Disable pin
	engineConfiguration->etbIo[0].controlPin = Gpio::C8;      // PWM pin
	engineConfiguration->etbIo[0].directionPin2 = Gpio::Unassigned;
	engineConfiguration->etb_use_two_wires = false;

	//Bias resistor
	engineConfiguration->clt.config.bias_resistor = 2200;
	engineConfiguration->iat.config.bias_resistor = 2200;

	//Analog calc
  engineConfiguration->analogInputDividerCoefficient = 1.55f;
  engineConfiguration->vbattDividerCoeff = (7.47f / 1.0f);
  engineConfiguration->adcVcc = 3.12f;

	// SPI2 EGT
	engineConfiguration->is_enabled_spi_2 = true;
	engineConfiguration->max31855spiDevice = SPI_DEVICE_2;
	engineConfiguration->spi1mosiPin = Gpio::Unassigned;
	engineConfiguration->spi1misoPin = Gpio::B14;
	engineConfiguration->spi1sckPin = Gpio::B13;
	engineConfiguration->max31855_cs[0] = Gpio::B12;

  //SPI3 SD card
  engineConfiguration->isSdCardEnabled = true;
	engineConfiguration->is_enabled_spi_3 = true;
	engineConfiguration->sdCardSpiDevice = SPI_DEVICE_3;
	engineConfiguration->spi3mosiPin = Gpio::C10;
	engineConfiguration->spi3misoPin = Gpio::C11;
	engineConfiguration->spi3sckPin = Gpio::C12;
	engineConfiguration->sdCardCsPin = Gpio::D2;
	engineConfiguration->sdCardLogFrequency = 20;

	//CAN1 bus overwrites
	engineConfiguration->canTxPin = Gpio::D1;
	engineConfiguration->canRxPin = Gpio::D0;
  engineConfiguration->canWriteEnabled = true;
	engineConfiguration->canReadEnabled = true;
	engineConfiguration->canSleepPeriodMs = 50;
	engineConfiguration->canBaudRate = B500KBPS;
	
        //CAN2 bus overwrites
	engineConfiguration->can2RxPin = Gpio::B5;
	engineConfiguration->can2TxPin = Gpio::B6;
	engineConfiguration->can2BaudRate = B500KBPS;

	//Engine configuration
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
	
	//Knock 
	engineConfiguration->enableSoftwareKnock = true;

}

void setup_custom_board_overrides() {

	custom_board_DefaultConfiguration = KRC_ECU_V3_boardDefaultConfiguration;
}
