#pragma once

#include "hal.h"

class Logging;
void processCanRxMessage(const CANRxFrame& msg, Logging* logger);
