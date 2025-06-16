/**
 * file can_bench_test.h
 */

#pragma once
#include "can.h"

void processCanQcBenchTest(const CANRxFrame& frame);
void sendQcBenchEventCounters();
void sendQcBenchButtonCounters();
void sendQcBenchAuxDigitalCounters();
void sendQcBenchRawAnalogValues();
void sendQcBenchBoardStatus(size_t bus = 0);
void initQcBenchControls();
