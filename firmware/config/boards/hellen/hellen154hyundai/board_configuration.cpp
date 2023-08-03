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

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = H144_LS_1;
	engineConfiguration->injectionPins[1] = H144_LS_2;
	engineConfiguration->injectionPins[2] = H144_LS_3;
	engineConfiguration->injectionPins[3] = H144_LS_4;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = H144_IGN_1;
	engineConfiguration->ignitionPins[1] = H144_IGN_2;
	engineConfiguration->ignitionPins[2] = H144_IGN_3;
	engineConfiguration->ignitionPins[3] = H144_IGN_4;
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
	engineConfiguration->vvtMode[0] = VVT_SINGLE_TOOTH;
	engineConfiguration->vvtMode[1] = VVT_SINGLE_TOOTH;

    engineConfiguration->vehicleSpeedSensorInputPin = H144_IN_VSS;

	setTPS1Inputs(H144_IN_TPS, H144_IN_AUX1);

	setPPSCalibration(0.73, 4.0, 0.34, 1.86);

	setPPSInputs(EFI_ADC_3, EFI_ADC_14);
	engineConfiguration->mafAdcChannel = EFI_ADC_NONE;
	engineConfiguration->map.sensor.hwChannel = H144_IN_MAP1;

	engineConfiguration->afr.hwChannel = EFI_ADC_NONE;

	engineConfiguration->clt.adcChannel = H144_IN_CLT;

	engineConfiguration->iat.adcChannel = H144_IN_IAT;
}

#include "hellen_leds_144.cpp"

static bool isFirstInvocation = true;

void setBoardConfigOverrides() {
	setupVbatt();

	setHellenSdCardSpi2();

    setDefaultHellenAtPullUps();

	// trigger inputs
	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;
	// Direct hall-only cam input
	// exhaust input same on both revisions
	engineConfiguration->camInputs[1] = H144_IN_D_AUX4;

	if (engine->engineState.hellenBoardId == -1) {
	    engineConfiguration->triggerInputPins[0] = H144_IN_CRANK;
	    engineConfiguration->camInputs[0] = H144_IN_CAM;

		// control pins are inverted since overall ECU pinout seems to be inverted
		engineConfiguration->etbIo[0].directionPin1 = H144_OUT_PWM3;
		engineConfiguration->etbIo[0].directionPin2 = H144_OUT_PWM2;
		engineConfiguration->etbIo[0].controlPin = H144_OUT_IO12;
		engineConfiguration->etb_use_two_wires = true;

		// first revision of did not have Hellen Board ID
		// https://github.com/rusefi/hellen154hyundai/issues/55
		engineConfiguration->etbIo[1].directionPin1 = Gpio::Unassigned;
		engineConfiguration->etbIo[1].directionPin2 = Gpio::Unassigned;
		engineConfiguration->etbIo[1].controlPin = Gpio::Unassigned;

		if (isFirstInvocation) {
			isFirstInvocation = false;
			efiSetPadMode("ETB FIX0", H144_OUT_PWM4, PAL_MODE_INPUT_ANALOG);
			efiSetPadMode("ETB FIX1", H144_OUT_PWM5, PAL_MODE_INPUT_ANALOG);
			efiSetPadMode("ETB FIX2", H144_OUT_IO13, PAL_MODE_INPUT_ANALOG);
		}
	} else if (engine->engineState.hellenBoardId == BOARD_ID_154hyundai_c) {
		engineConfiguration->triggerInputPins[0] = H144_IN_SENS2;
		engineConfiguration->camInputs[0] = H144_IN_SENS3;


		// todo You would not believe how you invert TLE9201 #4579
		engineConfiguration->stepperDcInvertedPins = true;

	    //ETB1
	    // PWM pin
	    engineConfiguration->etbIo[0].controlPin = H144_OUT_PWM2;
	    // DIR pin
		engineConfiguration->etbIo[0].directionPin1 = H144_OUT_PWM3;
	   	// Disable pin
	   	engineConfiguration->etbIo[0].disablePin = H144_OUT_IO12;
	   	// Unused
	 	engineConfiguration->etbIo[0].directionPin2 = Gpio::Unassigned;

		// wastegate DC motor
	    //ETB2
	    // PWM pin
	    engineConfiguration->etbIo[1].controlPin = H144_OUT_PWM4;
	    // DIR pin
		engineConfiguration->etbIo[1].directionPin1 = H144_OUT_PWM5;
	   	// Disable pin
	   	engineConfiguration->etbIo[1].disablePin = H144_OUT_IO13;
	   	// Unused
	 	engineConfiguration->etbIo[1].directionPin2 = Gpio::Unassigned;
    }
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

	engineConfiguration->isSdCardEnabled = true;

	engineConfiguration->enableSoftwareKnock = true;
	engineConfiguration->canNbcType = CAN_BUS_GENESIS_COUPE;

	engineConfiguration->canTxPin = H176_CAN_TX;
	engineConfiguration->canRxPin = H176_CAN_RX;

	engineConfiguration->fuelPumpPin = H144_OUT_IO9;
//	engineConfiguration->idle.solenoidPin = Gpio::D14;	// OUT_PWM5
//	engineConfiguration->fanPin = Gpio::D12;	// OUT_PWM8
	engineConfiguration->mainRelayPin = Gpio::G14;	// pin: 111a, OUT_IO3
	engineConfiguration->malfunctionIndicatorPin = H144_OUT_PWM8;

	engineConfiguration->brakePedalPin = H144_IN_RES3;
	engineConfiguration->clutchUpPin = H144_IN_RES2;
	engineConfiguration->acSwitch = H144_IN_RES1;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	engineConfiguration->etbFunctions[1] = DC_Wastegate;

	// Some sensible defaults for other options
	setCrankOperationMode();

	setAlgorithm(LM_SPEED_DENSITY);

	setEtbPID(8.8944, 70.2307, 0.1855);

	engineConfiguration->injectorCompensationMode = ICM_FixedRailPressure;

	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_3_4_2;
	engineConfiguration->displacement = 1.998;
	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_Hyundai);
	strcpy(engineConfiguration->engineCode, "Theta II");
	engineConfiguration->globalTriggerAngleOffset = 90;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK



    setTPS1Calibration(98, 926, 891, 69);
}
