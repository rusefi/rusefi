/**
 * @file map.h
 *
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine_ptr.h"

struct air_pressure_sensor_config_s;

void initMapDecoder();

float getBaroPressure();

float decodePressure(float voltage, air_pressure_sensor_config_s * mapConfig);

#define KPA_PER_PSI 6.89475728f

#define PSI2KPA(psi)  (KPA_PER_PSI * (psi))

#define BAR2KPA(bar) (100 * (bar))
#define KPA2BAR(kpa) (0.01f * (kpa))

// PSI (relative to atmosphere) to kPa (relative to vacuum)
#define PSI2KPA_RELATIVE(psi)  (101.32500411216164f + PSI2KPA(psi))

#define INHG2KPA(inhg) ((inhg) * 3.386375f)
#define KPA2INHG(kpa) ((kpa) / 3.386375f)

