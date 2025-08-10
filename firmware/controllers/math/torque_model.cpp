// =============================================================================
// FILE: firmware/controllers/math/torque_model.cpp
// =============================================================================

#include "torque_model.h"
#include "engine_math.h"
#include "sensor_reader.h"

TorqueModel::TorqueModel() {
    // Initialize rate limiters
    torque_rate_limiter.init(100.0f);  // 100 Nm/s default rate limit
    air_path_limiter.init(50.0f);      // 50 Nm/s air path limit
    ignition_path_limiter.init(500.0f); // 500 Nm/s ignition path limit
    
    // Clear active requests
    for (int i = 0; i < 8; i++) {
        active_requests[i].is_active = false;
    }
    
    // Initialize estimation
    current_estimation = {};
    current_reserve = {};
}

void TorqueModel::update() {
    // Update torque estimation
    current_estimation = estimateCurrentTorque();
    
    // Update torque reserve calculation  
    updateTorqueReserve();
    
    // Clean up expired requests
    uint32_t current_time = getTimeNowMs();
    for (int i = 0; i < 8; i++) {
        if (active_requests[i].is_active) {
            if (current_time - active_requests[i].timestamp_ms > 500) {
                active_requests[i].is_active = false;
            }
        }
    }
}

void TorqueModel::submitTorqueRequest(const TorqueRequest& request) {
    // Find slot for this request source
    int slot = -1;
    for (int i = 0; i < 8; i++) {
        if (!active_requests[i].is_active || 
            active_requests[i].source == request.source) {
            slot = i;
            break;
        }
    }
    
    if (slot >= 0) {
        active_requests[slot] = request;
        active_requests[slot].timestamp_ms = getTimeNowMs();
        active_requests[slot].is_active = true;
    }
}

void TorqueModel::clearTorqueRequest(TorqueRequestSource source) {
    for (int i = 0; i < 8; i++) {
        if (active_requests[i].source == source) {
            active_requests[i].is_active = false;
        }
    }
}

TorqueEstimation TorqueModel::estimateCurrentTorque() {
    TorqueEstimation estimation = {};
    
    // Get current engine conditions
    int rpm = Sensor::getOrZero(SensorType::Rpm);
    float map_kpa = Sensor::getOrZero(SensorType::Map);
    float intake_temp = Sensor::getOrZero(SensorType::Iat);
    float ignition_advance = engine->ignitionState.baseIgnitionAdvance;
    
    // Calculate air mass per stroke
    float air_mass_per_stroke = getAirmassFromMap(map_kpa, rpm, intake_temp);
    
    // Base torque calculation
    estimation.indicated_torque_nm = calculateBaseTorque(air_mass_per_stroke, rpm);
    
    // Apply ignition efficiency
    float ignition_efficiency = calculateIgnitionEfficiency(ignition_advance, rpm, air_mass_per_stroke);
    estimation.indicated_torque_nm *= ignition_efficiency;
    
    // Subtract friction losses
    estimation.friction_torque_nm = calculateFrictionLosses(rpm, engine->sensors.clt);
    estimation.brake_torque_nm = estimation.indicated_torque_nm - estimation.friction_torque_nm;
    
    // Calculate overall efficiency
    if (estimation.indicated_torque_nm > 0) {
        estimation.efficiency_factor = estimation.brake_torque_nm / estimation.indicated_torque_nm;
    }
    
    // Estimate confidence based on sensor quality
    estimation.confidence_level = 0.8f; // TODO: Calculate based on sensor health
    
    return estimation;
}

float TorqueModel::calculateBaseTorque(float load_mg_per_stroke, int rpm) {
    // Simple torque calculation based on air mass
    // Torque (Nm) ≈ Air Mass (mg/stroke) * Fuel Energy Density * Thermal Efficiency / Angular Velocity
    
    float displacement_per_stroke = engineConfiguration->displacement / engineConfiguration->cylindersCount / 2.0f;
    float volumetric_efficiency = getVeFromMap(rpm, load_mg_per_stroke);
    
    // Base torque calculation (simplified)
    float base_torque = (load_mg_per_stroke * 0.001f) * // Convert mg to g
                       (44.0f) * // Energy content of gasoline (MJ/kg) * efficiency factor
                       (60.0f / (2.0f * 3.14159f * rpm)) * 1000.0f; // Convert to Nm
    
    return base_torque * volumetric_efficiency;
}

float TorqueModel::calculateFrictionLosses(int rpm, float oil_temp) {
    // Friction model: Base friction + RPM-dependent + temperature correction
    float base_friction = 5.0f; // Base friction torque in Nm
    float rpm_factor = rpm * 0.01f; // RPM-dependent friction
    float temp_correction = 1.0f + (80.0f - oil_temp) * 0.005f; // Temperature correction
    
    return (base_friction + rpm_factor) * temp_correction;
}

float TorqueModel::calculateIgnitionEfficiency(float timing_advance, int rpm, float load) {
    // Simplified ignition efficiency model
    // Peak efficiency around MBT (Maximum Brake Torque) timing
    float mbt_timing = 15.0f + (rpm / 1000.0f * 5.0f); // Simplified MBT calculation
    float timing_error = abs(timing_advance - mbt_timing);
    
    // Efficiency drops with timing error
    float efficiency = 1.0f - (timing_error * 0.02f);
    return clampF(efficiency, 0.5f, 1.0f);
}

void TorqueModel::arbitrateTorqueRequests(TorqueRequest* result) {
    // Find highest priority active request
    TorqueRequest* highest_priority = nullptr;
    
    for (int i = 0; i < 8; i++) {
        if (active_requests[i].is_active) {
            if (highest_priority == nullptr || 
                active_requests[i].source < highest_priority->source) {
                highest_priority = &active_requests[i];
            }
        }
    }
    
    if (highest_priority) {
        *result = *highest_priority;
    } else {
        // No active requests - use driver request or idle
        result->demanded_torque_nm = 0.0f;
        result->source = TorqueRequestSource::IDLE;
        result->is_active = false;
    }
}

void TorqueModel::updateTorqueReserve() {
    // Calculate available torque headroom at current throttle position
    int rpm = Sensor::getOrZero(SensorType::Rpm);
    float current_throttle = Sensor::getOrZero(SensorType::Tps1);
    
    // Calculate maximum possible torque at current RPM
    float max_torque_available = calculateBaseTorque(getMaxAirmassForRpm(rpm), rpm);
    
    // Calculate current torque production
    float current_torque = current_estimation.brake_torque_nm;
    
    // Available positive reserve
    current_reserve.positive_reserve_nm = max_torque_available - current_torque;
    
    // Available negative reserve (limited by ignition timing retard capability)
    float max_ignition_retard_loss = current_torque * 0.3f; // 30% torque reduction max
    current_reserve.negative_reserve_nm = max_ignition_retard_loss;
    
    // Calculate current reserve usage
    if (max_torque_available > 0) {
        current_reserve.current_reserve_usage = current_torque / max_torque_available;
    }
}

ActuatorCommands TorqueModel::calculateActuatorCommands() {
    ActuatorCommands commands = {};
    
    // Arbitrate torque requests
    TorqueRequest target_request;
    arbitrateTorqueRequests(&target_request);
    
    if (!target_request.is_active) {
        // No active torque request - use traditional control
        commands.throttle_position_percent = -1.0f; // Signal to use traditional control
        return commands;
    }
    
    // Calculate required air mass for target torque
    int rpm = Sensor::getOrZero(SensorType::Rpm);
    float target_torque = target_request.demanded_torque_nm;
    
    // Account for friction losses
    float target_indicated_torque = target_torque + calculateFrictionLosses(rpm, engine->sensors.clt);
    
    // Calculate required air mass
    float required_air_mass = target_indicated_torque / calculateBaseTorque(1.0f, rpm);
    
    // Convert air mass to throttle position
    commands.throttle_position_percent = calculateThrottleForAirmass(required_air_mass, rpm);
    
    // Calculate ignition timing for torque reserve
    float base_timing = getBaseIgnitionTiming(rpm, required_air_mass);
    float reserve_timing_adjustment = calculateReserveTimingAdjustment();
    commands.ignition_advance_deg = base_timing + reserve_timing_adjustment;
    
    // Fuel correction (usually 1.0 for stoichiometric)
    commands.fuel_correction_factor = 1.0f;
    
    // Boost target (if applicable)
    commands.boost_target_kpa = calculateBoostTarget(required_air_mass, rpm);
    
    return commands;
}

float TorqueModel::predictTorqueForConditions(float load, int rpm, float timing) {
    float base_torque = calculateBaseTorque(load, rpm);
    float ignition_efficiency = calculateIgnitionEfficiency(timing, rpm, load);
    float friction_losses = calculateFrictionLosses(rpm, engine->sensors.clt);
    
    return (base_torque * ignition_efficiency) - friction_losses;
}