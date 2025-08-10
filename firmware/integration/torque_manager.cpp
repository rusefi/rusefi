// =============================================================================
// FILE: firmware/integration/torque_manager.cpp
// =============================================================================

#include "torque_manager.h"
#include "engine_controller.h"
#include "sensor_reader.h"
#include "electronic_throttle.h"

RusEfiTorqueManager::RusEfiTorqueManager() 
    : etb_controller(&torque_model),
      ignition_controller(&torque_model),
      idle_controller(&torque_model),
      transmission_interface(&torque_model) {
    
    is_initialized = false;
    is_torque_control_enabled = false;
    update_counter = 0;
    last_error_code = 0;
}

void RusEfiTorqueManager::initializeTorqueControl() {
    if (is_initialized) {
        return;
    }
    
    // Load configuration from engine config
    loadConfiguration(engineConfiguration->torqueModelConfig);
    
    // Initialize sub-controllers
    etb_controller.setThrottleRateLimit(config.max_air_path_rate_nm_s);
    ignition_controller.setReserveTimingRange(15.0f); // 15 degree max retard
    idle_controller.setTargetIdleRpm(engineConfiguration->idleRpmTarget);
    
    // Set up torque model with configuration
    torque_model.configureTorqueMaps(config);
    torque_model.setRateLimits(config.max_air_path_rate_nm_s, TorquePath::AIR_PATH);
    torque_model.setRateLimits(config.max_ignition_path_rate_nm_s, TorquePath::IGNITION_PATH);
    torque_model.setRateLimits(config.max_fuel_path_rate_nm_s, TorquePath::FUEL_PATH);
    
    is_initialized = true;
    is_torque_control_enabled = engineConfiguration->isTorqueControlEnabled;
    
    efiPrintf("Torque Control Initialized - Max Torque: %.1f Nm", config.max_torque_nm);
}

void RusEfiTorqueManager::updateTorqueControl() {
    if (!is_initialized || !is_torque_control_enabled) {
        return;
    }
    
    update_counter++;
    
    // Update core torque model
    torque_model.update();
    
    // Update sub-controllers
    etb_controller.update();
    ignition_controller.update();
    idle_controller.update();
    transmission_interface.update();
    
    // Safety checks
    performSafetyChecks();
}

void RusEfiTorqueManager::handleDriverTorqueRequest(float pedal_position_percent) {
    if (!is_torque_control_enabled) return;
    
    // Convert pedal position to torque demand
    float max_torque = config.max_torque_nm;
    float demanded_torque = (pedal_position_percent / 100.0f) * max_torque;
    
    // Apply pedal progression curve (non-linear response)
    float progression_factor = applyPedalProgression(pedal_position_percent);
    demanded_torque *= progression_factor;
    
    // Create torque request
    TorqueRequest request = {};
    request.demanded_torque_nm = demanded_torque;
    request.max_positive_rate_nm_s = config.max_air_path_rate_nm_s;
    request.max_negative_rate_nm_s = config.max_ignition_path_rate_nm_s * 2.0f; // Faster negative response
    request.source = TorqueRequestSource::DRIVER;
    request.is_active = true;
    
    torque_model.submitTorqueRequest(request);
}

void RusEfiTorqueManager::handleTractionControlRequest(float torque_reduction_nm) {
    if (!is_torque_control_enabled) return;
    
    // Get current torque estimate
    TorqueEstimation current = torque_model.estimateCurrentTorque();
    float target_torque = current.brake_torque_nm - torque_reduction_nm;
    
    // Ensure we don't go negative
    target_torque = maxF(target_torque, 0.0f);
    
    TorqueRequest request = {};
    request.demanded_torque_nm = target_torque;
    request.max_positive_rate_nm_s = config.max_ignition_path_rate_nm_s;
    request.max_negative_rate_nm_s = config.max_ignition_path_rate_nm_s * 3.0f; // Very fast reduction
    request.source = TorqueRequestSource::TRACTION_CONTROL;
    request.is_active = true;
    
    torque_model.submitTorqueRequest(request);
}

void RusEfiTorqueManager::handleLaunchControlRequest(float torque_limit_nm) {
    if (!is_torque_control_enabled) return;
    
    TorqueRequest request = {};
    request.demanded_torque_nm = torque_limit_nm;
    request.max_positive_rate_nm_s = config.max_air_path_rate_nm_s * 0.5f; // Slower increase
    request.max_negative_rate_nm_s = config.max_ignition_path_rate_nm_s;
    request.source = TorqueRequestSource::LAUNCH_CONTROL;
    request.is_active = true;
    
    torque_model.submitTorqueRequest(request);
}

void RusEfiTorqueManager::handleTransmissionRequest(float torque_nm, uint16_t ramp_time_ms) {
    if (!is_torque_control_enabled) return;
    
    // Calculate rate limit based on ramp time
    TorqueEstimation current = torque_model.estimateCurrentTorque();
    float torque_delta = abs(torque_nm - current.brake_torque_nm);
    float required_rate = (ramp_time_ms > 0) ? (torque_delta / (ramp_time_ms * 0.001f)) : config.max_air_path_rate_nm_s;
    
    TorqueRequest request = {};
    request.demanded_torque_nm = torque_nm;
    request.max_positive_rate_nm_s = required_rate;
    request.max_negative_rate_nm_s = required_rate;
    request.source = TorqueRequestSource::TRANSMISSION;
    request.is_active = true;
    
    torque_model.submitTorqueRequest(request);
}

void RusEfiTorqueManager::emergencyTorqueReduction(float torque_limit_nm) {
    // Emergency override - highest priority
    torque_model.clearTorqueRequest(TorqueRequestSource::ENGINE_PROTECTION);
    
    TorqueRequest request = {};
    request.demanded_torque_nm = torque_limit_nm;
    request.max_positive_rate_nm_s = 0.0f; // No positive rate allowed
    request.max_negative_rate_nm_s = 10000.0f; // Maximum possible reduction rate
    request.source = TorqueRequestSource::ENGINE_PROTECTION;
    request.is_active = true;
    
    torque_model.submitTorqueRequest(request);
}

ActuatorCommands RusEfiTorqueManager::getActuatorOverrides() {
    if (!is_torque_control_enabled) {
        ActuatorCommands empty = {};
        empty.throttle_position_percent = -1.0f; // Signal no override
        return empty;
    }
    
    return torque_model.calculateActuatorCommands();
}

bool RusEfiTorqueManager::shouldOverrideTraditionalControl() const {
    return is_torque_control_enabled && is_initialized;
}

void RusEfiTorqueManager::getTorqueStatus(TorqueStatus* status) {
    if (!status) return;
    
    TorqueEstimation estimation = torque_model.estimateCurrentTorque();
    TorqueReserve reserve = torque_model.getTorqueReserve();
    
    status->current_torque_nm = estimation.brake_torque_nm;
    status->target_torque_nm = 0.0f; // TODO: Get from current request
    status->torque_error_nm = 0.0f;  // TODO: Calculate error
    status->positive_reserve_nm = reserve.positive_reserve_nm;
    status->negative_reserve_nm = reserve.negative_reserve_nm;
    status->active_source = TorqueRequestSource::DRIVER; // TODO: Get from arbitration
    status->confidence_level = estimation.confidence_level;
    status->is_control_active = is_torque_control_enabled;
    status->error_flags = last_error_code;
}

void RusEfiTorqueManager::performSafetyChecks() {
    // Check for sensor failures
    if (!Sensor::isRedundant(SensorType::Rpm) && Sensor::getOrZero(SensorType::Rpm) < 100) {
        // RPM sensor failure at speed - disable torque control
        if (update_counter > 1000) { // Only after initial startup
            disableTorqueControl();
            last_error_code |= 0x01; // RPM sensor error
        }
    }
    
    // Check for MAP sensor failure
    if (!Sensor::hasSensor(SensorType::Map)) {
        disableTorqueControl();
        last_error_code |= 0x02; // MAP sensor error
    }
    
    // Check for TPS sensor failure
    if (!Sensor::hasSensor(SensorType::Tps1)) {
        disableTorqueControl();
        last_error_code |= 0x04; // TPS sensor error
    }
    
    // Check torque reserve adequacy
    TorqueReserve reserve = torque_model.getTorqueReserve();
    if (reserve.positive_reserve_nm < 5.0f && reserve.current_reserve_usage > 0.9f) {
        // Low torque reserve warning
        last_error_code |= 0x08;
    }
}

float RusEfiTorqueManager::applyPedalProgression(float pedal_percent) {
    // Non-linear pedal progression for better drivability
    // Light pedal application = reduced torque demand
    // Heavy pedal application = full torque demand
    
    if (pedal_percent < 10.0f) {
        return 0.3f; // 30% of linear response for light pedal
    } else if (pedal_percent < 50.0f) {
        return 0.3f + (pedal_percent - 10.0f) * 0.01f; // Gradual increase
    } else {
        return 0.7f + (pedal_percent - 50.0f) * 0.006f; // Near linear for heavy pedal
    }
}

void RusEfiTorqueManager::loadConfiguration(const TorqueModelConfig& cfg) {
    config = cfg;
    
    // Validate configuration
    if (config.max_torque_nm <= 0) {
        config.max_torque_nm = 200.0f; // Default 200 Nm
    }
    
    if (config.idle_torque_nm <= 0) {
        config.idle_torque_nm = 15.0f; // Default 15 Nm for idle
    }
    
    // Ensure rate limits are reasonable
    config.max_air_path_rate_nm_s = clampF(config.max_air_path_rate_nm_s, 10.0f, 500.0f);
    config.max_ignition_path_rate_nm_s = clampF(config.max_ignition_path_rate_nm_s, 100.0f, 2000.0f);
    config.max_fuel_path_rate_nm_s = clampF(config.max_fuel_path_rate_nm_s, 50.0f, 1000.0f);
}