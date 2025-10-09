// =============================================================================
// FILE: firmware/controllers/actuators/torque_based_etb.h
// =============================================================================

#pragma once

#include "torque_model.h"
#include "pid.h"

/**
 * ETB (Electronic Throttle Body) Integration with Torque Control
 */
class TorqueBasedEtbController {
private:
    TorqueModel* torque_model;
    PidController position_pid;
    
    // Airflow to throttle position mapping
    Table2D airflow_to_throttle_map;  // [Airflow] -> Throttle Position
    Table2D rpm_correction_map;       // [RPM] -> Correction factor
    
    // Reserve management
    float target_reserve_nm;
    float current_throttle_reserve_percent;
    
    // Rate limiting
    float max_throttle_rate_percent_s;
    float last_throttle_command_percent;
    uint32_t last_update_time_ms;
    
public:
    TorqueBasedEtbController(TorqueModel* model);
    
    void update();
    float calculateThrottlePosition(float target_torque_nm);
    void handleTorqueReserveRequest(float reserve_nm);
    
    // Configuration
    void setThrottleRateLimit(float rate_percent_s);
    void calibrateAirflowMap();
    
    // Diagnostics
    float getCurrentReservePercent() const { return current_throttle_reserve_percent; }
    bool isReserveAdequate() const { return current_throttle_reserve_percent > 10.0f; }
};