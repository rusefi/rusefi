#pragma once

#include "engine_module.h"

class PulseInputModule : public EngineModule {
public:
    // Called once at engine initialization
    void Init() override;

    // Called periodically in fast loop
    void onFastCallback() override;
};