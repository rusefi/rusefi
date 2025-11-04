/**
 * @file boards/hellen/hellen128/board_configuration.cpp
 *
 * All fabricated boards use 176 pin MCU
 *
 * @brief Configuration defaults for the hellen128 board
 *
 * See https://rusefi.com/s/hellen128
 *
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "hellen_all_meta.h"
#include "i2c_bb.h"
#include "defaults.h"
#include "m111.h"
#include "board_overrides.h"

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = H176_LS_1;
	engineConfiguration->injectionPins[1] = H176_LS_2;
	engineConfiguration->injectionPins[2] = H176_LS_3;
	engineConfiguration->injectionPins[3] = H176_LS_4;
	engineConfiguration->injectionPins[4] = H176_LS_5;
	engineConfiguration->injectionPins[5] = H176_LS_6;
	engineConfiguration->injectionPins[6] = H176_LS_7;
	engineConfiguration->injectionPins[7] = H176_LS_8;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = H176_IGN_1;
	engineConfiguration->ignitionPins[1] = H176_IGN_2;
	engineConfiguration->ignitionPins[2] = H176_IGN_3;
	engineConfiguration->ignitionPins[3] = H176_IGN_4;
	engineConfiguration->ignitionPins[4] = H176_IGN_5;
	engineConfiguration->ignitionPins[5] = Gpio::I5;
	engineConfiguration->ignitionPins[6] = Gpio::I6;
	engineConfiguration->ignitionPins[7] = Gpio::I7;
}

static void setupDefaultSensorInputs() {
	// trigger inputs
	engineConfiguration->triggerInputPins[0] = Gpio::B1;
	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;
	// Direct hall-only cam input
	engineConfiguration->camInputs[0] = Gpio::A6;

	setTPS1Inputs(EFI_ADC_4, EFI_ADC_8);

	engineConfiguration->mafAdcChannel = H144_IN_MAP1;
	engineConfiguration->map.sensor.hwChannel = H144_IN_MAP2;

	engineConfiguration->clt.adcChannel = H144_IN_CLT;

	engineConfiguration->iat.adcChannel = H144_IN_IAT;
}

static bool isFirstInvocation = true;

static void setHellen128ETBConfig() {
	BitbangI2c m_i2c;
	uint8_t variant[2] = {0xff, 0xff};

	//same pins as for LPS25
	if (isFirstInvocation) {
		isFirstInvocation = false;
		m_i2c.init(Gpio::B10, Gpio::B11);

		/* TODO: release pis for LPS25 */
	}
	// looks like we support PCF8575 i2c I/O expander
	m_i2c.read(/*address*/0x20, variant, sizeof(variant));

	//Rev C is different then Rev A/B
	if ((variant[0] == 0x63) && (variant[1] == 0x00)) {
	  efiPrintf("rev C Board Detected");
		// TLE9201 driver
		// This chip has three control pins:
		// DIR - sets direction of the motor
		// PWM - pwm control (enable high, coast low)
		// DIS - disables motor (enable low)

		//ETB1
		// PWM pin
		engineConfiguration->etbIo[0].controlPin = H176_OUT_PWM3;
		// DIR pin
		engineConfiguration->etbIo[0].directionPin1 = H176_OUT_PWM2;
		engineConfiguration->etbIo[0].directionPin2 = Gpio::Unassigned;
		// Disable pin
		engineConfiguration->etbIo[0].disablePin = H176_OUT_PWM1;

		//ETB2
		// PWM pin
		engineConfiguration->etbIo[1].controlPin = Gpio::I2;
		// DIR pin
		engineConfiguration->etbIo[1].directionPin1 = Gpio::H13;
		// Disable pin
		engineConfiguration->etbIo[1].disablePin = Gpio::B7;

		// we only have pwm/dir, no dira/dirb
		engineConfiguration->etb_use_two_wires = false;

	} else {
	  efiPrintf("A/B BoardID [%02x%02x] ", variant[0], variant[1]);
		//Set default ETB config
		engineConfiguration->etbIo[0].directionPin1 = H176_OUT_PWM2;
		engineConfiguration->etbIo[0].directionPin2 = H176_OUT_PWM3;
		engineConfiguration->etbIo[0].controlPin = H176_OUT_PWM1; // ETB_EN
		engineConfiguration->etbIo[0].disablePin = Gpio::Unassigned;
		engineConfiguration->etb_use_two_wires = true;
	}
}

#include "hellen_leds_176.cpp"

static void hellen128_boardConfigOverrides() {
	setHellenVbatt();

	setHellenSdCardSpi2();

	setHellen128ETBConfig();

    // this specific Hellen has less common pull-up value R49
	engineConfiguration->clt.config.bias_resistor = 2700;
	engineConfiguration->iat.config.bias_resistor = 2700;

	setHellenCan();
}

/**
 * @brief   Board-specific configuration defaults.
 *

 *

 */
static void hellen128_boardDefaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();

	engineConfiguration->fuelPumpPin = Gpio::D15;
	engineConfiguration->idle.solenoidPin = Gpio::Unassigned;
	engineConfiguration->fanPin = Gpio::D12;	// OUT_PWM8
	engineConfiguration->mainRelayPin = Gpio::Unassigned;

	engineConfiguration->starterControlPin = H176_OUT_IO10;
	engineConfiguration->startStopButtonPin = H176_IN_A16;
	engineConfiguration->startStopButtonMode = PI_PULLDOWN;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

  setMercedesM111EngineConfiguration();

	/**
	 * Jimmy best tune
	 * https://rusefi.com/online/view.php?msq=626
	 * md_sanci latest tune
	 * https://rusefi.com/online/view.php?msq=630
	 */
	setPPSInputs(H144_IN_PPS, EFI_ADC_14);
	setPPSCalibration(1.49, 4.72, 1.34, 4.24);

	engineConfiguration->vrThreshold[0].pin = Gpio::D14;
	hellenWbo();
}

void setup_custom_board_overrides() {
	custom_board_DefaultConfiguration = hellen128_boardDefaultConfiguration;
	custom_board_ConfigOverrides =  hellen128_boardConfigOverrides;
}
