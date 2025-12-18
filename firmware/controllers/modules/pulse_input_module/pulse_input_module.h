#pragma once

#include "engine_module.h"
#include "pulse_input.h"

class PulseInputModule final : public EngineModule {
public:
    void onConfigurationChange(engine_configuration_s* config) override;
    void onFastCallback() override;

private:
    PulseInput pulse;
    bool enabled = false;
};