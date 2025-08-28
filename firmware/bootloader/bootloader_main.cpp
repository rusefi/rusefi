
#include "pch.h"
#include "usbconsole.h"
#include "hardware.h"

#ifdef HW_HELLEN
#include "hellen_all_meta.h"
#endif // HW_HELLEN

extern "C" {
	#include "boot.h"
	#include "flash.h"
	#include "shared_params.h"
}

// used externaly by openblt_usb.cpp
blt_bool stayInBootloader;

static blt_bool waitedLongerThanTimeout = BLT_FALSE;
static blt_bool rebootLoop;
static blt_bool wdReset;

static const uint8_t maxWdRebootCounter = 10;

#if (BOOT_COP_HOOKS_ENABLE > 0)
extern "C" {
	void CopInitHook(void);
	void CopServiceHook(void);
}

// Functions for controlling the watchdog
void CopInitHook(void) {
	// Nothing to do...
}

void CopServiceHook(void) {
	// We need to reset WDT here
#if HAL_USE_WDG
	wdgResetI(&WDGD1);
#endif // HAL_USE_WDG
}
#endif // BOOT_COP_HOOKS_ENABLE

PUBLIC_API_WEAK bool OpenBltIsBoardOk()
{
	return true;
}

class BlinkyThread : public chibios_rt::BaseStaticThread<256> {
protected:
	void main(void) override {
		Gpio yellow = getWarningLedPin();
		Gpio blue = getCommsLedPin();
		Gpio green = getRunningLedPin();
		Gpio red = LED_CRITICAL_ERROR_BRAIN_PIN;

		efiSetPadMode("yellow", yellow, PAL_MODE_OUTPUT_PUSHPULL);
		efiSetPadMode("blue", blue, PAL_MODE_OUTPUT_PUSHPULL);
		efiSetPadMode("green", green, PAL_MODE_OUTPUT_PUSHPULL);
		efiSetPadMode("red", red, PAL_MODE_OUTPUT_PUSHPULL);

		auto yellowPort = getBrainPinPort(yellow);
		auto yellowPin = getBrainPinIndex(yellow);
		auto bluePort = getBrainPinPort(blue);
		auto bluePin = getBrainPinIndex(blue);
		auto greenPort = getBrainPinPort(green);
		auto greenPin = getBrainPinIndex(green);
		auto redPort = getBrainPinPort(red);
		auto redPin = getBrainPinIndex(red);

#ifdef BOOTLOADER_ENABLE_OUTPUT_PIN
		{
			ioportid_t en_port = getHwPort("blt-en-pin", BOOTLOADER_ENABLE_OUTPUT_PIN);
			uint8_t en_pin = getHwPin("blt-en-pin", BOOTLOADER_ENABLE_OUTPUT_PIN);
			palSetPadMode(en_port, en_pin, PAL_MODE_OUTPUT_PUSHPULL);
			palWritePad(en_port, en_pin, 1);
		}
#endif // BOOTLOADER_ENABLE_OUTPUT_PIN

#ifdef BOOTLOADER_ENABLE_OUTPUT_PIN2
		{
			ioportid_t en_port = getHwPort("blt-en-pin2", BOOTLOADER_ENABLE_OUTPUT_PIN2);
			uint8_t en_pin = getHwPin("blt-en-pin2", BOOTLOADER_ENABLE_OUTPUT_PIN2);
			palSetPadMode(en_port, en_pin, PAL_MODE_OUTPUT_PUSHPULL);
			palWritePad(en_port, en_pin, 1);
		}
#endif // BOOTLOADER_ENABLE_OUTPUT_PIN2

#ifdef BOOTLOADER_DISABLE_GREEN_LED
		greenPort = NULL;
#endif // BOOTLOADER_DISABLE_GREEN_LED

		if (yellowPort) {
			palSetPad(yellowPort, yellowPin);
		}
		if (bluePort) {
			palSetPad(bluePort, bluePin);
		}
		if (greenPort) {
			palSetPad(greenPort, greenPin);
		}
		if (redPort) {
			if (wdReset) {
				palClearPad(redPort, redPin);
			} else {
				palSetPad(redPort, redPin);
			}
		}

		while (true) {
			if (yellowPort) {
				palTogglePad(yellowPort, yellowPin);
			}
			if (bluePort && OpenBltIsBoardOk()) {
				palTogglePad(bluePort, bluePin);
			}
			if (greenPort) {
				palTogglePad(greenPort, greenPin);
			}
			if (redPort && !wdReset) {
				palTogglePad(redPort, redPin);
			}
			// blink 3 times faster if Dual Bank is not enabled
			auto delay = isFlashDualBank() ? 125 : 40;
			// blink faster if not in the waiting mode
			chThdSleepMilliseconds(waitedLongerThanTimeout ? (delay * 2) : delay);
		}
	}
};

static BlinkyThread blinky;

static blt_bool checkIfRebootIntoOpenBltRequested(void) {
	uint8_t value = 0x00;
	if (SharedParamsReadByIndex(0, &value) && (value == 0x01)) {
		/* clear */
		SharedParamsWriteByIndex(0, 0x00);
		return BLT_TRUE;
	}
	return BLT_FALSE;
}

static blt_bool checkIfResetLoop(void) {
	uint8_t wd_counter = 0;
	Reset_Cause_t resetCause = getMCUResetCause();
	if ((resetCause == Reset_Cause_IWatchdog) ||
		(resetCause == Reset_Cause_WWatchdog)) {
		// One of watchdogs
		SharedParamsReadByIndex(1, &wd_counter);
		wd_counter++;
		SharedParamsWriteByIndex(1, wd_counter);
		wdReset = BLT_TRUE;
	} else if ((resetCause == Reset_Cause_NRST_Pin) ||
			   (resetCause == Reset_Cause_POR)) {
		// power on or NRST reset
		// cleat WD counter
		SharedParamsWriteByIndex(1, wd_counter);
	}

	return (wd_counter > maxWdRebootCounter);
}

int main(void) {
	halInit();
	chSysInit();

	baseMCUInit();

	// Init openblt shared params
	SharedParamsInit();
	rebootLoop = checkIfResetLoop();
	stayInBootloader = checkIfRebootIntoOpenBltRequested() || rebootLoop;

	// start the blinky thread
	blinky.start(NORMALPRIO + 10);

	// Init openblt itself
	BootInit();

	blt_bool wasConnected = BLT_FALSE;

	while (true) {
		BootTask();

		// since BOOT_BACKDOOR_HOOKS_ENABLE==TRUE, BackDoorCheck() is not working
		// so we have to manually check if we need to jump to the main firmware
		if (ComIsConnected() == BLT_TRUE) {
			// remember we had connection attempt
			wasConnected = BLT_TRUE;
			continue;
		}
		if (stayInBootloader || wasConnected)
			continue;
#if (BOOT_BACKDOOR_ENTRY_TIMEOUT_MS > 0)
		blt_bool isTimeout = (TIME_I2MS(chVTGetSystemTime()) >= BOOT_BACKDOOR_ENTRY_TIMEOUT_MS);
#else
		blt_bool isTimeout = BLT_TRUE;
#endif // BOOT_BACKDOOR_ENTRY_TIMEOUT_MS
		if (isTimeout == BLT_TRUE) {
			waitedLongerThanTimeout = BLT_TRUE;
			CpuStartUserProgram();
		}
	}
}

// very basic version, supports on chip pins only (really only used for USB)
void efiSetPadMode(const char* msg, brain_pin_e brainPin, iomode_t mode) {
	ioportid_t port = getHwPort(msg, brainPin);
	ioportmask_t pin = getHwPin(msg, brainPin);
	/* paranoid */
	if (!port) {
		return;
	}

	palSetPadMode(port, pin, mode);
}
