/**
 * @file init.h
 */

#pragma once

#include "engine_ptr.h"

// Call this once at startup to initialize, configure, and subscribe sensors
void initNewSensors(DECLARE_ENGINE_PARAMETER_SIGNATURE);

void stopSensors(DECLARE_CONFIG_PARAMETER_SIGNATURE);

// Call this whenever the configuration may have changed, so any sensors
// can be reconfigured with the new settings.
// Note: this may not be necessarily possible for all sensors, so some may
// do nothing when this is called.
void reconfigureSensors(DECLARE_ENGINE_PARAMETER_SIGNATURE);

// Internal init functions for individual systems
// Sensor init/config
void initVbatt(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void initMaf(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void initMap(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void initTps(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void initOilPressure(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void initThermistors(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void initCanSensors();
void initLambda(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void initFlexSensor(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void initFuelLevel(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void initBaro(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void initAuxSensors(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void initVehicleSpeedSensor(DECLARE_ENGINE_PARAMETER_SIGNATURE);

// Sensor reconfiguration
void deinitVbatt();
void deinitTps();
void deinitThermistors();
void reconfigureOilPressure(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void deInitFlexSensor();
void deInitVehicleSpeedSensor();
