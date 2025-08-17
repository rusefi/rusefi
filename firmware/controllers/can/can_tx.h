// file can_tx.h

#include "pch.h"
#include "can.h"

#pragma once


class CanWrite final : public PeriodicController</*TStackSize*/512> {
public:
	CanWrite();
	void PeriodicTask(efitick_t nowNt) override;
};
