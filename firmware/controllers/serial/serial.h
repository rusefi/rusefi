/**
 * @file	serial.h
 *
 * @date Mar 19, 2020
 * @author Konstantin Smola, (c) 2020
 */

#pragma once

#include "hal.h"

#include "periodic_thread_controller.h"

class Logging;

class SerialWrite final : public PeriodicController<512> {
public:
	SerialWrite();
	void PeriodicTask(efitime_t nowNt) override;
};
