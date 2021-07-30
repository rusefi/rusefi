/**
 * @file boards/hellen/hellen81/board_configuration.cpp
 *
 *
 * @brief Configuration defaults for the Hellen81 board
 *
 * See https://rusefi.com/s/hellen81
 *
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "global.h"
#include "engine.h"
#include "engine_math.h"
#include "allsensors.h"
#include "fsio_impl.h"
#include "engine_configuration.h"


static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = GPIOG_7;
	engineConfiguration->injectionPins[1] = GPIOG_8;
	engineConfiguration->injectionPins[2] = GPIOD_11;
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
	engineConfiguration->ignitionPins[0] = GPIOI_8;
	engineConfiguration->ignitionPins[1] = GPIOE_5;
	engineConfiguration->ignitionPins[2] = GPIOE_4;
	engineConfiguration->ignitionPins[3] = GPIOE_3;

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

	//!!!!!!!!!!!
	engineConfiguration->runningLedPin = GPIO_UNASSIGNED;
	//engineConfiguration->runningLedPin = GPIOH_9;  // green
	
	engineConfiguration->warningLedPin = GPIOH_11; // yellow
}

static void setupVbatt() {
	// 4.7k high side/4.7k low side = 2.0 ratio divider
	engineConfiguration->analogInputDividerCoefficient = 2.0f;

	// set vbatt_divider 5.835
	// 33k / 6.8k
	engineConfiguration->vbattDividerCoeff = (33 + 6.8) / 6.8; // 5.835

	// pin input +12 from Main Relay
	engineConfiguration->vbattAdcChannel = EFI_ADC_5;

	engineConfiguration->adcVcc = 3.29f;
}

static void setupDefaultSensorInputs() {
	// trigger inputs
	engineConfiguration->triggerInputPins[0] = GPIOB_1;
	engineConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->triggerInputPins[2] = GPIO_UNASSIGNED;
	// Direct hall-only cam input
	engineConfiguration->camInputs[0] = GPIOA_6;

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_4;
	engineConfiguration->tps2_1AdcChannel = EFI_ADC_NONE;

	engineConfiguration->mafAdcChannel = EFI_ADC_NONE;
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_10;

	engineConfiguration->afr.hwChannel = EFI_ADC_0;	// ADC1_16

	engineConfiguration->clt.adcChannel = EFI_ADC_12;	// ADC3_0

	engineConfiguration->iat.adcChannel = EFI_ADC_13;	// ADC3_1

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

	engineConfiguration->isSdCardEnabled = true;

	engineConfiguration->canTxPin = GPIOD_1;
	engineConfiguration->canRxPin = GPIOD_0;

	engineConfiguration->fuelPumpPin = GPIOG_2;	// OUT_IO9
	engineConfiguration->fanPin = GPIOD_12;	// OUT_PWM8
	engineConfiguration->mainRelayPin = GPIOI_2;	// OUT_LOW3
	engineConfiguration->tachOutputPin = GPIOD_14;	// OUT_PWM6

	engineConfiguration->useStepperIdle = true;
	engineConfiguration->useHbridges = true;
	engineConfiguration->stepperDcIo[0].directionPin1 = GPIOC_6;	// COIL_A1 = OUT_PWM2
	engineConfiguration->stepperDcIo[0].directionPin2 = GPIOC_7;	// COIL_A2 = OUT_PWM3
	engineConfiguration->stepperDcIo[1].directionPin1 = GPIOC_8;	// COIL_B1 = OUT_PWM4
	engineConfiguration->stepperDcIo[1].directionPin2 = GPIOC_9;	// COIL_B2 = OUT_PWM5

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	// Some sensible defaults for other options
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;
	setAlgorithm(LM_SPEED_DENSITY PASS_CONFIG_PARAMETER_SUFFIX);

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;	// IM_SIMULTANEOUS; //IM_BATCH;
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
