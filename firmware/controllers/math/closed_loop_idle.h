#pragma once
#include "engine_module.h"
#include "rusefi_types.h"
#include "efi_pid.h"
#include "sensor.h"
#include "flash_main.h"
#include "table_helper.h"
#include "closed_loop_idle_generated.h"
#include <rusefi/interpolation.h>
#include <rusefi/timer.h>

constexpr int LTIT_TABLE_SIZE = CLT_IDLE_TABLE_CLT_SIZE;

class LongTermIdleTrim : public closed_loop_idle_s {
public:
    LongTermIdleTrim();
    float getLtitFactor(float rpm, float clt) const;
    void update(float rpm, float clt, bool acActive, bool fan1Active, bool fan2Active, float idleIntegral);
    void smoothLtitTable(float intensity);
    void onIgnitionStateChanged(bool ignitionOn);
    void checkIfShouldSave();
    bool updatedLtit = false;

    // Load data from flash to helper table
    void loadLtitFromConfig();
    // Check if table has valid data
    bool hasValidData() const;

private:
    float ltitTableHelper[LTIT_TABLE_SIZE];
    bool ltitTableInitialized = false;

    // Timers for control
    Timer m_updateTimer;
    Timer m_stableIdleTimer;
    Timer m_ignitionOffTimer;

    bool m_ignitionState = false;

    // Learning state management
    bool m_pendingSave = false;

    // Validation helpers
    bool isValidConditionsForLearning(float idleIntegral) const;
    void initializeTableWithDefaults();
};
