#pragma once

#include "wot_enrichment_state_generated.h"
#include <rusefi/timer.h>

class WotEnrichment : public wot_enrichment_state_s, public EngineModule {
public:
	using interface_t = WotEnrichment;
	void setDefaultConfiguration() override;
	void onSlowCallback() override;
	void onEngineStop() override;
	void onConfigurationChange(engine_configuration_s const* previousConfig) override;
	float getAfrAdder() const;

private:
	void reset();
	Timer m_wotTimer;
};
