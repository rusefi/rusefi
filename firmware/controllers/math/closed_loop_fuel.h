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
	public:
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