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
// #include "serial_sensor.h"

EXTERN_ENGINE;

static bool isSerialEnabled = false;
static bool isSerialTXEnabled = false;
static bool isSerialRXEnabled = false;
static LoggingWithStorage logger("SERIAL driver");


static SerialConfig uartCfg;
static SerialRead serialRead;
static SerialWrite serialWrite;


static void auxInfo(void)
{
	if (!isSerialEnabled) 
	{
		scheduleMsg(&logger, "AUX Serial is not enabled, please enable & restart");
		return;
	}

	scheduleMsg(&logger, "AUX Serial TX %s", hwPortname(CONFIG(auxSerialTxPin)));
	scheduleMsg(&logger, "AUX Serial RX %s", hwPortname(CONFIG(auxSerialRxPin)));
}

// static void rxIRQHandler(UARTDriver *uartp)
// {
// 	ParseSerialData();
// }

void enableAuxSerial(DECLARE_ENGINE_PARAMETER_SIGNATURE)
{
	CONFIG(auxSerialTxPin) = engineConfiguration->auxSerialTxPin;
	CONFIG(auxSerialRxPin) = engineConfiguration->auxSerialRxPin;
	CONFIG(auxSerialSpeed) = engineConfiguration->auxSerialSpeed;
	
	uartCfg.speed = engineConfiguration->auxSerialSpeed;
	sdStart(&SD6, &uartCfg);

	scheduleMsg(&logger, "AUX Serial started");
}

void stopAuxSerialPins(DECLARE_ENGINE_PARAMETER_SIGNATURE)
{
	brain_pin_markUnused(activeConfiguration.auxSerialTxPin);
	brain_pin_markUnused(activeConfiguration.auxSerialRxPin);
}

void startAuxSerialPins(DECLARE_ENGINE_PARAMETER_SIGNATURE)
{

	if (CONFIG(auxSerialTxPin))
		efiSetPadMode("AuxSerial TX", CONFIG(auxSerialTxPin), PAL_MODE_ALTERNATE(8));
	if (CONFIG(auxSerialRxPin))
		efiSetPadMode("AuxSerial RX", CONFIG(auxSerialRxPin), PAL_MODE_ALTERNATE(8));

	enableAuxSerial();
}

void initAuxSerial(void)
{
	addConsoleAction("auxinfo", auxInfo);

	isSerialEnabled =
		(CONFIG(auxSerialTxPin)) || // we need at least one pin set
		(CONFIG(auxSerialRxPin));

	isSerialRXEnabled = CONFIG(auxSerialRxPin);
	isSerialTXEnabled = CONFIG(auxSerialTxPin);

	// exit if no pin is configured
	if (!isSerialEnabled)
	{
		return;
	}

	// Validate pins 
	if (isSerialTXEnabled && !isValidSerialTxPin(CONFIG(auxSerialTxPin)))
	{
		// firmwareError(CUSTOM_OBD_70, "invalid Serial TX %s", hwPortname(CONFIG(canTxPin)));
		return;
	}

	if (isSerialRXEnabled && !isValidSerialRxPin(CONFIG(auxSerialRxPin)))
	{
		// firmwareError(CUSTOM_OBD_70, "invalid Serial RX %s", hwPortname(CONFIG(canRxPin)));
		return;
	}

	startAuxSerialPins();

	//now start the threads:
	if (isSerialTXEnabled)
		serialWrite.Start(); //start serialwrite thread
	if (isSerialRXEnabled)
		serialRead.Start(); //start serialwrite thread
}

#endif // EFI_AUX_SERIAL