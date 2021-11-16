/**
 * @file init.h
 */

#pragma once

#include "engine_ptr.h"

// Call this once at startup to initialize, configure, and subscribe sensors
void initNewSensors(DECLARE_ENGINE_PARAMETER_SIGNATURE);

void stopSensors();

// Call this whenever the configuration may have changed, so any sensors
// can be reconfigured with the new settings.
// Note: this may not be necessarily possible for all sensors, so some may
// do nothing when this is called.
void reconfigureSensors(DECLARE_ENGINE_PARAMETER_SIGNATURE);

// Internal init functions for individual systems
// Sensor init/config
void initVbatt();
void initMaf();
void initMap(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void initTps();
void initOilPressure();
void initThermistors();
void initCanSensors();
void initLambda(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void initFlexSensor();
void initFuelLevel();
void initBaro();
void initAuxSensors();
void initVehicleSpeedSensor(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void initTurbochargerSpeedSensor(DECLARE_ENGINE_PARAMETER_SIGNATURE);

// Sensor reconfiguration
void deinitVbatt();
void deinitTps();
void deinitThermistors();
void deinitOilPressure();
void deInitFlexSensor();
void deInitVehicleSpeedSensor();
void deinitTurbochargerSpeedSensor();
void deinitMap();
