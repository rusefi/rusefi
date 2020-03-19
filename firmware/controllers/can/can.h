/**
 * @file	can.h
 *
 * @date Mar 19, 2020
 * @author Matthew Kennedy, (c) 2020
 */

#pragma once

#include "hal.h"

class Logging;
void processCanRxMessage(const CANRxFrame& msg, Logging* logger);
