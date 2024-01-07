/**
 * @file	fuel_math.h
 *
 * @date May 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "rusefi_types.h"

void initFuelMap();

/**
 * @return baseFuel with CLT and IAT corrections
 */
float getRunningFuel(float baseFuel);

float getBaroCorrection();
percent_t getFuelALSCorrection(int rpm);
int getNumberOfInjections(injection_mode_e mode);
angle_t getInjectionOffset(float rpm, float load);
float getIatFuelCorrection();

float getCltFuelCorrection();
angle_t getCltTimingCorrection();
float getCrankingFuel(float baseFuel);
float getCrankingFuel3(float baseFuel, uint32_t revolutionCounterSinceStart);
float getInjectionMass(int rpm);
percent_t getInjectorDutyCycle(int rpm);
percent_t getInjectorDutyCycleStage2(int rpm);
float getStage2InjectionFraction(int rpm, float fuelLoad);

float getStandardAirCharge();
float getCylinderFuelTrim(size_t cylinderNumber, int rpm, float fuelLoad);

struct AirmassModelBase;
AirmassModelBase* getAirmassModel(engine_load_mode_e mode);

float getMaxAirflowAtMap(float map);
