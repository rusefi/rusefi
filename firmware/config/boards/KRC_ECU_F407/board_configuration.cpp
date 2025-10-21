#include "pch.h"

Gpio getCommsLedPin() {
    return Gpio::Unassigned;
}
Gpio getRunningLedPin() {
    return Gpio::Unassigned;
}
Gpio getWarningLedPin() {
    return Gpio::Unassigned;
}

void setBoardDefaultConfiguration() {

	//Digital out
	engineConfiguration->boostControlPin = Gpio::A8;
	engineConfiguration->fanPin = Gpio::B7;
	engineConfiguration->mainRelayPin = Gpio::C13;
	engineConfiguration->tachOutputPin = Gpio::D7;
	engineConfiguration->fuelPumpPin = Gpio::E11;

	//Input pin
	engineConfiguration->vehicleSpeedSensorInputPin = Gpio::B6;
	engineConfiguration->triggerInputPins[0] = Gpio::D3;
	engineConfiguration->camInputs[0] = Gpio::D4;

	// Idle configuration
        engineConfiguration->useStepperIdle = false;
        engineConfiguration->isDoubleSolenoidIdle = true;
        engineConfiguration->idle.solenoidPin = Gpio::B9;
        engineConfiguration->secondSolenoidPin = Gpio::B8;

	//Analog
	engineConfiguration->clt.adcChannel = EFI_ADC_0;
	engineConfiguration->iat.adcChannel = EFI_ADC_1;
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_6;
	engineConfiguration->vbattAdcChannel = EFI_ADC_4;
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_3;
	
        //Map and baro sensor
	engineConfiguration->map.sensor.type = MT_MPXH6400;
	engineConfiguration->baroSensor.type = MT_MPXH6400;

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

	// SPI1 AUX
	engineConfiguration->is_enabled_spi_1 = true;
	engineConfiguration->max31855spiDevice = SPI_DEVICE_1;
	engineConfiguration->spi1mosiPin = Gpio::B5;
	engineConfiguration->spi1misoPin = Gpio::B4;
	engineConfiguration->spi1sckPin = Gpio::B3;

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

	//CAN bus overwrites
	engineConfiguration->canTxPin = Gpio::D1;
	engineConfiguration->canRxPin = Gpio::D0;
        engineConfiguration->canWriteEnabled = true;
	engineConfiguration->canReadEnabled = true;
	engineConfiguration->canSleepPeriodMs = 50;
	engineConfiguration->canBaudRate = B500KBPS;

	//Engine configuration
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
	engineConfiguration->crankingInjectionMode = IM_SEQUENTIAL;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
	
	//Knock 
	engineConfiguration->enableSoftwareKnock = true;

}
