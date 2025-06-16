/**
 * file can_bench_test.h
 */

#pragma once
#include "can.h"

void processCanQcBenchTest(const CANRxFrame& frame);
void sendQcBenchEventCounters(size_t bus = 0);
void sendQcBenchButtonCounters();
void sendQcBenchAuxDigitalCounters();
void sendQcBenchRawAnalogValues(size_t bus = 0);
void sendQcBenchBoardStatus(size_t bus = 0);
void initQcBenchControls();
