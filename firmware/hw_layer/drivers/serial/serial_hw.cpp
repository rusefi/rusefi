/**
 * @file	serial_hw.cpp
 * @brief	SERIAL bus low level code
 *
 * @date Apr 17, 2020
 * @author Konstantin Smola, (c) 2020
 */

#include "pch.h"

#if EFI_AUX_SERIAL

#include "serial.h"
#include "serial_hw.h"
#include "string.h"
#include "mpu_util.h"

static bool isSerialEnabled = false;
static bool isSerialTXEnabled = false;
static bool isSerialRXEnabled = false;

static SerialConfig uartCfg;
static SerialRead serialRead;

static void auxInfo() {
	if (!isSerialEnabled) {
		efiPrintf("AUX Serial is not enabled, please enable & restart");
		return;
	}

	efiPrintf("AUX Serial TX %s", hwPortname(engineConfiguration->auxSerialTxPin));
	efiPrintf("AUX Serial RX %s", hwPortname(engineConfiguration->auxSerialRxPin));
}

void enableAuxSerial() {
	engineConfiguration->auxSerialTxPin = engineConfiguration->auxSerialTxPin;
	engineConfiguration->auxSerialRxPin = engineConfiguration->auxSerialRxPin;
	engineConfiguration->auxSerialSpeed = engineConfiguration->auxSerialSpeed;
	
	uartCfg.speed = engineConfiguration->auxSerialSpeed;
	sdStart(AUX_SERIAL_DEVICE, &uartCfg);

	efiPrintf("AUX Serial started");
}

void stopAuxSerialPins() {
	efiSetPadUnused(activeConfiguration.auxSerialTxPin);
	efiSetPadUnused(activeConfiguration.auxSerialRxPin);
}

void startAuxSerialPins() {
	if (engineConfiguration->auxSerialTxPin)
		efiSetPadMode("AuxSerial TX", engineConfiguration->auxSerialTxPin, PAL_MODE_ALTERNATE(8));
	if (engineConfiguration->auxSerialRxPin)
		efiSetPadMode("AuxSerial RX", engineConfiguration->auxSerialRxPin, PAL_MODE_ALTERNATE(8));

	enableAuxSerial();
}

void initAuxSerial(void) {
	addConsoleAction("auxinfo", auxInfo);

	isSerialEnabled =
		(engineConfiguration->auxSerialTxPin) || // we need at least one pin set
		(engineConfiguration->auxSerialRxPin);

	isSerialRXEnabled = engineConfiguration->auxSerialRxPin;
	isSerialTXEnabled = engineConfiguration->auxSerialTxPin;

	// exit if no pin is configured
	if (!isSerialEnabled)
		return;

	// Validate pins 
	if (isSerialTXEnabled && !isValidSerialTxPin(engineConfiguration->auxSerialTxPin)) {
		firmwareError(OBD_PCM_Processor_Fault, "unexpected aux TX pin");
		return;
	}

	if (isSerialRXEnabled && !isValidSerialRxPin(engineConfiguration->auxSerialRxPin)) {
		firmwareError(OBD_PCM_Processor_Fault, "unexpected aux RX pin");
		return;
	}

	startAuxSerialPins();

	if (isSerialRXEnabled)
		serialRead.Start();
}

#endif // EFI_AUX_SERIAL
