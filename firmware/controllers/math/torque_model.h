// torque_model.h

#pragma once

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

class TorqueModel {

public:
    TorqueModel();

};
