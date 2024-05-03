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
    Gpio::PROTEUS_LS_1,
	Gpio::PROTEUS_LS_2,
	Gpio::PROTEUS_LS_3,
	Gpio::PROTEUS_LS_4,
	Gpio::PROTEUS_LS_5,
	Gpio::PROTEUS_LS_6,
	Gpio::PROTEUS_LS_7,
	Gpio::PROTEUS_LS_8,
	Gpio::PROTEUS_LS_9,
	Gpio::PROTEUS_LS_10,
	Gpio::PROTEUS_LS_11,
	Gpio::PROTEUS_LS_12
};

static const brain_pin_e ignPins[] = {
	Gpio::PROTEUS_IGN_1,
	Gpio::PROTEUS_IGN_2,
	Gpio::PROTEUS_IGN_3,
	Gpio::PROTEUS_IGN_4,
	Gpio::PROTEUS_IGN_5,
	Gpio::PROTEUS_IGN_6,
	Gpio::PROTEUS_IGN_7,
	Gpio::PROTEUS_IGN_8,
	Gpio::PROTEUS_IGN_9,
	Gpio::PROTEUS_IGN_10,
	Gpio::PROTEUS_IGN_11,
	Gpio::PROTEUS_IGN_12,
};

static void setInjectorPins() {
	copyArray(engineConfiguration->injectionPins, injPins);
}

static void setIgnitionPins() {
	copyArray(engineConfiguration->ignitionPins, ignPins);
}

// PE3 is error LED, configured in board.mk
Gpio getCommsLedPin() {
	return Gpio::E4;
}

Gpio getRunningLedPin() {
	return Gpio::E5;
}

Gpio getWarningLedPin() {
	return Gpio::E6;
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

	// Throttle #2
	// PWM pin
	engineConfiguration->etbIo[1].controlPin = Gpio::D13;
	// DIR pin
	engineConfiguration->etbIo[1].directionPin1 = Gpio::D9;
	// Disable pin
	engineConfiguration->etbIo[1].disablePin = Gpio::D8;

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

    // see also enableAemXSeries
	// pin #28 WBO AFR "Analog Volt 10"
	engineConfiguration->afr.hwChannel = PROTEUS_IN_ANALOG_VOLT_10;
}

static void setupSdCard() {
	engineConfiguration->sdCardSpiDevice = SPI_DEVICE_3;
	engineConfiguration->sdCardCsPin = Gpio::D2;

	engineConfiguration->is_enabled_spi_3 = true;
	engineConfiguration->spi3sckPin = Gpio::C10;
	engineConfiguration->spi3misoPin = Gpio::C11;
	engineConfiguration->spi3mosiPin = Gpio::C12;

	engineConfiguration->is_enabled_spi_5 = true;
	engineConfiguration->spi5sckPin = Gpio::F7;
	engineConfiguration->spi5misoPin = Gpio::F8;
	engineConfiguration->spi5mosiPin = Gpio::F9;
}

void setBoardConfigOverrides() {
	setupSdCard();
	setupVbatt();

	engineConfiguration->clt.config.bias_resistor = PROTEUS_DEFAULT_AT_PULLUP;
	engineConfiguration->iat.config.bias_resistor = PROTEUS_DEFAULT_AT_PULLUP;

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

 */
void setBoardDefaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();
	setupEtb();

	engineConfiguration->isSdCardEnabled = true;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	engineConfiguration->enableSoftwareKnock = true;

#if HW_PROTEUS & EFI_PROD_CODE
	engineConfiguration->mainRelayPin = Gpio::PROTEUS_LS_12;
	engineConfiguration->fanPin = Gpio::PROTEUS_LS_11;
	engineConfiguration->fuelPumpPin = Gpio::PROTEUS_LS_10;
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

#if HW_PROTEUS
static Gpio PROTEUS_ME17_ADAPTER_OUTPUTS[] = {
    Gpio::PROTEUS_LS_1,
};

static Gpio PROTEUS_SBC_OUTPUTS[] = {
    Gpio::PROTEUS_LS_14, // inj 1 four times
    Gpio::PROTEUS_LS_14, // inj 1 four times
    Gpio::PROTEUS_LS_14, // inj 1 four times
    Gpio::PROTEUS_LS_14, // inj 1 four times

    Gpio::PROTEUS_LS_15, // inj 4 four times
    Gpio::PROTEUS_LS_15, // inj 4 four times
    Gpio::PROTEUS_LS_15, // inj 4 four times
    Gpio::PROTEUS_LS_15, // inj 4 four times

};

static Gpio PROTEUS_M73_OUTPUTS[] = {
    Gpio::PROTEUS_LS_1, // inj 1
    Gpio::PROTEUS_LS_2, // inj 2
    Gpio::PROTEUS_LS_3,
    Gpio::PROTEUS_LS_4,
    Gpio::PROTEUS_LS_5,
    Gpio::PROTEUS_LS_6,
    Gpio::PROTEUS_LS_7,
    Gpio::PROTEUS_LS_8,
    Gpio::PROTEUS_LS_9, // inj 9
    Gpio::PROTEUS_LS_10, // inj 3
};

static Gpio PROTEUS_CANAM_OUTPUTS[] = {
    Gpio::PROTEUS_LS_1, // inj 1
    Gpio::PROTEUS_LS_2, // inj 2
    Gpio::PROTEUS_LS_3, // inj 3
    Gpio::PROTEUS_LS_12, // main relay
    Gpio::PROTEUS_LS_14, // starter
    Gpio::PROTEUS_LS_15, // intercooler fan
    Gpio::PROTEUS_LS_4, // accessories relay
	Gpio::PROTEUS_IGN_1,
	Gpio::PROTEUS_IGN_2,
	Gpio::PROTEUS_IGN_3,
};

static Gpio PROTEUS_HARLEY_OUTPUTS[] = {
    Gpio::PROTEUS_LS_1,
    Gpio::PROTEUS_LS_2,
	Gpio::PROTEUS_IGN_1,
	Gpio::PROTEUS_IGN_2,
	Gpio::PROTEUS_IGN_8, // ACR
	Gpio::PROTEUS_IGN_9, // ACR2
};

int getBoardMetaLowSideOutputsCount() {
    if (engineConfiguration->engineType == engine_type_e::MAVERICK_X3) {
        return getBoardMetaOutputsCount();
    }
    if (engineConfiguration->engineType == engine_type_e::HARLEY) {
        return getBoardMetaOutputsCount();
    }
    if (engineConfiguration->engineType == engine_type_e::GM_SBC) {
        return getBoardMetaOutputsCount();
    }
    return 16;
}

static Gpio PROTEUS_OUTPUTS[] = {
Gpio::PROTEUS_LS_1,
Gpio::PROTEUS_LS_2,
Gpio::PROTEUS_LS_3,
Gpio::PROTEUS_LS_4,
Gpio::PROTEUS_LS_5,
Gpio::PROTEUS_LS_6,
Gpio::PROTEUS_LS_7,
Gpio::PROTEUS_LS_8,
Gpio::PROTEUS_LS_9,
Gpio::PROTEUS_LS_10,
Gpio::PROTEUS_LS_11,
Gpio::PROTEUS_LS_12,
Gpio::PROTEUS_LS_13,
Gpio::PROTEUS_LS_14,
Gpio::PROTEUS_LS_15,
Gpio::PROTEUS_LS_16,
	Gpio::PROTEUS_IGN_1,
	Gpio::PROTEUS_IGN_2,
	Gpio::PROTEUS_IGN_3,
	Gpio::PROTEUS_IGN_4,
	Gpio::PROTEUS_IGN_5,
	Gpio::PROTEUS_IGN_6,
	Gpio::PROTEUS_IGN_7,
	Gpio::PROTEUS_IGN_8,
	Gpio::PROTEUS_IGN_9,
	Gpio::PROTEUS_IGN_10,
	Gpio::PROTEUS_IGN_11,
	Gpio::PROTEUS_IGN_12,
	Gpio::PROTEUS_HS_1,
	Gpio::PROTEUS_HS_2,
	Gpio::PROTEUS_HS_3,
	Gpio::PROTEUS_HS_4
};

int getBoardMetaOutputsCount() {
    if (engineConfiguration->engineType == engine_type_e::MAVERICK_X3) {
        return efi::size(PROTEUS_CANAM_OUTPUTS);
    }
    if (engineConfiguration->engineType == engine_type_e::ME17_9_MISC) {
        return efi::size(PROTEUS_ME17_ADAPTER_OUTPUTS);
    }
    if (engineConfiguration->engineType == engine_type_e::HARLEY) {
        return efi::size(PROTEUS_HARLEY_OUTPUTS);
    }
    if (engineConfiguration->engineType == engine_type_e::GM_SBC) {
        return efi::size(PROTEUS_SBC_OUTPUTS);
    }
    if (engineConfiguration->engineType == engine_type_e::PROTEUS_BMW_M73) {
        return efi::size(PROTEUS_M73_OUTPUTS);
    }
    return efi::size(PROTEUS_OUTPUTS);
}

int getBoardMetaDcOutputsCount() {
    if (engineConfiguration->engineType == engine_type_e::PROTEUS_BMW_M73) {
        return 2;
    }
    if (engineConfiguration->engineType == engine_type_e::ME17_9_MISC ||
        engineConfiguration->engineType == engine_type_e::HARLEY ||
        engineConfiguration->engineType == engine_type_e::MAVERICK_X3
        ) {
        return 1;
    }
    return 1;
/*    return 2; proteus has two h-b ridges but stim board is short on channels to test :( */
}

Gpio* getBoardMetaOutputs() {
    if (engineConfiguration->engineType == engine_type_e::MAVERICK_X3) {
        return PROTEUS_CANAM_OUTPUTS;
    }
    if (engineConfiguration->engineType == engine_type_e::HARLEY) {
        return PROTEUS_HARLEY_OUTPUTS;
    }
    if (engineConfiguration->engineType == engine_type_e::GM_SBC) {
        return PROTEUS_SBC_OUTPUTS;
    }
    if (engineConfiguration->engineType == engine_type_e::PROTEUS_BMW_M73) {
        return PROTEUS_M73_OUTPUTS;
    }
    return PROTEUS_OUTPUTS;
}
#endif // HW_PROTEUS
