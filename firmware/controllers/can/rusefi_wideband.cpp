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

size_t getWidebandBus() {
	return engineConfiguration->widebandOnSecondBus ? 1 : 0;
}

#define EVT_BOOTLOADER_ACK EVENT_MASK(0)

static thread_t* waitingBootloaderThread = nullptr;
static char widebandRestart = 0;

void handleWidebandCan(const size_t busIndex, const CANRxFrame& frame) {
	// wrong bus
	if (busIndex != getWidebandBus()) {
		return;
	}

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

void setWidebandOffsetNoWait(uint8_t hwIndex, uint8_t index) {
	size_t bus = getWidebandBus();

	if (hwIndex == 0xff) {
		CanTxMessage m(CanCategory::WBO_SERVICE, WB_MSG_SET_INDEX, 1, bus, true);
		m[0] = index;
	} else {
		CanTxMessage m(CanCategory::WBO_SERVICE, WB_MSG_SET_INDEX, 2, bus, true);
		m[0] = index;
		m[1] = hwIndex;
	}
}

void setWidebandOffset(uint8_t hwIndex, uint8_t index) {
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
	} else {
		efiPrintf("Setting wideband with hwIndex %d to CAN index %d...", hwIndex, index);
	}
	setWidebandOffsetNoWait(hwIndex, index);

	if (!waitAck()) {
		efiPrintf("Wideband index set failed: no controller detected!");
		setStatus(WBO_RE_FAILED);
	} else {
		setStatus(WBO_RE_DONE);
	}

	waitingBootloaderThread = nullptr;
}

void setWidebandSensorType(uint8_t hwIndex, uint8_t type) {
	size_t bus = getWidebandBus();

	setStatus(WBO_RE_BUSY);

	// Clear any pending acks for this thread
	chEvtGetAndClearEvents(EVT_BOOTLOADER_ACK);

	// Send messages to the current thread when acks come in
	waitingBootloaderThread = chThdGetSelfX();

	{
		// Note position of hwIndex, compared to WB_MSG_SET_INDEX
		// TODO: replace madic number after updating WBO submodule
		CanTxMessage m(CanCategory::WBO_SERVICE, 0xEF7'0000 /* WB_MSG_SET_SENS_TYPE */, 2, bus, true);
		m[0] = hwIndex;
		m[1] = type;
	}

	if (!waitAck()) {
		efiPrintf("Wideband sensor type set failed: no controller detected!");
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

	// 25mS should be enought, lets do not block TS thread too long while waiting for WBO reply
	if (!waitAck(25)) {
		efiPrintf("Wideband ping failed: no controller detected!");
		setStatus(WBO_RE_FAILED);
	} else {
		efiPrintf("WBO_RE_DONE");
		setStatus(WBO_RE_DONE);
	}

	waitingBootloaderThread = nullptr;
}

void restartWideband() {
	// 10 times of 50 mS
	widebandRestart = 10;
}

// Called with 50mS interval and only if CAN and (on-boards) WBO(s) are enabled
void sendWidebandInfo() {
	static int counter = 0;

	CanTxMessage m(CanCategory::WBO_SERVICE, WB_MSG_ECU_STATUS, /*dlc*/2, getWidebandBus(), /*isExtended*/true);

	float vbatt = Sensor::getOrZero(SensorType::BatteryVoltage) * 10;

	m[0] = vbatt;

	// Offset 1 bit 0 = heater enable
	if (widebandRestart) {
		m[1] = 0x00;
		widebandRestart--;
	} else {
		m[1] = engine->engineState.heaterControlEnabled ? 0x01 : 0x00;
	}

	// 10 * 50 = 0.5S delay
	if (counter == 10) {
		for (size_t i = 0; i < CAN_WBO_COUNT; i++) {
			if ((engineConfiguration->canWbo[i].enableRemap) &&
				(engineConfiguration->canWbo[i].type == RUSEFI)) {
				// remap
				setWidebandOffsetNoWait(engineConfiguration->canWbo[i].reHwidx, engineConfiguration->canWbo[i].reId);
			}
		}
	}

	counter++;
}

#if EFI_WIDEBAND_FIRMWARE_UPDATE

// reboots to bootloader and erases FW area
static int updateWidebandFirmwarePrepare(size_t bus, uint8_t hwIndex) {
	int ret = -1;

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

	ret = 0;

exit:
	return ret;
}


// reboots to bootloader and erases FW area
static int updateWidebandFirmwareFinalize(size_t bus, uint8_t /* hwIndex */) {
	efiPrintf("Wideband Update: Update complete! Rebooting controller.");

	{
		// Reboot to firmware!
		CanTxMessage m(CanCategory::WBO_SERVICE, WB_BL_REBOOT, 0, bus, true);
	}

	if (!waitAck()) {
		efiPrintf("Wideband Update ERROR: No ack on reboot command");
		waitingBootloaderThread = nullptr;
		return -1;
	}

	return 0;
}

#if EFI_PROD_CODE

#if EFI_FILE_LOGGING
#include "ff.h"
#endif

// Yes, this file also contains bootloader, but this is how it was historicaly released in 2023
static const char wboFileName[] = "fw/wbo/wideband_image_with_bl.bin";
// FW goes after 6K bootloader
static const size_t wboFwOffset = 6 * 1024;
static const size_t wboFwMaxSize = 32 * 1024;

void updateWidebandFirmwareFromFile(uint8_t hwIndex)
{
#if EFI_FILE_LOGGING
	int ret = 0;
	size_t totalSize = 0;
	size_t bus = 0;
	FIL fil;

	if (f_open(&fil, wboFileName, FA_READ) != FR_OK) {
		efiPrintf("Wideband Update: filed to open %s", wboFileName);
		return;
	}

	if (f_lseek(&fil, wboFwOffset) != FR_OK) {
		efiPrintf("Wideband Update: filed to seek");
		goto exit;
	}

	totalSize = f_size(&fil) - wboFwOffset;
	bus = getWidebandBus();

	// this also sets waitingBootloaderThread
	ret = updateWidebandFirmwarePrepare(bus, hwIndex);
	if (ret < 0) {
		goto exit;
	}

	for (size_t i = 0; i < totalSize; i += 8) {
		UINT br;
		uint8_t tmp[8];

		if (f_read(&fil, tmp, sizeof(tmp), &br) != FR_OK) {
			efiPrintf("Wideband Update: filed to read");
			setStatus(WBO_RE_FAILED);
			goto exit;
		}

		if (br == 0) {
			// EOF
			break;
		}

		{
			CanTxMessage m(CanCategory::WBO_SERVICE, WB_BL_DATA_BASE | i, 8, bus, true);
			memcpy(&m[0], tmp, 8);
		}

		if (!waitAck()) {
			efiPrintf("Wideband Update ERROR: Expected ACK from data write, didn't get one.");
			setStatus(WBO_RE_FAILED);
			goto exit;
		}
	}

	ret = updateWidebandFirmwareFinalize(bus, hwIndex);
	if (ret) {
		goto exit;
	}

	setStatus(WBO_RE_DONE);

exit:
	waitingBootloaderThread = nullptr;
	f_close(&fil);
#else
	// Not supported
#endif
}
#endif

// This file contains an array called build_wideband_noboot_bin
// This array contains the firmware image for the wideband contoller
#include "wideband_firmware/for_rusefi/wideband_image.h"

void updateWidebandFirmware(uint8_t hwIndex) {
	size_t bus = getWidebandBus();
	size_t totalSize = sizeof(build_wideband_image_bin);

	// this also sets waitingBootloaderThread
	int ret = updateWidebandFirmwarePrepare(bus, hwIndex);
	if (ret < 0) {
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

	ret = updateWidebandFirmwareFinalize(bus, hwIndex);
	if (ret) {
		goto exit;
	}

	setStatus(WBO_RE_DONE);

exit:
	waitingBootloaderThread = nullptr;
}

#endif // EFI_WIDEBAND_FIRMWARE_UPDATE
#endif // HAL_USE_CAN
