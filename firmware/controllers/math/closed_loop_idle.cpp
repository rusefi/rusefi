#include "pch.h"
#include "closed_loop_idle.h"
#include "engine_math.h"
#include "efitime.h"
#include "engine.h"
#include <rusefi/rusefi_math.h>

// LTIT_TABLE_SIZE is defined in the header file

#if EFI_IDLE_CONTROL

LongTermIdleTrim::LongTermIdleTrim() {
    initializeTableWithDefaults();
    emaError = 0.0f; //TODO: unused?
    ltitTableInitialized = false;
    m_pendingSave = false;
}

void LongTermIdleTrim::initializeTableWithDefaults() {
    // Initialize with 100% (1.0 multiplier) as default
    for (int i = 0; i < LTIT_TABLE_SIZE; i++) {
        ltitTableHelper[i] = 100.0f;
    }
}

//TODO: move? add? to validateConfigOnStartUpOrBurn
bool LongTermIdleTrim::hasValidData() const {
    // More robust validation - check for reasonable range and distribution
    int validCount = 0;
    float totalValue = 0.0f;

    for (int i = 0; i < LTIT_TABLE_SIZE; i++) {
        float value = static_cast<float>(config->ltitTable[i]);

        // Check if value is in reasonable range (50% to 150%)
        if (value >= 50.0f && value <= 150.0f) {
            validCount++;
            totalValue += value;
        }
    }

    // Require at least half the table to be valid and reasonable average
    if (validCount < (LTIT_TABLE_SIZE / 2)) {
        return false;
    }

    float avgValue = totalValue / validCount;
    return (avgValue >= 80.0f && avgValue <= 120.0f); // Reasonable average range
}

void LongTermIdleTrim::loadLtitFromConfig() {
    if (hasValidData()) {
        // Convert autoscaled uint16_t to float
        for (int i = 0; i < LTIT_TABLE_SIZE; i++) {
            ltitTableHelper[i] = static_cast<float>(config->ltitTable[i]);
        }

        ltitTableInitialized = true;
    } else {
        //TODO: this is part of setDefaultEngineConfiguration?
        // Initialize with defaults if no valid data
        initializeTableWithDefaults();
        ltitTableInitialized = true;
    }
}

float LongTermIdleTrim::getLtitFactor(float rpm, float clt) const {
    //TODO: rpm unused?
    UNUSED(rpm);

    if (!ltitTableInitialized) {
        return 1.0f; // No correction if not initialized
    }

    // Use 2D interpolation based only on CLT (temperature)
    return interpolate2d(clt, config->cltIdleCorrBins, ltitTableHelper) * 0.01f;
}

bool LongTermIdleTrim::isValidConditionsForLearning(float idleIntegral) const {
    float minThreshold = engineConfiguration->ltitIntegratorThreshold;
    if (std::abs(idleIntegral) < minThreshold) {
        return false; // Integrator too low - PID not working hard enough
    }

    // Upper limit to avoid extreme conditions
    if (std::abs(idleIntegral) > 25.0f) {
        return false; // Integrator too high - unstable conditions
    }

    // Check if enough time has passed since ignition on
    if (!m_updateTimer.hasElapsedSec(engineConfiguration->ltitIgnitionOnDelay)) {
        return false;
    }

    // Must be in stable idle
    if (!isStableIdle) {
        return false;
    }

    return true;
}

void LongTermIdleTrim::update(float rpm, float clt, bool acActive, bool fan1Active, bool fan2Active, float idleIntegral) {
    //TODO: acActive unused, fan1Active, fan2Active, check comment about isValidConditionsForLearning
    UNUSED(acActive); UNUSED(fan1Active); UNUSED(fan2Active);

    if (!engineConfiguration->ltitEnabled) {
        return;
    }

    // Try to load data periodically until successful
    if (!ltitTableInitialized) {
        loadLtitFromConfig();
        return;
    }

    // Check ignition delay
    if (!m_updateTimer.hasElapsedSec(engineConfiguration->ltitIgnitionOnDelay)) {
        m_stableIdleTimer.reset();
        isStableIdle = false;
        return;
    }

    auto& idleController = engine->engineModules.get<IdleController>();
    auto currentPhase = idleController->getCurrentPhase();

    // LTIT should only learn during Phase::Idling
    if (currentPhase != IIdleController::Phase::Idling) {
        m_stableIdleTimer.reset();
        isStableIdle = false;
        return;
    }

    // Check if we're in idle RPM range
    float targetRpm = idleController->getTargetRpm(clt).ClosedLoopTarget;
    float rpmDelta = std::abs(rpm - targetRpm);
    bool isIdleRpm = rpmDelta < engineConfiguration->ltitStableRpmThreshold;

    // Check stability
    if (!isIdleRpm) {
        m_stableIdleTimer.reset();
        isStableIdle = false;
        return;
    }

    // Check if stable for minimum time
    // TODO: set ltitStableTime default value? also move to autoscale 0.1 like other configs with seconds?
    if (!isStableIdle && m_stableIdleTimer.hasElapsedSec(engineConfiguration->ltitStableTime)) {
        isStableIdle = true;
    }

    if (!isStableIdle) {
        return;
    }

    if (!idleController->isIdleClosedLoop) {
        return;
    }

    // Main table learning (now allowed even with AC/Fan active)
    // Validate conditions
    if (!isValidConditionsForLearning(idleIntegral)) {
        return;
    }

    // Check minimum update interval (fixed slowCallback period: 50ms)
    if (!m_updateTimer.hasElapsedSec(1.0f)) {
        return;
    }
    m_updateTimer.reset();

    //TODO: docs?, weird use of non-public interpolation.h, we are trying to the get bin index for X temp?
    // also ltitTableHelper scale will depend on clt idle bins?
    // Use proper bin finding with getBin function for CLT only
    auto cltBin = priv::getBin(clt, config->cltIdleCorrBins);

    // Apply correction rate in %/s (percentage per second)
    // Using fixed slowCallback delta time (50ms = 0.05s) for consistent behavior
    const float deltaTime = 0.05f; // SLOW_CALLBACK_PERIOD_MS / 1000.0f
    float correctionPerSecond = idleIntegral * engineConfiguration->ltitCorrectionRate * 0.01f; // Convert % to decimal
    float correction = correctionPerSecond * deltaTime; // Apply time-based correction
    float alpha = engineConfiguration->ltitEmaAlpha / 255.0f;

    // Primary cell (largest weight)
    float newValue = ltitTableHelper[cltBin.Idx] * (1.0f + correction);
    newValue = alpha * newValue + (1.0f - alpha) * ltitTableHelper[cltBin.Idx];

    // Apply clamping
    float clampMin = engineConfiguration->ltitClampMin > 0 ? engineConfiguration->ltitClampMin : 0.0f;
    float clampMax = engineConfiguration->ltitClampMax > 0 ? engineConfiguration->ltitClampMax : 250.0f;
    ltitTableHelper[cltBin.Idx] = clampF(clampMin, newValue, clampMax);

    // Apply to adjacent cells with reduced weight (for better interpolation)
    float adjWeight = 0.3f; // 30% weight for adjacent cells
    for (int di = -1; di <= 1; di++) {
        if (di == 0) continue; // Skip primary cell

        int adjI = cltBin.Idx + di;

        if (adjI >= 0 && adjI < LTIT_TABLE_SIZE) {
            float adjCorrection = correction * adjWeight;
            float adjNewValue = ltitTableHelper[adjI] * (1.0f + adjCorrection);
            adjNewValue = alpha * adjNewValue + (1.0f - alpha) * ltitTableHelper[adjI];
            ltitTableHelper[adjI] = clampF(clampMin, adjNewValue, clampMax);
        }
    }

    updatedLtit = true;
}

void LongTermIdleTrim::onIgnitionStateChanged(bool ignitionOn) {
    m_ignitionState = ignitionOn;

    if (ignitionOn) {
        // Reset timers when ignition turns on
        m_updateTimer.reset();
        m_stableIdleTimer.reset();
        isStableIdle = false;
        m_pendingSave = false;
    } else if (updatedLtit) {
        // Schedule save after ignition off
        // TODO: maybe move all this to EngineModule & use needsDelayedShutoff?
        m_pendingSave = true;
        m_ignitionOffTimer.reset();
        updatedLtit = false;
    }
}

void LongTermIdleTrim::checkIfShouldSave() {
    // Handle delayed save after ignition off
    if (m_pendingSave && !m_ignitionState) {
        float saveDelaySeconds = engineConfiguration->ltitIgnitionOffSaveDelay;
        if (saveDelaySeconds <= 0) {
            //TODO: this is part of setDefaultEngineConfiguration?
            saveDelaySeconds = 5.0f; // Default 5 seconds
        }

        if (m_ignitionOffTimer.hasElapsedSec(saveDelaySeconds)) {
            // TODO: copyArray?
            // Save to flash memory
            for (int i = 0; i < LTIT_TABLE_SIZE; i++) {
                // Convert float to autoscaled uint16_t
                config->ltitTable[i] = static_cast<uint16_t>(ltitTableHelper[i]);
            }

#if EFI_PROD_CODE
            //TODO: we need to use requestBurn here?
            setNeedToWriteConfiguration();
#endif // EFI_PROD_CODE
            m_pendingSave = false;
        }
    }
}

//TODO: unused?
void LongTermIdleTrim::smoothLtitTable(float intensity) {
    if (!engineConfiguration->ltitEnabled || intensity <= 0.0f || intensity > 100.0f) {
        return; // Invalid intensity or LTIT disabled
    }

    // Normalize intensity to 0.0-1.0 range
    float normalizedIntensity = intensity / 100.0f;

    // Apply 1D smoothing for the temperature-based curve
    float temp[LTIT_TABLE_SIZE];

    for (int i = 0; i < LTIT_TABLE_SIZE; i++) {
        float sum = ltitTableHelper[i];
        int count = 1;

        // Add values from adjacent cells if they exist
        if (i > 0) {
            sum += ltitTableHelper[i-1];
            count++;
        }
        if (i < LTIT_TABLE_SIZE-1) {
            sum += ltitTableHelper[i+1];
            count++;
        }

        // Calculate the average of the cell and its neighbors
        float avg = sum / count;

        // Apply weighted average based on intensity
        temp[i] = ltitTableHelper[i] * (1.0f - normalizedIntensity) + avg * normalizedIntensity;
    }

    // Copy back the smoothed values
    for (int i = 0; i < LTIT_TABLE_SIZE; i++) {
        ltitTableHelper[i] = temp[i];
    }

    // Mark for saving
    m_pendingSave = true;
}

#endif // EFI_IDLE_CONTROL
