#include "global.h"

#if EFI_WIDEBAND_FIRMWARE_UPDATE

#include "ch.h"
#include "can_msg_tx.h"

// This file contains an array called build_wideband_noboot_bin
// This array contains the firmware image for the wideband contoller
#include "wideband_firmware/for_rusefi/wideband_image.h"

#define EVT_BOOTLOADER_ACK EVENT_MASK(0)

static thread_t* waitingBootloaderThread = nullptr;

void handleWidebandBootloaderAck() {
	auto t = waitingBootloaderThread;
	if (t) {
		chEvtSignal(t, EVT_BOOTLOADER_ACK);
	}
}

bool waitAck() {
	return chEvtWaitAnyTimeout(EVT_BOOTLOADER_ACK, TIME_MS2I(1000)) != 0;
}

void updateWidebandFirmware(Logging* logging) {
	// Clear any pending acks for this thread
	chEvtGetAndClearEvents(EVT_BOOTLOADER_ACK);

	// Send messages to the current thread when acks come in
	waitingBootloaderThread = chThdGetSelfX();

	scheduleMsg(logging, "***************************************");
	scheduleMsg(logging, "        WIDEBAND FIRMWARE UPDATE");
	scheduleMsg(logging, "***************************************");
	scheduleMsg(logging, "Wideband Update: Rebooting to bootloader...");

	// The first request will reboot the chip (if necessary), and the second one will enable bootloader mode
	// If the chip was already in bootloader (aka manual mode), then that's ok - the second request will
	// just be safely ignored (but acked)
	for (int i = 0; i < 2; i++) {
		{
			// Send bootloader entry command
			CanTxMessage m(0xEF0'0000, 0, true);
		}

		if (!waitAck()) {
			scheduleMsg(logging, "Wideband Update ERROR: Expected ACK from entry to bootloader, didn't get one.");
			return;
		}

		// Let the controller reboot (and show blinky lights for a second before the update begins)
		chThdSleepMilliseconds(200);
	}

	scheduleMsg(logging, "Wideband Update: in update mode, erasing flash...");

	{
		// Erase flash - opcode 1, magic value 0x5A5A
		CanTxMessage m(0xEF1'5A5A, 0, true);
	}

	if (!waitAck()) {
		scheduleMsg(logging, "Wideband Update ERROR: Expected ACK from flash erase command, didn't get one.");
		return;
	}

	size_t totalSize = sizeof(build_wideband_image_bin);

	scheduleMsg(logging, "Wideband Update: Flash erased! Sending %d bytes...", totalSize);

	// Send flash data 8 bytes at a time
	for (size_t i = 0; i < totalSize; i += 8) {
		{
			CanTxMessage m(0xEF2'0000 + i, 8, true);
			memcpy(&m[0], build_wideband_image_bin + i, 8);
		}

		if (!waitAck()) {
			scheduleMsg(logging, "Wideband Update ERROR: Expected ACK from data write, didn't get one.");
			return;
		}
	}

	scheduleMsg(logging, "Wideband Update: Update complete! Rebooting controller.");

	{
		// Reboot to firmware!
		CanTxMessage m(0xEF3'0000, 0, true);
	}

	waitAck();

	waitingBootloaderThread = nullptr;
}

#endif // EFI_WIDEBAND_FIRMWARE_UPDATE
