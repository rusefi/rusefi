#include "pulse_input_module.h"
#include "engine_configuration.h"
#include "digital_input_exti.h"

static PulseInputModule pulseInputModule;

void PulseInputModule::onConfigurationChange(engine_configuration_s* config) {
    enabled = false;

    // User must select a pin in TS
    if (config->pulseInputPin == GPIO_UNASSIGNED) {
        return;
    }

    // Attach interrupt
    enableDigitalInputExti(
        config->pulseInputPin,
        [&]() {
            pulse.onEdge(getTimeNowUs());
        },
        PAL_EVENT_MODE_BOTH_EDGES
    );

    pulse.setAveragingWindowMs(config->pulseInputAveragingMs);

    enabled = true;
}

void PulseInputModule::onFastCallback() {
    if (!enabled) {
        return;
    }

    pulse.update(getTimeNowUs());
}