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
#include "../hellen_meta.h"

static void hellenWbo() {
	engineConfiguration->enableAemXSeries = true;
}

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
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = GPIOC_13;
	engineConfiguration->ignitionPins[1] = GPIOE_5;
	engineConfiguration->ignitionPins[2] = GPIOE_4;
	engineConfiguration->ignitionPins[3] = GPIOE_3;
	
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
	// trigger inputs
	engineConfiguration->triggerInputPins[0] = H144_IN_CRANK;
	engineConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->triggerInputPins[2] = GPIO_UNASSIGNED;
	// Direct hall-only cam input
	engineConfiguration->camInputs[0] = H144_IN_CAM;
	engineConfiguration->camInputs[1 * CAMS_PER_BANK] = H144_IN_D_AUX4;

    engineConfiguration->vehicleSpeedSensorInputPin = H144_IN_VSS;

	engineConfiguration->tps1_1AdcChannel = H144_IN_TPS;
	engineConfiguration->tps1_2AdcChannel = H144_IN_AUX1;

	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_3;
	engineConfiguration->throttlePedalPositionSecondAdcChannel = EFI_ADC_14;
	engineConfiguration->mafAdcChannel = EFI_ADC_NONE;
	engineConfiguration->map.sensor.hwChannel = H144_IN_MAP1;

	engineConfiguration->afr.hwChannel = EFI_ADC_NONE;

	engineConfiguration->clt.adcChannel = H144_IN_CLT;

	engineConfiguration->iat.adcChannel = H144_IN_IAT;

//	engineConfiguration->auxTempSensor1.adcChannel = H144_IN_O2S2;
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

	engineConfiguration->canTxPin = H176_CAN_TX;
	engineConfiguration->canRxPin = H176_CAN_RX;

//	engineConfiguration->fuelPumpPin = GPIOG_2;	// OUT_IO9
//	engineConfiguration->idle.solenoidPin = GPIOD_14;	// OUT_PWM5
//	engineConfiguration->fanPin = GPIOD_12;	// OUT_PWM8
	engineConfiguration->mainRelayPin = GPIOG_14;	// pin: 111a, OUT_IO3
	engineConfiguration->malfunctionIndicatorPin = H144_OUT_PWM8;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	engineConfiguration->etbIo[0].directionPin1 = H144_OUT_PWM2;
	engineConfiguration->etbIo[0].directionPin2 = H144_OUT_PWM3;
	engineConfiguration->etbIo[0].controlPin = H144_OUT_IO12;
	CONFIG(etb_use_two_wires) = true;

	// wastegate DC motor
	engineConfiguration->etbIo[1].directionPin1 = H144_OUT_PWM4;
	engineConfiguration->etbIo[1].directionPin2 = H144_OUT_PWM5;
	engineConfiguration->etbIo[1].controlPin = H144_OUT_IO13;
	CONFIG(etb_use_two_wires) = true;
	CONFIG(etbFunctions)[1] = ETB_Wastegate;

	// Some sensible defaults for other options
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);

	engineConfiguration->vvtCamSensorUseRise = true;
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;
	setAlgorithm(LM_SPEED_DENSITY PASS_CONFIG_PARAMETER_SUFFIX);


	engineConfiguration->injectorCompensationMode = ICM_FixedRailPressure;

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;
	engineConfiguration->specs.displacement = 1.998;
	strcpy(CONFIG(engineMake), ENGINE_MAKE_Hyundai);
	strcpy(CONFIG(engineCode), "Theta II");

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
