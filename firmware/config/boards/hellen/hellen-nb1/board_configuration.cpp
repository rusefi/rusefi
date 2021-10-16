/**
 * @file boards/hellen/hellen-nb1/board_configuration.cpp
 *
 *
 * @brief Configuration defaults for the hellen-nb1 board
 *
 * See http://rusefi.com/s/hellenNB1
 *
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "fsio_impl.h"
#include "custom_engine.h"
#include "../hellen_meta.h"

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = H144_LS_1;
	engineConfiguration->injectionPins[1] = H144_LS_2;
	engineConfiguration->injectionPins[2] = H144_LS_3;
	engineConfiguration->injectionPins[3] = H144_LS_4;

	// Disable remainder
	for (int i = 4; i < MAX_CYLINDER_COUNT;i++) {
		engineConfiguration->injectionPins[i] = GPIO_UNASSIGNED;
	}

	engineConfiguration->injectionPinMode = OM_DEFAULT;

	engineConfiguration->clutchDownPin = GPIOC_4; // Clutch switch input
	engineConfiguration->clutchDownPinMode = PI_PULLDOWN;
	engineConfiguration->launchActivationMode = CLUTCH_INPUT_LAUNCH;
	engineConfiguration->malfunctionIndicatorPin = H144_OUT_IO8;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = H144_IGN_1;
	engineConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPins[2] = H144_IGN_2;
	engineConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;

	// disable remainder
	for (int i = 4; i < MAX_CYLINDER_COUNT; i++) {
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
	// trigger inputs, hall
	engineConfiguration->triggerInputPins[0] = GPIOA_6;
	engineConfiguration->triggerInputPins[1] = GPIOB_1;
	engineConfiguration->triggerInputPins[2] = GPIO_UNASSIGNED;
	engineConfiguration->camInputs[0] = GPIO_UNASSIGNED;

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_4;
	engineConfiguration->tps2_1AdcChannel = EFI_ADC_NONE;

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

	CONFIG(enableSoftwareKnock) = true;

	engineConfiguration->boostControlPin = H144_LS_6;
	engineConfiguration->acSwitch = H144_IN_D_AUX3;
	engineConfiguration->acRelayPin = H144_OUT_IO6;
	engineConfiguration->fuelPumpPin = GPIOG_2;	// OUT_IO9
	engineConfiguration->idle.solenoidPin = GPIOD_14;	// OUT_PWM5
	engineConfiguration->fanPin = GPIOD_12;	// OUT_PWM8
	engineConfiguration->mainRelayPin = GPIOI_2;	// OUT_LOW3
    engineConfiguration->tachOutputPin = H144_OUT_PWM1;
	engineConfiguration->alternatorControlPin = H144_OUT_PWM7;
	engineConfiguration->fan2Pin = H144_OUT_IO2;

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
	engineConfiguration->injectionMode = IM_SIMULTANEOUS;//IM_BATCH;// IM_SEQUENTIAL;

	engineConfiguration->clutchDownPin = H144_IN_D_2;
	engineConfiguration->clutchDownPinMode = PI_PULLDOWN;
	engineConfiguration->launchActivationMode = CLUTCH_INPUT_LAUNCH;
// ?	engineConfiguration->malfunctionIndicatorPin = GPIOG_4; //1E - Check Engine Light
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
