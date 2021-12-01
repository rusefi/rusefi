#pragma once

#include "ac_control_generated.h"

class AcController final : public ac_control_s, public EngineModule {
public:
	void onSlowCallback() override;

	bool isAcEnabled() const;

private:
	bool getAcState();

	bool m_acEnabled = false;
};
