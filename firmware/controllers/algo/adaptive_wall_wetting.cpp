/*
 * @file adaptive_wall_wetting.cpp
 *
 * Adaptive Wall Wetting System Implementation
 * Based on Aquino Model with Lambda Feedback Learning
 * 
 * @author AI Assistant via Cursor
 * @date 2025
 */

#include "pch.h"
#include "adaptive_wall_wetting.h"
#include "dfco.h"
#include "tps.h"
#include "rusefi/interpolation.h"

void AdaptiveWallWettingController::onFastCallback() {
    if (!engineConfiguration->wwEnableAdaptiveLearning) {
        return;
    }
    
    // Update derivative filters for transient detection
    updateDerivativeFilters();
    
    // Add current lambda sample to buffer
    float lambdaError = getCurrentLambdaError();
    if (!std::isnan(lambdaError)) {
        addLambdaSample();
    }
    
    // Process state machine
    updateStateMachine();
}

void AdaptiveWallWettingController::onSlowCallback() {
    // Handle delayed save after ignition off (like LTIT)
    if (m_pendingSave && !m_ignitionState) {
        if (m_ignitionOffTimer.hasElapsedSec(5.0f)) {
            // Save correction tables to flash memory
            setNeedToWriteConfiguration();
            m_pendingSave = false;
        }
    }
}

void AdaptiveWallWettingController::onIgnitionStateChanged(bool ignitionOn) {
    m_ignitionState = ignitionOn;
    
    if (ignitionOn) {
        // Reset when ignition turns on
        m_updateTimer.reset();
        m_pendingSave = false;
        resetState();
    } else if (m_updatedCorrections) {
        // Schedule save after ignition turns off
        m_pendingSave = true;
        m_ignitionOffTimer.reset();
        m_updatedCorrections = false;
    }
}

void AdaptiveWallWettingController::updateStateMachine() {
    switch (m_state.state) {
        case AWWTransientState::IDLE:
            processIdleState();
            break;
            
        case AWWTransientState::TRANSIENT_DETECTED:
            processTransientDetected();
            break;
            
        case AWWTransientState::EARLY_COLLECTION:
            processEarlyCollection();
            break;
            
        case AWWTransientState::LATE_COLLECTION:
            processLateCollection();
            break;
            
        case AWWTransientState::PROCESSING_BETA:
            processAnalyzingBeta();
            break;
            
        case AWWTransientState::PROCESSING_TAU:
            processAnalyzingTau();
            break;
            
        case AWWTransientState::APPLYING_CORRECTIONS:
            processApplyingCorrections();
            break;
            
        case AWWTransientState::COOLDOWN:
            processCooldown();
            break;
    }
}

void AdaptiveWallWettingController::processIdleState() {
    if (canRunAdaptiveLearning() && detectTransient()) {
        m_state.state = AWWTransientState::TRANSIENT_DETECTED;
        m_transientTimer.reset(); // Start timing the transient
        
        // Clear buffer and reset counters
        m_bufferIndex = 0;
        m_state.betaSampleCount = 0;
        m_state.tauSampleCount = 0;
        m_state.betaLambdaSum = 0;
        
        // Store operating point for table update
        float map = Sensor::getOrZero(SensorType::Map);
        float rpm = Sensor::getOrZero(SensorType::Rpm);
        calculateTableIndices(map, rpm);
    }
}

void AdaptiveWallWettingController::processTransientDetected() {
    // Immediately start collecting data
    m_state.state = AWWTransientState::EARLY_COLLECTION;
}

void AdaptiveWallWettingController::processEarlyCollection() {
    // Collect beta data for 500ms
    if (m_transientTimer.hasElapsedSec(BETA_COLLECTION_DURATION)) {
        m_state.state = AWWTransientState::LATE_COLLECTION;
    }
}

void AdaptiveWallWettingController::processLateCollection() {
    // Collect tau data until 2s total
    if (m_transientTimer.hasElapsedSec(BETA_COLLECTION_DURATION + TAU_COLLECTION_DURATION)) {
        m_state.state = AWWTransientState::PROCESSING_BETA;
    }
}

void AdaptiveWallWettingController::processAnalyzingBeta() {
    analyzeBetaCorrection();
    // Analyze methods set next state
}

void AdaptiveWallWettingController::processAnalyzingTau() {
    analyzeTauCorrection();
    // Analyze methods set next state
}

void AdaptiveWallWettingController::processApplyingCorrections() {
    // Apply Beta correction
    if (fabsf(m_state.calculatedBetaCorrection - 1.0f) > 0.01f) {
        updateCorrectionTable(true, m_state.currentMapIndex, m_state.currentRpmIndex, 
                             m_state.calculatedBetaCorrection);
    }
    
    // Apply Tau correction  
    if (fabsf(m_state.calculatedTauCorrection - 1.0f) > 0.01f) {
        updateCorrectionTable(false, m_state.currentMapIndex, m_state.currentRpmIndex,
                             m_state.calculatedTauCorrection);
    }
    
    enterCooldown();
}

void AdaptiveWallWettingController::processCooldown() {
    // Wait 10 seconds before next learning opportunity
    if (m_cooldownTimer.hasElapsedSec(COOLDOWN_DURATION)) {
        resetState();
        m_state.state = AWWTransientState::IDLE;
    }
}

bool AdaptiveWallWettingController::canRunAdaptiveLearning() const {
    // Check main flag
    if (!engineConfiguration->wwEnableAdaptiveLearning) {
        return false;
    }
    
    // Minimum update interval (like LTFT)
    if (!m_updateTimer.hasElapsedSec(1.0f)) {
        return false;
    }
    
    // Check essential sensors
    auto lambda = Sensor::get(SensorType::Lambda1);
    auto clt = Sensor::get(SensorType::Clt);
    auto tps = Sensor::get(SensorType::DriverThrottleIntent);
    auto map = Sensor::get(SensorType::Map);
    auto rpm = Sensor::get(SensorType::Rpm);
    
    if (!lambda.Valid || !clt.Valid || !tps.Valid || !map.Valid || !rpm.Valid) {
        return false;
    }
    
    // Check operational ranges
    if (rpm.Value < 1000 || rpm.Value > 6000) return false;
    if (clt.Value < 60 || clt.Value > 110) return false;
    if (map.Value < 30 || map.Value > 120) return false;
    
    // Lambda within reasonable range
    if (lambda.Value < 0.7f || lambda.Value > 1.3f) return false;
    
    // Engine not in cut-off or other special modes
    if (engine->module<DfcoController>()->cutFuel()) return false;
    
    return true;
}

bool AdaptiveWallWettingController::detectTransient() {
    // Simple transient detection based on TPS and MAP derivatives
    // TODO: Add more sophisticated detection based on the spec
    
    const float TPS_ACCEL_THRESH = 70.0f; // %/s
    const float MAP_ACCEL_THRESH = 40.0f; // kPa/s
    const float TPS_DECEL_THRESH = -70.0f; // %/s
    const float MAP_DECEL_THRESH = -40.0f; // kPa/s
    
    // Check for acceleration
    if (m_tpsDerivative > TPS_ACCEL_THRESH || m_mapDerivative > MAP_ACCEL_THRESH) {
        m_state.isAcceleration = true;
        m_state.transientMagnitude = maxF(m_tpsDerivative / TPS_ACCEL_THRESH, 
                                         m_mapDerivative / MAP_ACCEL_THRESH);
        return true;
    }
    
    // Check for deceleration
    if (m_tpsDerivative < TPS_DECEL_THRESH || m_mapDerivative < MAP_DECEL_THRESH) {
        m_state.isAcceleration = false;
        m_state.transientMagnitude = maxF(-m_tpsDerivative / TPS_DECEL_THRESH, 
                                         -m_mapDerivative / MAP_DECEL_THRESH);
        return true;
    }
    
    return false;
}

void AdaptiveWallWettingController::updateDerivativeFilters() {
    float currentTps = Sensor::getOrZero(SensorType::DriverThrottleIntent);
    float currentMap = Sensor::getOrZero(SensorType::Map);
    
    const float dt = 0.005f; // 200Hz callback period
    const float alpha = 0.8f; // Simple low-pass filter
    
    // Calculate derivatives
    float tpsDerivative = (currentTps - m_lastTps) / dt;
    float mapDerivative = (currentMap - m_lastMap) / dt;
    
    // Apply low-pass filtering
    m_tpsDerivative = m_tpsDerivative * alpha + tpsDerivative * (1.0f - alpha);
    m_mapDerivative = m_mapDerivative * alpha + mapDerivative * (1.0f - alpha);
    
    // Store for next iteration
    m_lastTps = currentTps;
    m_lastMap = currentMap;
}

void AdaptiveWallWettingController::addLambdaSample() {
    float lambdaError = getCurrentLambdaError();
    float currentTime = m_transientTimer.getElapsedSeconds();
    
    // Add to circular buffer
    m_lambdaBuffer[m_bufferIndex] = lambdaError;
    m_timestampBuffer[m_bufferIndex] = currentTime;
    
    m_bufferIndex = (m_bufferIndex + 1) % LAMBDA_BUFFER_SIZE;
    
    // Update collection counts based on current state
    if (m_state.state == AWWTransientState::EARLY_COLLECTION) {
        m_state.betaSampleCount++;
        m_state.betaLambdaSum += lambdaError;
    } else if (m_state.state == AWWTransientState::LATE_COLLECTION) {
        m_state.tauSampleCount++;
    }
}

float AdaptiveWallWettingController::getCurrentLambdaError() const {
    auto lambda = Sensor::get(SensorType::Lambda1);
    if (!lambda.Valid) {
        return NAN;
    }
    
    // Get lambda target from AFR table or default to stoichiometric
    // For now, default to stoichiometric (1.0) but this could be improved
    // to read from the actual fuel/lambda target maps
    float lambdaTarget = 1.0f;
    
    // TODO: Get actual lambda target from fuel map based on current load/RPM
    // This would require access to the fuel system's target calculation
    
    return lambda.Value - lambdaTarget;
}

void AdaptiveWallWettingController::calculateTableIndices(float map, float rpm) {
    // Use the proper bin search functions for finding table indices
    // The correction tables use the same bins as the base tables
    
    // Find MAP index using getBin function
    auto mapBin = priv::getBin(map, config->wwCorrectionMapBins);
    m_state.currentMapIndex = static_cast<uint8_t>(maxI(0, minI(mapBin.Idx, 7))); // 8x8 table, so max index is 7
    
    // Find RPM index using getBin function  
    auto rpmBin = priv::getBin(rpm, config->wwCorrectionRpmBins);
    m_state.currentRpmIndex = static_cast<uint8_t>(maxI(0, minI(rpmBin.Idx, 7))); // 8x8 table, so max index is 7
}

void AdaptiveWallWettingController::updateCorrectionTable(bool isBeta, uint8_t mapIdx, uint8_t rpmIdx, float correction) {
    if (isBeta) {
        // Update Beta correction table with clamping
        float currentValue = config->wwBetaCorrection[mapIdx][rpmIdx];
        float newValue = currentValue * correction;
        config->wwBetaCorrection[mapIdx][rpmIdx] = clampF(0.5f, newValue, 2.0f);
    } else {
        // Update Tau correction table with clamping
        float currentValue = config->wwTauCorrection[mapIdx][rpmIdx];
        float newValue = currentValue * correction;
        config->wwTauCorrection[mapIdx][rpmIdx] = clampF(0.5f, newValue, 2.0f);
    }
    
    // Mark that corrections were updated
    m_updatedCorrections = true;
}

void AdaptiveWallWettingController::analyzeBetaCorrection() {
    if (m_state.betaSampleCount < 50) { // Minimum samples
        enterCooldown();
        return;
    }
    
    // Calculate average error during beta phase (first 500ms)
    float avgLambdaError = m_state.betaLambdaSum / m_state.betaSampleCount;
    
    // Apply gain based on transient direction
    float gain = m_state.isAcceleration ? 
        engineConfiguration->wwAquinoBetaAccelGain : 
        engineConfiguration->wwAquinoBetaDecelGain;
    
    // Calculate correction with saturation
    float correctionPercent = avgLambdaError * gain * 100.0f;
    correctionPercent = clampF(-50.0f, correctionPercent, 50.0f);
    
    // Convert to multiplier (ex: 5% -> 1.05)
    m_state.calculatedBetaCorrection = 1.0f + (correctionPercent / 100.0f);
    
    m_state.state = AWWTransientState::PROCESSING_TAU;
}

void AdaptiveWallWettingController::analyzeTauCorrection() {
    // Analyze tau data in the window from 500ms-2s
    float settlingThreshold = 0.02f; // 2% of lambda
    float settleTime = 0;
    bool overshootDetected = false;
    float overshootMagnitude = 0;
    
    // Find settling time by analyzing lambda buffer in the tau collection window
    float tauCollectionStartTime = BETA_COLLECTION_DURATION;
    float tauCollectionEndTime = BETA_COLLECTION_DURATION + TAU_COLLECTION_DURATION;
    
    // Look for settling time
    bool foundSettling = false;
    
    // Simple analysis: find when lambda error stays within threshold for a period
    int consecutiveSettledSamples = 0;
    const int minSettledSamples = 20; // ~100ms at 200Hz
    
    // Calculate initial error magnitude for overshoot detection
    float initialError = 0;
    int initialSamples = 0;
    
    // Analyze buffer data
    for (size_t i = 0; i < LAMBDA_BUFFER_SIZE; i++) {
        float timestamp = m_timestampBuffer[i];
        float lambdaError = m_lambdaBuffer[i];
        
        // Skip samples outside our analysis window
        if (timestamp < tauCollectionStartTime || timestamp > tauCollectionEndTime) {
            continue;
        }
        
        // Calculate initial error (first 100ms of tau window)
        if (timestamp < tauCollectionStartTime + 0.1f) {
            initialError += lambdaError;
            initialSamples++;
        }
        
        // Check for settling
        if (fabsf(lambdaError) < settlingThreshold) {
            consecutiveSettledSamples++;
            if (consecutiveSettledSamples >= minSettledSamples && !foundSettling) {
                settleTime = timestamp - tauCollectionStartTime;
                foundSettling = true;
            }
        } else {
            consecutiveSettledSamples = 0;
        }
        
        // Check for overshoot (sign change with significant magnitude)
        if (initialSamples > 0) {
            float avgInitialError = initialError / initialSamples;
            if ((avgInitialError > 0 && lambdaError < -0.03f) || 
                (avgInitialError < 0 && lambdaError > 0.03f)) {
                if (fabsf(lambdaError) > fabsf(overshootMagnitude)) {
                    overshootDetected = true;
                    overshootMagnitude = lambdaError;
                }
            }
        }
    }
    
    // If not settled by end of window, use full duration
    if (!foundSettling) {
        settleTime = TAU_COLLECTION_DURATION;
    }
    
    // Calculate expected settling time based on current tau
    float currentTau = 0.5f; // Default tau value - TODO: get actual current tau
    float expectedSettleTime = currentTau * 3.0f; // Approximately 3 time constants
    
    // Determine correction based on analysis
    if (overshootDetected) {
        // Overshoot detected - increase tau
        float gain = engineConfiguration->wwAquinoTauOvershootGain;
        float overshootFactor = fabsf(overshootMagnitude) / 0.03f; // Normalize to threshold
        m_state.calculatedTauCorrection = 1.0f + (gain * overshootFactor);
    } else {
        // No significant overshoot - check settling time
        float timeRatio = settleTime / expectedSettleTime;
        
        if (timeRatio > 1.2f) {
            // Too slow - decrease tau
            float gain = engineConfiguration->wwAquinoTauSlowGain;
            float correctionFactor = (timeRatio - 1.0f) * gain;
            m_state.calculatedTauCorrection = 1.0f - correctionFactor;
        } else if (timeRatio < 0.8f) {
            // Too fast - increase tau slightly
            float gain = engineConfiguration->wwAquinoTauFastGain;
            float correctionFactor = (1.0f - timeRatio) * gain;
            m_state.calculatedTauCorrection = 1.0f + correctionFactor;
        } else {
            // Within acceptable range - no correction
            m_state.calculatedTauCorrection = 1.0f;
        }
    }
    
    // Clamp correction to reasonable bounds
    m_state.calculatedTauCorrection = clampF(0.7f, m_state.calculatedTauCorrection, 1.5f);
    
    m_state.state = AWWTransientState::APPLYING_CORRECTIONS;
}

void AdaptiveWallWettingController::resetState() {
    m_state = adaptive_wall_wetting_state_s{}; // Reset to default values
    m_bufferIndex = 0;
}

void AdaptiveWallWettingController::enterCooldown() {
    m_state.state = AWWTransientState::COOLDOWN;
    m_cooldownTimer.reset();
} 