#include "pch.h"
#include "hellen_meta.h"

void hellenWbo() {
	engineConfiguration->enableAemXSeries = true;
}

// same for MM100, 144 and 176
void setHellenCan() {
	engineConfiguration->canTxPin = H176_CAN_TX;
	engineConfiguration->canRxPin = H176_CAN_RX;
}

void setHellenVbatt() {
	// 4.7k high side/4.7k low side = 2.0 ratio divider
	engineConfiguration->analogInputDividerCoefficient = 2.0f;

	// set vbatt_divider 5.835
	// 33k / 6.8k
	engineConfiguration->vbattDividerCoeff = (33 + 6.8) / 6.8; // 5.835

	engineConfiguration->vbattAdcChannel = H144_IN_VBATT;

	engineConfiguration->adcVcc = 3.29f;
}

void setHellen64Can() {
	engineConfiguration->canTxPin = Gpio::B9;
	engineConfiguration->canRxPin = Gpio::B8;
}

static OutputPin megaEn;

void setHellenEnPin(Gpio pin) {
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
	    megaEn.initPin("EN", pin);
	    megaEn.setValue(1);
	}
}

void setHellenMegaEnPin() {
    // H144_GP8 matches MM100_GP8 is used as PWR_EN on early mm100
    setHellenEnPin(H144_GP8); // OUT_PWR_EN
}

void setHellen64MegaEnPin() {
    setHellenEnPin(Gpio::MM64_GP1); // OUT_PWR_EN
}

void hellenBoardStandBy() {
    // we need to turn 'megaEn' and pause for a bit to make sure that WBO is off and does not wake main firmware right away
    megaEn.setValue(0);
    // todo: 200ms is totally random what's the science for this sleep duration?
    chThdSleepMilliseconds(200);
}

/**
 * dirty hack
 */
void configureHellenMegaAccCS2Pin() {
    static bool initialized = false;
    static OutputPin cs2pin;
    if (!initialized) {
        initialized = true;
	    cs2pin.initPin("mm-CS2", Gpio::H_SPI1_CS2);
	    cs2pin.setValue(1);
	}
}

void configureHellenCanTerminator() {
    static bool initialized = false;
    static OutputPin terminatorControlPin;
    if (!initialized) {
        initialized = true;
	    terminatorControlPin.initPin("CAN-term", H_SPI3_CS); // todo: make this pin configurable
	    terminatorControlPin.setValue(engineConfiguration->boardUseCanTerminator);
	}
}

void detectHellenBoardType() {
#ifndef EFI_BOOTLOADER
	engine->engineState.hellenBoardId = hackHellenBoardId(detectHellenBoardId());
#endif /* EFI_BOOTLOADER */
}
