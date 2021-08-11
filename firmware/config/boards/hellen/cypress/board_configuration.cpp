/**
 * @file boards/hellen/cypress/board_configuration.h
 *
 * @brief In this file we can override engine_configuration.cpp.
 *
 * @date Jan 27, 2020
 * @author andreika <prometheus.pcb@gmail.com>
 */

#include "pch.h"
#include "trigger_input.h"

void setBoardDefaultConfiguration(void) {
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);

	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;
	engineConfiguration->invertPrimaryTriggerSignal = true;
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->useNoiselessTriggerDecoder = true;

	engineConfiguration->isEngineChartEnabled = false;

	setAlgorithm(LM_SPEED_DENSITY PASS_CONFIG_PARAMETER_SUFFIX);

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;

	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SIMULTANEOUS;

	engineConfiguration->globalTriggerAngleOffset = 114;	// the end of 19th tooth?

	engineConfiguration->specs.displacement = 1.645;
	engineConfiguration->injector.flow = 200;
	
	engineConfiguration->cranking.baseFuel = 25;		// ???
	engineConfiguration->cranking.rpm = 600;

	engineConfiguration->rpmHardLimit = 3000; // yes, 3k. let's play it safe for now
	
	engineConfiguration->map.sensor.type = MT_MPX4250A;

	engineConfiguration->idleStepperReactionTime = 10;
	engineConfiguration->stepperDirectionPinMode = OM_INVERTED;
	engineConfiguration->idle.stepperDirectionPin = GPIO_UNASSIGNED;
	engineConfiguration->idle.stepperStepPin = GPIO_UNASSIGNED;
	engineConfiguration->stepperEnablePin = GPIO_UNASSIGNED;

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

	engineConfiguration->mapMinBufferLength = 4;

	engineConfiguration->communicationLedPin = GPIO_UNASSIGNED;//GPIOJ_0;
	engineConfiguration->runningLedPin = GPIO_UNASSIGNED;
	engineConfiguration->warningLedPin = GPIO_UNASSIGNED;
	engineConfiguration->triggerErrorPin = GPIO_UNASSIGNED;
	
	//engineConfiguration->checkEngineLedPin = GPIO_UNASSIGNED;
	//engineConfiguration->errorLedPin = GPIOJ_15;
	//engineConfiguration->fatalErrorPin = GPIOJ_15;

	//!!!!!!!!!
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_13;
	engineConfiguration->clt.adcChannel = EFI_ADC_26;
	engineConfiguration->iat.adcChannel = EFI_ADC_27;
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_3;
	engineConfiguration->afr.hwChannel = EFI_ADC_4;
	engineConfiguration->vbattAdcChannel = EFI_ADC_2;

#if 0
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_NONE;
	engineConfiguration->vbattAdcChannel = EFI_ADC_NONE;
	engineConfiguration->clt.adcChannel = EFI_ADC_NONE;
	engineConfiguration->iat.adcChannel = EFI_ADC_NONE;
	engineConfiguration->afr.hwChannel = EFI_ADC_NONE;
#endif

	engineConfiguration->auxFastSensor1_adcChannel = EFI_ADC_NONE;
	engineConfiguration->tps1_2AdcChannel = EFI_ADC_NONE;
	engineConfiguration->tps2_2AdcChannel = EFI_ADC_NONE;
	engineConfiguration->throttlePedalPositionSecondAdcChannel = EFI_ADC_NONE;
	
	engineConfiguration->mafAdcChannel = EFI_ADC_NONE;
	engineConfiguration->hipOutputChannel = EFI_ADC_NONE;
	engineConfiguration->fuelLevelSensor = EFI_ADC_NONE;
	engineConfiguration->oilPressure.hwChannel = EFI_ADC_NONE;

	engineConfiguration->acSwitch = GPIO_UNASSIGNED;
	engineConfiguration->triggerInputPins[0] = GPIOB_0;
	engineConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->triggerInputPins[2] = GPIO_UNASSIGNED;

#if 0	

	// todo:
	int i;
	for (i = 0; i < MAX_CYLINDER_COUNT; i++)
		engineConfiguration->injectionPins[i] = GPIO_UNASSIGNED;
	for (i = 0; i < MAX_CYLINDER_COUNT; i++)
		engineConfiguration->ignitionPins[i] = GPIO_UNASSIGNED;
	
	engineConfiguration->adcVcc = 5.0f;
	engineConfiguration->analogInputDividerCoefficient = 1;

	// we call it here because setDefaultBoardConfiguration() is not called for DEFAULT_ENGINE_TYPE=MINIMAL_PINS
	setSerialConfigurationOverrides();
#endif

	//!!!!!!!!!!!!!!!!!!!
	//engineConfiguration->isFastAdcEnabled = false;
}

void setPinConfigurationOverrides(void) {
}

void setSerialConfigurationOverrides(void) {
#if 0
	engineConfiguration->useSerialPort = true;
	engineConfiguration->binarySerialTxPin = GPIOC_7;
	engineConfiguration->binarySerialRxPin = GPIOC_6;
	engineConfiguration->consoleSerialTxPin = GPIOA_10;
	engineConfiguration->consoleSerialRxPin = GPIOA_11;
	engineConfiguration->tunerStudioSerialSpeed = SERIAL_SPEED;
	engineConfiguration->uartConsoleSerialSpeed = SERIAL_SPEED;
#endif
}

void setSdCardConfigurationOverrides(void) {
}

void setAdcChannelOverrides(void) {
	addAdcChannelForTrigger();
}
