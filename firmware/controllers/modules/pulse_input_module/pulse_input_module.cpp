#include "pulse_input_module.h"
#include "../../core/engine_module.h"
#include "pulse_input.h"

void PulseInputModule::onConfigurationChange(engine_configuration_s const * /*previousConfig*/) {
    // Configure pin + EXTI based on current TS selection
    pulseInputInit();
}

void PulseInputModule::onFastCallback() {
    pulseInputUpdate();
}