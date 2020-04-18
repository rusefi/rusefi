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


EXTERN_ENGINE;

// static int canReadCounter = 0;
// static int canWriteOk = 0;
// static int canWriteNotOk = 0;
static bool isSerialEnabled = false;
static LoggingWithStorage logger("SERIAL driver");


static SerialConfig uartCfg;
static SerialWrite serial;


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

void enableAuxSerial(DECLARE_ENGINE_PARAMETER_SIGNATURE)
{
	// SerialConfig uartCfg;

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

	efiSetPadMode("AuxSerial TX", CONFIG(auxSerialTxPin), PAL_MODE_ALTERNATE(8));
	efiSetPadMode("AuxSerial RX", CONFIG(auxSerialRxPin), PAL_MODE_ALTERNATE(8));

	enableAuxSerial();
}

void initAuxSerial(void)
{
	addConsoleAction("auxinfo", auxInfo);

	isSerialEnabled =
		(CONFIG(auxSerialTxPin) != GPIO_UNASSIGNED) && // both pins are set...
		(CONFIG(auxSerialRxPin) != GPIO_UNASSIGNED);

	// nothing to do if we aren't enabled...
	if (!isSerialEnabled)
	{
		return;
	}

	// Validate pins 
	if (!isValidSerialTxPin(CONFIG(auxSerialTxPin)))
	{
		// firmwareError(CUSTOM_OBD_70, "invalid Serial TX %s", hwPortname(CONFIG(canTxPin)));
		return;
	}

	if (!isValidSerialRxPin(CONFIG(auxSerialRxPin)))
	{
		// firmwareError(CUSTOM_OBD_70, "invalid Serial RX %s", hwPortname(CONFIG(canRxPin)));
		return;
	}

	startAuxSerialPins();
	serial.Start();
}

#endif // EFI_AUX_SERIAL