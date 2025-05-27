/*
 * @file wall_fuel.h
 *
 */

#pragma once

#include "wall_fuel_state_generated.h"
#include "engine_module.h"
#include <rusefi/timer.h>
#include "rusefi_types.h"

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

// *** BUFFER CIRCULAR PARA ANÁLISE DE DERIVADAS ***
// 150ms @ 5ms callback = 30 amostras
#define TRANSIENT_DETECTION_BUFFER_SIZE 30

struct TransientDetectionBuffer {
	float tpsValues[TRANSIENT_DETECTION_BUFFER_SIZE];
	float mapValues[TRANSIENT_DETECTION_BUFFER_SIZE];
	int currentIndex;
	bool bufferFilled;
	
	TransientDetectionBuffer() : currentIndex(0), bufferFilled(false) {
		for (int i = 0; i < TRANSIENT_DETECTION_BUFFER_SIZE; i++) {
			tpsValues[i] = 0.0f;
			mapValues[i] = 0.0f;
		}
	}
	
	void addSample(float tps, float map) {
		tpsValues[currentIndex] = tps;
		mapValues[currentIndex] = map;
		currentIndex = (currentIndex + 1) % TRANSIENT_DETECTION_BUFFER_SIZE;
		if (currentIndex == 0) {
			bufferFilled = true;
		}
	}
	
	bool hasEnoughSamples() const {
		return bufferFilled || currentIndex >= 10; // Mínimo 10 amostras (50ms)
	}
	
	// Calcula a derivada média nos últimos N samples
	float calculateTpsDerivative(int samples = 10) const {
		if (!hasEnoughSamples() || samples < 2) return 0.0f;
		
		samples = minI(samples, bufferFilled ? TRANSIENT_DETECTION_BUFFER_SIZE : currentIndex);
		
		int startIdx = (currentIndex - samples + TRANSIENT_DETECTION_BUFFER_SIZE) % TRANSIENT_DETECTION_BUFFER_SIZE;
		int endIdx = (currentIndex - 1 + TRANSIENT_DETECTION_BUFFER_SIZE) % TRANSIENT_DETECTION_BUFFER_SIZE;
		
		float startValue = tpsValues[startIdx];
		float endValue = tpsValues[endIdx];
		
		return (endValue - startValue) / samples; // Delta por amostra
	}
	
	float calculateMapDerivative(int samples = 10) const {
		if (!hasEnoughSamples() || samples < 2) return 0.0f;
		
		samples = minI(samples, bufferFilled ? TRANSIENT_DETECTION_BUFFER_SIZE : currentIndex);
		
		int startIdx = (currentIndex - samples + TRANSIENT_DETECTION_BUFFER_SIZE) % TRANSIENT_DETECTION_BUFFER_SIZE;
		int endIdx = (currentIndex - 1 + TRANSIENT_DETECTION_BUFFER_SIZE) % TRANSIENT_DETECTION_BUFFER_SIZE;
		
		float startValue = mapValues[startIdx];
		float endValue = mapValues[endIdx];
		
		return (endValue - startValue) / samples; // Delta por amostra
	}
	
	// *** NOVO: Método para obter amostra MAP de N posições atrás ***
	float getMapSample(int samplesBack) const {
		if (!hasEnoughSamples() || samplesBack < 0) return 0.0f;
		
		int maxSamples = bufferFilled ? TRANSIENT_DETECTION_BUFFER_SIZE : currentIndex;
		if (samplesBack >= maxSamples) samplesBack = maxSamples - 1;
		
		int targetIdx = (currentIndex - samplesBack - 1 + TRANSIENT_DETECTION_BUFFER_SIZE) % TRANSIENT_DETECTION_BUFFER_SIZE;
		return mapValues[targetIdx];
	}
};

// Simplified lightweight learning status for memory optimization
struct SimpleLearningStatus {
	uint8_t confidence;      // 0-255 scaled confidence (saves 3 bytes vs float)
	uint8_t sampleCount;     // max 255 samples (saves 3 bytes vs int)
	bool isConverged;        // convergence flag
	
	SimpleLearningStatus() : confidence(0), sampleCount(0), isConverged(false) {}
};

class WallFuelController : public IWallFuelController, public EngineModule {
public:
	using interface_t = IWallFuelController;

	WallFuelController();

	void onFastCallback() override;
	void adaptiveLearning(float rpm, float map, float lambda, float targetLambda, 
						 bool isTransient, TransientDirection direction, float clt);
	
	// Diagnóstico simplificado
	float getLastImmediateError() const { return lastImmediateError; }
	float getLastProlongedError() const { return lastProlongedError; }
	TransientDirection getLastTransientDirection() const { return lastTransientDirection; }
	bool getBetaAdjusted(int i, int j) const { return i < WW_CORRECTION_MAP_BINS && j < WW_CORRECTION_RPM_BINS ? betaLearningStatus[i][j].isConverged : false; }
	bool getTauAdjusted(int i, int j) const { return i < WW_CORRECTION_MAP_BINS && j < WW_CORRECTION_RPM_BINS ? tauLearningStatus[i][j].isConverged : false; }

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
	
	int calculateOptimalBufferSize(float tau, float rpm);
	void smoothCorrectionTable(scaled_channel<uint8_t, 100, 1> table[WW_CORRECTION_MAP_BINS][WW_CORRECTION_RPM_BINS], int centerI, int centerJ, float intensity);
	
	// *** DETECÇÃO DE TRANSIENTE COM BUFFER CIRCULAR ***
	TransientInfo detectTransientWithBuffer(float tps, float map);

private:
	bool m_enable = false;
	float m_alpha = 0;
	float m_beta = 0;
	bool m_ignitionState = false;
	
	Timer m_ignitionOffTimer;
	
	// *** BUFFER CIRCULAR PARA DETECÇÃO DE TRANSIENTES ***
	TransientDetectionBuffer m_transientBuffer;
	Timer m_transientCooldownTimer; // Para evitar detecções muito frequentes
	Timer m_monitoringTimeoutTimer; // Para timeout do monitoring
	
	// Enhanced transient detection
	TransientInfo m_currentTransient;
	
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
	
	// *** ESTADO GLOBAL DE MONITORAMENTO ***
	bool globalMonitoring;           // Estado global de monitoramento ativo
	TransientDirection monitoringDirection; // Direção do transiente sendo monitorado
	
	// Simplified diagnostics
	float lastImmediateError;
	float lastProlongedError;
	
	// *** CORREÇÃO FUNDAMENTAL: Armazenar condições iniciais do transiente ***
	int transientInitialMapIdx;     // Índice MAP das condições iniciais (para correção de Beta)
	int transientInitialRpmIdx;     // Índice RPM das condições iniciais (para correção de Beta)
	
	// Constantes básicas para validação (outras vêm da configuração)
	static constexpr float MIN_LAMBDA = 0.5f;
	static constexpr float MAX_LAMBDA = 1.5f;
};

// Template method implementation moved to .cpp file
