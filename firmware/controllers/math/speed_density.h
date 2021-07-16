/**
 * @file	speed_density.h
 *
 * @date May 29, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine_ptr.h"
#include "rusefi_types.h"

#define gramm_second_to_cc_minute(gs) ((gs) / 0.0119997981)
#define cc_minute_to_gramm_second(ccm) ((ccm) * 0.0119997981)

temperature_t getTCharge(int rpm, float tps DECLARE_ENGINE_PARAMETER_SUFFIX);

void initSpeedDensity(DECLARE_ENGINE_PARAMETER_SIGNATURE);
