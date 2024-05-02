/**
 * @file	bluetooth.cpp
 *
 *
 * It looks like Bluetooth modules arrive in all kinds of initial configuration.
 * Sometimes we need to execute a one-time initialization including settings the baud rate. rusEFI setting uartConsoleSerialSpeed or tunerStudioSerialSpeed
 * has to match BT module configuration.
 *
 *
 * @author andreika, (c) 2017
 */

#include "pch.h"

#include "tunerstudio.h"

#include "tunerstudio_io.h"
#include "bluetooth.h"

#include <stdio.h>
#include <ctype.h>

#if EFI_BLUETOOTH_SETUP

#ifndef EFI_BLUETOOTH_SETUP_DEBUG
#define EFI_BLUETOOTH_SETUP_DEBUG TRUE
#endif

static volatile bool btSetupIsRequested = false;

bluetooth_module_e btModuleType;
static int setBaudIdx = -1;
static char btName[20 + 1];
static char btPinCode[4 + 1];

// JDY-33 has 9: 128000 which we do not
static const struct {
	uint32_t rate;
	uint8_t code;
} baudRates[] = {
	//most popular first
	{115200, 8},
	{9600, 4},
	{38400, 6},
	{2400, 2},
	{4800, 3},
	{19200, 5},
	{57600, 7}
};

static const int btModuleTimeout = TIME_MS2I(2500);

static void btWrite(TsChannelBase* tsChannel, const char *str)
{
	/* Just a wrapper for debug purposes */
#if EFI_BLUETOOTH_SETUP_DEBUG
	efiPrintf("sending %s", str);
#endif
	tsChannel->write((uint8_t *)str, strlen(str));
}

static int btReadLine(TsChannelBase* tsChannel, char *str, size_t max_len) {
	size_t len = 0;

	/* read until end of line */
	do {
		if (len >= max_len) {
			efiPrintf("BT reply is unexpectedly long");
			return -1;
		}
		if (tsChannel->readTimeout((uint8_t *)&str[len], 1, btModuleTimeout) != 1) {
			efiPrintf("Timeout waiting for BT reply after %d byte(s)", len);
			return -1;
		}
	} while (str[len++] != '\n');

	/* termination */
	if (len < max_len)
		str[len] = 0;
	else
		str[max_len - 1] = 0;

#if EFI_BLUETOOTH_SETUP_DEBUG
	if (len) {
		efiPrintf("Received %d %s", len, str);
	}
#endif

	return len;
}

static int btWaitOk(SerialTsChannelBase* tsChannel) {
	int len;
	int ret = -1;
	char tmp[16];

	/* wait for '+OK\r\n' */
	len = btReadLine(tsChannel, tmp, sizeof(tmp));
	if (len == 5) {
		if (strncmp(tmp, "+OK", 3) == 0)
			ret = 0;
	}

	return ret;
}

static int btVerOk(SerialTsChannelBase* tsChannel)
{
	int len;
	int ret = -1;
	char tmp[38];

	/* wait for resposne  */
	len = btReadLine(tsChannel, tmp, sizeof(tmp));
	if ((len >= 36) && (len <= 40)) {
			ret = 0;
	}

	return ret;
}

// Main communication code
// We assume that the user has disconnected the software before starting the code.
static void runCommands(SerialTsChannelBase* tsChannel) {
	char tmp[64];
	size_t baudIdx = 0;
	bool baudFound = false;

	// find current baudrate
	while (baudFound == false) {
		tsChannel->stop();
		chThdSleepMilliseconds(10);	// safety

		if (baudIdx == efi::size(baudRates)) {
			efiPrintf("Failed to find current BT module baudrate");
			tsChannel->start(engineConfiguration->tunerStudioSerialSpeed);
			return;
		}

		efiPrintf("Restarting at %d", baudRates[baudIdx].rate);
		tsChannel->start(baudRates[baudIdx].rate);
		chThdSleepMilliseconds(10);	// safety

		/* Ping BT module */
		btWrite(tsChannel, "AT\r\n");
		if (btWaitOk(tsChannel) == 0) {
			baudFound = true;
		} else if (btModuleType == BLUETOOTH_JDY_3x) {
			/* try to disconnect in case device already configured and in silence mode */
			btWrite(tsChannel, "AT+DISC\r\n");
			if (btWaitOk(tsChannel) == 0) {
				efiPrintf("JDY33 disconnected");
				chThdSleepMilliseconds(10);	// safety
				baudFound = true;
			}
		} else if (btModuleType == BLUETOOTH_JDY_31) {
			/* try to disconnect in case device already configured and in silence mode */
			btWrite(tsChannel, "AT+VERSION\r\n");
			if (btVerOk(tsChannel) == 0) {
				efiPrintf("JDY31 disconnected");
				chThdSleepMilliseconds(10);	// safety
				baudFound = true;
			}
		}

		/* else try next baudrate */
		baudIdx++;
	}

	if (btModuleType == BLUETOOTH_JDY_3x) {
#if EFI_BLUETOOTH_SETUP_DEBUG
		/* Debug, get version, current settings */
		btWrite(tsChannel, "AT+VERSION\r\n");
		btReadLine(tsChannel, tmp, sizeof(tmp));

		btWrite(tsChannel, "AT+BAUD\r\n");
		btReadLine(tsChannel, tmp, sizeof(tmp));

		btWrite(tsChannel, "AT+TYPE\r\n");
		btReadLine(tsChannel, tmp, sizeof(tmp));

		btWrite(tsChannel, "AT+PIN\r\n");
		btReadLine(tsChannel, tmp, sizeof(tmp));

		btWrite(tsChannel, "AT+LADDR\r\n");
		btReadLine(tsChannel, tmp, sizeof(tmp));

		btWrite(tsChannel, "AT+STAT\r\n");
		btReadLine(tsChannel, tmp, sizeof(tmp));
#endif

		/* JDY33 specific settings */
		/* Reset to defaults */
		btWrite(tsChannel, "AT+DEFAULT\r\n");
		btWaitOk(tsChannel);

		/* No serial port status output */
		btWrite(tsChannel, "AT+ENLOG0\r\n");
		btWaitOk(tsChannel);

		/* SPP connection with no password */
		btWrite(tsChannel, "AT+TYPE0\r\n");
		btWaitOk(tsChannel);
	}

  if (btModuleType == BLUETOOTH_JDY_31) {
#if EFI_BLUETOOTH_SETUP_DEBUG
		/* Debug, get version, current settings */
		btWrite(tsChannel, "AT+VERSION\r\n");
		btReadLine(tsChannel, tmp, sizeof(tmp));

		btWrite(tsChannel, "AT+BAUD\r\n");
		btReadLine(tsChannel, tmp, sizeof(tmp));

		btWrite(tsChannel, "AT+PIN\r\n");
		btReadLine(tsChannel, tmp, sizeof(tmp));

		btWrite(tsChannel, "AT+LADDR\r\n");
		btReadLine(tsChannel, tmp, sizeof(tmp));


#endif

		/* JDY31 specific settings */
		/* Reset to defaults */
		btWrite(tsChannel, "AT+DEFAULT\r\n");
		btWaitOk(tsChannel);

		/* No serial port status output */
		btWrite(tsChannel, "AT+ENLOG0\r\n");
		btWaitOk(tsChannel);

	}


	/* restart with new baud */
	tsChannel->stop();
	chThdSleepMilliseconds(10);	// safety
	tsChannel->start(baudRates[setBaudIdx].rate);

	if (btModuleType == BLUETOOTH_HC_05)
		chsnprintf(tmp, sizeof(tmp), "AT+NAME=%s\r\n", btName);
	else
		chsnprintf(tmp, sizeof(tmp), "AT+NAME%s\r\n", btName);
	btWrite(tsChannel, tmp);
	if (btWaitOk(tsChannel) != 0) {
		goto cmdFailed;
	}
	if (btModuleType == BLUETOOTH_JDY_3x) {
		/* BLE broadcast name */
		chsnprintf(tmp, sizeof(tmp), "AT+NAMB%s-BLE\r\n", btName);
		btWrite(tsChannel, tmp);
		if (btWaitOk(tsChannel) != 0) {
			goto cmdFailed;
		}
	}

	if (btModuleType == BLUETOOTH_HC_05)
		chsnprintf(tmp, sizeof(tmp), "AT+PSWD=%s\r\n", btPinCode);
	else
		chsnprintf(tmp, sizeof(tmp), "AT+PIN%s\r\n", btPinCode);

	btWrite(tsChannel, tmp);
	if (btWaitOk(tsChannel) != 0) {
		goto cmdFailed;
	}

  if (btModuleType == BLUETOOTH_HC_05)
		chsnprintf(tmp, sizeof(tmp), "AT+UART=%d,0,0\r\n", baudRates[setBaudIdx].rate);	// baud rate, 0=(1 stop bit), 0=(no parity bits)
	else
		chsnprintf(tmp, sizeof(tmp), "AT+BAUD%d\r\n", baudRates[setBaudIdx].code);
	btWrite(tsChannel, tmp);
	if (btWaitOk(tsChannel) != 0) {
		goto cmdFailed;
	}
  
	if (btModuleType == BLUETOOTH_JDY_3x ) {
		/* Now reset module to apply new settings */
		btWrite(tsChannel, "AT+RESET\r\n");
		if (btWaitOk(tsChannel) != 0) {
			efiPrintf("JDY3x failed to reset");
		}
	}

  if (btModuleType == BLUETOOTH_JDY_31 ) {
		/* Now reset module to apply new settings */
		btWrite(tsChannel, "AT+RESET\r\n");
		if (btWaitOk(tsChannel) != 0) {
			efiPrintf("JDY3x failed to reset");
		}
	}

	efiPrintf("SUCCESS! All commands passed to the Bluetooth module!");
	return;

cmdFailed:
	efiPrintf("FAIL! Command %s failed", tmp);
}

void bluetoothStart(bluetooth_module_e moduleType, const char *baudRate, const char *name, const char *pinCode) {
	static const char *usage = "Usage: bluetooth_<hc05/hc06/bk/jdy> <baud> <name> <pincode>";

	if ((baudRate == nullptr) || (name == nullptr) || (pinCode == nullptr)) {
		efiPrintf("%s", usage);
		return;
	}

	if (getBluetoothChannel() == nullptr) {
		efiPrintf("This firmware does not support bluetooth [%s]", getTsSignature());
		return;
	}

	if (btSetupIsRequested) {
		efiPrintf("The Bluetooth module init procedure is already started!");
		return;
	}

	// now check the arguments and add other commands:
	// 1) baud rate
	int baud = atoi(baudRate);
	// find a known baud rate in our list
	setBaudIdx = -1;
	for (size_t i = 0; i < efi::size(baudRates); i++) {
		if ((int)baudRates[i].rate == baud) {
			setBaudIdx = i;
			break;
		}
	}
	// check the baud rate index
	if (setBaudIdx < 0) {
		// unknown baud rate
		efiPrintf("Wrong <baud> parameter '%s'! %s", baudRate, usage);
		return;
	}

	// 2) check name
	if ((strlen(name) < 1) || (strlen(name) > 20)) {
		efiPrintf("Wrong <name> parameter! Up to 20 characters expected! %s", usage);
		return;
	}

	// 3) check pin code
	if (strlen(pinCode) != 4) {
		efiPrintf("Wrong <pincode> parameter! 4 digits expected! %s", usage);
		return;
	}
	for (int i = 0; i < 4; i++) {
		if (!isdigit(pinCode[i])) {
			efiPrintf("<pincode> should contain digits only %s", usage);
			return;
		}
	}

	/* copy settings */
	strncpy(btName, name, 20);
	strncpy(btPinCode, pinCode, 4);

	btModuleType = moduleType;
	btSetupIsRequested = true;
}

// Called after 1S of silence on BT UART...
void bluetoothSoftwareDisconnectNotify(SerialTsChannelBase* tsChannel) {
	if (btSetupIsRequested) {
		efiPrintf("*** Bluetooth module setup procedure ***");

		/* JDY33 supports disconnect on request */
		if (btModuleType != BLUETOOTH_JDY_3x ) {
			efiPrintf("!Warning! Please make sure you're not currently using the BT module for communication (not paired)!");
			efiPrintf("TO START THE PROCEDURE: PLEASE DISCONNECT YOUR PC COM-PORT FROM THE BOARD NOW!");
			efiPrintf("After that please don't turn off the board power and wait for ~15 seconds to complete. Then reconnect to the board!");
		}

    /* JDY31 supports disconnect on request */
		if (btModuleType != BLUETOOTH_JDY_31 ) {
			efiPrintf("!Warning! Please make sure you're not currently using the BT module for communication (not paired)!");
			efiPrintf("TO START THE PROCEDURE: PLEASE DISCONNECT YOUR PC COM-PORT FROM THE BOARD NOW!");
			efiPrintf("After that please don't turn off the board power and wait for ~15 seconds to complete. Then reconnect to the board!");
		}

		uint8_t tmp[1];
		if (tsChannel->readTimeout(tmp, 1, BLUETOOTH_SILENT_TIMEOUT) != 0) {
			efiPrintf("The Bluetooth module init procedure is cancelled (wait for silent timeout)!");
			btSetupIsRequested = false;
			return;
		}

		runCommands(tsChannel);
		btSetupIsRequested = false;
	}
}

#endif /* EFI_BLUETOOTH_SETUP */
