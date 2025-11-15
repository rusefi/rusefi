// file can_rx.h

#include "pch.h"
#include "can.h"

#pragma once

void printCANRxFrame(const size_t busIndex, const CANRxFrame &rx);

uint32_t getFourBytesLsb(const CANRxFrame& frame, int offset);
uint16_t getTwoBytesLsb(const CANRxFrame& frame, int offset);
uint16_t getTwoBytesMsb(const CANRxFrame& frame, int offset);
