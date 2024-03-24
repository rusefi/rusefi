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

static void init5vpDiag() {
#ifdef DIAG_5VP_PIN
static bool is5vpInit = false;
  if (!is5vpInit) {
    efiSetPadMode("5VP_STATE", DIAG_5VP_PIN, PAL_MODE_INPUT);
    is5vpInit = true;
  }
#endif // DIAG_5VP_PIN
}

void setHellenVbatt() {
	// 4.7k high side/4.7k low side = 2.0 ratio divider
	engineConfiguration->analogInputDividerCoefficient = 2.0f;

	// set vbatt_divider 5.835
	// 33k / 6.8k
	engineConfiguration->vbattDividerCoeff = (33 + 6.8) / 6.8; // 5.835

	engineConfiguration->vbattAdcChannel = H144_IN_VBATT;

	engineConfiguration->adcVcc = 3.29f;

  init5vpDiag(); // piggy back on popular 'setHellenVbatt' method
}

void setHellen64Can() {
	engineConfiguration->canTxPin = Gpio::B9;
	engineConfiguration->canRxPin = Gpio::B8;
}

static OutputPin megaEn;

bool boardEnableSendWidebandInfo() {
  // when board is powered down we should be more CANbus silent
    return megaEn.getLogicValue();
}

bool fansDisabledByBoardStatus() {
  return !megaEn.getLogicValue();
}

void hellenEnableEn() {
	    megaEn.setValue(1);
}

void hellenDisableEn() {
	    megaEn.setValue(0);
}

void setHellenEnPin(Gpio pin) {
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
	    megaEn.initPin("EN", pin);
	    hellenEnableEn();
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
    hellenDisableEn();
    // todo: 200ms is totally random what's the science for this sleep duration?
    chThdSleepMilliseconds(200);
}

/**
 * We need to make sure that accelerometer device which physically exists does not conflict with SD card
 * in case of shared SPI.
 * We reply on specific order of execution here:
 * 1) accelerometer pre-initialization into safe CS pin state
 * 2) SD card initialization
 * 3) accelerometer main initialization if accelerometer feature is desired
 */
extern OutputPin accelerometerChipSelect;

void hellenMegaAccelerometerPreInitCS2Pin() {
#if EFI_ONBOARD_MEMS
    if (!accelerometerChipSelect.isInitialized()) {
	    accelerometerChipSelect.initPin("mm-CS2", Gpio::H_SPI1_CS2);
	    accelerometerChipSelect.setValue(1);
	}
#endif // EFI_ONBOARD_MEMS
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
