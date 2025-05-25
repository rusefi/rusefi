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

// Adaptação automática de tau/beta
#define WW_RPM_BINS 8
#define WW_MAP_BINS 8

#ifndef WWAE_RPM_SIZE
#define WWAE_RPM_SIZE 8
#endif

#ifndef WWAE_TABLE_SIZE
#define WWAE_TABLE_SIZE 8
#endif

// Estrutura para tracking de confiança e qualidade por célula
struct CellLearningStatus {
	float confidence;        // 0.0 a 1.0 - confiança na calibração
	int sampleCount;        // número de aprendizados realizados
	float variance;         // variância dos últimos ajustes
	float lastAdjustment;   // último ajuste aplicado
	bool isConverged;       // se a célula convergiu
	int consecutiveAdjustments; // ajustes consecutivos sem estabilização
	
	CellLearningStatus() : confidence(0.0f), sampleCount(0), variance(0.0f), 
						  lastAdjustment(0.0f), isConverged(false), consecutiveAdjustments(0) {}
};

// Estrutura para validação de qualidade de dados
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
	bool getBetaAdjusted(int i, int j) const { return i < WW_MAP_BINS && j < WW_RPM_BINS ? betaLearningStatus[i][j].isConverged : false; }
	bool getTauAdjusted(int i, int j) const { return i < WW_MAP_BINS && j < WW_RPM_BINS ? tauLearningStatus[i][j].isConverged : false; }
	
	// Novos métodos para diagnóstico avançado
	float getCellConfidence(int i, int j, bool isBeta) const;
	int getCellSampleCount(int i, int j, bool isBeta) const;
	float getCellVariance(int i, int j, bool isBeta) const;

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
	
	// Novos métodos para melhorias
	LearningDataQuality validateLearningData(float lambda, float targetLambda, float clt, float map, float rpm);
	bool isLearningDataValid(const LearningDataQuality& quality);
	void updateCellConfidence(int i, int j, bool isBeta, float adjustment, const LearningDataQuality& quality);
	void checkAndResetDrift();
	void resetCellsWithHighVariance();
	int calculateOptimalBufferSize(float tau, float rpm);
	float calculateWeightedAverage(int startIdx, int endIdx, float targetLambda);
	void smoothCorrectionTable(scaled_channel<uint8_t, 100, 1> table[WW_RPM_BINS][WW_MAP_BINS], int centerI, int centerJ, float intensity);

private:
	bool m_enable = false;
	float m_alpha = 0;
	float m_beta = 0;
	bool m_ignitionState = false;
	
	Timer m_transientTimer;
	Timer m_ignitionOffTimer;
	
	// Buffers movidos para membros da classe (thread safety)
	static constexpr int WW_BUFFER_MAX = 1000;
	float lambdaBuffer[WW_BUFFER_MAX];
	float rpmBuffer[WW_BUFFER_MAX];
	float mapBuffer[WW_BUFFER_MAX];
	int bufferIdx;
	int bufferMaxSize;
	bool monitoring;
	bool pendingWwSave;
	
	// Tracking de confiança por célula
	CellLearningStatus betaLearningStatus[WW_MAP_BINS][WW_RPM_BINS];
	CellLearningStatus tauLearningStatus[WW_MAP_BINS][WW_RPM_BINS];
	
	// Estado do transiente atual
	TransientDirection currentTransientDirection;
	TransientDirection lastTransientDirection;
	
	// Diagnósticos
	float lastImmediateError;
	float lastProlongedError;
	
	// Contadores para reset de drift
	int totalAdjustmentCycles;
	Timer lastResetTimer;
	
	// Constantes básicas para validação (outras vêm da configuração)
	static constexpr float MIN_LAMBDA = 0.5f;
	static constexpr float MAX_LAMBDA = 1.5f;
};

// Template method implementation moved to .cpp file
