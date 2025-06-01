#pragma once

#include "engine_module.h"
#include <rusefi/timer.h>

/**
 * States of the adaptive wall wetting state machine
 * Based on pattern used in LaunchControl and DFCO
 */
enum class AWWTransientState : uint8_t {
    IDLE = 0,                    // Waiting for adequate conditions
    TRANSIENT_DETECTED,          // Transient detected, starting analysis
    EARLY_COLLECTION,            // Collecting data for Beta analysis (0-500ms)
    LATE_COLLECTION,             // Collecting data for Tau analysis (500ms-2s)
    PROCESSING_BETA,             // Processing Beta correction
    PROCESSING_TAU,              // Processing Tau correction
    APPLYING_CORRECTIONS,        // Applying corrections to tables
    COOLDOWN                     // Waiting period before next cycle
};

/**
 * Adaptive wall wetting system data - following launch_control_state.txt pattern
 */
struct adaptive_wall_wetting_state_s {
    // State machine
    AWWTransientState state = AWWTransientState::IDLE;
    
    // Conditions for operation
    bool isAdaptiveLearningEnabled = false;
    bool isEngineStable = false;
    bool isLambdaValid = false;
    bool isConditionsMet = false;
    
    // Transient detection
    bool isTransientDetected = false;
    bool isAcceleration = false;
    float transientMagnitude = 0;
    
    // Position in tables
    uint8_t currentMapIndex = 0;
    uint8_t currentRpmIndex = 0;
    
    // Analysis data
    uint16_t betaSampleCount = 0;
    uint16_t tauSampleCount = 0;
    float betaLambdaSum = 0;
    float tauSettlingTime = 0;
    bool tauOvershootDetected = false;
    
    // Correction results
    float calculatedBetaCorrection = 0;
    float calculatedTauCorrection = 0;
    
    // Timing information
    float transientStartTime = 0;
    float lastUpdateTime = 0;
};

/**
 * Adaptive wall wetting controller
 * Integrated as EngineModule following pattern of other controllers
 */
class AdaptiveWallWettingController : public EngineModule {
public:
    using interface_t = AdaptiveWallWettingController;
    
    void onFastCallback() override;  // Called at 200Hz
    void onSlowCallback() override;  // Called at 20Hz
    void onIgnitionStateChanged(bool ignitionOn) override;
    
    // Public interface for diagnostics (similar to LaunchControl)
    AWWTransientState getState() const { return m_state.state; }
    bool isActive() const { return m_state.state != AWWTransientState::IDLE; }
    
private:
    adaptive_wall_wetting_state_s m_state;
    
    // Circular buffer for lambda data (similar to TPS accel enrichment)
    static constexpr size_t LAMBDA_BUFFER_SIZE = 400; // 2s at 200Hz
    float m_lambdaBuffer[LAMBDA_BUFFER_SIZE];
    float m_timestampBuffer[LAMBDA_BUFFER_SIZE];
    uint16_t m_bufferIndex = 0;
    
    // Filters for transient detection (similar to TPS accel)
    float m_lastTps = 0;
    float m_lastMap = 0;
    float m_tpsDerivative = 0;
    float m_mapDerivative = 0;
    
    // Timers for state machine control
    Timer m_transientTimer;
    Timer m_cooldownTimer;
    Timer m_updateTimer;
    
    // Save management
    bool m_ignitionState = false;
    bool m_pendingSave = false;
    bool m_updatedCorrections = false;
    Timer m_ignitionOffTimer;
    
    // Main state machine methods
    void updateStateMachine();
    void processIdleState();
    void processTransientDetected();
    void processEarlyCollection();
    void processLateCollection();
    void processAnalyzingBeta();
    void processAnalyzingTau();
    void processApplyingCorrections();
    void processCooldown();
    
    // Utility methods
    bool canRunAdaptiveLearning() const;
    bool detectTransient();
    void addLambdaSample();
    float getCurrentLambdaError() const;
    void calculateTableIndices(float map, float rpm);
    void updateCorrectionTable(bool isBeta, uint8_t mapIdx, uint8_t rpmIdx, float correction);
    void resetState();
    void enterCooldown();
    void updateDerivativeFilters();
    
    // Analysis methods
    void analyzeBetaCorrection();
    void analyzeTauCorrection();
    
    // Constants
    static constexpr float BETA_COLLECTION_DURATION = 0.5f;   // 500ms
    static constexpr float TAU_COLLECTION_DURATION = 1.5f;    // 1.5s
    static constexpr float COOLDOWN_DURATION = 10.0f;         // 10s
}; 