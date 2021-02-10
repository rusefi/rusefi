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
void initVbatt(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void initMap(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void initTps(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void initOilPressure(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void initThermistors(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void initCanSensors();
void initLambda(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void initFlexSensor(DECLARE_CONFIG_PARAMETER_SIGNATURE);

// Sensor reconfiguration
void reconfigureVbatt(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void reconfigureTps(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void reconfigureThermistors(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void reconfigureOilPressure(DECLARE_CONFIG_PARAMETER_SIGNATURE);
