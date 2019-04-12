/**
 * @file boards/subaru-ej20g/board_configuration.h
 *
 * @brief In this file we can override engine_configuration.cpp.
 *
 * @date Feb 19, 2019
 * @author Andrey Gusakov, 2019
 */

#include "global.h"
#include "engine.h"
#include "engine_math.h"
#include "allsensors.h"
#include "fsio_impl.h"
#include "engine_configuration.h"

/* TLE6240 pins go right after on chips */
#define TLE6240_PIN(n)		((brain_pin_e)((int)BRAIN_PIN_LAST_ONCHIP + 1 + (n)))
/* MC33972 pins go right after TLE6240 */
#define MC33972_PIN(n)		((brain_pin_e)((int)BRAIN_PIN_LAST_ONCHIP + 1 + 16 + (n)))

EXTERN_ENGINE;

static void setSubaruEJ20GDefaults(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	setOperationMode(engineConfiguration, FOUR_STROKE_CAM_SENSOR);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	//engineConfiguration->useOnlyRisingEdgeForTrigger = true;
	setAlgorithm(LM_SPEED_DENSITY PASS_CONFIG_PARAMETER_SUFFIX);

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

	engineConfiguration->rpmHardLimit = 8500;

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

}

void setSerialConfigurationOverrides(void) {
	boardConfiguration->useSerialPort = false;
	engineConfiguration->binarySerialTxPin = GPIOE_1;
	engineConfiguration->binarySerialRxPin = GPIOE_0;
	engineConfiguration->consoleSerialTxPin = GPIOA_9;
	engineConfiguration->consoleSerialRxPin = GPIOA_10;
	boardConfiguration->tunerStudioSerialSpeed = SERIAL_SPEED;
	engineConfiguration->uartConsoleSerialSpeed = SERIAL_SPEED;
}

void setSdCardConfigurationOverrides(void) {
	boardConfiguration->is_enabled_spi_1 = false;
	engineConfiguration->sdCardSpiDevice = SPI_DEVICE_1;
	boardConfiguration->sdCardCsPin = GPIOA_2;
	boardConfiguration->isSdCardEnabled = false;
}

/**
 * @brief   Board-specific configuration code overrides.
 * @todo    Add your board-specific code, if any.
 */
void setBoardConfigurationOverrides(void) {
	setSerialConfigurationOverrides();

	engineConfiguration->vbattAdcChannel = EFI_ADC_6;
	engineConfiguration->tpsAdcChannel = EFI_ADC_13;
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_14;
	engineConfiguration->clt.adcChannel = EFI_ADC_8;
	/* not yet */
	engineConfiguration->iat.adcChannel = EFI_ADC_NONE;
	/* narrow */
	engineConfiguration->afr.hwChannel = EFI_ADC_9;

	engineConfiguration->adcVcc = ADC_VCC;

	engineConfiguration->baroSensor.hwChannel = EFI_ADC_NONE;
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_NONE;

	/* Injectors */
	boardConfiguration->injectionPins[0] = GPIOB_11;
	boardConfiguration->injectionPins[1] = GPIOA_0;
	boardConfiguration->injectionPins[2] = GPIOA_1;
	boardConfiguration->injectionPins[3] = GPIOB_10;
	/* 5th injector */
	//boardConfiguration->injectionPins[4] = GPIOD_15;

	/* Ignition */
	boardConfiguration->ignitionPins[0] = GPIOB_8;
	boardConfiguration->ignitionPins[1] = GPIOB_9;
	boardConfiguration->ignitionPins[2] = GPIOF_8;
	boardConfiguration->ignitionPins[3] = GPIOF_9;
	//boardConfiguration->ignitionPinMode = OM_INVERTED;

	//?
	//engineConfiguration->vbattDividerCoeff = ((float) (2 + 10)) / 2;
	//engineConfiguration->clt.config.bias_resistor = 2700;
	//sengineConfiguration->iat.config.bias_resistor = 2700;

	//?
	//boardConfiguration->useStepperIdle = true;
	//boardConfiguration->idle.stepperDirectionPin = GPIOB_12;
	//boardConfiguration->idle.stepperStepPin = GPIOB_13;
	//engineConfiguration->stepperEnablePin = GPIO_UNASSIGNED;
	//engineConfiguration->stepperEnablePinMode = OM_DEFAULT;

	engineConfiguration->communicationLedPin = GPIOG_6;	/* LD2 - green */
	engineConfiguration->runningLedPin = GPIO_UNASSIGNED;
	engineConfiguration->warninigLedPin = GPIOG_8; 	/* LD3 - yellow*/
	engineConfiguration->fatalErrorPin = LED_ERROR_BRAIN_PIN;	/* LD1 - red */
	engineConfiguration->configResetPin = GPIO_UNASSIGNED;

	/* IF you have BOTH camshaft position sensor and crankshaft position sensor
	 * camshaft is always trigger#1 input and then crankshaft is trigger#2. */
	boardConfiguration->triggerInputPins[0] = GPIOC_8;
	boardConfiguration->triggerInputPins[1] = GPIOC_9;
	boardConfiguration->triggerInputPins[2] = GPIO_UNASSIGNED;
	engineConfiguration->camInput = GPIO_UNASSIGNED;

	/* spi driven - TLE6240 - OUT8, also direct driven by GPIOG_2 */
	boardConfiguration->tachOutputPin = TLE6240_PIN(0);
	boardConfiguration->tachOutputPinMode = OM_DEFAULT;
	/* spi driven - TLE6240 - OUT5 */
	boardConfiguration->fuelPumpPin = TLE6240_PIN(5);
	boardConfiguration->fuelPumpPinMode = OM_DEFAULT;
	/* self shutdownd? */
	boardConfiguration->mainRelayPin = GPIOG_4;
	boardConfiguration->mainRelayPinMode = OM_DEFAULT;
	/* spi driven - TLE6240 - OUT3, OUT4
	 * TODO: second fan */
	boardConfiguration->fanPin = TLE6240_PIN(2);
	boardConfiguration->fanPinMode = OM_DEFAULT;
	/* spi driven - TLE6240 - OUT8 */
	boardConfiguration->malfunctionIndicatorPin = TLE6240_PIN(7);
	boardConfiguration->malfunctionIndicatorPinMode = OM_DEFAULT;

	// starter block
	/* Starter signal connected through MC33972 - SG11 */
	//setFsio(0, (GPIOB_1), STARTER_BLOCK PASS_ENGINE_PARAMETER_SUFFIX);


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
	boardConfiguration->vehicleSpeedSensorInputPin = GPIO_UNASSIGNED;
	boardConfiguration->boardTestModeJumperPin = GPIO_UNASSIGNED;
	boardConfiguration->acRelayPin = GPIO_UNASSIGNED;
	boardConfiguration->digitalPotentiometerSpiDevice = SPI_NONE;
	boardConfiguration->max31855spiDevice = SPI_NONE;

	/////////////////////////////////////////////////////////

	boardConfiguration->is_enabled_spi_1 = true;
	boardConfiguration->is_enabled_spi_2 = false;
	boardConfiguration->is_enabled_spi_3 = true;

	boardConfiguration->spi1mosiPin = GPIO_UNASSIGNED;
	engineConfiguration->spi1MosiMode = PO_DEFAULT;
	boardConfiguration->spi1misoPin = GPIO_UNASSIGNED;
	engineConfiguration->spi1MisoMode = PO_DEFAULT;
	boardConfiguration->spi1sckPin = GPIO_UNASSIGNED;
	engineConfiguration->spi1SckMode = PO_DEFAULT;

	boardConfiguration->spi3mosiPin = GPIOC_12;
	engineConfiguration->spi3MosiMode = PO_DEFAULT;
	boardConfiguration->spi3misoPin = GPIOC_11;
	engineConfiguration->spi3MisoMode = PO_DEFAULT;
	boardConfiguration->spi3sckPin = GPIOC_10;
	engineConfiguration->spi3SckMode = PO_DEFAULT;

	engineConfiguration->hip9011SpiDevice = SPI_DEVICE_4;
	boardConfiguration->hip9011CsPin = GPIOF_1;
	boardConfiguration->hip9011CsPinMode = OM_OPENDRAIN;
	boardConfiguration->hip9011IntHoldPin = GPIOC_15;
	boardConfiguration->hip9011IntHoldPinMode = OM_OPENDRAIN;
	engineConfiguration->hipOutputChannel = EFI_ADC_7; // PA7
	boardConfiguration->isHip9011Enabled = true;

#if 0
	engineConfiguration->cj125SpiDevice = SPI_DEVICE_3;
	engineConfiguration->cj125ua = EFI_ADC_9;
	engineConfiguration->cj125ur = EFI_ADC_12;
	boardConfiguration->cj125CsPin = GPIOA_15;
	engineConfiguration->cj125CsPinMode = OM_OPENDRAIN;
	boardConfiguration->wboHeaterPin = GPIOC_13;
	boardConfiguration->o2heaterPin = GPIOC_13;
#endif
	boardConfiguration->isCJ125Enabled = false;

	boardConfiguration->canDeviceMode = CD_USE_CAN1;
	boardConfiguration->canTxPin = GPIOD_0;
	boardConfiguration->canRxPin = GPIOD_1;

	//!!!!!!!!!!!!!!!
#if 1
	setSubaruEJ20GDefaults(PASS_ENGINE_PARAMETER_SIGNATURE);
#endif
	/* actually i2c extension connector */
	boardConfiguration->triggerSimulatorPins[0] = GPIOF_14;
	boardConfiguration->triggerSimulatorPins[1] = GPIOF_15;
	boardConfiguration->triggerSimulatorPins[2] = GPIO_UNASSIGNED;
	boardConfiguration->triggerSimulatorPinModes[0] = OM_DEFAULT;
	boardConfiguration->triggerSimulatorPinModes[1] = OM_DEFAULT;
	boardConfiguration->triggerSimulatorPinModes[2] = OM_DEFAULT;

	boardConfiguration->logicAnalyzerPins[0] = GPIO_UNASSIGNED;
	boardConfiguration->logicAnalyzerPins[1] = GPIO_UNASSIGNED;
	boardConfiguration->logicAnalyzerPins[2] = GPIO_UNASSIGNED;
	boardConfiguration->logicAnalyzerPins[3] = GPIO_UNASSIGNED;
	boardConfiguration->logicAnalyzerMode[0] = OM_DEFAULT;
	boardConfiguration->logicAnalyzerMode[1] = OM_DEFAULT;
	boardConfiguration->logicAnalyzerMode[2] = OM_DEFAULT;
	boardConfiguration->logicAnalyzerMode[3] = OM_DEFAULT;
	//!!!!!!!!!!!!!!!!!!!
	//engineConfiguration->silentTriggerError = true;

	//!!!!!!!!!!!!!
	//engineConfiguration->isEngineChartEnabled = false;
}
