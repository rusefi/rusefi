/**
 * @file boards/hellen/uaefi/board_configuration.cpp
 *
 * See https://rusefi.com/s/uaefi
 *
 * @author Andrey Belomutskiy, (c) 2012-2023
 */

#include "pch.h"
#include "defaults.h"
#include "hellen_meta.h"
#ifndef HW_HELLEN_UAEFI121
// normal uaefi here
#include "hellen_leds_100.cpp"
#else
// uaefi121
Gpio getCommsLedPin() {
	return Gpio::MM100_LED3_BLUE;
}

Gpio getRunningLedPin() {
	// this one is used to drive tach pin 43
	return Gpio::Unassigned;
}

Gpio getWarningLedPin() {
	return Gpio::MM100_LED4_YELLOW;
}
#endif
#include "board_overrides.h"
#include "connectors/generated_board_pin_names.h"

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
	engineConfiguration->map.sensor.hwChannel = PIN_D9;

	setPPSInputs(MM100_IN_PPS_ANALOG, MM100_IN_AUX2_ANALOG);

	engineConfiguration->clt.adcChannel = MM100_IN_CLT_ANALOG;
	engineConfiguration->iat.adcChannel = MM100_IN_IAT_ANALOG;

	engineConfiguration->triggerInputPins[0] = Gpio::MM100_UART8_TX; // VR2 max9924 is the safer default
	engineConfiguration->camInputs[0] = Gpio::MM100_IN_D1; // HALL1
	engineConfiguration->camInputs[1] = Gpio::MM100_IN_D2; // HALL2

  engineConfiguration->vehicleSpeedSensorInputPin = Gpio::MM100_IN_D3;
}

static void uaefi_boardConfigOverrides() {
	setHellenMegaEnPin();
	setHellenVbatt();

	hellenMegaSdWithAccelerometer();

  engineConfiguration->vrThreshold[0].pin = Gpio::MM100_OUT_PWM6;

	setHellenCan();

	setDefaultHellenAtPullUps();

}

bool validateBoardConfig() {
#ifndef HW_HELLEN_UAEFI121
  // this same file is used for both uaefi and uaefi121
  if (engineConfiguration->can2RxPin != Gpio::B12) {
	  setHellenCan2();
  }
#endif
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
static void uaefi_boardDefaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();
	setUaefiDefaultETBPins();

  setHellenMMbaro();

	engineConfiguration->displayLogicLevelsInEngineSniffer = true;
	engineConfiguration->isSdCardEnabled = true;

	engineConfiguration->enableSoftwareKnock = true;

	engineConfiguration->canTxPin = Gpio::MM100_CAN_TX;
	engineConfiguration->canRxPin = Gpio::MM100_CAN_RX;
#ifndef HW_HELLEN_UAEFI121
  // this same file is used for both uaefi and uaefi121
	setHellenCan2();
#endif

#if (EFI_CAN_BUS_COUNT >= 3)
	engineConfiguration->can3TxPin = Gpio::MM100_CAN3_TX;
	engineConfiguration->can3RxPin = Gpio::MM100_CAN3_RX;
#endif

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
	Gpio::MM100_INJ6, // B1 injector output 6
	Gpio::MM100_INJ5, // B2 injector output 5
	Gpio::MM100_INJ4, // B3 injector output 4
	Gpio::MM100_INJ3, // B4 injector output 3
	Gpio::MM100_INJ2, // B5 injector output 2
	Gpio::MM100_INJ1, // B6 injector output 1
	Gpio::MM100_INJ7, // B7 Low Side output 1
	Gpio::MM100_IGN8, // B8 Fan Relay Weak Low Side output 2
	Gpio::MM100_IGN7, // B9 Main Relay Weak Low Side output 1
	Gpio::MM100_OUT_PWM2, // B16 Low Side output 4 / Fuel Pump
	Gpio::MM100_OUT_PWM1, // B17 Low Side output 3
	Gpio::MM100_INJ8, // B18 Low Side output 2
	// high sides
	Gpio::MM100_IGN6, // B10 Coil 6
	Gpio::MM100_IGN4, // B11 Coil 4
	Gpio::MM100_IGN3, // B12 Coil 3
	Gpio::MM100_IGN5, // B13 Coil 5
	Gpio::MM100_IGN2, // B14 Coil 2
	Gpio::MM100_IGN1, // B15 Coil 1
};

int getBoardMetaOutputsCount() {
    return efi::size(OUTPUTS);
}

int getBoardMetaLowSideOutputsCount() {
  return getBoardMetaOutputsCount() - 6;
}

Gpio* getBoardMetaOutputs() {
    return OUTPUTS;
}

int getBoardMetaDcOutputsCount() {
    if (engineConfiguration->engineType == engine_type_e::HONDA_OBD1 ||
      engineConfiguration->engineType == engine_type_e::MAZDA_MIATA_NA6 ||
      engineConfiguration->engineType == engine_type_e::MAZDA_MIATA_NA94 ||
      engineConfiguration->engineType == engine_type_e::MAZDA_MIATA_NA96 ||
      engineConfiguration->engineType == engine_type_e::MAZDA_MIATA_NB1 ||
      engineConfiguration->engineType == engine_type_e::MAZDA_MIATA_NB2) {
        return 0;
    }
    return 2;
}

void setup_custom_board_overrides() {
	custom_board_DefaultConfiguration = uaefi_boardDefaultConfiguration;
	custom_board_ConfigOverrides = uaefi_boardConfigOverrides;
}

int boardGetAnalogInputDiagnostic(adc_channel_e hwChannel, float voltage) {
	/* we do not check voltage for valid ragne yet */
	(void)voltage;

	switch (hwChannel) {
		/* inputs that may be affected by incorrect reference voltage */
		case MM100_IN_TPS_ANALOG:
		case MM100_IN_PPS_ANALOG:
		case MM100_IN_IAT_ANALOG:
		case MM100_IN_CLT_ANALOG:
		case MM100_IN_O2S_ANALOG:
		case MM100_IN_O2S2_ANALOG:
		case MM100_IN_MAP1_ANALOG:
		case MM100_IN_AUX1_ANALOG:
		case MM100_IN_AUX2_ANALOG:
		case MM100_IN_AUX4_ANALOG:
			/* TODO: more? */
			return (boardGetAnalogDiagnostic() == ObdCode::None) ? 0 : -1;
		/* all other inputs should not rely on output 5V */
		default:
			return 0;
	}

	return 0;
}
