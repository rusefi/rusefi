/**
 * @file init.h
 */

#pragma once

#include "globalaccess.h"

class Logging;

// Call this once at startup to initialize, configure, and subscribe sensors
void initNewSensors(Logging* logger DECLARE_ENGINE_PARAMETER_SUFFIX);

// Call this whenever the configuration may have changed, so any sensors
// can be reconfigured with the new settings.
// Note: this may not be necessarily possible for all sensors, so some may
// do nothing when this is called.
void reconfigureSensors(DECLARE_ENGINE_PARAMETER_SIGNATURE);

// Internal init functions for individual systems
// Sensor init/config
void initTps(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void initOilPressure(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void initNewThermistors(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void initCanSensors(DECLARE_ENGINE_PARAMETER_SIGNATURE);

// Sensor reconfiguration
void reconfigureTps(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void reconfigureThermistors(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void reconfigureOilPressure(DECLARE_ENGINE_PARAMETER_SIGNATURE);
