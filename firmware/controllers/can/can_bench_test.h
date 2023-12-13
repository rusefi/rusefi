/**
 * file can_bench_test.h
 */

#pragma once
#include "can.h"

void processCanBenchTest(const CANRxFrame& frame);
void sendEventCounters();
void sendButtonCounters();
void sendRawAnalogValues();
void sendBoardStatus();
void initQcControls();
