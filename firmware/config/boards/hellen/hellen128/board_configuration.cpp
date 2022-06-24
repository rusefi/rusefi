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
#include "custom_engine.h"
#include "hellen_meta.h"
#include "i2c_bb.h"

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
		engineConfiguration->injectionPins[i] = Gpio::Unassigned;
	}

	engineConfiguration->injectionPinMode = OM_DEFAULT;
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
	
	// disable remainder
	for (int i = 8; i < MAX_CYLINDER_COUNT; i++) {
		engineConfiguration->ignitionPins[i] = Gpio::Unassigned;
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
	engineConfiguration->triggerInputPins[0] = Gpio::B1;
	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;
	// Direct hall-only cam input
	engineConfiguration->camInputs[0] = Gpio::A6;

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_4;
	engineConfiguration->tps1_2AdcChannel = EFI_ADC_8;
	engineConfiguration->tps2_1AdcChannel = EFI_ADC_NONE;

	engineConfiguration->mafAdcChannel = H144_IN_MAP1;
	engineConfiguration->map.sensor.hwChannel = H144_IN_MAP2;

	engineConfiguration->afr.hwChannel = EFI_ADC_1;

	engineConfiguration->clt.adcChannel = H144_IN_CLT;

	engineConfiguration->iat.adcChannel = H144_IN_IAT;

	engineConfiguration->auxTempSensor1.adcChannel = EFI_ADC_NONE;
	engineConfiguration->auxTempSensor2.adcChannel = EFI_ADC_NONE;
}

static bool isFirstInvocation = true;

static void setHellen128ETBConfig() {
	BitbangI2c m_i2c;
	uint8_t variant[2]={0xff,0xff};

	//same pins as for LPS25
	if (isFirstInvocation) {
		isFirstInvocation = false;
		m_i2c.init(Gpio::B10, Gpio::B11);
	}
	m_i2c.read(0x20, variant, sizeof(variant));

	efiPrintf ("BoardID [%02x%02x] ", variant[0],variant[1] );

	//Rev C is different then Rev A/B
	if ((variant[0] == 0x63) && (variant[1] == 0x00)) {
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
		// Disable pin
		engineConfiguration->etbIo[0].disablePin = H176_OUT_PWM1;
		// Unused
		engineConfiguration->etbIo[0].directionPin2 = Gpio::Unassigned;

		//ETB2
		// PWM pin
		engineConfiguration->etbIo[1].controlPin = Gpio::I2;
		// DIR pin
		engineConfiguration->etbIo[1].directionPin1 = Gpio::H13;
		// Disable pin
		engineConfiguration->etbIo[1].disablePin = Gpio::B7;
		// Unused
		engineConfiguration->etbIo[1].directionPin2 = Gpio::Unassigned;

		// we only have pwm/dir, no dira/dirb
		engineConfiguration->etb_use_two_wires = false;

	} else {
		//Set default ETB config
		engineConfiguration->etbIo[0].directionPin1 = H176_OUT_PWM2;
		engineConfiguration->etbIo[0].directionPin2 = H176_OUT_PWM3;
		engineConfiguration->etbIo[0].controlPin = H176_OUT_PWM1; // ETB_EN
		engineConfiguration->etb_use_two_wires = true;
	}	
}

void setBoardConfigOverrides() {
	setHellen176LedPins();
	setupVbatt();

	setHellenSdCardSpi2();

	setHellen128ETBConfig();

    // this specific Hellen has less common pull-up value R49
	engineConfiguration->clt.config.bias_resistor = 2700;
	engineConfiguration->iat.config.bias_resistor = 2700;

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

	engineConfiguration->fuelPumpPin = Gpio::D15;
	engineConfiguration->idle.solenoidPin = Gpio::Unassigned;
	engineConfiguration->fanPin = Gpio::D12;	// OUT_PWM8
	engineConfiguration->mainRelayPin = Gpio::Unassigned;

	engineConfiguration->starterControlPin = H176_OUT_IO10;
	engineConfiguration->startStopButtonPin = H176_IN_A16;
	engineConfiguration->startStopButtonMode = PI_PULLDOWN;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;
	engineConfiguration->specs.displacement = 2.295f;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK
	engineConfiguration->crankingInjectionMode = IM_SEQUENTIAL;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;//IM_BATCH;// IM_SEQUENTIAL;

	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_MERCEDES);
	strcpy(engineConfiguration->engineCode, "");

	/**
	 * Jimmy best tune
	 * https://rusefi.com/online/view.php?msq=626
	 * md_sanci latest tune
	 * https://rusefi.com/online/view.php?msq=630
	 */
	engineConfiguration->throttlePedalPositionAdcChannel = H144_IN_PPS;
	engineConfiguration->throttlePedalPositionSecondAdcChannel = EFI_ADC_14;
	engineConfiguration->throttlePedalUpVoltage = 1.49;
	engineConfiguration->throttlePedalWOTVoltage = 4.72;
	engineConfiguration->throttlePedalSecondaryUpVoltage = 1.34;
	engineConfiguration->throttlePedalSecondaryWOTVoltage = 4.24;

	engineConfiguration->vrThreshold[0].pin = Gpio::D14;
	hellenWbo();
}
