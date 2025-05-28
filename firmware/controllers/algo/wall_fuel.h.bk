/*
 * @file wall_fuel.h
 *
 */

#pragma once

#include "wall_fuel_state_generated.h"
#include "engine_module.h"
#include <rusefi/timer.h>
#include "rusefi_types.h"
#include "cyclic_buffer.h"

/**
 * Wall wetting, also known as fuel film
 * See https://github.com/rusefi/rusefi/issues/151 for the theory
 * 
 * IMPLEMENTAÇÃO BASEADA EM:
 * - SAE 810494 (Aquino) - Modelo físico correto
 * - "On-Line Adaptation of Wall-Wetting Model Parameters" (2001) - RLS Eqs. 10-22
 * 
 * PRINCIPAIS CORREÇÕES SEGUINDO O ARTIGO:
 * 1. RLS baseado em regressor físico real do modelo Aquino
 * 2. Sincronização com atrasos de transporte/combustão  
 * 3. Detecção focada em excursões de A/F (lambda error)
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

// *** ESTRUTURAS FÍSICAS CORRIGIDAS BASEADAS NO ARTIGO ***

// Condições exatas de injeção para sincronização física
struct InjectionConditions {
	float rpm;              // RPM no momento da injeção
	float map;              // MAP no momento da injeção  
	float clt;              // CLT no momento da injeção
	float injectedMass;     // Massa efetivamente injetada
	float wallFuelMass;     // Massa do filme no momento da injeção
	uint32_t timestamp;     // Timestamp da injeção
	uint32_t observationTime; // Quando esta injeção será observada no lambda
	int cylinderIndex;      // Índice do cilindro (0-based) para múltiplos cilindros
	bool valid;             // Dados válidos
	
	InjectionConditions() : rpm(0), map(0), clt(0), injectedMass(0), wallFuelMass(0), 
						   timestamp(0), observationTime(0), cylinderIndex(0), valid(false) {}
};

// *** RLS FÍSICO BASEADO NO MODELO AQUINO (Eqs. 10-22) ***
class PhysicalRLSAdapter {
private:
	static constexpr int PARAM_COUNT = 2;    // beta_correction, tau_correction
	static constexpr float DEFAULT_FORGETTING = 0.98f;
	
	float theta[PARAM_COUNT];                 // Parâmetros estimados [beta_corr, tau_corr]
	float P[PARAM_COUNT][PARAM_COUNT];        // Matriz de covariância  
	float forgettingFactor;
	bool initialized;
	int sampleCount;
	float errorSum;
	
	// Função auxiliar para calcular tau (implementar no .cpp)
	float computeTau(float rpm, float map, float clt) const;
	
public:
	PhysicalRLSAdapter() : forgettingFactor(DEFAULT_FORGETTING), initialized(false), 
						   sampleCount(0), errorSum(0.0f) {
		reset();
	}
	
	void reset() {
		// Inicialização conservadora
		theta[0] = 1.0f;  // beta_correction = 1.0 (neutro)
		theta[1] = 1.0f;  // tau_correction = 1.0 (neutro)
		
		// Matriz de covariância inicial  
		for (int i = 0; i < PARAM_COUNT; i++) {
			for (int j = 0; j < PARAM_COUNT; j++) {
				P[i][j] = (i == j) ? 10.0f : 0.0f;  // Diagonal dominante
			}
		}
		initialized = true;
		sampleCount = 0;
		errorSum = 0.0f;
	}
	
	void update(float lambdaError, const InjectionConditions& injection, 
				float currentRpm, float currentMap);
	
	float getBetaCorrection() const { return initialized ? theta[0] : 1.0f; }
	float getTauCorrection() const { return initialized ? theta[1] : 1.0f; }
	bool isInitialized() const { return initialized; }
	int getSampleCount() const { return sampleCount; }
	float getAverageError() const { return sampleCount > 0 ? errorSum / sampleCount : 0.0f; }
	
	void setForgettingFactor(float factor) { 
		forgettingFactor = clampF(0.95f, factor, 0.999f); 
	}
};

// *** SISTEMA SINCRONIZADO COM ATRASOS DE TRANSPORTE ***
class SynchronizedWallWettingAdapter {
private:
	PhysicalRLSAdapter physicalRLS;
	cyclic_buffer<InjectionConditions, 100> injectionQueue;  // Fila de injeções aguardando observação
	
	// Thresholds baseados no artigo
	static constexpr float LAMBDA_ERROR_THRESHOLD = 0.10f;  // 2% - mais sensível para detectar excursões A/F
	static constexpr int MIN_SAMPLES_FOR_CORRECTION = 10;   // Mínimo para convergência  
	
	// Função auxiliar para obter massa atual do filme
	float getCurrentWallFuelMass() const;
	
	// Validação de condições estáveis
	bool areConditionsStable(const InjectionConditions& injection) const;
	
public:
	SynchronizedWallWettingAdapter() {}
	
	// Interface principal seguindo o artigo
	void onFuelInjection(float rpm, float map, float clt, float injectedMass);
	void onLambdaObservation(float lambdaError, float currentRpm, float currentMap);
	
	// Aplicação física das correções
	bool shouldApplyCorrection(const InjectionConditions& injection);
	void applyPhysicalCorrection(const InjectionConditions& injection);
	
	// *** SUAVIZAÇÃO PARA EVITAR BURACOS NO MAPA ***
	void applySmoothingCorrection(int centerMapIdx, int centerRpmIdx, 
								  float betaCorrection, float tauCorrection);
	
	// Reset para novo ciclo
	void reset() {
		physicalRLS.reset();
		injectionQueue.clear();
	}
	
	// Estado
	bool isActive() const { return injectionQueue.getCount() > 0; }
	int getSampleCount() const { return physicalRLS.getSampleCount(); }
	float getAverageError() const { return physicalRLS.getAverageError(); }
};

// *** CONTROLADOR PRINCIPAL CORRIGIDO ***
class WallFuelController : public IWallFuelController, public EngineModule {
public:
	using interface_t = IWallFuelController;

	WallFuelController();

	void onFastCallback() override;
	void onIgnitionStateChanged(bool ignitionOn) override;
	
	// Interface básica - SEM ALTERAÇÕES
	bool getEnable() const override { return m_enable; }
	float getAlpha() const override { return m_alpha; }
	float getBeta() const override { return m_beta; }
	
	// Diagnóstico para adaptação
	bool isAdaptationActive() const { return synchronizedAdapter.isActive(); }
	int getCurrentSampleCount() const { return synchronizedAdapter.getSampleCount(); }
	float getCurrentAverageError() const { return synchronizedAdapter.getAverageError(); }
	
	// *** NOVA FUNÇÃO PARA INTEGRAÇÃO COM SISTEMA DE INJEÇÃO ***
	void onActualFuelInjection(float injectedMass, int cylinderIndex = 0);

protected:
	bool m_enable;
	float m_alpha;
	float m_beta;
	
	// *** NOVO SISTEMA SINCRONIZADO ***
	SynchronizedWallWettingAdapter synchronizedAdapter;
	
	// Funções auxiliares - mantidas para compatibilidade
	float computeTau() const;
	float computeBeta() const;
	
	// Nova função necessária para integração
	float getLastInjectedMass() const;  // Implementar no .cpp
	
private:
	// *** ESTADO DE IGNIÇÃO RESTAURADO ***
	bool ignitionState;
	Timer ignitionOffTimer;
};

// *** CONFIGURAÇÃO MÍNIMA - APENAS 3 PARÂMETROS ***
// Todos os outros parâmetros são calculados automaticamente ou fixos
// engineConfiguration->wwAdaptiveLearningEnabled: bool
// engineConfiguration->wwRLSForgettingFactor: float (0.95-0.999, default 0.98)
// engineConfiguration->wwTransientSensitivity: float (0.5-2.0, default 1.0)

#define WW_CORRECTION_RPM_BINS 8
#define WW_CORRECTION_MAP_BINS 8
