/**
 * @file boards/hellen/hellen64_miataNA6_94/board_configuration.cpp
 *
 *
 * @brief Configuration defaults for the hellen64_miataNA6_94 board
 *
 * See https://rusefi.com/s/hellenNA6
 *
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "hellen_meta.h"

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = H176_LS_1;
	engineConfiguration->injectionPins[1] = Gpio::G8;
	engineConfiguration->injectionPins[2] = Gpio::D11;
	engineConfiguration->injectionPins[3] = Gpio::D10;

	engineConfiguration->injectionPinMode = OM_DEFAULT;

	engineConfiguration->clutchDownPin = Gpio::C4; // Clutch switch input
	engineConfiguration->clutchDownPinMode = PI_PULLDOWN;
	engineConfiguration->launchActivationMode = CLUTCH_INPUT_LAUNCH;
	engineConfiguration->malfunctionIndicatorPin = Gpio::G4; //1E - Check Engine Light
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = Gpio::I8; // 3F - IGN_1 (1&4)
	engineConfiguration->ignitionPins[1] = Gpio::Unassigned ; // Gpio::E4
	engineConfiguration->ignitionPins[2] = Gpio::E5; // 3I - IGN_2 (2&3)
	engineConfiguration->ignitionPins[3] = Gpio::Unassigned; // Gpio::E3

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
	engineConfiguration->triggerInputPins[0] = Gpio::A6;
	engineConfiguration->triggerInputPins[1] = Gpio::B1;
	engineConfiguration->camInputs[0] = Gpio::Unassigned;

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_4;

	engineConfiguration->mafAdcChannel = EFI_ADC_10;
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_11;

	engineConfiguration->afr.hwChannel = EFI_ADC_1;

	engineConfiguration->clt.adcChannel = H144_IN_CLT;

	engineConfiguration->iat.adcChannel = H144_IN_IAT;
}

#include "hellen_leds_176.cpp"

void setBoardConfigOverrides() {
	setupVbatt();

	setHellenSdCardSpi2();

    setDefaultHellenAtPullUps();

	engineConfiguration->canTxPin = H176_CAN_TX;
	engineConfiguration->canRxPin = H176_CAN_RX;
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

	// these stm32 pins do not match hellen_meta, is that because hellenNA6 is not using latest MCU version?
	engineConfiguration->acRelayPin = Gpio::H15; // 1J - AC Relay
	engineConfiguration->fuelPumpPin = Gpio::G2;	// OUT_IO9
	engineConfiguration->idle.solenoidPin = Gpio::D14;	// OUT_PWM5
	engineConfiguration->fanPin = Gpio::D12;	// OUT_PWM8
	engineConfiguration->mainRelayPin = Gpio::I2;	// OUT_LOW3
	engineConfiguration->tachOutputPin = Gpio::I0;
	engineConfiguration->malfunctionIndicatorPin = Gpio::G9;

	engineConfiguration->vehicleSpeedSensorInputPin = H144_IN_VSS;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_3_4_2;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK


}
