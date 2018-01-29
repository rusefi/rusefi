/**
 * @file boards/Prometheus/board_configuration.h
 *
 * @brief In this file we can override engine_configuration.cpp.
 *
 * @date Apr 22, 2017
 * @author andreika, (c) 2017
 */

#include "main.h"
#include "engine.h"
#include "engine_math.h"
#include "allsensors.h"
#include "fsio_impl.h"
#include "engine_configuration.h"

EXTERN_ENGINE;

#ifdef STM32F469xx
static bool is469 = true;
#else
static bool is469 = false;
#endif


static void setPrometheusDefaults(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR/*FOUR_STROKE_CAM_SENSOR*/);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	//engineConfiguration->useOnlyRisingEdgeForTrigger = true;
	setAlgorithm(LM_SPEED_DENSITY PASS_ENGINE_PARAMETER_SUFFIX);

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SIMULTANEOUS;//IM_BATCH;// IM_SEQUENTIAL;

	engineConfiguration->globalTriggerAngleOffset = 114;	// the end of 19th tooth?

	engineConfiguration->specs.displacement = 1.645;
	engineConfiguration->injector.flow = 200;
	
	engineConfiguration->cranking.baseFuel = 5;		// ???
	engineConfiguration->crankingChargeAngle = 70;
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

	boardConfiguration->mapMinBufferLength = 4;

}

void setPinConfigurationOverrides(void) {
#if 0
	boardConfiguration->injectionPins[0] = is469 ? GPIOD_9  : GPIOB_14; // #1
	boardConfiguration->injectionPins[1] = is469 ? GPIOD_15 : GPIOC_7;  // #2
	boardConfiguration->injectionPins[2] = is469 ? GPIOD_10 : GPIOB_15; // #3
	boardConfiguration->injectionPins[3] = is469 ? GPIOD_14 : GPIOC_6;  // #4
	
	boardConfiguration->ignitionPins[0] = GPIOA_10;
	boardConfiguration->ignitionPins[1] = GPIOA_9;
	boardConfiguration->ignitionPins[2] = GPIOA_8;
	boardConfiguration->ignitionPins[3] = GPIOA_11;
	boardConfiguration->ignitionPinMode = OM_INVERTED;

	enginePins.startInjectionPins();
	enginePins.startIgnitionPins();

	for (int i = 0; i < 4; i++) {
		enginePins.injectors[i].setLow();
		enginePins.coils[i].setLow();
	}
#else
	palSetPadMode(GPIOA, 10, PAL_MODE_OUTPUT_PUSHPULL);
	palWritePad(GPIOA, 10, true);
	palSetPadMode(GPIOA, 9, PAL_MODE_OUTPUT_PUSHPULL);
	palWritePad(GPIOA, 9, true);
	palSetPadMode(GPIOA, 8, PAL_MODE_OUTPUT_PUSHPULL);
	palWritePad(GPIOA, 8, true);
	palSetPadMode(GPIOA, 11, PAL_MODE_OUTPUT_PUSHPULL);
	palWritePad(GPIOA, 11, true);

	palSetPadMode(GPIOD, 9, PAL_MODE_OUTPUT_PUSHPULL);
	palWritePad(GPIOD, 9, false);
	palSetPadMode(GPIOD, 15, PAL_MODE_OUTPUT_PUSHPULL);
	palWritePad(GPIOD, 15, false);
	palSetPadMode(GPIOD, 10, PAL_MODE_OUTPUT_PUSHPULL);
	palWritePad(GPIOD, 10, false);
	palSetPadMode(GPIOD, 14, PAL_MODE_OUTPUT_PUSHPULL);
	palWritePad(GPIOD, 14, false);
#endif
}

void setSerialConfigurationOverrides(void) {
	boardConfiguration->useSerialPort = true;
	engineConfiguration->binarySerialTxPin = GPIOA_0;
	engineConfiguration->binarySerialRxPin = GPIOA_1;
	engineConfiguration->consoleSerialTxPin = GPIOA_0;
	engineConfiguration->consoleSerialRxPin = GPIOA_1;
	boardConfiguration->tunerStudioSerialSpeed = SERIAL_SPEED;
	engineConfiguration->uartConsoleSerialSpeed = SERIAL_SPEED;
}

void setSdCardConfigurationOverrides(void) {
	boardConfiguration->is_enabled_spi_1 = true;
	engineConfiguration->sdCardSpiDevice = SPI_DEVICE_1;
	boardConfiguration->sdCardCsPin = GPIOA_2;
	boardConfiguration->isSdCardEnabled = true;
}

/**
 * @brief   Board-specific configuration code overrides.
 * @todo    Add your board-specific code, if any.
 */
void setBoardConfigurationOverrides(void) {
	// give a chance to trigger SWD programmer... Wait for 2 secs (=2000 ms).
	// TODO: remove it when the bootloader is ready
	Delay(2000);

	setSerialConfigurationOverrides();

	engineConfiguration->vbattAdcChannel = EFI_ADC_13;
	engineConfiguration->tpsAdcChannel = is469 ? EFI_ADC_7 : EFI_ADC_14;
	engineConfiguration->map.sensor.hwChannel = is469 ? EFI_ADC_8 : EFI_ADC_15;
	engineConfiguration->clt.adcChannel = EFI_ADC_3;
	engineConfiguration->iat.adcChannel = EFI_ADC_4;
	engineConfiguration->afr.hwChannel = is469 ? EFI_ADC_9 : EFI_ADC_8;

	engineConfiguration->adcVcc = ADC_VCC;

	engineConfiguration->baroSensor.hwChannel = EFI_ADC_NONE;
	engineConfiguration->pedalPositionChannel = EFI_ADC_NONE;

	boardConfiguration->injectionPins[0] = is469 ? GPIOD_9  : GPIOB_14; // #1
	boardConfiguration->injectionPins[1] = is469 ? GPIOD_15 : GPIOC_7;  // #2
	boardConfiguration->injectionPins[2] = is469 ? GPIOD_10 : GPIOB_15; // #3
	boardConfiguration->injectionPins[3] = is469 ? GPIOD_14 : GPIOC_6;  // #4
	
	boardConfiguration->ignitionPins[0] = GPIOA_10;
	boardConfiguration->ignitionPins[1] = GPIOA_9;
	boardConfiguration->ignitionPins[2] = GPIOA_8;
	boardConfiguration->ignitionPins[3] = GPIOA_11;
	boardConfiguration->ignitionPinMode = OM_INVERTED;

	engineConfiguration->vbattDividerCoeff = ((float) (2 + 10)) / 2;
	engineConfiguration->clt.config.bias_resistor = 2700;
	engineConfiguration->iat.config.bias_resistor = 2700;
	
	boardConfiguration->useStepperIdle = true;
	boardConfiguration->idle.stepperDirectionPin = is469 ? GPIOB_14 : GPIOB_12;
	boardConfiguration->idle.stepperStepPin = is469 ? GPIOB_15 : GPIOB_13;
	engineConfiguration->stepperEnablePin = GPIO_UNASSIGNED;
	engineConfiguration->stepperEnablePinMode = OM_DEFAULT;

	engineConfiguration->communicationPin = GPIO_UNASSIGNED;// GPIOA_13; // yellow LED
	engineConfiguration->runningPin = GPIOA_13; //GPIOA_13; // yellow LED
	engineConfiguration->fatalErrorPin = GPIOA_13;
	engineConfiguration->warninigPin = GPIO_UNASSIGNED;
	engineConfiguration->configResetPin = GPIO_UNASSIGNED;

	boardConfiguration->triggerInputPins[0] = is469 ? GPIOA_5 : GPIOA_5;
	boardConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
	boardConfiguration->triggerInputPins[2] = GPIO_UNASSIGNED;
	engineConfiguration->camInput = is469 ? GPIOE_9 : GPIOA_6;
	
	boardConfiguration->tachOutputPin = GPIOC_8;
	boardConfiguration->tachOutputPinMode = OM_DEFAULT;
	boardConfiguration->fuelPumpPin = is469 ? GPIOD_6 : GPIOB_7;
	boardConfiguration->fuelPumpPinMode = OM_DEFAULT;
	boardConfiguration->mainRelayPin = is469 ? GPIOB_11 : GPIOB_2;
	boardConfiguration->mainRelayPinMode = OM_DEFAULT;
	boardConfiguration->fanPin = GPIOC_9;
	boardConfiguration->fanPinMode = OM_DEFAULT;
	boardConfiguration->malfunctionIndicatorPin = GPIOC_1;
	boardConfiguration->malfunctionIndicatorPinMode = OM_DEFAULT;

	// starter block
	setFsio(0, (is469 ? GPIOB_10 : GPIOB_1), STARTER_BLOCK PASS_ENGINE_PARAMETER_SUFFIX);

	
	// debug pad
	boardConfiguration->triggerSimulatorPins[0] = GPIOD_8;
	boardConfiguration->triggerSimulatorPinModes[0] = OM_DEFAULT;

	// not used
	engineConfiguration->dizzySparkOutputPin = GPIO_UNASSIGNED;
	engineConfiguration->externalKnockSenseAdc = EFI_ADC_NONE;
	engineConfiguration->displayMode = DM_NONE;
	boardConfiguration->HD44780_rs = GPIO_UNASSIGNED;
	boardConfiguration->HD44780_e = GPIO_UNASSIGNED;
	boardConfiguration->HD44780_db4 = GPIO_UNASSIGNED;
	boardConfiguration->HD44780_db5 = GPIO_UNASSIGNED;
	boardConfiguration->HD44780_db6 = GPIO_UNASSIGNED;
	boardConfiguration->HD44780_db7 = GPIO_UNASSIGNED;
	boardConfiguration->digitalPotentiometerChipSelect[0] = GPIO_UNASSIGNED;
	boardConfiguration->digitalPotentiometerChipSelect[1] = GPIO_UNASSIGNED;
	boardConfiguration->digitalPotentiometerChipSelect[2] = GPIO_UNASSIGNED;
	boardConfiguration->digitalPotentiometerChipSelect[3] = GPIO_UNASSIGNED;
	boardConfiguration->triggerSimulatorPins[1] = GPIO_UNASSIGNED;
	boardConfiguration->triggerSimulatorPins[2] = GPIO_UNASSIGNED;
	boardConfiguration->triggerSimulatorPinModes[1] = OM_DEFAULT;
	boardConfiguration->triggerSimulatorPinModes[2] = OM_DEFAULT;
	boardConfiguration->vehicleSpeedSensorInputPin = GPIO_UNASSIGNED;
	boardConfiguration->boardTestModeJumperPin = GPIO_UNASSIGNED;
	boardConfiguration->acRelayPin = GPIO_UNASSIGNED;
	boardConfiguration->digitalPotentiometerSpiDevice = SPI_NONE;
	boardConfiguration->max31855spiDevice = SPI_NONE;

	/////////////////////////////////////////////////////////
	
	boardConfiguration->is_enabled_spi_1 = true;
	boardConfiguration->is_enabled_spi_2 = false;
	boardConfiguration->is_enabled_spi_3 = true;
	
	boardConfiguration->spi1mosiPin = GPIOB_5;
	engineConfiguration->spi1MosiMode = 0;	// PAL_STM32_OTYPE_PUSHPULL
	boardConfiguration->spi1misoPin = GPIOB_4;
	engineConfiguration->spi1MisoMode = 0;	// PAL_STM32_OTYPE_PUSHPULL
	boardConfiguration->spi1sckPin = GPIOB_3;
	engineConfiguration->spi1SckMode = 0;	// PAL_STM32_OTYPE_PUSHPULL

	boardConfiguration->spi3mosiPin = GPIOC_12;
	engineConfiguration->spi3MosiMode = PAL_STM32_OTYPE_OPENDRAIN; // 4
	boardConfiguration->spi3misoPin = GPIOC_11;
	engineConfiguration->spi3MisoMode = PAL_STM32_PUPDR_PULLUP; // 32
	boardConfiguration->spi3sckPin = GPIOC_10;
	engineConfiguration->spi3SckMode = PAL_STM32_OTYPE_OPENDRAIN; // 4

	engineConfiguration->hip9011SpiDevice = SPI_DEVICE_3;
	boardConfiguration->hip9011CsPin = is469 ? GPIOD_1 : GPIOD_2;
	boardConfiguration->hip9011CsPinMode = OM_OPENDRAIN;
	boardConfiguration->hip9011IntHoldPin = GPIOC_14;
	boardConfiguration->hip9011IntHoldPinMode = OM_OPENDRAIN;
    engineConfiguration->hipOutputChannel = EFI_ADC_10; // PC0
    boardConfiguration->isHip9011Enabled = true;

	engineConfiguration->cj125SpiDevice = SPI_DEVICE_3;
	engineConfiguration->cj125ua = is469 ? EFI_ADC_9 : EFI_ADC_8;
	engineConfiguration->cj125ur = EFI_ADC_12;
	boardConfiguration->cj125CsPin = GPIOA_15;
	engineConfiguration->cj125CsPinMode = OM_OPENDRAIN;
	boardConfiguration->wboHeaterPin = GPIOC_13;
	boardConfiguration->o2heaterPin = GPIOC_13;
	//boardConfiguration->isCJ125Enabled = true;
	boardConfiguration->isCJ125Enabled = false;

	boardConfiguration->canDeviceMode = CD_USE_CAN1;
	boardConfiguration->canTxPin = GPIOB_9;
	boardConfiguration->canRxPin = GPIOB_8;
	
	//!!!!!!!!!!!!!!!
#if 1
	setPrometheusDefaults(PASS_ENGINE_PARAMETER_SIGNATURE);
#endif

	//!!!!!!!!!!!!!!!!!!!
	//engineConfiguration->silentTriggerError = true;

	//!!!!!!!!!!!!!
	//engineConfiguration->isEngineChartEnabled = false;
}

