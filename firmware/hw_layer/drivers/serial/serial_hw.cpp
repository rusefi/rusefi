/**
 * @file	serial_hw.cpp
 * @brief	SERIAL bus low level code
 *
 * @date Apr 17, 2020
 * @author Konstantin Smola, (c) 2020
 */

#include "global.h"

#if EFI_AUX_SERIAL

#include "serial.h"
#include "engine_configuration.h"
#include "pin_repository.h"
#include "serial_hw.h"
#include "string.h"
#include "mpu_util.h"
#include "engine.h"

static bool isSerialEnabled = false;
static bool isSerialTXEnabled = false;
static bool isSerialRXEnabled = false;

static SerialConfig uartCfg;
static SerialRead serialRead;

static void auxInfo(void) {
	if (!isSerialEnabled) {
		efiPrintf("AUX Serial is not enabled, please enable & restart");
		return;
	}

	efiPrintf("AUX Serial TX %s", hwPortname(CONFIG(auxSerialTxPin)));
	efiPrintf("AUX Serial RX %s", hwPortname(CONFIG(auxSerialRxPin)));
}

void enableAuxSerial(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	engineConfiguration->auxSerialTxPin = CONFIG(auxSerialTxPin);
	engineConfiguration->auxSerialRxPin = CONFIG(auxSerialRxPin);
	engineConfiguration->auxSerialSpeed = CONFIG(auxSerialSpeed);
	
	uartCfg.speed = engineConfiguration->auxSerialSpeed;
	sdStart(AUX_SERIAL_DEVICE, &uartCfg);

	efiPrintf("AUX Serial started");
}

void stopAuxSerialPins(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	efiSetPadUnused(activeConfiguration.auxSerialTxPin);
	efiSetPadUnused(activeConfiguration.auxSerialRxPin);
}

void startAuxSerialPins(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (CONFIG(auxSerialTxPin))
		efiSetPadMode("AuxSerial TX", CONFIG(auxSerialTxPin), PAL_MODE_ALTERNATE(8));
	if (CONFIG(auxSerialRxPin))
		efiSetPadMode("AuxSerial RX", CONFIG(auxSerialRxPin), PAL_MODE_ALTERNATE(8));

	enableAuxSerial();
}

void initAuxSerial(void) {
	addConsoleAction("auxinfo", auxInfo);

	isSerialEnabled =
		(CONFIG(auxSerialTxPin)) || // we need at least one pin set
		(CONFIG(auxSerialRxPin));

	isSerialRXEnabled = CONFIG(auxSerialRxPin);
	isSerialTXEnabled = CONFIG(auxSerialTxPin);

	// exit if no pin is configured
	if (!isSerialEnabled)
		return;

	// Validate pins 
	if (isSerialTXEnabled && !isValidSerialTxPin(CONFIG(auxSerialTxPin)))
		return;

	if (isSerialRXEnabled && !isValidSerialRxPin(CONFIG(auxSerialRxPin)))
		return;

	startAuxSerialPins();

	if (isSerialRXEnabled)
		serialRead.Start();
}

#endif // EFI_AUX_SERIAL
