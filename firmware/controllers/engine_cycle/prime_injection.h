/*
 * @file prime_injection.h

 */

#pragma once

#include "engine_module.h"
#include "rusefi_types.h"
#include "scheduler.h"
#include "prime_injection_generated.h"

class PrimeController : public EngineModule, public prime_injection_s {
public:
	void onIgnitionStateChanged(bool ignitionOn) override;
	void onSlowCallback() override;

	floatms_t getPrimeDuration() const;

	void onPrimeStart();
	void onPrimeEnd();

	bool isPriming() const {
		return m_isPriming;
	}

private:

	static void onPrimeStartAdapter(PrimeController* instance) {
		instance->onPrimeStart();
	}

	static void onPrimeEndAdapter(PrimeController* instance) {
		instance->onPrimeEnd();
	}

	uint32_t getKeyCycleCounter() const;
	void setKeyCycleCounter(uint32_t count);
};
