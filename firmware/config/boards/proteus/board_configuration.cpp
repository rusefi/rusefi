/**
 * @file boards/proteus/board_configuration.cpp
 *
 * @brief Configuration defaults for the Proteus board
 *
 * @author Matthew Kennedy, (c) 2019
 */

#include "pch.h"
#include "proteus_meta.h"

static const brain_pin_e injPins[] = {
	PROTEUS_LS_1,
	PROTEUS_LS_2,
	PROTEUS_LS_3,
	PROTEUS_LS_4,
	PROTEUS_LS_5,
	PROTEUS_LS_6,
	PROTEUS_LS_7,
	PROTEUS_LS_8,
	PROTEUS_LS_9,
	PROTEUS_LS_10,
	PROTEUS_LS_11,
	PROTEUS_LS_12
};

static const brain_pin_e ignPins[] = {
	PROTEUS_IGN_1,
	PROTEUS_IGN_2,
	PROTEUS_IGN_3,
	PROTEUS_IGN_4,
	PROTEUS_IGN_5,
	PROTEUS_IGN_6,
	PROTEUS_IGN_7,
	PROTEUS_IGN_8,
	PROTEUS_IGN_9,
	PROTEUS_IGN_10,
	PROTEUS_IGN_11,
	PROTEUS_IGN_12,
};

static void setInjectorPins() {
	copyArray(engineConfiguration->injectionPins, injPins);
	engineConfiguration->injectionPinMode = OM_DEFAULT;
}

static void setIgnitionPins() {
	copyArray(engineConfiguration->ignitionPins, ignPins);
	engineConfiguration->ignitionPinMode = OM_DEFAULT;
}

static void setLedPins() {
	// PE3 is error LED, configured in board.mk
	engineConfiguration->communicationLedPin = Gpio::E4;
	engineConfiguration->runningLedPin = Gpio::E5;
	engineConfiguration->warningLedPin = Gpio::E6;
}

static void setupVbatt() {
	// 5.6k high side/10k low side = 1.56 ratio divider
	engineConfiguration->analogInputDividerCoefficient = 1.56f;

	// 82k high side/10k low side = 9.2
	engineConfiguration->vbattDividerCoeff = (92.0f / 10.0f);

	// Battery sense on PA7
	engineConfiguration->vbattAdcChannel = EFI_ADC_7;

	engineConfiguration->adcVcc = 3.3f;
}

static void setupEtb() {
	// TLE9201 driver
	// This chip has three control pins:
	// DIR - sets direction of the motor
	// PWM - pwm control (enable high, coast low)
	// DIS - disables motor (enable low)

	// Throttle #1
	// PWM pin
	engineConfiguration->etbIo[0].controlPin = Gpio::D12;
	// DIR pin
	engineConfiguration->etbIo[0].directionPin1 = Gpio::D10;
	// Disable pin
	engineConfiguration->etbIo[0].disablePin = Gpio::D11;
	// Unused
	engineConfiguration->etbIo[0].directionPin2 = Gpio::Unassigned;

	// Throttle #2
	// PWM pin
	engineConfiguration->etbIo[1].controlPin = Gpio::D13;
	// DIR pin
	engineConfiguration->etbIo[1].directionPin1 = Gpio::D9;
	// Disable pin
	engineConfiguration->etbIo[1].disablePin = Gpio::D8;
	// Unused
	engineConfiguration->etbIo[1].directionPin2 = Gpio::Unassigned;

	// we only have pwm/dir, no dira/dirb
	engineConfiguration->etb_use_two_wires = false;
}

static void setupDefaultSensorInputs() {
	// trigger inputs
#if VR_HW_CHECK_MODE
	// set_trigger_input_pin 0 PE7
	engineConfiguration->triggerInputPins[0] = PROTEUS_VR_1;
	engineConfiguration->camInputs[0] = PROTEUS_VR_2;
#else
	// Digital channel 1 as default - others not set
	engineConfiguration->triggerInputPins[0] = PROTEUS_DIGITAL_1;
	engineConfiguration->camInputs[0] = Gpio::Unassigned;
#endif

	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;


	engineConfiguration->clt.adcChannel = PROTEUS_IN_CLT;
	engineConfiguration->iat.adcChannel = PROTEUS_IN_IAT;
	engineConfiguration->tps1_1AdcChannel = PROTEUS_IN_TPS;
	engineConfiguration->map.sensor.hwChannel = PROTEUS_IN_MAP;

	// pin #28 WBO AFR "Analog Volt 10"
	engineConfiguration->afr.hwChannel = EFI_ADC_5;
}

static void setupSdCard() {
	engineConfiguration->sdCardSpiDevice = SPI_DEVICE_3;
	engineConfiguration->sdCardCsPin = Gpio::D2;

	engineConfiguration->is_enabled_spi_3 = true;
	engineConfiguration->spi3sckPin = Gpio::C10;
	engineConfiguration->spi3misoPin = Gpio::C11;
	engineConfiguration->spi3mosiPin = Gpio::C12;
}

void setBoardConfigOverrides() {
	setupSdCard();
	setLedPins();
	setupVbatt();

	engineConfiguration->clt.config.bias_resistor = 2700;
	engineConfiguration->iat.config.bias_resistor = 2700;

	engineConfiguration->canTxPin = Gpio::D1;
	engineConfiguration->canRxPin = Gpio::D0;

#if defined(STM32F4) || defined(STM32F7)
	engineConfiguration->can2RxPin = Gpio::B12;
	engineConfiguration->can2TxPin = Gpio::B13;
#endif

	engineConfiguration->lps25BaroSensorScl = Gpio::B10;
	engineConfiguration->lps25BaroSensorSda = Gpio::B11;
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
	setupEtb();

	engineConfiguration->isSdCardEnabled = true;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	engineConfiguration->specs.cylindersCount = 8;
	engineConfiguration->specs.firingOrder = FO_1_8_7_2_6_5_4_3;

	engineConfiguration->enableSoftwareKnock = true;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SIMULTANEOUS;

#if HW_PROTEUS & EFI_PROD_CODE
	engineConfiguration->mainRelayPin = PROTEUS_LS_13;
	engineConfiguration->fanPin = PROTEUS_LS_15;
	engineConfiguration->fuelPumpPin = PROTEUS_LS_16;
#endif // HW_PROTEUS

	// If we're running as hardware CI, borrow a few extra pins for that
#ifdef HARDWARE_CI
	engineConfiguration->triggerSimulatorPins[0] = Gpio::G3;
	engineConfiguration->triggerSimulatorPins[1] = Gpio::G2;
#endif
}

void boardPrepareForStop() {
	// Wake on the CAN RX pin
	palEnableLineEvent(PAL_LINE(GPIOD, 0), PAL_EVENT_MODE_RISING_EDGE);
}
