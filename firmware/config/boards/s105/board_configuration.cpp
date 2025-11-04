#include "pch.h"
#include "board_overrides.h"

/**
 * @brief   Board-specific configuration defaults.

 */

/* NOTE:
 * All outputs are in open drain mode as pull-up to 5V is
 * used as level shifter for MOSFETs/IGBTs */

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = Gpio::D8;
	engineConfiguration->injectionPins[1] = Gpio::D9;
	engineConfiguration->injectionPins[2] = Gpio::D10;
	engineConfiguration->injectionPins[3] = Gpio::D11;

	engineConfiguration->injectionPinMode = OM_OPENDRAIN;
}

Gpio getWarningLedPin() {
	// this board has no warning led
	return Gpio::Unassigned;
}

Gpio getCommsLedPin() {
	// this board has no comms led
	return Gpio::Unassigned;
}

Gpio getRunningLedPin() {
	return Gpio::Unassigned;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = Gpio::D13;
	engineConfiguration->ignitionPins[1] = Gpio::D12;
	engineConfiguration->ignitionPins[2] = Gpio::D15;
	engineConfiguration->ignitionPins[3] = Gpio::D14;

	engineConfiguration->ignitionPinMode = OM_OPENDRAIN;
}

static void s105_boardDefaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();

    strcpy(engineConfiguration->engineMake, "UAZ");
    strcpy(engineConfiguration->engineCode, "451");

	// No LEDs on this board, do we need some unused pin tp toggle? PC14 of LSI did not work for blinking, any better alternatives?

    engineConfiguration->malfunctionIndicatorPin = Gpio::E14;
	engineConfiguration->malfunctionIndicatorPinMode = OM_OPENDRAIN;
	engineConfiguration->tachOutputPin = Gpio::B8;	/* not populated by default */
	engineConfiguration->tachOutputPinMode = OM_OPENDRAIN;
	//engineConfiguration->idle.solenoidPin = ?
	engineConfiguration->idle.solenoidPinMode = OM_OPENDRAIN;
	engineConfiguration->fanPin = Gpio::D6;			/* not populated by default */
	engineConfiguration->fanPinMode = OM_OPENDRAIN;
	//engineConfiguration->mainRelayPin = ?;
	engineConfiguration->mainRelayPinMode = OM_OPENDRAIN;

	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_2_4_3;

	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

    engineConfiguration->verboseCan = true;

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_1;
	engineConfiguration->mafAdcChannel = EFI_ADC_11;
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_0;
	engineConfiguration->afr.hwChannel = EFI_ADC_NONE;
	engineConfiguration->clt.adcChannel = EFI_ADC_3;
	engineConfiguration->iat.adcChannel = EFI_ADC_14;
	engineConfiguration->triggerInputPins[0] = Gpio::B10;
}

static void setupVbatt() {
	// There are two different input dividers:
	// 22K (high) + 22K (low) on IAT and CLT
	// 22K (high) + 41.2K (low) on TPS and MAP
	engineConfiguration->analogInputDividerCoefficient = (22.0 + 22.0) / 22.0;
	// TODO: TPS and MAP
	//engineConfiguration->analogInputDividerCoefficient = (22.0 + 41.2) / 22.0;

	// 68K + 6.8k
	engineConfiguration->vbattDividerCoeff = (68 + 6.8) / 6.8;

	// pin input +12 from Main Relay
	engineConfiguration->vbattAdcChannel = EFI_ADC_9;

	engineConfiguration->adcVcc = 3.3f;
}

static void s105_boardConfigOverrides() {
	setupVbatt();

	//1.5K
	engineConfiguration->clt.config.bias_resistor = 1500;
	engineConfiguration->iat.config.bias_resistor = 1500;

	//CAN 1 bus overwrites
	engineConfiguration->canRxPin = Gpio::A11;
	engineConfiguration->canTxPin = Gpio::A12;
}

float getAnalogInputDividerCoefficient(adc_channel_e hwChannel) {
	switch (hwChannel) {
		/* MAP, TPS, MAF (na), AC press (na) */
		case EFI_ADC_0:
		case EFI_ADC_1:
		case EFI_ADC_11:
		case EFI_ADC_6:
			return (22.0 + 41.2) / 41.2;

		/* IAT, CLT */
		case EFI_ADC_3:
		case EFI_ADC_14:
			/* no divider, 1.5K pull-up to 3.3V, 22K pull-down */
			/* TODO: handle both pull-up and pull-down */
			return 1.0;

		/* AFR 1, AFR 2 (na), knock (TBD) */
		case EFI_ADC_7:
		case EFI_ADC_5:
		case EFI_ADC_2:
			/* no divider */
			return 1.0;

		/* +12 sense, Ignition switch */
		case EFI_ADC_9:
		case EFI_ADC_8:
			return (68.0 + 6.8) / 6.8;

		/* 5V feedback */
		case EFI_ADC_10:
			return (10.0 + 10.0) / 10.0;

		/* Ignition feedback */
		case EFI_ADC_12:
		case EFI_ADC_13:
			/* TBD */
			return 1.0;

		default:
			return engineConfiguration->analogInputDividerCoefficient;
	}
	return engineConfiguration->analogInputDividerCoefficient;
}

void setup_custom_board_overrides() {
	custom_board_DefaultConfiguration = s105_boardDefaultConfiguration;
	custom_board_ConfigOverrides = s105_boardConfigOverrides;
}
