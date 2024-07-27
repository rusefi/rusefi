#include "pch.h"
#include "hellen_meta.h"
#include "adc_subscription.h"

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

// newer Hellen boards with megamodule have power management for SD cards etc, older Hellen board do not have that
PUBLIC_API_WEAK bool isBoardWithPowerManagement() {
  return false;
}

bool getHellenBoardEnabled() {
  return !isBoardWithPowerManagement() || megaEn.getLogicValue();
}

bool boardEnableSendWidebandInfo() {
  // when board is powered down we should be more CANbus silent
    return getHellenBoardEnabled();
}

static bool hellenEnPinInitialized = false;

/*PUBLIC_API_WEAK*/ bool fansDisabledByBoardStatus() {
  return !getHellenBoardEnabled();
}

void hellenEnableEn(const char *msg) {
  efiPrintf("Turning board ON [%s]", msg);
	    megaEn.setValue(1, /*isForce*/ true);
  AdcSubscription::ResetFilters();
}

void hellenDisableEn(const char *msg) {
  efiPrintf("Turning board off [%s]", msg);
	    megaEn.setValue(0, /*isForce*/ true);
  AdcSubscription::ResetFilters();
}

void setHellenEnPin(Gpio pin, bool enableBoardOnStartUp) {
    if (!hellenEnPinInitialized) {
        hellenEnPinInitialized = true;
	    megaEn.initPin("EN", pin);
	    if (enableBoardOnStartUp) {
	      hellenEnableEn("start-up");
	    }
	}
}

void setHellenMegaEnPin(bool enableBoardOnStartUp) {
    // H144_GP8 matches MM100_GP8 which is used as PWR_EN on early mm100
    setHellenEnPin(H144_GP8, enableBoardOnStartUp); // OUT_PWR_EN
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

void hellenMegaSdWithAccelerometer() {
		setHellenSdCardSpi1();
		// weird order of operations? i guess it does not really matter
		hellenMegaAccelerometerPreInitCS2Pin();
}

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

void setupTLE9201(Gpio controlPin, Gpio direction, Gpio disable, int dcIndex) {
	// TLE9201 driver
	// This chip has three control pins:
	// DIR - sets direction of the motor
	// PWM - pwm control (enable high, coast low)
	// DIS - disables motor (enable low)

	// PWM pin
	engineConfiguration->etbIo[0].controlPin = controlPin;
	// DIR pin
	engineConfiguration->etbIo[0].directionPin1 = direction;
	// Disable pin
	engineConfiguration->etbIo[0].disablePin = disable;

	// we only have pwm/dir, no dira/dirb
	engineConfiguration->etb_use_two_wires = false;
}
