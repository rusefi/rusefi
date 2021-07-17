/**
 * @file boards/Prometheus/board_configuration.h
 *
 * @brief In this file we can override engine_configuration.cpp.
 *
 * @date Apr 22, 2017
 * @author andreika, (c) 2017
 */

#include "allsensors.h"
#include "fsio_impl.h"

#ifdef STM32F469xx
static bool is469 = true;
#else
static bool is469 = false;
#endif


static void setPrometheusDefaults(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR/*FOUR_STROKE_CAM_SENSOR*/);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	//engineConfiguration->useOnlyRisingEdgeForTrigger = true;
	setAlgorithm(LM_SPEED_DENSITY PASS_CONFIG_PARAMETER_SUFFIX);

	CONFIG(useCicPidForIdle) = true;

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SIMULTANEOUS;//IM_BATCH;// IM_SEQUENTIAL;

	engineConfiguration->globalTriggerAngleOffset = 114;	// the end of 19th tooth?

	engineConfiguration->specs.displacement = 1.645;
	engineConfiguration->injector.flow = 200;
	
	engineConfiguration->cranking.baseFuel = 25;		// ???
	engineConfiguration->cranking.rpm = 600;

	engineConfiguration->rpmHardLimit = 8500; // yes, 3k. let's play it safe for now
	
	engineConfiguration->map.sensor.type = MT_MPX4250;

	engineConfiguration->idleStepperReactionTime = 10;
	engineConfiguration->stepperDirectionPinMode = OM_INVERTED;
	engineConfiguration->useLinearCltSensor = true;

	//engineConfiguration->canNbcType = CAN_BUS_NBC_BMW;
	engineConfiguration->canNbcType = CAN_BUS_MAZDA_RX8;
	engineConfiguration->canReadEnabled = true;
	engineConfiguration->canWriteEnabled = false;

	engineConfiguration->tpsMin = convertVoltageTo10bitADC(0.250);
	engineConfiguration->tpsMax = convertVoltageTo10bitADC(4.538);
	engineConfiguration->tpsErrorDetectionTooLow = -10; // -10% open
	engineConfiguration->tpsErrorDetectionTooHigh = 110; // 110% open

	engineConfiguration->mapMinBufferLength = 4;

}

void setPinConfigurationOverrides(void) {
#if 0
	engineConfiguration->injectionPins[0] = is469 ? GPIOD_9  : GPIOB_14; // #1
	engineConfiguration->injectionPins[1] = is469 ? GPIOD_15 : GPIOC_7;  // #2
	engineConfiguration->injectionPins[2] = is469 ? GPIOD_10 : GPIOB_15; // #3
	engineConfiguration->injectionPins[3] = is469 ? GPIOD_14 : GPIOC_6;  // #4
	
	engineConfiguration->ignitionPins[0] = GPIOA_10;
	engineConfiguration->ignitionPins[1] = GPIOA_9;
	engineConfiguration->ignitionPins[2] = GPIOA_8;
	engineConfiguration->ignitionPins[3] = GPIOA_11;
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

void setSerialConfigurationOverrides(void) {
	engineConfiguration->useSerialPort = true;
	engineConfiguration->binarySerialTxPin = GPIOA_0;
	engineConfiguration->binarySerialRxPin = GPIOA_1;
//	engineConfiguration->consoleSerialTxPin = GPIOA_0;
//	engineConfiguration->consoleSerialRxPin = GPIOA_1;
	engineConfiguration->tunerStudioSerialSpeed = SERIAL_SPEED;
	engineConfiguration->uartConsoleSerialSpeed = SERIAL_SPEED;
}

void setSdCardConfigurationOverrides(void) {
	engineConfiguration->is_enabled_spi_1 = true;
	engineConfiguration->sdCardSpiDevice = SPI_DEVICE_1;
	engineConfiguration->sdCardCsPin = GPIOA_2;
	engineConfiguration->isSdCardEnabled = true;
}

/**
 * @brief   Board-specific configuration defaults.
 * @todo    Add your board-specific code, if any.
 */
void setBoardDefaultConfiguration(void) {
	// give a chance to trigger SWD programmer... Wait for 2 secs (=2000 ms).
	// TODO: remove it when the bootloader is ready
	chThdSleepMilliseconds(2000);

	setSerialConfigurationOverrides();

	engineConfiguration->vbattAdcChannel = EFI_ADC_13;
	engineConfiguration->tps1_1AdcChannel = is469 ? EFI_ADC_7 : EFI_ADC_14;
	engineConfiguration->map.sensor.hwChannel = is469 ? EFI_ADC_8 : EFI_ADC_15;
	engineConfiguration->clt.adcChannel = EFI_ADC_3;
	engineConfiguration->iat.adcChannel = EFI_ADC_4;
	engineConfiguration->afr.hwChannel = is469 ? EFI_ADC_9 : EFI_ADC_8;

	engineConfiguration->adcVcc = ADC_VCC;

	engineConfiguration->baroSensor.hwChannel = EFI_ADC_NONE;
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_NONE;

	engineConfiguration->injectionPins[0] = is469 ? GPIOD_9  : GPIOB_14; // #1
	engineConfiguration->injectionPins[1] = is469 ? GPIOD_15 : GPIOC_7;  // #2
	engineConfiguration->injectionPins[2] = is469 ? GPIOD_10 : GPIOB_15; // #3
	engineConfiguration->injectionPins[3] = is469 ? GPIOD_14 : GPIOC_6;  // #4
	
	engineConfiguration->ignitionPins[0] = GPIOA_10;
	engineConfiguration->ignitionPins[1] = GPIOA_9;
	engineConfiguration->ignitionPins[2] = GPIOA_8;
	engineConfiguration->ignitionPins[3] = GPIOA_11;
	engineConfiguration->ignitionPinMode = OM_INVERTED;

	engineConfiguration->vbattDividerCoeff = ((float) (2 + 10)) / 2;
	engineConfiguration->clt.config.bias_resistor = 2700;
	engineConfiguration->iat.config.bias_resistor = 2700;
	
	engineConfiguration->useStepperIdle = true;
	engineConfiguration->idle.stepperDirectionPin = is469 ? GPIOB_14 : GPIOB_12;
	engineConfiguration->idle.stepperStepPin = is469 ? GPIOB_15 : GPIOB_13;
	engineConfiguration->stepperEnablePin = GPIO_UNASSIGNED;
	engineConfiguration->stepperEnablePinMode = OM_DEFAULT;

	engineConfiguration->communicationLedPin = GPIO_UNASSIGNED;// GPIOA_13; // yellow LED
	engineConfiguration->runningLedPin = GPIOA_13; //GPIOA_13; // yellow LED
	engineConfiguration->warningLedPin = GPIO_UNASSIGNED;

	engineConfiguration->triggerInputPins[0] = GPIOA_5;
	engineConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->triggerInputPins[2] = GPIO_UNASSIGNED;
	engineConfiguration->camInputs[0] = is469 ? GPIOE_9 : GPIOA_6;
	
	engineConfiguration->tachOutputPin = GPIOC_8;
	engineConfiguration->tachOutputPinMode = OM_DEFAULT;
	engineConfiguration->fuelPumpPin = is469 ? GPIOD_6 : GPIOB_7;
	engineConfiguration->fuelPumpPinMode = OM_DEFAULT;
	engineConfiguration->mainRelayPin = is469 ? GPIOB_11 : GPIOB_2;
	engineConfiguration->mainRelayPinMode = OM_DEFAULT;
	engineConfiguration->fanPin = GPIOC_9;
	engineConfiguration->fanPinMode = OM_DEFAULT;
	engineConfiguration->malfunctionIndicatorPin = GPIOC_1;
	engineConfiguration->malfunctionIndicatorPinMode = OM_DEFAULT;

	// starter block
	setFsio(0, (is469 ? GPIOB_10 : GPIOB_1), STARTER_RELAY_LOGIC PASS_CONFIG_PARAMETER_SUFFIX);

	
	// debug pad
	engineConfiguration->triggerSimulatorPins[0] = GPIOD_8;
	engineConfiguration->triggerSimulatorPinModes[0] = OM_DEFAULT;

	// not used
	engineConfiguration->externalKnockSenseAdc = EFI_ADC_NONE;
	engineConfiguration->displayMode = DM_NONE;
	engineConfiguration->HD44780_rs = GPIO_UNASSIGNED;
	engineConfiguration->HD44780_e = GPIO_UNASSIGNED;
	engineConfiguration->HD44780_db4 = GPIO_UNASSIGNED;
	engineConfiguration->HD44780_db5 = GPIO_UNASSIGNED;
	engineConfiguration->HD44780_db6 = GPIO_UNASSIGNED;
	engineConfiguration->HD44780_db7 = GPIO_UNASSIGNED;
	for (int i = 0; i < DIGIPOT_COUNT ; i++) {
		engineConfiguration->digitalPotentiometerChipSelect[i] = GPIO_UNASSIGNED;
	}
	engineConfiguration->triggerSimulatorPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->triggerSimulatorPins[2] = GPIO_UNASSIGNED;
	engineConfiguration->triggerSimulatorPinModes[1] = OM_DEFAULT;
	engineConfiguration->triggerSimulatorPinModes[2] = OM_DEFAULT;
	engineConfiguration->vehicleSpeedSensorInputPin = GPIO_UNASSIGNED;

	/////////////////////////////////////////////////////////
	
	engineConfiguration->is_enabled_spi_1 = true;
	engineConfiguration->is_enabled_spi_2 = false;
	CONFIG(is_enabled_spi_3) = true;
	
	engineConfiguration->spi1mosiPin = GPIOB_5;
	engineConfiguration->spi1MosiMode = PO_DEFAULT;	// PAL_STM32_OTYPE_PUSHPULL
	engineConfiguration->spi1misoPin = GPIOB_4;
	engineConfiguration->spi1MisoMode = PO_DEFAULT;	// PAL_STM32_OTYPE_PUSHPULL
	engineConfiguration->spi1sckPin = GPIOB_3;
	engineConfiguration->spi1SckMode = PO_DEFAULT;	// PAL_STM32_OTYPE_PUSHPULL

	engineConfiguration->spi3mosiPin = GPIOC_12;
	engineConfiguration->spi3MosiMode = PO_OPENDRAIN; // 4
	engineConfiguration->spi3misoPin = GPIOC_11;
	engineConfiguration->spi3MisoMode = PO_PULLUP; // 32
	engineConfiguration->spi3sckPin = GPIOC_10;
	engineConfiguration->spi3SckMode = PO_OPENDRAIN; // 4

	engineConfiguration->hip9011SpiDevice = SPI_DEVICE_3;
	engineConfiguration->hip9011CsPin = is469 ? GPIOD_1 : GPIOD_2;
	engineConfiguration->hip9011CsPinMode = OM_OPENDRAIN;
	engineConfiguration->hip9011IntHoldPin = GPIOC_14;
	engineConfiguration->hip9011IntHoldPinMode = OM_OPENDRAIN;
    engineConfiguration->hipOutputChannel = EFI_ADC_10; // PC0
    engineConfiguration->isHip9011Enabled = true;

	engineConfiguration->cj125SpiDevice = SPI_DEVICE_3;
	engineConfiguration->cj125ua = is469 ? EFI_ADC_9 : EFI_ADC_8;
	engineConfiguration->cj125ur = EFI_ADC_12;
	engineConfiguration->cj125CsPin = GPIOA_15;
	engineConfiguration->cj125CsPinMode = OM_OPENDRAIN;
	engineConfiguration->wboHeaterPin = GPIOC_13;
	engineConfiguration->o2heaterPin = GPIOC_13;
	//engineConfiguration->isCJ125Enabled = true;
	engineConfiguration->isCJ125Enabled = false;

	engineConfiguration->canTxPin = GPIOB_9;
	engineConfiguration->canRxPin = GPIOB_8;
	
	//!!!!!!!!!!!!!!!
#if 1
	setPrometheusDefaults(PASS_ENGINE_PARAMETER_SIGNATURE);
#endif

	//!!!!!!!!!!!!!!!!!!!
	//engineConfiguration->silentTriggerError = true;

	//!!!!!!!!!!!!!
	//engineConfiguration->isEngineChartEnabled = false;
}

