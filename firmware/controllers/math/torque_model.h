// =============================================================================
// FILE: firmware/controllers/math/torque_model.h
// =============================================================================

#pragma once

#include "engine.h"
#include "interpolation.h"
#include "table_helper.h"

/**
 * Torque Request Sources - Priority-based arbitration
 */
enum class TorqueRequestSource : uint8_t {
    DRIVER = 0,           // Highest priority - Driver pedal input
    IDLE,                 // Idle speed control
    TRACTION_CONTROL,     // Traction control system
    LAUNCH_CONTROL,       // Launch control
    TRANSMISSION,         // TCU requests
    SPEED_LIMITER,        // Vehicle speed limiting
    ENGINE_PROTECTION,    // Engine protection (overrev, etc.)
    FUEL_CUT             // Fuel cut strategies
};

/**
 * Torque Path Types - Different actuator paths for torque delivery
 */
enum class TorquePath : uint8_t {
    AIR_PATH,            // Throttle/boost control (slow response, ~100-500ms)
    IGNITION_PATH,       // Spark timing (fast response, ~10-50ms)
    FUEL_PATH            // Fuel delivery control (medium response, ~50-200ms)
};

/**
 * Torque Request Structure
 */
struct TorqueRequest {
    float demanded_torque_nm;        // Requested torque in Nm
    float max_positive_rate_nm_s;    // Maximum positive torque rate limit
    float max_negative_rate_nm_s;    // Maximum negative torque rate limit
    TorqueRequestSource source;      // Request source for arbitration
    uint32_t timestamp_ms;          // Request timestamp for timeout handling
    bool is_active;                 // Request validity flag
};

/**
 * Torque Estimation Structure
 */
struct TorqueEstimation {
    float indicated_torque_nm;      // Torque before friction losses
    float brake_torque_nm;          // Net torque at flywheel
    float friction_torque_nm;       // Internal friction losses
    float efficiency_factor;        // Overall efficiency (0.0-1.0)
    float confidence_level;         // Estimation confidence (0.0-1.0)
};

/**
 * Torque Reserve Structure
 */
struct TorqueReserve {
    float positive_reserve_nm;      // Available positive torque headroom
    float negative_reserve_nm;      // Available negative torque headroom
    float current_reserve_usage;    // Current reserve utilization (0.0-1.0)
};

/**
 * Actuator Commands Structure
 */
struct ActuatorCommands {
    float throttle_position_percent;    // Throttle position command
    float ignition_advance_deg;         // Ignition timing command
    float fuel_correction_factor;       // Fuel delivery correction
    float boost_target_kpa;            // Boost control target
    bool fuel_cut_active;              // Fuel cut flag
};

/**
 * Core Torque Model Class
 */
class TorqueModel {
private:
    // Configuration tables
    Table3D torque_vs_load_rpm;           // Base torque map [Load][RPM] -> Torque
    Table3D ignition_efficiency_map;      // Ignition timing efficiency
    Table2D friction_loss_map;            // RPM -> Friction losses
    Table2D ve_correction_map;            // VE corrections for torque
    
    // Current state
    TorqueRequest active_requests[8];     // Active torque requests
    TorqueEstimation current_estimation;  // Current torque estimation
    TorqueReserve current_reserve;        // Current torque reserve
    
    // Rate limiters
    RateLimiter torque_rate_limiter;
    RateLimiter air_path_limiter;
    RateLimiter ignition_path_limiter;
    
    // Internal methods
    float calculateBaseTorque(float load_mg_per_stroke, int rpm);
    float calculateFrictionLosses(int rpm, float oil_temp);
    float calculateIgnitionEfficiency(float timing_advance, int rpm, float load);
    void arbitrateTorqueRequests(TorqueRequest* result);
    void updateTorqueReserve();
    
public:
    TorqueModel();
    
    // Main update cycle
    void update();
    
    // Torque request management
    void submitTorqueRequest(const TorqueRequest& request);
    void clearTorqueRequest(TorqueRequestSource source);
    
    // Torque estimation
    TorqueEstimation estimateCurrentTorque();
    float predictTorqueForConditions(float load, int rpm, float timing);
    
    // Actuator commands
    ActuatorCommands calculateActuatorCommands();
    
    // Reserve management
    TorqueReserve getTorqueReserve() const { return current_reserve; }
    void setReserveTarget(float positive_nm, float negative_nm);
    
    // Configuration
    void configureTorqueMaps(const TorqueModelConfig& config);
    void setRateLimits(float max_rate_nm_s, TorquePath path);
};