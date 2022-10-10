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
#define EFI_BLUETOOTH_SETUP_DEBUG FALSE
#endif

static bool btProcessIsStarted = false;
static const char *commands[5];
static int numCommands = 0;
static int setBaudIdx = -1;

bluetooth_module_e btModuleType;
static char cmdBaud[25];
static char cmdName[30];
static char cmdPin[16];

// JDY-33 has 9: 128000 which we do not
static const unsigned int baudRates[] = 	{	115200, 9600, 	38400,	2400,	4800,	19200,	57600 };
static const unsigned int baudRateCodes[] = {	8,		4,		6,		2,		3,		5,		7 };
static const int btModuleTimeout = TIME_MS2I(500);

static SerialTsChannelBase *tsChannel;

static THD_WORKING_AREA(btThreadStack, UTILITY_THREAD_STACK_SIZE);
static thread_t *btThread = nullptr;
static thread_reference_t btThreadRef = nullptr; // used by thread suspend/resume as a flag

static void btWrite(const char *str)
{
	/* Just a wrapper for debug purposes */
#if EFI_BLUETOOTH_SETUP_DEBUG
	efiPrintf("sending %s", str);
#endif
	tsChannel->write((uint8_t *)str, strlen(str));
}

static int btReadLine(char *str, size_t max_len)
{
	size_t len = 0;

	/* read until end of line */
	do {
		if (len >= max_len) {
			efiPrintf("Too long reply from BT");
			return -1;
		}
		if (tsChannel->readTimeout((uint8_t *)&str[len], 1, btModuleTimeout) != 1) {
			efiPrintf("Timeout waiting for BT reply");
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

static int btWaitOk(void)
{
	int len;
	int ret = -1;
	char tmp[16];

	/* wait for '+OK\r\n' */
	len = btReadLine(tmp, sizeof(tmp));
	if (len == 5) {
		if (strncmp(tmp, "+OK", 3) == 0)
			ret = 0;
	}

	return ret;
}

// Main communication code
// We assume that the user has disconnected the software before starting the code.
static void runCommands() {
	size_t baudIdx = 0;
	bool baudFound = false;

	if (!btProcessIsStarted)
		return;
	
	efiPrintf("Sleeping...");
	chThdSleepMilliseconds(1000);	// safety

	// find current baudrate
	while (baudFound == false) {
		tsChannel->stop();
		chThdSleepMilliseconds(10);	// safety

		if (chThdShouldTerminateX() || (baudIdx == 7)) {
			if (baudIdx == 7)
				efiPrintf("Failed to find current BT module baudrate");
			tsChannel->start(engineConfiguration->tunerStudioSerialSpeed);
		}

		efiPrintf("Restarting at %d", baudRates[baudIdx]);
		tsChannel->start(baudRates[baudIdx]);
		chThdSleepMilliseconds(10);	// safety

		/* Ping BT module */
		btWrite("AT\r\n");
		if (btWaitOk() == 0) {
			baudFound = true;
		} else if (btModuleType == BLUETOOTH_JDY_3x) {
			/* try to diconnect in case device already configured and in silence mode */
			btWrite("AT+DISC\r\n");
			if (btWaitOk() == 0) {
				efiPrintf("JDY33 disconnected");
				chThdSleepMilliseconds(10);	// safety
				baudFound = true;
			}
		}

		/* else try next baudrate */
		baudIdx++;
	}

	if (btModuleType == BLUETOOTH_JDY_3x) {
#if EFI_BLUETOOTH_SETUP_DEBUG
		char tmp[64];

		/* Debug, get version, current settings */
		btWrite("AT+VERSION\r\n");
		btReadLine(tmp, sizeof(tmp));

		btWrite("AT+BAUD\r\n");
		btReadLine(tmp, sizeof(tmp));

		btWrite("AT+TYPE\r\n");
		btReadLine(tmp, sizeof(tmp));

		btWrite("AT+PIN\r\n");
		btReadLine(tmp, sizeof(tmp));
#endif

		/* JDY33 specific settings */
		/* Reset to defaults */
		btWrite("AT+DEFAULT\r\n");
		btWaitOk();

		/* No serial port status output */
		btWrite("AT+ENLOG0\r\n");
		btWaitOk();

		/* SPP connection with no password */
		btWrite("AT+TYPE0\r\n");
		btWaitOk();
	}

	int cmdIdx;
	// run all commands
	for (cmdIdx = 0; cmdIdx < numCommands; cmdIdx++) {
		int ret;
		const char * command = commands[cmdIdx];
		// send current command
		btWrite(command);

		// waiting for an answer
		ret = btWaitOk();
		if (ret == 0) {
			// if we changed the baud rate
			if (commands[cmdIdx] == cmdBaud) {
				tsChannel->stop();
				chThdSleepMilliseconds(10);	// safety
				tsChannel->start(baudRates[setBaudIdx]);
			}
			// move to the next command
		} else {
			efiPrintf("command %s failed %d", command, ret);
			break;
		}
	}

	// the connection is already restored to the current baud rate, so print the result
	if (cmdIdx == numCommands) {
		if (btModuleType == BLUETOOTH_JDY_3x) {
			/* Now reset module to apply new settings */
			btWrite("AT+RESET\r\n");
			if (btWaitOk() != 0) {
				efiPrintf("JDY33 fialed to reset");
			}
		}

		efiPrintf("SUCCESS! All commands (%d of %d) passed to the Bluetooth module!", cmdIdx, numCommands);
		// Update baudrate in setting
		engineConfiguration->tunerStudioSerialSpeed = baudRates[baudIdx];
	} else {
		efiPrintf("FAIL! Only %d commands (of %d total) were passed to the Bluetooth module!", cmdIdx, numCommands);
	}
}

static THD_FUNCTION(btThreadEntryPoint, arg) {
	(void) arg;
	chRegSetThreadName("bluetooth thread");

	efiPrintf("*** Bluetooth module setup procedure ***");

	/* JDY33 supports disconnect on request */
	if (btModuleType != BLUETOOTH_JDY_3x) {
		efiPrintf("!Warning! Please make sure you're not currently using the BT module for communication (not paired)!");
		efiPrintf("TO START THE PROCEDURE: PLEASE DISCONNECT YOUR PC COM-PORT FROM THE BOARD NOW!");
		efiPrintf("After that please don't turn off the board power and wait for ~15 seconds to complete. Then reconnect to the board!");
	}

	// now wait
	chSysLock();
	btProcessIsStarted = true;
	msg_t msg = chThdSuspendTimeoutS(&btThreadRef, BLUETOOTH_COMMAND_TIMEOUT);
	chSysUnlock();
	
	if (msg == MSG_TIMEOUT) {
		// timeout waiting for silence on uart...
		efiPrintf("The Bluetooth module init procedure is cancelled (timeout)!");
	} else {
		// call this when the thread is resumed (after the disconnect)
		runCommands();
	}

	// release the command
	btProcessIsStarted = false;
}

void bluetoothStart(bluetooth_module_e moduleType, const char *baudRate, const char *name, const char *pinCode) {
	static const char *usage = "Usage: bluetooth_<hc05/hc06/bk/jdy> <baud> <name> <pincode>";

	if ((baudRate == nullptr) || (name == nullptr) || (pinCode == nullptr)) {
		efiPrintf("%s", usage);
		return;
	}

	tsChannel = getBluetoothChannel();
	if (tsChannel == nullptr) {
		efiPrintf("No Bluetooth channel configured! Check your board config.");
		return;
	}

	if (btProcessIsStarted) {
		efiPrintf("The Bluetooth module init procedure is already started and waiting! To cancel it, run \"bluetooth_cancel\" command!");
		return;
	}

	numCommands = 0;

	// now check the arguments and add other commands:
	// 1) baud rate
	int baud = atoi(baudRate);
	// find a known baud rate in our list
	setBaudIdx = -1;
	for (int i = 0; baudRates[i] > 0; i++) {
		if (baudRates[i] == baud) {
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
	}
	for (int i = 0; i < 4; i++) {
		if (!isdigit(pinCode[i])) {
			efiPrintf("<pincode> should contain digits only %s", usage);
		}
	}

	btModuleType = moduleType;

	// ok, add commands!
	switch (moduleType) {
	case BLUETOOTH_HC_05:
		chsnprintf(cmdBaud, sizeof(cmdBaud), "AT+UART=%d,0,0\r\n", baud);	// baud rate, 0=(1 stop bit), 0=(no parity bits)
		chsnprintf(cmdName, sizeof(cmdName), "AT+NAME=%s\r\n", name);
		chsnprintf(cmdPin, sizeof(cmdPin), "AT+PSWD=%s\r\n", pinCode);
		// todo: add more commands?
		// AT+RMAAD
		// AT+ROLE=0
		break;
	case BLUETOOTH_HC_06:
		chsnprintf(cmdBaud, sizeof(cmdBaud), "AT+BAUD%d", baudRateCodes[setBaudIdx]);
		chsnprintf(cmdName, sizeof(cmdName), "AT+NAME%s", name);
		chsnprintf(cmdPin, sizeof(cmdPin), "AT+PIN%s", pinCode);
		break;
	case BLUETOOTH_BK3231:
		chsnprintf(cmdBaud, sizeof(cmdBaud), "AT+BAUD%d\r\n", baudRateCodes[setBaudIdx]);
		chsnprintf(cmdName, sizeof(cmdName), "AT+NAME%s\r\n", name);
		chsnprintf(cmdPin, sizeof(cmdPin), "AT+PIN%s\r\n", pinCode);
		break;
	case BLUETOOTH_JDY_3x:
		chsnprintf(cmdBaud, sizeof(cmdBaud), "AT+BAUD%d\r\n", baudRateCodes[setBaudIdx]);
		chsnprintf(cmdName, sizeof(cmdName), "AT+NAME%s\r\n", name);
		chsnprintf(cmdPin, sizeof(cmdPin), "AT+PIN%s\r\n", pinCode);
		break;
	default:
		// todo: add support for other BT module types
		efiPrintf("This Bluetooth module is currently not supported!");
		return;
	}
	commands[numCommands++] = cmdBaud;
	commands[numCommands++] = cmdName;
   	commands[numCommands++] = cmdPin;

	// create a thread to execute these commands after TS disconnected
	// See bluetoothSoftwareDisconnectNotify
	btThread = chThdCreateStatic(btThreadStack, sizeof(btThreadStack), PRIO_CONSOLE, (tfunc_t)btThreadEntryPoint, NULL);
}

// Called after 1S of silence on BT UART...
void bluetoothSoftwareDisconnectNotify() {
	if (btProcessIsStarted) {
		// start communication with the module
		chThdResume(&btThreadRef, MSG_OK);
		// wait the thread to finish
		chThdWait(btThread);
	}
}

void bluetoothCancel() {
	if (!btProcessIsStarted) {
		efiPrintf("The Bluetooth module init procedure was not started! Nothing to cancel!");
		return;
	}
	
	// terminate thread
	chThdTerminate(btThread);
	
	btProcessIsStarted = false;
	efiPrintf("The Bluetooth module init procedure is cancelled!");
}

#endif /* EFI_BLUETOOTH_SETUP */
