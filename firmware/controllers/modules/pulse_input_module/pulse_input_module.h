#pragma once

#include "controllers/core/engine_module.h"

class PulseInputModule : public EngineModule {
public:
    void onConfigurationChange(engine_configuration_s const * previousConfig) override;
    void onFastCallback() override;
};