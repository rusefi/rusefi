/**
 * @file boards/hellen/hellen121nissan/board_configuration.cpp
 *
 *
 * @brief Configuration defaults for the hellen121nissan board
 *
 * See https://rusefi.com/s/hellen121nissan
 *
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "fsio_impl.h"
#include "custom_engine.h"

static void hellenWbo() {
	engineConfiguration->enableAemXSeries = true;
}

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = GPIOG_7;
	engineConfiguration->injectionPins[1] = GPIOG_8;
	engineConfiguration->injectionPins[2] = GPIOD_11;
	engineConfiguration->injectionPins[3] = GPIOD_10;
	engineConfiguration->injectionPins[4] = GPIOD_9;
	engineConfiguration->injectionPins[5] = GPIOF_12;


	// Disable remainder
	for (int i = 6; i < MAX_CYLINDER_COUNT;i++) {
		engineConfiguration->injectionPins[i] = GPIO_UNASSIGNED;
	}

	engineConfiguration->injectionPinMode = OM_DEFAULT;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = GPIOC_13;
	engineConfiguration->ignitionPins[1] = GPIOE_5;
	engineConfiguration->ignitionPins[2] = GPIOE_4;
	engineConfiguration->ignitionPins[3] = GPIOE_3;
	engineConfiguration->ignitionPins[4] = GPIOE_2;
	engineConfiguration->ignitionPins[5] = GPIOB_8;
	
	// disable remainder
	for (int i = 6; i < MAX_CYLINDER_COUNT; i++) {
		engineConfiguration->ignitionPins[i] = GPIO_UNASSIGNED;
	}

	engineConfiguration->ignitionPinMode = OM_DEFAULT;
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
	engineConfiguration->triggerInputPins[0] = GPIOB_1;
	engineConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->triggerInputPins[2] = GPIO_UNASSIGNED;
	// Direct hall-only cam input
	engineConfiguration->camInputs[0] = GPIOA_6;
	// todo: remove from default since 4 cylinder does not use it
	// todo: this requires unit test change
	engineConfiguration->camInputs[1 * CAMS_PER_BANK] = GPIOA_7;

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_4;
	engineConfiguration->tps1_2AdcChannel = EFI_ADC_8;

	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_3;
	engineConfiguration->throttlePedalPositionSecondAdcChannel = EFI_ADC_14;
	engineConfiguration->mafAdcChannel = EFI_ADC_10;
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_11;

	engineConfiguration->afr.hwChannel = EFI_ADC_1;

	engineConfiguration->clt.adcChannel = EFI_ADC_12;

	engineConfiguration->iat.adcChannel = EFI_ADC_13;

	engineConfiguration->auxTempSensor1.adcChannel = EFI_ADC_NONE;
	engineConfiguration->auxTempSensor2.adcChannel = EFI_ADC_NONE;
}

void setBoardConfigOverrides(void) {
	setHellen144LedPins();
	setupVbatt();
	setSdCardConfigurationOverrides();

	engineConfiguration->clt.config.bias_resistor = 4700;
	engineConfiguration->iat.config.bias_resistor = 4700;
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

	engineConfiguration->displayLogicLevelsInEngineSniffer = true;
	engineConfiguration->isSdCardEnabled = true;

	CONFIG(enableSoftwareKnock) = true;
	CONFIG(canNbcType) = CAN_BUS_NISSAN_VQ;

	engineConfiguration->canTxPin = GPIOD_1;
	engineConfiguration->canRxPin = GPIOD_0;

	engineConfiguration->fuelPumpPin = GPIOD_12;	// OUT_IO9 // 113 Fuel Pump Relay
	engineConfiguration->idle.solenoidPin = GPIO_UNASSIGNED;
//	engineConfiguration->fanPin = GPIOD_12;	// OUT_PWM8
	engineConfiguration->mainRelayPin = GPIOG_14;	// pin: 111a, OUT_IO3

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	engineConfiguration->etbIo[0].directionPin1 = GPIOD_15; // out_pwm7
	engineConfiguration->etbIo[0].directionPin2 = GPIOD_14; // out_pwm6
	engineConfiguration->etbIo[0].controlPin = GPIOD_13; // ETB_EN out_pwm1
	CONFIG(etb_use_two_wires) = true;

	// Some sensible defaults for other options
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);

	engineConfiguration->vvtCamSensorUseRise = true;
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;
	setAlgorithm(LM_SPEED_DENSITY PASS_CONFIG_PARAMETER_SUFFIX);


	// Bosch VQ40 VR56 VK56 0280158007
	engineConfiguration->injector.flow = 296.2;

	strcpy(CONFIG(engineMake), ENGINE_MAKE_NISSAN);

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SIMULTANEOUS;//IM_BATCH;// IM_SEQUENTIAL;

	engineConfiguration->luaOutputPins[0] = GPIOG_5; // 104 ETB Relay

	engineConfiguration->throttlePedalUpVoltage = 0.75;
	engineConfiguration->throttlePedalWOTVoltage = 4.45;
	engineConfiguration->throttlePedalSecondaryUpVoltage = 0.43;
	engineConfiguration->throttlePedalSecondaryWOTVoltage = 2.20;

	engineConfiguration->tpsMin = 100;
	engineConfiguration->tpsMax = 889;

	engineConfiguration->tps1SecondaryMin = 891;
	engineConfiguration->tps1SecondaryMax = 102;
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
