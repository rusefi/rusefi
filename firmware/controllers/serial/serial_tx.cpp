/**
 * @file	serial_tx.cpp
 *
 * This file handles auxilery serial communication.
 *
 * @date Mar 19, 2020
 * @author Konstantin Smola, (c) 2020
 */

#include "globalaccess.h"

#if EFI_AUX_SERIAL
#include "engine.h"
#include "serial.h"
#include "serial_sensor.h"

#include "allsensors.h"
#include "vehicle_speed.h"


SerialWrite::SerialWrite()
	: PeriodicController("Serial TX", NORMALPRIO, 50)
{
}

void SerialWrite::PeriodicTask(efitime_t nowNt)
{
	UNUSED(nowNt);

	SerialSendTest();

}

#endif // EFI_AUX_SERIAL
