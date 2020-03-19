/**
 * @file	can.h
 *
 * @date Mar 19, 2020
 * @author Matthew Kennedy, (c) 2020
 */

#pragma once

#include "hal.h"

#include "periodic_thread_controller.h"

class Logging;
void processCanRxMessage(const CANRxFrame& msg, Logging* logger);

class CanWrite final : public PeriodicController<512> {
public:
	CanWrite();
	void PeriodicTask(efitime_t nowNt) override;
};
