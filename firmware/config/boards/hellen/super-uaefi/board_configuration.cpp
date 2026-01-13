/**
 * See https://rusefi.com/s/super-uaefi
 *
 * @author Andrey Belomutskiy, (c) 2012-2023
 */

#include "pch.h"
#include "defaults.h"
#include "hellen_meta.h"
#include "board_overrides.h"
//#include "connectors/generated_board_pin_names.h"

Gpio getCommsLedPin() {
	return Gpio::MM100_LED3_BLUE;
}

Gpio getRunningLedPin() {
	// this one is used to drive 20D - High side output
	return Gpio::Unassigned;
}

Gpio getWarningLedPin() {
	return Gpio::MM100_LED4_YELLOW;
}

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = Gpio::MM100_INJ1;
	engineConfiguration->injectionPins[1] = Gpio::MM100_INJ2;
	engineConfiguration->injectionPins[2] = Gpio::MM100_INJ3;
	engineConfiguration->injectionPins[3] = Gpio::MM100_INJ4;
	engineConfiguration->injectionPins[4] = Gpio::MM100_INJ5;
	engineConfiguration->injectionPins[5] = Gpio::MM100_INJ6;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = Gpio::MM100_IGN1;
	engineConfiguration->ignitionPins[1] = Gpio::MM100_IGN2;
	engineConfiguration->ignitionPins[2] = Gpio::MM100_IGN3;
	engineConfiguration->ignitionPins[3] = Gpio::MM100_IGN4;
	engineConfiguration->ignitionPins[4] = Gpio::MM100_IGN5;
	engineConfiguration->ignitionPins[5] = Gpio::MM100_IGN6;
}

static void setupDefaultSensorInputs() {
	engineConfiguration->tps1_1AdcChannel = MM100_IN_TPS_ANALOG;
	engineConfiguration->tps1_2AdcChannel = MM100_IN_AUX1_ANALOG;
//	engineConfiguration->map.sensor.hwChannel = PIN_D9;

	setPPSInputs(MM100_IN_PPS_ANALOG, MM100_IN_AUX2_ANALOG);

	engineConfiguration->clt.adcChannel = MM100_IN_CLT_ANALOG;
	engineConfiguration->iat.adcChannel = MM100_IN_IAT_ANALOG;

	engineConfiguration->triggerInputPins[0] = Gpio::MM100_UART8_TX; // VR2 max9924 is the safer default
	engineConfiguration->camInputs[0] = Gpio::MM100_IN_D1; // HALL1
	engineConfiguration->camInputs[1] = Gpio::MM100_IN_D2; // HALL2

  engineConfiguration->vehicleSpeedSensorInputPin = Gpio::MM100_IN_D3;
}

static void super_uaefi_boardConfigOverrides() {
	setHellenMegaEnPin();
	setHellenVbatt();

	hellenMegaSdWithAccelerometer();

  engineConfiguration->vrThreshold[0].pin = Gpio::MM100_OUT_PWM6;

	setHellenCan();

	setDefaultHellenAtPullUps();

}

bool validateBoardConfig() {
  return true;
}

void setUaefiDefaultETBPins() {
  // users would want to override those if using H-bridges for stepper idle control
  setupTLE9201IncludingStepper(/*PWM controlPin*/Gpio::MM100_OUT_PWM3, Gpio::MM100_OUT_PWM4, Gpio::MM100_SPI2_MISO);
  setupTLE9201IncludingStepper(/*PWM controlPin*/Gpio::MM100_OUT_PWM5, Gpio::MM100_SPI2_MOSI, Gpio::MM100_USB1ID, 1);
}

/**
 * @brief   Board-specific configuration defaults.
 *

 *
 */
static void super_uaefi_boardDefaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();
	setUaefiDefaultETBPins();

  setHellenMMbaro();

	engineConfiguration->displayLogicLevelsInEngineSniffer = true;
	engineConfiguration->isSdCardEnabled = true;

	engineConfiguration->enableSoftwareKnock = true;

	engineConfiguration->canTxPin = Gpio::MM100_CAN_TX;
	engineConfiguration->canRxPin = Gpio::MM100_CAN_RX;

  engineConfiguration->mainRelayPin = Gpio::MM100_IGN7;
 	engineConfiguration->fanPin = Gpio::MM100_IGN8;
	engineConfiguration->fuelPumpPin = Gpio::MM100_OUT_PWM2;

	// SPI3 for on-board EGT
	engineConfiguration->is_enabled_spi_3 = true;
	// MOSI not needed, we have one-way communication here
	engineConfiguration->spi3misoPin = Gpio::C11;
	engineConfiguration->spi3sckPin = Gpio::C10;
	engineConfiguration->max31855_cs[0] = Gpio::A15;
	engineConfiguration->max31855spiDevice = SPI_DEVICE_3;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();
	engineConfiguration->enableVerboseCanTx = true;

	engineConfiguration->etbFunctions[0] = DC_Throttle1;

	// Some sensible defaults for other options
	setCrankOperationMode();

	setAlgorithm(engine_load_mode_e::LM_SPEED_DENSITY);

	engineConfiguration->injectorCompensationMode = ICM_FixedRailPressure;

#ifndef EFI_BOOTLOADER
	setCommonNTCSensor(&engineConfiguration->clt, HELLEN_DEFAULT_AT_PULLUP);
	setCommonNTCSensor(&engineConfiguration->iat, HELLEN_DEFAULT_AT_PULLUP);
#endif // EFI_BOOTLOADER

    setTPS1Calibration(100, 650);
	hellenWbo();
}

static Gpio OUTPUTS[] = {
	Gpio::MM100_INJ1, // 13B INJ_1
	Gpio::MM100_INJ2, // 12B INJ_2
	Gpio::MM100_INJ3, // 11B INJ_3
	Gpio::MM100_INJ4, // 10B INJ_4
	Gpio::MM100_INJ5, // 9B INJ_5
	Gpio::MM100_INJ6, // 8B INJ_6
	Gpio::MM100_SPI2_CS, // 19B INJ_7
	Gpio::MM100_SPI2_SCK, // 18B INJ_8
	Gpio::MM100_INJ7, // 20B LS1
	Gpio::MM100_INJ8, // 21B LS2
	Gpio::MM100_OUT_PWM1, // 22B LS3
	Gpio::MM100_OUT_PWM2, // 31C LS4
	Gpio::MM100_IGN7, // 30C LS5_HOT
	Gpio::MM100_IGN8, // 29C LS6_HOT
	Gpio::MM100_IGN1, // Coil 1
	Gpio::MM100_IGN2, // Coil 2
	Gpio::MM100_IGN3, // Coil 3
	Gpio::MM100_IGN4, // Coil 4
	Gpio::MM100_IGN5, // Coil 5
	Gpio::MM100_IGN6, // Coil 6
	Gpio::MM100_LED2_GREEN, // 20D High Side Output
};

int getBoardMetaOutputsCount() {
    return efi::size(OUTPUTS);
}

int getBoardMetaLowSideOutputsCount() {
  return getBoardMetaOutputsCount() - 7;
}

Gpio* getBoardMetaOutputs() {
    return OUTPUTS;
}

int getBoardMetaDcOutputsCount() {
    return 2;
}

void setup_custom_board_overrides() {
	custom_board_DefaultConfiguration = super_uaefi_boardDefaultConfiguration;
	custom_board_ConfigOverrides = super_uaefi_boardConfigOverrides;
}
