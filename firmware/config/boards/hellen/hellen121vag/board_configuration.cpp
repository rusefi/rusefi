/**
 * @file boards/hellen/hellen121vag/board_configuration.cpp
 *
 *
 * @brief Configuration defaults for the hellen121vag board
 *
 * See https://rusefi.com/s/hellen121vag
 *
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "fsio_impl.h"

static void hellenWbo() {
	engineConfiguration->enableAemXSeries = true;
}

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = GPIOG_7; // 96 - INJ_1
	engineConfiguration->injectionPins[1] = GPIOG_8;
	engineConfiguration->injectionPins[2] = GPIOD_11; // 97 - INJ_3
	engineConfiguration->injectionPins[3] = GPIOD_10;

	//engineConfiguration->injectionPins[4] = GPIOD_9;
	//engineConfiguration->injectionPins[5] = GPIOF_12;
	//engineConfiguration->injectionPins[6] = GPIOF_13;
	//engineConfiguration->injectionPins[7] = GPIOF_14;

	// Disable remainder
	for (int i = 4; i < MAX_CYLINDER_COUNT;i++) {
		engineConfiguration->injectionPins[i] = GPIO_UNASSIGNED;
	}

	engineConfiguration->injectionPinMode = OM_DEFAULT;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = GPIOI_8; // 102 - IGN_1
	engineConfiguration->ignitionPins[1] = GPIOE_5 ; // 7 - IGN_2
	engineConfiguration->ignitionPins[2] = GPIOE_4; // 111 - IGN_3
	engineConfiguration->ignitionPins[3] = GPIOE_3; // 94 - IGN_4

	//engineConfiguration->ignitionPins[4] = GPIOE_2;
	//engineConfiguration->ignitionPins[5] = GPIOI_5;
	//engineConfiguration->ignitionPins[6] = GPIOI_6;
	//engineConfiguration->ignitionPins[7] = GPIOI_7;
	
	// disable remainder
	for (int i = 4; i < MAX_CYLINDER_COUNT; i++) {
		engineConfiguration->ignitionPins[i] = GPIO_UNASSIGNED;
	}

	engineConfiguration->ignitionPinMode = OM_DEFAULT;
}

static void setLedPins() {
#ifdef EFI_COMMUNICATION_PIN
	engineConfiguration->communicationLedPin = EFI_COMMUNICATION_PIN;
#else
	engineConfiguration->communicationLedPin = GPIOH_10;
#endif /* EFI_COMMUNICATION_PIN */
	engineConfiguration->runningLedPin = GPIOH_9;  // green
	engineConfiguration->warningLedPin = GPIOH_11; // yellow
}

static void setupVbatt() {
	// 4.7k high side/4.7k low side = 2.0 ratio divider
	engineConfiguration->analogInputDividerCoefficient = 2.0f;

	// set vbatt_divider 5.835
	// 33k / 6.8k
	engineConfiguration->vbattDividerCoeff = (33 + 6.8) / 6.8; // 5.835

	// pin input +12 from Main Relay
	engineConfiguration->vbattAdcChannel = EFI_ADC_5; // 4T

	engineConfiguration->adcVcc = 3.29f;
}

static void setupDefaultSensorInputs() {
	// trigger inputs
	engineConfiguration->triggerInputPins[0] = GPIOB_1; // 82 - VR
	engineConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->triggerInputPins[2] = GPIO_UNASSIGNED;
	// Direct hall-only cam input
	engineConfiguration->camInputs[0] = GPIOA_6; // 86 - CAM1

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_4; // 92 - TPS 1
	engineConfiguration->tps2_1AdcChannel = EFI_ADC_NONE;

	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_3; // 34 In PPS1
	engineConfiguration->throttlePedalPositionSecondAdcChannel = EFI_ADC_14; // 35 In PPS2

	CONFIG(throttlePedalUpVoltage) = 0.4;
	CONFIG(throttlePedalWOTVoltage) = 2;
	CONFIG(throttlePedalSecondaryUpVoltage) = 0.7;
	CONFIG(throttlePedalSecondaryWOTVoltage) = 4.1;

	engineConfiguration->mafAdcChannel = EFI_ADC_10;
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_11;

	engineConfiguration->afr.hwChannel = EFI_ADC_1;

	engineConfiguration->clt.adcChannel = EFI_ADC_12;

	engineConfiguration->iat.adcChannel = EFI_ADC_13;

	engineConfiguration->auxTempSensor1.adcChannel = EFI_ADC_NONE;
	engineConfiguration->auxTempSensor2.adcChannel = EFI_ADC_NONE;
}

void setBoardConfigOverrides(void) {
	setLedPins();
	setupVbatt();
	setSdCardConfigurationOverrides();

	engineConfiguration->clt.config.bias_resistor = 4700;
	engineConfiguration->iat.config.bias_resistor = 4700;

	engineConfiguration->canTxPin = GPIOD_1;
	engineConfiguration->canRxPin = GPIOD_0;
}

void setPinConfigurationOverrides(void) {
}

void setSerialConfigurationOverrides(void) {
	engineConfiguration->useSerialPort = false;
	engineConfiguration->binarySerialTxPin = GPIO_UNASSIGNED;
	engineConfiguration->binarySerialRxPin = GPIO_UNASSIGNED;
//	engineConfiguration->consoleSerialTxPin = GPIO_UNASSIGNED;
//	engineConfiguration->consoleSerialRxPin = GPIO_UNASSIGNED;
}


/**
 * @brief   Board-specific configuration defaults.
 *
 * See also setDefaultEngineConfiguration
 *
 * @todo    Add your board-specific code, if any.
 */
void setBoardDefaultConfiguration(void) {
	setInjectorPins();
	setIgnitionPins();

	engineConfiguration->etbIo[0].directionPin1 = GPIOC_6; // out_pwm2
	engineConfiguration->etbIo[0].directionPin2 = GPIOC_7; // out_pwm3
	engineConfiguration->etbIo[0].controlPin = GPIOA_8; // ETB_EN out_io12
	CONFIG(etb_use_two_wires) = true;

	engineConfiguration->isSdCardEnabled = true;

	// todo: should this be a global default not just Hellen121?
	engineConfiguration->boostCutPressure = 200;

	engineConfiguration->vvtMode[0] = VVT_BOSCH_QUICK_START;
	engineConfiguration->map.sensor.type = MT_BOSCH_2_5;

	CONFIG(enableSoftwareKnock) = true;

	engineConfiguration->fuelPumpPin = GPIOH_14;	// 65 - Fuel Pump
	engineConfiguration->malfunctionIndicatorPin = GPIOG_4; // 47 - CEL
	engineConfiguration->tachOutputPin = GPIOD_13; // 37 - TACH
	engineConfiguration->idle.solenoidPin = GPIOD_14;	// OUT_PWM5
	engineConfiguration->fanPin = GPIOD_12;	// OUT_PWM8
	engineConfiguration->mainRelayPin = GPIOI_2;	// 21 - Main Relay

//	engineConfiguration->injectorCompensationMode
	engineConfiguration->fuelReferencePressure = 300;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	// Some sensible defaults for other options
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;
	setAlgorithm(LM_SPEED_DENSITY PASS_CONFIG_PARAMETER_SUFFIX);

	strcpy(CONFIG(engineMake), ENGINE_MAKE_VAG);
	strcpy(CONFIG(engineCode), "base");

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SIMULTANEOUS;//IM_BATCH;// IM_SEQUENTIAL;

	hellenWbo();
}

/**
 * @brief   Board-specific SD card configuration code overrides. Needed by bootloader code.
 * @todo    Add your board-specific code, if any.
 */
void setSdCardConfigurationOverrides(void) {
	engineConfiguration->sdCardSpiDevice = SPI_DEVICE_3;

	engineConfiguration->spi3mosiPin = GPIOC_12;
	engineConfiguration->spi3misoPin = GPIOC_11;
	engineConfiguration->spi3sckPin = GPIOC_10;
	engineConfiguration->sdCardCsPin = GPIOA_15;

//	engineConfiguration->spi2mosiPin = GPIOB_15;
//	engineConfiguration->spi2misoPin = GPIOB_14;
//	engineConfiguration->spi2sckPin = GPIOB_13;
//	engineConfiguration->sdCardCsPin = GPIOB_12;
	CONFIG(is_enabled_spi_3) = true;
}
