// =============================================================================
// FILE: firmware/integration/torque_manager.h
// =============================================================================

#pragma once

#include "torque_model.h"
#include "torque_based_etb.h"

/**
 * Configuration structure for torque model
 */
struct TorqueModelConfig {
    // Base torque characteristics
    float max_torque_nm;                    // Maximum engine torque
    float idle_torque_nm;                   // Torque required for idle
    float friction_base_nm;                 // Base friction torque
    
    // Efficiency parameters
    float peak_thermal_efficiency;          // Peak engine efficiency
    float ignition_efficiency_range;        // Ignition timing efficiency range
    
    // Rate limiting
    float max_air_path_rate_nm_s;          // Air path rate limit
    float max_ignition_path_rate_nm_s;     // Ignition path rate limit
    float max_fuel_path_rate_nm_s;         // Fuel path rate limit
    
    // Reserve management
    float default_positive_reserve_nm;      // Default positive reserve
    float default_negative_reserve_nm;      // Default negative reserve
    float reserve_response_time_ms;         // Reserve response time target
    
    // Calibration tables
    float torque_map_data[16][16];         // RPM vs Load torque map
    float friction_map_data[16];           // RPM vs friction losses
    float ignition_eff_data[16][16];       // Timing vs efficiency map
};

/**
 * Torque-based Ignition Controller
 */
class TorqueBasedIgnitionController {
private:
    TorqueModel* torque_model;
    Table3D torque_to_timing_map;  // [Torque][RPM] -> Timing advance
    
    float base_timing_advance;
    float torque_reserve_timing_retard;
    
public:
    TorqueBasedIgnitionController(TorqueModel* model);
    
    void update();
    angle_t calculateIgnitionTiming(float target_torque_nm);
    angle_t calculateTorqueReserveTiming(float reserve_torque_nm);
    
    // Configuration
    void setBaseTimingMap(const Table3D& timing_map);
    void setReserveTimingRange(float max_retard_deg);
};

/**
 * Torque-based Idle Controller
 */
class TorqueBasedIdleController {
private:
    TorqueModel* torque_model;
    PidController rpm_pid;
    
    float target_idle_rpm;
    float idle_torque_demand_nm;
    float max_idle_torque_nm;
    
    // Idle torque reserve for fast corrections
    float idle_reserve_nm;
    
public:
    TorqueBasedIdleController(TorqueModel* model);
    
    void update();
    void setTargetIdleRpm(float rpm);
    TorqueRequest calculateIdleTorqueRequest();
    
    // Diagnostics
    float getCurrentIdleTorque() const { return idle_torque_demand_nm; }
    bool isIdleStable() const;
};

/**
 * Transmission Torque Interface
 */
class TransmissionTorqueInterface {
private:
    TorqueModel* torque_model;
    
    // CAN message structures
    struct TorqueStatusMessage {
        uint16_t current_torque_nm;      // Current engine torque
        uint16_t available_torque_nm;    // Maximum available torque
        uint8_t torque_accuracy;         // Estimation accuracy percentage
        uint8_t torque_reserve_percent;  // Available reserve percentage
    };
    
    struct TorqueRequestMessage {
        uint16_t requested_torque_nm;    // Requested torque
        uint8_t request_type;            // Normal/Emergency/Gradual
        uint16_t ramp_time_ms;          // Time to reach target torque
    };
    
    TorqueStatusMessage last_status_msg;
    TorqueRequestMessage last_request_msg;
    uint32_t last_status_send_time;
    
public:
    TransmissionTorqueInterface(TorqueModel* model);
    
    void update();
    void sendTorqueStatus();
    void processTcuTorqueRequest();
    
    // Standard torque message formats
    void sendME72TorqueMessage();    // Mercedes ME7.2 format
    void sendBoschTorqueMessage();   // Bosch standard format
    void sendBMWTorqueMessage();     // BMW MSR format
};

/**
 * Main Torque Manager - integrates with existing RusEFI architecture
 */
class RusEfiTorqueManager {
private:
    TorqueModel torque_model;
    TorqueBasedEtbController etb_controller;
    TorqueBasedIgnitionController ignition_controller;
    TorqueBasedIdleController idle_controller;
    TransmissionTorqueInterface transmission_interface;
    
    // Configuration
    TorqueModelConfig config;
    bool is_initialized;
    bool is_torque_control_enabled;
    
    // Diagnostics
    uint32_t update_counter;
    uint32_t last_error_code;
    
public:
    RusEfiTorqueManager();
    
    // Initialization and configuration
    void initializeTorqueControl();
    void loadConfiguration(const TorqueModelConfig& cfg);
    bool isTorqueControlActive() const { return is_torque_control_enabled; }
    
    // Main control loop integration
    void updateTorqueControl();
    
    // External interfaces for different control systems
    void handleDriverTorqueRequest(float pedal_position_percent);
    void handleTractionControlRequest(float torque_reduction_nm);
    void handleLaunchControlRequest(float torque_limit_nm);
    void handleTransmissionRequest(float torque_nm, uint16_t ramp_time_ms);
    void handleSpeedLimiterRequest(float max_torque_nm);
    
    // Override commands for emergency situations
    void emergencyTorqueReduction(float torque_limit_nm);
    void disableTorqueControl();
    void enableTorqueControl();
    
    // Diagnostic and monitoring interfaces
    struct TorqueStatus {
        float current_torque_nm;
        float target_torque_nm;
        float torque_error_nm;
        float positive_reserve_nm;
        float negative_reserve_nm;
        TorqueRequestSource active_source;
        float confidence_level;
        bool is_control_active;
        uint32_t error_flags;
    };
    
    void getTorqueStatus(TorqueStatus* status);
    void resetTorqueModel();
    void calibrateTorqueModel();
    
    // Integration points for existing RusEFI systems
    ActuatorCommands getActuatorOverrides();
    bool shouldOverrideTraditionalControl() const;
    float getTorqueBasedThrottlePosition();
    float getTorqueBasedIgnitionTiming();
    float getTorqueBasedFuelCorrection();
};