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
 * See https://github.com/rusefi/rusefi/wiki/Hardware-microRusEfi-wiring
 *
 * @author Matthew Kennedy, (c) 2019
 */

#include "pch.h"
#include "mre_meta.h"

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = Gpio::TLE8888_PIN_1;
	engineConfiguration->injectionPins[1] = Gpio::TLE8888_PIN_2;
	engineConfiguration->injectionPins[2] = Gpio::TLE8888_PIN_3;
	engineConfiguration->injectionPins[3] = Gpio::TLE8888_PIN_4;

	// Disable remainder
	for (int i = 4; i < MAX_CYLINDER_COUNT;i++) {
		engineConfiguration->injectionPins[i] = Gpio::Unassigned;
	}

	engineConfiguration->injectionPinMode = OM_DEFAULT;
}

static void setIgnitionPins() {
	// todo: I wonder if these are not right in light of the network rename and the +12 VP issue?
	engineConfiguration->ignitionPins[0] = Gpio::D4;
	engineConfiguration->ignitionPins[1] = Gpio::D3;
	engineConfiguration->ignitionPins[2] = Gpio::D2;
	engineConfiguration->ignitionPins[3] = Gpio::D1;

	// disable remainder
	for (int i = 4; i < MAX_CYLINDER_COUNT; i++) {
		engineConfiguration->ignitionPins[i] = Gpio::Unassigned;
	}

	engineConfiguration->ignitionPinMode = OM_DEFAULT;
}

static void setLedPins() {
#ifdef EFI_COMMUNICATION_PIN
	engineConfiguration->communicationLedPin = EFI_COMMUNICATION_PIN;
#else
	engineConfiguration->communicationLedPin = Gpio::E2; // d23 = blue
#endif /* EFI_COMMUNICATION_PIN */
	engineConfiguration->runningLedPin = Gpio::E4;		// d22 = green
	engineConfiguration->warningLedPin = Gpio::E1;		// d27 = orange or yellow
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

	// Enable and wire up SPI1
	engineConfiguration->is_enabled_spi_1 = true;
	engineConfiguration->spi1mosiPin = Gpio::B5;
	engineConfiguration->spi1misoPin = Gpio::B4;
	engineConfiguration->spi1sckPin = Gpio::B3;

	// Chip select
	engineConfiguration->tle8888_cs = Gpio::D5;

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
	engineConfiguration->etbIo[0].controlPin = Gpio::C7;
	// DIR pin
	engineConfiguration->etbIo[0].directionPin1 = Gpio::A8;
	// Disable pin
	engineConfiguration->etbIo[0].disablePin = Gpio::C8;
	// Unused
	engineConfiguration->etbIo[0].directionPin2 = Gpio::Unassigned;

	// we only have pwm/dir, no dira/dirb
	engineConfiguration->etb_use_two_wires = false;
}

static void setupDefaultSensorInputs() {
	// trigger inputs
	// tle8888 VR conditioner
	engineConfiguration->triggerInputPins[0] = Gpio::C6;
	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;
	// Direct hall-only cam input
	engineConfiguration->camInputs[0] = Gpio::A5;

	// open question if it's great to have TPS in default TPS - the down-side is for
	// vehicles without TPS or for first start without TPS one would have to turn in off
	// to avoid cranking corrections based on wrong TPS data
	engineConfiguration->tps1_1AdcChannel = MRE_IN_TPS;
	engineConfiguration->tps2_1AdcChannel = EFI_ADC_NONE;


	engineConfiguration->map.sensor.hwChannel = MRE_IN_MAP;

	// EFI_ADC_14: "32 - AN volt 6"
	engineConfiguration->afr.hwChannel = EFI_ADC_14;

	engineConfiguration->clt.adcChannel = MRE_IN_CLT;

	engineConfiguration->iat.adcChannel = MRE_IN_IAT;

	setCommonNTCSensor(&engineConfiguration->auxTempSensor1, 2700);
	setCommonNTCSensor(&engineConfiguration->auxTempSensor2, 2700);

#if HW_CHECK_MODE
	engineConfiguration->auxTempSensor1.adcChannel = EFI_ADC_2;
	engineConfiguration->auxTempSensor2.adcChannel = EFI_ADC_3;
#endif // HW_CHECK_MODE
}

void setBoardConfigOverrides() {
	setLedPins();
	setupVbatt();
	setupTle8888();
	setupEtb();

	engineConfiguration->clt.config.bias_resistor = 2700;
	engineConfiguration->iat.config.bias_resistor = 2700;

	engineConfiguration->canTxPin = Gpio::B6;
	engineConfiguration->canRxPin = Gpio::B12;

	// SPI2 for onboard SD card on v0.6.0
	engineConfiguration->is_enabled_spi_2 = true;
	engineConfiguration->spi2mosiPin = Gpio::B15;
	engineConfiguration->spi2misoPin = Gpio::B14;
	engineConfiguration->spi2sckPin = Gpio::B13;

	// SPI3 for expansion header
	// Don't override enable since you might want these pins for something else
	engineConfiguration->spi3mosiPin = Gpio::C12;
	engineConfiguration->spi3misoPin = Gpio::C11;
	engineConfiguration->spi3sckPin = Gpio::C10;
}

/**
 * @brief   Board-specific configuration defaults.
 *
 * See also setDefaultEngineConfiguration
 *
 * @todo    Add your board-specific code, if any.
 */
void setBoardDefaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();

	// MRE has a special main relay control low side pin
	// rusEfi firmware is totally not involved with main relay control on microRusEfi board
	// todo: maybe even set EFI_MAIN_RELAY_CONTROL to FALSE for MRE configuration
	// TLE8888 half bridges (pushpull, lowside, or high-low)  TLE8888_IN11 / TLE8888_OUT21
	// Gpio::TLE8888_PIN_21: "35 - GP Out 1"
	engineConfiguration->fuelPumpPin = Gpio::TLE8888_PIN_21;

//	engineConfiguration->isSdCardEnabled = true;

	// TLE8888 high current low side: VVT2 IN9 / OUT5
	// Gpio::E10: "3 - Lowside 2"
	engineConfiguration->idle.solenoidPin = Gpio::TLE8888_PIN_5;


	// Gpio::TLE8888_PIN_22: "34 - GP Out 2"
	engineConfiguration->fanPin = Gpio::TLE8888_PIN_22;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	// Enable onboard SD card on v0.6.0
	engineConfiguration->sdCardSpiDevice = SPI_DEVICE_2;
	engineConfiguration->isSdCardEnabled = true;
	engineConfiguration->sdCardCsPin = Gpio::E15;

	// Don't enable expansion header SPI by default
	engineConfiguration->is_enabled_spi_3 = false;

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SIMULTANEOUS;//IM_BATCH;// IM_SEQUENTIAL;
}
