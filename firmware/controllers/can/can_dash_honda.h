// file can_dash_honda.h

#pragma once

#include "can.h"
#include "can_msg_tx.h"

uint8_t calculateHondaChecksum(uint16_t canId, uint8_t length, uint8_t* msg);
void canDashboardHondaK(CanCycle cycle);

