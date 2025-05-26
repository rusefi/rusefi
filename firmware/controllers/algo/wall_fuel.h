/*
 * @file wall_fuel.h
 *
 */

#pragma once

#include "wall_fuel_state_generated.h"
#include "engine_module.h"
#include <rusefi/timer.h>
#include "rusefi_types.h"

// Compile-time feature toggles for conditional functionality
#ifndef WW_ENABLE_ROBUST_VALIDATION
#define WW_ENABLE_ROBUST_VALIDATION 0  // Disabled by default to save memory
#endif

#ifndef WW_ENABLE_DRIFT_RESET
#define WW_ENABLE_DRIFT_RESET 0       // Disabled by default to save memory
#endif

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
};

// Adicionando enumeração para direção do transiente
enum class TransientDirection {
	NONE,
	POSITIVE,  // Aceleração
	NEGATIVE   // Desaceleração
};

// Enhanced transient detection types
enum class TransientIntensity {
	NONE,
	LIGHT,     // Gentle transients
	NORMAL,    // Standard transients  
	HEAVY      // Aggressive transients
};

struct TransientInfo {
	TransientDirection direction;
	TransientIntensity intensity;
	float tpsRate;      // %/s
	float mapRate;      // kPa/s
	float duration;     // ms
	bool isValid;
	
	TransientInfo() : direction(TransientDirection::NONE), intensity(TransientIntensity::NONE), 
					  tpsRate(0), mapRate(0), duration(0), isValid(false) {}
};

// Wall wetting adaptive correction tables
#define WW_CORRECTION_RPM_BINS 8
#define WW_CORRECTION_MAP_BINS 8

#ifndef WWAE_RPM_SIZE
#define WWAE_RPM_SIZE 8
#endif

#ifndef WWAE_TABLE_SIZE
#define WWAE_TABLE_SIZE 6
#endif

#ifndef WWAE_CORRECTION_SIZE
#define WWAE_CORRECTION_SIZE 8
#endif

// Simplified lightweight learning status for memory optimization
struct SimpleLearningStatus {
	uint8_t confidence;      // 0-255 scaled confidence (saves 3 bytes vs float)
	uint8_t sampleCount;     // max 255 samples (saves 3 bytes vs int)
	bool isConverged;        // convergence flag
	
	SimpleLearningStatus() : confidence(0), sampleCount(0), isConverged(false) {}
};

// Conditional data quality structure - only used when robust validation is enabled
struct LearningDataQuality {
	bool lambdaValid;
	bool conditionsStable;
	bool tempAppropriate;
	bool loadAppropriate;
	float qualityScore;  // 0.0-1.0
	
	LearningDataQuality() : lambdaValid(false), conditionsStable(false), 
						   tempAppropriate(false), loadAppropriate(false), qualityScore(0.0f) {}
};

class WallFuelController : public IWallFuelController, public EngineModule {
public:
	using interface_t = IWallFuelController;

	WallFuelController();

	void onFastCallback() override;
	void adaptiveLearning(float rpm, float map, float lambda, float targetLambda, 
						 bool isTransient, TransientDirection direction, float clt);
	
	// Diagnóstico
	float getLastImmediateError() const { return lastImmediateError; }
	float getLastProlongedError() const { return lastProlongedError; }
	TransientDirection getLastTransientDirection() const { return lastTransientDirection; }
	bool getBetaAdjusted(int i, int j) const { return i < WW_CORRECTION_MAP_BINS && j < WW_CORRECTION_RPM_BINS ? betaLearningStatus[i][j].isConverged : false; }
	bool getTauAdjusted(int i, int j) const { return i < WW_CORRECTION_MAP_BINS && j < WW_CORRECTION_RPM_BINS ? tauLearningStatus[i][j].isConverged : false; }
	
	// Simplified diagnostic methods for memory optimization
	float getCellConfidence(int i, int j, bool isBeta) const;
	int getCellSampleCount(int i, int j, bool isBeta) const;

	bool getEnable() const override {
		return m_enable;
	}

	float getAlpha() const override {
		return m_alpha;
	}

	float getBeta() const override {
		return m_beta;
	}

	void onIgnitionStateChanged(bool ignitionOn) override;

protected:
	float computeTau() const;
	float computeBeta() const;
	float computeTauWithDirection(TransientDirection direction) const;
	float computeBetaWithDirection(TransientDirection direction) const;
	
	// Conditional methods - only compiled if robust validation is enabled
	#if WW_ENABLE_ROBUST_VALIDATION
	LearningDataQuality validateLearningData(float lambda, float targetLambda, float clt, float map, float rpm);
	bool isLearningDataValid(const LearningDataQuality& quality);
	void updateCellConfidence(int i, int j, bool isBeta, float adjustment, const LearningDataQuality& quality);
	void checkAndResetDrift();
	void resetCellsWithHighVariance();
	#endif
	
	int calculateOptimalBufferSize(float tau, float rpm);
	float calculateWeightedAverage(int startIdx, int endIdx, float targetLambda);
	void smoothCorrectionTable(scaled_channel<uint8_t, 100, 1> table[WW_CORRECTION_MAP_BINS][WW_CORRECTION_RPM_BINS], int centerI, int centerJ, float intensity);
	
	// Enhanced transient detection methods
	TransientInfo detectTransientEnhanced(float tps, float map, float rpm);
	bool isTransientValid(const TransientInfo& transient);
	TransientIntensity classifyTransientIntensity(float tpsRate, float mapRate);
	bool applyTransientFiltering(float rate);
	void updateTransientDuration();

private:
	bool m_enable = false;
	float m_alpha = 0;
	float m_beta = 0;
	bool m_ignitionState = false;
	
	Timer m_transientTimer;
	Timer m_ignitionOffTimer;
	
	// Enhanced transient detection
	TransientInfo m_currentTransient;
	Timer m_transientDurationTimer;
	float m_transientFilterBuffer[10];  // For filtering false detections
	int m_filterBufferIdx = 0;
	bool m_filterBufferFilled = false;
	
	// Dramatically reduced buffer sizes for memory optimization
	static constexpr int WW_BUFFER_MAX = 200;  // Reduced from 1000 to 200 (-9.6KB)
	float lambdaBuffer[WW_BUFFER_MAX];
	float rpmBuffer[WW_BUFFER_MAX];
	float mapBuffer[WW_BUFFER_MAX];
	int bufferIdx;
	int bufferMaxSize;
	bool monitoring;
	bool pendingWwSave;
	
	// Learning status for correction tables
	SimpleLearningStatus betaLearningStatus[WW_CORRECTION_MAP_BINS][WW_CORRECTION_RPM_BINS];
	SimpleLearningStatus tauLearningStatus[WW_CORRECTION_MAP_BINS][WW_CORRECTION_RPM_BINS];
	
	// Estado do transiente atual
	TransientDirection currentTransientDirection;
	TransientDirection lastTransientDirection;
	
	// Simplified diagnostics
	float lastImmediateError;
	float lastProlongedError;
	
	// Optional advanced features - only included if needed
	#if WW_ENABLE_DRIFT_RESET
	int totalAdjustmentCycles;
	Timer lastResetTimer;
	#endif
	
	// Constantes básicas para validação (outras vêm da configuração)
	static constexpr float MIN_LAMBDA = 0.5f;
	static constexpr float MAX_LAMBDA = 1.5f;
};

// Template method implementation moved to .cpp file
