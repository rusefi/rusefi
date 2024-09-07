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

void initBenchTest();
void onConfigurationChangeBenchTest();

bool isRunningBenchTest();
const OutputPin *getOutputOnTheBenchTest();
void fatalErrorForPresetApply();

void fanBench();
void fan2Bench();
void fuelPumpBench();
void acRelayBench();
void milBench();
void starterRelayBench();

void executeTSCommand(uint16_t subsystem, uint16_t index);
void handleBenchCategory(uint16_t index);
int getSavedBenchTestPinStates(uint32_t durationsInStateMs[2]);
