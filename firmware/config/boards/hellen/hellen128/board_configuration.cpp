/**
 * @file boards/hellen/hellen128/board_configuration.cpp
 *
 *
 * @brief Configuration defaults for the hellen128 board
 *
 * See https://rusefi.com/s/hellen128
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
	engineConfiguration->injectionPins[0] = H176_LS_1;
	engineConfiguration->injectionPins[1] = H176_LS_2;
	engineConfiguration->injectionPins[2] = H176_LS_3;
	engineConfiguration->injectionPins[3] = H176_LS_4;
	engineConfiguration->injectionPins[4] = H176_LS_5;
	engineConfiguration->injectionPins[5] = H176_LS_6;
	engineConfiguration->injectionPins[6] = H176_LS_7;
	engineConfiguration->injectionPins[7] = H176_LS_8;

	// Disable remainder
	for (int i = 8; i < MAX_CYLINDER_COUNT;i++) {
		engineConfiguration->injectionPins[i] = GPIO_UNASSIGNED;
	}

	engineConfiguration->injectionPinMode = OM_DEFAULT;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = GPIOI_8;
	engineConfiguration->ignitionPins[1] = GPIOE_5;
	engineConfiguration->ignitionPins[2] = GPIOE_4;
	engineConfiguration->ignitionPins[3] = GPIOE_3;
	engineConfiguration->ignitionPins[4] = GPIOE_2;
	engineConfiguration->ignitionPins[5] = GPIOI_5;
	engineConfiguration->ignitionPins[6] = GPIOI_6;
	engineConfiguration->ignitionPins[7] = GPIOI_7;
	
	// disable remainder
	for (int i = 8; i < MAX_CYLINDER_COUNT; i++) {
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

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_4;
	engineConfiguration->tps1_2AdcChannel = EFI_ADC_8;
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
	setHellen176LedPins();
	setupVbatt();
	setSdCardConfigurationOverrides();

    // this specific Hellen has less common pull-up value R49
	engineConfiguration->clt.config.bias_resistor = 2700;
	engineConfiguration->iat.config.bias_resistor = 2700;

	engineConfiguration->canTxPin = H176_CAN_TX;
	engineConfiguration->canRxPin = H176_CAN_RX;
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

	engineConfiguration->enableSoftwareKnock = true;

	engineConfiguration->fuelPumpPin = GPIOD_15;
	engineConfiguration->idle.solenoidPin = GPIO_UNASSIGNED;
	engineConfiguration->fanPin = GPIOD_12;	// OUT_PWM8
	engineConfiguration->mainRelayPin = GPIO_UNASSIGNED;

	engineConfiguration->starterControlPin = H176_OUT_IO10;
	engineConfiguration->startStopButtonPin = H176_IN_A16;
	engineConfiguration->startStopButtonMode = PI_PULLDOWN;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	// Some sensible defaults for other options
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;
	setAlgorithm(LM_SPEED_DENSITY);

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;
	engineConfiguration->specs.displacement = 2.295f;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK
	engineConfiguration->crankingInjectionMode = IM_SEQUENTIAL;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;//IM_BATCH;// IM_SEQUENTIAL;

	//Set default ETB config
	engineConfiguration->etbIo[0].directionPin1 = H176_OUT_PWM2;
	engineConfiguration->etbIo[0].directionPin2 = H176_OUT_PWM3;
	engineConfiguration->etbIo[0].controlPin = H176_OUT_PWM1; // ETB_EN
	engineConfiguration->etb_use_two_wires = true;

	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_MERCEDES);
	strcpy(engineConfiguration->engineCode, "");

	/**
	 * Jimmy best tune
	 * https://rusefi.com/online/view.php?msq=626
	 * md_sanci latest tune
	 * https://rusefi.com/online/view.php?msq=630
	 */
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_3;
	engineConfiguration->throttlePedalPositionSecondAdcChannel = EFI_ADC_14;
	engineConfiguration->throttlePedalUpVoltage = 1.49;
	engineConfiguration->throttlePedalWOTVoltage = 4.72;
	engineConfiguration->throttlePedalSecondaryUpVoltage = 1.34;
	engineConfiguration->throttlePedalSecondaryWOTVoltage = 4.24;


	hellenWbo();
}

/**
 * @brief   Board-specific SD card configuration code overrides. Needed by bootloader code.
 * @todo    Add your board-specific code, if any.
 */
void setSdCardConfigurationOverrides(void) {
	engineConfiguration->sdCardSpiDevice = SPI_DEVICE_2;

	engineConfiguration->spi2mosiPin = H_SPI2_MOSI;
	engineConfiguration->spi2misoPin = H_SPI2_MISO;
	engineConfiguration->spi2sckPin = H_SPI2_SCK;
	engineConfiguration->sdCardCsPin = H_SPI2_CS;
	engineConfiguration->is_enabled_spi_2 = true;
}
