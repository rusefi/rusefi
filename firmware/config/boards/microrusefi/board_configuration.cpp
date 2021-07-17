/**
 * @file boards/microrusefi/board_configuration.cpp
 *
 *
 * @brief Configuration defaults for the microRusefi board
 *
 * MICRO_RUS_EFI
 * set engine_type 60
 *
 * MRE_BOARD_OLD_TEST
 * set engine_type 30
 *
 * MRE_BOARD_NEW_TEST
 * set engine_type 31
 *
 * See https://github.com/rusefi/rusefi/wiki/Hardware_microRusEfi_wiring
 *
 * @author Matthew Kennedy, (c) 2019
 */

#include "allsensors.h"
#include "fsio_impl.h"

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = TLE8888_PIN_1;
	engineConfiguration->injectionPins[1] = TLE8888_PIN_2;
	engineConfiguration->injectionPins[2] = TLE8888_PIN_3;
	engineConfiguration->injectionPins[3] = TLE8888_PIN_4;

	// Disable remainder
	for (int i = 4; i < MAX_CYLINDER_COUNT;i++) {
		engineConfiguration->injectionPins[i] = GPIO_UNASSIGNED;
	}

	engineConfiguration->injectionPinMode = OM_DEFAULT;
}

static void setIgnitionPins() {
	// todo: I wonder if these are not right in light of the network rename and the +12 VP issue?
	engineConfiguration->ignitionPins[0] = GPIOD_4;
	engineConfiguration->ignitionPins[1] = GPIOD_3;
	engineConfiguration->ignitionPins[2] = GPIOD_2;
	engineConfiguration->ignitionPins[3] = GPIOD_1;

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
	engineConfiguration->communicationLedPin = GPIOE_2; // d23 = blue
#endif /* EFI_COMMUNICATION_PIN */
	engineConfiguration->runningLedPin = GPIOE_4;		// d22 = green
	engineConfiguration->warningLedPin = GPIOE_1;		// d27 = orange or yellow
}

static void setupVbatt() {
/*
 below 0.4
	// 1k high side/1.5k low side = 1.6667 ratio divider
	engineConfiguration->analogInputDividerCoefficient = 2.5f / 1.5f;
*/

	// 6.8k high side/10k low side = 1.68 ratio divider
	engineConfiguration->analogInputDividerCoefficient = 16.8f / 10.0f;

	// set vbatt_divider 8.23
	// R139=39k high side/R141=10k low side multiplied by above analogInputDividerCoefficient = 8.232f
	engineConfiguration->vbattDividerCoeff = (49.0f / 10.0f) * engineConfiguration->analogInputDividerCoefficient;
	// PC1, pin #1 input +12 from Main Relay. Main Relay controlled by TLE8888
	engineConfiguration->vbattAdcChannel = EFI_ADC_11;

	engineConfiguration->adcVcc = 3.29f;
}

static void setupTle8888() {
	// on microRusEFI SPI3 is exposed on PC10/PC11 and there is interest to use SD card there
	// PB3/PB4 could be either SPI1 or SP3, let's use not SPI3 to address the contention
	// Enable spi1
	CONFIG(is_enabled_spi_1) = true;

	// Wire up spi1
	engineConfiguration->spi1mosiPin = GPIOB_5;
	engineConfiguration->spi1misoPin = GPIOB_4;
	engineConfiguration->spi1sckPin = GPIOB_3;

	// Chip select
	engineConfiguration->tle8888_cs = GPIOD_5;

	// SPI device
	engineConfiguration->tle8888spiDevice = SPI_DEVICE_1;
}

static void setupEtb() {
	// TLE9201 driver
	// This chip has three control pins:
	// DIR - sets direction of the motor
	// PWM - pwm control (enable high, coast low)
	// DIS - disables motor (enable low)

	// PWM pin
	engineConfiguration->etbIo[0].controlPin = GPIOC_7;
	// DIR pin
	engineConfiguration->etbIo[0].directionPin1 = GPIOA_8;
	// Disable pin
	engineConfiguration->etbIo[0].disablePin = GPIOC_8;
	// Unused
	engineConfiguration->etbIo[0].directionPin2 = GPIO_UNASSIGNED;

	// set_analog_input_pin pps PA7
	// EFI_ADC_7: "31 - AN volt 3" - PA7
	// engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_7;

	// we only have pwm/dir, no dira/dirb
	engineConfiguration->etb_use_two_wires = false;
}

static void setupDefaultSensorInputs() {
	// trigger inputs
	// tle8888 VR conditioner
	engineConfiguration->triggerInputPins[0] = GPIOC_6;
	engineConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->triggerInputPins[2] = GPIO_UNASSIGNED;
	// Direct hall-only cam input
	engineConfiguration->camInputs[0] = GPIOA_5;

	// open question if it's great to have TPS in default TPS - the down-side is for
	// vehicles without TPS or for first start without TPS one would have to turn in off
	// to avoid cranking corrections based on wrong TPS data
	// tps = "20 - AN volt 5" PC3
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_13;
	engineConfiguration->tps2_1AdcChannel = EFI_ADC_NONE;


	// EFI_ADC_10: "27 - AN volt 1"
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_10;

	// EFI_ADC_14: "32 - AN volt 6"
	engineConfiguration->afr.hwChannel = EFI_ADC_14;

	// clt = "18 - AN temp 1"
	engineConfiguration->clt.adcChannel = EFI_ADC_0;

	// iat = "23 - AN temp 2"
	engineConfiguration->iat.adcChannel = EFI_ADC_1;

	setCommonNTCSensor(&engineConfiguration->auxTempSensor1, 2700);
	setCommonNTCSensor(&engineConfiguration->auxTempSensor2, 2700);

#if HW_CHECK_MODE
	engineConfiguration->auxTempSensor1.adcChannel = EFI_ADC_2;
	engineConfiguration->auxTempSensor2.adcChannel = EFI_ADC_3;
#endif // HW_CHECK_MODE
}

void setBoardConfigOverrides(void) {
	setLedPins();
	setupVbatt();
	setupTle8888();
	setupEtb();

	engineConfiguration->clt.config.bias_resistor = 2700;
	engineConfiguration->iat.config.bias_resistor = 2700;

	engineConfiguration->canTxPin = GPIOB_6;
	engineConfiguration->canRxPin = GPIOB_12;

	// SPI for SD card
	CONFIG(is_enabled_spi_3) = true;
	engineConfiguration->sdCardSpiDevice = SPI_DEVICE_3;
	engineConfiguration->sdCardCsPin = GPIOB_9;

	engineConfiguration->spi3mosiPin = GPIOC_12;
	engineConfiguration->spi3misoPin = GPIOC_11;
	engineConfiguration->spi3sckPin = GPIOC_10;
}

void setPinConfigurationOverrides(void) {
}

void setSerialConfigurationOverrides(void) {
	// why would MRE disable serial by default? we definitely have pads exposed
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

	// MRE has a special main relay control low side pin
	// rusEfi firmware is totally not involved with main relay control on microRusEfi board
	// todo: maybe even set EFI_MAIN_RELAY_CONTROL to FALSE for MRE configuration
	// TLE8888 half bridges (pushpull, lowside, or high-low)  TLE8888_IN11 / TLE8888_OUT21
	// TLE8888_PIN_21: "35 - GP Out 1"
	engineConfiguration->fuelPumpPin = TLE8888_PIN_21;

//	engineConfiguration->isSdCardEnabled = true;

	// TLE8888 high current low side: VVT2 IN9 / OUT5
	// GPIOE_10: "3 - Lowside 2"
	engineConfiguration->idle.solenoidPin = TLE8888_PIN_5;


	// TLE8888_PIN_22: "34 - GP Out 2"
	engineConfiguration->fanPin = TLE8888_PIN_22;

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
}

/**
 * @brief   Board-specific SD card configuration code overrides. Needed by bootloader code.
 * @todo    Add your board-specific code, if any.
 */
void setSdCardConfigurationOverrides(void) {
}
