#include "pch.h"
#include "closed_loop_idle.h"
#include "engine_math.h"
#include "efitime.h"
#include "engine.h"
#include <cmath>
#include <rusefi/math.h>

// LTIT_TABLE_SIZE is defined in the header file

LongTermIdleTrim::LongTermIdleTrim() {
    initializeTableWithDefaults();
    emaError = 0.0f;
    ltitTableInitialized = false;
    m_pendingSave = false;
}

void LongTermIdleTrim::initializeTableWithDefaults() {
    // Initialize with 100% (1.0 multiplier) as default
    for (int i = 0; i < LTIT_TABLE_SIZE; i++) {
        for (int j = 0; j < LTIT_TABLE_SIZE; j++) {
            ltitTableHelper[i][j] = 100.0f;
        }
    }
}

bool LongTermIdleTrim::hasValidData() const {
    // More robust validation - check for reasonable range and distribution
    int validCount = 0;
    float totalValue = 0.0f;
    
    for (int i = 0; i < LTIT_TABLE_SIZE; i++) {
        for (int j = 0; j < LTIT_TABLE_SIZE; j++) {
            float value = static_cast<float>(config->ltitTable[i][j]);
            
            // Check if value is in reasonable range (50% to 150%)
            if (value >= 50.0f && value <= 150.0f) {
                validCount++;
                totalValue += value;
            }
        }
    }
    
    // Require at least half the table to be valid and reasonable average
    if (validCount < (LTIT_TABLE_SIZE * LTIT_TABLE_SIZE / 2)) {
        return false;
    }
    
    float avgValue = totalValue / validCount;
    return (avgValue >= 80.0f && avgValue <= 120.0f); // Reasonable average range
}

void LongTermIdleTrim::loadLtitFromConfig() {
    if (hasValidData()) {
        // Convert autoscaled uint16_t to float
        for (int i = 0; i < LTIT_TABLE_SIZE; i++) {
            for (int j = 0; j < LTIT_TABLE_SIZE; j++) {
                ltitTableHelper[i][j] = static_cast<float>(config->ltitTable[i][j]);
            }
        }
        
        ltitTableInitialized = true;
    } else {
        // Initialize with defaults if no valid data
        initializeTableWithDefaults();
        ltitTableInitialized = true;
    }
}

float LongTermIdleTrim::getLtitFactor(float rpm, float clt) const {
    if (!ltitTableInitialized) {
        return 1.0f; // No correction if not initialized
    }
    
    // Use proper bin finding with interpolation
    return interpolate3d(ltitTableHelper, 
                        config->cltIdleCorrBins, clt,
                        config->rpmIdleCorrBins, rpm) * 0.01f;
}

bool LongTermIdleTrim::isValidConditionsForLearning(float idleIntegral) const {
    // Validate integral is within reasonable range
    if (fabsf(idleIntegral) < 0.1f || fabsf(idleIntegral) > 25.0f) {
        return false;
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
    
    // Check if we're in idle RPM range
    float targetRpm = engine->module<IdleController>().unmock().getTargetRpm(clt);
    float rpmDelta = fabsf(rpm - targetRpm);
    bool isIdleRpm = rpmDelta < engineConfiguration->ltitStableRpmThreshold;
    
    // Check stability
    if (!isIdleRpm) {
        m_stableIdleTimer.reset();
        isStableIdle = false;
        return;
    }
    
    // Check if stable for minimum time
    if (!isStableIdle && m_stableIdleTimer.hasElapsedSec(engineConfiguration->ltitStableTime)) {
        isStableIdle = true;
    }
    
    if (!isStableIdle) {
        return;
    }
    
    // Main table learning (now allowed even with AC/Fan active)
    // Validate conditions
    if (!isValidConditionsForLearning(idleIntegral)) {
        return;
    }
    
    // Check minimum update interval
    if (!m_updateTimer.hasElapsedSec(1.0f)) {
        return;
    }
    m_updateTimer.reset();
    
    // Use proper bin finding with getBin function
    auto cltBin = priv::getBin(clt, config->cltIdleCorrBins);
    auto rpmBin = priv::getBin(rpm, config->rpmIdleCorrBins);
    
    // Apply correction with multiple cells for better interpolation
    float correction = idleIntegral * engineConfiguration->ltitCorrectionRate * 0.01f;
    float alpha = engineConfiguration->ltitEmaAlpha / 255.0f;
    
    // Primary cell (largest weight)
    float newValue = ltitTableHelper[cltBin.Idx][rpmBin.Idx] * (1.0f + correction);
    newValue = alpha * newValue + (1.0f - alpha) * ltitTableHelper[cltBin.Idx][rpmBin.Idx];
    
    // Apply clamping
    float clampMin = engineConfiguration->ltitClampMin > 0 ? engineConfiguration->ltitClampMin : 50.0f;
    float clampMax = engineConfiguration->ltitClampMax > 0 ? engineConfiguration->ltitClampMax : 150.0f;
    ltitTableHelper[cltBin.Idx][rpmBin.Idx] = clampF(clampMin, newValue, clampMax);
    
    // Apply to adjacent cells with reduced weight (for better interpolation)
    float adjWeight = 0.3f; // 30% weight for adjacent cells
    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            if (di == 0 && dj == 0) continue; // Skip primary cell
            
            int adjI = cltBin.Idx + di;
            int adjJ = rpmBin.Idx + dj;
            
            if (adjI >= 0 && adjI < LTIT_TABLE_SIZE && adjJ >= 0 && adjJ < LTIT_TABLE_SIZE) {
                float adjCorrection = correction * adjWeight;
                float adjNewValue = ltitTableHelper[adjI][adjJ] * (1.0f + adjCorrection);
                adjNewValue = alpha * adjNewValue + (1.0f - alpha) * ltitTableHelper[adjI][adjJ];
                ltitTableHelper[adjI][adjJ] = clampF(clampMin, adjNewValue, clampMax);
            }
        }
    }
    
    updatedLtit = true;
    
    // Apply smoothing if configured
    // smoothLtitTable(engineConfiguration->ltitSmoothingIntensity);
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
        m_pendingSave = true;
        m_ignitionOffTimer.reset();
        updatedLtit = false;
    }
}

void LongTermIdleTrim::onSlowCallback() {
    // Handle delayed save after ignition off
    if (m_pendingSave && !m_ignitionState) {
        float saveDelaySeconds = engineConfiguration->ltitIgnitionOffSaveDelay;
        if (saveDelaySeconds <= 0) {
            saveDelaySeconds = 5.0f; // Default 5 seconds
        }
        
        if (m_ignitionOffTimer.hasElapsedSec(saveDelaySeconds)) {
            // Save to flash memory
            for (int i = 0; i < LTIT_TABLE_SIZE; i++) {
                for (int j = 0; j < LTIT_TABLE_SIZE; j++) {
                    // Convert float to autoscaled uint16_t
                    config->ltitTable[i][j] = static_cast<uint16_t>(ltitTableHelper[i][j]);
                }
            }
            
            setNeedToWriteConfiguration();
            m_pendingSave = false;
        }
    }
}

void LongTermIdleTrim::smoothLtitTable(float intensity) {
    if (!engineConfiguration->ltitEnabled || intensity <= 0.0f || intensity > 100.0f) {
        return; // Invalid intensity or LTIT disabled
    }
    
    // Normalize intensity to 0.0-1.0 range
    float normalizedIntensity = intensity / 100.0f;
    
    // Apply smoothing using the template function from table_helper.h
    smoothTable<float, LTIT_TABLE_SIZE, LTIT_TABLE_SIZE>(ltitTableHelper, normalizedIntensity);
    
    // Mark for saving
    m_pendingSave = true;
} 