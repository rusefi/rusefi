/**
 * @file boards/kinetis/board_configuration.h
 *
 * @brief In this file we can override engine_configuration.cpp.
 *
 * @date Mar 22, 2019
 * @author andreika, (c) 2019
 */

#include "global.h"
#include "engine.h"
#include "engine_configuration.h"
#include "adc_inputs.h"
#include "engine_math.h"
#include "tps.h"

EXTERN_ENGINE;

#if 0
char __debugBuffer[80];
int __debugEnabled = 0;
#endif

void setBoardConfigurationOverrides(void) {
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;

	boardConfiguration->isFasterEngineSpinUpEnabled = true;
	boardConfiguration->useNoiselessTriggerDecoder = true;

	setAlgorithm(LM_SPEED_DENSITY PASS_CONFIG_PARAMETER_SUFFIX);

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;

	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SIMULTANEOUS;

	engineConfiguration->globalTriggerAngleOffset = 114;	// the end of 19th tooth?

	engineConfiguration->specs.displacement = 1.645;
	engineConfiguration->injector.flow = 200;
	
	engineConfiguration->cranking.baseFuel = 5;		// ???
	engineConfiguration->crankingChargeAngle = 70;
	engineConfiguration->cranking.rpm = 600;

	engineConfiguration->rpmHardLimit = 3000; // yes, 3k. let's play it safe for now
	
	engineConfiguration->map.sensor.type = MT_MPX4250A;

	engineConfiguration->idleStepperReactionTime = 10;
	engineConfiguration->stepperDirectionPinMode = OM_INVERTED;

	engineConfiguration->useLinearCltSensor = true;
	// todo:
	engineConfiguration->clt.config.resistance_1 = 0;
	engineConfiguration->clt.config.tempC_1 = -40.0f;
	engineConfiguration->clt.config.resistance_2 = 5.0f;
	engineConfiguration->clt.config.tempC_2 = 120.0f,
	engineConfiguration->clt.config.bias_resistor = 3300;
	
	//engineConfiguration->canNbcType = CAN_BUS_NBC_BMW;
	engineConfiguration->canNbcType = CAN_BUS_MAZDA_RX8;
	engineConfiguration->canReadEnabled = true;
	engineConfiguration->canWriteEnabled = false;

	engineConfiguration->tpsMin = convertVoltageTo10bitADC(0.250);
	engineConfiguration->tpsMax = convertVoltageTo10bitADC(4.538);
	engineConfiguration->tpsErrorDetectionTooLow = -10; // -10% open
	engineConfiguration->tpsErrorDetectionTooHigh = 110; // 110% open

	boardConfiguration->mapMinBufferLength = 4;

	// todo:
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_NONE;
	engineConfiguration->mafAdcChannel = EFI_ADC_NONE;
	engineConfiguration->hipOutputChannel = EFI_ADC_NONE;
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_NONE;
	engineConfiguration->fuelLevelSensor = EFI_ADC_NONE;
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_NONE;
	engineConfiguration->vbattAdcChannel = EFI_ADC_NONE;
	engineConfiguration->clt.adcChannel = EFI_ADC_NONE;
	engineConfiguration->iat.adcChannel = EFI_ADC_NONE;
	engineConfiguration->afr.hwChannel = EFI_ADC_NONE;
	engineConfiguration->oilPressure.hwChannel = EFI_ADC_NONE;
	engineConfiguration->acSwitchAdc = EFI_ADC_NONE;
	
	engineConfiguration->clt.adcChannel = EFI_ADC_14;

	boardConfiguration->triggerInputPins[0] = GPIOE_7;
	boardConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
	boardConfiguration->triggerInputPins[2] = GPIO_UNASSIGNED;

	engineConfiguration->tle6240spiDevice = SPI_DEVICE_1;
	engineConfiguration->tle6240_cs = GPIOB_0;
	
	// todo:
	int i;
	for (i = 0; i < INJECTION_PIN_COUNT; i++)
		boardConfiguration->injectionPins[i] = GPIO_UNASSIGNED;
	for (i = 0; i < IGNITION_PIN_COUNT; i++)
		boardConfiguration->ignitionPins[i] = GPIO_UNASSIGNED;
	
	engineConfiguration->adcVcc = 5.0f;
	engineConfiguration->analogInputDividerCoefficient = 1;

	//boardConfiguration->isFastAdcEnabled = false;
		
	// we call it here because setDefaultBoardConfiguration() is not called for DEFAULT_ENGINE_TYPE=MINIMAL_PINS
	setSerialConfigurationOverrides();
}

void setPinConfigurationOverrides(void) {
}

void setSerialConfigurationOverrides(void) {
	boardConfiguration->useSerialPort = true;
	engineConfiguration->binarySerialTxPin = GPIOC_7;
	engineConfiguration->binarySerialRxPin = GPIOC_6;
	engineConfiguration->consoleSerialTxPin = GPIOA_10;
	engineConfiguration->consoleSerialRxPin = GPIOA_11;
	boardConfiguration->tunerStudioSerialSpeed = SERIAL_SPEED;
	engineConfiguration->uartConsoleSerialSpeed = SERIAL_SPEED;
}

void setSdCardConfigurationOverrides(void) {
}

void setAdcChannelOverrides(void) {
	// on Kinetis, ADC_FAST & SLOW are not really "fast" or "slow", 
	// they are just different ADC numbers with different sets of channels
	removeChannel("VBatt", engineConfiguration->vbattAdcChannel);
	addChannel("VBatt", engineConfiguration->vbattAdcChannel, ADC_FAST);
	
	removeChannel("TPS", engineConfiguration->tps1_1AdcChannel);
	addChannel("TPS", engineConfiguration->tps1_1AdcChannel, ADC_SLOW);
}
