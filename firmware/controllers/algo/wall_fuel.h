/*
 * @file wall_fuel.h
 *
 */

#pragma once

#include "wall_fuel_state_generated.h"
#include "engine_module.h"

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

// Adaptação automática de tau/beta
#define WW_RPM_BINS 8
#define WW_MAP_BINS 8

class WallFuelController : public IWallFuelController, public EngineModule {
public:
	using interface_t = IWallFuelController;

	WallFuelController();

	void onFastCallback() override;
	void adaptiveLearning(float rpm, float map, float lambda, float targetLambda, bool isTransient, float clt);
	// Diagnóstico
	float getLastImmediateError() const { return lastImmediateError; }
	float getLastProlongedError() const { return lastProlongedError; }
	bool getBetaAdjusted(int i, int j) const { return betaAdjusted[i][j]; }
	bool getTauAdjusted(int i, int j) const { return tauAdjusted[i][j]; }

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

private:
	bool m_enable = false;
	float m_alpha = 0;
	float m_beta = 0;
	// Buffers e flags para aprendizado
	float lastImmediateError = 0;
	float lastProlongedError = 0;
	bool betaAdjusted[WW_RPM_BINS][WW_MAP_BINS] = {{false}};
	bool tauAdjusted[WW_RPM_BINS][WW_MAP_BINS] = {{false}};
	uint32_t lastSaveTime = 0;
	bool pendingWwSave = false;
	void smoothCorrectionTable(float table[WW_RPM_BINS][WW_MAP_BINS], float intensity);
};
