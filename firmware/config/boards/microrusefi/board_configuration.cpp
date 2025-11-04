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
#include "defaults.h"
#include "board_overrides.h"

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = MRE_INJ_1;
	engineConfiguration->injectionPins[1] = MRE_INJ_2;
	engineConfiguration->injectionPins[2] = MRE_INJ_3;
	engineConfiguration->injectionPins[3] = MRE_INJ_4;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = Gpio::D4;
	engineConfiguration->ignitionPins[1] = Gpio::D3;
	engineConfiguration->ignitionPins[2] = Gpio::D2;
	engineConfiguration->ignitionPins[3] = Gpio::D1;
}

Gpio getCommsLedPin() {
	return Gpio::E2; // d23 = blue
}

Gpio getRunningLedPin() {
	// D22 = green
	return Gpio::E4;
}

Gpio getWarningLedPin() {
	// D27 = orange or yellow
	return Gpio::E1;
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

	engineConfiguration->adcVcc = 3.3f;
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
	setupTLE9201(/*PWM controlPin*/Gpio::C7, Gpio::A8, Gpio::C8);
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

	engineConfiguration->map.sensor.hwChannel = MRE_IN_MAP;

	// EFI_ADC_14: "32 - AN volt 6"
	engineConfiguration->afr.hwChannel = EFI_ADC_14;

	engineConfiguration->clt.adcChannel = MRE_IN_CLT;

	engineConfiguration->iat.adcChannel = MRE_IN_IAT;

#ifndef EFI_BOOTLOADER
	setCommonNTCSensor(&engineConfiguration->auxTempSensor1, MRE_DEFAULT_AT_PULLUP);
	setCommonNTCSensor(&engineConfiguration->auxTempSensor2, MRE_DEFAULT_AT_PULLUP);
#endif // EFI_BOOTLOADER
}

static void microrusefi_boardConfigOverrides() {
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

 *

 */
static void microrusefi_boardDefaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();

	// MRE has a special main relay control low side pin
	// rusEfi firmware is totally not involved with main relay control on microRusEfi board
	// todo: maybe even set EFI_MAIN_RELAY_CONTROL to FALSE for MRE configuration
	// TLE8888 half bridges (pushpull, lowside, or high-low)  TLE8888_IN11 / TLE8888_OUT21
	// Gpio::TLE8888_PIN_21: "35 - GP Out 1"
	engineConfiguration->fuelPumpPin = MRE_GPOUT_1;

//	engineConfiguration->isSdCardEnabled = true;

	// TLE8888 high current low side: VVT2 IN9 / OUT5
	// Gpio::E10: "3 - Lowside 2"
	engineConfiguration->idle.solenoidPin = MRE_LS_2;


	// Gpio::TLE8888_PIN_22: "34 - GP Out 2"
	engineConfiguration->fanPin = MRE_GPOUT_2;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	// Enable onboard SD card on v0.6.0
	engineConfiguration->sdCardSpiDevice = SPI_DEVICE_2;
	engineConfiguration->isSdCardEnabled = true;
	engineConfiguration->sdCardCsPin = Gpio::E15;

	// Don't enable expansion header SPI by default
	engineConfiguration->is_enabled_spi_3 = false;

	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_3_4_2;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SIMULTANEOUS;//IM_BATCH;// IM_SEQUENTIAL;
}

static Gpio MRE_OUTPUTS[] = {
MRE_INJ_1,
MRE_INJ_2,
MRE_INJ_3,
MRE_INJ_4,
MRE_LS_1,
};

static Gpio M111_OUTPUTS[] = {
MRE_INJ_1, // green
MRE_INJ_2, // white
MRE_INJ_3, // blue
MRE_INJ_4, //
#if HW_MICRO_RUSEFI
MRE_AV9_REUSE, // brown boost control
MRE_LS_1, // VVT
MRE_LS_2, // SC clutch
//MRE_GPOUT_3, // SC Bypass
#endif // HW_MICRO_RUSEFI
};

int getBoardMetaOutputsCount() {
    if (engineConfiguration->engineType == engine_type_e::MERCEDES_M111) {
        return efi::size(M111_OUTPUTS);
    }
    return efi::size(MRE_OUTPUTS);
}

Gpio* getBoardMetaOutputs() {
    if (engineConfiguration->engineType == engine_type_e::MERCEDES_M111) {
        return M111_OUTPUTS;
    }
    return MRE_OUTPUTS;
}

int getBoardMetaDcOutputsCount() {
    return 1;
}

void setup_custom_board_overrides() {
	custom_board_DefaultConfiguration = microrusefi_boardDefaultConfiguration;
	custom_board_ConfigOverrides = microrusefi_boardConfigOverrides;
}
