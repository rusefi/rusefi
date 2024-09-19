#include "pch.h"
#include "can.h"

#pragma once

uint16_t getTwoBytesLsb(const CANRxFrame& frame, int offset);
uint16_t getTwoBytesMsb(const CANRxFrame& frame, int offset);
