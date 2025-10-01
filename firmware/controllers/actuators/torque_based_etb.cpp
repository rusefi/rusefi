// =============================================================================
// FILE: firmware/controllers/actuators/torque_based_etb.cpp
// =============================================================================

#include "torque_based_etb.h"
#include "sensor_reader.h"
#include "engine_math.h"

TorqueBasedEtbController::TorqueBasedEtbController(TorqueModel* model) 
    : torque_model(model) {
    
    // Initialize PID controller for throttle position
    position_pid.init(1.0f, 0.1f, 0.05f);  // P, I, D terms
    position_pid.setOutputLimits(-100.0f, 100.0f);
    
    // Initialize state
    target_reserve_nm = 20.0f;  // 20 Nm default reserve
    current_throttle_reserve_percent = 0.0f;
    max_throttle_rate_percent_s = 200.0f;  // 200%/s max rate
    last_throttle_command_percent = 0.0f;
    last_update_time_ms = 0;
}

void TorqueBasedEtbController::update() {
    uint32_t current_time = getTimeNowMs();
    float dt_s = (current_time - last_update_time_ms) * 0.001f;
    
    if (dt_s > 0.1f) dt_s = 0.1f; // Cap delta time for safety
    
    // Get torque model commands
    ActuatorCommands commands = torque_model->calculateActuatorCommands();
    
    if (commands.throttle_position_percent < 0) {
        // Torque control is not active, use traditional ETB control
        return;
    }
    
    // Apply rate limiting
    float throttle_change = commands.throttle_position_percent - last_throttle_command_percent;
    float max_change = max_throttle_rate_percent_s * dt_s;
    
    if (abs(throttle_change) > max_change) {
        throttle_change = (throttle_change > 0) ? max_change : -max_change;
    }
    
    float limited_throttle_command = last_throttle_command_percent + throttle_change;
    
    // Calculate throttle position for torque reserve
    float reserve_adjustment = calculateReserveThrottleAdjustment();
    float final_throttle_position = limited_throttle_command + reserve_adjustment;
    
    // Clamp to valid range
    final_throttle_position = clampF(final_throttle_position, 0.0f, 100.0f);
    
    // Send command to ETB hardware
    setEtbPosition(final_throttle_position);
    
    // Update state
    last_throttle_command_percent = limited_throttle_command;
    last_update_time_ms = current_time;
    
    // Update reserve calculation
    updateReserveCalculation();
}

float TorqueBasedEtbController::calculateThrottlePosition(float target_torque_nm) {
    int rpm = Sensor::getOrZero(SensorType::Rpm);
    
    // Calculate required air mass for target torque
    float required_air_mass = calculateAirmassForTorque(target_torque_nm, rpm);
    
    // Convert air mass to throttle position using calibrated map
    float base_throttle_position = interpolateTable2D(airflow_to_throttle_map, required_air_mass);
    
    // Apply RPM correction
    float rpm_correction = interpolateTable2D(rpm_correction_map, rpm);
    
    return base_throttle_position * rpm_correction;
}

void TorqueBasedEtbController::handleTorqueReserveRequest(float reserve_nm) {
    target_reserve_nm = reserve_nm;
}

float TorqueBasedEtbController::calculateReserveThrottleAdjustment() {
    // Calculate how much additional throttle opening is needed for reserve
    TorqueReserve current_reserve = torque_model->getTorqueReserve();
    
    if (current_reserve.positive_reserve_nm < target_reserve_nm) {
        // Need more throttle opening for reserve
        float reserve_deficit = target_reserve_nm - current_reserve.positive_reserve_nm;
        
        // Convert torque deficit to throttle opening (simplified)
        float additional_throttle = reserve_deficit * 0.5f; // 0.5% throttle per Nm
        
        return clampF(additional_throttle, 0.0f, 20.0f); // Max 20% additional opening
    }
    
    return 0.0f;
}

void TorqueBasedEtbController::updateReserveCalculation() {
    int rpm = Sensor::getOrZero(SensorType::Rpm);
    float current_throttle = Sensor::getOrZero(SensorType::Tps1);
    
    // Calculate maximum airflow at 100% throttle
    float max_airflow = calculateMaxAirflowForRpm(rpm);
    
    // Calculate current airflow
    float current_airflow = calculateCurrentAirflow(current_throttle, rpm);
    
    // Reserve percentage
    if (max_airflow > 0) {
        current_throttle_reserve_percent = ((max_airflow - current_airflow) / max_airflow) * 100.0f;
    }
}

void TorqueBasedEtbController::setThrottleRateLimit(float rate_percent_s) {
    max_throttle_rate_percent_s = clampF(rate_percent_s, 10.0f, 1000.0f);
}

void TorqueBasedEtbController::calibrateAirflowMap() {
    // Auto-calibration routine for airflow-to-throttle mapping
    // This would run during initial setup or recalibration
    
    // TODO: Implement automatic calibration based on MAP sensor feedback
    // For now, use default values
    
    // Example calibration points (throttle position -> relative airflow)
    float throttle_points[] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    float airflow_points[] = {0, 0.05, 0.12, 0.22, 0.35, 0.50, 0.67, 0.80, 0.90, 0.97, 1.0};
    
    // Populate the table (simplified - in real implementation would use Table2D methods)
    for (int i = 0; i < 11; i++) {
        // setTableValue(airflow_to_throttle_map, airflow_points[i], throttle_points[i]);
    }
}