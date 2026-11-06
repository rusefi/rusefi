/*
 * @file bmw_m73.h
 *
 * @date Nov 1, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine_configuration.h"

// M73 settings shared by the full V12 presets and the two-ECU 6+6 microRusEFI split
void m73engineCommon();

// the whole 12-cylinder engine on one ECU, see [tag:max_cylinder_count_engine_guard]
#if MAX_CYLINDER_COUNT >= 12
void m73engine();

void setBMW_M73_TwoCoilUnitTest();

// set engine_type 63
void setEngineBMW_M73_Proteus();
#endif // MAX_CYLINDER_COUNT >= 12

void setEngineBMW_M73_microRusEfi();
