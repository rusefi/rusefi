#pragma once

#include "flash_main.h"
#include "table_helper.h"

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
	bool ltftTableHelperInit = 0;
	uint32_t lastLtftUpdateTime = 0;
	float ltftResult = 1;
	bool updatedLtft = 0;
	// Novas variáveis para aprimoramentos
	float stftEma = 1.0f;
	uint32_t ignitionOnTimestamp = 0;
	uint32_t ignitionOffTimestamp = 0;
	bool ignitionOnDelayActive = false;
	bool ignitionOffSaveDelayActive = false;
	bool pendingSave = false;
	// Para detecção regional
	float regionalErrorBuffer[16]; // Exemplo: buffer para erros por faixa de RPM
	int regionalErrorCount[16];
	// Para suavização automática
	void smoothHoles();
	void applyRegionalCorrection(float load, float rpm, float correction);
	void updateTimers(bool ignitionState);
	bool canLearn();
	float filterStft(float stftRaw);
public:
	LongTermFuelTrim();
	float getLtft(float load, float rpm);
	void resetLtftTimer();
	void updateLtft(float load, float rpm);
	void onIgnitionStateChanged(bool ignitionState) override {
		if(!ignitionState && updatedLtft) {
			copyTable(config->ltftTable, ltftTableHelper);
			setNeedToWriteConfiguration();
			updatedLtft = 0;
		}
	}
};