#pragma once
#include "engine_module.h"
#include "rusefi_types.h"
#include "efi_pid.h"
#include "sensor.h"
#include "flash_main.h"
#include "table_helper.h"
#include <rusefi/interpolation.h>
#include <rusefi/timer.h>

constexpr int LTIT_TABLE_SIZE = 16;

class LongTermIdleTrim {
public:
    LongTermIdleTrim();
    float getLtitFactor(float rpm, float clt) const;
    float getLtitAcTrim() const;
    float getLtitFan1Trim() const;
    float getLtitFan2Trim() const;
    void update(float rpm, float clt, bool acActive, bool fan1Active, bool fan2Active, float idleIntegral);
    void smoothLtitTable(float intensity);
    void onIgnitionStateChanged(bool ignitionOn);
    void onSlowCallback();
    bool updatedLtit = false;
    
    // Load data from flash to helper table
    void loadLtitFromConfig();
    // Check if table has valid data
    bool hasValidData() const;
    
private:
    float ltitTableHelper[LTIT_TABLE_SIZE][LTIT_TABLE_SIZE];
    float acTrim = 0.0f, fan1Trim = 0.0f, fan2Trim = 0.0f;
    float emaError = 0.0f;
    bool ltitTableInitialized = false;
    
    // Timers for control
    Timer m_updateTimer;
    Timer m_stableIdleTimer;
    Timer m_ignitionOffTimer;
    Timer m_acStateTimer;
    Timer m_fan1StateTimer;
    Timer m_fan2StateTimer;
    
    // State tracking
    bool isStableIdle = false;
    bool m_ignitionState = false;
    bool m_lastAcState = false;
    bool m_lastFan1State = false;
    bool m_lastFan2State = false;
    
    // Learning state management
    bool m_pendingSave = false;
    
    // Validation helpers
    bool isValidConditionsForLearning(float idleIntegral) const;
    void updateTrimLearning(bool acActive, bool fan1Active, bool fan2Active, float idleIntegral);
    void initializeTableWithDefaults();
}; 