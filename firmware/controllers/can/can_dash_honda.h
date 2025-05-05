#pragma once

#include "can.h"

uint8_t calculateHondaChecksum(uint16_t canId, uint8_t length, uint8_t* data);
void canDashboardHondaK(CanCycle cycle);

