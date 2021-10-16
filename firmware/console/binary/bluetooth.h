/**
 * @file	bluetooth.h
 * @file Bluethoot modules hardware initialization
 *
 * @date Aug 26, 2013
 * @author andreika, (c) 2017
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once
#include "global.h"
#include "tunerstudio_io.h"


// The Bluetooth setup procedure will wait 10 seconds for the user to disconnect the UART cable.
// This is required because the BT setup procedure reads a response from the module during the communication.
// Thus any bytes sent from the Console Software may interfere with the procedure.
#define BLUETOOTH_COMMAND_TIMEOUT TIME_MS2I(10000)

// Supported Bluetooth module types
typedef enum {
	BLUETOOTH_HC_05,
	BLUETOOTH_HC_06,
	BLUETOOTH_SPP,
} bluetooth_module_e;

/**
 * Start Bluetooth module initialization using UART connection:
 * - wait for PC communication disconnect;
 * - reconfigure the UART;
 * - send AT-commands to the module;
 * - restore connection to PC.
 */
void bluetoothStart(SerialTsChannelBase *btChannel, bluetooth_module_e moduleType, const char *baudRate, const char *name, const char *pinCode);

/**
 * Cancel Bluetooth procedure
 */
void bluetoothCancel(void);

/**
 * Called by runBinaryProtocolLoop() if a connection disconnect is detected.
 * Bluetooth init code needs to make sure that there's no interference of the BT module and USB-UART (connected to PC)
 */
void bluetoothSoftwareDisconnectNotify();

