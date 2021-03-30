/**
 * @file	hip9011_lookup.h
 *
 * @date Jan 4, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine.h"

#define INT_LOOKUP_SIZE 32
#define GAIN_LOOKUP_SIZE 64
#define BAND_LOOKUP_SIZE 64

#define PIF 3.14159f
#define DESIRED_OUTPUT_VALUE 5.0f

extern const int integratorValues[INT_LOOKUP_SIZE];
extern const float gainLookupInReverseOrder[GAIN_LOOKUP_SIZE];
extern const float bandFreqLookup[BAND_LOOKUP_SIZE];

extern float rpmLookup[INT_LOOKUP_SIZE];
void initEngineNoiseTable(DECLARE_ENGINE_PARAMETER_SIGNATURE);

