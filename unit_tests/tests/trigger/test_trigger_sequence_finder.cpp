#include "pch.h"

TEST(trigger, finder) {
    trigger_config_s config;
    config.type = trigger_type_e::TT_VVT_BOSCH_QUICK_START;

    TriggerWaveform form;
    form.initializeTriggerWaveform(FOUR_STROKE_CAM_SENSOR, config, /*isCrank*/false);

}
