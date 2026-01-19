/*
 * @file rotational_idle.h
 *
 * @date: ene 16, 2026
 * @author FDSoftware
 */

#pragma once

#include "live_data_rotational_idle_generated.h"

class RotationalIdle: public live_data_rotational_idle_s {
public:
    RotationalIdle();
    bool shouldEngageRotationalIdle();
    bool shouldSkipSparkRotationalIdle(int8_t offset);
};