#include "pulse_input_module.h"
#include "pulse_input.h"

// Called once at engine initialization
void PulseInputModule::Init() {
    pulseInputInit();
}

// Called periodically in fast loop
void PulseInputModule::onFastCallback() {
    pulseInputUpdate();
}