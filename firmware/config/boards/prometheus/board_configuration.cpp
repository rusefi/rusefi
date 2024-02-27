/**
 * @file boards/Prometheus/board_configuration.h
 *
 * @brief In this file we can override engine_configuration.cpp.
 *
 * @date Apr 22, 2017
 * @author andreika, (c) 2017
 */

#include "pch.h"

#ifdef STM32F469xx
static bool is469 = true;
#else
static bool is469 = false;
#endif

Gpio getCommsLedPin() {
	// this board has no comms led
	return Gpio::Unassigned;
}

Gpio getWarningLedPin() {
	// this board has no warning led
	return Gpio::Unassigned;
}

static void setPrometheusDefaults() {
	engineConfiguration->useCicPidForIdle = true;

	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_3_4_2;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SIMULTANEOUS;//IM_BATCH;// IM_SEQUENTIAL;

	engineConfiguration->globalTriggerAngleOffset = 114;	// the end of 19th tooth?

	engineConfiguration->displacement = 1.645;
	engineConfiguration->injector.flow = 200;

	engineConfiguration->cranking.baseFuel = 25;
	engineConfiguration->cranking.rpm = 600;

	engineConfiguration->rpmHardLimit = 8500;

	engineConfiguration->map.sensor.type = MT_MPX4250;

	engineConfiguration->idleStepperReactionTime = 10;
	engineConfiguration->stepperDirectionPinMode = OM_INVERTED;
	engineConfiguration->useLinearCltSensor = true;

	engineConfiguration->canNbcType = CAN_BUS_MAZDA_RX8;
	engineConfiguration->canReadEnabled = true;
	engineConfiguration->canWriteEnabled = false;

	engineConfiguration->tpsMin = convertVoltageTo10bitADC(0.250);
	engineConfiguration->tpsMax = convertVoltageTo10bitADC(4.538);
	engineConfiguration->tpsErrorDetectionTooLow = -10; // -10% open
	engineConfiguration->tpsErrorDetectionTooHigh = 110; // 110% open

	engineConfiguration->mapMinBufferLength = 4;

}

void setPinConfigurationOverrides() {
#if 0
	engineConfiguration->injectionPins[0] = is469 ? Gpio::D9  : Gpio::B14; // #1
	engineConfiguration->injectionPins[1] = is469 ? Gpio::D15 : Gpio::C7;  // #2
	engineConfiguration->injectionPins[2] = is469 ? Gpio::D10 : Gpio::B15; // #3
	engineConfiguration->injectionPins[3] = is469 ? Gpio::D14 : Gpio::C6;  // #4

	engineConfiguration->ignitionPins[0] = Gpio::A10;
	engineConfiguration->ignitionPins[1] = Gpio::A9;
	engineConfiguration->ignitionPins[2] = Gpio::A8;
	engineConfiguration->ignitionPins[3] = Gpio::A11;
	engineConfiguration->ignitionPinMode = OM_INVERTED;

	enginePins.startInjectionPins();
	enginePins.startIgnitionPins();

	for (int i = 0; i < 4; i++) {
		enginePins.injectors[i].setLow();
		enginePins.coils[i].setLow();
	}
#else
	palSetPadMode(GPIOA, 10, PAL_MODE_OUTPUT_PUSHPULL);
	palWritePad(GPIOA, 10, 1);
	palSetPadMode(GPIOA, 9, PAL_MODE_OUTPUT_PUSHPULL);
	palWritePad(GPIOA, 9, 1);
	palSetPadMode(GPIOA, 8, PAL_MODE_OUTPUT_PUSHPULL);
	palWritePad(GPIOA, 8, 1);
	palSetPadMode(GPIOA, 11, PAL_MODE_OUTPUT_PUSHPULL);
	palWritePad(GPIOA, 11, 1);

	palSetPadMode(GPIOD, 9, PAL_MODE_OUTPUT_PUSHPULL);
	palWritePad(GPIOD, 9, 0);
	palSetPadMode(GPIOD, 15, PAL_MODE_OUTPUT_PUSHPULL);
	palWritePad(GPIOD, 15, 0);
	palSetPadMode(GPIOD, 10, PAL_MODE_OUTPUT_PUSHPULL);
	palWritePad(GPIOD, 10, 0);
	palSetPadMode(GPIOD, 14, PAL_MODE_OUTPUT_PUSHPULL);
	palWritePad(GPIOD, 14, 0);
#endif
}

/**
 * @brief   Board-specific configuration defaults.

 */
void setBoardDefaultConfiguration() {
	// give a chance to trigger SWD programmer... Wait for 2 secs (=2000 ms).
//	chThdSleepMilliseconds(2000);

	engineConfiguration->binarySerialTxPin = Gpio::A0;
	engineConfiguration->binarySerialRxPin = Gpio::A1;
	engineConfiguration->tunerStudioSerialSpeed = SERIAL_SPEED;
	engineConfiguration->uartConsoleSerialSpeed = SERIAL_SPEED;

	engineConfiguration->vbattAdcChannel = EFI_ADC_13;
	engineConfiguration->tps1_1AdcChannel = is469 ? EFI_ADC_7 : EFI_ADC_14;
	engineConfiguration->map.sensor.hwChannel = is469 ? EFI_ADC_8 : EFI_ADC_15;
	engineConfiguration->clt.adcChannel = EFI_ADC_3;
	engineConfiguration->iat.adcChannel = EFI_ADC_4;
	engineConfiguration->afr.hwChannel = is469 ? EFI_ADC_9 : EFI_ADC_8;

	engineConfiguration->adcVcc = ADC_VCC;

	engineConfiguration->baroSensor.hwChannel = EFI_ADC_NONE;
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_NONE;

	engineConfiguration->injectionPins[0] = is469 ? Gpio::D9  : Gpio::B14; // #1
	engineConfiguration->injectionPins[1] = is469 ? Gpio::D15 : Gpio::C7;  // #2
	engineConfiguration->injectionPins[2] = is469 ? Gpio::D10 : Gpio::B15; // #3
	engineConfiguration->injectionPins[3] = is469 ? Gpio::D14 : Gpio::C6;  // #4

	engineConfiguration->ignitionPins[0] = Gpio::A10;
	engineConfiguration->ignitionPins[1] = Gpio::A9;
	engineConfiguration->ignitionPins[2] = Gpio::A8;
	engineConfiguration->ignitionPins[3] = Gpio::A11;
	engineConfiguration->ignitionPinMode = OM_INVERTED;

	engineConfiguration->vbattDividerCoeff = ((float) (2 + 10)) / 2;
	engineConfiguration->clt.config.bias_resistor = 2700;
	engineConfiguration->iat.config.bias_resistor = 2700;

	engineConfiguration->useStepperIdle = true;
	engineConfiguration->idle.stepperDirectionPin = is469 ? Gpio::B14 : Gpio::B12;
	engineConfiguration->idle.stepperStepPin = is469 ? Gpio::B15 : Gpio::B13;
	engineConfiguration->stepperEnablePin = Gpio::Unassigned;

	engineConfiguration->triggerInputPins[0] = Gpio::A5;
	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;
	engineConfiguration->camInputs[0] = is469 ? Gpio::E9 : Gpio::A6;

	engineConfiguration->tachOutputPin = Gpio::C8;
	engineConfiguration->fuelPumpPin = is469 ? Gpio::D6 : Gpio::B7;
	engineConfiguration->mainRelayPin = is469 ? Gpio::B11 : Gpio::B2;
	engineConfiguration->fanPin = Gpio::C9;
	engineConfiguration->malfunctionIndicatorPin = Gpio::C1;


	// debug pad
	engineConfiguration->triggerSimulatorPins[0] = Gpio::D8;

	// not used
	engineConfiguration->triggerSimulatorPins[1] = Gpio::Unassigned;
	engineConfiguration->vehicleSpeedSensorInputPin = Gpio::Unassigned;

	engineConfiguration->is_enabled_spi_2 = false;
	engineConfiguration->is_enabled_spi_3 = true;

	engineConfiguration->spi1mosiPin = Gpio::B5;
	engineConfiguration->spi1misoPin = Gpio::B4;
	engineConfiguration->spi1sckPin = Gpio::B3;

	engineConfiguration->spi3mosiPin = Gpio::C12;
	engineConfiguration->spi3MosiMode = PO_OPENDRAIN; // 4
	engineConfiguration->spi3misoPin = Gpio::C11;
	engineConfiguration->spi3MisoMode = PO_PULLUP; // 32
	engineConfiguration->spi3sckPin = Gpio::C10;
	engineConfiguration->spi3SckMode = PO_OPENDRAIN; // 4

	engineConfiguration->hip9011SpiDevice = SPI_DEVICE_3;
	engineConfiguration->hip9011CsPin = is469 ? Gpio::D1 : Gpio::D2;
	engineConfiguration->hip9011CsPinMode = OM_OPENDRAIN;
	engineConfiguration->hip9011IntHoldPin = Gpio::C14;
	engineConfiguration->hip9011IntHoldPinMode = OM_OPENDRAIN;
    engineConfiguration->hipOutputChannel = EFI_ADC_10; // PC0
    engineConfiguration->isHip9011Enabled = true;

	engineConfiguration->canTxPin = Gpio::B9;
	engineConfiguration->canRxPin = Gpio::B8;

	setPrometheusDefaults();

	engineConfiguration->is_enabled_spi_1 = true;
	engineConfiguration->sdCardSpiDevice = SPI_DEVICE_1;
	engineConfiguration->sdCardCsPin = Gpio::A2;
	engineConfiguration->isSdCardEnabled = true;
}

Gpio getRunningLedPin() {
	return Gpio::A13; //Gpio::A13; // yellow LED
}
