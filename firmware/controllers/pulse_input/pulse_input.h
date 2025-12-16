#pragma once

#include "engine.h"

// Initialize the pulse input (register EXTI)
void pulseInputInit();

// Update averages and publish results to engine_state
void pulseInputUpdate();