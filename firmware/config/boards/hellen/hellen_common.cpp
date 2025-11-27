#include "pch.h"
#include "hellen_meta.h"
#include "adc_subscription.h"
#include "mmc_card.h"
#include "storage.h"

void hellenWbo() {
	engineConfiguration->enableAemXSeries = true;
}

// same for MM100, 144 and 176 - but different for 64 see 'setHellen64Can'
void setHellenCan() {
	engineConfiguration->canTxPin = H176_CAN_TX;
	engineConfiguration->canRxPin = H176_CAN_RX;
}

// same for lqfp64
void setHellenCan2() {
	engineConfiguration->can2RxPin = Gpio::B12;
	engineConfiguration->can2TxPin = Gpio::B13;
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

void setHellenAnalogDividers() {
	// 4.7k high side/4.7k low side = 2.0 ratio divider
	engineConfiguration->analogInputDividerCoefficient = 2.0f;

	// set vbatt_divider 5.835
	// 33k / 6.8k
	engineConfiguration->vbattDividerCoeff = (33 + 6.8) / 6.8; // 5.835
	engineConfiguration->adcVcc = 3.3f;
}

void setHellenVbatt() {
  setHellenAnalogDividers();

	engineConfiguration->vbattAdcChannel = H144_IN_VBATT;

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

PUBLIC_API_WEAK void onHellenEnChange(int value) { UNUSED(value); }

// Board specific helper to enable SD card only
PUBLIC_API_WEAK bool onHellenSdChange(int value) {
	UNUSED(value);

	// most Hellen board have no separate SD card power control
	// return false and let MegaEn to be enabled
	return false;
}

#ifndef EFI_BOOTLOADER
Timer hellenEnPinStateChange;
#endif

static void setHellenEnValue(int value) {
  // todo: can we use 'megaEn.getLogicValue()' instead?
  static int currentHellenEnValue = -1;

	megaEn.setValue(value, /*isForce*/ true);
	if (currentHellenEnValue != value) {
	  currentHellenEnValue = value;
#ifndef EFI_BOOTLOADER
	  hellenEnPinStateChange.reset();
#endif
	}
	// todo: shall we move below callbacks into 'only-if-changed' conditional block?
	onHellenEnChange(value);
  AdcSubscription::ResetFilters();
}

void hellenEnableEn(const char *msg) {
  efiPrintf("Turning board ON [%s]", msg);
  setHellenEnValue(1);
#if EFI_STORAGE_MFS
  chThdSleepMilliseconds(1);
	storagRequestRegisterStorage(STORAGE_MFS_EXT_FLASH);
#endif
}

void hellenDisableEn(const char *msg) {
#if (EFI_FILE_LOGGING || EFI_CONFIGURATION_STORAGE) && EFI_PROD_CODE
	// un-mount before turning power off SD card
	// wait up to 1 second for SD card to become unmounted
	efiPrintf("Long poll for SD card unmount");
	int timeout = 1000;
	sdCardRequestMode(SD_MODE_UNMOUNT);
#if EFI_CONFIGURATION_STORAGE
	// safe to call, even board does not have EXT FLASH
	storagRequestUnregisterStorage(STORAGE_MFS_EXT_FLASH);
#endif // EFI_CONFIGURATION_STORAGE
	do {
		chThdSleepMilliseconds(10);
		if (sdCardGetCurrentMode() == SD_MODE_IDLE) {
			break;
		}
		timeout -= 10;
	} while (timeout > 0);
#if EFI_CONFIGURATION_STORAGE
	while (storageIsStorageRegistered(STORAGE_MFS_EXT_FLASH) && timeout > 0) {
		chThdSleepMilliseconds(10);
		timeout -= 10;
	}
#endif // EFI_CONFIGURATION_STORAGE
#endif
  efiPrintf("Turning board off [%s]", msg);
  hellenDisableEnSilently();
}

void hellenDisableEnSilently() {
	// this function is called from criticalShutdown() that may be called from hardFault handler
	// please no call to OS functions!
	setHellenEnValue(0);
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
		setHellenSdCardSpi1Hardware();
		// weird order of operations? i guess it does not really matter
		hellenMegaAccelerometerPreInitCS2Pin();
}

void hellenMegaModule() {
	setHellenVbatt();
	hellenMegaSdWithAccelerometer();
	setDefaultHellenAtPullUps();
}

void hellenMegaAccelerometerPreInitCS2Pin() {
#if EFI_ONBOARD_MEMS
    if (!accelerometerChipSelect.isInitialized()) {
	    accelerometerChipSelect.initPin("mm-CS2", Gpio::H_SPI1_CS2);
	    accelerometerChipSelect.setValue(1);
	}
#else
  criticalError("probably broken MEMS configuration?");
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

ObdCode boardGetAnalogDiagnostic()
{
#ifdef DIAG_5VP_PIN
	/* paranoid check */
	if (!isBrainPinValid(DIAG_5VP_PIN)) {
		/* Pin is not defined - return success */
		return ObdCode::None;
	}

	return efiReadPin(DIAG_5VP_PIN) ? ObdCode::None : ObdCode::OBD_ECM_VSS_OUTPUT_A_MALFUNCTION;
#else
	return ObdCode::None;
#endif
}

float getAnalogInputDividerCoefficient(adc_channel_e hwChannel) {
#if (HELLEN_BOARD_MM64 == TRUE)
	(void)hwChannel;
	if (0)
#endif
#if (HELLEN_BOARD_MM100 == TRUE)
	if (hwChannel == MM100_IN_CRANK_ANALOG) [[unlikely]]
#endif
#if (HELLEN_BOARD_MM144 == TRUE)
	if (hwChannel == H144_IN_CRANK_ANALOG) [[unlikely]]
#endif
#if (HELLEN_BOARD_MM176 == TRUE)
	if (hwChannel == MM176_IN_CRANK_ANALOG) [[unlikely]]
#endif
	{
		// (4.7K || 5.1K) + 4.7K divider
		// 4.7K || 5.1K == 2.445K
		return (4.7f + 2.445f) / 4.7f;
	}

	return engineConfiguration->analogInputDividerCoefficient;
}

#ifndef EFI_BOOTLOADER
bool boardSdCardEnable() {
	// on mega-module we manage SD card power supply
	if (getHellenBoardEnabled()) {
		return true;
	}

	// Board can enable SD card power without enabling WBOs
	if (onHellenSdChange(1)) {
		efiPrintf("    *** turning SD power ONLY ***");
		return true;
	}

	if (getTimeNowS() > 4 && !isIgnVoltage()) {
		// looks like vehicle is OFF and we are hooked to USB - turn on peripheral to get Mass Storage Device USB profile
		efiPrintf("    *** turning board ON to power SD card ***");
		hellenEnableEn();
		chThdSleepMilliseconds(200);

		//check state
		return getHellenBoardEnabled();
	}

	return false;
}
#endif // ! EFI_BOOTLOADER
