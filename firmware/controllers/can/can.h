#pragma once

#include "hal.h"

#include "periodic_thread_controller.h"

class Logging;
void processCanRxMessage(const CANRxFrame& msg, Logging* logger);

class CanWrite final : public PeriodicController<256> {
public:
	CanWrite();
	void PeriodicTask(efitime_t nowNt) override;
};
