#pragma once

#include "flash_main.h"
#include "table_helper.h"
#include "engine_module.h"
#include <rusefi/timer.h>

struct stft_s;

struct ClosedLoopFuelResult {
	ClosedLoopFuelResult() {
		// Default is no correction, aka 1.0 multiplier
		for (size_t i = 0; i < STFT_BANK_COUNT; i++) {
			banks[i] = 1.0f;
		}
	}

	float banks[STFT_BANK_COUNT];
};

ClosedLoopFuelResult fuelStftClosedLoopCorrection();
size_t computeStftBin(float rpm, float load, stft_s& cfg);
bool shouldUpdateCorrection(SensorType sensor);

class LongTermFuelTrim : public EngineModule{
	float ltftTableHelper[16][16];
	bool ltftTableHelperInit = false;
	float ltftResult = 1;
	bool updatedLtft = false;
	// Novas variáveis para aprimoramentos
	float stftEma = 1.0f;
	bool m_ignitionState = false;
	// Para detecção regional
	float regionalErrorBuffer[16]; // Exemplo: buffer para erros por faixa de RPM
	int regionalErrorCount[16];
	// Para suavização automática
	void smoothHoles();
	void applyRegionalCorrection(float load, float rpm, float correction);
	bool canLearn();
	float filterStft(float stftRaw);
	
	// Timer-based implementation
	Timer m_updateTimer;            // Controls update frequency
	Timer m_ignitionOnTimer;        // Tracks time since ignition on
	Timer m_ignitionOffTimer;       // Tracks time since ignition off
	bool isLearnConditionsMet = false;
public:
	LongTermFuelTrim();
	float getLtft(float load, float rpm);
	void resetLtftTimer();
	void updateLtft(float load, float rpm);
	void onIgnitionStateChanged(bool ignitionState) override;
};