/**
 * @file boards/hellen/hellen154hyundai/board_configuration.cpp
 *
 *
 * @brief Configuration defaults for the hellen154hyundai board
 *
 * See https://rusefi.com/s/hellen154hyundai
 *
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "defaults.h"
#include "hellen_meta.h"
#include "board_overrides.h"

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = Gpio::H144_LS_1;
	engineConfiguration->injectionPins[1] = Gpio::H144_LS_2;
	engineConfiguration->injectionPins[2] = Gpio::H144_LS_3;
	engineConfiguration->injectionPins[3] = Gpio::H144_LS_4;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = Gpio::H144_IGN_1;
	engineConfiguration->ignitionPins[1] = Gpio::H144_IGN_2;
	engineConfiguration->ignitionPins[2] = Gpio::H144_IGN_3;
	engineConfiguration->ignitionPins[3] = Gpio::H144_IGN_4;
}

static void setupDefaultSensorInputs() {
	engineConfiguration->vvtMode[0] = VVT_SINGLE_TOOTH;
	engineConfiguration->vvtMode[1] = VVT_SINGLE_TOOTH;

    engineConfiguration->vehicleSpeedSensorInputPin = Gpio::H144_IN_VSS;

	setTPS1Inputs(H144_IN_TPS, H144_IN_AUX1_ANALOG);

	setPPSInputs(EFI_ADC_3, EFI_ADC_14);

	engineConfiguration->map.sensor.hwChannel = H144_IN_MAP1;

	engineConfiguration->afr.hwChannel = EFI_ADC_NONE;

	engineConfiguration->clt.adcChannel = H144_IN_CLT;

	engineConfiguration->iat.adcChannel = H144_IN_IAT;
}

#ifndef EFI_BOOTLOADER
static bool isFirstInvocation = true;
#endif // EFI_BOOTLOADER

/*PUBLIC_API_WEAK*/ int hackHellenBoardId(int detectedId) {
  if (detectedId == BOARD_ID_VAG121_D) {
    // wow something bad has happened to that batch of boards?!
    return BOARD_ID_154HYUNDAI_C;
  }
  return detectedId;
}

static void hellen154_hyundai_boardConfigOverrides() {
	setHellenVbatt();

	setHellenSdCardSpi2();

    setDefaultHellenAtPullUps();

	// trigger inputs
	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;
	// Direct hall-only cam input
	// exhaust input same on both revisions
	engineConfiguration->camInputs[1] = Gpio::H144_ORIGINAL_MCU_IN_D_AUX4;

#ifndef EFI_BOOTLOADER
    int16_t hellenBoardId = engine->engineState.hellenBoardId;

    if (hellenBoardId == -1) {
	    engineConfiguration->triggerInputPins[0] = Gpio::H144_IN_CRANK;
	    engineConfiguration->camInputs[0] = Gpio::H144_IN_CAM;

		// control pins are inverted since overall ECU pinout seems to be inverted
		engineConfiguration->etbIo[0].directionPin1 = Gpio::H144_OUT_PWM3;
		engineConfiguration->etbIo[0].directionPin2 = Gpio::H144_OUT_PWM2;
		engineConfiguration->etbIo[0].controlPin = Gpio::H144_OUT_IO12;
	   	engineConfiguration->etbIo[0].disablePin = Gpio::Unassigned;
		engineConfiguration->etb_use_two_wires = true;

		// first revision of did not have Hellen Board ID
		// https://github.com/rusefi/hellen154hyundai/issues/55
		engineConfiguration->etbIo[1].directionPin1 = Gpio::Unassigned;
		engineConfiguration->etbIo[1].directionPin2 = Gpio::Unassigned;
		engineConfiguration->etbIo[1].controlPin = Gpio::Unassigned;
	   	engineConfiguration->etbIo[1].disablePin = Gpio::Unassigned;

		if (isFirstInvocation) {
			isFirstInvocation = false;
			efiSetPadMode("ETB FIX0", Gpio::H144_OUT_PWM4, PAL_MODE_INPUT_ANALOG);
			efiSetPadMode("ETB FIX1", Gpio::H144_OUT_PWM5, PAL_MODE_INPUT_ANALOG);
			efiSetPadMode("ETB FIX2", Gpio::H144_OUT_IO13, PAL_MODE_INPUT_ANALOG);
		}
		engineConfiguration->stepperDcInvertedPins = false;
	} else if (hellenBoardId == BOARD_ID_154HYUNDAI_C || hellenBoardId == BOARD_ID_154HYUNDAI_D) {
		engineConfiguration->triggerInputPins[0] = Gpio::H144_IN_SENS2;
		engineConfiguration->camInputs[0] = Gpio::H144_IN_SENS3;


		// todo You would not believe how you invert TLE9201 #4579
		engineConfiguration->stepperDcInvertedPins = true;

	    //ETB1
	    // PWM pin
	    engineConfiguration->etbIo[0].controlPin = Gpio::H144_OUT_PWM2;
	    // DIR pin
		engineConfiguration->etbIo[0].directionPin1 = Gpio::H144_OUT_PWM3;
	   	// Disable pin
	   	engineConfiguration->etbIo[0].disablePin = Gpio::H144_OUT_IO12;

		// wastegate DC motor
	    //ETB2
	    // PWM pin
	    engineConfiguration->etbIo[1].controlPin = Gpio::H144_OUT_PWM4;
	    // DIR pin
		engineConfiguration->etbIo[1].directionPin1 = Gpio::H144_OUT_PWM5;
	   	// Disable pin
	   	engineConfiguration->etbIo[1].disablePin = Gpio::H144_OUT_IO13;
    }
#endif // EFI_BOOTLOADER
}

/**
 * @brief   Board-specific configuration defaults.
 *

 *

 */
static void hellen154_hyundai_boardDefaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();

	engineConfiguration->isSdCardEnabled = true;
	setHellenCan();

	engineConfiguration->fuelPumpPin = Gpio::H144_OUT_IO9;
	engineConfiguration->fanPin = Gpio::H144_OUT_IO7;
	engineConfiguration->mainRelayPin = Gpio::H144_OUT_IO3;	// pin: 111a
	// BK1 uses wire, BK2 uses CANbus
	engineConfiguration->malfunctionIndicatorPin = Gpio::H144_OUT_PWM8;

	engineConfiguration->brakePedalPin = Gpio::H144_IN_RES3;
	engineConfiguration->clutchUpPin = Gpio::H144_IN_RES2;
	engineConfiguration->acSwitch = Gpio::H144_IN_RES1;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	engineConfiguration->etbFunctions[1] = DC_Wastegate;
}

static Gpio OUTPUTS[] = {
	Gpio::H144_LS_1, // 0: INJ_1 k25
	Gpio::H144_LS_2, // INJ_2 k26
	Gpio::H144_LS_3, // INJ_3 k27
	Gpio::H144_LS_4, // 3: INJ_4 k28
	Gpio::H144_LS_5, // VVT1
	Gpio::H144_LS_6, // 5: VVT2
	Gpio::H144_LS_7, // K47 BK1 Wastegate Solenoid
	Gpio::H144_OUT_IO7, // Fan Relay Low
	Gpio::H144_OUT_IO3, // Main Relay K64
	Gpio::H144_OUT_IO9, // Fuel Pump K70
	Gpio::H144_OUT_IO10, // 10: K87 AC Relay
	Gpio::H144_OUT_IO8, // Fan Relay HI
	Gpio::H144_IGN_1, // Coil 1
	Gpio::H144_IGN_2, // Coil 2
	Gpio::H144_IGN_3, // Coil 3
	Gpio::H144_IGN_4, // Coil 4
	Gpio::H144_OUT_PWM8, // MIL
//	QC procedure seems to not work for that one Gpio::H144_OUT_PWM7, // low side? Tacho unused CAN tachometer right?
//	Gpio::H_SPI1_SCK, // X8 AuxLS1
};

int getBoardMetaOutputsCount() {
    return efi::size(OUTPUTS);
}

int getBoardMetaLowSideOutputsCount() {
    return getBoardMetaOutputsCount();
}

int getBoardMetaDcOutputsCount() {
    return 2;
}

Gpio* getBoardMetaOutputs() {
    return OUTPUTS;
}

void setup_custom_board_overrides() {
	custom_board_DefaultConfiguration = hellen154_hyundai_boardDefaultConfiguration;
	custom_board_ConfigOverrides = hellen154_hyundai_boardConfigOverrides;
}
