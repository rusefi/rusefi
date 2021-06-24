/**
 * @file    bench_test.h
 * @brief	Utility methods related to bench testing.
 *
 * todo: rename this file
 *
 * @date Sep 8, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"

void initBenchTest();

bool isRunningBenchTest(void);

void fanBench(void);
void fan2Bench(void);
void fuelPumpBench(void);
void acRelayBench(void);
void milBench(void);
void starterRelayBench(void);

void executeTSCommand(uint16_t subsystem, uint16_t index);
