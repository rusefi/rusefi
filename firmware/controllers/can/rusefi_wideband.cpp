#include "pch.h"

/*
 * - we have many board with one or two on-board F0 WBO module
 * - we have a requirement to use external AEM controller instead of onboard module ('Genesis use-case')
 * - we also have requirement for CANbus ID conflict resolution in case of vehicle integration ('Camaro use-case')
 */

// do we use some sort of a custom bootloader protocol in rusEFI WBO?
// todo: should we move to any widely used protocol like OpenBLT or else?

#if EFI_CAN_SUPPORT

#include "ch.h"
#include "can_msg_tx.h"
#include "rusefi_wideband.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#include "wideband_firmware/for_rusefi/wideband_can.h"
#pragma GCC diagnostic pop

static size_t getWidebandBus() {
	return engineConfiguration->widebandOnSecondBus ? 1 : 0;
}

#define EVT_BOOTLOADER_ACK EVENT_MASK(0)

static thread_t* waitingBootloaderThread = nullptr;

void handleWidebandCan(const CANRxFrame& frame) {
	// Bootloader acks with address 0x727573 aka ascii "rus"
	if (CAN_EID(frame) != WB_ACK) {
		return;
	}

	if (frame.DLC == 0)
	{
		// Ack reply
		// Nop
	} else if (frame.DLC == 8)
	{
		// Ping reply
		#if EFI_TUNER_STUDIO
			auto data = reinterpret_cast<const wbo::PongData*>(&frame.data8[0]);

			engine->outputChannels.canReWidebandVersion = data->Version;
			engine->outputChannels.canReWidebandFwDay = data->day;
			engine->outputChannels.canReWidebandFwMon = data->month;
			engine->outputChannels.canReWidebandFwYear = data->year;
		#endif
	} else {
		// Unknown
		return;
	}

	// Wake thread in any case
	auto t = waitingBootloaderThread;
	if (t) {
		chEvtSignal(t, EVT_BOOTLOADER_ACK);
	}
}

bool waitAck(int timeout = 1000) {
	return chEvtWaitAnyTimeout(EVT_BOOTLOADER_ACK, TIME_MS2I(timeout)) != 0;
}

static void setStatus(can_wbo_re_status_e status)
{
#if EFI_TUNER_STUDIO
	engine->outputChannels.canReWidebandCmdStatus = static_cast<uint8_t>(status);
#endif
}

void setWidebandOffset(uint8_t hwIndex, uint8_t index) {
	size_t bus = getWidebandBus();

	setStatus(WBO_RE_BUSY);

	// Clear any pending acks for this thread
	chEvtGetAndClearEvents(EVT_BOOTLOADER_ACK);

	// Send messages to the current thread when acks come in
	waitingBootloaderThread = chThdGetSelfX();

	efiPrintf("***************************************");
	efiPrintf("          WIDEBAND INDEX SET");
	efiPrintf("***************************************");

	if (hwIndex == 0xff) {
		efiPrintf("Setting all connected widebands to index %d...", index);
		CanTxMessage m(CanCategory::WBO_SERVICE, WB_MSG_SET_INDEX, 1, bus, true);
		m[0] = index;
	} else {
		efiPrintf("Setting wideband with hwIndex %d to CAN index %d...", hwIndex, index);
		CanTxMessage m(CanCategory::WBO_SERVICE, WB_MSG_SET_INDEX, 2, bus, true);
		m[0] = index;
		m[1] = hwIndex;
	}

	if (!waitAck()) {
		efiPrintf("Wideband index set failed: no controller detected!");
		setStatus(WBO_RE_FAILED);
	} else {
		setStatus(WBO_RE_DONE);
	}

	waitingBootloaderThread = nullptr;
}

void pingWideband(uint8_t hwIndex) {
	size_t bus = getWidebandBus();

	setStatus(WBO_RE_BUSY);

#if EFI_TUNER_STUDIO
	engine->outputChannels.canReWidebandVersion = 0;
	engine->outputChannels.canReWidebandFwDay = 0;
	engine->outputChannels.canReWidebandFwMon = 0;
	engine->outputChannels.canReWidebandFwYear = 0;
#endif

	// Clear any pending acks for this thread
	chEvtGetAndClearEvents(EVT_BOOTLOADER_ACK);

	// Send messages to the current thread when acks come in
	waitingBootloaderThread = chThdGetSelfX();

	{
		// TODO: replace magic number with WB_MSG_PING after updating Wideband submodule
		CanTxMessage m(CanCategory::WBO_SERVICE, 0xEF6'0000, 1, bus, true);
		m[0] = hwIndex;
	}

	if (!waitAck()) {
		efiPrintf("Wideband ping failed: no controller detected!");
		setStatus(WBO_RE_FAILED);
	} else {
		setStatus(WBO_RE_DONE);
	}

	waitingBootloaderThread = nullptr;
}

void sendWidebandInfo() {
	CanTxMessage m(CanCategory::WBO_SERVICE, WB_MGS_ECU_STATUS, /*dlc*/2, getWidebandBus(), /*isExtended*/true);

	float vbatt = Sensor::getOrZero(SensorType::BatteryVoltage) * 10;

	m[0] = vbatt;

	// Offset 1 bit 0 = heater enable
	m[1] = engine->engineState.heaterControlEnabled ? 0x01 : 0x00;
}

#if EFI_WIDEBAND_FIRMWARE_UPDATE

// This file contains an array called build_wideband_noboot_bin
// This array contains the firmware image for the wideband contoller
#include "wideband_firmware/for_rusefi/wideband_image.h"

void updateWidebandFirmware(uint8_t hwIndex) {
	size_t bus = getWidebandBus();
	size_t totalSize = sizeof(build_wideband_image_bin);

	setStatus(WBO_RE_BUSY);

	// Clear any pending acks for this thread
	chEvtGetAndClearEvents(EVT_BOOTLOADER_ACK);

	// Send messages to the current thread when acks come in
	waitingBootloaderThread = chThdGetSelfX();

	efiPrintf("***************************************");
	efiPrintf("        WIDEBAND FIRMWARE UPDATE");
	efiPrintf("***************************************");
	if (hwIndex != 0xff) {
		efiPrintf("Wideband Update: Rebooting WBO hwIndex %d to bootloader...", hwIndex);
	} else {
		efiPrintf("Wideband Update: Rebooting any WBO to bootloader...");
	}
	// The first request will reboot the chip (if necessary), and the second one will enable bootloader mode
	// If the chip was already in bootloader (aka manual mode), then that's ok - the second request will
	// just be safely ignored (but acked)
	for (int i = 0; i < 2; i++) {
		// Send bootloader entry command
		// First packet will ask main FW to reboot to bootloader
		// Second will ask bootloader to stay in bootloader and wait for FW upload
		// First packet can be new format - individually addressed
		// Second one should have zero payload - bootloader expects no payload.
		if ((hwIndex != 0xff) && (i == 0)) {
			// New format - individually addressed
			CanTxMessage m(CanCategory::WBO_SERVICE, WB_BL_ENTER, 1, bus, true);
			m[0] = hwIndex;
		} else {
			CanTxMessage m(CanCategory::WBO_SERVICE, WB_BL_ENTER, 0, bus, true);
		}

		if (!waitAck()) {
			efiPrintf("Wideband Update ERROR: Expected ACK from entry to bootloader, didn't get %s.",
				i ? "second (from bootloader)" : "first (from app)");
			setStatus(WBO_RE_FAILED);
			goto exit;
		}

		// Let the controller reboot (and show blinky lights for a second before the update begins)
		chThdSleepMilliseconds(200);
	}

	efiPrintf("Wideband Update: in update mode, erasing flash...");

	{
		// Erase flash - opcode 1, magic value 0x5A5A
		// TODO: replace magic numer with WB_BL_ERASE when define is fixed in wideband_can.h, currently mess with shift and addition precedence
		//CanTxMessage m(CanCategory::WBO_SERVICE, WB_BL_ERASE, 0, bus, true);
		CanTxMessage m(CanCategory::WBO_SERVICE, 0xEF1'5A5A, 0, bus, true);
	}

	if (!waitAck()) {
		efiPrintf("Wideband Update ERROR: Expected ACK from flash erase command, didn't get one.");
		setStatus(WBO_RE_FAILED);
		goto exit;
	}

	efiPrintf("Wideband Update: Flash erased! Sending %d bytes...", totalSize);

	// Send flash data 8 bytes at a time
	for (size_t i = 0; i < totalSize; i += 8) {
		{
			CanTxMessage m(CanCategory::WBO_SERVICE, WB_BL_DATA_BASE | i, 8, bus, true);
			memcpy(&m[0], build_wideband_image_bin + i, 8);
		}

		if (!waitAck()) {
			efiPrintf("Wideband Update ERROR: Expected ACK from data write, didn't get one.");
			setStatus(WBO_RE_FAILED);
			goto exit;
		}
	}

	efiPrintf("Wideband Update: Update complete! Rebooting controller.");

	{
		// Reboot to firmware!
		CanTxMessage m(CanCategory::WBO_SERVICE, WB_BL_REBOOT, 0, bus, true);
	}

	// TODO:
	waitAck();

	setStatus(WBO_RE_DONE);

exit:
	waitingBootloaderThread = nullptr;
}

#endif // EFI_WIDEBAND_FIRMWARE_UPDATE
#endif // HAL_USE_CAN
