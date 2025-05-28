/*
 * @file wall_fuel.h
 *
 */

#pragma once

#include "wall_fuel_state_generated.h"
#include "engine_module.h"
#include <rusefi/timer.h>
#include "cyclic_buffer.h"

/**
 * Wall wetting, also known as fuel film
 * See https://github.com/rusefi/rusefi/issues/151 for the theory
 */
class WallFuel : public wall_fuel_state_s {
public:
	/**
	 * @param desiredMassGrams desired fuel quantity, in grams
	 * @return total adjusted fuel squirt mass in grams once wall wetting is taken into effect
	 */
	float adjust(float desiredMassGrams);
	float getWallFuel() const;
	void resetWF();
	int invocationCounter = 0;
};

struct IWallFuelController {
	virtual bool getEnable() const = 0;
	virtual float getAlpha() const = 0;
	virtual float getBeta() const = 0;
	virtual void onActualFuelInjection(float injectedMass, int cylinderIndex = 0) = 0;
};

// Circular buffer for load derivative calculation
#define WW_LOAD_BUFFER_SIZE 8
#define WW_IMMEDIATE_BUFFER_SIZE 40  // Beta: primeiros 200ms (40 amostras a 200Hz)
#define WW_PROLONGED_BUFFER_SIZE_MAX 1000 // Tau: máximo para tau=5s (1000 amostras a 200Hz)
#define WW_TAU_MULTIPLIER 3.0f       // Coleta dados por 3×tau (captura ~95% do efeito)

// Adaptive correction data structure
struct WwAdaptiveData {
	float loadBuffer[WW_LOAD_BUFFER_SIZE];
	int bufferIndex = 0;
	float lastLoad = 0;
	float loadDerivative = 0;
	
	// Transient detection and timing
	bool isPositiveTransient = false;
	bool isNegativeTransient = false;
	float transientMagnitude = 0;
	float transientStartTime = 0;
	bool waitingForResponse = false;
	
	// Separate buffers for beta (immediate) and tau (prolonged) responses
	float immediateLambdaBuffer[WW_IMMEDIATE_BUFFER_SIZE];  // Beta: 0-200ms
	float prolongedLambdaBuffer[WW_PROLONGED_BUFFER_SIZE_MAX];  // Tau: 200ms-3s
	int immediateBufferIndex = 0;
	int prolongedBufferIndex = 0;
	int immediateBufferCount = 0;
	int prolongedBufferCount = 0;
	
	// Response phase tracking
	bool collectingImmediate = false;  // 0-200ms for beta
	bool collectingProlonged = false;  // 200ms-3s for tau
	float phaseStartTime = 0;
	
	// Dynamic prolonged phase duration based on tau
	float currentTau = 1.0f;           // Current tau value for this transient
	float prolongedPhaseDuration = 0;  // Duration in seconds (WW_TAU_MULTIPLIER × tau)
	int prolongedBufferSizeTarget = 0; // Target buffer size for current tau
	
	// Average errors for correction calculation
	float avgImmediateLambdaError = 0;  // For beta correction
	float avgProlongedLambdaError = 0;  // For tau correction
	
	// Transient conditions for correction
	float transientRpm = 0;
	float transientMap = 0;
	bool hasValidTransientData = false;
	
	// Separate conditions for beta (initial) and tau (final) corrections
	float initialTransientRpm = 0;  // Beta: condições no início do transiente
	float initialTransientMap = 0;
	float finalTransientRpm = 0;    // Tau: condições no final do transiente  
	float finalTransientMap = 0;
	
	// Transient completion tracking
	bool transientCompleted = false;
	bool incompleteTransientDetected = false;
	float transientDuration = 0;
	float minTransientDuration = 0.5f; // 500ms minimum for complete transient
	float incompleteTimeout = 5.0f;    // 5s timeout for incomplete transients (increased from 3.0f)
	
	// Decoupled adaptation periods to avoid beta-tau coupling
	enum AdaptationMode {
		ADAPT_BETA_ONLY,    // Adapt only beta, keep tau fixed
		ADAPT_TAU_ONLY,     // Adapt only tau, keep beta fixed
		ADAPT_BOTH          // Adapt both (for comparison/testing)
	};
	
	AdaptationMode currentAdaptationMode = ADAPT_BETA_ONLY;
	int transientCounter = 0;
	int adaptationCycleLength = 2;   // 2 transients per adaptation period (initialized)
	int betaAdaptationCycles = 3;     // 3 cycles for beta adaptation (6 transients total)
	int tauAdaptationCycles = 3;      // 3 cycles for tau adaptation (6 transients total)
	int currentCycleCount = 0;
	
	// Statistics for debugging
	int interruptedBetaPhases = 0;   // Count of beta phases interrupted by new transients
	int interruptedTauPhases = 0;    // Count of tau phases interrupted by new transients
	int completedLearningCycles = 0; // Count of successfully completed learning cycles
	
	void reset() {
		// Reset all learning state
		isPositiveTransient = false;
		isNegativeTransient = false;
		collectingImmediate = false;
		collectingProlonged = false;
		transientCompleted = false;
		incompleteTransientDetected = false;
		
		// Reset buffers
		immediateBufferCount = 0;
		prolongedBufferCount = 0;
		immediateBufferIndex = 0;
		prolongedBufferIndex = 0;
		
		// Reset timing
		transientStartTime = 0;
		phaseStartTime = 0;
		transientDuration = 0;
		
		// Reset conditions
		initialTransientRpm = 0;
		initialTransientMap = 0;
		finalTransientRpm = 0;
		finalTransientMap = 0;
		
		// Reset errors
		avgImmediateLambdaError = 0;
		avgProlongedLambdaError = 0;
		
		// Note: Don't reset adaptation mode variables here
		// They should persist across individual transient resets
		// Only reset on ignition cycle or manual reset
		
		// Note: Don't reset statistics counters (interruptedBetaPhases, etc.)
		// They should persist to provide debugging information across multiple cycles
	}
	
	void resetAdaptationCycle() {
		// Reset adaptation cycle (called on ignition or manual reset)
		currentAdaptationMode = ADAPT_BETA_ONLY;
		transientCounter = 0;
		currentCycleCount = 0;
	}
};

class WallFuelController : public IWallFuelController, public EngineModule {
public:
	using interface_t = IWallFuelController;

	void onFastCallback() override;
	void onSlowCallback() override;
	void onIgnitionStateChanged(bool ignitionOn) override;

	bool getEnable() const override {
		return m_enable;
	}

	float getAlpha() const override {
		return m_alpha;
	}

	float getBeta() const override {
		return m_beta;
	}

protected:
	float computeTau() const;
	float computeBeta() const;

private:
	bool m_enable = false;
	float m_alpha = 0;
	float m_beta = 0;
	
	// Adaptive learning system
	WwAdaptiveData m_adaptiveData;
	Timer m_learningTimer;
	Timer m_ignitionOffTimer;
	bool m_ignitionState = false;
	bool m_pendingSave = false;
	
	// Adaptive learning methods
	void updateLoadDerivative(float currentLoad);
	void detectTransients();
	void updateLambdaResponse(float lambdaError, float currentTime);
	void startImmediatePhase();
	void startProlongedPhase();
	void applyAdaptiveCorrections();
	void applyIncompleteTransientCorrection();
	void applyCorrectionToTable(float betaCorrection, float tauCorrection, float rpm, float map);
	void smoothCorrectionTable(int mapIdx, int rpmIdx, float betaCorrection, float tauCorrection);

	// Separate correction calculations for beta and tau
	float calculateBetaCorrection(float avgImmediateLambdaError);
	float calculateTauCorrection();
	
	// Decoupled adaptation management
	void updateAdaptationMode();
	bool shouldAdaptBeta() const;
	bool shouldAdaptTau() const;
	
	// Integration with injection system
	void onActualFuelInjection(float injectedMass, int cylinderIndex = 0) override;
};